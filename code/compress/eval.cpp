#include "eval.hpp"
#include "bictl.hpp"

double CountingEval::eval(const ChessBox& cb, int p) const {
    int m = cb.countPieces(p);
    int o = cb.countPieces(!p);
    if (m > o) {
        return (100 * m) / (m + o) ;
    } else if (m < 0){
        return -(100 * o) / (m + o);
    } else {
        return 0;
    }
}

double TableEval::eval(const ChessBox& cb, int p) const {
    double ret = 0;
    for (int i = 0; i < 64; ++i) {
        if (cb.isBlackPiece(i)) {
            ret += VALUE[i];
        } else if (cb.isWhitePiece(i)) {
            ret -= VALUE[i];
        }
    }
    if (p == BLACK_ID) {
        return ret;
    } else {
        return -ret;
    }
}

double CornerEval::eval(const ChessBox& cb, int p) const {
    int my = 0, op = 0;
    my += cb.isBlackPiece(cb.TO_SQUARE(0, 0));
    op += cb.isWhitePiece(cb.TO_SQUARE(0, 0));
    my += cb.isBlackPiece(cb.TO_SQUARE(0, 7));
    op += cb.isWhitePiece(cb.TO_SQUARE(0, 7));
    my += cb.isBlackPiece(cb.TO_SQUARE(7, 0));
    op += cb.isWhitePiece(cb.TO_SQUARE(7, 0));
    my += cb.isBlackPiece(cb.TO_SQUARE(7, 7));
    op += cb.isWhitePiece(cb.TO_SQUARE(7, 7));
    double c = 25 * (my - op);

    if (p == BLACK_ID) {
        return c; // 80
    } else {
        return -c;
    }
}

double CloseCornerEval::eval(const ChessBox& cb, int p) const {
    double my = 0; 
    double op = 0;
    if (cb.isEmpty(cb.TO_SQUARE(0, 0))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(0, 1));
        op += cb.isWhitePiece(cb.TO_SQUARE(0, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 1));
        op += cb.isWhitePiece(cb.TO_SQUARE(1, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 0));
        op += cb.isWhitePiece(cb.TO_SQUARE(1, 0));
    }
    if (cb.isEmpty(cb.TO_SQUARE(0, 7))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(0, 6));
        op += cb.isWhitePiece(cb.TO_SQUARE(0, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 6));
        op += cb.isWhitePiece(cb.TO_SQUARE(1, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 7));
        op += cb.isWhitePiece(cb.TO_SQUARE(1, 7));
    }
    if (cb.isEmpty(cb.TO_SQUARE(7, 7))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 6));
        op += cb.isWhitePiece(cb.TO_SQUARE(6, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 7));
        op += cb.isWhitePiece(cb.TO_SQUARE(6, 7));
        my += cb.isBlackPiece(cb.TO_SQUARE(7, 6));
        op += cb.isWhitePiece(cb.TO_SQUARE(7, 6));
    }
    if (cb.isEmpty(cb.TO_SQUARE(7, 0))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 0));
        op += cb.isWhitePiece(cb.TO_SQUARE(6, 0));
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 1));
        op += cb.isWhitePiece(cb.TO_SQUARE(6, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(7, 1));
        op += cb.isWhitePiece(cb.TO_SQUARE(7, 1));
    }
    double l = -12.5 * (my - op);
    if (p == BLACK_ID) {
        return l; // 38
    } else {
        return -l;
    }

}

double FrontEval::eval(const ChessBox& cb, int p) const {
    u64 f = cb.getFrontier();
    u64 my = cb.__getBoard(p);
    u64 otr = cb.__getBoard(!p);
    double my_b = __builtin_popcountll(f & my);
    double otr_b = __builtin_popcountll(f & otr);
    if (my_b > otr_b) {
        return (-100 * my_b) / (my_b + otr_b);
    } else if (my_b < otr_b) {
        return (100 * otr_b) / (my_b + otr_b);
    } else {
        return 0;
    }
}

double AllInOneEval::eval(const ChessBox& cb, int p) const {
    double ret = 0;
    for (int i = 0; i < evals.size(); ++i) {
        ret += evals[i]->eval(cb, p) * coefficients[i];
    }
    // for (const auto& eval: evals) {
    //     ret += eval->eval(cb, p);
    // }
    return ret;
}

void AllInOneEval::detailEval(const ChessBox& cb, int p) const {
    double ret;
    for (int i = 0; i < evals.size(); ++i) {
        ret = evals[i]->eval(cb, p) * coefficients[i];
        cout << ret << " ";
    }
    // for (const auto& eval: evals) {
    //     ret = eval->eval(cb, p);
    //     cout << ret << " ";
    // }
    cout << endl;
}

double ActionEval::eval(const ChessBox& cb, int p) const {
    double my = __builtin_popcountll(cb.getMovable(p));
    double other = __builtin_popcountll(cb.getMovable(!p));
    if (my > other) {
        return 100 * my / (my + other);
    } else if (my < other) {
        return -100 * other / (my + other);
    } else {
        return 0;
    }
}

double BorderEval::eval(const ChessBox& cb, int p) const {
    u64 me = cb.__getBoard(p) & BORDER;
    u64 opp = cb.__getBoard(!p) & BORDER;
    int me_bonus = __builtin_popcountll(me);
    int opp_bonus = __builtin_popcountll(opp);
    if (me_bonus == opp_bonus) return 0;
    if (me_bonus > opp_bonus) {
        return 100 * me_bonus / (me_bonus + opp_bonus);
    } else {
        return -100 * opp_bonus / (me_bonus + opp_bonus);
    }
}

#define HALF_STABLE_HELPER(dir, who) \
    tmp = who & BORDER; \
    for (int i = 0; i < 3; ++i) { \
        tmp |= dir(tmp) & who; \
    } \
    ret |= tmp;

double HalfStableEval::eval(const ChessBox& cb, int p) const {
    // p first
    u64 cur = cb.__getBoard(p);
    u64 tmp, ret = 0;
    HALF_STABLE_HELPER(N, cur);
    HALF_STABLE_HELPER(S, cur);
    HALF_STABLE_HELPER(E, cur);
    HALF_STABLE_HELPER(W, cur);
    HALF_STABLE_HELPER(NE, cur);
    HALF_STABLE_HELPER(NW, cur);
    HALF_STABLE_HELPER(SE, cur);
    HALF_STABLE_HELPER(SW, cur);
    int my_bonus = __builtin_popcountll(ret);

    u64 opp = cb.__getBoard(!p);
    ret = 0;
    HALF_STABLE_HELPER(N, opp);
    HALF_STABLE_HELPER(S, opp);
    HALF_STABLE_HELPER(E, opp);
    HALF_STABLE_HELPER(W, opp);
    HALF_STABLE_HELPER(NE, opp);
    HALF_STABLE_HELPER(NW, opp);
    HALF_STABLE_HELPER(SE, opp);
    HALF_STABLE_HELPER(SW, opp);
    int opp_bonus = __builtin_popcountll(ret);
    return 10 * (my_bonus - opp_bonus);
}