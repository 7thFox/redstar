using System;
using Redstar.Parser;
using System.Collections.Generic;

namespace Redstar
{
    public class Scope
    {
        public static long _lastScopeID = 0;
        public long ScopeID { get; }
        public Scope(Scope parent)
        {
            Parent = parent;
            parent?._children.Add(this);
            ScopeID = ++_lastScopeID;
        }

        private readonly List<Scope> _children = new List<Scope>();
        public IReadOnlyList<Scope> Children => _children;

        public Scope Parent { get; }

        public void NewIdent(SymbolTable symbols, RedstarParser.IdentContext ident)
        {
            if (_declaredIdents.ContainsKey(ident.GetText()))
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"{ident.Start.Location()} Ident redefined");
                Console.ForegroundColor = ConsoleColor.White;
            }
            else
            {
                var symbol = symbols.CreateIdentiferSymbol(ident);
                Console.WriteLine($"{symbol.Declaration.Location()} New ident: {symbol.Name} (SymbolID {symbol.ID}, ScopeID {ScopeID})");
                _declaredIdents.Add(symbol.Name, symbol);
            }
        }

        private Dictionary<string, ISymbol> _declaredIdents { get; } = new Dictionary<string, ISymbol>();
        public IReadOnlyDictionary<string, ISymbol> DeclaredIdents => _declaredIdents;

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