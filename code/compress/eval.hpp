#ifndef __EVAL_HPP_
#define __EVAL_HPP_
#include "chessbox.hpp"
#include <vector>
using std::vector;
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


class TableEval : public EvalBase {
public:
    TableEval(){}
    virtual double eval(const ChessBox& cb, int p) const;
    virtual ~TableEval() {}
private:
    const int VALUE[8*8] = {
    20, -3, 11,  8,  8, 11, -3, 20,
    -3, -7, -4,  1,  1, -4, -7, -3,
    11, -4,  2,  2,  2,  2, -4, 11,
     8,  1,  2, -3, -3,  2,  1,  8,
     8,  1,  2, -3, -3,  2,  1,  8,
    11, -4,  2,  2,  2,  2, -4, 11,
    -3, -7, -4,  1,  1, -4, -7, -3,
    20, -3, 11,  8,  8, 11, -3, 20,
    };
};

class CornerEval : public EvalBase {
public:
    CornerEval(){}
    virtual double eval(const ChessBox& cb, int p) const;
    virtual ~CornerEval() {}

};

class FrontEval : public EvalBase {
public:
    FrontEval(){}
    virtual double eval(const ChessBox& cb, int p) const;
    virtual ~FrontEval() {}
};

class AllInOneEval : public EvalBase {
public:
    AllInOneEval() {}
    virtual double eval(const ChessBox& cb, int p) const;
    void detailEval(const ChessBox& cb, int p) const;
    virtual ~AllInOneEval() {}
    void addEval(EvalBase& eval, double co) {
        evals.push_back(&eval);
        coefficients.push_back(co);
    }
    int size() const { return evals.size(); }
    void set(int idx, double val) { coefficients[idx] = val; }
    double get(int idx) const { return coefficients[idx]; }
    vector<double> getParams() const { return coefficients; }
    void setParams(const vector<double>& params) {
        coefficients.assign(params.cbegin(), params.cend());
    }
private:
    vector<EvalBase*> evals;
    vector<double> coefficients;
};


class ActionEval : public EvalBase {
public:
    ActionEval() {}
    virtual double eval(const ChessBox& cb, int p) const;
    virtual ~ActionEval() {}
private:
    vector<EvalBase*> evals;
};


#endif