#include "json/jsoncpp/json.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
// #include "jsoncpp/json.h" // 在平台上，C++编译时默认包含此库

#include <cctype>
#include "blackwhite.hpp"
#include "solution.hpp"

using namespace std;

int currBotColor;         // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[8][8] = {0}; // 先x后y，记录棋盘状态
int blackPieceCount = 2, whitePieceCount = 2;

// 向Direction方向改动坐标，并返回是否越界
inline bool MoveStep(int &x, int &y, int Direction)
{
    if (Direction == 0 || Direction == 6 || Direction == 7)
        x++;
    if (Direction == 0 || Direction == 1 || Direction == 2)
        y++;
    if (Direction == 2 || Direction == 3 || Direction == 4)
        x--;
    if (Direction == 4 || Direction == 5 || Direction == 6)
        y--;
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return false;
    return true;
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int xPos, int yPos, int color, bool checkOnly = false)
{
    int effectivePoints[8][2];
    int dir, x, y, currCount;
    bool isValidMove = false;
    if (gridInfo[xPos][yPos] != 0)
        return false;
    for (dir = 0; dir < 8; dir++)
    {
        x = xPos;
        y = yPos;
        currCount = 0;
        while (1)
        {
            if (!MoveStep(x, y, dir))
            {
                currCount = 0;
                break;
            }
            if (gridInfo[x][y] == -color)
            {
                currCount++;
                effectivePoints[currCount][0] = x;
                effectivePoints[currCount][1] = y;
            }
            else if (gridInfo[x][y] == 0)
            {
                currCount = 0;
                break;
            }
            else
            {
                break;
            }
        }
        if (currCount != 0)
        {
            isValidMove = true;
            if (checkOnly)
                return true;
            if (color == 1)
            {
                blackPieceCount += currCount;
                whitePieceCount -= currCount;
            }
            else
            {
                whitePieceCount += currCount;
                blackPieceCount -= currCount;
            }
            while (currCount > 0)
            {
                x = effectivePoints[currCount][0];
                y = effectivePoints[currCount][1];
                gridInfo[x][y] *= -1;
                currCount--;
            }
        }
    }
    if (isValidMove)
    {
        gridInfo[xPos][yPos] = color;
        if (color == 1)
            blackPieceCount++;
        else
            whitePieceCount++;
        return true;
    }
    else
        return false;
}

// 检查color方有无合法棋步
bool CheckIfHasValidMove(int color)
{
    int x, y;
    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            if (ProcStep(x, y, color, true))
                return true;
    return false;
}

int main()
{
    int x, y;

    // 初始化棋盘
    gridInfo[3][4] = gridInfo[4][3] = 1;  //|白|黑|
    gridInfo[3][3] = gridInfo[4][4] = -1; //|黑|白|

    // 读入JSON
    string str;
    getline(cin, str);
    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);

    // 分析自己收到的输入和自己过往的输出，并恢复状态
    int turnID = input["responses"].size();
    currBotColor = input["requests"][(Json::Value::UInt)0]["x"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
    for (int i = 0; i < turnID; i++)
    {
        // 根据这些输入输出逐渐恢复状态到当前回合
        x = input["requests"][i]["x"].asInt();
        y = input["requests"][i]["y"].asInt();
        if (x >= 0)
            ProcStep(x, y, -currBotColor); // 模拟对方落子
        x = input["responses"][i]["x"].asInt();
        y = input["responses"][i]["y"].asInt();
        if (x >= 0)
            ProcStep(x, y, currBotColor); // 模拟己方落子
    }

    // 看看自己本回合输入
    x = input["requests"][turnID]["x"].asInt();
    y = input["requests"][turnID]["y"].asInt();
    if (x >= 0)
        ProcStep(x, y, -currBotColor); // 模拟对方落子

    // 找出合法落子点
    int possiblePos[64][2], posCount = 0, choice;

    for (y = 0; y < 8; y++)
        for (x = 0; x < 8; x++)
            if (ProcStep(x, y, currBotColor, true))
            {
                possiblePos[posCount][0] = x;
                possiblePos[posCount++][1] = y;
            }

    // 做出决策（你只需修改以下部分）

    int resultX, resultY;

    char ME = currBotColor == 1 ? BLACK : WHITE;
    char OTHER = ME == BLACK ? WHITE : BLACK;

    ChessBox chessbox(ME);

    TableEval eval;
    MiniMaxSolution minimax_solution(chessbox, eval, 10);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (gridInfo[i][j] == 1) {
                chessbox.Drop(i, j, BLACK);
            } else if (gridInfo[i][j] == -1) {
                chessbox.Drop(i, j, WHITE);
            } else {
                assert(gridInfo[i][j] == 0) ;
            }
        }
    }

    auto p = minimax_solution.solve(ME);
    resultX = p.first;
    resultY = p.second;



    if (posCount > 0)
    {
        srand(time(0));
        choice = rand() % posCount;
        resultX = possiblePos[choice][0];
        resultY = possiblePos[choice][1];
    }
    else
    {
        resultX = -1;
        resultY = -1;
    }

    // 决策结束，输出结果（你只需修改以上部分）

    Json::Value ret;
    ret["response"]["x"] = resultX;
    ret["response"]["y"] = resultY;
    ret["debug"] = "";
    Json::FastWriter writer;
    cout << writer.write(ret) << endl;
    return 0;
}