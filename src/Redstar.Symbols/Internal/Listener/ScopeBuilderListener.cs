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
        private readonly SymbolTable _symbols;
        private readonly Stack<Scope> _scopes = new ();
        public ScopeBuilderListener(SymbolTable symbols, CompilationUnit unit)
        {
            _symbols = symbols;
            _unit = unit;
            _scopes.Push(_unit.UnitScope);
        }

        private void EnterScope([NotNull] Location location)
        {
            var scope = Scope(_scopes.Peek(), location);
            _unit.AddScope(scope);
            _scopes.Push(scope);
            Out.Debug(DebugCategory.Scope, location, $"Enter Scope ID {scope.ID}");
            _symbols.SetScope(scope);
        }

        private void ExitScope(Location location)
        {
            var scope = _scopes.Pop();
            Out.Debug(DebugCategory.Scope, location, $"Exit Scope ID {scope.ID}");
            _symbols.SetScope(scope);
        }

        public override void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context)
        {
            // only add if we declare parameters
            EnterScope(context.Start.Location());
        }

        public override void ExitFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            if (context.funcParameters() != null)// only added for params
            {
                ExitScope(context.Stop.Location());
            }
        }

        public override void EnterBody([NotNull] RedstarParser.BodyContext context)
        {
            EnterScope(context.Start.Location());
        }

        public override void ExitBody([NotNull] RedstarParser.BodyContext context)
        {
            ExitScope(context.Stop.Location());
        }

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            if (_scopes.Count > 1)
            {
                throw new Exception("Scope stack uneven");
            }

            _symbols.SetScope(_scopes.Pop());
        }
    }
}