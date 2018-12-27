#include "chessbox.hpp"
#include "bictl.hpp"
#include <iostream>
using namespace std;

ChessBox::ChessBox(int default_player) : player(default_player) {
    assert(default_player == BLACK_ID || default_player == WHITE_ID);
    assert(__size >= 1);
}

void ChessBox::defaultInit() {
    int half = __size / 2;
    __place(TO_SQUARE(half-1, half-1), BLACK_ID);
    __place(TO_SQUARE(half, half-1), WHITE_ID);
    __place(TO_SQUARE(half - 1, half), WHITE_ID);
    __place(TO_SQUARE(half, half), BLACK_ID);
}

inline bool ChessBox::isEmpty(int sq) const {
    return !((boards[0] | boards[1]) & (1ull << sq));
}

inline bool ChessBox::isMyPiece(int sq) const {
    return (boards[player] >> sq) & 1;
}

inline bool ChessBox::isOppPiece(int sq) const {
    return (boards[!player] >> sq) & 1;
}

inline bool ChessBox::isBlackPiece(int sq) const {
    return (boards[BLACK_ID] >> sq) & 1;
}

inline bool ChessBox::isWhitePiece(int sq) const {
    return (boards[WHITE_ID] >> sq) & 1;
}

inline bool ChessBox::isPiece(int sq, int p) const {
    return (boards[p] >> sq) & 1;
}

int ChessBox::countAllPieces() const {
    return __builtin_popcountll(boards[0] | boards[1]);
}

int ChessBox::countMyPieces() const {
    return __builtin_popcountll(boards[player]);
}

int ChessBox::countOppPieces() const {
    return __builtin_popcountll(boards[!player]);
}

int ChessBox::countPieces(int p) const {
    return __builtin_popcountll(boards[p]);
}

inline bool ChessBox::isEnd() const {
    return ~(boards[0] | boards[1]) == 0;
}

ostream& operator<<(ostream& os, const ChessBox& chessbox) {
    int size = chessbox.size();
    u64 movables = chessbox.getMovable(chessbox.player);
    os << "  ";
    for (int i = 0; i < chessbox.size(); ++i) {
        os << i << " ";
    }
    os << endl;
    for (int i = 0; i < size; ++i) {
        os << i << " ";
        for (int j = 0; j < size; ++j) {
            if (chessbox.isBlackPiece(chessbox.TO_SQUARE(i, j))) {
                os << "X ";
            } else if (chessbox.isWhitePiece(chessbox.TO_SQUARE(i, j))) {
                os << "O ";
            } else if (movables >> chessbox.TO_SQUARE(i, j) & 1) {
                os << "+ ";
            } else {
                os << "  ";
            }
        }
        os << endl;
    }
    return os;
}

void ChessBox::__place(int sq, int p) {
    assert(p == BLACK_ID || p == WHITE_ID);
    boards[p] |= 1ull << sq;
}

inline u64 ChessBox::getEmpty() const {
    return ~(boards[0] | boards[1]);
}

#define MOVABLE_HELPER(dir) \
    tmp = dir(cur) & opp; \
    for (int i = 0; i < 5; ++i) { \
        tmp |= dir(tmp) & opp; \
    } \
    ret |= dir(tmp) & empty;

inline u64 ChessBox::getMovable(int p) const {
    u64 empty = getEmpty();
    u64 tmp, ret = 0;
    u64 cur = boards[p];
    u64 opp = boards[!p];

    MOVABLE_HELPER(N);
    MOVABLE_HELPER(S);
    MOVABLE_HELPER(W);
    MOVABLE_HELPER(E);
    MOVABLE_HELPER(NW);
    MOVABLE_HELPER(NE);
    MOVABLE_HELPER(SW);
    MOVABLE_HELPER(SE);

    return ret;
}

inline u64 ChessBox::getFrontier() const {
    u64 empty = getEmpty();
    return (N(empty)
        | S(empty)
        | W(empty)
        | E(empty)
        | NW(empty)
        | NE(empty)
        | SW(empty)
        | SE(empty));
}

#define FLIP_HELPER(dir) \
    if (dir(1ull << sq) & opp) { \
        mask = 0; \
        tmp = dir(1ull << sq); \
        for (; tmp & opp;tmp = dir(tmp)) { \
            mask |= tmp; \
        } \
        if (tmp & cur) { \
            cur ^= mask; \
            opp ^= mask; \
        } \
    }

void ChessBox::__flip(int sq, int p) {
    assert(p == BLACK_ID || p == WHITE_ID);
    u64 mask, tmp;
    u64& cur = boards[p];
    u64& opp = boards[!p];

    FLIP_HELPER(N);
    FLIP_HELPER(S);
    FLIP_HELPER(E);
    FLIP_HELPER(W);
    FLIP_HELPER(NE);
    FLIP_HELPER(NW);
    FLIP_HELPER(SE);
    FLIP_HELPER(SW);
}

void ChessBox::drop(int row, int col, int p) {
    int sq = TO_SQUARE(row, col);
    __place(sq, p);
    __flip(sq, p);
}