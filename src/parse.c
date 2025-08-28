#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/parse.h"

int output_file(int fd, struct dbheader_t *header) {
  if (fd < 0) {
    printf("Got bad fd from the user\n");
    return STATUS_ERROR;
  }

  if (header == NULL) {
    printf("cyce\n");
  }

  header->version = htons(header->version);
  header->count = htons(header->count);
  header->magic = htonl(header->magic);
  header->fileSize = htonl(header->fileSize);

  lseek(fd, 0, SEEK_SET);
  write(fd, header, sizeof(struct dbheader_t));

  return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  if (fd < 0) {
    printf("Got a bad FD from the user\n");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create db header\n");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct dbheader_t)) !=
      sizeof(struct dbheader_t)) {
    perror("read");
    free(header);
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->fileSize = ntohl(header->fileSize);

  if (header->version != 1) {
    printf("Improper header version\n");
    free(header);
    return STATUS_ERROR;
  }

  if (header->magic != HEADER_MAGIC) {
    printf("Improper header magic\n");
    free(header);
    return STATUS_ERROR;
  }

  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if (header->fileSize != dbstat.st_size) {
    printf("Improper file size\n");
    free(header);
    return STATUS_ERROR;
  }

  *headerOut = header;
  return STATUS_SUCCESS;
}

int create_db_header(struct dbheader_t **headerOut) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create the db header\n");
    return STATUS_ERROR;
  }

  header->magic = HEADER_MAGIC;
  header->version = 0x1;
  header->count = 0;
  header->fileSize = sizeof(struct dbheader_t);

  *headerOut = header;

  return STATUS_SUCCESS;
}
