#include <bits/stdc++.h>

#include "board.hpp"
#include "common.hpp"
#include "position.hpp"
#include "solver.hpp"

using namespace std;





void test1() {
    const string input(1+R"(
5 3
#.<.....
.>...#0.
.#.^.0##
1#.#.#.1
..v.#...
...2#.v.
...#....
...2#.<<
)");

    Position pos = pos_read(input);
    ASSERT(pos_str(pos) == 1+R"(
#.<.....
.>...#0.
.#.^.0##
1#.#.#.1
..v.#...
...2#.v.
...#....
...2#.<<
)");
    ASSERT(board_str(pos.wall) == 1+R"(
10000000
00000100
01000011
01010100
00001000
00001000
00010000
00001000
)");
    ASSERT(board_str(pos.up) == 1+R"(
00000000
00000000
00010000
00000000
00000000
00000000
00000000
00000000
)");
    ASSERT(board_str(pos.down) == 1+R"(
00000000
00000000
00000000
00000000
00100000
00000010
00000000
00000000
)");
    ASSERT(board_str(pos.left) == 1+R"(
00100000
00000000
00000000
00000000
00000000
00000000
00000000
00000011
)");
    ASSERT(board_str(pos.right) == 1+R"(
00000000
01000000
00000000
00000000
00000000
00000000
00000000
00000000
)");
    ASSERT(board_str(pos.piece[0]) == 1+R"(
00000000
00000010
00000100
00000000
00000000
00000000
00000000
00000000
)");
    ASSERT(board_str(pos.piece[1]) == 1+R"(
00000000
00000000
00000000
10000001
00000000
00000000
00000000
00000000
)");
    ASSERT(board_str(pos.piece[2]) == 1+R"(
00000000
00000000
00000000
00000000
00000000
00010000
00000000
00010000
)");
    ASSERT(all_of(begin(pos.piece)+3, end(pos.piece), [](u64 e) { return e == 0; }));

    ASSERT(board_str(pos.area) == 1+R"(
00111111
01111111
01111111
01111111
00101111
00001111
00011111
00001111
)");






}

void test2() {
    const string input(1+R"(
0 1
########
.>######
######v#
0#0#####
#0######
#^######
#####<##
########
)");

    Position pos = pos_read(input);
    ASSERT(pos_str(pos) == 1+R"(
########
.>######
######v#
0#0#####
#0######
#^######
#####<##
########
)");
    ASSERT(board_str(pos.area) == 1+R"(
10000000
01000000
10000000
00000000
00000000
00000000
00000000
00000000
)");
}

int test() {
    test1();
    test2();

    return 0;
}

int main(int argc, char** argv) {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed;
    cerr << fixed;

    if(argc == 2 && strcmp(argv[1],"-test") == 0)
        return test();

    string input = read_all(cin);
    Position pos = pos_read(input);

    solve(pos);

    for(const auto& sol : solution_all) {
        cout << sol_str(sol) << "\n";
    }

    return 0;
}
