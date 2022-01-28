using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal static class SymbolFactory
    {
        private static long _symbolID = 0;
        // public static ISymbol LocalVariableSymbol(RedstarParser.IdentContext ident)
        // {
        //     return new LocalVariableSymbol(++_symbolID, ident.GetText(), ident.Start);
        // }

        // public static ISymbol AttributeSymbol(RedstarParser.IdentContext ident)
        // {
        //     return new AttributeSymbol(++_symbolID, ident.GetText(), ident.Start);
        // }

        // public static ISymbol FuncSymbol(RedstarParser.IdentContext ident)
        // {
        //     return new FuncSymbol(++_symbolID, ident.GetText(), ident.Start);
        // }

        public static Scope Scope([AllowNull] Scope parent)
        {
            return new Scope(++_symbolID, parent);
        }

        public static TypeSymbol Type(string name, [AllowNull] IToken location)
        {
            return new TypeSymbol(++_symbolID, name, location);
        }

        public static CompilationUnit Unit(string path, Scope scope)
        {
            return new CompilationUnit(++_symbolID, path, scope);
        }
    }
}