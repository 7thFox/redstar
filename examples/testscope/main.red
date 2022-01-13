x := 0;// 1

x := 1;// 2 ERROR
// SCOPE 1
func foo(y: int) int {// 3 ( 4 )
    // SCOPE 3
    return x + y; // 1 + 4
}

func foobar(y: int, z: int) int { // 5 ( 6 7 )
    // SCOPE 5
    func baz(v: int) {// 8 ( 9 )
        // SCOPE 7
        return v + 3; // 9
    }
    // SCOPE 5
    foo := 1;// 10 ERROR
    w := foo(y);// 11 = 3 6
    u := baz(w);// 12 = 8 ( 10 )
    return w + u + y + z + x;// 10 12 6 7 2
}

func foobaz(u: int) {// 13 ( 14 )
    // SCOPE 9
    func baz(v: int) int { // 15 ( 16 ERROR )
        // SCOPE 11
        return u;// 14
    }
    // SCOPE 9
    func w() int {// 17
        // SCOPE 10
        return 3;
    }
    // SCOPE 9
    v := w() + x + u;// 18 = 15 1 14
}