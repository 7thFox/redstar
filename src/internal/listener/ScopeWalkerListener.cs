using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parser;

namespace Redstar.Internal.Listener
{
    // public class ScopeWalkerListener : RedstarBaseListener
    // {
    //     private SymbolTable _symbols;
    //     private Stack<Scope> _scopeStack = new Stack<Scope>();
    //     public ScopeWalkerListener(SymbolTable symbols) => _symbols = symbols;

    //     public override void EnterStart([NotNull] RedstarParser.StartContext context)
    //     {
    //         _scopeStack.Push(_symbols.ToplevelScope);
    //     }

    //     // Enter
    //     public override void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
    //     {
    //         _symbols.OpenScope();
    //     }

    //     public override void EnterBody([NotNull] RedstarParser.BodyContext context)
    //     {
    //         _symbols.OpenScope();
    //     }

    //     // Exit

    //     public override void ExitFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
    //     {
    //         _symbols.CloseScope();
    //     }

    //     public override void ExitBody([NotNull] RedstarParser.BodyContext context)
    //     {
    //         _symbols.CloseScope();
    //     }

    //     private void ValidateScope(Scope scopeRoot)
    //     {
    //         // This has to be run as a post-loop so any upper-scope variables
    //         // defined after the redecl are caught.

    //         verify(scopeRoot, new Dictionary<string, ISymbol>());

    //         void verify(Scope scope, IReadOnlyDictionary<string, ISymbol> declAbove)
    //         {
    //             var declThis = new Dictionary<string, ISymbol>(declAbove);
    //             foreach (var decl in scope.DeclaredIdents)
    //             {
    //                 if (!declThis.TryAdd(decl.Key, decl.Value))
    //                 {
    //                     Out.Error(2, decl.Value.Declaration, $"{decl.Value.Name} redefined outer scope variable");
    //                 }
    //             }

    //             foreach (var child in scope.Children)
    //             {
    //                 verify(child, declThis);
    //             }
    //         }
    //     }
    // }
}