#include "alphabet.hpp"
#include <algorithm>
using std::max;
using std::min;
#define INF (1e8)

Solution::~Solution(){}

// 假设估值函数永远以黑子为先地估值
vector<Position> AlphaBetaSolve::n_solve(int n, int p) const {
    vector<int> moves = chessbox.movessq(p);
    vector<Position> candidates;
    for (int move : moves) {
        ChessBox ncb(chessbox);
        ncb.drop(move / 8, move % 8, p);
        int val = alphabeta(ncb, 8,  -INF, INF, !p);
        candidates.emplace_back(move/8, move%8, val);
    }
    sort(candidates.begin(), candidates.end());
    if ((int)candidates.size() > n) {
        candidates.resize(n);
    }
    return candidates;
}

Position AlphaBetaSolve::solve(int p) const {
    return n_solve(1, p).front();
}

AlphaBetaSolve::~AlphaBetaSolve() {}

double AlphaBetaSolve::alphabeta(ChessBox cb, int depth, double alpha, double beta, int player) const {
    if (depth == 0) {
        return eval.eval(cb, BLACK_ID);
    }
    vector<int> movables = cb.movessq(player);
    double v;

    if (movables.empty()) {
        return eval.eval(cb, BLACK_ID);
    }

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