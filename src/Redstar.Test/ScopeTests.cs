using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace Redstar.Test;

[TestClass]
public class ScopeTests
{
    [TestMethod]
    public void SimpleRedefine()
    {
        new TestRun()
            .AddSource("test.red",@"
x := 1
[|RS0001||x := 2|]
            ")
            .Exectue();
    }
}