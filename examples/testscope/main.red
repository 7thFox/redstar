x := 0;// 1

x := 1;// 2 Error

func foo(y: int) int {// 3 ( 4 )
    return x + y; // 1 + 4
}

func foobar(y: int, z: int) int { // 5 ( 6 7 )
    func baz(v: int) {// 8 ( 9 )
        return v + 3; // 9
    }
    foo := 1;// 10 ERROR
    w := foo(y);// 11 = 3 6
    u := baz(w);// 12 = 8 ( 10 )
    return w + u + y + z + x;// 10 12 6 7 2
}

func foobaz(u: int) {// 13 ( 14 )
    func baz(v: int) int { // 15 ( 16 ERROR )
        return u;// 14
    }

    func w() int {// 17
        return 3;
    }

    v := w() + x + u;// 18 = 15 1 14
}