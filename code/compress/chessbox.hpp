#ifndef __CHESSBOX_HPP_
#define __CHESSBOX_HPP_
#include <iostream>
using namespace std;

#define BLACK ('X')
#define WHITE ('O')
#define BLACK_ID (1)
#define WHITE_ID (0)

struct Position {
    int x;
    int y;
    int val;
    Position(int _x, int _y, double v = 0): x(_x), y(_y), val(v) {}
    Position(): x(-1), y(-1), val(0) {}
    Position(const Position& rhs) = default;
};
bool blackPosCmp(const Position& lhs, const Position& rhs);
bool whitePosCmp(const Position& lhs, const Position& rhs);
typedef unsigned long long u64;
class ChessBox {
public:
    ChessBox(int default_palyer = BLACK_ID);
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
    int countBlackPieces() const;
    int countWhitePieces() const;

    int winner() const {
        cout << __builtin_popcountll(boards[BLACK_ID]) << " " << __builtin_popcountll(boards[WHITE_ID]) << endl;
        return __builtin_popcountll(boards[BLACK_ID]) > __builtin_popcountll(boards[WHITE_ID]) ? BLACK_ID : WHITE_ID;
    }

    bool isEnd() const;
    int size() const { return __size; }

    u64 getEmpty() const;
    u64 getMovable(int p) const;
    u64 getFrontier() const;

    friend ostream& operator<<(ostream&, const ChessBox&);

    // what you should really use is below
    int TO_SQUARE(int row, int col) const { return row * __size + col; }
    void drop(int row, int col, int player);
    vector<int> movessq(int player) const ;
    
    void __changePlayer() { player = !player; }
    void __place(int sq, int p);
    void __flip(int sq, int player);
    u64 __getBoard(int p) const { return boards[p]; }
private:
    inline void __debug_check_sq(int sq) const { 
        #ifdef DEBUG
        assert(sq >= 0 && sq < 64); 
        #endif
    }
    inline void __debug_check_player(int p) const{
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