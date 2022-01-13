using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parser;

namespace Redstar.Internal
{
    public class Scope
    {
        public long ScopeID { get; }
        public Scope(long id, [AllowNull] Scope parent)
        {
            Parent = parent;
            parent?._children.Add(this);
            ScopeID = id;
        }

        private readonly List<Scope> _children = new List<Scope>();
        public IReadOnlyList<Scope> Children => _children;

        [AllowNull]
        public Scope Parent { get; }

        public void AddSymbol(ISymbol symbol)
        {
            if (_declaredIdents.TryAdd(symbol.Name, symbol))
            {
                Out.Debug(symbol.Declaration, $"New ident: {symbol.Name,-15} (SymbolID {symbol.ID}, ScopeID {ScopeID})");
            }
            else
            {
                Out.Error(1, symbol.Declaration, $"{symbol.Name} redefined");
            }
        }

        private Dictionary<string, ISymbol> _declaredIdents { get; } = new Dictionary<string, ISymbol>();
        public IReadOnlyDictionary<string, ISymbol> DeclaredIdents => _declaredIdents;

        [return: MaybeNull]
        public ISymbol Find(string identName)
        {
            if (_declaredIdents.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            if (Parent != null)
            {
                return Parent.Find(identName);
            }

            return null;
        }
    }
}