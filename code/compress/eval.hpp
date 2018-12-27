#ifndef __EVAL_HPP_
#define __EVAL_HPP_
#include "chessbox.hpp"
class EvalBase {
public:
    EvalBase() {}
    virtual double eval(const ChessBox& cb, const int p) const = 0;
    virtual ~EvalBase() {}
protected:
};

class CountingEval : public EvalBase {
public:
    CountingEval(){}
    virtual double eval(const ChessBox& cb, int p) const;
    virtual ~CountingEval() {}
};
#endif