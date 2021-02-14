#pragma once

#include "board.h"

class Solver
{
    constexpr static std::initializer_list<std::pair<int, int>> good_moves = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
public:
    explicit Solver(const Board & board);

    Solver(const Solver & other) = default;

    Solver & operator = (const Solver & other) = default;

    std::size_t moves() const;

    auto begin() const
    { return m_moves.begin(); }

    auto end() const
    { return m_moves.end(); }

private:
    // FIXME
    std::vector<Board> m_moves;
};
