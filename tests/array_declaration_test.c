#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);
START_TEST(test_array_declaration_in_parameters) {
    Node* return_type = create_value_node("int");
    Node** params = (Node**)malloc(1 * sizeof(Node*));
    Node* array = create_array_declaration_node(create_identifier_node("array"), create_empty_statement_node());
    Node** declarator = (Node**)malloc(1 * sizeof(Node*));
    declarator[0] = create_declaration_node(array, create_empty_statement_node());
    Node* parameter = create_declarators_list_node(declarator, 1);
    ((DeclaratorsListNode*)parameter)->type_specifier = create_value_node("float");
    params[0] = parameter;
    Node* parameters_node = create_parameters_node(params, 1);
    Node* function_declarator =
        create_function_declarator_node(create_identifier_node("function_name"), parameters_node);
    Node** statements = (Node**)malloc(1 * sizeof(Node*));
    statements[0] = create_empty_statement_node();
    Node* function_body = create_compound_statement_node(statements, 1);
    Node* root = create_function_declaration_node(return_type, function_declarator, create_empty_statement_node());

    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected =
        "int function_name(float array[]){\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_array_declaration) {
    Node* array = create_array_declaration_node(create_identifier_node("array"), create_constant_int_node(10));

    Node** declarator = (Node**)malloc(1 * sizeof(Node*));
    declarator[0] = create_declaration_node(array, create_empty_statement_node());
    Node* root = create_declarators_list_node(declarator, 1);
    ((DeclaratorsListNode*)root)->type_specifier = create_value_node("float");

    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "float array[10];";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_multidimensional_array_declaration) {
    Node* dim1 = create_array_declaration_node(create_identifier_node("x"), create_constant_int_node(1));
    Node* dim2 = create_array_declaration_node(dim1, create_constant_int_node(2));
    Node** declarator = (Node**)malloc(1 * sizeof(Node*));
    declarator[0] = create_declaration_node(dim2, create_empty_statement_node());
    Node* root = create_declarators_list_node(declarator, 1);
    ((DeclaratorsListNode*)root)->type_specifier = create_value_node("int");

    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = "int x[1][2];";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_array_declaration(void) {
    Suite* s = suite_create("Function call generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_array_declaration);
    tcase_add_test(tc, test_array_declaration_in_parameters);
    tcase_add_test(tc, test_multidimensional_array_declaration);
    suite_add_tcase(s, tc);
    return s;
}