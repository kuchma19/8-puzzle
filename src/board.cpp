#include "board.h"

#include <cmath>
#include <random>
#include <algorithm>


static std::random_device rd;
static std::mt19937 gen(rd());


Board Board::create_goal(const unsigned size)
{
    return Board(create_goal_(size));
}

std::vector<std::vector<unsigned>> Board::create_goal_(const unsigned size) {
    board_type goal_board(size, std::vector<unsigned>(size));
    unsigned iter = 1;
    for (int i = 0; i < static_cast<int>(size); i++) {
        for (int j = 0; j < static_cast<int>(size); j++) {
            goal_board[i][j] = iter++;
        }
    }
    if (size > 0) {
        goal_board[static_cast<std::size_t>(size) - 1][static_cast<std::size_t>(size) - 1] = 0;
    }
    return goal_board;
}

Board::Board(const unsigned size) :
    m_board(size, std::vector<unsigned>(size)),
    m_goal_board(create_goal_(size))
{
    fill_m_pos_goal_elements(size);
    fill_m_pos_elements();
    for (int i = 0; i < static_cast<int>(m_pos_elements.size()); i++) {
        m_board[m_pos_elements[i].first][m_pos_elements[i].second] = i;
    }
}

Board::Board(const std::vector<std::vector<unsigned>> & data) : m_board(data)
{
    m_goal_board = create_goal_(data.size());
    fill_m_pos_goal_elements(data.size());
    m_pos_elements.resize(data.size() * data.size());
    for (int i = 0; i < static_cast<int>(data.size()); i++) {
        for (int j = 0; j < static_cast<int>(data.size()); j++) {
            m_pos_elements[m_board[i][j]] = std::make_pair(i, j);
        }
    }
}

std::size_t Board::size() const
{
    return m_board.size();
}

bool Board::is_goal() const
{
    return m_goal_board == m_board;
}

unsigned Board::hamming() const
{

    unsigned iter = 1;
    unsigned answer = 0;
    for (int i = 0; i < static_cast<int>(m_board.size()); i++) {
        for (int j = 0; j < static_cast<int>(m_board.size()); j++) {
            if (i == static_cast<int>(m_board.size()) - 1 && j == static_cast<int>(m_board.size()) - 1) {
                continue;
            }
            answer += static_cast<unsigned>(m_board[i][j] != iter++);
        }
    }
    if (static_cast<int>(m_board.size()) > 0) {
        answer += static_cast<unsigned>(m_board[static_cast<int>(m_board.size()) - 1][static_cast<int>(m_board.size()) - 1] != 0);
    }
    return answer;
}

unsigned Board::manhattan() const
{
    int answer = 0;
    for (std::size_t i = 1; i < m_pos_elements.size(); i++) {
        answer += std::abs(m_pos_elements[i].first - m_pos_goal_elements[i].first);
        answer += std::abs(m_pos_elements[i].second - m_pos_goal_elements[i].second);
    }
    return static_cast<unsigned>(answer);
}

std::string Board::to_string() const
{
    std::string string;
    for (int i = 0; i < static_cast<int>(m_board.size()); i++) {
        for (int j = 0; j < static_cast<int>(m_board.size()); j++) {
            string += std::to_string(m_board[i][j]) + " ";
        }
        string += '\n';
    }
    return string;
}

bool Board::is_solvable() const
{
    if (static_cast<int>(m_board.size()) == 0 || static_cast<int>(m_board.size()) == 1) {
        return true;
    }
    unsigned N = 0;
    std::vector<unsigned> permutation_board;
    permutation_board.reserve(m_board.size() * m_board.size());
    for (int i = 0; i < static_cast<int>(m_board.size()); i++) {
        for (int j = 0; j < static_cast<int>(m_board.size()); j++) {
            permutation_board.push_back(m_board[i][j]);
        }
    }
    for (std::size_t i = 0; i < permutation_board.size(); i++) {
        if (permutation_board[i] != 0) {
            for (std::size_t j = 0; j < i; j++) {
                if (permutation_board[j] > permutation_board[i]) {
                    N++;
                }
            }
        }
    }

    for (int i = 0; i < static_cast<int>(permutation_board.size()) && static_cast<int>(m_board.size()) % 2 == 0; i++) {
        if (permutation_board[i] == 0) {
            N += 1 + i / static_cast<int>(m_board.size());
        }
    }
    return N % 2 == 0;
}

const std::vector<unsigned> & Board::operator [] (const std::size_t i) const
{
    return m_board[i];
}

void Board::fill_m_pos_elements() {
    m_pos_elements.resize(m_board.size() * m_board.size());
    std::copy(m_pos_goal_elements.begin(), m_pos_goal_elements.end(), m_pos_elements.begin());
    std::shuffle(m_pos_elements.begin(), m_pos_elements.end(), gen);
}

bool Board::is_good_shift_pos(int shift_line, int shift_column) {
    std::pair<int, int> pos = std::make_pair(m_pos_elements[0].first + shift_line, m_pos_elements[0].second + shift_column);
    return pos.first >= 0 && pos.first < static_cast<int>(m_board.size()) && pos.second >= 0 && pos.second < static_cast<int>(m_board.size());
}

// i * i + j * j == 1 && is_good_shift_pos(i, j)
Board Board::move_ij(int i, int j) {
    board_type new_board = m_board;
    std::swap(new_board[m_pos_elements[0].first][m_pos_elements[0].second], new_board[m_pos_elements[0].first + i][m_pos_elements[0].second + j]);
    return Board(new_board);
}