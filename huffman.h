#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *s;
    ssize_t length;
} String;

// frequencies is a 256-number array with a number for each character
String generate_huffman_tree(ssize_t *frequencies);
