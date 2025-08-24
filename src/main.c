#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t -n   - create new database filen\n");
  printf("\t -f   - (required) path to database file\n");
  return;
}

int main(int argc, char *argv[]) {
  int c;
  bool newFile = false;
  char *filePath = NULL;

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

  if (filePath == NULL) {
    printf("FilePath is a required argument\n");
    print_usage(argv);
  }
  printf("NewFile: %d\n", newFile);
  printf("FilePath: %s\n", filePath);

  return 0;
}
