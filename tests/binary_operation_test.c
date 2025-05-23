#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);
START_TEST(test_binary_operation_addition) {
    Node* return_type = create_value_node("int");
    Node* function_declarator =
        create_function_declarator_node(create_identifier_node("function_name"), create_empty_statement_node());
    Node** statements = (Node**)malloc(1 * sizeof(Node*));
    Node** declarators = (Node**)malloc(1 * sizeof(Node*));
    declarators[0] = create_declaration_node(
        create_identifier_node("var"),
        create_binary_operation_node(
            "+", create_binary_operation_node("+", create_constant_int_node(5), create_constant_int_node(5)),
            create_constant_int_node(15)));
    Node* decl_list = create_declarators_list_node(declarators, 1);
    ((DeclaratorsListNode*)decl_list)->type_specifier = create_value_node("int");
    statements[0] = decl_list;
    Node* function_body = create_compound_statement_node(statements, 1);
    Node* root = create_function_declaration_node(return_type, function_declarator, function_body);
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected =
        "int function_name(){\n"
        "int __internal_temp1 = 5 + 5;\n"
        "int var = __internal_temp1 + 15;\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_binary_operation_multiplication) {
    Node* return_type = create_value_node("int");
    Node* function_declarator =
        create_function_declarator_node(create_identifier_node("function_name"), create_empty_statement_node());
    Node** statements = (Node**)malloc(1 * sizeof(Node*));
    Node** declarators = (Node**)malloc(1 * sizeof(Node*));
    declarators[0] = create_declaration_node(
        create_identifier_node("var"),
        create_binary_operation_node(
            "*", create_binary_operation_node("*", create_constant_int_node(5), create_constant_int_node(5)),
            create_constant_int_node(15)));
    Node* decl_list = create_declarators_list_node(declarators, 1);
    ((DeclaratorsListNode*)decl_list)->type_specifier = create_value_node("int");
    statements[0] = decl_list;
    Node* function_body = create_compound_statement_node(statements, 1);
    Node* root = create_function_declaration_node(return_type, function_declarator, function_body);
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected =
        "int function_name(){\n"
        "int __internal_temp1 = 5 * 5;\n"
        "int var = __internal_temp1 * 15;\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_binary_operation(void) {
    Suite* s = suite_create("Binary operation generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_binary_operation_addition);
    tcase_add_test(tc, test_binary_operation_multiplication);
    suite_add_tcase(s, tc);
    return s;
}