#ifndef CONSTANT_H_
#define CONSTANT_H_

#ifdef _BOTZONE_ONLINE
#define NDEBUG
#include "jsoncpp/json.h"
#else
#include "../json/jsoncpp/json.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <string>

// Board
typedef unsigned long long u64;

const int BOARD_WIDTH = 8;
const int BOARD_SIZE = 64;

const int DIR_N = -8;
const int DIR_S = +8;
const int DIR_W = -1;
const int DIR_E = +1;
const int DIR_NW = DIR_N + DIR_W;
const int DIR_NE = DIR_N + DIR_E;
const int DIR_SW = DIR_S + DIR_W;
const int DIR_SE = DIR_S + DIR_E;

const int MAX_MOVES = 32;

// Evaluation
typedef int Value;

const Value INF = 1e8;
const Value BND = 1e8 + 100;
const Value UNKNOWN_VALUE = 2e8;
const Value VALUE[BOARD_SIZE] = {
    20, -3, 11,  8,  8, 11, -3, 20,
    -3, -7, -4,  1,  1, -4, -7, -3,
    11, -4,  2,  2,  2,  2, -4, 11,
     8,  1,  2, -3, -3,  2,  1,  8,
     8,  1,  2, -3, -3,  2,  1,  8,
    11, -4,  2,  2,  2,  2, -4, 11,
    -3, -7, -4,  1,  1, -4, -7, -3,
    20, -3, 11,  8,  8, 11, -3, 20,
};

// Hash
typedef long long HashValue;

enum HashType {
    EXACT,
    ALPHA,                      // lower bound
    BETA                        // upper bound
};

const HashValue PIECE_HASH_VALUE[BOARD_SIZE][2] = {
{4594751534691652102,1851944401440995956}, {2747968243137263198,2207742792711565793}, {3848004645121087451,3216815717814836416}, {1516548209265803966,2020642571208227878}, {2337722696409584694,3437059740345402271}, {2898258925230917030,1303855352451580365}, {6741448498992359,4377852815973168715}, {4372139272928371625,3315674304057407093},
{2878955577234187687,3310911488601986908}, {3582136329572115185,3886893032669033165}, {708434475528378106,1888811197289616306}, {1100757220377106200,2346419301461881836}, {1995470285667003037,3344903240187633289}, {379138142664434743,2911313461119289748}, {769900647706363980,4570344310535739107}, {3830265281089289793,3708897551051154464},
{3315091279816950970,273872670825111973}, {2041102417123238912,3112772639692668910}, {4299351587052741497,492310524505137784}, {4010522851494399473,362833556870927538}, {3744809366975419281,4226902705789150687}, {592324386571251539,1676350313352030418}, {297655183198752013,4509827100803281127}, {1403475634077929604,1892578936484963221},
{1715531292376832429,1613769283609259830}, {504753725576921797,2392422484281687273}, {1111776814178973127,1894484583724300455}, {1353368917078917269,980842078909767887}, {3122588916900066851,1904920763832515654}, {604593662070056141,4610439775266802671}, {3503933470494872029,465452319321546006}, {4524482860969100773,418483288981978408},
{2147108479613262724,402603979465422751}, {1068073449811579883,773275828138712383}, {313392534030852928,2844281495097492616}, {4421182344573758470,2690463909089054058}, {3564969729531244233,2153253714589235195}, {2048902308496474304,2035312525942884256}, {247110080496067937,460126216295236977}, {2910862077868453624,1236872844561888511},
{4434503285626937640,1788235057956326105}, {617585277383179775,3098146742097029318}, {544179776494490277,3070740981001682629}, {1133217526157530591,2597739258228835604}, {2291314423671849210,3285761339244847317}, {3210152689877290283,4256064628862687295}, {983114329860268105,669680476452377688}, {3546030145041577463,3617169695558622675},
{3355229619035115716,4340260948930533822}, {4157463891367309266,4259738129252256818}, {3868311963489900326,292106685109972994}, {3029152897729491494,443633747780305188}, {4438782935932979823,476344706333946286}, {225228141352298397,437312018862372058}, {3013545812034048651,1882826361476336167}, {1682028769400164518,490632862661383976},
{3045929461469737835,705051332461977505}, {3071896532965914314,4398387264278704174}, {2968653874300097863,2681664983844070838}, {2243007804517107923,2718365947100986101}, {3315433970204178064,2094902054428670605}, {3639206564310825114,495668310978194141}, {3266804706767133950,137318566489138708}, {847159218020085283,3988956296565724071},
};

