#ifndef __CHESSBOX_HPP_
#define __CHESSBOX_HPP_
#include <iostream>
using namespace std;

#define BLACK ('X')
#define WHITE ('O')
#define BLACK_ID (1)
#define WHITE_ID (0)

typedef unsigned long long u64;
class ChessBox {
public:
    ChessBox(int default_palyer);
    void defaultInit();

    bool isBlackPlayer() const { return player == BLACK_ID; }
    bool isWhitePlayer() const { return player == WHITE_ID; }
    int getCurrentPlayer() const { return player; }
    bool isEmpty(int sq) const;
    bool isMyPiece(int sq) const;
    bool isOppPiece(int sq) const;
    bool isBlackPiece(int sq) const;
    bool isWhitePiece(int sq) const;
    bool isPiece(int sq, int p) const;

    int countAllPieces() const;
    int countMyPieces() const;
    int countOppPieces() const;
    int countPieces(int p) const;

    bool isEnd() const;
    int size() const { return __size; }

    u64 getEmpty() const;
    u64 getMovable(int p) const;
    u64 getFrontier() const;

    friend ostream& operator<<(ostream&, const ChessBox&);

    void __changePlayer() { player = !player; }
    void __place(int sq, int p);
    void __flip(int sq, int player);
    void drop(int row, int col, int player);

    int TO_SQUARE(int row, int col) const { return row * __size + col; }
    
private:
    inline void __debug_check_sq(int sq) const { 
        #ifdef DEBUG
        assert(sq >= 0 && sq < 64); 
        #endif
    }
    inline void __debug_check_player(int p) const {
        #ifdef DEBUG
        assert(p == BLACK_ID || p == WHITE_ID);
        #endif
    }
    inline void __debug_check_intersect() const {
        #ifdef DEBUG
        assert((boards[0] & boards[1]) == 0);
        #endif
    }
    int __size = 8;
    int player; // 1 for black and 0 for white
    u64 boards[2] = {0ull, 0ull};
};


#endif