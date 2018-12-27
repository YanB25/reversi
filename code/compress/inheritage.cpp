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
// int initParams[] = {1, 1, 10, 8, 7};
vector<double> initParams{1, 1, 10, 8, 7};

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
    AllInOneEval eval;
    EvalGroup(vector<double> params) {
        eval.addEval(table_eval, params[0]);
        eval.addEval(corner_eval, params[1]);
        eval.addEval(counting_eval, params[2]);
        eval.addEval(action_eval, params[3]);
        eval.addEval(front_eval, params[4]);

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
};

vector<vector<double>> param;
vector<ChessBox> chessboxes;
vector<EvalGroup> evalGroups;
vector<vector<int>> win_lose;
std::mutex g_win_lose_lock;

void self_play(int i, int j) {
    // cout << "hello from " << i << " " << j << endl;
    ChessBox chessbox(BLACK_ID);
    chessbox.defaultInit();
    AlphaBetaSolve ab_left(evalGroups[i].eval, chessbox);
    AlphaBetaSolve ab_right(evalGroups[j].eval, chessbox);

    for (int r = 0; r < ROUND; ++r) {
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
        cout << i << " " << j << " " << black-white << endl;
        win_lose[j][i] += white - black;
        // cout << win_lose[j][i] << endl;
        g_win_lose_lock.unlock();
    }

}
int main() {
    srand(time(0));
    for (int _ = 0; _ < 2; ++_) {
        vector<thread> threads;

        param.clear();
        param.resize(GAME);
        param[0].assign(initParams.begin(), initParams.end());
        for (int i = 1; i < GAME; ++i) {
            vector<double> tmps(param[0].begin(), param[0].end());
            for (int k = 0; k < param[0].size(); ++k) {
                double percentage = ((rand() % 50) - 25) * 0.01;
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
        cout << "====== show result ======" << endl;
        vector<int> sumup(GAME);
        for (int i = 0; i < GAME; ++i) {
            for (int j = 0; j < GAME; ++j) {
                cout << win_lose[i][j] << " ";
                sumup[i] += win_lose[i][j];
            }
            cout << endl;
        }
        cout << endl;
        for (int i = 0; i < GAME; ++i) {
            print(param[i]);
        }
        cout << endl;
        cout << "wins" << endl;
        for (int i = 0; i < GAME; ++i) {
            cout << sumup[i] << " ";
        }
        cout << endl;

        auto argmax = std::max_element(sumup.begin(), sumup.end());
        int argmax_idx = argmax - sumup.begin();
        initParams.assign(param[argmax_idx].begin(), param[argmax_idx].end());
    }
}