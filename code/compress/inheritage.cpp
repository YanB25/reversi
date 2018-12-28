#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <mutex>
#include <algorithm>
#include "chessbox.hpp"
#include "alphabet.hpp"
#include "eval.hpp"

using namespace std;

const int GAME = 3;
const int ROUND = 2;
const int ITERS = 3;
// int initParams[] = {1, 1, 10, 8, 7};
vector<double> initParams{1, 80, 10, 8, 7, 38};

void print(const vector<double>& p) {
    for (int i = 0; i < p.size(); ++i) {
        cout << p[i] << " ";
    }
    cout << endl;
}

struct EvalGroup {
    TableEval table_eval;
    CornerEval corner_eval;
    CountingEval counting_eval;
    ActionEval action_eval;
    FrontEval front_eval;
    CloseCornerEval close_corner_eval;
    AllInOneEval eval;
    EvalGroup(vector<double> params) {
        __params = params;
    }
    void init() {
        eval.addEval(table_eval, __params[0]);
        eval.addEval(corner_eval, __params[1]);
        eval.addEval(counting_eval, __params[2]);
        eval.addEval(action_eval, __params[3]);
        eval.addEval(front_eval, __params[4]);
        eval.addEval(close_corner_eval, __params[5]);

    }
    double get(int idx) const {
        return eval.get(idx);
    }

    void set(int idx, double val) {
        eval.set(idx, val);
    }

    vector<double> getParams() const {
        return eval.getParams();
    }
    void setParams(const vector<double>& params) {
        eval.setParams(params);
    }
    vector<double> __params;
};

vector<vector<double>> param;
vector<ChessBox> chessboxes;
vector<EvalGroup> evalGroups;
vector<vector<int>> win_lose;
std::mutex g_win_lose_lock;

void self_play(int i, int j) {
    // cout << "hello from " << i << " " << j << endl;
    for (int r = 0; r < ROUND; ++r) {
        ChessBox chessbox(BLACK_ID);
        chessbox.defaultInit();
        AlphaBetaSolve ab_left(evalGroups[i].eval, chessbox);
        AlphaBetaSolve ab_right(evalGroups[j].eval, chessbox);
        while(!chessbox.isEnd()) {
            auto p1 = ab_left.solve(BLACK_ID, 6);
            chessbox.drop(p1.x, p1.y, BLACK_ID);
            // printf("(%d, %d) r:%d\n", i, j, r);
            // cout << chessbox << endl;
            auto p2 = ab_right.solve(WHITE_ID, 6);
            chessbox.drop(p2.x, p2.y, WHITE_ID);
            // printf("(%d, %d) r:%d\n", i, j, r);
            // cout << chessbox << endl;
        }
        int black = chessbox.countBlackPieces();
        int white = chessbox.countWhitePieces();

        g_win_lose_lock.lock();
        win_lose[i][j] += black - white;
        // cout << i << " " << j << " " << black-white << endl;
        win_lose[j][i] += white - black;
        // cout << win_lose[j][i] << endl;
        g_win_lose_lock.unlock();
    }

}
int main() {
    srand(time(0));
    printf("iters, ids, wins, newrecord, p1, p2, p3, p4, p5\n");
    for (int _ = 0; _ < ITERS; ++_) {
        vector<thread> threads;

        param.clear();
        param.resize(GAME);
        param[0].assign(initParams.begin(), initParams.end());
        for (int i = 1; i < GAME; ++i) {
            vector<double> tmps(param[0].begin(), param[0].end());
            for (int k = 0; k < param[0].size(); ++k) {
                double percentage = ((rand() % 30) - 15) * 0.01;
                tmps[k] *= 1 + percentage;
            }
            param[i].assign(tmps.begin(), tmps.end());
        }

        win_lose.clear();
        win_lose.resize(GAME, vector<int>(GAME));
        
        evalGroups.clear();
        for (int i = 0; i < GAME; ++i) {
            evalGroups.push_back(EvalGroup(param[i]));
        }
        for (int i = 0; i < GAME; ++i) {
            evalGroups[i].init();
        }

        threads.clear();
        for (int i = 0; i < GAME; ++i) {
            for (int j = 0; j < GAME; ++j) {
                if (i != j) {
                    threads.push_back(thread(self_play, i, j));
                }
            }
        }

        for (auto& thread: threads) {
            thread.join();
        }
        // cout << "====== show result ======" << endl;
        vector<int> sumup(GAME);
        for (int i = 0; i < GAME; ++i) {
            for (int j = 0; j < GAME; ++j) {
                // cout << win_lose[i][j] << " ";
                sumup[i] += win_lose[i][j];
            }
            // cout << endl;
        }
        // cout << endl;
        // for (int i = 0; i < GAME; ++i) {
        //     print(param[i]);
        // }
        // cout << endl;
        // cout << "wins" << endl;
        // for (int i = 0; i < GAME; ++i) {
        //     cout << sumup[i] << " ";
        // }
        // cout << endl;
        for (int i = 0; i < GAME; ++i) {
            printf("%d, %d, %d, %d, %lf, %lf, %lf, %lf, %lf\n", _, i, sumup[i], 0, param[i][0],
                param[i][1], param[i][2], param[i][3], param[i][4]);
            fflush(stdout);
        }

        auto argmax = std::max_element(sumup.begin(), sumup.end());
        int argmax_idx = argmax - sumup.begin();
        initParams.assign(param[argmax_idx].begin(), param[argmax_idx].end());
    }
}
