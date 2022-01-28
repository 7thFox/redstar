using System;
using Redstar;
using Redstar.Symbols;

var debugCategories = new List<DebugCategory>();
var filePaths = new List<string>();

IEnumerator<string> argEnumerator = args.AsEnumerable().GetEnumerator();
while (argEnumerator.MoveNext())
{
    if (filePaths.Any())
    {
        filePaths.Add(argEnumerator.Current);
    }
    else if (argEnumerator.Current.StartsWith("--"))// Long args
    {
        switch (argEnumerator.Current.Substring(2).ToLower())
        {
            case "printscope":
                debugCategories.Add(DebugCategory.Scope);
                break;
            case "printtypes":
                debugCategories.Add(DebugCategory.Type);
                break;
            case "printsymbols":
                debugCategories.Add(DebugCategory.Symbol);
                break;
            default:
                throw new Exception($"Invalid argument {argEnumerator.Current}");
        }
    }
    else if (argEnumerator.Current.StartsWith("-"))// short args
    {
        switch (argEnumerator.Current.Substring(1).ToLower())
        {
            default:
                throw new Exception($"Invalid argument {argEnumerator.Current}");
        }
    }
    else // first filename
    {
        filePaths.Add(argEnumerator.Current);
    }
}

Out.DebugCategories = debugCategories;
if (filePaths.Count == 0)
{
    filePaths.Add("/home/josh/src/redstar/examples/testscope/main.red");
}
Symbols.ParseFiles(filePaths);