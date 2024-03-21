#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define COMP_1_byte
//#define COMP_2_bytes

/* this function simulates a bug 
 * of NULL pointer dereference */
void bug(void) {
  char * p = NULL;
  *p = 0;
}

int compare(char * src, char * dst) {

#ifdef COMP_1_byte
  if (src[0] == dst[0])
    if (src[1] == dst[1])
      if (src[2] == dst[2])
        if (src[3] == dst[3])
          if (src[4] == dst[4])
            if (src[5] == dst[5])
              if (src[6] == dst[6])
                if (src[7] == dst[7])
                  return 0;
#endif

#ifdef COMP_2_bytes
  uint16_t * src2 = (uint16_t *) src;
  uint16_t * dst2 = (uint16_t *) dst;
  if (src2[0] == dst2[0])
    if (src2[1] == dst2[1])
      if (src2[2] == dst2[2])
        if (src2[3] == dst2[3])
          return 0;
#endif

  return 1;
}

void process(char * buf) {

  if (compare(buf, "01234567") == 0)
    bug();
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: ./simple input-file\n");
    return -1;
  }

  char * filename = argv[1];

  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "failed to open file %s\n", filename);
    return -1;
  }

  char buf[100];
  fread(buf, 100, 1, f);

  fclose(f);

  process(buf);

  fprintf(stderr, "so far so good\n");

  return 0;
}

