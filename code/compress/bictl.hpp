#ifndef BINARY_CONTROL_HPP
#define BINARY_CONTROL_HPP
#define N(x) ((x) >> 8)
#define S(x) ((x) << 8)
#define W(x) (((x) & 0xfefefefefefefefeull) >> 1)
#define E(x) (((x) & 0x7f7f7f7f7f7f7f7full) << 1)
#define NW(x) (W(N(x)))
#define NE(x) (E(N(x)))
#define SW(x) (W(S(x)))
#define SE(x) (E(S(x)))

const int DIR_N = -8;
const int DIR_S = 8;
const int DIR_W = -1;
const int DIR_E = 1;
const int DIR_NW = DIR_N + DIR_W;
const int DIR_NE = DIR_N + DIR_E;
const int DIR_SW = DIR_S + DIR_W;
const int DIR_SE = DIR_S + DIR_E;
#endif