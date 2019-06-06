#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def idx2yx(i):
    return i//8, i%8

def least_to_solve(y1, x1, y2, x2):
    # y1 <= y2, x1 <= x2 に揃える
    if y1 > y2: y1,y2=(y2,y1)
    if x1 > x2: x1,x2=(x2,x1)

    dy = min((y1-y2)%8, (y2-y1)%8)
    dx = min((x1-x2)%8, (x2-x1)%8)
    dist = dy + dx

    # "1..<...1" みたいなのは距離1でも1手必要
    if dist == 1 and ((x1==0 and x2==7) or (y1==0 and y2==7)):
        return 1
    elif dist == 0:
        return 0
    else:
        return dist - 1

def main():
    table = [64*[None] for _ in range(64)]

    for i1 in range(64):
        for i2 in range(64):
            y1,x1 = idx2yx(i1)
            y2,x2 = idx2yx(i2)
            table[i1][i2] = least_to_solve(y1,x1,y2,x2)

    print("static constexpr u8 LEAST_TO_SOLVE[64][64] {")
    for row in table:
        print(f"    {{ {','.join(map(str,row))} }},")
    print("};")

    #print("const DIST: [[u32; 64]; 64] = [")
    #for i in range(64):
    #    print("[{}],".format(",".join(map(str,table[i]))))
    #print("];")

if __name__ == "__main__": main()
