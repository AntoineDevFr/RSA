#include "gn.h"
#include <string.h>

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

//Prochaine séance:
//addition et multiplication modulaire