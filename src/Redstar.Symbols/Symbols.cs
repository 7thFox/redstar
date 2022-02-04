using System.Linq;
using Redstar.Parse;
using static Redstar.Symbols.SymbolFactory;
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
            var implicitScope = Scope(null, Location.Implicit);
            Out.Debug(DebugCategory.Scope, Location.Implicit, $"Implicit Scope ID {implicitScope.ID}");
            var symbolTable = new SymbolTable(implicitScope);
            symbolTable.CreateImplicitSymbols();

            foreach (var stream in streams)
            {
                var unit = Unit(stream.SourceName, Scope(symbolTable.ImplicitScope, Location.Implicit));
                Out.Debug(DebugCategory.Scope, Location.Implicit, $"Unit Scope ID {unit.UnitScope.ID}");
                symbolTable.Register(unit);
                unit.Parse(symbolTable, stream);
            }

            // Validate and finalize
            symbolTable.CopyInternalData();
            foreach (var unit in symbolTable.Units)
            {
                foreach (var scope in unit.Scopes)
                {
                    foreach (var symbol in scope.DeclaredSymbols.Values)
                    {
                        if (symbol is ISymbolInternal internalSymbol)
                        {
                            internalSymbol.CopyInternalData();
                        }

                        if (symbol is ITypedSymbol typedSymbol)
                        {
                            if (typedSymbol.Type == null)
                            {
                                Out.Panic(symbol.Declaration, $"Symbol '{symbol.Name}' has no type annotation");
                            }
                        }
                    }
                }
            }

            return symbolTable;
        }
    }
}