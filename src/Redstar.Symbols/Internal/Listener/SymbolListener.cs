using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Redstar.Symbols.Internal;
using Antlr4.Runtime.Tree;
using static Redstar.Symbols.Internal.SymbolFactory;

namespace Redstar.Symbols.Internal.Listener
{
    internal class SymbolListener : RedstarBaseListener
    {
        private readonly SymbolTable _symbols;
        public SymbolListener(SymbolTable symbolTable) => _symbols = symbolTable;

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            // ResolveReferences();
        }

        // public override void EnterFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        // {
        //     _symbols.Register(FuncDecl)
        //     _symbols.CreateFuncSymbol(context.ident());
        // }

        public override void EnterFuncParameter([NotNull] RedstarParser.FuncParameterContext context)
        {
            _symbols.Register(LocalVariableSymbol(context.ident()));
        }

        // public override void EnterVarDefine([NotNull] RedstarParser.VarDefineContext context)
        // {
        //     _symbols.CreateLocalVariableSymbol(context.ident());
        // }

        // public override void EnterAttrDecl([NotNull] RedstarParser.AttrDeclContext context)
        // {
        //     _symbols.CreateAttributeSymbol(context.ident());
        // }

        // public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        // {
        //     // _toResolve.Add((context, _scope.Peek()));
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