#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <functional>
#include <stdexcept>
#include <vector>

namespace ASTImpl {
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class FormulaAST {
public:
	using CellValueGetter = std::function<CellInterface::Value(Position)>;

    explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr, std::forward_list<Position> cells);
    FormulaAST(FormulaAST&&) = default;
    FormulaAST& operator=(FormulaAST&&) = default;
    ~FormulaAST();

    CellInterface::Value Execute(const CellValueGetter& get_cell_value) const;
    void Print(std::ostream& out) const;
    void PrintFormula(std::ostream& out) const;

    std::vector<Position> GetReferencedCells() const;

private:
    std::unique_ptr<ASTImpl::Expr> root_expr_;
    std::vector<Position> cells_;
};

FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);