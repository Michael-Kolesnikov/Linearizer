#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);
START_TEST(test_function_call_without_arguments)
{
    Node* root = create_function_call_node(create_identifier_node("function_call"),create_empty_statement_node());
    // Node** args = (Node**)malloc(1 * sizeof(Node*));
    // args[0] = create_empty_statement_node();
    // Node* root = create_function_declaration_node(return_type,function_declarator,create_empty_statement_node());
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = 
        "function_call()";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_function_call_with_one_argument)
{
    Node** args = (Node**)malloc(1 * sizeof(Node*));
    args[0] = create_identifier_node("arg");
    Node* args_node = create_arguments_node(args, 1);
    Node* root = create_function_call_node(create_identifier_node("function_call"),args_node);
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = 
        "function_call(arg)";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_function_call_with_onemore_arguments)
{
    Node** args = (Node**)malloc(2 * sizeof(Node*));
    args[0] = create_identifier_node("arg1");
    args[1] = create_identifier_node("arg2");
    Node* args_node = create_arguments_node(args, 2);
    Node* root = create_function_call_node(create_identifier_node("function_call"),args_node);
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = 
        "function_call(arg1, arg2)";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_function_call(void) {
    Suite* s = suite_create("Function call generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_function_call_without_arguments);
    tcase_add_test(tc, test_function_call_with_one_argument);
    tcase_add_test(tc, test_function_call_with_onemore_arguments);
    suite_add_tcase(s, tc);
    return s;
}