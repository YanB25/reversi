#include <iostream>
#include <cctype>
#include "blackwhite.hpp"
#include "solution.hpp"
using namespace std;
int main() {
    char PLAYER = BLACK;
    char COMPUTER = WHITE;

    ChessBox chessbox(WHITE);
    cout << chessbox << endl;

    SimpleEval se;
    NaiveEval ne;
    ActionPressEval ape;
    OnlyDroppableEval ode;
    OnlyDroppableEnhanceEval odee;
    // RandomSolution random_solution(chessbox, ape);
    MiniMaxSolution X(chessbox, ape, 9);
    MiniMaxSolution O(chessbox, se, 9);
    while (cin) {
        int y = -1, x = -1;
        cout << ">>> ";
        if (!isdigit(cin.peek())) {
            return 0;
        } 
        cin >> y >> x;
        if (y == -1) {
            return 0;
        }
        if (!cin) {
            cin.clear();
            continue;
        }

        chessbox.Drop(y, x, PLAYER);
        cout << chessbox << endl << O.evaluation(chessbox) << endl;

        auto p = X.solve(COMPUTER);
        chessbox.Drop(p.first, p.second, COMPUTER);
        cout << chessbox << endl << O.evaluation(chessbox) << endl;



        cin.get();
    }
    // while (true) {
    //     int times;
    //     cin >> times;
    //     if (times == -1) break;
    //     for (int i = 0; i < times; ++i) {
    //         auto p1 = X.solve(PLAYER);
    //         chessbox.Drop(p1.first, p1.second, PLAYER);
    //         cout << chessbox << endl;
    //         if (chessbox.isFinished()) {
    //             cout << "winner is " << chessbox.whoWin() << endl;
    //             break;
    //         }
    //         auto p2 = O.solve(COMPUTER);
    //         chessbox.Drop(p2.first, p2.second, COMPUTER);
    //         cout << chessbox << endl;
    //         if (chessbox.isFinished()) {
    //             cout << "winner is " << chessbox.whoWin() << endl;
    //             break;
    //         }
    //     }
    // }
}