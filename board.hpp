#pragma once

/**
 * bitboard
 *
 * 座標系:
 *   01234567
 *   89ABCDEF
 *   ...
 */

#include "common.hpp"

inline string board_str(u64 board) {
    ostringstream out;
    REP(i, 64) {
        out << ((board&(1ULL<<i)) ? '1' : '0');
        if(i%8 == 7) out << '\n';
    }
    return out.str();
}

// 上下左右いずれかに隣接する1があれば1
inline u64 board_neighbor(u64 board) {
    return ((board&0x7F7F7F7F'7F7F7F7F)<<1) |
           ((board&0xFEFEFEFE'FEFEFEFE)>>1) |
           (board<<8) | (board>>8);
}

// 上下左右に2つ以上繋がっていれば1
inline u64 board_connected(u64 board) {
    return board & board_neighbor(board);
}

// 単一bitからインデックスへの変換用
// pos != 0 を仮定
inline int board_bsf(u64 board) {
    return __builtin_ctzll(board);
}

// 列 x を座標系基準で上ローテート
inline u64 board_rotate_up(u64 board, int x) {
    u64 mask = 0x01010101'01010101 << x;
    u64 tmp = __rorq(board, 8);
    return (tmp&mask) | (x&~mask);
}

// 列 x を座標系基準で下ローテート
inline u64 board_rotate_down(u64 board, int x) {
    u64 mask = 0x01010101'01010101 << x;
    u64 tmp = __rolq(board, 8);
    return (tmp&mask) | (x&~mask);
}

// 行 y を座標系基準で左ローテート
inline u64 board_rotate_left(u64 board, int y) {
    u64 tmp = __rorq(board, 8*y);
    u64 b   = __rorb(u8(tmp), 1);
    return __rolq(b|(tmp&~0xFF), 8*y);
}

// 行 y を座標系基準で右ローテート
inline u64 board_rotate_right(u64 board, int y) {
    u64 tmp = __rorq(board, 8*y);
    u64 b   = __rolb(u8(tmp), 1);
    return __rolq(b|(tmp&~0xFF), 8*y);
}

// seed から到達できる領域を返す(境界含む)
inline u64 board_floodfill(u64 board, u64 seed) {
    u64 cur = seed & (seed ^ board);
    for(;;) {
        u64 tmp = cur | board_neighbor(cur);
        u64 nex = tmp & (tmp ^ board);
        if(cur == nex)
            return tmp;
        cur = nex;
    }
}


