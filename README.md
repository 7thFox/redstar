# redstar

Experimental language focused on the of developer-defined compile-time constraints.

# example

```
use core/rand

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
// y := divide(3, x);
// z := 3 / x;

if (x != 0) {
    anno x [NonZero];// add [NonZero] annotation to x

    y := divide(3, x);
    z := 3 / x;
}
```
