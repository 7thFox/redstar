x := 0;// 1

func foo(y: int) int {// 2 ( 5 )
    return x + y; // 1 + 5
}

func foobar(y: int, z: int) int { // 3 ( 6 7 )
    func baz(v: int) {// 8 ( 11 )
        return v + 3; // 11
    }
    w := foo(y);// 9 = 2 6
    u := baz(w);// 10 = 8 ( 9 )
    return w + u + y + z + x;// 9 10 6 7 1
}

func foobaz(u: int) {// 4 ( 12 )
    func baz(v: int) int { // 13 ( 16 )
        return u;// 12
    }

    func w() int {// 14
        return 3;
    }

    v := w() + x + u;// 15 = 14 1 12
}