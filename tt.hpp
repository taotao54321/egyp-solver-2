#pragma once

#include <bits/stdc++.h>

#include "common.hpp"
#include "board.hpp"

using namespace std;

#define CHECK_COLLISION

#ifdef CHECK_COLLISION
struct alignas(128) TTEntry {
    u64 area;
    u64 bbs[14];
#else
struct TTEntry {
#endif
    u8 depth;  // 1-based, 0ならエントリが無効
};

class TranspositionTable {
public:
#ifdef CHECK_COLLISION
    static constexpr size_t BUCKET_COUNT = 1ULL << 23;
#else
    static constexpr size_t BUCKET_COUNT = 1ULL << 30;
#endif

    TranspositionTable()
        : bucket_(reinterpret_cast<TTEntry*>(aligned_alloc(128,sizeof(TTEntry)*BUCKET_COUNT)))
    {
        // どうせ探索開始時に clear() するので
        //clear();
    }
    ~TranspositionTable() {
        free(bucket_);
    }

    void clear() {
        memset(bucket_, 0, sizeof(TTEntry)*BUCKET_COUNT);
    }

    // 探索を打ち切ってよければ true を返す
    // そうでない場合、適当にテーブルを更新して false を返す
    bool check(int depth, u64 area, const u64 (&bbs)[TL_COUNT]) {
        u64 idx = hash_position(area, bbs) & (BUCKET_COUNT-1);
        auto& entry = bucket_[idx];

        auto write_entry = [depth,area,&bbs,&entry] {
#ifdef CHECK_COLLISION
            entry.area = area;
            memcpy(entry.bbs, bbs, 8*TL_COUNT);
#endif
            entry.depth = u8(depth + 1);
        };

        // エントリが空なら現局面を格納して探索続行
        if(entry.depth == 0) {
            write_entry();
            return false;
        }

        // エントリの深さが depth 以上なら探索続行
        // このときエントリの深さが depth より真に大きいなら現局面で上書き
        if(depth <= entry.depth-1) {
            if(depth < entry.depth-1)
                write_entry();
            return false;
        }

        // エントリの深さが depth より小さいため、探索を打ち切れる可能性がある

#ifdef CHECK_COLLISION
        // ハッシュ衝突した場合何もせず探索続行(深さの小さい局面を残す)
        bool same = area == entry.area &&
                    equal(begin(bbs), end(bbs), begin(entry.bbs), end(entry.bbs));
        if(!same) return false;
#endif

        return true;
    }

    size_t entry_count() const {
        return count_if(bucket_, bucket_+BUCKET_COUNT, [](const auto& e) { return e.depth != 0; });
    }

private:
    static u64 hash_position(u64 area, const u64(&bbs)[TL_COUNT]) {
        u64 h = area;
        for(auto bb : bbs) {
            h = hash_combine(h,bb);
        }
        return h;
    }

    static u64 hash_combine(u64 h, u64 x) {
        static constexpr u64 M = 0xc6a4a7935bd1e995;
        static constexpr int R = 47;

        x *= M;
        x ^= x >> R;
        x *= M;

        h ^= x;
        h *= M;
        h += 0xe6546b64;

        return h;
    }

    TTEntry* const bucket_;
};
