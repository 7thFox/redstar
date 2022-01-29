using Microsoft.VisualStudio.TestTools.UnitTesting;
using Redstar.Symbols;
using System.Linq;

namespace Redstar.Test.BaseTests;

[TestClass]
public class TestRunSourceParseTests
{
    [TestMethod]
    public void SourceNameSet()
    {
        var run = new TestRun();
        var name = "foobar/baz/foo.txt";

        run.AddSource(name, "");

        Assert.AreEqual(name, run.SourceStreams.Single().SourceName);
    }

    [TestMethod]
    public void NoDiagnostics()
    {
        var run = new TestRun();
        var src = @"

here's some text
   n stuff witout any escapes

        ";

        run.AddSource("name.txt", src);

        Assert.AreEqual(src, run.Sources.Single().SourceCode);
        Assert.AreEqual(0, run.ExpectedDiagnostics.Count);
    }

    [TestMethod]
    public void Diagnostic_DiagnosticMatches_Start()
    {
        var run = new TestRun();
        var src       = @"[|RS0231||This should be|] an easy one.";

        run.AddSource("test.txt", src);

        Assert.AreEqual(1, run.ExpectedDiagnostics.Count);
        new CompilerMessage()
        {
            HasErrorCode = true,
            ErrorCodeString = "RS0231",
            ErrorCode = (ErrorCodes)231,
            Location = new Location()
            {
                FileName = "test.txt",
                Line = 1,
                Column = 1,
            },
            Message = null,
        }.AssertPerfectMatch(run.ExpectedDiagnostics.Single());
    }

    [TestMethod]
    public void Diagnostic_DiagnosticMatches_FirstLine()
    {
        var run = new TestRun();
        var src       = @"abc [|RS0231||now we're|] offset.";

        run.AddSource("test.txt", src);

        Assert.AreEqual(1, run.ExpectedDiagnostics.Count);
        new CompilerMessage()
        {
            HasErrorCode = true,
            ErrorCodeString = "RS0231",
            ErrorCode = (ErrorCodes)231,
            Location = new Location()
            {
                FileName = "test.txt",
                Line = 1,
                Column = 5,
            },
            Message = null,
        }.AssertPerfectMatch(run.ExpectedDiagnostics.Single());
    }

    [TestMethod]
    public void Diagnostic_DiagnosticMatches_MultiLine()
    {
        var run = new TestRun();
        var src       = @"abc
            llslsli [[]]][][]
                assdfli [|RS0231||now we're|]
    jjfjs offset.";

        run.AddSource("test.txt", src);

        Assert.AreEqual(1, run.ExpectedDiagnostics.Count);
        new CompilerMessage()
        {
            HasErrorCode = true,
            ErrorCodeString = "RS0231",
            ErrorCode = (ErrorCodes)231,
            Location = new Location()
            {
                FileName = "test.txt",
                Line = 3,
                Column = 25,
            },
            Message = null,
        }.AssertPerfectMatch(run.ExpectedDiagnostics.Single());
    }

    [TestMethod]
    public void Diagnostic_ParsedSourceMatches()
    {
        var run = new TestRun();
        var src       = @"now [|RS0231||we have some error diagnostics|] to worry about.";
        var srcParsed = @"now we have some error diagnostics to worry about.";

        run.AddSource("test.txt", src);

        Assert.AreEqual(1, run.ExpectedDiagnostics.Count);
        Assert.AreEqual(srcParsed, run.Sources.Single().SourceCode);
    }
}
