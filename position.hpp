#pragma once

#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"

using namespace std;

inline u64 pos_area(u64 point, u64 w, u64 u, u64 d, u64 l, u64 r) {
    u64 boundary = w | u | d | l | r;
    u64 seed = board_neighbor(point);
    return board_floodfill(boundary, seed);
}

struct Position {
    u64 area;
    u64 wall;
    u64 up;
    u64 down;
    u64 left;
    u64 right;
    u64 piece[9];
    Position() : area(), wall(), up(), down(), left(), right(), piece() {}
    Position(u64 a, u64 w, u64 u, u64 d, u64 l, u64 r,
             u64 c0, u64 c1, u64 c2, u64 c3, u64 c4, u64 c5, u64 c6, u64 c7, u64 c8)
        : area(a), wall(w), up(u), down(d), left(l), right(r),
          piece{c0,c1,c2,c3,c4,c5,c6,c7,c8} {}

    static Position from_point(
        u64 point, u64 w, u64 u, u64 d, u64 l, u64 r,
        u64 c0, u64 c1, u64 c2, u64 c3, u64 c4, u64 c5, u64 c6, u64 c7, u64 c8)
    {
        return Position {
            pos_area(point,w,u,d,l,r),
            w, u, d, l, r,
            c0, c1, c2, c3, c4, c5, c6, c7, c8
        };
    }
};

enum PositionResult {
    SOLVED,
    STUCK,
    UNKNOWN,
};

inline Position pos_read(const string& s) {
    istringstream in(s);

    int player_x,player_y; in >> player_x >> player_y;
    u64 player_point = 1ULL << yx2idx(player_y,player_x);

    vector<string> grid(8);
    REP(y, 8) {
        in >> grid[y];
    }

    u64 w=0, u=0, d=0, l=0, r=0;
    u64 piece[9] {};
    REP(y, 8) REP(x, 8) {
        char c = grid[y][x];
        int  i = yx2idx(y,x);
        if(c == '#') {
            w |= 1ULL << i;
        }
        else if(c == '^') {
            u |= 1ULL << i;
        }
        else if(c == 'v') {
            d |= 1ULL << i;
        }
        else if(c == '<') {
            l |= 1ULL << i;
        }
        else if(c == '>') {
            r |= 1ULL << i;
        }
        else if('0' <= c && c <= '8') {
            piece[c-'0'] |= 1ULL << i;
        }
    }

    return Position::from_point(
        player_point,
        w, u, d, l, r,
        piece[0], piece[1], piece[2], piece[3], piece[4], piece[5], piece[6], piece[7], piece[8]
    );
}

inline string pos_str(const Position& pos) {
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
    for(const auto& s : grid)
        out << s << '\n';
    return out.str();
}

// UDLR
inline tuple<u64,u64,u64,u64> pos_moves(const Position& pos) {
    return make_tuple(
        pos.area & pos.up,
        pos.area & pos.down,
        pos.area & pos.left,
        pos.area & pos.right
    );
}

inline PositionResult pos_result(const Position& pos) {
    REP(i, 9) {
        int k = board_popcnt(pos.piece[i]);
        if(k == 1) return STUCK;
        if(k >= 2) return UNKNOWN;
    }
    return SOLVED;
}

inline Position pos_move_make(
    u64 point,
    u64 w, u64 u, u64 d, u64 l, u64 r,
    u64 c0, u64 c1, u64 c2, u64 c3, u64 c4, u64 c5, u64 c6, u64 c7, u64 c8)
{
    u64 boundary = w | u | d | l | r;
    u64 seed = board_neighbor(point);
    u64 area = board_floodfill(boundary, seed);

    // erase
    c0 &= ~board_neighbor(c0);
    c1 &= ~board_neighbor(c1);
    c2 &= ~board_neighbor(c2);
    c3 &= ~board_neighbor(c3);
    c4 &= ~board_neighbor(c4);
    c5 &= ~board_neighbor(c5);
    c6 &= ~board_neighbor(c6);
    c7 &= ~board_neighbor(c7);
    c8 &= ~board_neighbor(c8);

    return Position {
        area, w, u, d, l, r,
        c0, c1, c2, c3, c4, c5, c6, c7, c8
    };
}

inline Position pos_move_up(const Position& pos, u64 point) {
    int x = board_bsf(point) % 8;
    return pos_move_make(
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
        board_rotate_up(pos.piece[8], x)
    );
}

inline Position pos_move_down(const Position& pos, u64 point) {
    int x = board_bsf(point) % 8;
    return pos_move_make(
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
        board_rotate_down(pos.piece[8], x)
    );
}

inline Position pos_move_left(const Position& pos, u64 point) {
    int y = board_bsf(point) / 8;
    return pos_move_make(
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
        board_rotate_left(pos.piece[8], y)
    );
}

inline Position pos_move_right(const Position& pos, u64 point) {
    int y = board_bsf(point) / 8;
    return pos_move_make(
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
        board_rotate_right(pos.piece[8], y)
    );
}

