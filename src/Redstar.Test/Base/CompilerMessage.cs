using System.Text.RegularExpressions;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Redstar.Symbols;

namespace Redstar.Test;

public struct CompilerMessage
{
    public bool HasErrorCode { get; set; }
    public string? ErrorCodeString { get; set; }
    public ErrorCodes? ErrorCode { get; set; }

    public bool IsImplicit { get; set; }
    public string? FileName { get; set; }
    public int Line { get; set; }
    public int Column { get; set; }

    public string? Message { get; set; }

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

    public void AssertPerfectMatch(CompilerMessage? actual)
    {
        Assert.IsNotNull(actual);
        Assert.AreEqual(HasErrorCode, actual.Value.HasErrorCode);
        Assert.AreEqual(ErrorCodeString, actual.Value.ErrorCodeString);
        Assert.AreEqual(ErrorCode, actual.Value.ErrorCode);
        Assert.AreEqual(IsImplicit, actual.Value.IsImplicit);
        Assert.AreEqual(FileName, actual.Value.FileName);
        Assert.AreEqual(Line, actual.Value.Line);
        Assert.AreEqual(Column, actual.Value.Column);
        Assert.AreEqual(Message, actual.Value.Message);
    }

    public void AssertMatchIgnoreMessage(CompilerMessage actual)
    {
        Assert.AreEqual(HasErrorCode, actual.HasErrorCode);
        Assert.AreEqual(ErrorCodeString, actual.ErrorCodeString);
        Assert.AreEqual(ErrorCode, actual.ErrorCode);
        Assert.AreEqual(IsImplicit, actual.IsImplicit);
        Assert.AreEqual(FileName, actual.FileName);
        Assert.AreEqual(Line, actual.Line);
        Assert.AreEqual(Column, actual.Column);
    }

    public bool LocationMatch(CompilerMessage msg)
    {
        if (IsImplicit)
        {
            return msg.IsImplicit;
        }

        return msg.FileName == FileName &&
            msg.Line == Line &&
            msg.Column == Column;
    }
}