#include <check.h>
#include <stdlib.h>
#include <stdio.h>
Suite* codegen_test_function_declaration(void);
Suite* codegen_test_function_call(void);
Suite* codegen_test_unary_operator(void);

char* read_file_to_string(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(size + 1);
    if (!content) {
        return NULL;
    }

    fread(content, 1, size, file);
    content[size] = '\0';
    return content;
}

int main(void) {
    SRunner* sr = srunner_create(NULL);

    srunner_add_suite(sr, codegen_test_function_declaration());
    srunner_add_suite(sr, codegen_test_function_call());
    srunner_add_suite(sr, codegen_test_unary_operator());
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}