const int HASH_TABLE_SIZE = 1 << 23;
const int HASH_TABLE_MASK = HASH_TABLE_SIZE - 1;

// instead of std::pair<int, Value>
struct SVPair {
    int first;
    Value second;
    SVPair() { }
    SVPair(int x, Value y): first(x), second(y) { }
};

const int MAX_DEPTH = 64;
const int DEBUG_MAX_DEPTH = 12;

#endif

#ifndef BITBOARD_H_
#define BITBOARD_H_

// #ifndef _BOTZONE_ONLINE
// #include "constant.h"
// #endif

inline int popcount(u64 x)
{
    return __builtin_popcountll(x);
}

inline u64 getlsb(u64 x)
{
    assert(x);
    return x & -x;
}

inline int getlsbid(u64 x)
{
    assert(x);
    return __builtin_ffsll(x) - 1;
}

inline u64 getEmpty(u64 cur, u64 opp)
{
    assert(!(cur & opp));
    return ~(cur | opp);
}

inline void printBB(u64 x)
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            printf("%llu", (x >> (i * 8 + j) & 1));
        }
        printf("\n");
    }
    printf("\n");
}

u64 getFrontierBB(u64 cur, u64 opp);
u64 getMovesBB(u64 cur, u64 opp);
void getFlipped(int sq, int player, u64 &cur, u64 &opp, Value &val, HashValue &hash);

#endif

#ifndef POSITION_H_
#define POSITION_H_

// #ifndef _BOTZONE_ONLINE
// #include "constant.h"
// #include "bitboard.h"
// #endif

inline int toSquare(int x, int y)
{
    return x * BOARD_WIDTH + y;
}

inline int getSquareX(int sq)
{
    return sq / BOARD_WIDTH;
}

inline int getSquareY(int sq)
{
    return sq % BOARD_WIDTH;
}

class Position {
public:
    Position();

    bool isBlackPlayer() const;
    bool isWhitePlayer() const;
    bool isEmpty(int sq) const;
    bool isMyPiece(int sq) const;
    bool isOppPiece(int sq) const;
    bool isBlackPiece(int sq) const;
    bool isWhitePiece(int sq) const;
    int countAllPieces() const;
    int countBlackPieces() const;
    int countWhitePieces() const;
    int countMyPieces() const;
    int countOppPieces() const;
    bool isGameEnd() const;
    int generateMoves(int moves[]) const;

    Value getEval() const;
    Value getGameEndEval() const;

    HashValue getHashValue() const;

    void print() const;

    void changePlayer();
    void applyMove(int sq);
    void applyMoveWithHint(int sq, int hint);
    void applyNullMove();

private:
    u64 board[2];
    int player;
    int nullMoveCount;
    int basicEval;
    HashValue hashValue;

    void place(int sq);
    void replace(int sq);
};

inline bool Position::isBlackPlayer() const
{
    return player == 0;
}

inline bool Position::isWhitePlayer() const
{
    return player == 1;
}

inline bool Position::isEmpty(int sq) const
{
    return !(board[0] >> sq & 1) && !(board[1] >> sq & 1);
}

inline bool Position::isMyPiece(int sq) const
{
    return board[player] >> sq & 1;
}

inline bool Position::isOppPiece(int sq) const
{
    return board[player ^ 1] >> sq & 1;
}

inline bool Position::isBlackPiece(int sq) const
{
    return board[0] >> sq & 1;
}

inline bool Position::isWhitePiece(int sq) const
{
    return board[1] >> sq & 1;
}

inline int Position::countAllPieces() const
{
    return popcount(board[0] | board[1]);
}

inline int Position::countBlackPieces() const
{
    return popcount(board[0]);
}

inline int Position::countWhitePieces() const
{
    return popcount(board[1]);
}

inline int Position::countMyPieces() const
{
    return popcount(board[player]);
}

inline int Position::countOppPieces() const
{
    return popcount(board[player ^ 1]);
}

inline bool Position::isGameEnd() const
{
    return countAllPieces() == BOARD_SIZE || nullMoveCount > 1;
}

