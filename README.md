# redstar

Experimental language focused on the of developer-defined compile-time constraints.

# example

```
use core/rand

attr NonZero

func divide(num: int, denom: [NonZero] int) int {// aliased by compiler?
    return num / denom;
}

x := rand_int();

if (x != 0) {
    [NonZero] x;

    y := divide(3, x);
    z := 3 / x;
}
```
