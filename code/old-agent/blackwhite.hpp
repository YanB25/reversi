#ifndef BLACK_WHILTE_HPP
#define BLACK_WHILTE_HPP

#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::pair;
using std::vector;

typedef pair<int, int> Position;

const char BLACK = 'X';
const char WHITE = 'O';
const char NONE = ' ';
const char DROP = '+';
class ChessBox {
public:
    explicit ChessBox(char role);
    friend ostream& operator<<(ostream& os, const ChessBox&);
    vector<Position> Dropable(char role) const;
    bool Drop(int y, int x, char role);
    int size() const { return SIZE; }
    char val(int y, int x) const { return map[y][x];}
    const char role() const { return __role; }
    bool isFinished() const {return Dropable(BLACK).size() == 0 && Dropable(WHITE).size() == 0; }
    char whoWin() const;
private:
    const static int SIZE = 7;
    char map[SIZE][SIZE] = {};
    const char __role;
    const char __play_role;

    bool __check_dropable(int y, int x, char role) const;
    bool __check_inbound(int y, int x) const;
    void __fresh_dropable(char role);
    void __revert(int y, int x, char role);
    const int DIR_X[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int DIR_Y[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
};

ostream& operator<<(ostream& os, const ChessBox& cb);



#endif