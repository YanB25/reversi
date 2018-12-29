# 接口说明文档
## 棋盘类
### 位置表示
位置的表示有两种。第一种是
``` C++
typedef unsigned long long u64;
u64 cur = ...;
```
此时一个cur代表棋盘上的若干个点。如果u64的某个bit为1，代表有棋子。否则代表无棋子。

另一种表示是
``` C++
int sq = 5;
```
此时sq代表某一个棋子在棋盘中的位置。代表这个棋子在棋盘中的sq / 8行，sq % 8列。
### 样例代码
``` C++
#include "chessbox.hpp" // 棋盘类
int main() {
    ChessBox chessbox();
    chessbox.defaultInit(); // 在棋盘的中央放上4个子，作为开局

    // 取值0 ~ 63, 代表 sequence / 8 行， sequence % 8 列的位置
    int sq = 4; 
    int player = BLACK_ID; // BLACK_ID或者WHITE_ID

    // row, column, player-id, 下棋并翻转棋盘
    chessbox.drop(5, 2, BLACK_ID)

    int sq2 = chessbox.TO_SQUARE(5, 4); // 将行列转换为sq表示法

    // 得到黑棋的所有可行动位置。位置以sequence的方式表示
    vector<int> movessq(BLACK_ID);

    // chessbox.__place(sequence, player) // player在sequence位置放上一个子（只是放子而已，不翻转）
}
```

### 常用函数头
``` C++
class ChessBox {
public:
    bool isEmpty(int sq) const; // 位置sq没有棋子
    bool isBlackPiece(int sq) const; // 位置sq是黑子
    bool isWhitePiece(int sq) const; // 位置sq是白子
    bool isPiece(int sq, int p) const; // 位置sq有子

    int countAllPieces() const; // 棋盘上棋子总数
    // 统计棋盘上玩家p的棋子数。p in [BLACK_ID, WHITE_ID]
    int countPieces(int p) const; 
    int countBlackPieces() const; // 黑棋个数
    int countWhitePieces() const; // 白棋个数

    bool isEnd() const; // 游戏是否结束
    int winner() const; // 返回胜利者是谁。返回BLACK_ID 或 WHITE_ID

    u64 getEmpty() const; // 以u64的形式返回棋盘空位
    u64 getMovable(int p) const; // 以u64的形式返回可下子点
    u64 getFrontier() const; // 以u64的形式返回棋盘的"边缘子”

    void __place(int sq, int p); // 放置一个子（仅仅放置而已）
    void __flip(int sq, int player); // 显式地翻转棋盘
    u64 __getBoard(int p) const { return boards[p]; } // 拿到玩家p的棋子位置
```

## alpha-beta类
``` C++
#include "alphabeta.hpp"
int main() {
    // 选两种估值方式
    TableEval te;
    CornerEval ce;

    // 对上述估值做加权求和估值
    AllInOneEval eval;
    eval.addEval(te, 5);
    eval.addEval(cte, 14);

    ChessBox chessbox;
    chessbox.defaultInit();

    AlphaBetaSolve abs(eval, chessbox);

    Position p = abs.solve(BLACK_ID); // 得到一个解
    // vector<Position> ps = abs.n_solve(BLACK_ID, 5); // 得到最多5个解，ps[0]是最优解，ps[ps.size()-1]是最劣解。
    int resultX = p.x;
    int resultY = p.y;
    int evaluation = p.val; // **黑方**的估值

    chessbox.drop(chessbox.TO_SQUARE(resultX, resultY)); // 下子
}
```