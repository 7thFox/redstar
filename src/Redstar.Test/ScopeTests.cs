using System;
using System.Text.RegularExpressions;
using Redstar.Symbols;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Text;
using System.Collections.Generic;
using Antlr4.Runtime;
using System.Linq;

namespace Redstar.Test;

[TestClass]
public class ScopeTests
{
    [TestMethod]
    public void TestMethod1()
    {
        new TestRun()
        {

        }.Exectue();
    }
}

[TestClass]
public class CompilerMessageParseTests
{
    [TestMethod]
    public void DebugMessage()
    {
        var msgMaybe = CompilerMessage.Parse("        examples/testscope/main.red:7:16        Deferred reference resolution");
        Assert.IsNotNull(msgMaybe);
        var msg = msgMaybe.Value;
        Assert.AreEqual(false, msg.HasErrorCode);
        Assert.AreEqual(null, msg.ErrorCodeString);
        Assert.AreEqual(null, msg.ErrorCode);
        Assert.AreEqual(false, msg.IsImplicit);
        Assert.AreEqual("examples/testscope/main.red", msg.FileName);
        Assert.AreEqual(7, msg.Line);
        Assert.AreEqual(16, msg.Column);
        Assert.AreEqual("Deferred reference resolution", msg.Message);
    }

    [TestMethod]
    public void ErrorMessage()
    {
        var msgMaybe = CompilerMessage.Parse("RS0001  examples/testscope/main.red:3:1         x redefined");
        Assert.IsNotNull(msgMaybe);
        var msg = msgMaybe.Value;
        Assert.AreEqual(true, msg.HasErrorCode);
        Assert.AreEqual("RS0001", msg.ErrorCodeString);
        Assert.AreEqual((ErrorCodes)1, msg.ErrorCode);
        Assert.AreEqual(false, msg.IsImplicit);
        Assert.AreEqual("examples/testscope/main.red", msg.FileName);
        Assert.AreEqual(3, msg.Line);
        Assert.AreEqual(1, msg.Column);
        Assert.AreEqual("x redefined", msg.Message);
    }
    [TestMethod]
    public void ImplicitDebugMessage()
    {
        var msgMaybe = CompilerMessage.Parse("        implicit                                In Scope ID 1 Symbol CompilationUnit ID 14 (examples/testscope/main.red)");
        Assert.IsNotNull(msgMaybe);
        var msg = msgMaybe.Value;
        Assert.AreEqual(false, msg.HasErrorCode);
        Assert.AreEqual(null, msg.ErrorCodeString);
        Assert.AreEqual(null, msg.ErrorCode);
        Assert.AreEqual(true, msg.IsImplicit);
        Assert.AreEqual(null, msg.FileName);
        Assert.AreEqual(default, msg.Line);
        Assert.AreEqual(default, msg.Column);
        Assert.AreEqual("In Scope ID 1 Symbol CompilationUnit ID 14 (examples/testscope/main.red)", msg.Message);
    }
}

public class TestRun
{

    public void Exectue()
    {
        var stderr = Console.Error;

        using (var stream = new MemoryStream())
        {
            using (var stdtest = new StreamWriter(stream, Encoding.UTF8, 2048, leaveOpen: true))
            {
                Console.SetError(stdtest);
                Symbols.Symbols.ParseStreams(new[] { CharStreams.fromString(@"
x := 0
x := 1
            ") });
                stdtest.Flush();
                Console.SetError(stderr);
            }

            stream.Seek(0, SeekOrigin.Begin);
            var messages = new List<CompilerMessage>();
            CompilerMessage? msg;
            using (var reader = new StreamReader(stream))
            {
                while (!reader.EndOfStream)
                {
                    if ((msg = CompilerMessage.Parse(reader.ReadLine())) != null)
                    {
                        messages.Add(msg.Value);
                    }
                }
            }

            Assert.IsTrue(messages.Any());

        }
    }
}

public struct CompilerMessage
{
    public bool HasErrorCode { get; private set; }
    public string? ErrorCodeString { get; private set; }
    public ErrorCodes? ErrorCode { get; private set; }

    public bool IsImplicit { get; private set; }
    public string? FileName { get; private set; }
    public int Line { get; private set; }
    public int Column { get; private set; }

    public string Message { get; private set; }

    private static readonly Regex _parse = new Regex(@"
                    (RS(\d{4}))?\s+                   # 1 2
                    (?:(implicit)|(.+?):(\d+):(\d+))\s* # (3|4:5:6)
                    (.*)                              # 7",
                    RegexOptions.Compiled | RegexOptions.IgnorePatternWhitespace);
    public static CompilerMessage? Parse(string? line)
    {
        if (line == null)
        {
            return null;
        }

        var match = _parse.Match(line);
        if (!match.Success)
        {
            return null;
        }

        var msg = new CompilerMessage();

        msg.ErrorCodeString = match.Groups[1].Value;
        msg.HasErrorCode = !string.IsNullOrEmpty(msg.ErrorCodeString);
        if (msg.HasErrorCode)
        {
            msg.ErrorCode = (ErrorCodes)int.Parse(match.Groups[2].Value);
        }
        else
        {
            msg.ErrorCodeString = null;
        }

        msg.IsImplicit = !string.IsNullOrEmpty(match.Groups[3].Value);
        if (!msg.IsImplicit)
        {
            msg.FileName = match.Groups[4].Value;
            msg.Line = int.Parse(match.Groups[5].Value);
            msg.Column = int.Parse(match.Groups[6].Value);
        }

        msg.Message = match.Groups[7].Value;

        return msg;
    }
}