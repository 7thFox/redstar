attr NonZero

// Binds (bind) and annotations (anno) tie attributes to function signatures
// including operators. Binds define what is required for a function call to
// be valid, while annotations simply define rules for the return type if
// the type annotations match.

// Both statements are in the format:
// ( bind | anno ) func_name attr_list(s) => attr_list
// where the attribute lists follow the same order as the parameters.

// Parameters with no requirement are represented using an _. Returns without
// a definition or void returns are defined by "=> _" may be dropped. Without
// a bind or anno defined for a certain pattern or function, the return is
// unannotated.

// Require 2nd parameter of "/" operator to have an annotation for [NonZero]
bind / _, [NonZero]
// or bind / _, [NonZero] => _

// define return types for * and /
anno * [NonZero], [NonZero] => [NonZero]
anno / [NonZero], [NonZero] => [NonZero]

// there are some short-hands to avoid specifying each parameter annotation
// individually: any, all, and none. They work as one would expect and may not be
// combined with specifying them by position.
attr Even;
anno * any [Even] => [Even]

attr Odd
anno * all => [Odd]

// _ may be used as a wildcard to specify all functions and operators as a default
// unless specified for that func/op.
attr Insecure
anno _ any => [Insecure]
bind db_search none [Insecure] => _
func db_search(x: string, y: string) string {}// just pretend this is more than it looks

// Here's what a typical func decl might look like:
anno multiply all [NonZero] => [NonZero]
anno multiply any [Even] => [Even]
anno multiply all [Odd] => [Odd]
func multiply(x: int, y: int) int {
    return x * y;
}

func foo() {

    // Attribute annotations may be placed explicitly when declaring a variable:
    x : [Odd, NonZero] int = 3;

    // You can also specify the annotations later via the anno statement:
    y := rand();
    if (y % 2 == 0) {
        [Even] y;
        // ...
    }
    else {
        // anno can define multiple attributes to annotate to a variable at once.
        [Odd, NonZero] y;
        // ...
    }

    // Because annotations are checked at compile time, the annotations are
    // removed once leaving the scope of the anno statement, so y currently has
    // no annotations on it.

    // anno can also specify multiple variables to apply annotations to.
    [NonZero] x, y;

    // finally, annotations can be removed as part of an anno statement by
    // pre-fixing the attribute with a -.
    [-Insecure] y;
}
