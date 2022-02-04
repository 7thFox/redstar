using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using static Redstar.Symbols.SymbolFactory;

namespace Redstar.Symbols.Listener
{
    internal class SymbolCreationListener : RedstarBaseListener
    {
        private readonly SymbolTable _symbols;
        public SymbolCreationListener(SymbolTable symbolTable) => _symbols = symbolTable;

        public override void EnterFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            _symbols.Register(Func(context.ident()));
        }

        public override void EnterFuncParameter([NotNull] RedstarParser.FuncParameterContext context)
        {
            _symbols.Register(LocalVariable(context.ident()));
        }

        public override void EnterVarDefine([NotNull] RedstarParser.VarDefineContext context)
        {
            _symbols.Register(LocalVariable(context.ident()));
        }

        public override void EnterAttrDecl([NotNull] RedstarParser.AttrDeclContext context)
        {
            _symbols.Register(Attribute(context.ident()));
        }

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            // This has to be run as a post-loop so any upper-scope variables
            // defined after the redecl are caught.

            // CurrentScope = unit scope
            verify(_symbols.CurrentScope, new Dictionary<string, ISymbol>());

            static void verify(Scope scope, IReadOnlyDictionary<string, ISymbol> declAbove)
            {
                var declThis = new Dictionary<string, ISymbol>(declAbove);
                foreach (var decl in scope.DeclaredSymbols)
                {
                    if (!declThis.TryAdd(decl.Key, decl.Value))
                    {
                        Out.Error(ErrorCodes.Redefined, decl.Value.Declaration, $"{decl.Value.Name} redefined outer scope variable");
                    }
                }

                foreach (var child in scope.Children)
                {
                    verify(child, declThis);
                }
            }
        }
    }
}