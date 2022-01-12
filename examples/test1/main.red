attr NonZero

// require denom to be annotated with [NonZero]
bind divide _, [NonZero]// implied => _
func divide(num: int, denom: int) int {
    return num / denom;
}

// we can define the same constraint to the / operator
bind / _, [NonZero]

x := rand_int();

// will fail at compile-time because x doesn't have the correct annotations:
y_fail := divide(3, x);
z_fail := 3 / x;

if (x != 0) {
    [NonZero] x;// add [NonZero] annotation to x

    // could do:
    // y := divide(3, [NonZero]x);
    y := divide(333, x);
    z := 3 / x;
}
