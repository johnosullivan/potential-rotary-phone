#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include <stack>

#include "../../common/visitor.h"

#include "../parser/ast.h"

namespace core::visitor {
    class Scope {
        public:

        bool already_declared(std::string);
        bool already_declared(std::string, std::vector<parser::TYPE>);

        void declare(std::string, int);
        void declare(std::string, float);
        void declare(std::string, std::string);
        void declare(std::string, bool);

        void declare(std::string, std::vector<parser::TYPE>, std::vector<std::string>, parser::ASTBlockNode*);

        parser::TYPE type_of(std::string);
        value_t value_of(std::string);

        std::vector<std::string> variable_names_of(std::string, std::vector<parser::TYPE>);
        std::vector<std::tuple<std::string, std::string, std::string>>  all_variable_list();
        parser::ASTBlockNode* block_of(std::string, std::vector<parser::TYPE>);

    private:
        std::map<std::string,
                 std::pair<parser::TYPE,
                           value_t>> variable_symbol_table;

        std::multimap<std::string,
                      std::tuple<std::vector<parser::TYPE>,
                                 std::vector<std::string>,
                                 parser::ASTBlockNode*>> function_symbol_table;

    };

    class Interpreter : public Visitor {
    public:
        Interpreter();
        Interpreter(Scope*);
        ~Interpreter();

        void visit(parser::ASTProgramNode*) override;

        void visit(parser::ASTLiteralNode<int>*) override;
        void visit(parser::ASTLiteralNode<float>*) override;
        void visit(parser::ASTLiteralNode<std::string>*) override;
        void visit(parser::ASTLiteralNode<bool>*) override;

        void visit(parser::ASTBinaryExprNode*) override;

        void visit(parser::ASTDeclarationNode*) override;
        void visit(parser::ASTIdentifierNode*) override;

        void visit(parser::ASTAssignmentNode*) override;

        void visit(parser::ASTStdOutNode*) override;

        void visit(parser::ASTFuncNode*) override;
        void visit(parser::ASTBlockNode*) override;
        void visit(parser::ASTReturnNode*) override;
        void visit(parser::ASTExprFuncCallNode*) override;

        std::pair<parser::TYPE, value_t> current_expr();

    private:
        parser::TYPE current_expression_type;
        value_t current_expression_value;

        std::vector<std::string> current_function_parameters;
        std::vector<std::pair<parser::TYPE, value_t>> current_function_arguments;

        std::vector<Scope*> scopes;
    };

    std::string type_str(parser::TYPE);
}

#endif //INTERPRETER_H
