#pragma once

#include <bits/stdc++.h>

using namespace std;

#define CPP_STR(x) CPP_STR_I(x)
#define CPP_CAT(x,y) CPP_CAT_I(x,y)
#define CPP_STR_I(args...) #args
#define CPP_CAT_I(x,y) x ## y

#define ASSERT(expr...) assert((expr))

using i8   = int8_t;
using u8   = uint8_t;
using i16  = int16_t;
using u16  = uint16_t;
using i32  = int32_t;
using u32  = uint32_t;
using i64  = int64_t;
using u64  = uint64_t;

using f32 = float;
using f64 = double;

#define FOR(i, start, end) for(int i = (start), CPP_CAT(i,xxxx_end)=(end); i < CPP_CAT(i,xxxx_end); ++i)
#define REP(i, n) FOR(i, 0, n)

template<typename T>
void DBG_IMPL(int line, const char* expr, const T& value) {
    cerr << "[L " << line << "]: ";
    cerr << expr << " = \n";
    cerr << value << "\n";
    cerr << "\n";
}
#define NO_DBG
#ifdef NO_DBG
    #define DBG(expr)
#else
    #define DBG(expr) DBG_IMPL(__LINE__, CPP_STR(expr), (expr))
#endif

inline string read_all(istream& in) {
    ostringstream out;
    out << in.rdbuf();
    return out.str();
}

template<typename T>
string to_str(const T& x) {
    ostringstream out;
    x.write_to(out);
    return out.str();
}

template<typename T>
T from_str(const string& input) {
    istringstream in(input);
    return T::read_from(in);
}

inline u64 get_ticks_ms() {
    static const auto START = chrono::high_resolution_clock::now();
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now-START).count();
}

template<typename F>
inline u64 bench(F&& f) {
    u64 t1 = get_ticks_ms();
    f();
    u64 t2 = get_ticks_ms();
    return t2 - t1;
}

inline pair<int,int> idx2yx(int i) {
    return { i/8, i%8 };
}

inline int yx2idx(int y, int x) {
    return 8*y + x;
}
