using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Linq;
using Redstar.Symbols;
using System.Security.AccessControl;

namespace Redstar.Test;

[TestClass]
public class TypedSymbolTests
{
    private const string TESTING_FILE_NAME = "test.red";

    [TestMethod]
    public void Explicit_Primitive_FuncParam()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
func foo(x: int) {
}")
            .Execute();

        AssertImplicitType(st, "int", 2, 10);
    }

    [TestMethod]
    public void Explicit_Primitive_LocalDecl()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
x : int = 0;
")
            .Execute();

        AssertImplicitType(st, "int", 2, 1);
    }

    [TestMethod]
    public void Implicit_Primitive_LocalDecl_Literal()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
x := 0;
")
            .Execute();

        AssertImplicitType(st, "int", 2, 1);
    }

    [TestMethod]
    public void Implicit_Primitive_LocalDecl_BinaryExp()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
x := 1 * 2 + 3;
")
            .Execute();

        AssertImplicitType(st, "int", 2, 1);
    }

    [TestMethod]
    public void Implicit_Primitive_LocalDecl_SameScopeLocal()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
y := 0;
x := 3 * y + 5;
")
            .Execute();

        AssertImplicitType(st, "int", 3, 1);
    }

    [TestMethod]
    public void Implicit_Primitive_LocalDecl_ParentScopeLocal()
    {
        var st = new TestRun()
            .AddSource(TESTING_FILE_NAME, @"
y := 0;
func foo() {
    x := 3 * y + 5;
}
")
            .Execute();

        AssertImplicitType(st, "int", 4, 5);
    }

    private void AssertImplicitType(SymbolTable st, string implicitName, int line, int col)
    {
        var implSymbol = st.FastFind(implicitName);
        Assert.IsNotNull(implSymbol, $"Could not get '{implicitName}' symbol");

        var testSymbol = st.FastFind(new Location()
        {
            FileName = TESTING_FILE_NAME,
            Line = line,
            Column = col,
        });
        Assert.IsNotNull(testSymbol,"Could not get tested symbol");

        if (!(testSymbol is ITypedSymbol testTypedSymbol))
        {
            Assert.Fail("Symbol does not inherit ITypedSymbol");
            return;
        }

        Assert.IsNotNull(testTypedSymbol.Type, "Type annotation not added to symbol");

        Assert.AreEqual(implSymbol!.ID, testTypedSymbol.Type!.ID, "Type did not match");
    }
}
