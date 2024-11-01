#include <stdio.h>
#include "parser.tab.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore);
int main()
{
  int ret = yyparse(); 
  exporta (arvore);
  yylex_destroy();
  return ret;
}