#include <iostream>
#include "../chessbox.hpp"
#include <vector>
using namespace std;
int main() {
    ChessBox chessbox(BLACK_ID);
    chessbox.defaultInit();
    cout << chessbox << endl;
    // int a, b;
    // cout << "is black? " << chessbox.isBlackPlayer() << endl;
    // cout << "is white? " << chessbox.isWhitePlayer() << endl;
    // cout << "who am i? " << chessbox.getCurrentPlayer() << endl;

    // cout << "input Int Int to test drop black" << endl;
    // cin >> a >> b;
    // chessbox.place(chessbox.TO_SQUARE(a, b));
    // chessbox.__flip(chessbox.TO_SQUARE(a, b), BLACK_ID);
    // cout << chessbox;
    // cout << "input Int Int to test drop white" << endl;
    // cin >> a >> b;
    // chessbox.placePlayer(chessbox.TO_SQUARE(a, b), WHITE_ID);
    // chessbox.__flip(chessbox.TO_SQUARE(a, b), WHITE_ID);
    // cout << chessbox;

    // cout << "let counts" << endl;
    // int bl, w, al;
    // al = chessbox.countAllPieces();
    // bl = chessbox.countMyPieces();
    // w = chessbox.countOppPieces();
    // cout << "black, white, all " << bl << " " << w << " " << al << endl;

    int player = BLACK_ID;
    for (int i = 0; i < 10; ++i) {
        int a, b;
        cin >> a >> b;
        chessbox.drop(a, b, player);
        chessbox.__changePlayer();
        cout << chessbox;
        player = !player;

        vector<int> movables = chessbox.movessq(player);
        for (const auto& move: movables) {
            cout << move << " ";
        }
        cout << endl;
    }

};