inline Value Position::getGameEndEval() const
{
    int d = countMyPieces() - countOppPieces();
    if (d == 0)
        return 0;
    return (d > 0 ? +INF : -INF) + d;
}

inline HashValue Position::getHashValue() const
{
    return hashValue << 1 | player;
}

inline void Position::changePlayer()
{
    player ^= 1;
    basicEval = -basicEval;
}

inline void Position::place(int sq)
{
    assert(isEmpty(sq));

    board[player] ^= 1ull << sq;
    basicEval += VALUE[sq];
    hashValue ^= PIECE_HASH_VALUE[sq][player];
}

#endif

#ifndef SEARCH_H_
#define SEARCH_H_

// #ifndef _BOTZONE_ONLINE
// #include "constant.h"
// #include "bitboard.h"
// #include "position.h"
// #endif

struct HashNode {
    HashValue key;
    Value value;
    int depth;
    HashType type;
    int bestMove;
};

bool isTimeUp();

void initHashTable();
SVPair probeHash(int depth, Value alpha, Value beta, const Position &pos, bool &cut);
void recordHash(const Position &pos, Value value, int depth, HashType type, int bestMove);
SVPair alphabeta(int depth, Value alpha, Value beta, const Position &pos, bool requireMove = false);
SVPair getBestMove(const Position &pos, int &maxdepth);

inline bool isTimeUp()
{
    return clock() > 0.98 * CLOCKS_PER_SEC;
}

#endif

#ifndef UI_H_
#define UI_H_

// #ifndef _BOTZONE_ONLINE
// #include "constant.h"
// #include "position.h"
// #include "search.h"
// #endif

namespace UI
{

const int MAX_INPUT_LENGTH = 100;

void errorMessage(const char *msg);
void initMessage();

void printChessboard(const Position &pos);
void getHumanMove(Position &pos);
void exec();

inline void errorMessage(const char *msg)
{
    printf("Error: %s\n", msg);
}

inline void initMessage()
{
    system("clear");

    printf("====================\n");
    printf("|    Iris Alpha    |\n");
    printf("====================\n");
    printf("\n");
}

}

#endif

#ifndef BOT_H_
#define BOT_H_

// #ifndef _BOTZONE_ONLINE
// #include "constant.h"
// #include "position.h"
// #include "search.h"
// #endif

namespace Bot
{

Position input();
void output(SVPair move, int depth);
void exec();

}

#endif

// #ifndef _BOTZONE_ONLINE
// #include "bitboard.h"
// #endif

#define N(x) ((x) >> 8)
#define S(x) ((x) << 8)
#define W(x) (((x) & 0xfefefefefefefefeull) >> 1)
#define E(x) (((x) & 0x7f7f7f7f7f7f7f7full) << 1)
#define NW(x) (N(W(x)))
#define NE(x) (N(E(x)))
#define SW(x) (S(W(x)))
#define SE(x) (S(E(x)))

u64 getFrontierBB(u64 cur, u64 opp)
{
    u64 empty = getEmpty(cur, opp);
    return (N(empty)
            | S(empty)
            | W(empty)
            | E(empty)
            | NW(empty)
            | NE(empty)
            | SW(empty)
            | SE(empty));
}
// 找到所有向某个方向前进6次的自己与对方重叠的点
// 再前进因此，和空位做或运算。
#define GETMOVE_HELPER(dir)                               \
    tmp = dir(cur) & opp;                                 \
    for (int i = 0; i < 5; ++i) tmp |= dir(tmp) & opp;    \
    ret |= dir(tmp) & empty;

u64 getMovesBB(u64 cur, u64 opp)
{
    u64 empty = getEmpty(cur, opp);
    u64 tmp, ret = 0;

    GETMOVE_HELPER(N);
    GETMOVE_HELPER(S);
    GETMOVE_HELPER(W);
    GETMOVE_HELPER(E);
    GETMOVE_HELPER(NW);
    GETMOVE_HELPER(NE);
    GETMOVE_HELPER(SW);
    GETMOVE_HELPER(SE);

    return ret;
}

