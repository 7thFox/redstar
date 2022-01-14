using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Redstar.Parse;

namespace Redstar.Symbols.Mutable
{
    internal class Scope
    {
        public long ID { get; }
        public Scope(long id, [AllowNull] Scope parent)
        {
            Parent = parent;
            parent?._children.Add(this);
            ID = id;
        }

        private readonly List<Scope> _children = new List<Scope>();
        public IReadOnlyList<Scope> Children => _children;

        [AllowNull]
        public Scope Parent { get; }

        public void AddSymbol(ISymbol symbol)
        {
            if (!_declaredIdents.TryAdd(symbol.Name, symbol))
            {
                Out.Error(1, symbol.Declaration, $"{symbol.Name} redefined");
            }
            else
            {
                Out.Debug(DebugCategory.ScopeAndSymbol, symbol.Declaration, $"New ident: {symbol.Name,-15} (SymbolID {symbol.ID}, ScopeID {ID})");
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

        [return: MaybeNull]
        public ISymbol FindInThisScope(string identName)
        {
            if (_declaredIdents.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            return null;
        }

        public override string ToString()
        {
            return ID.ToString() + "\n" + string.Join("\n", Children.Select(x => x.ToString(1)));
        }

        public string ToString(int level)
        {
            return new string(' ', level * 2) +
                ID.ToString() +
                "\n" +
                string.Join("\n", Children.Select(x => x.ToString(level + 1)));
        }
    }
}