x := 0;// 1

x := 1;// Error

func foo(y: int) int {// 2 ( 3 )
    return x + y; // 1 + 3
}

func foobar(y: int, z: int) int { // 4 ( 5 6 )
    func baz(v: int) {// 7 ( 8 )
        return v + 3; // 8
    }
    w := foo(y);// 9 = 2 5
    u := baz(w);// 10 = 7 ( 9 )
    return w + u + y + z + x;// 9 10 5 6 1
}

func foobaz(u: int) {// 11 ( 12 )
    func baz(v: int) int { // 13 ( 14 ) ERROR (v)
        return u;// 12
    }

    func w() int {// 15
        return 3;
    }

    v := w() + x + u;// 16 = 15 1 12
}