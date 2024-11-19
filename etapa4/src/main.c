/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include <stdio.h>
#include "parser.tab.h"
#include "tree.h"
#include "table.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore);
int main() {
  // int ret = yyparse(); 
  // exporta (arvore);
  // yylex_destroy();
  // node_free((struct node_t *)arvore);

  struct symbol_table_t *table = enter_scope(NULL);

  struct symbol_t *s1 = create_symbol("a", 1, SYMBOL_VARIABLE, DATA_INT);
  declare_symbol(table, s1);

  struct symbol_t *s2 = create_symbol("b", 1, SYMBOL_FUNCTION, DATA_FLOAT);
  declare_symbol(table, s2);

  struct symbol_t *s3 = create_symbol("c", 1, SYMBOL_VARIABLE, DATA_FLOAT);
  declare_symbol(table, s3);
  print_table(table);

  printf("==============================\n");
  table = enter_scope(table);
  declare_symbol(table, create_symbol("a1", 2, SYMBOL_VARIABLE, DATA_INT));
  declare_symbol(table, create_symbol("b1", 3, SYMBOL_VARIABLE, DATA_INT));
  declare_symbol(table, create_symbol("c1", 2, SYMBOL_VARIABLE, DATA_INT));

  print_table(table);

  printf("==============================\n");
  table = exit_scope(table);
  print_table(table);

  return 0;
}