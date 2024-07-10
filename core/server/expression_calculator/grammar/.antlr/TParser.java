// Generated from c:/Users/Igor/Desktop/FDESolverServer/core/server/expression_calculator/grammar/TParser.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class TParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		PLUS=1, MINUS=2, TIMES=3, DIV=4, LPAREN=5, RPAREN=6, LT=7, GT=8, LEQ=9, 
		GEQ=10, EQ=11, NEQ=12, NOT=13, AND=14, OR=15, COMMA=16, DOT=17, QUESTION=18, 
		COLON=19, Identifier=20, Constant=21, DigitSequence=22, WS=23;
	public static final int
		RULE_root = 0, RULE_mainExpression = 1, RULE_conditionalExpression = 2, 
		RULE_logicalOrExpression = 3, RULE_logicalAndExpression = 4, RULE_equalityExpression = 5, 
		RULE_relationalExpression = 6, RULE_additiveExpression = 7, RULE_multiplicativeExpression = 8, 
		RULE_castExpression = 9, RULE_unaryExpression = 10, RULE_functionExpression = 11, 
		RULE_argumentExpressionList = 12;
	private static String[] makeRuleNames() {
		return new String[] {
			"root", "mainExpression", "conditionalExpression", "logicalOrExpression", 
			"logicalAndExpression", "equalityExpression", "relationalExpression", 
			"additiveExpression", "multiplicativeExpression", "castExpression", "unaryExpression", 
			"functionExpression", "argumentExpressionList"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'<'", "'>'", "'<='", 
			"'>='", "'=='", "'!='", "'!'", "'&&'", "'||'", "','", "'.'", "'?'", "':'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "PLUS", "MINUS", "TIMES", "DIV", "LPAREN", "RPAREN", "LT", "GT", 
			"LEQ", "GEQ", "EQ", "NEQ", "NOT", "AND", "OR", "COMMA", "DOT", "QUESTION", 
			"COLON", "Identifier", "Constant", "DigitSequence", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "TParser.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public TParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class RootContext extends ParserRuleContext {
		public MainExpressionContext mainExpression() {
			return getRuleContext(MainExpressionContext.class,0);
		}
		public TerminalNode EOF() { return getToken(TParser.EOF, 0); }
		public RootContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_root; }
	}

	public final RootContext root() throws RecognitionException {
		RootContext _localctx = new RootContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_root);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(26);
			mainExpression();
			setState(27);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MainExpressionContext extends ParserRuleContext {
		public MainExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mainExpression; }
	 
		public MainExpressionContext() { }
		public void copyFrom(MainExpressionContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class MainExpressionIdentifierContext extends MainExpressionContext {
		public TerminalNode Identifier() { return getToken(TParser.Identifier, 0); }
		public MainExpressionIdentifierContext(MainExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class MainExpressionConstantContext extends MainExpressionContext {
		public TerminalNode Constant() { return getToken(TParser.Constant, 0); }
		public MainExpressionConstantContext(MainExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class MainExpressionCondExprContext extends MainExpressionContext {
		public ConditionalExpressionContext conditionalExpression() {
			return getRuleContext(ConditionalExpressionContext.class,0);
		}
		public MainExpressionCondExprContext(MainExpressionContext ctx) { copyFrom(ctx); }
	}

	public final MainExpressionContext mainExpression() throws RecognitionException {
		MainExpressionContext _localctx = new MainExpressionContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_mainExpression);
		try {
			setState(32);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
			case 1:
				_localctx = new MainExpressionCondExprContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(29);
				conditionalExpression();
				}
				break;
			case 2:
				_localctx = new MainExpressionConstantContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(30);
				match(Constant);
				}
				break;
			case 3:
				_localctx = new MainExpressionIdentifierContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(31);
				match(Identifier);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ConditionalExpressionContext extends ParserRuleContext {
		public LogicalOrExpressionContext logicalOrExpression() {
			return getRuleContext(LogicalOrExpressionContext.class,0);
		}
		public TerminalNode QUESTION() { return getToken(TParser.QUESTION, 0); }
		public MainExpressionContext mainExpression() {
			return getRuleContext(MainExpressionContext.class,0);
		}
		public TerminalNode COLON() { return getToken(TParser.COLON, 0); }
		public ConditionalExpressionContext conditionalExpression() {
			return getRuleContext(ConditionalExpressionContext.class,0);
		}
		public ConditionalExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditionalExpression; }
	}

	public final ConditionalExpressionContext conditionalExpression() throws RecognitionException {
		ConditionalExpressionContext _localctx = new ConditionalExpressionContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_conditionalExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(34);
			logicalOrExpression();
			setState(40);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==QUESTION) {
				{
				setState(35);
				match(QUESTION);
				setState(36);
				mainExpression();
				setState(37);
				match(COLON);
				setState(38);
				conditionalExpression();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LogicalOrExpressionContext extends ParserRuleContext {
		public List<LogicalAndExpressionContext> logicalAndExpression() {
			return getRuleContexts(LogicalAndExpressionContext.class);
		}
		public LogicalAndExpressionContext logicalAndExpression(int i) {
			return getRuleContext(LogicalAndExpressionContext.class,i);
		}
		public List<TerminalNode> OR() { return getTokens(TParser.OR); }
		public TerminalNode OR(int i) {
			return getToken(TParser.OR, i);
		}
		public LogicalOrExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_logicalOrExpression; }
	}

	public final LogicalOrExpressionContext logicalOrExpression() throws RecognitionException {
		LogicalOrExpressionContext _localctx = new LogicalOrExpressionContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_logicalOrExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(42);
			logicalAndExpression();
			setState(47);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==OR) {
				{
				{
				setState(43);
				match(OR);
				setState(44);
				logicalAndExpression();
				}
				}
				setState(49);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LogicalAndExpressionContext extends ParserRuleContext {
		public List<EqualityExpressionContext> equalityExpression() {
			return getRuleContexts(EqualityExpressionContext.class);
		}
		public EqualityExpressionContext equalityExpression(int i) {
			return getRuleContext(EqualityExpressionContext.class,i);
		}
		public List<TerminalNode> AND() { return getTokens(TParser.AND); }
		public TerminalNode AND(int i) {
			return getToken(TParser.AND, i);
		}
		public LogicalAndExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_logicalAndExpression; }
	}

	public final LogicalAndExpressionContext logicalAndExpression() throws RecognitionException {
		LogicalAndExpressionContext _localctx = new LogicalAndExpressionContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_logicalAndExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(50);
			equalityExpression();
			setState(55);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==AND) {
				{
				{
				setState(51);
				match(AND);
				setState(52);
				equalityExpression();
				}
				}
				setState(57);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class EqualityExpressionContext extends ParserRuleContext {
		public List<RelationalExpressionContext> relationalExpression() {
			return getRuleContexts(RelationalExpressionContext.class);
		}
		public RelationalExpressionContext relationalExpression(int i) {
			return getRuleContext(RelationalExpressionContext.class,i);
		}
		public TerminalNode EQ() { return getToken(TParser.EQ, 0); }
		public TerminalNode NEQ() { return getToken(TParser.NEQ, 0); }
		public EqualityExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_equalityExpression; }
	}

	public final EqualityExpressionContext equalityExpression() throws RecognitionException {
		EqualityExpressionContext _localctx = new EqualityExpressionContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_equalityExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(58);
			relationalExpression();
			setState(61);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQ || _la==NEQ) {
				{
				setState(59);
				_la = _input.LA(1);
				if ( !(_la==EQ || _la==NEQ) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(60);
				relationalExpression();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class RelationalExpressionContext extends ParserRuleContext {
		public List<AdditiveExpressionContext> additiveExpression() {
			return getRuleContexts(AdditiveExpressionContext.class);
		}
		public AdditiveExpressionContext additiveExpression(int i) {
			return getRuleContext(AdditiveExpressionContext.class,i);
		}
		public TerminalNode LT() { return getToken(TParser.LT, 0); }
		public TerminalNode GT() { return getToken(TParser.GT, 0); }
		public TerminalNode LEQ() { return getToken(TParser.LEQ, 0); }
		public TerminalNode GEQ() { return getToken(TParser.GEQ, 0); }
		public RelationalExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_relationalExpression; }
	}

	public final RelationalExpressionContext relationalExpression() throws RecognitionException {
		RelationalExpressionContext _localctx = new RelationalExpressionContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_relationalExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(63);
			additiveExpression();
			setState(66);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 1920L) != 0)) {
				{
				setState(64);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 1920L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(65);
				additiveExpression();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AdditiveExpressionContext extends ParserRuleContext {
		public List<MultiplicativeExpressionContext> multiplicativeExpression() {
			return getRuleContexts(MultiplicativeExpressionContext.class);
		}
		public MultiplicativeExpressionContext multiplicativeExpression(int i) {
			return getRuleContext(MultiplicativeExpressionContext.class,i);
		}
		public List<TerminalNode> PLUS() { return getTokens(TParser.PLUS); }
		public TerminalNode PLUS(int i) {
			return getToken(TParser.PLUS, i);
		}
		public List<TerminalNode> MINUS() { return getTokens(TParser.MINUS); }
		public TerminalNode MINUS(int i) {
			return getToken(TParser.MINUS, i);
		}
		public AdditiveExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_additiveExpression; }
	}

	public final AdditiveExpressionContext additiveExpression() throws RecognitionException {
		AdditiveExpressionContext _localctx = new AdditiveExpressionContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_additiveExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(68);
			multiplicativeExpression();
			setState(73);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PLUS || _la==MINUS) {
				{
				{
				setState(69);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(70);
				multiplicativeExpression();
				}
				}
				setState(75);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MultiplicativeExpressionContext extends ParserRuleContext {
		public List<CastExpressionContext> castExpression() {
			return getRuleContexts(CastExpressionContext.class);
		}
		public CastExpressionContext castExpression(int i) {
			return getRuleContext(CastExpressionContext.class,i);
		}
		public List<TerminalNode> TIMES() { return getTokens(TParser.TIMES); }
		public TerminalNode TIMES(int i) {
			return getToken(TParser.TIMES, i);
		}
		public List<TerminalNode> DIV() { return getTokens(TParser.DIV); }
		public TerminalNode DIV(int i) {
			return getToken(TParser.DIV, i);
		}
		public MultiplicativeExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_multiplicativeExpression; }
	}

	public final MultiplicativeExpressionContext multiplicativeExpression() throws RecognitionException {
		MultiplicativeExpressionContext _localctx = new MultiplicativeExpressionContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_multiplicativeExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(76);
			castExpression();
			setState(81);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==TIMES || _la==DIV) {
				{
				{
				setState(77);
				_la = _input.LA(1);
				if ( !(_la==TIMES || _la==DIV) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(78);
				castExpression();
				}
				}
				setState(83);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CastExpressionContext extends ParserRuleContext {
		public CastExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_castExpression; }
	 
		public CastExpressionContext() { }
		public void copyFrom(CastExpressionContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CastExpressionParenContext extends CastExpressionContext {
		public TerminalNode LPAREN() { return getToken(TParser.LPAREN, 0); }
		public ConditionalExpressionContext conditionalExpression() {
			return getRuleContext(ConditionalExpressionContext.class,0);
		}
		public TerminalNode RPAREN() { return getToken(TParser.RPAREN, 0); }
		public CastExpressionParenContext(CastExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CastExpressionUnaryContext extends CastExpressionContext {
		public UnaryExpressionContext unaryExpression() {
			return getRuleContext(UnaryExpressionContext.class,0);
		}
		public CastExpressionUnaryContext(CastExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CastExpressionConstantContext extends CastExpressionContext {
		public TerminalNode Constant() { return getToken(TParser.Constant, 0); }
		public CastExpressionConstantContext(CastExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class CastExpressionIdentifierContext extends CastExpressionContext {
		public TerminalNode Identifier() { return getToken(TParser.Identifier, 0); }
		public CastExpressionIdentifierContext(CastExpressionContext ctx) { copyFrom(ctx); }
	}

	public final CastExpressionContext castExpression() throws RecognitionException {
		CastExpressionContext _localctx = new CastExpressionContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_castExpression);
		try {
			setState(91);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,8,_ctx) ) {
			case 1:
				_localctx = new CastExpressionUnaryContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(84);
				unaryExpression();
				}
				break;
			case 2:
				_localctx = new CastExpressionParenContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(85);
				match(LPAREN);
				setState(86);
				conditionalExpression();
				setState(87);
				match(RPAREN);
				}
				break;
			case 3:
				_localctx = new CastExpressionConstantContext(_localctx);
				enterOuterAlt(_localctx, 3);
				{
				setState(89);
				match(Constant);
				}
				break;
			case 4:
				_localctx = new CastExpressionIdentifierContext(_localctx);
				enterOuterAlt(_localctx, 4);
				{
				setState(90);
				match(Identifier);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class UnaryExpressionContext extends ParserRuleContext {
		public UnaryExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_unaryExpression; }
	 
		public UnaryExpressionContext() { }
		public void copyFrom(UnaryExpressionContext ctx) {
			super.copyFrom(ctx);
		}
	}
	@SuppressWarnings("CheckReturnValue")
	public static class UnaryExpressionUnCastExprContext extends UnaryExpressionContext {
		public CastExpressionContext castExpression() {
			return getRuleContext(CastExpressionContext.class,0);
		}
		public TerminalNode PLUS() { return getToken(TParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(TParser.MINUS, 0); }
		public TerminalNode NOT() { return getToken(TParser.NOT, 0); }
		public UnaryExpressionUnCastExprContext(UnaryExpressionContext ctx) { copyFrom(ctx); }
	}
	@SuppressWarnings("CheckReturnValue")
	public static class UnaryExpressionFuncContext extends UnaryExpressionContext {
		public FunctionExpressionContext functionExpression() {
			return getRuleContext(FunctionExpressionContext.class,0);
		}
		public UnaryExpressionFuncContext(UnaryExpressionContext ctx) { copyFrom(ctx); }
	}

	public final UnaryExpressionContext unaryExpression() throws RecognitionException {
		UnaryExpressionContext _localctx = new UnaryExpressionContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_unaryExpression);
		int _la;
		try {
			setState(96);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PLUS:
			case MINUS:
			case NOT:
				_localctx = new UnaryExpressionUnCastExprContext(_localctx);
				enterOuterAlt(_localctx, 1);
				{
				setState(93);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 8198L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(94);
				castExpression();
				}
				break;
			case Identifier:
				_localctx = new UnaryExpressionFuncContext(_localctx);
				enterOuterAlt(_localctx, 2);
				{
				setState(95);
				functionExpression();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FunctionExpressionContext extends ParserRuleContext {
		public TerminalNode Identifier() { return getToken(TParser.Identifier, 0); }
		public TerminalNode LPAREN() { return getToken(TParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(TParser.RPAREN, 0); }
		public ArgumentExpressionListContext argumentExpressionList() {
			return getRuleContext(ArgumentExpressionListContext.class,0);
		}
		public FunctionExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionExpression; }
	}

	public final FunctionExpressionContext functionExpression() throws RecognitionException {
		FunctionExpressionContext _localctx = new FunctionExpressionContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_functionExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(98);
			match(Identifier);
			setState(99);
			match(LPAREN);
			setState(101);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & 3153958L) != 0)) {
				{
				setState(100);
				argumentExpressionList();
				}
			}

			setState(103);
			match(RPAREN);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArgumentExpressionListContext extends ParserRuleContext {
		public List<MainExpressionContext> mainExpression() {
			return getRuleContexts(MainExpressionContext.class);
		}
		public MainExpressionContext mainExpression(int i) {
			return getRuleContext(MainExpressionContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(TParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(TParser.COMMA, i);
		}
		public ArgumentExpressionListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_argumentExpressionList; }
	}

	public final ArgumentExpressionListContext argumentExpressionList() throws RecognitionException {
		ArgumentExpressionListContext _localctx = new ArgumentExpressionListContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_argumentExpressionList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(105);
			mainExpression();
			setState(110);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(106);
				match(COMMA);
				setState(107);
				mainExpression();
				}
				}
				setState(112);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u0017r\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001\u0002"+
		"\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004\u0002"+
		"\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007\u0002"+
		"\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b\u0002"+
		"\f\u0007\f\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0003\u0001!\b\u0001\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0003\u0002)\b\u0002\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0005\u0003.\b\u0003\n\u0003\f\u00031\t\u0003"+
		"\u0001\u0004\u0001\u0004\u0001\u0004\u0005\u00046\b\u0004\n\u0004\f\u0004"+
		"9\t\u0004\u0001\u0005\u0001\u0005\u0001\u0005\u0003\u0005>\b\u0005\u0001"+
		"\u0006\u0001\u0006\u0001\u0006\u0003\u0006C\b\u0006\u0001\u0007\u0001"+
		"\u0007\u0001\u0007\u0005\u0007H\b\u0007\n\u0007\f\u0007K\t\u0007\u0001"+
		"\b\u0001\b\u0001\b\u0005\bP\b\b\n\b\f\bS\t\b\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0003\t\\\b\t\u0001\n\u0001\n\u0001\n\u0003"+
		"\na\b\n\u0001\u000b\u0001\u000b\u0001\u000b\u0003\u000bf\b\u000b\u0001"+
		"\u000b\u0001\u000b\u0001\f\u0001\f\u0001\f\u0005\fm\b\f\n\f\f\fp\t\f\u0001"+
		"\f\u0000\u0000\r\u0000\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014"+
		"\u0016\u0018\u0000\u0005\u0001\u0000\u000b\f\u0001\u0000\u0007\n\u0001"+
		"\u0000\u0001\u0002\u0001\u0000\u0003\u0004\u0002\u0000\u0001\u0002\r\r"+
		"s\u0000\u001a\u0001\u0000\u0000\u0000\u0002 \u0001\u0000\u0000\u0000\u0004"+
		"\"\u0001\u0000\u0000\u0000\u0006*\u0001\u0000\u0000\u0000\b2\u0001\u0000"+
		"\u0000\u0000\n:\u0001\u0000\u0000\u0000\f?\u0001\u0000\u0000\u0000\u000e"+
		"D\u0001\u0000\u0000\u0000\u0010L\u0001\u0000\u0000\u0000\u0012[\u0001"+
		"\u0000\u0000\u0000\u0014`\u0001\u0000\u0000\u0000\u0016b\u0001\u0000\u0000"+
		"\u0000\u0018i\u0001\u0000\u0000\u0000\u001a\u001b\u0003\u0002\u0001\u0000"+
		"\u001b\u001c\u0005\u0000\u0000\u0001\u001c\u0001\u0001\u0000\u0000\u0000"+
		"\u001d!\u0003\u0004\u0002\u0000\u001e!\u0005\u0015\u0000\u0000\u001f!"+
		"\u0005\u0014\u0000\u0000 \u001d\u0001\u0000\u0000\u0000 \u001e\u0001\u0000"+
		"\u0000\u0000 \u001f\u0001\u0000\u0000\u0000!\u0003\u0001\u0000\u0000\u0000"+
		"\"(\u0003\u0006\u0003\u0000#$\u0005\u0012\u0000\u0000$%\u0003\u0002\u0001"+
		"\u0000%&\u0005\u0013\u0000\u0000&\'\u0003\u0004\u0002\u0000\')\u0001\u0000"+
		"\u0000\u0000(#\u0001\u0000\u0000\u0000()\u0001\u0000\u0000\u0000)\u0005"+
		"\u0001\u0000\u0000\u0000*/\u0003\b\u0004\u0000+,\u0005\u000f\u0000\u0000"+
		",.\u0003\b\u0004\u0000-+\u0001\u0000\u0000\u0000.1\u0001\u0000\u0000\u0000"+
		"/-\u0001\u0000\u0000\u0000/0\u0001\u0000\u0000\u00000\u0007\u0001\u0000"+
		"\u0000\u00001/\u0001\u0000\u0000\u000027\u0003\n\u0005\u000034\u0005\u000e"+
		"\u0000\u000046\u0003\n\u0005\u000053\u0001\u0000\u0000\u000069\u0001\u0000"+
		"\u0000\u000075\u0001\u0000\u0000\u000078\u0001\u0000\u0000\u00008\t\u0001"+
		"\u0000\u0000\u000097\u0001\u0000\u0000\u0000:=\u0003\f\u0006\u0000;<\u0007"+
		"\u0000\u0000\u0000<>\u0003\f\u0006\u0000=;\u0001\u0000\u0000\u0000=>\u0001"+
		"\u0000\u0000\u0000>\u000b\u0001\u0000\u0000\u0000?B\u0003\u000e\u0007"+
		"\u0000@A\u0007\u0001\u0000\u0000AC\u0003\u000e\u0007\u0000B@\u0001\u0000"+
		"\u0000\u0000BC\u0001\u0000\u0000\u0000C\r\u0001\u0000\u0000\u0000DI\u0003"+
		"\u0010\b\u0000EF\u0007\u0002\u0000\u0000FH\u0003\u0010\b\u0000GE\u0001"+
		"\u0000\u0000\u0000HK\u0001\u0000\u0000\u0000IG\u0001\u0000\u0000\u0000"+
		"IJ\u0001\u0000\u0000\u0000J\u000f\u0001\u0000\u0000\u0000KI\u0001\u0000"+
		"\u0000\u0000LQ\u0003\u0012\t\u0000MN\u0007\u0003\u0000\u0000NP\u0003\u0012"+
		"\t\u0000OM\u0001\u0000\u0000\u0000PS\u0001\u0000\u0000\u0000QO\u0001\u0000"+
		"\u0000\u0000QR\u0001\u0000\u0000\u0000R\u0011\u0001\u0000\u0000\u0000"+
		"SQ\u0001\u0000\u0000\u0000T\\\u0003\u0014\n\u0000UV\u0005\u0005\u0000"+
		"\u0000VW\u0003\u0004\u0002\u0000WX\u0005\u0006\u0000\u0000X\\\u0001\u0000"+
		"\u0000\u0000Y\\\u0005\u0015\u0000\u0000Z\\\u0005\u0014\u0000\u0000[T\u0001"+
		"\u0000\u0000\u0000[U\u0001\u0000\u0000\u0000[Y\u0001\u0000\u0000\u0000"+
		"[Z\u0001\u0000\u0000\u0000\\\u0013\u0001\u0000\u0000\u0000]^\u0007\u0004"+
		"\u0000\u0000^a\u0003\u0012\t\u0000_a\u0003\u0016\u000b\u0000`]\u0001\u0000"+
		"\u0000\u0000`_\u0001\u0000\u0000\u0000a\u0015\u0001\u0000\u0000\u0000"+
		"bc\u0005\u0014\u0000\u0000ce\u0005\u0005\u0000\u0000df\u0003\u0018\f\u0000"+
		"ed\u0001\u0000\u0000\u0000ef\u0001\u0000\u0000\u0000fg\u0001\u0000\u0000"+
		"\u0000gh\u0005\u0006\u0000\u0000h\u0017\u0001\u0000\u0000\u0000in\u0003"+
		"\u0002\u0001\u0000jk\u0005\u0010\u0000\u0000km\u0003\u0002\u0001\u0000"+
		"lj\u0001\u0000\u0000\u0000mp\u0001\u0000\u0000\u0000nl\u0001\u0000\u0000"+
		"\u0000no\u0001\u0000\u0000\u0000o\u0019\u0001\u0000\u0000\u0000pn\u0001"+
		"\u0000\u0000\u0000\f (/7=BIQ[`en";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}