#define GETFLIPPED_HELPER(dir)                                          \
    if (dir(1ull << sq) & opp) {                                        \
        mask = 0;                                                       \
        v = 0;                                                          \
        h = 0;                                                          \
        id = sq + DIR_##dir;                                            \
        tmp = dir(1ull << sq);                                          \
        for (; tmp & opp; id += DIR_##dir, tmp = dir(tmp)) {            \
            mask |= tmp;                                                \
            v += VALUE[id];                                             \
            h ^= PIECE_HASH_VALUE[id][player];                          \
            h ^= PIECE_HASH_VALUE[id][player ^ 1];                      \
        }                                                               \
        if (tmp & cur) {                                                \
            cur ^= mask;                                                \
            opp ^= mask;                                                \
            val += v * 2;                                               \
            hash ^= h;                                                  \
        }                                                               \
    }

void getFlipped(int sq, int player, u64 &cur, u64 &opp, Value &val, HashValue &hash)
{
    u64 mask, tmp;
    int id;
    Value v;
    HashValue h;

    GETFLIPPED_HELPER(N);
    GETFLIPPED_HELPER(S);
    GETFLIPPED_HELPER(W);
    GETFLIPPED_HELPER(E);
    GETFLIPPED_HELPER(NW);
    GETFLIPPED_HELPER(NE);
    GETFLIPPED_HELPER(SW);
    GETFLIPPED_HELPER(SE);
}

// #ifndef _BOTZONE_ONLINE
// #include "position.h"
// #endif

Position::Position()
{
    board[0] = (1ull << toSquare(3, 4)) | (1ull << toSquare(4, 3));
    board[1] = (1ull << toSquare(3, 3)) | (1ull << toSquare(4, 4));
    player = 0;
    nullMoveCount = 0;
    basicEval = 0;
    hashValue = PIECE_HASH_VALUE[toSquare(3, 3)][1]
              ^ PIECE_HASH_VALUE[toSquare(3, 4)][0]
              ^ PIECE_HASH_VALUE[toSquare(4, 3)][0]
              ^ PIECE_HASH_VALUE[toSquare(4, 4)][1];
}

int Position::generateMoves(int moves[]) const
{
    u64 movesBB = getMovesBB(board[player], board[player ^ 1]);
    int tot = 0;
    while (movesBB) {
        moves[tot++] = getlsbid(movesBB);
        movesBB ^= getlsb(movesBB);
    }
    assert(tot <= MAX_MOVES);
    return tot;
}

void Position::applyMove(int sq)
{
    place(sq);
    getFlipped(sq, player, board[player], board[player ^ 1], basicEval, hashValue);
    nullMoveCount = 0;
    changePlayer();
}

void Position::applyNullMove()
{
    assert(nullMoveCount <= 1);

    ++nullMoveCount;
    changePlayer();
}

