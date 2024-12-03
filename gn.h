#ifndef GN_H
#define GN_H

#include <stdint.h>
#include <stdio.h>

#define BLOC                     uint32_t
#define BLOC_64                  uint64_t
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define MAX_VAL                  ((BLOC_64)0xFFFFFFFF)
#define ARRAY_SIZE               65

struct gn {
    BLOC array[ARRAY_SIZE];
};

void gn_init(struct gn* n);
void gn_from_string(struct gn* n, const char* str, int nbrCarac);
void gn_print(struct gn* n);
void gn_writeFile(struct gn* n, FILE* file);
void gn_add(struct gn* a, struct gn* b, struct gn* c);
void gn_generate_1024(struct gn* n);
void gn_mul(struct gn* a, struct gn* b, struct gn* c);
void generate_testVector(FILE* file, int nbrTest);

#endif  // GN_H
