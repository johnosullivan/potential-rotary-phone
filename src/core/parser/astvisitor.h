#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include <fstream>

#include "../../common/visitor.h"

#include "ast.h"

namespace core::visitor {

    class ASTVisitor : public core::visitor::Visitor {

    public:
        ASTVisitor();
        ~ASTVisitor();

        void visit(parser::ASTProgramNode*) override;
        void visit(parser::ASTLiteralNode<int>*) override;
        void visit(parser::ASTBinaryExprNode*) override;

        void visit(parser::ASTDeclarationNode*) override;
        void visit(parser::ASTIdentifierNode*) override;

        void visit(parser::ASTAssignmentNode*) override;
    private:
        unsigned int indentation_level;
        const std::string TAB = "    ";
        std::string indentation();
        std::string type_str(parser::TYPE);
        std::string safe_op(std::string);
    };

}

#endif //XML_VISITOR_H