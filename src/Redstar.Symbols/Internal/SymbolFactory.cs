using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using static Redstar.Symbols.Internal.Assertions;

namespace Redstar.Symbols.Internal
{
    internal static class SymbolFactory
    {
        private static long _symbolID = 0;

        public static LocalVariableSymbol LocalVariable(RedstarParser.IdentContext ident)
        {
            return new LocalVariableSymbol(++_symbolID, ident.GetText(), ident.Start.Location());
        }

        public static AttributeSymbol Attribute(RedstarParser.IdentContext ident)
        {
            return new AttributeSymbol(++_symbolID, ident.GetText(), ident.Start.Location());
        }

        public static FuncSymbol Func(RedstarParser.IdentContext ident)
        {
            return new FuncSymbol(++_symbolID, ident.GetText(), ident.Start.Location());
        }

        public static Scope Scope(Scope? parent, Location declaration)
        {
            return new Scope(++_symbolID, parent, declaration);
        }

        public static TypeSymbol Type(string name, Location location, TypeSymbol typeSymbol)
        {
            Assert(typeSymbol.Name == "typeref");
            Assert(typeSymbol.Declaration.IsImplicit);
            return new TypeSymbol(++_symbolID, name, location, typeSymbol);
        }

        public static TypeSymbol TypeRefType()
        {
            return new TypeSymbol(++_symbolID);
        }

        public static CompilationUnit Unit(string path, Scope scope)
        {
            return new CompilationUnit(++_symbolID, path, scope);
        }
    }
}