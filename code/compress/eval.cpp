#include "eval.hpp"

double CountingEval::eval(const ChessBox& cb, int p) const {
    int m = cb.countPieces(p);
    int o = cb.countPieces(!p);
    return (100 * m) / (m + o) ;
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
    return ret;
}

double CornerEval::eval(const ChessBox& cb, int p) const {
    int my = 0, op = 0;
    my += cb.isBlackPiece(cb.TO_SQUARE(0, 0));
    op += cb.isBlackPiece(cb.TO_SQUARE(0, 0));
    my += cb.isBlackPiece(cb.TO_SQUARE(0, 7));
    op += cb.isBlackPiece(cb.TO_SQUARE(0, 7));
    my += cb.isBlackPiece(cb.TO_SQUARE(7, 0));
    op += cb.isBlackPiece(cb.TO_SQUARE(7, 0));
    my += cb.isBlackPiece(cb.TO_SQUARE(7, 7));
    op += cb.isBlackPiece(cb.TO_SQUARE(7, 7));
    double c = 25 * (my - op);

    my = 0; op = 0;
    if (cb.isEmpty(cb.TO_SQUARE(0, 0))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(0, 1));
        op += cb.isBlackPiece(cb.TO_SQUARE(0, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 1));
        op += cb.isBlackPiece(cb.TO_SQUARE(1, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 0));
        op += cb.isBlackPiece(cb.TO_SQUARE(1, 0));
    }
    if (cb.isEmpty(cb.TO_SQUARE(0, 7))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(0, 6));
        op += cb.isBlackPiece(cb.TO_SQUARE(0, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 6));
        op += cb.isBlackPiece(cb.TO_SQUARE(1, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(1, 7));
        op += cb.isBlackPiece(cb.TO_SQUARE(1, 7));
    }
    if (cb.isEmpty(cb.TO_SQUARE(7, 7))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 6));
        op += cb.isBlackPiece(cb.TO_SQUARE(6, 6));
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 7));
        op += cb.isBlackPiece(cb.TO_SQUARE(6, 7));
        my += cb.isBlackPiece(cb.TO_SQUARE(7, 6));
        op += cb.isBlackPiece(cb.TO_SQUARE(7, 6));
    }
    if (cb.isEmpty(cb.TO_SQUARE(7, 0))) {
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 0));
        op += cb.isBlackPiece(cb.TO_SQUARE(6, 0));
        my += cb.isBlackPiece(cb.TO_SQUARE(6, 1));
        op += cb.isBlackPiece(cb.TO_SQUARE(6, 1));
        my += cb.isBlackPiece(cb.TO_SQUARE(7, 1));
        op += cb.isBlackPiece(cb.TO_SQUARE(7, 1));
    }
    double l = -12.5 * (my - op);
    return 80 * c + 38 * l;
}

double AllInOneEval::eval(const ChessBox& cb, int p) const {
    double ret = 0;
    for (const auto& eval: evals) {
        ret += eval->eval(cb, p);
    }
    return ret;
}