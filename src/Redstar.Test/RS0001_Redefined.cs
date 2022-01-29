using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Linq;

namespace Redstar.Test;

[TestClass]
public class RS0001_Redefined
{
    [TestMethod]
    public void RedefinedSameScope()
    {
        new TestRun()
            .AddSource("test.red",@"
x := 1;
[|RS0001||x := 2|];
            ")
            .Exectue();
    }

    [TestMethod]
    public void RedefinedSubScopeAfter()
    {
        new TestRun()
            .AddSource("test.red",@"
x := 1;

func foobar() {
    [|RS0001||x := 2|];
}
            ")
            .Exectue();
    }

    [TestMethod]
    public void RedefinedSubScopeBefore()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar() {
    [|RS0001||x := 2|];
}

x := 1;
            ")
            .Exectue();
    }

    [TestMethod]
    public void NegativeSiblingScopes()
    {
        new TestRun()
            .AddSource("test.red",@"
func foo() {
    x := 2;
}

func bar() {
    x := 2;
}
            ")
            .Exectue();
    }

    [TestMethod]
    public void RedefinedParamAfter()
    {
        new TestRun()
            .AddSource("test.red",@"
x := 1;

func foobar(string [|RS0001||x|]) {
}
            ")
            .Exectue();
    }

    [TestMethod]
    public void RedefinedParamBefore()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar(string [|RS0001||x|]) {
}

x := 1;
            ")
            .Exectue();
    }


    [TestMethod]
    public void RedefinedParamInner()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar(string x) {
    [|RS0001||x := 1|];
}
            ")
            .Exectue();
    }
}