Value Position::getEval() const
{
    int my_tiles = 0, opp_tiles = 0, my_front_tiles = 0, opp_front_tiles = 0;
    double p = 0, c = 0, l = 0, m = 0, f = 0, d = 0;

    d = basicEval;

    // Piece difference, frontier disks and disk squares
    u64 frontier = getFrontierBB(board[player], board[player ^ 1]);
    my_tiles = countMyPieces();
    opp_tiles = countOppPieces();
    my_front_tiles = popcount(frontier & board[player]);
    opp_front_tiles = popcount(frontier & board[player ^ 1]);
    // 棋子个数(越多越好，但不是很主要)， -100～100
    if (my_tiles > opp_tiles)
        p = (100.0 * my_tiles) / (my_tiles + opp_tiles);
    else if (my_tiles < opp_tiles)
        p = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);
    // 棋子的边缘个数（越少越好), -100~100
    if (my_front_tiles > opp_front_tiles)
        f = -(100.0 * my_front_tiles) / (my_front_tiles + opp_front_tiles);
    else if (my_front_tiles < opp_front_tiles)
        f = (100.0 * opp_front_tiles) / (my_front_tiles + opp_front_tiles);

    // Corner occupancy, 给予角落每个25的额外加分
    my_tiles = opp_tiles = 0;
    my_tiles += isMyPiece(toSquare(0, 0));
    opp_tiles += isOppPiece(toSquare(0, 0));
    my_tiles += isMyPiece(toSquare(0, 7));
    opp_tiles += isOppPiece(toSquare(0, 7));
    my_tiles += isMyPiece(toSquare(7, 0));
    opp_tiles += isOppPiece(toSquare(7, 0));
    my_tiles += isMyPiece(toSquare(7, 7));
    opp_tiles += isOppPiece(toSquare(7, 7));
    c = 25 * (my_tiles - opp_tiles);

    // Corner closeness 给予不是角落的靠近角落点的额外扣分
    my_tiles = opp_tiles = 0;
    if (isEmpty(toSquare(0, 0))) {
        my_tiles += isMyPiece(toSquare(0, 1));
        opp_tiles += isOppPiece(toSquare(0, 1));
        my_tiles += isMyPiece(toSquare(1, 0));
        opp_tiles += isOppPiece(toSquare(1, 0));
        my_tiles += isMyPiece(toSquare(1, 1));
        opp_tiles += isOppPiece(toSquare(1, 1));
    }
    if (isEmpty(toSquare(0, 7))) {
        my_tiles += isMyPiece(toSquare(0, 6));
        opp_tiles += isOppPiece(toSquare(0, 6));
        my_tiles += isMyPiece(toSquare(1, 6));
        opp_tiles += isOppPiece(toSquare(1, 6));
        my_tiles += isMyPiece(toSquare(1, 7));
        opp_tiles += isOppPiece(toSquare(1, 7));
    }
    if (isEmpty(toSquare(7, 0))) {
        my_tiles += isMyPiece(toSquare(6, 0));
        opp_tiles += isOppPiece(toSquare(6, 0));
        my_tiles += isMyPiece(toSquare(6, 1));
        opp_tiles += isOppPiece(toSquare(6, 1));
        my_tiles += isMyPiece(toSquare(7, 1));
        opp_tiles += isOppPiece(toSquare(7, 1));
    }
    if (isEmpty(toSquare(7, 7))) {
        my_tiles += isMyPiece(toSquare(6, 6));
        opp_tiles += isOppPiece(toSquare(6, 6));
        my_tiles += isMyPiece(toSquare(6, 7));
        opp_tiles += isOppPiece(toSquare(6, 7));
        my_tiles += isMyPiece(toSquare(7, 6));
        opp_tiles += isOppPiece(toSquare(7, 6));
    }
    l = -12.5 * (my_tiles - opp_tiles);

    // Mobility 可行动数量
    my_tiles = popcount(getMovesBB(board[player], board[player ^ 1]));
    opp_tiles = popcount(getMovesBB(board[player ^ 1], board[player]));
    if (my_tiles > opp_tiles)
        m = (100.0 * my_tiles) / (my_tiles + opp_tiles);
    else if (my_tiles < opp_tiles)
        m = -(100.0 * opp_tiles) / (my_tiles + opp_tiles);

    // final weighted score
    double score = ((10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d)) * 100;

    return (Value)score;
}

void Position::print() const
{
    printf("Round #%d\n", countAllPieces() - 4 + 1);
    printf("Black: %d   White: %d\n", countBlackPieces(), countWhitePieces());
    if (isBlackPlayer())
        printf("Now BLACK\n");
    else
        printf("Now WHITE\n");
    printf("Hash Value: %lld\n", getHashValue());
    printf("Evaluation: %d\n", getEval());

    printf("  ");
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            int sq = toSquare(i, j);
            if (isEmpty(sq))
                printf(". ");
            else if (isBlackPiece(sq))
                printf("b ");
            else {
                assert(isWhitePiece(sq));
                printf("w ");
            }
        }
        printf("\n");
    }
}

// #ifndef _BOTZONE_ONLINE
// #include "search.h"
// #endif

HashNode hashTable[HASH_TABLE_SIZE + 1];

void initHashTable()
{
    memset(hashTable, 0, sizeof(hashTable));
}

SVPair probeHash(int depth, Value alpha, Value beta, const Position &pos, bool &cut)
{
    HashValue key = pos.getHashValue();
    int idx = key & HASH_TABLE_MASK;
    HashNode &node = hashTable[idx];

    if (node.key != key)
        return SVPair(-1, UNKNOWN_VALUE);

    if (node.depth >= depth) {
        cut = true;
        if (node.type == EXACT) {
            return SVPair(node.bestMove, node.value);
        } else if (node.type == ALPHA) {
            if (node.value <= alpha)
                return SVPair(node.bestMove, alpha);
        } else {
            assert(node.type == BETA);
            if (node.value >= beta)
                return SVPair(node.bestMove, beta);
        }
        cut = false;
    }

    return SVPair(node.bestMove, node.value);
}

