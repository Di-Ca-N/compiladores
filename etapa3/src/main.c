/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include <stdio.h>
#include "parser.tab.h"
#include "tree.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore);
int main()
{
  int ret = yyparse(); 
  exporta (arvore);
  yylex_destroy();
  node_free((struct node_t *)arvore);
  return ret;
}