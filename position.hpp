#pragma once

#include "board.hpp"

struct Position {
    u64 player;
    u64 wall;
    u64 up;
    u64 down;
    u64 left;
    u64 right;
    u64 piece[9];
    Position() : player(), wall(), up(), down(), left(), right(), piece() {}
};

inline Position pos_read(const string& s) {
    istringstream in(s);
    Position pos;

    int player_y,player_x; in >> player_y >> player_x;
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
    out << player_y << ' ' << player_x << '\n';
    for(const auto& s : grid)
        out << s << '\n';
    return out.str();
}

inline u64 pos_area(const Position& pos) {
    u64 boundary = pos.wall | pos.up | pos.down | pos.left | pos.right;
    u64 seed = board_neighbor(pos.player);
    return board_floodfill(boundary, seed);
}


