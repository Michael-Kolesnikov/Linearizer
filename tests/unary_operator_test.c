#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);

START_TEST(test_unary_plus) {
    Node* root = create_unary_operator_expression_node("+", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "+some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_unary_minus) {
    Node* root = create_unary_operator_expression_node("-", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "-some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_unary_dereference) {
    Node* root = create_unary_operator_expression_node("*", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "*some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_unary_logical_not) {
    Node* root = create_unary_operator_expression_node("!", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "!some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_unary_bitwise_not) {
    Node* root = create_unary_operator_expression_node("~", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "~some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_unary_address_of) {
    Node* root = create_unary_operator_expression_node("&", create_identifier_node("some_expression"));
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "&some_expression";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}

START_TEST(test_unary_minus_in_arguments) {
    Node** args = (Node**)malloc(1 * sizeof(Node*));
    Node* unary = create_unary_operator_expression_node("-", create_identifier_node("some_expression"));
    args[0] = unary;
    Node* args_node = create_arguments_node(args, 1);
    Node* root = create_function_call_node(create_identifier_node("function_call"), args_node);

    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "function_call(-some_expression)";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_unary_operator(void) {
    Suite* s = suite_create("Unary operator generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_unary_plus);
    tcase_add_test(tc, test_unary_minus);
    tcase_add_test(tc, test_unary_address_of);
    tcase_add_test(tc, test_unary_bitwise_not);
    tcase_add_test(tc, test_unary_dereference);
    tcase_add_test(tc, test_unary_logical_not);
    tcase_add_test(tc, test_unary_minus_in_arguments);
    suite_add_tcase(s, tc);
    return s;
}