#pragma once

#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"

class Position {
public:
    Position(u64 player, const Board& board) noexcept : player_(player), board_(board) {}

    u64 player() const noexcept { return player_; }
    const Board& board() const noexcept { return board_; }

    Board::Result result() const noexcept {
        return board_.result();
    }

    // (area,up,down,left,right)
    tuple<u64,u64,u64,u64,u64> moves() const noexcept {
        return board_.moves(player_);
    }

    void move_up(u64 point) noexcept {
        player_ = point;
        int x = bitboard_bsf(point) % 8;
        board_.rotate_up(x);
    }

    void move_down(u64 point) noexcept {
        player_ = point;
        int x = bitboard_bsf(point) % 8;
        board_.rotate_down(x);
    }

    void move_left(u64 point) noexcept {
        player_ = point;
        int y = bitboard_bsf(point) / 8;
        board_.rotate_left(y);
    }

    void move_right(u64 point) noexcept {
        player_ = point;
        int y = bitboard_bsf(point) / 8;
        board_.rotate_right(y);
    }

    int least_to_solve() const noexcept {
        return board_.least_to_solve();
    }

    void write_to(ostream& out) const {
        int player_i = bitboard_bsf(player_);
        int player_y,player_x; tie(player_y,player_x) = idx2yx(player_i);

        out << player_x << ' ' << player_y << '\n';
        board_.write_to(out);
    }

    static Position read_from(istream& in) {
        int player_y,player_x; in >> player_x >> player_y;
        int player_i = yx2idx(player_y, player_x);

        u64 player = 1ULL << player_i;
        auto board = Board::read_from(in);

        return Position(player, board);
    }

private:
    u64 player_;
    Board board_;
};
