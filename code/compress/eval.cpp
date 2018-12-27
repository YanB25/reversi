#include "eval.hpp"

double CountingEval::eval(const ChessBox& cb, int p) const {
    int m = cb.countPieces(p);
    int o = cb.countPieces(!p);
    if (m > o) {
        return 10 * (100 * m) / (m + o) ;
    } else if (m < 0){
        return 10 * -(100 * o) / (m + o);
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

    my = 0; op = 0;
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
        return 80 * c + 38 * l;
    } else {
        return -(80 * c + 38 * l);
    }
}

double FrontEval::eval(const ChessBox& cb, int p) const {
    u64 f = cb.getFrontier();
    u64 my = cb.__getBoard(p);
    u64 otr = cb.__getBoard(!p);
    double my_b = __builtin_popcountll(f & my);
    double otr_b = __builtin_popcountll(f & otr);
    if (my_b > otr_b) {
        return 7 *(-100 * my_b) / (my_b + otr_b);
    } else if (my_b < otr_b) {
        return 7 * (100 * otr_b) / (my_b + otr_b);
    } else {
        return 0;
    }
}

double AllInOneEval::eval(const ChessBox& cb, int p) const {
    double ret = 0;
    for (const auto& eval: evals) {
        ret += eval->eval(cb, p);
    }
    return ret;
}

void AllInOneEval::detailEval(const ChessBox& cb, int p) const {
    double ret;
    for (const auto& eval: evals) {
        ret = eval->eval(cb, p);
        cout << ret << " ";
    }
    cout << endl;
}

double ActionEval::eval(const ChessBox& cb, int p) const {
    double my = __builtin_popcountll(cb.getMovable(p));
    double other = __builtin_popcountll(cb.getMovable(!p));
    if (my > other) {
        return 8 * 100 * my / (my + other);
    } else if (my < other) {
        return 8 * -100 * other / (my + other);
    } else {
        return 0;
    }
}
