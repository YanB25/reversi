#include "alphabet.hpp"
#define INF (1e8)
// 假设估值函数永远以黑子ß为先地估值
Position AlphaBetaSolve::solve(int p) const {
    alphabeta(chessbox, 8, -INF, INF, !p);
}

double AlphaBetaSolve::alphabeta(ChessBox cb, int depth, int alpha, int beta, int player) {
    if (depth == 0) {
        // TODO: 
        return 0;
    }
    if (player == WHITE_ID) {
        int v = INF;
        int moves;
    }


}