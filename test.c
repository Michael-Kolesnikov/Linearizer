#include <stdio.h> // Preprocessor directive

int square(int num) {
    return num * num;
}

int main() {
    int a = 10;
    float b = 3.14;
    char c = 'A';
    const char* str = "Hello, Lexer!";

    printf("Integer: %d\n", a);
    printf("Float: %.2f\n", b);
    printf("Character: %c\n", c);
    printf("String: %s\n", str);

    int result = square(a);
    printf("Square of %d: %d\n", a, result);

    if (a > b) {
        printf("a is greater than b\n");
    } else {
        printf("b is greater than or equal to a\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("i = %d\n", i);
    }

    // Comment examples
    // This is a single-line comment
    /* This is a
       multi-line comment */

    return 0;
}
