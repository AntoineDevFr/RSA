#include "gn.h"
#include <stdlib.h>
#include <string.h>

void gn_init(struct gn* n) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        n->array[i] = 0;
    }
}

void gn_from_string(struct gn* n, const char* str, int nbrCarac) {
    gn_init(n);
    BLOC tmp;
    int i = nbrCarac - 8;
    int j = 0;

    while (i >= 0) {
        tmp = 0;
        sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
        n->array[j] = tmp;
        i -= 8;
        j += 1;
    }
}

void gn_print(struct gn* n) {
    for (int i = ARRAY_SIZE - 1; i >= 0; i--) {
        printf(SPRINTF_FORMAT_STR, n->array[i]);
    }
    printf("\n");
}

void gn_writeFile(struct gn* n, FILE* file) {
    for (int i = ARRAY_SIZE - 1; i >= 0; i--) {
        fprintf(file, SPRINTF_FORMAT_STR, n->array[i]);
    }
    fprintf(file, ";");
}

void gn_add(struct gn* a, struct gn* b, struct gn* c) {
    BLOC_64 tmp;
    gn_init(c);
    int carry = 0;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        tmp = (BLOC_64)a->array[i] + b->array[i] + carry;
        carry = (tmp > MAX_VAL);
        c->array[i] = (tmp & MAX_VAL);
    }
}

void gn_soustraction(struct gn* a, struct gn* b, struct gn* c)
{
  BLOC_64 res;
  BLOC_64 tmp1;
  BLOC_64 tmp2;
  int borrow = 0;
  int i;
  for (i = 0; i < ARRAY_SIZE; ++i)
  {
    tmp1 = (BLOC_64)a->array[i] + (MAX_VAL + 1); /* + number_base */
    tmp2 = (BLOC_64)b->array[i] + borrow;;
    res = (tmp1 - tmp2);
    c->array[i] = (BLOC)(res & MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
    borrow = (res <= MAX_VAL);
  }
}

void gn_generate_1024(struct gn* n) {
    gn_init(n);
    for (int i = 0; i < 32; ++i) {
        n->array[i] = rand();
    }
}

void gn_generate_max(struct gn* n) {
    gn_init(n);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        n->array[i] = rand();
    }
}

void gn_mul(struct gn* a, struct gn* b, struct gn* c) {
    gn_init(c);
    BLOC_64 tmp;
    BLOC_64 carry;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        carry = 0;
        for (int j = 0; j < ARRAY_SIZE; ++j) {
            if (i + j < ARRAY_SIZE) {
                tmp = (BLOC_64)a->array[i] * (BLOC_64)b->array[j] + c->array[i + j] + carry;
                c->array[i + j] = tmp & MAX_VAL; // conserve les 32 bits de poids faible
                carry = tmp >> 32; //déplace vers la droite de 32 bits, garde les 32 bits de poids fort
            }
        }
        if (carry > 0 && (i + ARRAY_SIZE) < ARRAY_SIZE) {
            c->array[i + ARRAY_SIZE] += carry;
        }
    }
}

void generate_testVector(FILE* file, int nbrTest) {
    struct gn p, q, add, mult;
    file = fopen("test.txt", "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    for (int i = 0; i < nbrTest; i++) {
        gn_generate_1024(&p);
        gn_generate_1024(&q);
        gn_add(&p, &q, &add);
        gn_mul(&p, &q, &mult);
        gn_writeFile(&p, file);
        gn_writeFile(&q, file);
        gn_writeFile(&add, file);
        gn_writeFile(&mult, file);
    }
    fclose(file);
}

int gn_compare(struct gn* a, struct gn* b) {
    for (int i = ARRAY_SIZE - 1; i >= 0; --i) {
        if (a->array[i] > b->array[i]) {
            return 1; // A > B
        } else if (a->array[i] < b->array[i]) {
            return -1; // A < B
        }
    }
    return 0; // A == B
}

//A et B sont des entiers modulo N
void gn_addition_modulaire(struct gn* a, struct gn* b, struct gn* n, struct gn* c) {
    gn_init(c);

    //A+B
    struct gn aPb;
    gn_init(&aPb);
    gn_add(a, b, &aPb);

    if (gn_compare(&aPb, n) == -1) {
        //A+B
        gn_add(a, b, c);
    } else {
        //A+B-N
        gn_print(&aPb);
        gn_soustraction(&aPb, n, c);
    }
}   

void gn_mult_modulaire_montgomery(struct gn* a, struct gn* b, struct gn* c) {

}
