#pragma once

/**
 * bitboard
 *
 * 座標系:
 *   01234567
 *   89ABCDEF
 *   ...
 */

#include <bits/stdc++.h>

#include <x86intrin.h>

#include "common.hpp"

using namespace std;

inline string bitboard_str(u64 bb) {
    ostringstream out;
    REP(i, 64) {
        out << ((bb&(1ULL<<i)) ? '1' : '0');
        if(i%8 == 7) out << '\n';
    }
    return out.str();
}

// 単一bitからインデックスへの変換用
// bb != 0 を仮定
inline int bitboard_bsf(u64 bb) {
    return __builtin_ctzll(bb);
}

// 上下左右いずれかに隣接する1があれば1
inline u64 bitboard_neighbor(u64 bb) {
    return ((bb&0x7F7F7F7F'7F7F7F7F)<<1) |
           ((bb&0xFEFEFEFE'FEFEFEFE)>>1) |
           (bb<<8) | (bb>>8);
}

// seed から到達できる領域を返す(境界含む)
inline u64 bitboard_floodfill(u64 bb, u64 seed) {
    u64 cur = seed & (seed ^ bb);
    for(;;) {
        u64 tmp = cur | bitboard_neighbor(cur);
        u64 nex = tmp & (tmp ^ bb);
        if(cur == nex)
            return tmp | seed;
        cur = nex;
    }
}

enum Tile {
    TL_U = 0,
    TL_D,
    TL_L,
    TL_R,
    TL_WALL,
    TL_PC0, TL_PC1, TL_PC2, TL_PC3, TL_PC4, TL_PC5, TL_PC6, TL_PC7, TL_PC8,

    TL_FIRST = 0,
    TL_LAST  = 14,
    TL_COUNT = TL_LAST - TL_FIRST,

    TL_DIR_FIRST = 0,
    TL_DIR_LAST  = 4,
    TL_DIR_COUNT = TL_DIR_LAST - TL_DIR_FIRST,

    TL_PC_FIRST = 5,
    TL_PC_LAST  = 14,
    TL_PC_COUNT = TL_PC_LAST - TL_PC_FIRST,
};

class Board {
public:
    enum Result { SOLVED, STUCK, UNKNOWN };

    explicit Board(const u64 (&bbs)[TL_COUNT]) {
        memcpy(bbs_, bbs, 8*TL_COUNT);
    }

    const u64 (&bbs() const)[TL_COUNT] { return bbs_; }

    u64 get(Tile tile) const { return bbs_[tile]; }

    Result result() const {
        FOR(i, TL_PC_FIRST, TL_PC_LAST) {
            int k = __builtin_popcountll(bbs_[i]);
            if(k == 1) return STUCK;
            if(k >= 2) return UNKNOWN;
        }
        return SOLVED;
    }

    // (area,up,down,left,right)
    tuple<u64,u64,u64,u64,u64> moves(u64 point) const {
        u64 ar = area(point);
        return {
            ar,
            bbs_[TL_U] & ar,
            bbs_[TL_D] & ar,
            bbs_[TL_L] & ar,
            bbs_[TL_R] & ar,
        };
    }

    void rotate_up(int x) {
        u64 mask = 0x01010101'01010101 << x;
        for(auto& bb : bbs_) {
            u64 tmp = __rorq(bb, 8);
            bb = (tmp&mask) | (bb&~mask);
        }

        erase_pcs();
    }

    void rotate_down(int x) {
        u64 mask = 0x01010101'01010101 << x;
        for(auto& bb : bbs_) {
            u64 tmp = __rolq(bb, 8);
            bb = (tmp&mask) | (bb&~mask);
        }

        erase_pcs();
    }

    void rotate_left(int y) {
        for(auto& bb : bbs_) {
            u64 tmp = __rorq(bb, 8*y);
            u64 b   = __rorb(u8(tmp), 1);
            bb = __rolq(b|(tmp&~0xFF), 8*y);
        }

        erase_pcs();
    }

    void rotate_right(int y) {
        for(auto& bb : bbs_) {
            u64 tmp = __rorq(bb, 8*y);
            u64 b   = __rolb(u8(tmp), 1);
            bb = __rolq(b|(tmp&~0xFF), 8*y);
        }

        erase_pcs();
    }

    void write_to(ostream& out) const {
        vector<string> grid(8, string(8,SYM_EMPTY));
        REP(y, 8) REP(x, 8) {
            int i = yx2idx(y,x);
            FOR(j, TL_FIRST, TL_LAST) {
                if(bbs_[j] & (1ULL<<i)) {
                    grid[y][x] = SYMS_TL[j-TL_FIRST];
                    break;
                }
            }
        }

        for(const auto& s : grid)
            out << s << '\n';
    }

    static Board read_from(istream& in) {
        vector<string> grid(8);
        REP(y, 8) {
            in >> grid[y];
        }

        u64 bbs[TL_COUNT] {};
        REP(y, 8) REP(x, 8) {
            char c = grid[y][x];
            int  i = yx2idx(y,x);
            FOR(j, TL_FIRST, TL_LAST) {
                char sym = SYMS_TL[j-TL_FIRST];
                if(c == sym) {
                    bbs[j] |= 1ULL << i;
                    break;
                }
            }
        }

        return Board(bbs);
    }

private:
    static constexpr char SYMS_TL[TL_COUNT] {
        '^','v','<','>','#','0','1','2','3','4','5','6','7','8'
    };
    static constexpr char SYM_EMPTY = '.';

    u64 area(u64 point) const {
        u64 boundary = bbs_[TL_U] | bbs_[TL_D] | bbs_[TL_L] | bbs_[TL_R] | bbs_[TL_WALL];
        u64 seed = bitboard_neighbor(point);
        return bitboard_floodfill(boundary, seed);
    }

    void erase_pcs() {
        FOR(i, TL_PC_FIRST, TL_PC_LAST) {
            bbs_[i] &= ~bitboard_neighbor(bbs_[i]);
        }
    }

    u64 bbs_[TL_COUNT];
};
