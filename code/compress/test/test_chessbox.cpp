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
    eval.addEval(te, 5.767431);
    eval.addEval(ce, 93.199025);
    eval.addEval(cte, 14.310416);
    eval.addEval(ae, 26.005846);
    eval.addEval(fe, 25.107981);
    eval.addEval(cee, 153.294812);
    eval.addEval(be, 23.111579);
    eval.addEval(hse, 15.381006);
    AlphaBetaSolve abs(eval, chessbox);
// 5.767431, 93.199025, 14.310416, 26.005846, 25.107981, 153.294812, 23.111579, 15.381006
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
