#include <iostream>
#include "../chessbox.hpp"
#include "../alphabet.hpp"
#include <vector>
using namespace std;
int main() {
    ChessBox chessbox(BLACK_ID);
    chessbox.defaultInit();
    cout << chessbox << endl;

    int player = BLACK_ID;
    TableEval te;
    CornerEval ce;
    CountingEval cte;
    ActionEval ae;
    FrontEval fe;

    AllInOneEval eval;
    eval.addEval(te);
    eval.addEval(ce);
    eval.addEval(cte);
    eval.addEval(ae);
    eval.addEval(fe);

    AlphaBetaSolve abs(eval, chessbox);

    while(!chessbox.isEnd()) {
        int a, b;
        cin >> a >> b;
        chessbox.drop(a, b, player);
        cout << chessbox;

        vector<Position> s = abs.n_solve(10, !player, 8);
        for (const auto& pos : s) {
            cout << pos.x << " " << pos.y << " " << pos.val << endl;
        }
        Position pos = s.front();
        chessbox.drop(pos.x, pos.y, !player);

        cout << chessbox << endl;
        s = abs.n_solve(10, player, 8);
        for (const auto& pos : s) {
            cout << pos.x << " " << pos.y << " " << pos.val << endl;
        }
        eval.detailEval(chessbox, BLACK_ID);
    }

};
