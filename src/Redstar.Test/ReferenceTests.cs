using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Linq;
using Redstar.Symbols;
using System.Security.AccessControl;

namespace Redstar.Test;

[TestClass]
public class ReferenceTests
{
    [TestMethod]
    public void SameScope()
    {
        AssertReference(@"
x := 1;
y := x;
            ",
            ((2, 1), (2, 1)),
            ((3, 6), (2, 1)));
    }

    [TestMethod]
    public void FunctionParameter()
    {
        var xDef = (2, 11);
        var yDef = (2, 19);

        AssertReference(@"
func test(x: int, y: int) string {
    a := x;
    b := y;
    c := x + y;
    return ""foobar"";
}",
            (xDef, xDef),
            (yDef, yDef),
            ((3, 10), xDef),
            ((4, 10), yDef),
            ((5, 10), xDef),
            ((5, 14), yDef));
    }


    [TestMethod]
    public void FunctionWithOuterScope()
    {
        var xDef = (2, 1);
        var yDef = (4, 11);
        var zDef = (5, 5);

        AssertReference(@"
x := 5;

func test(y: int) string {
    z := 3;

    a := y + x + z;

    return ""foobar"";
}

b := x;
c := [|RS0002||y|] + [|RS0002||z|];
",
            (xDef, xDef),
            (yDef, yDef),
            (zDef, zDef),
            ((7, 10), yDef),
            ((7, 14), xDef),
            ((7, 18), zDef),
            ((12, 6), xDef));
    }

    private static void AssertReference(string source, params ((int, int), (int,int)?)[] references)
    {
        var st = new TestRun()
            .AddSource("test.red", source)
            .Execute();

        foreach ((var toTest, var referencedDefinition) in references)
        {
            var symbol = st.FastFind(new Location()
            {
                FileName = "test.red",
                Line = toTest.Item1,
                Column = toTest.Item2,
            });

            Assert.IsNotNull(symbol, $"{toTest}: no symbol found");
            if (referencedDefinition == null)
            {
                Assert.IsTrue(symbol.Declaration.IsImplicit);
            }
            else
            {
                Assert.IsFalse(symbol.Declaration.IsImplicit);
                Assert.AreEqual("test.red", symbol.Declaration.FileName);
                Assert.AreEqual(referencedDefinition, (symbol.Declaration.Line, symbol.Declaration.Column));
            }
        }
    }
}
