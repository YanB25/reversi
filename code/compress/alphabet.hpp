#ifndef ALPHA_BET_HPP
#define ALPHA_BET_HPP

#include "chessbox.hpp"
#include <vector>
using namespace std;

class Solution {
public:
    Solution() = default;
    virtual Position solve(int p) const = 0;
    virtual vector<Position> n_solve(int n, int p) const = 0;
    virtual ~Solution();
};
class BaseSolution : public Solution{
public:
    BaseSolution(const ChessBox& cb): chessbox(cb) {}
protected:
    const ChessBox& chessbox;
};

class AlphaBetaSolve : public BaseSolution {
public:
    AlphaBetaSolve(const ChessBox& cb): BaseSolution(cb) {}
    double alphabeta(ChessBox cb, int depth, double alpha, double beta, int p) const;
    virtual Position solve(int p) const;
    virtual vector<Position> n_solve(int n, int p) const;
    // virtual vector<Position> solve_candidates(int p) const;
    virtual ~AlphaBetaSolve();
};
#endif