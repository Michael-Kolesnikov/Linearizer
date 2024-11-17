#include <stdio.h>
#include <string.h>

extern int yyparse();
extern void yy_scan_string(const char *str);
extern int result;


int process_expression(const char *input) {
    yy_scan_string(input);
    if (yyparse() == 0) {
        return result;
    } else {
        fprintf(stderr, "Error in parsing input.\n");
        return -1;
    }
}

int main() {
    char input[] = "4";

    int output = process_expression(input);
    if (output != -1) {
        printf("Result: %d\n", output);
    }

    return 0;
}
