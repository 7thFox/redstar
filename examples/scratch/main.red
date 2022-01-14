x := 1 + 2 + 3 + 5;
y := "this is a string";
z := 1 + "test" + 3 + 3;
xx := 1 + x;

a := (b := "foo");

func foobar() int {
    return "foobar";
    return 3;
}

n := foobar();

func bar(p1 : int) string {

}

bar(n);
bar(y);// ERROR
bar(foobar());
bar(foobar);
