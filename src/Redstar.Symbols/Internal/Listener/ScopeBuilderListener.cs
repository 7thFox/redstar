using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using static Redstar.Symbols.Internal.SymbolFactory;

namespace Redstar.Symbols.Internal.Listener
{
    internal class ScopeBuilderListener : RedstarBaseListener
    {
        private readonly CompilationUnit _unit;
        // private SymbolTable _symbols;
        private readonly Stack<Scope> _scopes = new ();
        public ScopeBuilderListener(CompilationUnit unit)
        {
            _unit = unit;
            _scopes.Push(_unit.UnitScope);
        }

        private void EnterScope([NotNull] IToken location)
        {
            var scope = Scope(_scopes.Peek(), location);
            Out.Debug(DebugCategory.Scope, location, $"Enter Scope ID {scope.ID}");
            _unit.AddScope(scope);
            _scopes.Push(scope);
        }

        private void ExitScope(IToken location)
        {
            var scope = _scopes.Pop();
            Out.Debug(DebugCategory.Scope, location, $"Exit Scope ID {scope.ID}");
        }

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            if (_scopes.Count > 1)
            {
                throw new Exception("Scope stack uneven");
            }
            // ValidateScope(_symbols.ToplevelScope);
        }

        public override void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
        {
            // only created after the func name
            EnterScope(context.Start);
        }
        public override void ExitFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            if (context.funcParameters() != null)
            {
                // only closed after the body
                ExitScope(context.Stop);
            }
        }

        public override void EnterBody([NotNull] RedstarParser.BodyContext context)
        {
            EnterScope(context.Start);
        }
        public override void ExitBody([NotNull] RedstarParser.BodyContext context)
        {
            ExitScope(context.Stop);
        }

        // private void ValidateScope(Scope scopeRoot)
        // {
        //     // This has to be run as a post-loop so any upper-scope variables
        //     // defined after the redecl are caught.

        //     verify(scopeRoot, new Dictionary<string, ISymbol>());

        //     void verify(Scope scope, IReadOnlyDictionary<string, ISymbol> declAbove)
        //     {
        //         var declThis = new Dictionary<string, ISymbol>(declAbove);
        //         foreach (var decl in scope.DeclaredIdents)
        //         {
        //             if (!declThis.TryAdd(decl.Key, decl.Value))
        //             {
        //                 Out.Error(2, decl.Value.Declaration, $"{decl.Value.Name} redefined outer scope variable");
        //             }
        //         }

        //         foreach (var child in scope.Children)
        //         {
        //             verify(child, declThis);
        //         }
        //     }
        // }
    }
}