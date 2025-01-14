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
void gn_shift_right_blocs(struct gn* m, int k) {
    if (k <= 0) return; // Rien à faire si k <= 0
    if (k >= ARRAY_SIZE) {
        gn_init(m); // Décale de plus que la taille => tout est 0
        return;
    }

    // Décaler les blocs vers la droite
    for (int i = 0; i < ARRAY_SIZE - k; ++i) {
        m->array[i] = m->array[i + k];
    }

    // Remplir les k derniers blocs avec des zéros
    for (int i = ARRAY_SIZE - k; i < ARRAY_SIZE; ++i) {
        m->array[i] = 0;
    }
}


// k = 65 avec r = 2**(32*65)
void montgomery_multiplication(struct gn* A, struct gn* B, struct gn* n, struct gn* v, int k, struct gn* result) {
    struct gn s, t, m, u;
    gn_init(&s);
    gn_init(&t);
    gn_init(&m);
    gn_init(&u);
    gn_init(result);

    // s = A * B
    gn_mul(A, B, &s);

    // t = (s * v) mod r
    gn_mul(&s, v, &t);

    // Réduire t modulo r : comme r est une puissance de 2, on garde seulement les k premiers blocs de t
    for (int i = k; i < ARRAY_SIZE; ++i) {
        t.array[i] = 0;  // t mod r
    }
    //t est vérifié 
    
    // m = s + t * n
    struct gn tmp;
    gn_init(&tmp);
    gn_mul(&t, n, &tmp);  // tmp = t * n
    gn_add(&s, &tmp, &m); // m = s + t * n

    //m est vérifié

    // Étape 4: u = m / r (Décalage de m de k blocs vers la droite)
    gn_shift_right_blocs(&m, k); // Décaler m de k blocs vers la droite

    //u est vérifié
    // Étape 5: Ajustement final : si u >= n alors result = u - n, sinon result = u
    if (gn_compare(&m, n) == 1) {
        gn_soustraction(&m, n, result);  // result = u - n
    } else {
        gn_add(&m, &u, result);  // result = u
    }
}

void square_and_multiply(struct gn* a, struct gn* k, struct gn* r2, struct gn* r, struct gn* n, struct gn* v, struct gn* result) {
    struct gn A, P, un,null, tmp; 
    uint32_t H;
    int i = ARRAY_SIZE - 1;
    int j;
    int l = 0;
    int bin[32];

    gn_init(&P);
    gn_init(&A);
    gn_init(result);
    gn_init(&un);
    gn_init(&tmp);
    gn_init(&null);
    un.array[0] = 1;

    
    montgomery_multiplication(a, r2, n, v, 65, &A); // A = a * r mod n
    gn_soustraction(r, n, &P); // P = r - n
    
    while (i >= 0) {
        H = k->array[i];
        l = 0;
        //printf("%d\n",i);
        while( H > 0 ){
            if (H % 2 == 0){
                bin[l] = 0;
            }else{
                bin[l] = 1;
            }
            H = H/2;
            l++;
	    }
        //bin contient le nombre en binaire 

        j = 31;
        while(j>0){
            montgomery_multiplication(&P, &P, n, v, 65, &tmp);
            gn_add(&tmp, &null, &P);
            gn_init(&tmp);

            if(bin[j] == 1 ){
                montgomery_multiplication(&P, &A, n, v, 65, &tmp);
                gn_add(&tmp, &null, &P);
                gn_init(&tmp);
            }
            j--;
	    }
        i--;
    }
    montgomery_multiplication(&P, &un, n, v, 65, result); 
}