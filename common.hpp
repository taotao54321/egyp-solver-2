#pragma once

#include <bits/stdc++.h>

#include <x86intrin.h>

using namespace std;

#define CPP_STR(x) CPP_STR_I(x)
#define CPP_CAT(x,y) CPP_CAT_I(x,y)
#define CPP_STR_I(args...) #args
#define CPP_CAT_I(x,y) x ## y

#define ASSERT(expr...) assert((expr))

using u8  = uint8_t;
using u64 = uint64_t;

#define FOR(i, start, end) for(int i = (start), CPP_CAT(i,xxxx_end)=(end); i < CPP_CAT(i,xxxx_end); ++i)
#define REP(i, n) FOR(i, 0, n)

inline string read_all(istream& in) {
    ostringstream out;
    out << in.rdbuf();
    return out.str();
}

inline pair<int,int> idx2yx(int i) {
    return { i/8, i%8 };
}

inline int yx2idx(int y, int x) {
    return 8*y + x;
}
