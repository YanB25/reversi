#ifndef ALPHA_BET_HPP
#define ALPHA_BET_HPP

#include "chessbox.hpp"
#include <vector>
using namespace std;

class Solution {
public:
    Solution(const Chessbox& cb): chessbox(cb) {}
    virtual Position solve(int p) const = 0;
    virtual vector<Position> solve_candidates(int p) const = 0;
    virtual ~Solution();
protected:
    const ChessBox& chessbox;
};

class AlphaBetaSolve : public Solution {
public:
    AlphaBetaSolve(const Chessbox& cb): Solution(cb) {}
    double alphabeta(ChessBox cb, int depth, int alpha, int beta, int p);
    virtual Position solve(int p) const;
    virtual vector<Position> solve_candidates(int p) const;
    virtual ~AlphaBetaSolve();
};
#endif