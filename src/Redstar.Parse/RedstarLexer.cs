//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     ANTLR Version: 4.9.3
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from src/Redstar.Parse/Redstar.g4 by ANTLR 4.9.3

// Unreachable code detected
#pragma warning disable 0162
// The variable '...' is assigned but its value is never used
#pragma warning disable 0219
// Missing XML comment for publicly visible type or member '...'
#pragma warning disable 1591
// Ambiguous reference in cref attribute
#pragma warning disable 419

namespace Redstar.Parse {
using System;
using System.IO;
using System.Text;
using Antlr4.Runtime;
using Antlr4.Runtime.Atn;
using Antlr4.Runtime.Misc;
using DFA = Antlr4.Runtime.Dfa.DFA;

[System.CodeDom.Compiler.GeneratedCode("ANTLR", "4.9.3")]
[System.CLSCompliant(false)]
public partial class RedstarLexer : Lexer {
	protected static DFA[] decisionToDFA;
	protected static PredictionContextCache sharedContextCache = new PredictionContextCache();
	public const int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, STRINGLIT=28, INT=29, OP=30, IDENT=31, WS=32, 
		COMMENT=33;
	public static string[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static string[] modeNames = {
		"DEFAULT_MODE"
	};

	public static readonly string[] ruleNames = {
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
		"T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
		"T__17", "T__18", "T__19", "T__20", "T__21", "T__22", "T__23", "T__24", 
		"T__25", "T__26", "STRINGLIT", "INT", "OP", "IDENT", "WS", "COMMENT"
	};


	public RedstarLexer(ICharStream input)
	: this(input, Console.Out, Console.Error) { }

	public RedstarLexer(ICharStream input, TextWriter output, TextWriter errorOutput)
	: base(input, output, errorOutput)
	{
		Interpreter = new LexerATNSimulator(this, _ATN, decisionToDFA, sharedContextCache);
	}

	private static readonly string[] _LiteralNames = {
		null, "';'", "'attr'", "'anno'", "'_'", "'=>'", "'bind'", "'any'", "'all'", 
		"'none'", "','", "'func'", "'('", "')'", "':'", "'{'", "'}'", "'if'", 
		"'else'", "'['", "'-'", "']'", "'='", "'return'", "'e'", "'.'", "'int'", 
		"'string'"
	};
	private static readonly string[] _SymbolicNames = {
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, null, null, "STRINGLIT", "INT", "OP", "IDENT", "WS", "COMMENT"
	};
	public static readonly IVocabulary DefaultVocabulary = new Vocabulary(_LiteralNames, _SymbolicNames);

	[NotNull]
	public override IVocabulary Vocabulary
	{
		get
		{
			return DefaultVocabulary;
		}
	}

	public override string GrammarFileName { get { return "Redstar.g4"; } }

	public override string[] RuleNames { get { return ruleNames; } }

	public override string[] ChannelNames { get { return channelNames; } }

	public override string[] ModeNames { get { return modeNames; } }

	public override string SerializedAtn { get { return new string(_serializedATN); } }

	static RedstarLexer() {
		decisionToDFA = new DFA[_ATN.NumberOfDecisions];
		for (int i = 0; i < _ATN.NumberOfDecisions; i++) {
			decisionToDFA[i] = new DFA(_ATN.GetDecisionState(i), i);
		}
	}
	private static char[] _serializedATN = {
		'\x3', '\x608B', '\xA72A', '\x8133', '\xB9ED', '\x417C', '\x3BE7', '\x7786', 
		'\x5964', '\x2', '#', '\xFC', '\b', '\x1', '\x4', '\x2', '\t', '\x2', 
		'\x4', '\x3', '\t', '\x3', '\x4', '\x4', '\t', '\x4', '\x4', '\x5', '\t', 
		'\x5', '\x4', '\x6', '\t', '\x6', '\x4', '\a', '\t', '\a', '\x4', '\b', 
		'\t', '\b', '\x4', '\t', '\t', '\t', '\x4', '\n', '\t', '\n', '\x4', '\v', 
		'\t', '\v', '\x4', '\f', '\t', '\f', '\x4', '\r', '\t', '\r', '\x4', '\xE', 
		'\t', '\xE', '\x4', '\xF', '\t', '\xF', '\x4', '\x10', '\t', '\x10', '\x4', 
		'\x11', '\t', '\x11', '\x4', '\x12', '\t', '\x12', '\x4', '\x13', '\t', 
		'\x13', '\x4', '\x14', '\t', '\x14', '\x4', '\x15', '\t', '\x15', '\x4', 
		'\x16', '\t', '\x16', '\x4', '\x17', '\t', '\x17', '\x4', '\x18', '\t', 
		'\x18', '\x4', '\x19', '\t', '\x19', '\x4', '\x1A', '\t', '\x1A', '\x4', 
		'\x1B', '\t', '\x1B', '\x4', '\x1C', '\t', '\x1C', '\x4', '\x1D', '\t', 
		'\x1D', '\x4', '\x1E', '\t', '\x1E', '\x4', '\x1F', '\t', '\x1F', '\x4', 
		' ', '\t', ' ', '\x4', '!', '\t', '!', '\x4', '\"', '\t', '\"', '\x3', 
		'\x2', '\x3', '\x2', '\x3', '\x3', '\x3', '\x3', '\x3', '\x3', '\x3', 
		'\x3', '\x3', '\x3', '\x3', '\x4', '\x3', '\x4', '\x3', '\x4', '\x3', 
		'\x4', '\x3', '\x4', '\x3', '\x5', '\x3', '\x5', '\x3', '\x6', '\x3', 
		'\x6', '\x3', '\x6', '\x3', '\a', '\x3', '\a', '\x3', '\a', '\x3', '\a', 
		'\x3', '\a', '\x3', '\b', '\x3', '\b', '\x3', '\b', '\x3', '\b', '\x3', 
		'\t', '\x3', '\t', '\x3', '\t', '\x3', '\t', '\x3', '\n', '\x3', '\n', 
		'\x3', '\n', '\x3', '\n', '\x3', '\n', '\x3', '\v', '\x3', '\v', '\x3', 
		'\f', '\x3', '\f', '\x3', '\f', '\x3', '\f', '\x3', '\f', '\x3', '\r', 
		'\x3', '\r', '\x3', '\xE', '\x3', '\xE', '\x3', '\xF', '\x3', '\xF', '\x3', 
		'\x10', '\x3', '\x10', '\x3', '\x11', '\x3', '\x11', '\x3', '\x12', '\x3', 
		'\x12', '\x3', '\x12', '\x3', '\x13', '\x3', '\x13', '\x3', '\x13', '\x3', 
		'\x13', '\x3', '\x13', '\x3', '\x14', '\x3', '\x14', '\x3', '\x15', '\x3', 
		'\x15', '\x3', '\x16', '\x3', '\x16', '\x3', '\x17', '\x3', '\x17', '\x3', 
		'\x18', '\x3', '\x18', '\x3', '\x18', '\x3', '\x18', '\x3', '\x18', '\x3', 
		'\x18', '\x3', '\x18', '\x3', '\x19', '\x3', '\x19', '\x3', '\x1A', '\x3', 
		'\x1A', '\x3', '\x1B', '\x3', '\x1B', '\x3', '\x1B', '\x3', '\x1B', '\x3', 
		'\x1C', '\x3', '\x1C', '\x3', '\x1C', '\x3', '\x1C', '\x3', '\x1C', '\x3', 
		'\x1C', '\x3', '\x1C', '\x3', '\x1D', '\x3', '\x1D', '\x3', '\x1D', '\x3', 
		'\x1D', '\a', '\x1D', '\xA4', '\n', '\x1D', '\f', '\x1D', '\xE', '\x1D', 
		'\xA7', '\v', '\x1D', '\x3', '\x1D', '\x3', '\x1D', '\x3', '\x1E', '\x6', 
		'\x1E', '\xAC', '\n', '\x1E', '\r', '\x1E', '\xE', '\x1E', '\xAD', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', 
		'\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x3', '\x1F', '\x5', 
		'\x1F', '\xD8', '\n', '\x1F', '\x3', ' ', '\x3', ' ', '\a', ' ', '\xDC', 
		'\n', ' ', '\f', ' ', '\xE', ' ', '\xDF', '\v', ' ', '\x3', ' ', '\x3', 
		' ', '\x6', ' ', '\xE3', '\n', ' ', '\r', ' ', '\xE', ' ', '\xE4', '\x5', 
		' ', '\xE7', '\n', ' ', '\x3', '!', '\x6', '!', '\xEA', '\n', '!', '\r', 
		'!', '\xE', '!', '\xEB', '\x3', '!', '\x3', '!', '\x3', '\"', '\x3', '\"', 
		'\x3', '\"', '\x3', '\"', '\a', '\"', '\xF4', '\n', '\"', '\f', '\"', 
		'\xE', '\"', '\xF7', '\v', '\"', '\x3', '\"', '\x3', '\"', '\x3', '\"', 
		'\x3', '\"', '\x4', '\xA5', '\xF5', '\x2', '#', '\x3', '\x3', '\x5', '\x4', 
		'\a', '\x5', '\t', '\x6', '\v', '\a', '\r', '\b', '\xF', '\t', '\x11', 
		'\n', '\x13', '\v', '\x15', '\f', '\x17', '\r', '\x19', '\xE', '\x1B', 
		'\xF', '\x1D', '\x10', '\x1F', '\x11', '!', '\x12', '#', '\x13', '%', 
		'\x14', '\'', '\x15', ')', '\x16', '+', '\x17', '-', '\x18', '/', '\x19', 
		'\x31', '\x1A', '\x33', '\x1B', '\x35', '\x1C', '\x37', '\x1D', '\x39', 
		'\x1E', ';', '\x1F', '=', ' ', '?', '!', '\x41', '\"', '\x43', '#', '\x3', 
		'\x2', '\t', '\x5', '\x2', '\f', '\f', '$', '$', '^', '^', '\x6', '\x2', 
		'$', '$', '^', '^', 'p', 'p', 't', 't', '\x3', '\x2', '\x32', ';', '\b', 
		'\x2', '\'', '(', ',', '-', '/', '/', '\x31', '\x31', '>', '@', '~', '~', 
		'\x4', '\x2', '\x43', '\\', '\x63', '|', '\x6', '\x2', '\x32', ';', '\x43', 
		'\\', '\x61', '\x61', '\x63', '|', '\x5', '\x2', '\v', '\f', '\xF', '\xF', 
		'\"', '\"', '\x2', '\x114', '\x2', '\x3', '\x3', '\x2', '\x2', '\x2', 
		'\x2', '\x5', '\x3', '\x2', '\x2', '\x2', '\x2', '\a', '\x3', '\x2', '\x2', 
		'\x2', '\x2', '\t', '\x3', '\x2', '\x2', '\x2', '\x2', '\v', '\x3', '\x2', 
		'\x2', '\x2', '\x2', '\r', '\x3', '\x2', '\x2', '\x2', '\x2', '\xF', '\x3', 
		'\x2', '\x2', '\x2', '\x2', '\x11', '\x3', '\x2', '\x2', '\x2', '\x2', 
		'\x13', '\x3', '\x2', '\x2', '\x2', '\x2', '\x15', '\x3', '\x2', '\x2', 
		'\x2', '\x2', '\x17', '\x3', '\x2', '\x2', '\x2', '\x2', '\x19', '\x3', 
		'\x2', '\x2', '\x2', '\x2', '\x1B', '\x3', '\x2', '\x2', '\x2', '\x2', 
		'\x1D', '\x3', '\x2', '\x2', '\x2', '\x2', '\x1F', '\x3', '\x2', '\x2', 
		'\x2', '\x2', '!', '\x3', '\x2', '\x2', '\x2', '\x2', '#', '\x3', '\x2', 
		'\x2', '\x2', '\x2', '%', '\x3', '\x2', '\x2', '\x2', '\x2', '\'', '\x3', 
		'\x2', '\x2', '\x2', '\x2', ')', '\x3', '\x2', '\x2', '\x2', '\x2', '+', 
		'\x3', '\x2', '\x2', '\x2', '\x2', '-', '\x3', '\x2', '\x2', '\x2', '\x2', 
		'/', '\x3', '\x2', '\x2', '\x2', '\x2', '\x31', '\x3', '\x2', '\x2', '\x2', 
		'\x2', '\x33', '\x3', '\x2', '\x2', '\x2', '\x2', '\x35', '\x3', '\x2', 
		'\x2', '\x2', '\x2', '\x37', '\x3', '\x2', '\x2', '\x2', '\x2', '\x39', 
		'\x3', '\x2', '\x2', '\x2', '\x2', ';', '\x3', '\x2', '\x2', '\x2', '\x2', 
		'=', '\x3', '\x2', '\x2', '\x2', '\x2', '?', '\x3', '\x2', '\x2', '\x2', 
		'\x2', '\x41', '\x3', '\x2', '\x2', '\x2', '\x2', '\x43', '\x3', '\x2', 
		'\x2', '\x2', '\x3', '\x45', '\x3', '\x2', '\x2', '\x2', '\x5', 'G', '\x3', 
		'\x2', '\x2', '\x2', '\a', 'L', '\x3', '\x2', '\x2', '\x2', '\t', 'Q', 
		'\x3', '\x2', '\x2', '\x2', '\v', 'S', '\x3', '\x2', '\x2', '\x2', '\r', 
		'V', '\x3', '\x2', '\x2', '\x2', '\xF', '[', '\x3', '\x2', '\x2', '\x2', 
		'\x11', '_', '\x3', '\x2', '\x2', '\x2', '\x13', '\x63', '\x3', '\x2', 
		'\x2', '\x2', '\x15', 'h', '\x3', '\x2', '\x2', '\x2', '\x17', 'j', '\x3', 
		'\x2', '\x2', '\x2', '\x19', 'o', '\x3', '\x2', '\x2', '\x2', '\x1B', 
		'q', '\x3', '\x2', '\x2', '\x2', '\x1D', 's', '\x3', '\x2', '\x2', '\x2', 
		'\x1F', 'u', '\x3', '\x2', '\x2', '\x2', '!', 'w', '\x3', '\x2', '\x2', 
		'\x2', '#', 'y', '\x3', '\x2', '\x2', '\x2', '%', '|', '\x3', '\x2', '\x2', 
		'\x2', '\'', '\x81', '\x3', '\x2', '\x2', '\x2', ')', '\x83', '\x3', '\x2', 
		'\x2', '\x2', '+', '\x85', '\x3', '\x2', '\x2', '\x2', '-', '\x87', '\x3', 
		'\x2', '\x2', '\x2', '/', '\x89', '\x3', '\x2', '\x2', '\x2', '\x31', 
		'\x90', '\x3', '\x2', '\x2', '\x2', '\x33', '\x92', '\x3', '\x2', '\x2', 
		'\x2', '\x35', '\x94', '\x3', '\x2', '\x2', '\x2', '\x37', '\x98', '\x3', 
		'\x2', '\x2', '\x2', '\x39', '\x9F', '\x3', '\x2', '\x2', '\x2', ';', 
		'\xAB', '\x3', '\x2', '\x2', '\x2', '=', '\xD7', '\x3', '\x2', '\x2', 
		'\x2', '?', '\xE6', '\x3', '\x2', '\x2', '\x2', '\x41', '\xE9', '\x3', 
		'\x2', '\x2', '\x2', '\x43', '\xEF', '\x3', '\x2', '\x2', '\x2', '\x45', 
		'\x46', '\a', '=', '\x2', '\x2', '\x46', '\x4', '\x3', '\x2', '\x2', '\x2', 
		'G', 'H', '\a', '\x63', '\x2', '\x2', 'H', 'I', '\a', 'v', '\x2', '\x2', 
		'I', 'J', '\a', 'v', '\x2', '\x2', 'J', 'K', '\a', 't', '\x2', '\x2', 
		'K', '\x6', '\x3', '\x2', '\x2', '\x2', 'L', 'M', '\a', '\x63', '\x2', 
		'\x2', 'M', 'N', '\a', 'p', '\x2', '\x2', 'N', 'O', '\a', 'p', '\x2', 
		'\x2', 'O', 'P', '\a', 'q', '\x2', '\x2', 'P', '\b', '\x3', '\x2', '\x2', 
		'\x2', 'Q', 'R', '\a', '\x61', '\x2', '\x2', 'R', '\n', '\x3', '\x2', 
		'\x2', '\x2', 'S', 'T', '\a', '?', '\x2', '\x2', 'T', 'U', '\a', '@', 
		'\x2', '\x2', 'U', '\f', '\x3', '\x2', '\x2', '\x2', 'V', 'W', '\a', '\x64', 
		'\x2', '\x2', 'W', 'X', '\a', 'k', '\x2', '\x2', 'X', 'Y', '\a', 'p', 
		'\x2', '\x2', 'Y', 'Z', '\a', '\x66', '\x2', '\x2', 'Z', '\xE', '\x3', 
		'\x2', '\x2', '\x2', '[', '\\', '\a', '\x63', '\x2', '\x2', '\\', ']', 
		'\a', 'p', '\x2', '\x2', ']', '^', '\a', '{', '\x2', '\x2', '^', '\x10', 
		'\x3', '\x2', '\x2', '\x2', '_', '`', '\a', '\x63', '\x2', '\x2', '`', 
		'\x61', '\a', 'n', '\x2', '\x2', '\x61', '\x62', '\a', 'n', '\x2', '\x2', 
		'\x62', '\x12', '\x3', '\x2', '\x2', '\x2', '\x63', '\x64', '\a', 'p', 
		'\x2', '\x2', '\x64', '\x65', '\a', 'q', '\x2', '\x2', '\x65', '\x66', 
		'\a', 'p', '\x2', '\x2', '\x66', 'g', '\a', 'g', '\x2', '\x2', 'g', '\x14', 
		'\x3', '\x2', '\x2', '\x2', 'h', 'i', '\a', '.', '\x2', '\x2', 'i', '\x16', 
		'\x3', '\x2', '\x2', '\x2', 'j', 'k', '\a', 'h', '\x2', '\x2', 'k', 'l', 
		'\a', 'w', '\x2', '\x2', 'l', 'm', '\a', 'p', '\x2', '\x2', 'm', 'n', 
		'\a', '\x65', '\x2', '\x2', 'n', '\x18', '\x3', '\x2', '\x2', '\x2', 'o', 
		'p', '\a', '*', '\x2', '\x2', 'p', '\x1A', '\x3', '\x2', '\x2', '\x2', 
		'q', 'r', '\a', '+', '\x2', '\x2', 'r', '\x1C', '\x3', '\x2', '\x2', '\x2', 
		's', 't', '\a', '<', '\x2', '\x2', 't', '\x1E', '\x3', '\x2', '\x2', '\x2', 
		'u', 'v', '\a', '}', '\x2', '\x2', 'v', ' ', '\x3', '\x2', '\x2', '\x2', 
		'w', 'x', '\a', '\x7F', '\x2', '\x2', 'x', '\"', '\x3', '\x2', '\x2', 
		'\x2', 'y', 'z', '\a', 'k', '\x2', '\x2', 'z', '{', '\a', 'h', '\x2', 
		'\x2', '{', '$', '\x3', '\x2', '\x2', '\x2', '|', '}', '\a', 'g', '\x2', 
		'\x2', '}', '~', '\a', 'n', '\x2', '\x2', '~', '\x7F', '\a', 'u', '\x2', 
		'\x2', '\x7F', '\x80', '\a', 'g', '\x2', '\x2', '\x80', '&', '\x3', '\x2', 
		'\x2', '\x2', '\x81', '\x82', '\a', ']', '\x2', '\x2', '\x82', '(', '\x3', 
		'\x2', '\x2', '\x2', '\x83', '\x84', '\a', '/', '\x2', '\x2', '\x84', 
		'*', '\x3', '\x2', '\x2', '\x2', '\x85', '\x86', '\a', '_', '\x2', '\x2', 
		'\x86', ',', '\x3', '\x2', '\x2', '\x2', '\x87', '\x88', '\a', '?', '\x2', 
		'\x2', '\x88', '.', '\x3', '\x2', '\x2', '\x2', '\x89', '\x8A', '\a', 
		't', '\x2', '\x2', '\x8A', '\x8B', '\a', 'g', '\x2', '\x2', '\x8B', '\x8C', 
		'\a', 'v', '\x2', '\x2', '\x8C', '\x8D', '\a', 'w', '\x2', '\x2', '\x8D', 
		'\x8E', '\a', 't', '\x2', '\x2', '\x8E', '\x8F', '\a', 'p', '\x2', '\x2', 
		'\x8F', '\x30', '\x3', '\x2', '\x2', '\x2', '\x90', '\x91', '\a', 'g', 
		'\x2', '\x2', '\x91', '\x32', '\x3', '\x2', '\x2', '\x2', '\x92', '\x93', 
		'\a', '\x30', '\x2', '\x2', '\x93', '\x34', '\x3', '\x2', '\x2', '\x2', 
		'\x94', '\x95', '\a', 'k', '\x2', '\x2', '\x95', '\x96', '\a', 'p', '\x2', 
		'\x2', '\x96', '\x97', '\a', 'v', '\x2', '\x2', '\x97', '\x36', '\x3', 
		'\x2', '\x2', '\x2', '\x98', '\x99', '\a', 'u', '\x2', '\x2', '\x99', 
		'\x9A', '\a', 'v', '\x2', '\x2', '\x9A', '\x9B', '\a', 't', '\x2', '\x2', 
		'\x9B', '\x9C', '\a', 'k', '\x2', '\x2', '\x9C', '\x9D', '\a', 'p', '\x2', 
		'\x2', '\x9D', '\x9E', '\a', 'i', '\x2', '\x2', '\x9E', '\x38', '\x3', 
		'\x2', '\x2', '\x2', '\x9F', '\xA5', '\a', '$', '\x2', '\x2', '\xA0', 
		'\xA4', '\n', '\x2', '\x2', '\x2', '\xA1', '\xA2', '\a', '^', '\x2', '\x2', 
		'\xA2', '\xA4', '\t', '\x3', '\x2', '\x2', '\xA3', '\xA0', '\x3', '\x2', 
		'\x2', '\x2', '\xA3', '\xA1', '\x3', '\x2', '\x2', '\x2', '\xA4', '\xA7', 
		'\x3', '\x2', '\x2', '\x2', '\xA5', '\xA6', '\x3', '\x2', '\x2', '\x2', 
		'\xA5', '\xA3', '\x3', '\x2', '\x2', '\x2', '\xA6', '\xA8', '\x3', '\x2', 
		'\x2', '\x2', '\xA7', '\xA5', '\x3', '\x2', '\x2', '\x2', '\xA8', '\xA9', 
		'\a', '$', '\x2', '\x2', '\xA9', ':', '\x3', '\x2', '\x2', '\x2', '\xAA', 
		'\xAC', '\t', '\x4', '\x2', '\x2', '\xAB', '\xAA', '\x3', '\x2', '\x2', 
		'\x2', '\xAC', '\xAD', '\x3', '\x2', '\x2', '\x2', '\xAD', '\xAB', '\x3', 
		'\x2', '\x2', '\x2', '\xAD', '\xAE', '\x3', '\x2', '\x2', '\x2', '\xAE', 
		'<', '\x3', '\x2', '\x2', '\x2', '\xAF', '\xB0', '\a', '>', '\x2', '\x2', 
		'\xB0', '\xB1', '\a', '>', '\x2', '\x2', '\xB1', '\xD8', '\a', '?', '\x2', 
		'\x2', '\xB2', '\xB3', '\a', '@', '\x2', '\x2', '\xB3', '\xB4', '\a', 
		'@', '\x2', '\x2', '\xB4', '\xD8', '\a', '?', '\x2', '\x2', '\xB5', '\xB6', 
		'\a', '(', '\x2', '\x2', '\xB6', '\xB7', '\a', '(', '\x2', '\x2', '\xB7', 
		'\xD8', '\a', '?', '\x2', '\x2', '\xB8', '\xB9', '\a', '~', '\x2', '\x2', 
		'\xB9', '\xBA', '\a', '~', '\x2', '\x2', '\xBA', '\xD8', '\a', '?', '\x2', 
		'\x2', '\xBB', '\xBC', '\a', '\x41', '\x2', '\x2', '\xBC', '\xBD', '\a', 
		'\x41', '\x2', '\x2', '\xBD', '\xD8', '\a', '?', '\x2', '\x2', '\xBE', 
		'\xBF', '\a', '\x41', '\x2', '\x2', '\xBF', '\xD8', '\a', '\x41', '\x2', 
		'\x2', '\xC0', '\xC1', '\a', '?', '\x2', '\x2', '\xC1', '\xD8', '\a', 
		'?', '\x2', '\x2', '\xC2', '\xC3', '\a', '#', '\x2', '\x2', '\xC3', '\xD8', 
		'\a', '?', '\x2', '\x2', '\xC4', '\xC5', '\a', '>', '\x2', '\x2', '\xC5', 
		'\xD8', '\a', '?', '\x2', '\x2', '\xC6', '\xC7', '\a', '@', '\x2', '\x2', 
		'\xC7', '\xD8', '\a', '?', '\x2', '\x2', '\xC8', '\xC9', '\a', '@', '\x2', 
		'\x2', '\xC9', '\xD8', '\a', '@', '\x2', '\x2', '\xCA', '\xCB', '\a', 
		'>', '\x2', '\x2', '\xCB', '\xD8', '\a', '>', '\x2', '\x2', '\xCC', '\xCD', 
		'\a', '(', '\x2', '\x2', '\xCD', '\xD8', '\a', '(', '\x2', '\x2', '\xCE', 
		'\xCF', '\a', '~', '\x2', '\x2', '\xCF', '\xD8', '\a', '~', '\x2', '\x2', 
		'\xD0', '\xD1', '\a', '(', '\x2', '\x2', '\xD1', '\xD8', '\a', '?', '\x2', 
		'\x2', '\xD2', '\xD3', '\a', '~', '\x2', '\x2', '\xD3', '\xD8', '\a', 
		'?', '\x2', '\x2', '\xD4', '\xD5', '\a', '\'', '\x2', '\x2', '\xD5', '\xD8', 
		'\a', '?', '\x2', '\x2', '\xD6', '\xD8', '\t', '\x5', '\x2', '\x2', '\xD7', 
		'\xAF', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xB2', '\x3', '\x2', '\x2', 
		'\x2', '\xD7', '\xB5', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xB8', '\x3', 
		'\x2', '\x2', '\x2', '\xD7', '\xBB', '\x3', '\x2', '\x2', '\x2', '\xD7', 
		'\xBE', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xC0', '\x3', '\x2', '\x2', 
		'\x2', '\xD7', '\xC2', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xC4', '\x3', 
		'\x2', '\x2', '\x2', '\xD7', '\xC6', '\x3', '\x2', '\x2', '\x2', '\xD7', 
		'\xC8', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xCA', '\x3', '\x2', '\x2', 
		'\x2', '\xD7', '\xCC', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xCE', '\x3', 
		'\x2', '\x2', '\x2', '\xD7', '\xD0', '\x3', '\x2', '\x2', '\x2', '\xD7', 
		'\xD2', '\x3', '\x2', '\x2', '\x2', '\xD7', '\xD4', '\x3', '\x2', '\x2', 
		'\x2', '\xD7', '\xD6', '\x3', '\x2', '\x2', '\x2', '\xD8', '>', '\x3', 
		'\x2', '\x2', '\x2', '\xD9', '\xDD', '\t', '\x6', '\x2', '\x2', '\xDA', 
		'\xDC', '\t', '\a', '\x2', '\x2', '\xDB', '\xDA', '\x3', '\x2', '\x2', 
		'\x2', '\xDC', '\xDF', '\x3', '\x2', '\x2', '\x2', '\xDD', '\xDB', '\x3', 
		'\x2', '\x2', '\x2', '\xDD', '\xDE', '\x3', '\x2', '\x2', '\x2', '\xDE', 
		'\xE7', '\x3', '\x2', '\x2', '\x2', '\xDF', '\xDD', '\x3', '\x2', '\x2', 
		'\x2', '\xE0', '\xE2', '\a', '\x61', '\x2', '\x2', '\xE1', '\xE3', '\t', 
		'\a', '\x2', '\x2', '\xE2', '\xE1', '\x3', '\x2', '\x2', '\x2', '\xE3', 
		'\xE4', '\x3', '\x2', '\x2', '\x2', '\xE4', '\xE2', '\x3', '\x2', '\x2', 
		'\x2', '\xE4', '\xE5', '\x3', '\x2', '\x2', '\x2', '\xE5', '\xE7', '\x3', 
		'\x2', '\x2', '\x2', '\xE6', '\xD9', '\x3', '\x2', '\x2', '\x2', '\xE6', 
		'\xE0', '\x3', '\x2', '\x2', '\x2', '\xE7', '@', '\x3', '\x2', '\x2', 
		'\x2', '\xE8', '\xEA', '\t', '\b', '\x2', '\x2', '\xE9', '\xE8', '\x3', 
		'\x2', '\x2', '\x2', '\xEA', '\xEB', '\x3', '\x2', '\x2', '\x2', '\xEB', 
		'\xE9', '\x3', '\x2', '\x2', '\x2', '\xEB', '\xEC', '\x3', '\x2', '\x2', 
		'\x2', '\xEC', '\xED', '\x3', '\x2', '\x2', '\x2', '\xED', '\xEE', '\b', 
		'!', '\x2', '\x2', '\xEE', '\x42', '\x3', '\x2', '\x2', '\x2', '\xEF', 
		'\xF0', '\a', '\x31', '\x2', '\x2', '\xF0', '\xF1', '\a', '\x31', '\x2', 
		'\x2', '\xF1', '\xF5', '\x3', '\x2', '\x2', '\x2', '\xF2', '\xF4', '\v', 
		'\x2', '\x2', '\x2', '\xF3', '\xF2', '\x3', '\x2', '\x2', '\x2', '\xF4', 
		'\xF7', '\x3', '\x2', '\x2', '\x2', '\xF5', '\xF6', '\x3', '\x2', '\x2', 
		'\x2', '\xF5', '\xF3', '\x3', '\x2', '\x2', '\x2', '\xF6', '\xF8', '\x3', 
		'\x2', '\x2', '\x2', '\xF7', '\xF5', '\x3', '\x2', '\x2', '\x2', '\xF8', 
		'\xF9', '\a', '\f', '\x2', '\x2', '\xF9', '\xFA', '\x3', '\x2', '\x2', 
		'\x2', '\xFA', '\xFB', '\b', '\"', '\x2', '\x2', '\xFB', '\x44', '\x3', 
		'\x2', '\x2', '\x2', '\f', '\x2', '\xA3', '\xA5', '\xAD', '\xD7', '\xDD', 
		'\xE4', '\xE6', '\xEB', '\xF5', '\x3', '\b', '\x2', '\x2',
	};

	public static readonly ATN _ATN =
		new ATNDeserializer().Deserialize(_serializedATN);


}
} // namespace Redstar.Parse
