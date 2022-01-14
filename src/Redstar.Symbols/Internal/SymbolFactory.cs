using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;

namespace Redstar.Symbols.Internal
{
    internal static class SymbolFactory
    {
        private static long _symbolID = 0;
        public static ISymbol CreateLocalVariableSymbol(RedstarParser.IdentContext ident)
        {
            return new LocalVariableSymbol(++_symbolID, ident.GetText(), ident.Start);
        }

        public static ISymbol CreateAttributeSymbol(RedstarParser.IdentContext ident)
        {
            return new AttributeSymbol(++_symbolID, ident.GetText(), ident.Start);
        }

        public static ISymbol CreateFuncSymbol(RedstarParser.IdentContext ident)
        {
            return new FuncSymbol(++_symbolID, ident.GetText(), ident.Start);
        }

        public static Scope CreateScope([AllowNull] Scope parent)
        {
            return new Scope(++_symbolID, parent);
        }
    }
}