#include "solution.hpp"
#include <algorithm>
// using std::random_shuffle;
class SPCom {
public:
    SPCom(int size): __size(size) {}
    bool operator()(const Position& lhs, const Position& rhs) const {
        bool lft = lhs.first == 0 || lhs.first == __size-1 || lhs.second == 0 || lhs.second == __size-1;
        bool rgt = rhs.first == 0 || rhs.first == __size-1 || rhs.second == 0 || rhs.second == __size - 1;
        return lft && !rgt;
        return false;
    }
private:
    const int __size;
};

Solution::Solution(const ChessBox& cb): chessbox(cb){}

RandomSolution::RandomSolution(const ChessBox& cb, const Evaluation& eval) : Solution(cb){}

Position RandomSolution::solve(char role) const {
    assert(role == BLACK || role == WHITE);
    auto dropable = chessbox.Dropable(role);
    std::random_shuffle(dropable.begin(), dropable.end());
    if (dropable.size()) {
        return dropable[0];
    } else {
        return Position(-1, -1);
    }
}

MiniMaxSolution::MiniMaxSolution(const ChessBox& cb, const Evaluation& eval, int depth)
    : Solution(cb), __depth(depth), __eval(eval) {}

Position MiniMaxSolution::solve(char role) const {
    vector<Position> dropables = chessbox.Dropable(role);

    auto spcom = SPCom(chessbox.size());
    std::sort(dropables.begin(), dropables.end(), spcom);

    if (dropables.size() == 0) {
        return Position(-1, -1);
    }
    double alpha = -1e8;
    double beta = 1e8;
    vector<double> values;
    for (const auto& p : dropables) {
        double val = __solve(chessbox, p, role, alpha, beta, 0);
        // max node, update alpha
        alpha = std::max(alpha, val);
        values.push_back(val);
    }

    int idx = -1;
    int maxval = -1e8;
    // try to get max
    for (int i = 0; i < values.size(); ++i) {
        if (values[i] > maxval) {
            idx = i;
            maxval = values[i];
        }
    }
    return dropables[idx];
}

double SimpleEval::evaluate(const ChessBox& chessbox, char role) const {
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;
    int size = chessbox.size();
    int eval = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int factor;
            if (chessbox.val(i, j) == role) {
                factor = 1;
            } else if (chessbox.val(i, j) == otherRole) {
                factor = -1;
            } else {
                factor = 0;
            } 
            eval += factor;
            if (i == 0 || j == 0) {
                eval += size * factor;
            }
            if (i == size-1 || j == size-1) {
                eval += size * factor;
            }
        }
    }
    return eval;
}

double NaiveEval::evaluate(const ChessBox& cb, char role) const {
    double init = 0;
    char otherRole = role == BLACK ? WHITE : BLACK;
    for (int i = 0; i < cb.size(); ++i) {
        for (int j = 0; j < cb.size(); ++j) {
            char m = cb.val(i,j);
            if (m == role) {
                init += 1;
            }
            if (m == otherRole) {
                init -= 1;
            }
        }
    }
    return init;
}

double MiniMaxSolution::__solve(const ChessBox& cb, const Position& position, char role, double alpha, double beta, int depth) const {
    // this position has not been dropped
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    if (alpha >= beta) {
        printf("ERROR alpha %lf beta %lf\n", alpha, beta);
    }
    assert(alpha < beta);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;

    // recursive base
    if (depth >= __depth) {
        // should return.
        return evaluation(cb);
    }


    ChessBox new_chess_box = cb;
    new_chess_box.Drop(position.first, position.second, role);

    double pivot;
    if (depth % 2 == 0) {
        // min node
        pivot = 1e8;
    } else {
        pivot = -1e8;
    }

    vector<Position> dropables = new_chess_box.Dropable(otherRole);
    auto spcom = SPCom(cb.size());
    std::sort(dropables.begin(), dropables.end(), spcom);

    // vector<double> values;
    if (dropables.size() == 0) {
        return evaluation(cb);
    }
    for (const auto& p : dropables) {
        double val = __solve(new_chess_box, p, otherRole, alpha, beta, depth + 1);
        if (depth % 2 == 0) {
            // min node, update beta
            beta = std::min(beta, val);
            pivot = std::min(pivot, val);
            if (alpha >= beta) {
                break;
            }
        } else {
            // max node, update alpha
            alpha = std::max(alpha, val);
            pivot = std::max(pivot, val);
            if (alpha >= beta) {
                break;
            }
        }
        // values.push_back(val);
    }
    return pivot;

    // if (depth % 2 == 0) {
    //     // min
    //     double pivot = 1e8;
    //     for (int i = 0; i < values.size(); ++i) {
    //         if (values[i] < pivot) {
    //             pivot = values[i];
    //         }
    //     }
    //     return pivot;
    // } else {
    //     // max
    //     double pivot = -1e8;
    //     for (int i = 0; i < values.size(); ++i) {
    //         if (values[i] > pivot) {
    //             pivot = values[i];
    //         }
    //     }
    //     return pivot;
    // }
    // assert(false);
    // return 0;
}

double ActionPressEval::evaluate(const ChessBox& chessbox, char role) const {
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;
    int size = chessbox.size();
    int eval = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int factor;
            if (chessbox.val(i, j) == role) {
                factor = 1;
            } else if (chessbox.val(i, j) == otherRole) {
                factor = -1;
            } else {
                factor = 0;
            } 
            eval += factor;
            if (i == 0 || j == 0) {
                eval += size * factor;
            }
            if (i == size-1 || j == size-1) {
                eval += size * factor;
            }
        }
    }
    eval += chessbox.Dropable(role).size() ;
    eval -= chessbox.Dropable(otherRole).size() ;
    return eval;
}


double OnlyDroppableEval::evaluate(const ChessBox& chessbox, char role) const {
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;
    int size = chessbox.size();
    int eval = 0;
    eval += chessbox.Dropable(role).size() ;
    eval -= chessbox.Dropable(otherRole).size() ;
    return eval;
}

double OnlyDroppableEnhanceEval::evaluate(const ChessBox& chessbox, char role) const {
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;
    int size = chessbox.size();
    int eval = 0;
    eval += chessbox.Dropable(role).size() ;
    int os = chessbox.Dropable(otherRole).size();
    eval -=  os * 3;
    if (os == 0) {
        eval += 1000;
    } else if (os == 1) {
        eval += 10;
    } else if (os == 2) {
        eval += 5;
    }
    return eval;
}

TableEval::TableEval(){
    double tmp[] = {
        1000, -50, 25, 10, 10, 25, -50, 1000,
        -50, -100, 1,   1,  1,  1, -100, -50,
        25, 1,     2,   1,  1,  1,   1,   25,
        10, 1,     1,   1,  1,  1,   1,   10,
        10, 1,     1,   1,  1,  1,   1,   10,
        25, 1,     2,   1,  1,  1,   1,   25,
        -50, -100, 1,   1,  1,  1, -100, -50,
        1000, -50, 25, 10, 10, 25, -50, 1000
    };
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            eval_table[i][j] = tmp[i * 8 + j];
        }
    }
}
double TableEval::evaluate(const ChessBox& chessbox, char role) const {
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    char otherRole = role == BLACK ? WHITE : BLACK;
    int size = chessbox.size();
    int eval = 0;
    eval += chessbox.Dropable(role).size() ;
    int os = chessbox.Dropable(otherRole).size();
    eval -=  os * 3;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (chessbox.val(i, j) == role) {
                eval += eval_table[i][j];
            } else if (chessbox.val(i, j) == otherRole) {
                eval -= eval_table[i][j];
            }
        }
    }
    return eval;
}