#ifndef ALPHA_BET_HPP
#define ALPHA_BET_HPP

#include <vector>
#include "chessbox.hpp"
#include "eval.hpp"
using namespace std;

class Solution {
public:
    Solution() = default;
    virtual Position solve(int p, int depth, bool) const = 0;
    virtual vector<Position> n_solve(int n, int p, int depth, bool) const = 0;
    virtual ~Solution();
};
class BaseSolution : public Solution{
public:
    BaseSolution(const EvalBase& eval, const ChessBox& cb): eval(eval), chessbox(cb) {}
protected:
    const EvalBase& eval;
    const ChessBox& chessbox;
};

class AlphaBetaSolve : public BaseSolution {
public:
    AlphaBetaSolve(const EvalBase& eval, const ChessBox& cb): BaseSolution(eval, cb) {}
    double alphabeta(ChessBox cb, int depth, double alpha, double beta, int p, bool trunc=true) const;
    virtual Position solve(int p, int depth = 8, bool trunc=true) const;
    virtual vector<Position> n_solve(int n, int p, int depth = 8, bool trunc=true) const;
    // virtual vector<Position> solve_candidates(int p) const;
    virtual ~AlphaBetaSolve();
};
#endif