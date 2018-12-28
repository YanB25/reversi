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
    CloseCornerEval cee;
    BorderEval be;
    HalfStableEval hse;

    AllInOneEval eval;
    eval.addEval(te, 1.477074);
    eval.addEval(ce, 86.617467);
    eval.addEval(cte, 7.202244);
    eval.addEval(ae, 7.099412);
    eval.addEval(fe, 7.743564);
    eval.addEval(cee, 33.808937);
    eval.addEval(be, 10);
    eval.addEval(hse, 15);
    AlphaBetaSolve abs(eval, chessbox);

    while(!chessbox.isEnd()) {
        int a, b;
        cin >> a >> b;
        chessbox.drop(a, b, player);
        cout << chessbox;

        vector<Position> s = abs.n_solve(10, !player, 6);
        for (const auto& pos : s) {
            cout << pos.x << " " << pos.y << " " << pos.val << endl;
        }
        Position pos = s.front();
        chessbox.drop(pos.x, pos.y, !player);

        cout << chessbox << endl;
        s = abs.n_solve(10, player, 6);
        for (const auto& pos : s) {
            cout << pos.x << " " << pos.y << " " << pos.val << endl;
        }
        // eval.detailEval(chessbox, BLACK_ID);
    }

};
