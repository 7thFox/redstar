using System.Linq;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using static Redstar.Symbols.Internal.SymbolFactory;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public static class Symbols
    {
        public static SymbolTable ParseFiles(IEnumerable<string> paths)
            => ParseStreams(paths.Select(p => CharStreams.fromPath(p)));

        public static SymbolTable ParseStreams(IEnumerable<ICharStream> streams)
        {
            var implicitScope = Scope(null, null);
            Out.Debug(DebugCategory.Scope, null, $"Implicit Scope ID {implicitScope.ID}");
            var symbolTable = new SymbolTable(implicitScope);
            symbolTable.CreateImplicitSymbols();

            foreach (var stream in streams)
            {
                var unit = Unit(stream.SourceName, Scope(symbolTable.ImplicitScope, null));
                Out.Debug(DebugCategory.Scope, null, $"Unit Scope ID {unit.UnitScope.ID}");
                symbolTable.Register(unit);
                unit.Parse(symbolTable, stream);
            }

            return symbolTable;
        }
    }
}