void recordHash(const Position &pos, Value value, int depth, HashType type, int bestMove)
{
    HashValue key = pos.getHashValue();
    int idx = key & HASH_TABLE_MASK;
    HashNode &node = hashTable[idx];

    if (node.depth <= depth) {
        node.key = key;
        node.value = value;
        node.depth = depth;
        node.type = type;
        node.bestMove = bestMove;
    }
}

SVPair alphabeta(int depth, Value alpha, Value beta, const Position &pos, bool requireMove)
{
    if (pos.isGameEnd())
        return SVPair(-1, pos.getGameEndEval());

    bool cut = false;
    SVPair lastsv = probeHash(depth, alpha, beta, pos, cut);
    if (cut && (!requireMove || lastsv.first != -1))
        return lastsv;

    if (depth == 0) {
        Value val = pos.getEval();
        recordHash(pos, val, depth, EXACT, -1);
        return SVPair(-1, val);
    }

#ifndef FIXED_DEPTH
    if (isTimeUp())
        return SVPair(-1, pos.getEval());
#endif

    int moves[MAX_MOVES];
    int totMoves = pos.generateMoves(moves);

    if (totMoves == 0) {
        Position newPos(pos);
        newPos.applyNullMove();
        Value val = -alphabeta(depth, -beta, -alpha, newPos).second;
        return SVPair(-1, val);
    }

    if (lastsv.first != -1) {
        for (int i = 0; i < totMoves; ++i) {
            if (moves[i] == lastsv.first) {
                std::swap(moves[0], moves[i]);
                break;
            }
        }
        assert(moves[0] == lastsv.first);
    }

    Value bestValue = -BND;
    int bestMove = -1;
    HashType hasht = ALPHA;

    for (int i = 0; i < totMoves; ++i) {
        Position newPos(pos);
        newPos.applyMove(moves[i]);

        Value val;
        val = -alphabeta(depth - 1, -beta, -alpha, newPos).second;

        if (val > bestValue) {
            bestValue = val;
            bestMove = moves[i];
            if (val >= beta) {
                recordHash(pos, bestValue, depth, BETA, bestMove);
                return SVPair(bestMove, bestValue);
            }
            if (val > alpha) {
                alpha = val;
                hasht = EXACT;
            }
        }
    }

    recordHash(pos, bestValue, depth, hasht, bestMove);

    return SVPair(bestMove, bestValue);
}

void printBestPath(int depth, const Position &pos)
{
    Value val = pos.isGameEnd() ? pos.getGameEndEval() : pos.getEval();
    printf("dep=%d, val=%d\n", depth, val);
    pos.print();

    if (depth != 0) {
        bool cut = false;
        SVPair lastsv = probeHash(depth, -BND, BND, pos, cut);
        assert(cut);
        Position newPos(pos);
        newPos.applyMove(lastsv.first);
        printBestPath(depth - 1, newPos);
    }
}

