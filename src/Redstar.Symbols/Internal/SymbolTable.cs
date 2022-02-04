using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Collections.Immutable;
using static Redstar.Symbols.Internal.Assertions;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        internal Scope CurrentScope { get; private set; }
        internal ImmutableArray<CompilationUnit>.Builder _units;

        internal SymbolTable(Scope implicitScope)
        {
            CurrentScope = ImplicitScope = implicitScope;
            _units = ImmutableArray.CreateBuilder<CompilationUnit>();
        }

        internal void CreateImplicitSymbols()
        {
            var typeref = SymbolFactory.TypeRefType();
            Register(typeref);
            Register(SymbolFactory.Type("void", Location.Implicit, typeref));
            Register(SymbolFactory.Type("byte", Location.Implicit, typeref));
            Register(SymbolFactory.Type("short", Location.Implicit, typeref));
            Register(SymbolFactory.Type("int", Location.Implicit, typeref));
            Register(SymbolFactory.Type("long", Location.Implicit, typeref));
            Register(SymbolFactory.Type("bool", Location.Implicit, typeref));
            Register(SymbolFactory.Type("char", Location.Implicit, typeref));
            Register(SymbolFactory.Type("string", Location.Implicit, typeref));
            Register(SymbolFactory.Type("decimal", Location.Implicit, typeref));
            Register(SymbolFactory.Type("float", Location.Implicit, typeref));
            Register(SymbolFactory.Type("double", Location.Implicit, typeref));
        }

        private readonly Dictionary<Location, ISymbol> _symbolByToken = new ();

        internal void AddReference(ISymbol symbolReferenced, IToken locationReferenced)
            => AddReference(symbolReferenced, locationReferenced.Location());

        internal void AddReference(ISymbol symbolReferenced, Location locationReferenced)
        {
            Assert(!locationReferenced.IsImplicit);
            if (!_symbolByToken.TryAdd(locationReferenced, symbolReferenced))
            {
                Out.Panic(locationReferenced, "Symbol reference already added for token");
            }
        }

        internal T Register<T>(T symbol) where T : ISymbolInternal
        {
            if (!symbol.Declaration.IsImplicit)
            {
                AddReference(symbol, symbol.Declaration);
            }

            if (CurrentScope.AddSymbol(symbol))
            {
                Out.Debug(DebugCategory.Symbol, symbol.Declaration, $"In Scope ID {CurrentScope.ID} Symbol {symbol.Kind} ID {symbol.ID} ({symbol.Name})");
            }
            return symbol;
        }

        internal void SetScope(Scope scope)
        {
            CurrentScope = scope;
        }
    }
}