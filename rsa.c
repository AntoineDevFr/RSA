#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOC                     uint32_t
#define BLOC_64                  uint64_t
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define MAX_VAL                  ((BLOC_64)0xFFFFFFFF)        
#define ARRAY_SIZE               65

/*32 bits fois ARRAY_SIZE */
struct gn
{
  BLOC array[ARRAY_SIZE];
};

void gn_init(struct gn* n)
{
  for (int i = 0; i < ARRAY_SIZE; ++i)
  {
    n->array[i] = 0;
  }
}

/*Permet de convertir une chaine de caractère hexa en un grand nombre (multiple de 8)*/
void gn_from_string(struct gn* n,const char* str, int nbrCarac)
{ 
  gn_init(n);
  BLOC tmp; 
  int i = nbrCarac - 8; //On commence par la fin de la chaine
  int j = 0; 

  while (i >= 0)
  {
    tmp = 0;
    sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
    n->array[j] = tmp;
    i -= 8; 
    j += 1;               
  }
}

void gn_print(struct gn* n) {
    for (int i = ARRAY_SIZE - 1 ; i >= 0; i--) {
        printf(SPRINTF_FORMAT_STR, n->array[i]);
    }
    printf("\n");
}

void gn_writeFile(struct gn* n, FILE* file) {
    for (int i = ARRAY_SIZE - 1 ; i >= 0; i--) {
        fprintf(file, SPRINTF_FORMAT_STR, n->array[i]);
    }
    fprintf(file, ";");
}

void gn_add(struct gn* a, struct gn* b, struct gn* c)
{
  BLOC_64 tmp;
  gn_init(c);
  int carry = 0;
  int i;
  for (i = 0; i < ARRAY_SIZE; ++i)
  {
    tmp = (BLOC_64)a->array[i] + b->array[i] + carry;
    carry = (tmp > MAX_VAL);
    c->array[i] = (tmp & MAX_VAL);
  }
}

void gn_generate_1024(struct gn* n)
{
  gn_init(n);
  for (int i = 0; i < 32; ++i)
  {
    n->array[i] = rand();
  }
}

void gn_mul(struct gn* a, struct gn* b, struct gn* c)
{
    gn_init(c);
    BLOC_64 tmp;
    BLOC_64 carry;

    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        carry = 0;
        for (int j = 0; j < ARRAY_SIZE; ++j)
        {
            if (i + j < ARRAY_SIZE)
            {
                tmp = (BLOC_64)a->array[i] * (BLOC_64)b->array[j] + c->array[i + j] + carry; 

                c->array[i + j] = tmp & MAX_VAL;  // Stocker le résultat dans l'array (32 bits)
                carry = tmp >> 32;  // Calculer la retenue (bits supérieurs au 32e)
            }
        }

        // Si après la dernière colonne il y a une retenue, on la place dans la case suivante
        if (carry > 0 && (i + ARRAY_SIZE) < ARRAY_SIZE)
        {
            c->array[i + ARRAY_SIZE] += carry;
        }
    }
}

void generate_testVector(FILE* file, int nbrTest) {
  struct gn p, q, add, mult;
    file = fopen("test.txt", "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }

  for (int i = 0; i < nbrTest; i++) {
      gn_generate_1024(&p);
      gn_generate_1024(&q);
      gn_init(&add);
      gn_init(&mult);

      gn_add(&p, &q, &add);
      gn_mul(&p, &q, &mult);

      gn_writeFile(&p, file);
      gn_writeFile(&q, file);
      gn_writeFile(&add, file);
      gn_writeFile(&mult, file);
    }
};

int main() {
  /*Test*/
  FILE *file;
  generate_testVector(file, 1000);

  /*Test clés cours*/
  struct gn p, q, result;
  gn_init(&p);
  gn_init(&q);
  gn_init(&result);
  char p_str[] = "000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000283";
  char q_str[] = "000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000439";
  
  gn_from_string(&p, p_str, strlen(p_str));
  gn_from_string(&q, q_str, strlen(q_str));

  printf("P = "); 
  gn_print(&p);

  printf("Q = ");
  gn_print(&q);

  printf("Resultat de la multiplication:\n");
  gn_mul(&p, &q, &result);
  gn_print(&result);

  return 0;
}
