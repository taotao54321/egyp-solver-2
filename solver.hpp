#pragma once

#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"
#include "position.hpp"

using namespace std;

// XXX: ヘッダ内グローバル変数
int depth_max;
u64 node_count;
vector<u64> solution;
vector<vector<u64>> solution_all;

inline string sol_str(const vector<u64>& sol) {
    ostringstream out;
    for(u64 e : sol) {
        out << board_bsf(e) << " ";
    }
    return out.str();
}

inline void dfs(const Position& pos, int depth) {
    ++node_count;
    PositionResult pr = pos_result(pos);
    DBG(depth);
    DBG(pos_str(pos));
    DBG(pr);
    if(pr == SOLVED) {
        solution_all.emplace_back(solution);
        return;
    }
    if(depth+1 > depth_max) return;
    if(pr == STUCK) return;

    DBG(board_str(pos_area(pos)));
    u64 up,down,left,right; tie(up,down,left,right) = pos_moves(pos);
    DBG(board_str(up));
    DBG(board_str(down));
    DBG(board_str(left));
    DBG(board_str(right));

    while(up != 0) {
        u64 point = up & (-up);
        solution.emplace_back(point);
        dfs(pos_move_up(pos,point), depth+1);
        solution.pop_back();
        up &= ~point;
    }

    while(down != 0) {
        u64 point = down & (-down);
        solution.emplace_back(point);
        dfs(pos_move_down(pos,point), depth+1);
        solution.pop_back();
        down &= ~point;
    }

    while(left != 0) {
        u64 point = left & (-left);
        solution.emplace_back(point);
        dfs(pos_move_left(pos,point), depth+1);
        solution.pop_back();
        left &= ~point;
    }

    while(right != 0) {
        u64 point = right & (-right);
        solution.emplace_back(point);
        dfs(pos_move_right(pos,point), depth+1);
        solution.pop_back();
        right &= ~point;
    }
}

inline void solve(const Position& pos) {
    solution.reserve(30);
    solution_all.reserve(1000);

    for(depth_max = 0; ; ++depth_max) {
        //if(depth_max > 10) break;  // for gprof
        node_count = 0;

        cerr << "Depth " << depth_max << ": ";

        u64 dur = bench([&pos]() { dfs(pos,0); });
        if(dur == 0) dur = 1;
        f64 nps = 1000.0 * f64(node_count) / f64(dur);

        cerr << "Nodes=" << node_count << ", "
             << "TimeMS=" << dur << ", "
             << "NPS=" << nps << "\n";

        if(!solution_all.empty()) break;
    }
}
