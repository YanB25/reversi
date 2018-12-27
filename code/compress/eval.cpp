#include "eval.hpp"

double CountingEval::eval(const ChessBox& cb, int p) const {
    return cb.countPieces(p) - cb.countPieces(!p);
}