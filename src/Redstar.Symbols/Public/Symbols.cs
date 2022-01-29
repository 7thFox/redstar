using System.Linq;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using static Redstar.Symbols.Internal.SymbolFactory;

namespace Redstar.Symbols
{
    public static class Symbols
    {
        public static SymbolTable ParseFiles(IEnumerable<string> paths)
        {
            var implicitScope = Scope(null, null);
            Out.Debug(DebugCategory.Scope, null, $"Implicit Scope ID {implicitScope.ID}");
            var symbolTable = new SymbolTable(implicitScope);
            symbolTable.CreateImplicitSymbols();

            foreach (var path in paths)
            {
                var unit = Unit(path, Scope(symbolTable.ImplicitScope, null));
                Out.Debug(DebugCategory.Scope, null, $"Unit Scope ID {unit.UnitScope.ID}");
                symbolTable.Register(unit);
                unit.Parse(symbolTable);
            }

            return symbolTable;
        }
    }
}