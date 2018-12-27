#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "blackwhite.hpp"
class Solution {
public:
    explicit Solution(const ChessBox&);
    virtual Position solve(char role) const = 0;
    virtual ~Solution() {};
protected:
    const ChessBox& chessbox;
};

class Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const = 0;
};

class SimpleEval : public Evaluation{
public:
    virtual double evaluate(const ChessBox&, char role) const;
};

class NaiveEval : public Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const;
};

class ActionPressEval : public Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const;
};

class OnlyDroppableEval : public Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const;
};

class OnlyDroppableEnhanceEval : public Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const;
};

class TableEval : public Evaluation {
public:
    virtual double evaluate(const ChessBox&, char role) const;
    TableEval();
private:
    double eval_table[8][8];
};

class MiniMaxSolution : public Solution{
public:
    explicit MiniMaxSolution(const ChessBox& cb, const Evaluation& eval, int depth = 10);
    virtual Position solve(char role) const;
    double __solve(const ChessBox& cb, const Position& p, char role, double alpha, double beta, int depth) const;
    virtual ~MiniMaxSolution() {}
    double evaluation(const ChessBox& cb) const {
        return __eval.evaluate(cb, cb.role());
    }
private:
    int __depth;
    const Evaluation& __eval;
};

class RandomSolution: public Solution {
public:
    explicit RandomSolution(const ChessBox&, const Evaluation& eval);
    virtual Position solve(char role) const;
    virtual ~RandomSolution() {}
private:
};

#endif