#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern void generate_code_from_ast(Node* node, FILE* output);
extern char* read_file_to_string(FILE* file);

START_TEST(test_if_statement) {
    Node* return_type = create_value_node("int");
    Node* function_declarator =
        create_function_declarator_node(create_identifier_node("function_name"), create_empty_statement_node());
    Node** statements = (Node**)malloc(2 * sizeof(Node*));
    Node** declarators = (Node**)malloc(1 * sizeof(Node*));
    declarators[0] = create_declaration_node(create_value_node("a"), create_constant_int_node(2));
    Node* decl_list = create_declarators_list_node(declarators, 1);
    ((DeclaratorsListNode*)decl_list)->type_specifier = create_value_node("int");
    statements[0] = decl_list;
    Node** then_compound_statements = (Node**)malloc(1 * sizeof(Node*));
    then_compound_statements[0] = create_return_node(create_constant_int_node(0));
    Node** else_compound_statements = (Node**)malloc(1 * sizeof(Node*));
    else_compound_statements[0] = create_return_node(create_constant_int_node(1));
    statements[1] =
        create_if_node(create_logical_operation_node(create_identifier_node("a"), ">", create_constant_int_node(0)),
                       create_compound_statement_node(then_compound_statements, 1),
                       create_compound_statement_node(else_compound_statements, 1));
    Node* function_body = create_compound_statement_node(statements, 2);
    Node* root = create_function_declaration_node(return_type, function_declarator, function_body);
    FILE* output = tmpfile();
    ck_assert_ptr_nonnull(output);
    generate_code_from_ast(root, output);

    char* result = read_file_to_string(output);
    const char* expected =
        "int function_name(){\n"
        "int a = 2;\n"
        "int __internal_temp1 = (a > 0);\n"
        "if (__internal_temp1) goto lbl1;\n"
        "return 1;\n"
        "goto lbl2;\n"
        "lbl1:\n"
        "return 0;\n"
        "lbl2:\n"
        "}\n";
    ck_assert_str_eq(result, expected);

    fclose(output);
    free(result);
}
END_TEST

Suite* codegen_test_if_statement(void) {
    Suite* s = suite_create("if statement generation");
    TCase* tc = tcase_create("Core");
    tcase_add_test(tc, test_if_statement);
    suite_add_tcase(s, tc);
    return s;
}