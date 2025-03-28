%{
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
int lineno = 1;
static void comment(void);
extern void yyerror(const char *);
%}

%option noyywrap

%%
"/*"                    { comment();  }
"//".*                  { }

"auto"					{ yylval.str = strdup(yytext); return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ yylval.str = strdup(yytext); return(CHAR); }
"const"					{ return(CONST); }
"continue"				{ return(CONTINUE); }
"default"				{ return(DEFAULT); }
"do"					{ return(DO); }
"double"				{ yylval.str = strdup(yytext); return(DOUBLE); }
"else"					{ return(ELSE); }
"enum"					{ return(ENUM); }
"extern"				{ yylval.str = strdup(yytext); return(EXTERN); }
"float"					{ yylval.str = strdup(yytext); return(FLOAT); }
"for"					{ return(FOR); }
"goto"					{ return(GOTO); }
"if"					{ return(IF); }
"inline"				{ yylval.str = strdup(yytext); return(INLINE); }
"int"					{ yylval.str = strdup(yytext); return(INT); }
"long"					{ yylval.str = strdup(yytext); return(LONG); }
"register"				{ yylval.str = strdup(yytext); return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ yylval.str = strdup(yytext); return(SHORT); }
"signed"				{ yylval.str = strdup(yytext); return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ yylval.str = strdup(yytext); return(STATIC); }
"struct"				{ yylval.str = strdup(yytext); return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef"				{ yylval.str = strdup(yytext); return(TYPEDEF); }
"union"					{ yylval.str = strdup(yytext); return(UNION); }
"unsigned"				{ yylval.str = strdup(yytext); return(UNSIGNED); }
"void"					{ yylval.str = strdup(yytext); return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"              { return ALIGNAS; }
"_Alignof"              { return ALIGNOF; }
"_Atomic"               { return ATOMIC; }
"_Bool"                 { yylval.str = strdup(yytext); return BOOL; }
"_Complex"              { yylval.str = strdup(yytext); return COMPLEX; }
"_Generic"              { return GENERIC; }
"_Imaginary"            { yylval.str = strdup(yytext); return IMAGINARY; }
"_Noreturn"             { yylval.str = strdup(yytext); return NORETURN; }
"_Static_assert"        { return STATIC_ASSERT; }
"_Thread_local"         { yylval.str = strdup(yytext); return THREAD_LOCAL; }
"__func__"              { return FUNC_NAME; }

[a-zA-Z_][a-zA-Z0-9_]*    { yylval.str = strdup(yytext); return IDENTIFIER; }
"0"|[0-9]+              { yylval.ival = atoi(yytext); return I_CONST; }
[0-9]+\.[0-9]+          { yylval.fval = atof(yytext); return F_CONST; }

"#"[a-zA-Z]+.*          { return PREPROCESSOR; }

((u8|u|U|L)?\"([^"\\\n]|(\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+)))*\"[ \t\v\n\f]*)+         { yylval.str = strdup(yytext); return STRING; }
\'(\\.|[^'])\'          { return CHARACTER; }

"..."					{ return ELLIPSIS; }
">>="					{ return RIGHT_ASSIGN; }
"<<="					{ return LEFT_ASSIGN; }
"+="					{ return ADD_ASSIGN; }
"-="					{ return SUB_ASSIGN; }
"*="					{ return MUL_ASSIGN; }
"/="					{ return DIV_ASSIGN; }
"%="					{ return MOD_ASSIGN; }
"&="					{ return AND_ASSIGN; }
"^="					{ return XOR_ASSIGN; }
"|="					{ return OR_ASSIGN; }
">>"					{ return RIGHT_OP; }
"<<"					{ return LEFT_OP; }
"++"					{ return INCR_OP; }
"--"					{ return DECR_OP; }
"->"					{ return POINTER_OP; }
"&&"					{ return AND_OP; }
"||"					{ return OR_OP; }
"<="					{ return LE_OP; }
">="					{ return GE_OP; }
"=="					{ return EQ_OP; }
"!="					{ return NE_OP; }

";"                     { return(';'); }
("{"|"<%")              { return('{'); }
("}"|"%>")              { return('}'); }
","                     { return(','); }
":"                     { return(':'); }
"="                     { return('='); }
"("                     { return('('); }
")"                     { return(')'); }
("["|"<:")              { return('['); }
("]"|":>")              { return(']'); }
"."                     { return('.'); }
"&"                     { return('&'); }
"!"                     { return('!'); }
"~"                     { return('~'); }
"-"                     { return('-'); }
"+"                     { return('+'); }
"*"                     { return('*'); }
"/"                     { return('/'); }
"%"                     { return('%'); }
"<"                     { return('<'); }
">"                     { return('>'); }
"^"                     { return('^'); }
"|"                     { return('|'); }
"?"                     { return('?'); }

"\n" { lineno += 1; }
[ \t\r]+ ;
. ;

%%

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}