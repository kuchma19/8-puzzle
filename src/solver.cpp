#include "solver.h"
#include <algorithm>
#include <unordered_map>

using std::vector;

Solver::Solver(const Board & board)
{
    bool is_goal = board.is_goal();
    if (!is_goal && board.is_solvable()) {
        vector<Board> number_board;
        std::unordered_map<int, bool> used_board;
        vector<int> parent;
        vector<int> queue;
        vector<int> cost_road_start_to_number_board;
        number_board.emplace_back(board);
        queue.push_back(0);
        cost_road_start_to_number_board.push_back(0);
        parent.push_back(0);
        int finish_board = -1;
        while(!queue.empty()) {
            int current = -1;
            unsigned min_manhattan = std::numeric_limits<unsigned>::max();
            std::size_t index_current = -1;
            for (std::size_t i = 0; i < queue.size(); i++) {
                int v = queue[i];
                if (number_board[v].manhattan() < min_manhattan) {
                    current = v;
                    min_manhattan = number_board[v].manhattan();
                    index_current = i;
                }
            }
            if (number_board[current].is_goal()) {
                finish_board = current;
                break;
            }
            std::swap(queue[index_current], queue.back());
            queue.pop_back();
            used_board[current] = true;
            for (const auto [di, dj] : good_moves) {
                if (number_board[current].is_good_shift_pos(di, dj)) {
                    Board u_board = number_board[current].move_ij(di, dj);
                    auto it = std::find(number_board.begin(), number_board.end(), u_board);
                    int number_board_to_queue = -1;
                    if (it == number_board.end()) {
                        number_board.emplace_back(u_board);
                        cost_road_start_to_number_board.push_back(cost_road_start_to_number_board[current] + 1);
                        number_board_to_queue = static_cast<int>(number_board.size()) - 1;
                        parent.push_back(current);
                    } else {
                        number_board_to_queue = static_cast<int>(it - number_board.begin());
                    }
                    int tentativeScore = cost_road_start_to_number_board[current] + 1;
                    if (used_board[number_board_to_queue] && tentativeScore >= cost_road_start_to_number_board[number_board_to_queue]) {
                        continue;
                    }
                    if (!used_board[number_board_to_queue] || tentativeScore < cost_road_start_to_number_board[number_board_to_queue]) {
                        parent[number_board_to_queue] = current;
                        cost_road_start_to_number_board[number_board_to_queue] = tentativeScore;
                        bool in_queue = false;
                        for (int i : queue) {
                            if (i == number_board_to_queue) {
                                in_queue = true;
                                break;
                            }
                        }
                        if (!in_queue) {
                            queue.push_back(number_board_to_queue);
                        }
                    }

                }
            }

        }

        for (int t = finish_board; t != parent[t]; t = parent[t]) {
            m_moves.emplace_back(number_board[t]);
        }
        m_moves.emplace_back(board);
        std::reverse(m_moves.begin(), m_moves.end());
    } else {
        if (is_goal) {
            m_moves.resize(1, board);
        } else {
            m_moves.resize(0);
        }
    }
}

std::size_t Solver::moves() const
{
    if (m_moves.size() == 0) {
        return m_moves.size();
    }
    else {
        return m_moves.size() - 1;
    }
}
