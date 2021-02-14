#pragma once

#include <string>
#include <vector>


class Board
{
    using board_type = std::vector<std::vector<unsigned>>;
    board_type m_board;
    board_type m_goal_board;
    std::vector<std::pair<int, int>> m_pos_elements;
    std::vector<std::pair<int, int>> m_pos_goal_elements;
    void fill_m_pos_goal_elements(unsigned size) {
        if (size > 0) {
            m_pos_goal_elements.emplace_back(size - 1, size - 1);
            for (unsigned i = 0; i < size; i++) {
                for (unsigned j = 0; j < size; j++) {
                    m_pos_goal_elements.emplace_back(i, j);
                }
            }
            m_pos_goal_elements.pop_back();
        }
    }
    void fill_m_pos_elements();
    static board_type create_goal_(const unsigned size);
public:
    static Board create_goal(unsigned size);

    Board() = default;

    Board(const Board & other) = default;

    Board & operator = (const Board & other) = default;

    explicit Board(unsigned size);

    explicit Board(const std::vector<std::vector<unsigned>> & data);

    std::size_t size() const;

    bool is_goal() const;

    unsigned hamming() const;

    unsigned manhattan() const;

    std::string to_string() const;

    bool is_solvable() const;

    friend bool operator == (const Board & lhs, const Board & rhs)
    { return lhs.m_board == rhs.m_board; }

    friend bool operator != (const Board & lhs, const Board & rhs)
    { return !(lhs == rhs); }

    // TODO:
    const std::vector<unsigned> & operator [] (std::size_t i) const;

    friend std::ostream & operator << (std::ostream & out, const Board & board)
    { return out << board.to_string(); }

    Board move_left();
    Board move_right();
    Board move_up();
    Board move_down();
    bool is_good_shift_pos(int shift_line, int shift_column);
    Board move_ij(int i, int j);
};
