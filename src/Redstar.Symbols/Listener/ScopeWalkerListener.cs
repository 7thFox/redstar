using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Redstar.Symbols.Mutable;

namespace Redstar.Symbols.Listener
{
    internal class ScopeWalkerListener : RedstarBaseListener
    {
        private long _lastNewScopeID;
        private SymbolTable _symbols;
        private Stack<long> _scopes = new Stack<long>();
        public ScopeWalkerListener(SymbolTable symbols) => _symbols = symbols;

        public override void EnterStart([NotNull] RedstarParser.StartContext context)
        {
            _lastNewScopeID = _symbols.ToplevelScope.ID;
            _scopes.Push(_symbols.SetScope(_lastNewScopeID));
        }

        public override void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
        {
            // only created after the func name
            _scopes.Push(_symbols.SetScope(++_lastNewScopeID));
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
            _scopes.Push(_symbols.SetScope(++_lastNewScopeID));
        }
        public override void ExitBody([NotNull] RedstarParser.BodyContext context)
        {
            _scopes.Pop();
            _symbols.SetScope(_scopes.Peek());
        }
    }
}