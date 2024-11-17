%{
#include "parser.tab.h"
%}

%%
[0-9]+    { return NUMBER; }
.          { return yytext[0]; }
%%

// required by Flex
int yywrap() { return 1; }
