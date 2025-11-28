#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>
#include <vector>

class Sheet : public SheetInterface {
public:
    Sheet();
    ~Sheet();

    void SetCell(Position pos, std::string text) override;
    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;
    void ClearCell(Position pos) override;
    Size GetPrintableSize() const override;
    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;


private:
    struct PositionHash {
        size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.row) ^ (std::hash<int>()(pos.col) << 1);
        }
    };

    struct PositionEqual {
        bool operator()(const Position& lhs, const Position& rhs) const {
            return lhs.row == rhs.row && lhs.col == rhs.col;
        }
    };

    using Table = std::unordered_map<Position, std::unique_ptr<Cell>, PositionHash, PositionEqual>;
    Table cells_;
    mutable std::optional<Size> cached_size_;

    // Вспомогательные методы
    void InvalidateCache();
    void UpdateSize() const;
    Size CalculateSize() const;
    
    // Проверка циклических зависимостей
    void CheckCircularDependency(Position pos, const std::vector<Position>& new_references) const;
    bool HasCycle(Position current, Position target, std::unordered_set<Position, PositionHash>& visited) const;
    
    Cell* GetConcreteCell(Position pos);
    const Cell* GetConcreteCell(Position pos) const;
};