using System;
using Redstar.Symbols;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;
using System.Text;
using System.Collections.Generic;
using Antlr4.Runtime;
using System.Linq;
using System.Security.Cryptography;

namespace Redstar.Test;

public class TestRun
{
    public bool OnlyVerifyErrorsAndWarnings { get; set; } = true;
    public IReadOnlyList<(string Name, string SourceCode)> Sources => _sources;
    public IReadOnlyList<CompilerMessage> ExpectedDiagnostics => _expected;
    public IEnumerable<ICharStream> SourceStreams => _sources.Select(src =>
    {
        var srcStream = CharStreams.fromString(src.SourceCode);
        (srcStream as BaseInputCharStream)!.name = src.Name;
        return srcStream;
    });

    private readonly List<(string Name, string SourceCode)> _sources = new();
    private readonly List<CompilerMessage> _expected = new();


    // [|RS####||code with diagnostic|]
    // escape start: [[| => [|
    // end escape: ||] => |]
    public TestRun AddSource(string name, string source)
    {
        _sources.Add((name, ParseSource(name, source)));
        return this;
    }

    public SymbolTable Execute()
    {
        SymbolTable st;
        var stderr = Console.Error;
        var messages = new List<CompilerMessage>();

        using (var stream = new MemoryStream())
        {
            using (var stdtest = new StreamWriter(stream, Encoding.UTF8, 2048, leaveOpen: true))
            {

                Console.SetError(stdtest);
                st = Symbols.Symbols.ParseStreams(SourceStreams);
                stdtest.Flush();
                Console.SetError(stderr);
            }

            stream.Seek(0, SeekOrigin.Begin);
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
        }

        foreach (var exp in _expected)
        {
            // TODO JOSH: will fail erroneously if 2 errors at 1 location
            var found = messages.Where(exp.LocationMatch).ToArray();

            if (found.Length == 0)
            {
                Assert.Fail($"No errors at {exp.Location}");
            }
            else if (found.Length == 1)
            {
                exp.AssertMatchIgnoreMessage(found.Single());
                messages.Remove(found.Single());
            }
            else
            {
                var matchCode = found.Where(m => m.ErrorCode == exp.ErrorCode).ToArray();
                if (matchCode.Length == 0)
                {
                    Assert.Fail($"Error {exp.ErrorCodeString} not found, but others were: {string.Join(", ", found.Select(m => m.ErrorCodeString))}");
                }
                else if (matchCode.Length > 1)
                {
                    Assert.Fail($"More than one {exp.ErrorCodeString} found at {exp.Location}");
                }
                else
                {
                    exp.AssertMatchIgnoreMessage(matchCode.Single());
                    messages.Remove(matchCode.Single());
                }
            }
        }

        if (messages.Count > 0)
        {
            Assert.Fail(string.Join("\n", messages.Select(m => $"Unexpected {m.ErrorCodeString} at {m.Location}")));
        }

        return st;
    }

    private string ParseSource(string name, string source)
    {
        var sb = new StringBuilder(source.Length);

        var chars = source.GetEnumerator();
        int line = 1;
        int col = 1;

        parse();

        return sb.ToString();

        void parse()
        {
            while (chars.MoveNext())
            {
                if (chars.Current == '[')
                {
                    if (chars.MoveNext())
                    {
                        // potential escapes
                        if (chars.Current == '[')
                        {
                            parseStartEscape();
                        }
                        else if (chars.Current == '|')
                        {
                            _expected.Add(parseCode());
                            readToEndSequence();
                        }
                        else
                        {
                            writeAndInc();
                        }
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    writeAndInc();
                }
            }
        }

        void readToEndSequence()
        {
            while (chars.MoveNext())
            {
                if (chars.Current == '|')
                {
                    if (chars.MoveNext())
                    {
                        // potential escapes
                        if (chars.Current == '|')
                        {
                            parseEndEscape();
                            continue;
                        }
                        else if (chars.Current == ']')
                        {
                            return;
                        }
                    }
                }

                writeAndInc();
            }

            Assert.Fail("Diagnostic region not closed");
        }

        void writeAndInc()
        {
            sb.Append(chars.Current);
            if (chars.Current == '\n')
            {
                line++;
                col = 1;
            }
            else
            {
                col++;
            }
        }

        void parseEndEscape()
        {
            // ||

            if (!chars.MoveNext())// ||<EOF>
            {
                col += 2;
                sb.Append("||");
            }
            else if (chars.Current == '\n')// ||\n
            {
                line++;
                col = 1;
                sb.Append("||\n");
            }
            else if (chars.Current == ']')// [[| (escape)
            {
                col += 2;
                sb.Append("|]");
                sb.Append(chars.Current);
            }
            else// [[something else
            {
                col += 3;
                sb.Append("[[");
                sb.Append(chars.Current);
            }
        }

        void parseStartEscape()
        {
            // [[

            if (!chars.MoveNext())// [[<EOF>
            {
                col += 2;
                sb.Append("[[");
            }
            else if (chars.Current == '\n')// [[\n
            {
                line++;
                col = 1;
                sb.Append("[[\n");
            }
            else if (chars.Current == '|')// [[| => [|
            {
                col += 2;
                sb.Append("[|");
            }
            else// [[something else
            {
                col += 3;
                sb.Append("[[");
                sb.Append(chars.Current);
            }
        }

        CompilerMessage parseCode()
        {
            // [|
            var msg = new CompilerMessage()
            {
                Location = new Location()
                {
                    FileName = name,
                    Line = line,
                    Column = col,
                },
                HasErrorCode = true,
            };

            Assert.IsTrue(chars.MoveNext());
            Assert.AreEqual("R", chars.Current.ToString(), ignoreCase: true);
            Assert.IsTrue(chars.MoveNext());
            Assert.AreEqual("S", chars.Current.ToString(), ignoreCase: true);
            var code = new char[4];
            for (int i = 0; i < 4; i++)
            {
                Assert.IsTrue(chars.MoveNext());
                code[i] = chars.Current;
            }

            msg.ErrorCode = (ErrorCodes)int.Parse(new string(code));
            msg.ErrorCodeString = $"RS{(int)msg.ErrorCode:0000}";

            Assert.IsTrue(chars.MoveNext());
            Assert.AreEqual('|', chars.Current);
            Assert.IsTrue(chars.MoveNext());
            Assert.AreEqual('|', chars.Current);

            return msg;
        }
    }
}
