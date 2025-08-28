#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/common.h"
#include "../include/file.h"
#include "../include/parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t -n   - create new database filen\n");
  printf("\t -f   - (required) path to database file\n");
  return;
}

int main(int argc, char *argv[]) {

  struct dbheader_t *dbhdr = NULL;
  int c;
  bool newFile = false;
  char *filePath = NULL;

  int dbFd = -1;

  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
    case 'n':
      newFile = true;
      break;
    case 'f':
      filePath = optarg;
      break;
    case '?':
      printf("Unknown option -%c\n", c);
      break;
    default:
      return -1;
    }
  }

  printf("\n");
  if (filePath == NULL) {
    printf("FilePath is a required argument\n");
    print_usage(argv);
  }

  if (newFile) {
    dbFd = create_db_file(filePath);
    if (dbFd == STATUS_ERROR) {
      printf("Unable to create db file\n");
      return -1;
    }

    if (create_db_header(dbFd, &dbhdr) == STATUS_ERROR) {
      printf("Failed to create db header");
      return -1;
    }
  } else {
    dbFd = open_db_file(filePath);
    if (dbFd == STATUS_ERROR) {
      printf("Unable to open file\n");
      return -1;
    }
    if (validate_db_header(dbFd, &dbhdr) == STATUS_ERROR) {
      printf("Validation failed\n");
      return -1;
    }
  }

  output_file(dbFd, dbhdr);
  return 0;
}
