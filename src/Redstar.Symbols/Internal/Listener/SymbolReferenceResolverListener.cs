using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Redstar.Symbols.Internal;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal.Listener
{
    internal class SymbolReferenceResolverListener : RedstarBaseListener
    {
        private readonly SymbolTable _symbols;
        private readonly List<(RedstarParser.IdentContext ident, Scope parentScope)> _deferredResolve = new();
        public SymbolReferenceResolverListener(SymbolTable symbolTable) => _symbols = symbolTable;

        public override void ExitStart([NotNull] RedstarParser.StartContext context)
        {
            ResolveDeferredReferences();
        }

        public override void EnterTypename([NotNull] RedstarParser.TypenameContext context)
        {
            ISymbol? symbol;
            var ident = context.ident();
            if (ident != null)
            {
                symbol = _symbols.FastFind(ident);
            }
            else // primitive type
            {
                symbol = _symbols.FastFind(context.GetText()).MustFind();
            }

            if (symbol != null)
            {
                _symbols.AddReference(symbol, context.Start);
                Out.Debug(DebugCategory.Reference, context.Start, $"Type Reference to Symbol ID {symbol.ID} ({symbol.Name} @ {symbol.Declaration})");
            }
        }

        public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        {
            if (context.Parent?.RuleIndex != RedstarParser.RULE_typename &&
                context.Parent?.RuleIndex != RedstarParser.RULE_varDefine &&
                context.Parent?.RuleIndex != RedstarParser.RULE_funcParameter &&
                context.Parent?.RuleIndex != RedstarParser.RULE_funcDecl)
            {
                var symbol = _symbols.FastFind(context);
                if (symbol != null)
                {
                    _symbols.AddReference(symbol, context.Start);
                    Out.Debug(DebugCategory.Reference, context.Start, $"Reference to Symbol ID {symbol.ID} ({symbol.Name} @ {symbol.Declaration})");
                }
                else
                {
                    Out.Debug(DebugCategory.Reference, context.Start, "Deferred reference resolution");
                    // We already searched the current scope, so we can start at the parent.
                    // This is never the implicit scope. Parent will always be non-null.
                    _deferredResolve.Add((context, _symbols.CurrentScope.Parent!));
                }
            }
        }

        // PERFORMANCE: We can scope references like we do with symbol declarations
        // and have a single de-reference for all references within a scope.
        private void ResolveDeferredReferences()
        {
            foreach ((var ident, var scope) in _deferredResolve)
            {
                var symbol = scope.Find(ident.GetText());
                if (symbol != null)
                {
                    Out.Debug(DebugCategory.Reference, ident.Start, $"Reference to Symbol ID {symbol.ID} ({symbol.Name} @ {symbol.Declaration})");
                }
                else
                {
                    Out.Error(ErrorCodes.UnresolvedReference, ident.Start, $"Unresolved identifer '{ident.GetText()}'");
                }
            }
        }
    }
}