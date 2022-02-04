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
            Register(SymbolFactory.Type("void", Location.Implicit));
            Register(SymbolFactory.Type("byte", Location.Implicit));
            Register(SymbolFactory.Type("short", Location.Implicit));
            Register(SymbolFactory.Type("int", Location.Implicit));
            Register(SymbolFactory.Type("long", Location.Implicit));
            Register(SymbolFactory.Type("bool", Location.Implicit));
            Register(SymbolFactory.Type("char", Location.Implicit));
            Register(SymbolFactory.Type("string", Location.Implicit));
            Register(SymbolFactory.Type("decimal", Location.Implicit));
            Register(SymbolFactory.Type("float", Location.Implicit));
            Register(SymbolFactory.Type("double", Location.Implicit));
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
                Out.Debug(DebugCategory.Symbol, symbol.Declaration, $"In Scope ID {CurrentScope.ID} Symbol {symbol.Type} ID {symbol.ID} ({symbol.Name})");
            }
            return symbol;
        }

        internal void SetScope(Scope scope)
        {
            CurrentScope = scope;
        }
    }
}