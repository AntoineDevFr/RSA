#ifndef GN_H
#define GN_H

#include <stdint.h>
#include <stdio.h>

#define BLOC                     uint32_t
#define BLOC_64                  uint64_t
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define MAX_VAL                  ((BLOC_64)0xFFFFFFFF)
#define ARRAY_SIZE               129

struct gn {
    BLOC array[ARRAY_SIZE];
};

void gn_init(struct gn* n);
void gn_from_string(struct gn* n, const char* str, int nbrCarac);
void gn_print(struct gn* n);
void gn_writeFile(struct gn* n, FILE* file);
void gn_add(struct gn* a, struct gn* b, struct gn* c);
void gn_generate_1024(struct gn* n);
void gn_generate_max(struct gn* n);
void gn_mul(struct gn* a, struct gn* b, struct gn* c);
void generate_testVector(FILE* file, int nbrTest);
void gn_soustraction(struct gn* a, struct gn* b, struct gn* c);
void gn_addition_modulaire(struct gn* a, struct gn* b, struct gn* n, struct gn* c);
int gn_compare(struct gn* a, struct gn* b);
void gn_shift_right_blocs(struct gn* m, int k);
void montgomery_multiplication(struct gn* A, struct gn* B, struct gn* n, struct gn* r, struct gn* v, int k, struct gn* result);
void square_and_multiply(struct gn* A,struct gn* k, struct gn* r, struct gn* n, struct gn* v, struct gn* result);

#endif  // GN_H
