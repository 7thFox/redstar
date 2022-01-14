using System;
using Redstar;
using Redstar.Symbols;

Out.DebugCategories = new[]
{
    // DebugCategory.ScopeAndSymbol,
    DebugCategory.Type,
};

if (args.Length > 0)
{
    Symbols.ParseFiles(args);
}
else
{
    Symbols.ParseFiles("/home/josh/src/redstar/examples/scratch/main.red");
}