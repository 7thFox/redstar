using System;
using System.Linq;
using Redstar.Parser;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Internal.Listener
{
    public class TypeCheckerListener : RedstarBaseListener
    {
        // private List<(string, Scope)> _deferredFinds = new List<(string, Scope)>();
        private SymbolTable _symbols = new SymbolTable();
        public TypeCheckerListener(SymbolTable symbolTable) => _symbols = symbolTable;
        public Stack<ISymbol> _statementTypes = new Stack<ISymbol>();

        public override void EnterStatement([NotNull] RedstarParser.StatementContext context)
        {
            _statementTypes.Clear();
        }

        public override void EnterLiteralInt([NotNull] RedstarParser.LiteralIntContext context)
        {
            Out.Debug(DebugCategory.Type, context.Start, "int");
            _statementTypes.Push(_symbols.ImplicitScope.FindInThisScope("int").MustFind());
        }

        public override void EnterLiteralString([NotNull] RedstarParser.LiteralStringContext context)
        {
            Out.Debug(DebugCategory.Type, context.Start, "string");
            _statementTypes.Push(_symbols.ImplicitScope.FindInThisScope("string").MustFind());
        }

        public override void ExitBinaryExp([NotNull] RedstarParser.BinaryExpContext context)
        {
            var right = _statementTypes.Pop();
            var left = _statementTypes.Peek();// Pop then re-push
            if (!left.SymbolEqual(right))
            {
                Out.Error(3, context.Start, $"Cannot perform operation with {left.Name} and {right.Name}");
            }
            Out.Debug(DebugCategory.Type, context.Start, left.Name);
        }

        public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        {
            base.EnterIdent(context);
        }

        // public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        // {
        //     var symbolName = context.GetText();
        //     var fastSymbol = _symbols.FastFind(symbolName);
        //     if (fastSymbol != null)
        //     {

        //     }
        //     else
        //     {

        //     }
        // }

        // private void ResolveReferences()
        // {
        //     foreach ((var ident, var scope) in _toResolve)
        //     {
        //         var symbol = scope.Find(ident.GetText());
        //         if (symbol != null)
        //         {
        //             Console.WriteLine($"{ident.Start.Location()} Reference SymbolID {symbol.ID}");
        //         }
        //         else
        //         {
        //             Console.ForegroundColor = ConsoleColor.Red;
        //             Console.WriteLine($"{ident.Start.Location()} Cannot determine symbol reference");
        //             Console.ForegroundColor = ConsoleColor.White;
        //         }
        //     }
        // }
    }
}