#pragma once

#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"
#include "position.hpp"
#include "tt.hpp"

using namespace std;

inline string sol_str(const vector<u64>& sol) {
    ostringstream out;
    for(u64 e : sol) {
        out << bitboard_bsf(e) << " ";
    }
    return out.str();
}

class Solver {
public:
    Solver() {
        solution_.reserve(30);
        solution_all_.reserve(1000);
    }

    vector<vector<u64>> solve(const Position& pos) {
        tt_.clear();
        solution_.clear();
        solution_all_.clear();

        for(depth_max_ = 0; ; ++depth_max_) {
            //if(depth_max_ > 10) break;  // for gprof
            node_count_ = 0;
            tt_hit_count_ = 0;

            cerr << "Depth " << depth_max_ << ": ";

            u64 dur = bench([this,&pos]() { dfs(pos,0); });
            if(dur == 0) dur = 1;
            f64 nps = round(1000.0 * f64(node_count_) / f64(dur));

            cerr << "Nodes=" << node_count_ << ", "
                 << "Time=" << f64(dur)/1000.0 << ", "
                 << "NPS=" << nps << ", "
                 << "TTHit=" << tt_hit_count_ << ", "
                 << "TTRate=" << tt_.entry_count() << "/" << TranspositionTable::BUCKET_COUNT << "\n";

            if(!solution_all_.empty()) break;
        }

        return solution_all_;
    }

private:
    void dfs(const Position& pos, int depth) {
        ++node_count_;
        int lts = pos.least_to_solve();
        if(lts == 0) {
            solution_all_.emplace_back(solution_);
            return;
        }
        if(depth+lts > depth_max_) return;

        u64 ar,up,down,left,right; tie(ar,up,down,left,right) = pos.moves();

        bool tt_hit = tt_.check(depth, ar, pos.board().bbs());
        if(tt_hit) {
            ++tt_hit_count_;
            return;
        }

        while(up != 0) {
            u64 point = up & (-up);
            up &= up-1;
            //up &= ~point;
            solution_.emplace_back(point);

            Position pos2(pos);
            pos2.move_up(point);
            dfs(pos2, depth+1);

            solution_.pop_back();
        }

        while(down != 0) {
            u64 point = down & (-down);
            down &= down-1;
            //down &= ~point;
            solution_.emplace_back(point);

            Position pos2(pos);
            pos2.move_down(point);
            dfs(pos2, depth+1);

            solution_.pop_back();
        }

        while(left != 0) {
            u64 point = left & (-left);
            left &= left-1;
            //left &= ~point;
            solution_.emplace_back(point);

            Position pos2(pos);
            pos2.move_left(point);
            dfs(pos2, depth+1);

            solution_.pop_back();
        }

        while(right != 0) {
            u64 point = right & (-right);
            right &= right-1;
            //right &= ~point;
            solution_.emplace_back(point);

            Position pos2(pos);
            pos2.move_right(point);
            dfs(pos2, depth+1);

            solution_.pop_back();
        }
    }

    TranspositionTable tt_;
    int depth_max_;
    u64 node_count_;
    u64 tt_hit_count_;
    vector<u64> solution_;
    vector<vector<u64>> solution_all_;
};
