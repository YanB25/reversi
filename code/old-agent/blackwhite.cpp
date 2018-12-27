#include "blackwhite.hpp"
ChessBox::ChessBox(char role) : __role(role), __play_role(role == BLACK ? WHITE : BLACK) {
    assert(role == BLACK || role == WHITE);
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            map[i][j] = ' ';
        }
    }

    int mid = (SIZE - 1) / 2;
    map[mid][mid] = BLACK;
    map[mid][mid+1] = WHITE;
    map[mid+1][mid+1] = BLACK; 
    map[mid+1][mid] = WHITE;
    __fresh_dropable(__play_role);
}
char ChessBox::whoWin() const {
    int comcnt = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            char m = map[i][j];
            if (m == __role) {
                comcnt++;
            }
            if (m == __play_role) {
                comcnt--;
            }
        }
    }
    if (comcnt > 0) {
        return __role;
    } else {
        return __play_role;
    }
}

ostream& operator<<(ostream& os, const ChessBox& cb) {
    os << "  ";
    for (int i = 0; i < cb.SIZE; ++i) {
        os << i << " ";
    }
    os << endl;
    for (int i = 0; i < cb.SIZE; ++i) {
        os << i << " ";
        for (int j = 0; j < cb.SIZE; ++j) {
            os << cb.map[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

vector<Position> ChessBox::Dropable(char role) const {
    vector<Position> res;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (__check_dropable(i, j, role)) {
                res.push_back(Position(i, j));
            }
        }
    }
    return res;
}

bool ChessBox::__check_inbound(int y, int x) const {
    return y >= 0 && x >= 0 && y < SIZE && x < SIZE;
}

bool ChessBox::__check_dropable(int y, int x, char role) const {
    if (!__check_inbound(y, x)) return false;
    if (map[y][x] == BLACK || map[y][x] == WHITE) return false;
    #ifdef DEBUG
    assert(__check_inbound(y, x));
    assert(role == BLACK || role == WHITE);
    #endif

    char otherRole = role == BLACK ? WHITE : BLACK;

    for (int i = 0; i < 8; ++i) {
        // seven direction. ul, u, ur, l, r, dl, d, dr
        // (ny, nx) only forwards one step.
        int ny = y + DIR_Y[i];
        int nx = x + DIR_X[i];
        if (!__check_inbound(ny, nx)) continue;
        if (map[ny][nx] != otherRole) continue;
        for (int delta = 2; ; delta++) {
            int delny = y + delta * DIR_Y[i];
            int delnx = x + delta * DIR_X[i];
            if (!__check_inbound(delny, delnx)) break;
            char m = map[delny][delnx];

            #ifdef DEBUG
            assert(m == NONE || m == BLACK || m == WHITE || m == DROP);
            #endif

            if (m == otherRole) {
                continue;
            }
            else if (m == role) {
                return true;
            } else {
                break;
            }
        }
    }

    return false;
}

void ChessBox::__fresh_dropable(char role) {
    #ifdef SHOW_DROPPABLE

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (map[i][j] == DROP) {
                map[i][j] = NONE;
            }
        }
    }
    vector<Position> player_dropable = Dropable(role);
    for (auto position : player_dropable) {
        int y = position.first;
        int x = position.second;
        map[y][x] = DROP;
    }

    #endif
}

bool ChessBox::Drop(int y, int x, char role) {
    if (!__check_dropable(y, x, role)) return false;
    #ifdef DEBUG
    assert(role == BLACK || role == WHITE);
    #endif
    const char otherRole = role == BLACK ? WHITE : BLACK;

    map[y][x] = role;
    __revert(y, x, role);
    __fresh_dropable(otherRole);
    return true;
}

void ChessBox::__revert(int y, int x, char role) {
    #ifdef DEBUG
    assert(map[y][x] == role);
    assert(__check_inbound(y, x));
    assert(role == BLACK || role == WHITE);
    #endif
    const char otherRole = role == BLACK ? WHITE : BLACK;
    for (int i = 0; i < 8; ++i) {
        int nx = x + DIR_X[i];
        int ny = y + DIR_Y[i];
        if (!__check_inbound(ny, nx)) continue;
        char m = map[ny][nx];
        if (m != otherRole) continue;
        bool __revertable = false;
        for (int delta = 2;; delta++) {
            int nnx = x + delta * DIR_X[i];
            int nny = y + delta * DIR_Y[i];
            if (!__check_inbound(nny, nnx)) break;
            char mm = map[nny][nnx];
            if (mm == otherRole) {
                continue;
            } else if (mm == role) {
                __revertable = true;
                break;
            } else {
                #ifdef DEBUG
                assert(mm == NONE || mm == DROP);
                #endif
                break;
            }
        }
        if (__revertable) {
            for (int delta = 1;;delta++) {
                int nnx = x + delta * DIR_X[i];
                int nny = y + delta * DIR_Y[i];
                if (!__check_inbound(y, x)) break;
                if (map[nny][nnx] != otherRole) break;
                map[nny][nnx] = role;
            }
        }
    }
}