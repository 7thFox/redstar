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
        AssertReference(@"
func test(int x, int y) string {
    a := x;
    b := y;
    c := x + y;
    return ""foobar"";
}
            ",
            ((2, 15), (2, 15)),
            ((2, 22), (2, 22)),
            ((3, 10), (2, 15)),
            ((4, 10), (2, 22)),
            ((5, 10), (2, 15)),
            ((5, 14), (2, 22)));
    }

    private static void AssertReference(string source, params ((int, int), (int,int))[] references)
    {
        var st = new TestRun()
            .AddSource("test.red", source)
            .Exectue();

        foreach ((var toTest, var referencedDefinition) in references)
        {
            var symbol = st.FastFind(new Location()
            {
                FileName = "test.red",
                Line = toTest.Item1,
                Column = toTest.Item2,
            });

            Assert.IsNotNull(symbol);
            Assert.AreEqual("test.red", symbol.Declaration.FileName);
            Assert.AreEqual(referencedDefinition, (symbol.Declaration.Line, symbol.Declaration.Column));
        }
    }
}
