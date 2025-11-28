#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>
#include <optional>
#include <memory>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;
	
	// Методы для управления зависимостями
    void AddDependentCell(Position pos);
    void RemoveDependentCell(Position pos);
    const std::unordered_set<Position, PositionHash>& GetDependentCells() const;
    
    // Управление кэшем
    void InvalidateCache();
    void InvalidateDependentCellsCache();
    bool IsCacheValid() const;

private:
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    Sheet& sheet_;
    std::unique_ptr<Impl> impl_;
	
	// Кэшированное значение
    mutable std::optional<Value> cached_value_;
    mutable bool cache_is_valid_ = false;
    
    // Обратные зависимости - ячейки, которые зависят от этой
    std::unordered_set<Position, PositionHash> dependent_cells_;
    
    struct PositionHash {
        size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.row) ^ (std::hash<int>()(pos.col) << 1);
        }
    };

    // Проверка циклических зависимостей
    bool WouldIntroduceCircularDependency(const std::vector<Position>& new_references) const;
    void UpdateDependencies(const std::vector<Position>& new_references);
};