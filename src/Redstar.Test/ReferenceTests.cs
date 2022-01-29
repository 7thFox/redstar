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
        var st = new TestRun()
            .AddSource("test.red",@"
x := 1;
y := x;
            ")
            .Exectue();

        var symbol = st.FastFind(new Location()
        {
            FileName = "test.red",
            Line = 3,
            Column = 6,
        });

        Assert.IsNotNull(symbol);
        new Location()
        {
            FileName = "test.red",
            Line = 2,
            Column = 1,
        }.AssertLocation(symbol.Declaration);
    }
}

public static class TestingExtensions
{
    public static void AssertLocation(this Location expected, Location actual)
    {
        Assert.AreEqual(expected.FileName, actual.FileName, "FileName");
        Assert.AreEqual(expected.Line, actual.Line, "Line");
        Assert.AreEqual(expected.Column, actual.Column, "Column");
    }
}