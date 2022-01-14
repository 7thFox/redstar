using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Redstar.Symbols.Mutable;

namespace Redstar.Symbols.Listener
{
    internal class ScopeBuilderListener : RedstarBaseListener
    {
        private SymbolTable _symbols;
        private Stack<long> _scopes = new Stack<long>();
        public ScopeBuilderListener(SymbolTable symbols) => _symbols = symbols;

        public override void EnterStart([NotNull] RedstarParser.StartContext context)
        {
            // var scope = SymbolFactory.CreateScope(null);// File top-level
            // _symbols.RegisterSymbol(scope);
            _scopes.Push(_symbols.SetScope(_symbols.ToplevelScope.ID));
        }

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            ValidateScope(_symbols.ToplevelScope);
        }

        public override void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
        {
            // only created after the func name
            _scopes.Push(_symbols.SetScope(_symbols.CreateScope()));
        }
        public override void ExitFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            if (context.funcParameters() != null)
            {
                // only closed after the body
                _scopes.Pop();
                _symbols.SetScope(_scopes.Peek());
            }
        }

        public override void EnterBody([NotNull] RedstarParser.BodyContext context)
        {
            _scopes.Push(_symbols.SetScope(_symbols.CreateScope()));
        }
        public override void ExitBody([NotNull] RedstarParser.BodyContext context)
        {
            _scopes.Pop();
            _symbols.SetScope(_scopes.Peek());
        }

        private void ValidateScope(Scope scopeRoot)
        {
            // This has to be run as a post-loop so any upper-scope variables
            // defined after the redecl are caught.

            verify(scopeRoot, new Dictionary<string, ISymbol>());

            void verify(Scope scope, IReadOnlyDictionary<string, ISymbol> declAbove)
            {
                var declThis = new Dictionary<string, ISymbol>(declAbove);
                foreach (var decl in scope.DeclaredIdents)
                {
                    if (!declThis.TryAdd(decl.Key, decl.Value))
                    {
                        Out.Error(2, decl.Value.Declaration, $"{decl.Value.Name} redefined outer scope variable");
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