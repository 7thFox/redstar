using Redstar.Symbols;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Redstar.Test.BaseTests;

[TestClass]
public class CompilerMessageParseTests
{
    [TestMethod]
    public void DebugMessage()
    {
        var msg = CompilerMessage.Parse("        examples/testscope/main.red:7:16        Deferred reference resolution");
        new CompilerMessage()
        {
            HasErrorCode = false,
            ErrorCodeString = null,
            ErrorCode = null,
            IsImplicit = false,
            FileName = "examples/testscope/main.red",
            Line = 7,
            Column = 16,
            Message = "Deferred reference resolution"
        }.AssertPerfectMatch(msg);
    }

    [TestMethod]
    public void ErrorMessage()
    {
        var msg = CompilerMessage.Parse("RS0001  examples/testscope/main.red:3:1         x redefined");
        new CompilerMessage()
        {
            HasErrorCode = true,
            ErrorCodeString = "RS0001",
            ErrorCode = (ErrorCodes)1,
            IsImplicit = false,
            FileName = "examples/testscope/main.red",
            Line = 3,
            Column = 1,
            Message = "x redefined"
        }.AssertPerfectMatch(msg);
    }

    [TestMethod]
    public void ImplicitDebugMessage()
    {
        var msg = CompilerMessage.Parse("        implicit                                In Scope ID 1 Symbol CompilationUnit ID 14 (examples/testscope/main.red)");

        new CompilerMessage()
        {
            HasErrorCode = false,
            ErrorCodeString = null,
            ErrorCode = null,
            IsImplicit = true,
            FileName = null,
            Line = default,
            Column = default,
            Message = "In Scope ID 1 Symbol CompilationUnit ID 14 (examples/testscope/main.red)"
        }.AssertPerfectMatch(msg);
    }
}
