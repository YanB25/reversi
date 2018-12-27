#include "alphabet.hpp"
#include <algorithm>
using std::max;
using std::min;
#define INF (1e8)

Solution::~Solution(){}

// 假设估值函数永远以黑子为先地估值
vector<Position> AlphaBetaSolve::n_solve(int n, int p, int depth, bool trunc) const {
    vector<int> moves = chessbox.movessq(p);
    vector<Position> candidates;
    int target_num = max((unsigned long)(moves.size() * 0.7), 3ul);

    if (trunc && (int)moves.size() > target_num) {
        vector<Position> prefilters = n_solve(n, p, depth / 2, false);
        moves.clear();
        for (int i = 0; i < target_num; ++i) {
            moves.push_back(prefilters[i].x * 8 + prefilters[i].y);
        }
    }

    for (int move : moves) {
        ChessBox ncb(chessbox);
        ncb.drop(move / 8, move % 8, p);
        int val = alphabeta(ncb, depth,  -INF, INF, !p);
        candidates.emplace_back(move/8, move%8, val);
    }
    if (p == BLACK_ID) {
        sort(candidates.begin(), candidates.end(), blackPosCmp);
    } else {
        assert(p == WHITE_ID);
        sort(candidates.begin(), candidates.end(), whitePosCmp);
    }
    if ((int)candidates.size() > n) {
        candidates.resize(n);
    }
    return candidates;
}

Position AlphaBetaSolve::solve(int p, int depth, bool trunc) const {
    return n_solve(1, p, depth, trunc).front();
}

AlphaBetaSolve::~AlphaBetaSolve() {}

double AlphaBetaSolve::alphabeta(ChessBox cb, int depth, double alpha, double beta, int player, bool trunc) const {
    if (depth == 0) {
        return eval.eval(cb, BLACK_ID);
    }
    vector<int> movables = cb.movessq(player);
    double v;

    if (movables.empty()) {
        return eval.eval(cb, BLACK_ID);
    }

    // TODO: bug
    // // trying to trunc more
    // int target_num = max(3ul, movables.size() / 2);
    // vector<Position> candidates;
    // if (trunc && depth >= 6 && target_num > movables.size()) {
    //     for (int move: movables) {
    //         ChessBox ncb(chessbox);
    //         ncb.drop(move / 8, move % 8, player);
    //         double v = alphabeta(ncb, depth / 2, alpha, beta, !player, false);
    //         candidates.emplace_back(move/8, move%8, v);
    //     }

    //     if (player == BLACK_ID) {
    //         sort(candidates.begin(), candidates.end(), blackPosCmp);
    //     } else {
    //         assert(player == WHITE_ID);
    //         sort(candidates.begin(), candidates.end(), whitePosCmp);
    //     }
    //     movables.clear();

    //     for (int i = 0; i < target_num; ++i) {
    //         movables.push_back(candidates[i].x * 8 + candidates[i].y);
    //     }
    // }

    if (player == WHITE_ID) {
        v = INF;
        for (int move: movables) {
            ChessBox ncb(cb);
            // ncb.__place(move, player);
            ncb.drop(move / 8, move % 8, player);
            v = min(v, alphabeta(ncb, depth - 1, alpha, beta, !player));
            beta = min(beta, v);
            if (beta <= alpha) {
                break;
            }
        }
    } else {
        assert(player == BLACK_ID);
        v = -INF;
        for (int move : movables) {
            ChessBox ncb(cb);
            // ncb.__place(move, player);
            ncb.drop(move / 8, move % 8, player);
            v = max(v, alphabeta(ncb, depth - 1, alpha, beta, !player));
            alpha = max(alpha, v);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return v;
}

vector<int> AlphaBetaSolve::search_helper(const ChessBox& cb, int n, int player, int depth) const {
    vector<int> moves = cb.movessq(player);
    vector<Position> candidates;
    for (int move : moves) {
        ChessBox ncb(cb);
        ncb.drop(move/8, move%8, player);
        double ret = alphabeta(ncb, depth, -INF, INF, !player, false);
        candidates.emplace_back(move / 8, move % 8, ret);
    }
    if (player == BLACK_ID) {
        sort(candidates.begin(), candidates.end(), blackPosCmp);
    } else {
        assert(player == WHITE_ID);
        sort(candidates.begin(), candidates.end(), whitePosCmp);
    }
    vector<int> new_moves;
    for (int i = 0; i < n && i < candidates.size(); ++i) {
        new_moves.push_back(candidates[i].x * 8 + candidates[i].y);
    }
    return new_moves;
}