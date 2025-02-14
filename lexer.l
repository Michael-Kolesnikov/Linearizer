%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%option noyywrap

%%
"auto"					{ return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ return(CHAR); }
"const"					{ return(CONST); }
"continue"				{ return(CONTINUE); }
"default"				{ return(DEFAULT); }
"do"					{ return(DO); }
"double"				{ return(DOUBLE); }
"else"					{ return(ELSE); }
"enum"					{ return(ENUM); }
"extern"				{ return(EXTERN); }
"float"					{ return(FLOAT); }
"for"					{ return(FOR); }
"goto"					{ return(GOTO); }
"if"					{ return(IF); }
"inline"				{ return(INLINE); }
"int"					{ return(INT); }
"long"					{ return(LONG); }
"register"				{ return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ return(SHORT); }
"signed"				{ return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ return(STATIC); }
"struct"				{ return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef"				{ return(TYPEDEF); }
"union"					{ return(UNION); }
"unsigned"				{ return(UNSIGNED); }
"void"					{ return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"              { return ALIGNAS; }
"_Alignof"              { return ALIGNOF; }
"_Atomic"               { return ATOMIC; }
"_Bool"                 { return BOOL; }
"_Complex"              { return COMPLEX; }
"_Generic"              { return GENERIC; }
"_Imaginary"            { return IMAGINARY; }
"_Noreturn"             { return NORETURN; }
"_Static_assert"        { return STATIC_ASSERT; }
"_Thread_local"         { return THREAD_LOCAL; }
"__func__"              { return FUNC_NAME; }

"#"[a-zA-Z]+.*          {yylval.sval = strdup(yytext); printf("PREPROCESSOR "); return PREPROCESSOR; }
"//".*                  {yylval.sval = strdup(yytext); printf("COMMENT "); return COMMENT; }
"/\\*".*?"\\*/"         {yylval.sval = strdup(yytext); printf("COMMENT "); return COMMENT; }

\"(\\.|[^"])*\"          {yylval.sval = strdup(yytext); printf("STRING "); return STRING; }
\'(\\.|[^'])\'           {yylval.sval = strdup(yytext); printf("CHARACTER "); return CHARACTER; }

[0-9]+ {yylval.sval = strdup(yytext); printf("NUMBER "); return NUMBER; }
[0-9]+\.[0-9]+ {yylval.sval = strdup(yytext); printf("FLOAT "); return FLOAT; }


";"                     {   printf(";"); return(';');    }
("{"|"<%")              {   return('{');    }
("}"|"%>")              {   return('}');    }
","                     {   return(',');    }
":"                     {   return(':');    }
"="                     {   return('=');    }
"("                     {   return('(');    }
")"                     {   return(')');    }
("["|"<:")              {   return('[');    }
("]"|":>")              {   return(']');    }
"."                     {   return('.');    }
"&"                     {   return('&');    }
"!"                     {   return('!');    }
"~"                     {   return('~');    }
"-"                     {   return('-');    }
"+"                     {   return('+');    }
"*"                     {   return('*');    }
"/"                     {   return('/');    }
"%"                     {   return('%');    }
"<"                     {   return('<');    }
">"                     {   return('>');    }
"^"                     {   return('^');    }
"|"                     {   return('|');    }
"?"                     {   return('?');    }

"=="|"!="|"<="|">="     {yylval.sval = strdup(yytext);printf("OPERATOR "); return OPERATOR; }

[ \t\n\r]+ ;
. ;

%%

