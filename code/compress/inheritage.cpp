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

int start = 63;
const int GAME = 5;
const int ROUND = 2;
const int ITERS = 4000;
// vector<double> initParams{1, 80, 10, 8, 7, 38}; // standard
vector<double> initParams{3.839793, 105.464095, 12.543550, 23.364513, 35.068640, 127.346112, 24.436968, 13.140249};
// 0.685248, 45.434353, 2.611930, 4.371898, 4.344627, 18.193775
// 1.327349, 88.602155, 6.009883, 6.683687, 8.286318, 30.431086
//  93:
//  0.938195, 194.651977, 5.535919, 14.761468, 11.102766, 37.021361
// 
//  0.990352, 183.959853, 4.315917, 12.575973, 14.143714, 32.74673
//  
// 1.713400, 117.945492, 4.606499, 10.064300, 16.278962, 58.631710, 33.117000, 32.795700
// 1.387683, 131.202565, 4.599129, 9.079005, 17.244304, 58.578941, 34.123757, 31.483872
// 3.839793, 105.464095, 12.543550, 23.364513, 35.068640, 127.346112, 24.436968, 13.140249
int best_record = -1;

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
    BorderEval border_eval;
    HalfStableEval half_stable_eval;
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
        eval.addEval(border_eval, __params[6]);
        eval.addEval(half_stable_eval, __params[7]);

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
    printf("iters, ids, wins, newrecord, p1, p2, p3, p4, p5, p6, p7, p8\n");
    for (int _ = start; _ < ITERS; ++_) {
        bool mutation = (rand() % 10) == 0;
        vector<thread> threads;

        param.clear();
        param.resize(GAME);
        param[0].assign(initParams.begin(), initParams.end());
        for (int i = 1; i < GAME; ++i) {
            vector<double> tmps(param[0].begin(), param[0].end());
            for (int k = 0; k < param[0].size(); ++k) {
                double percentage = 0;
                if (mutation) {
                    percentage = ((rand() % 84) - 34) * 0.01;
                } else {
                    percentage = ((rand() % 37) - 17) * 0.01;
                }
                tmps[k] *= 1 + percentage;
            }
            param[i].assign(tmps.begin(), tmps.end());
            for (auto& val : param[i]) {
                if (val > 1000) {
                    val = 1000;
                }
            }
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
        auto argmax = std::max_element(sumup.begin(), sumup.end());
        int argmax_idx = argmax - sumup.begin();

        for (int i = 0; i < GAME; ++i) {
            int is_best_record = 0;
            if (sumup[i] > best_record) {
                is_best_record = 1;
                best_record = sumup[i];
            }
            printf("%d, %d, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n", _, i, sumup[i], i == argmax_idx, param[i][0],
                param[i][1], param[i][2], param[i][3], param[i][4], param[i][5], param[i][6], param[i][7]);
            fflush(stdout);
        }

        initParams.assign(param[argmax_idx].begin(), param[argmax_idx].end());
    }
    cout << "here end" << endl;
}
