#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);
START_TEST(test_function_declaration_without_params)
{
    Node* return_type = create_value_node("int");
    Node* function_declarator = create_function_declarator_node(create_identifier_node("function_name"),create_empty_statement_node());
    Node** statements = (Node**)malloc(1 * sizeof(Node*));
    statements[0] = create_empty_statement_node();
    Node* function_body = create_compound_statement_node(statements, 1);
    Node* root = create_function_declaration_node(return_type,function_declarator,create_empty_statement_node());
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = 
        "int function_name(){\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

START_TEST(test_function_declaration_with_one_parameter)
{
    Node* return_type = create_value_node("int");
    Node** params = (Node**)malloc(1 * sizeof(Node*));
    Node* first_param = create_declaration_node(create_identifier_node("param1"),create_empty_statement_node());
    ((DeclarationNode*)first_param)->type_specifier = create_value_node("int");
    params[0] = first_param;
    Node* parameters_node = create_parameters_node(params, 1);
    Node* function_declarator = create_function_declarator_node(create_identifier_node("function_name"),parameters_node);
    Node** statements = (Node**)malloc(1 * sizeof(Node*));
    statements[0] = create_empty_statement_node();
    Node* function_body = create_compound_statement_node(statements, 1);
    Node* root = create_function_declaration_node(return_type,function_declarator,create_empty_statement_node());
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected = 
        "int function_name(int param1){\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_function_declaration(void) {
    Suite* s = suite_create("Function declaration Generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_function_declaration_without_params);
    tcase_add_test(tc, test_function_declaration_with_one_parameter);
    suite_add_tcase(s, tc);
    return s;
}