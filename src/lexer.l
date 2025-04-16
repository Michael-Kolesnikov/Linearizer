%{
#include "symbolTable.h"
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
static void comment(void);
extern void yyerror(const char *);

int lineno = 1;
static int typedef_depth = 0;

int is_typedef_name(const char *name) {
    Symbol *s = symtab_lookup(name);
    return s && s->type == SYM_TYPEDEF;
}

%}
%option debug
%option noyywrap
%x TYPEDEF_STATE TYPEDEF_DECLARATOR
identifier [a-zA-Z_][a-zA-Z0-9_]*

%%
<*>"/*"                    { comment();  }
<*>"//".*                  { }

"auto"					{ yylval.str = strdup(yytext); return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ yylval.str = strdup(yytext); return(CHAR); }
"const"					{ yylval.str = strdup(yytext); return(CONST); }
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
"restrict"				{ yylval.str = strdup(yytext); return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ yylval.str = strdup(yytext); return(SHORT); }
"signed"				{ yylval.str = strdup(yytext); return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ yylval.str = strdup(yytext); return(STATIC); }
"static_assert"         { yylval.str = strdup(yytext); return(STATIC_ASSERT); }
"struct"				{ yylval.str = strdup(yytext); return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef" {
    BEGIN(TYPEDEF_STATE);
    typedef_depth = 0;
    yylval.str = strdup(yytext);
    return(TYPEDEF);
}
"union"					{ yylval.str = strdup(yytext); return(UNION); }
"unsigned"				{ yylval.str = strdup(yytext); return(UNSIGNED); }
"void"					{ yylval.str = strdup(yytext); return(VOID); }
"volatile"				{ yylval.str = strdup(yytext); return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"              { return ALIGNAS; }
"_Alignof"              { return ALIGNOF; }
"_Atomic"               { yylval.str = strdup(yytext); return ATOMIC; }
"_Bool"                 { yylval.str = strdup(yytext); return BOOL; }
"_Complex"              { yylval.str = strdup(yytext); return COMPLEX; }
"_Generic"              { return GENERIC; }
"_Imaginary"            { yylval.str = strdup(yytext); return IMAGINARY; }
"_Noreturn"             { yylval.str = strdup(yytext); return NORETURN; }
"_Static_assert"        { return STATIC_ASSERT; }
"_Thread_local"         { yylval.str = strdup(yytext); return THREAD_LOCAL; }
"__func__"              { yylval.str = strdup(yytext); return FUNC_NAME; }

{identifier} {
    yylval.str = strdup(yytext);
    if (is_typedef_name(yytext)) {
        return TYPEDEF_NAME;
    }
    symtab_add(yytext, SYM_IDENTIFIER, NULL);
    return IDENTIFIER;
}
<*>"0"|[0-9]+              { yylval.ival = atoi(yytext); return I_CONST; }
<*>[0-9]+\.[0-9]+          { yylval.fval = atof(yytext); return F_CONST; }

<*>((u8|u|U|L)?\"([^"\\\n]|(\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+)))*\"[ \t\v\n\f]*)+         { yylval.str = strdup(yytext); return STRING; }

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

";" { 
    return(';');
}
("{"|"<%") { 
    return('{');
}
("}"|"%>") {
    return('}');
}
","                     { return(','); }
<*>":"                     { return(':'); }
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

<*>"\n" { lineno += 1; }
[ \t\r]+ ;
. ;

<TYPEDEF_STATE>"const" {
    yylval.str = strdup(yytext);
    return CONST;
}

<TYPEDEF_STATE>"restrict" {
    yylval.str = strdup(yytext);
    return RESTRICT;
}

<TYPEDEF_STATE>"volatile" {
    yylval.str = strdup(yytext);
    return VOLATILE;
}

<TYPEDEF_STATE>"_Atomic" {
    yylval.str = strdup(yytext);
    return ATOMIC;
}

<TYPEDEF_STATE>"struct" {
    yylval.str = strdup(yytext);
    return STRUCT;
}

<TYPEDEF_STATE>"union" {
    yylval.str = strdup(yytext);
    return UNION;
}

<TYPEDEF_STATE>"enum" {
    yylval.str = strdup(yytext);
    return ENUM;
}

<TYPEDEF_STATE>"int" {
    yylval.str = strdup(yytext);
    if(typedef_depth == 0){
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return INT;
}

<TYPEDEF_STATE>"float" {
    yylval.str = strdup(yytext);
    if(typedef_depth == 0){
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return FLOAT;
}

<TYPEDEF_STATE>"char" {
    yylval.str = strdup(yytext);
    if(typedef_depth == 0){
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return CHAR;
}

<TYPEDEF_STATE>"void" {
    yylval.str = strdup(yytext);
    if(typedef_depth == 0){
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return VOID;
}

<TYPEDEF_STATE>"double" {
    yylval.str = strdup(yytext);
    if(typedef_depth == 0){
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return DOUBLE;
}

<TYPEDEF_STATE>"{" {
    typedef_depth++;
    return '{';
}

<TYPEDEF_STATE>"}" {
    typedef_depth--;
    if (typedef_depth == 0) {
        BEGIN(TYPEDEF_DECLARATOR);
    }
    return '}';
}

<TYPEDEF_STATE>{identifier} {
    yylval.str = strdup(yytext);
    if (is_typedef_name(yytext)) {
        return TYPEDEF_NAME;
    }
    return IDENTIFIER;
}

<TYPEDEF_STATE>"*" {
    return '*';
}

<TYPEDEF_STATE>"(" {
    return '(';
}

<TYPEDEF_STATE>")" {
    return ')';
}

<TYPEDEF_STATE>";" {
    if(typedef_depth == 0){
        BEGIN(INITIAL);
    }
    return ';';
}

<TYPEDEF_STATE>"," {
    return ',';
}

<TYPEDEF_STATE>. {
}

<TYPEDEF_DECLARATOR>{identifier}  {
    if (strcmp(yytext, "struct") != 0 && strcmp(yytext, "union") != 0 && 
        strcmp(yytext, "enum") != 0 && strcmp(yytext, "const") != 0 && 
        strcmp(yytext, "volatile") != 0 && strcmp(yytext, "static") != 0 && 
        strcmp(yytext, "extern") != 0 && strcmp(yytext, "inline") != 0 &&
        strcmp(yytext, "int") != 0 && strcmp(yytext, "float") != 0 &&
        strcmp(yytext, "char") != 0 && strcmp(yytext, "void") != 0 &&
        strcmp(yytext, "double") != 0 && strcmp(yytext, "long") != 0 &&
        strcmp(yytext, "short") != 0 && strcmp(yytext, "signed") != 0 &&
        strcmp(yytext, "unsigned") != 0) {
        symtab_add(yytext, SYM_TYPEDEF, NULL);
    }
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

<TYPEDEF_DECLARATOR>"," {
    return (',');
}

<TYPEDEF_DECLARATOR>";" {
    BEGIN(INITIAL);
    return ';';
}

<TYPEDEF_DECLARATOR>"*" {
    return '*';
}

<TYPEDEF_DECLARATOR>"(" {
    return '(';
}

<TYPEDEF_DECLARATOR>")" {
    BEGIN(INITIAL);
    return ')';
}

<TYPEDEF_DECLARATOR>.
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