SVPair getBestMove(const Position &pos, int &maxdepth)
{
    initHashTable();

    SVPair ret(-1, 0);

#ifndef FIXED_DEPTH
    for (int depth = 1; depth <= MAX_DEPTH; ++depth) {
#else
    for (int depth = 1; depth <= DEBUG_MAX_DEPTH; ++depth) {
#endif
        SVPair cur = alphabeta(depth, -BND, +BND, pos, true);
#ifndef FIXED_DEPTH
        if (isTimeUp())
            break;
#else
        printf("finish depth %d (%d,%d)\n", depth, cur.first, cur.second);
#endif
        ret = cur;
        maxdepth = depth;
    }

// #ifdef FIXED_DEPTH
//     printBestPath(DEBUG_MAX_DEPTH, pos);
// #endif

    return ret;
}

// #ifndef _BOTZONE_ONLINE
// #include "ui.h"
// #endif

void UI::printChessboard(const Position &pos)
{
    system("clear");
    pos.print();
}

void UI::getHumanMove(Position &pos)
{
    for (;;) {
        printf("Your move please(e.g. c4): ");

        char inp[MAX_INPUT_LENGTH];
        char x, y;
        scanf("%s", inp);
        sscanf(inp, "%c%c", &x, &y);

        int column = x - 'a';
        int line = y - '1';
        if (line < 0 || line >= BOARD_WIDTH || column < 0 || column >= BOARD_WIDTH) {
            errorMessage("Invalid input!");
            continue;
        }

        int sq = toSquare(line, column);
        // TODO: valid move check
        // if (!pos.isValidMove(sq)) {
        //     errorMessage("Invalid move!");
        //     continue;
        // }

        pos.applyMove(sq);
        break;
    }
}

void UI::exec()
{
    initMessage();

    int humanPlayer;
    for (;;) {
        printf("Chosse black(0) or white(1): ");

        char inp[MAX_INPUT_LENGTH];
        scanf("%s", inp);
        sscanf(inp, "%d", &humanPlayer);

        if (humanPlayer != 0 && humanPlayer != 1) {
            errorMessage("Invalid input!");
            continue;
        }
        break;
    }

    int currentPlayer = 0;
    Position pos;
    SVPair aiMove(-1, 0);

    for (;;) {
        printChessboard(pos);

        printf("AI Move: ");
        if (aiMove.first != -1) {
            printf("%c%c\n", getSquareY(aiMove.first) + 'a', getSquareX(aiMove.first) + '1');
            printf("eval: %d", aiMove.second);
        } else
            printf("null");
        printf("\n");

        if (pos.isGameEnd()) {
            printf("Game End!\n");

            Value val = pos.getGameEndEval();
            if (val == 0)
                printf("Equal!\n");
            else if ((pos.isBlackPlayer() && val == +INF)
                     || (pos.isWhitePlayer() && val == -INF))
                printf("Black win!\n");
            else
                printf("White win!\n");

            break;
        }

        int tmp[MAX_MOVES];

        if (pos.generateMoves(tmp) > 0) {
            if (currentPlayer == humanPlayer)
                getHumanMove(pos);
            else {
                printf("Computing...\n");

                int maxdepth;
                aiMove = getBestMove(pos, maxdepth);
                pos.applyMove(aiMove.first);
            }
        } else {
            pos.applyNullMove();

            aiMove.first = -1;
        }

        currentPlayer ^= 1;
    }
}

// #ifndef _BOTZONE_ONLINE
// #include "bot.h"
// #endif

Position Bot::input()
{
    Position pos;
    int x, y;

    std::string str;
    std::getline(std::cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);

	int turnID = input["responses"].size();
    bool black = input["requests"][(Json::Value::UInt)0]["x"].asInt() < 0;
	for (int i = 0; i < turnID; i++) {
        if (i > 0 || !black) {
            x = input["requests"][i]["x"].asInt();
            y = input["requests"][i]["y"].asInt();
            if (x >= 0)
                pos.applyMove(toSquare(x, y));
            else
                pos.applyNullMove();
        }
		x = input["responses"][i]["x"].asInt();
		y = input["responses"][i]["y"].asInt();
        if (x >= 0)
            pos.applyMove(toSquare(x, y));
        else
            pos.applyNullMove();
	}

    if (turnID > 0 || !black) {
        x = input["requests"][turnID]["x"].asInt();
        y = input["requests"][turnID]["y"].asInt();
        if (x >= 0)
            pos.applyMove(toSquare(x, y));
        else
            pos.applyNullMove();
    }

    return pos;
}

void Bot::output(SVPair move, int depth)
{
    Json::Value ret;
    if (move.first != -1) {
        ret["response"]["x"] = getSquareX(move.first);
        ret["response"]["y"] = getSquareY(move.first);
    } else {
        ret["response"]["x"] = -1;
        ret["response"]["y"] = -1;
    }
    ret["debug"] = "eval=" + std::to_string(move.second) + ", depth=" + std::to_string(depth);

	Json::FastWriter writer;
    std::cout << writer.write(ret) << std::endl;
}

void Bot::exec()
{
    Position pos = input();
    assert(!pos.isGameEnd());

    int tmp[MAX_MOVES];
    int depth = 0;
    SVPair move(-1, 0);

    if (pos.generateMoves(tmp) > 0)
        move = getBestMove(pos, depth);

    output(move, depth);
}

// #ifndef _BOTZONE_ONLINE
// #include "ui.h"
// #include "bot.h"
// #endif

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "interactive") == 0)
        UI::exec();
    else
        Bot::exec();

    return 0;
}