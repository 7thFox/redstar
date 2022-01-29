using System.Text.RegularExpressions;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Redstar.Symbols;

namespace Redstar.Test;

public struct CompilerMessage
{
    public bool HasErrorCode { get; set; }
    public string? ErrorCodeString { get; set; }
    public ErrorCodes? ErrorCode { get; set; }

    public Location Location { get; set; }

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

        var isImplicit = !string.IsNullOrEmpty(match.Groups[3].Value);
        msg.Location = isImplicit ? Location.Implicit : new Location()
        {
            FileName = match.Groups[4].Value,
            Line = int.Parse(match.Groups[5].Value),
            Column = int.Parse(match.Groups[6].Value),
        };

        msg.Message = match.Groups[7].Value;

        return msg;
    }

    public void AssertPerfectMatch(CompilerMessage? actual)
    {
        Assert.IsNotNull(actual);
        AssertMatchIgnoreMessage(actual.Value);
        Assert.AreEqual(Message, actual.Value.Message, "Message");
    }

    public void AssertMatchIgnoreMessage(CompilerMessage actual)
    {
        Assert.AreEqual(HasErrorCode, actual.HasErrorCode, "HasErrorCode");
        Assert.AreEqual(ErrorCodeString, actual.ErrorCodeString, "ErrorCodeString");
        Assert.AreEqual(ErrorCode, actual.ErrorCode, "ErrorCode");
        Location.AssertLocation(actual.Location);
    }

    public bool LocationMatch(CompilerMessage msg)
    {
        if (Location.IsImplicit)
        {
            return msg.Location.IsImplicit;
        }

        return msg.Location.FileName == Location.FileName &&
            msg.Location.Line == Location.Line &&
            msg.Location.Column == Location.Column;
    }
}