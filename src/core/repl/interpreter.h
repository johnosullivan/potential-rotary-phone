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
        void declare(std::string, int);
        void declare(std::string, float);

        parser::TYPE type_of(std::string);
        value_t value_of(std::string);

        std::vector<std::string> variable_names_of(std::string, std::vector<parser::TYPE>);
        std::vector<std::tuple<std::string, std::string, std::string>>  variable_list();

    private:
        std::map<std::string,
                 std::pair<parser::TYPE,
                           value_t>> variable_symbol_table;

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

        void visit(parser::ASTBinaryExprNode*) override;

        void visit(parser::ASTDeclarationNode*) override;
        void visit(parser::ASTIdentifierNode*) override;

        void visit(parser::ASTAssignmentNode*) override;

        std::pair<parser::TYPE, value_t> current_expr();

    private:
        parser::TYPE current_expression_type;
        value_t current_expression_value;

        std::vector<Scope*> scopes;
    };

    std::string type_str(parser::TYPE);
}

#endif //INTERPRETER_H
