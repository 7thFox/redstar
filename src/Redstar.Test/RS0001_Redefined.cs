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
            .Execute();
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
            .Execute();
    }

    [TestMethod]
    public void RedefinedSubScopeBefore()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar() {
    [|RS0001||x := 2|];
}

x := 1;")
            .Execute();
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
            .Execute();
    }

    [TestMethod]
    public void RedefinedParamAfter()
    {
        new TestRun()
            .AddSource("test.red",@"
x := 1;

func foobar([|RS0001||x|]: string) {
}
            ")
            .Execute();
    }

    [TestMethod]
    public void RedefinedParamBefore()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar([|RS0001||x|]: string) {
}

x := 1;
            ")
            .Execute();
    }


    [TestMethod]
    public void RedefinedParamInner()
    {
        new TestRun()
            .AddSource("test.red",@"

func foobar(x: string) {
    [|RS0001||x := 1|];
}
            ")
            .Execute();
    }
}