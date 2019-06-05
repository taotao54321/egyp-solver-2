#pragma once

#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"

using namespace std;

struct Position {
    u64 player;
    u64 wall;
    u64 up;
    u64 down;
    u64 left;
    u64 right;
    u64 piece[9];
    Position() : player(), wall(), up(), down(), left(), right(), piece() {}
    Position(u64 p, u64 w, u64 u, u64 d, u64 l, u64 r,
             u64 c0, u64 c1, u64 c2, u64 c3, u64 c4, u64 c5, u64 c6, u64 c7, u64 c8)
        : player(p), wall(w), up(u), down(d), left(l), right(r),
          piece{c0,c1,c2,c3,c4,c5,c6,c7,c8} {}
};

enum PositionResult {
    SOLVED,
    STUCK,
    UNKNOWN,
};

inline Position pos_read(const string& s) {
    istringstream in(s);
    Position pos;

    int player_x,player_y; in >> player_x >> player_y;
    pos.player = 1ULL << yx2idx(player_y,player_x);

    vector<string> grid(8);
    REP(y, 8) {
        in >> grid[y];
    }

    REP(y, 8) REP(x, 8) {
        char c = grid[y][x];
        int  i = yx2idx(y,x);
        if(c == '#') {
            pos.wall |= 1ULL << i;
        }
        else if(c == '^') {
            pos.up |= 1ULL << i;
        }
        else if(c == 'v') {
            pos.down |= 1ULL << i;
        }
        else if(c == '<') {
            pos.left |= 1ULL << i;
        }
        else if(c == '>') {
            pos.right |= 1ULL << i;
        }
        else if('0' <= c && c <= '8') {
            pos.piece[c-'0'] |= 1ULL << i;
        }
    }

    return pos;
}

inline string pos_str(const Position& pos) {
    int player_i = board_bsf(pos.player);
    int player_y,player_x; tie(player_y,player_x) = idx2yx(player_i);

    vector<string> grid(8, string(8,'.'));

    REP(y, 8) REP(x, 8) {
        int i = yx2idx(y,x);
        if(pos.wall & (1ULL<<i)) {
            grid[y][x] = '#';
        }
        else if(pos.up & (1ULL<<i)) {
            grid[y][x] = '^';
        }
        else if(pos.down & (1ULL<<i)) {
            grid[y][x] = 'v';
        }
        else if(pos.left & (1ULL<<i)) {
            grid[y][x] = '<';
        }
        else if(pos.right & (1ULL<<i)) {
            grid[y][x] = '>';
        }
        else {
            REP(k, 9) {
                if(pos.piece[k] & (1ULL<<i)){
                    grid[y][x] = static_cast<char>('0' + k);
                    break;
                }
            }
        }
    }

    ostringstream out;
    out << player_x << ' ' << player_y << '\n';
    for(const auto& s : grid)
        out << s << '\n';
    return out.str();
}

inline u64 pos_area(const Position& pos) {
    u64 boundary = pos.wall | pos.up | pos.down | pos.left | pos.right;
    u64 seed = board_neighbor(pos.player);
    return board_floodfill(boundary, seed);
}

// UDLR
inline tuple<u64,u64,u64,u64> pos_moves(const Position& pos) {
    u64 area = pos_area(pos);
    u64 up    = pos.up & area;
    u64 down  = pos.down & area;
    u64 left  = pos.left & area;
    u64 right = pos.right & area;
    return make_tuple(up, down, left, right);
}

inline PositionResult pos_result(const Position& pos) {
    REP(i, 9) {
        int k = board_popcnt(pos.piece[i]);
        if(k == 1) return STUCK;
        if(k >= 2) return UNKNOWN;
    }
    return SOLVED;
}

inline void pos_erase(Position& pos) {
    for(u64& e : pos.piece) {
        e &= ~board_neighbor(e);
    }
}

inline Position pos_move_up(const Position& pos, u64 point) {
    int x = board_bsf(point) % 8;
    Position res {
        point,
        board_rotate_up(pos.wall, x),
        board_rotate_up(pos.up, x),
        board_rotate_up(pos.down, x),
        board_rotate_up(pos.left, x),
        board_rotate_up(pos.right, x),
        board_rotate_up(pos.piece[0], x),
        board_rotate_up(pos.piece[1], x),
        board_rotate_up(pos.piece[2], x),
        board_rotate_up(pos.piece[3], x),
        board_rotate_up(pos.piece[4], x),
        board_rotate_up(pos.piece[5], x),
        board_rotate_up(pos.piece[6], x),
        board_rotate_up(pos.piece[7], x),
        board_rotate_up(pos.piece[8], x),
    };
    pos_erase(res);
    return res;
}

inline Position pos_move_down(const Position& pos, u64 point) {
    int x = board_bsf(point) % 8;
    Position res {
        point,
        board_rotate_down(pos.wall, x),
        board_rotate_down(pos.up, x),
        board_rotate_down(pos.down, x),
        board_rotate_down(pos.left, x),
        board_rotate_down(pos.right, x),
        board_rotate_down(pos.piece[0], x),
        board_rotate_down(pos.piece[1], x),
        board_rotate_down(pos.piece[2], x),
        board_rotate_down(pos.piece[3], x),
        board_rotate_down(pos.piece[4], x),
        board_rotate_down(pos.piece[5], x),
        board_rotate_down(pos.piece[6], x),
        board_rotate_down(pos.piece[7], x),
        board_rotate_down(pos.piece[8], x),
    };
    pos_erase(res);
    return res;
}

inline Position pos_move_left(const Position& pos, u64 point) {
    int y = board_bsf(point) / 8;
    Position res {
        point,
        board_rotate_left(pos.wall, y),
        board_rotate_left(pos.up, y),
        board_rotate_left(pos.down, y),
        board_rotate_left(pos.left, y),
        board_rotate_left(pos.right, y),
        board_rotate_left(pos.piece[0], y),
        board_rotate_left(pos.piece[1], y),
        board_rotate_left(pos.piece[2], y),
        board_rotate_left(pos.piece[3], y),
        board_rotate_left(pos.piece[4], y),
        board_rotate_left(pos.piece[5], y),
        board_rotate_left(pos.piece[6], y),
        board_rotate_left(pos.piece[7], y),
        board_rotate_left(pos.piece[8], y),
    };
    pos_erase(res);
    return res;
}

inline Position pos_move_right(const Position& pos, u64 point) {
    int y = board_bsf(point) / 8;
    Position res {
        point,
        board_rotate_right(pos.wall, y),
        board_rotate_right(pos.up, y),
        board_rotate_right(pos.down, y),
        board_rotate_right(pos.left, y),
        board_rotate_right(pos.right, y),
        board_rotate_right(pos.piece[0], y),
        board_rotate_right(pos.piece[1], y),
        board_rotate_right(pos.piece[2], y),
        board_rotate_right(pos.piece[3], y),
        board_rotate_right(pos.piece[4], y),
        board_rotate_right(pos.piece[5], y),
        board_rotate_right(pos.piece[6], y),
        board_rotate_right(pos.piece[7], y),
        board_rotate_right(pos.piece[8], y),
    };
    pos_erase(res);
    return res;
}

