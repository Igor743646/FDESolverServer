// Generated from c:/Users/Igor/Desktop/FDESolverServer/core/server/expression_calculator/grammar/TParser.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link TParser}.
 */
public interface TParserListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link TParser#root}.
	 * @param ctx the parse tree
	 */
	void enterRoot(TParser.RootContext ctx);
	/**
	 * Exit a parse tree produced by {@link TParser#root}.
	 * @param ctx the parse tree
	 */
	void exitRoot(TParser.RootContext ctx);
	/**
	 * Enter a parse tree produced by the {@code function}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterFunction(TParser.FunctionContext ctx);
	/**
	 * Exit a parse tree produced by the {@code function}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitFunction(TParser.FunctionContext ctx);
	/**
	 * Enter a parse tree produced by the {@code brackets_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterBrackets_operation(TParser.Brackets_operationContext ctx);
	/**
	 * Exit a parse tree produced by the {@code brackets_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitBrackets_operation(TParser.Brackets_operationContext ctx);
	/**
	 * Enter a parse tree produced by the {@code idential}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterIdential(TParser.IdentialContext ctx);
	/**
	 * Exit a parse tree produced by the {@code idential}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitIdential(TParser.IdentialContext ctx);
	/**
	 * Enter a parse tree produced by the {@code value}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterValue(TParser.ValueContext ctx);
	/**
	 * Exit a parse tree produced by the {@code value}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitValue(TParser.ValueContext ctx);
	/**
	 * Enter a parse tree produced by the {@code unary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterUnary_operation(TParser.Unary_operationContext ctx);
	/**
	 * Exit a parse tree produced by the {@code unary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitUnary_operation(TParser.Unary_operationContext ctx);
	/**
	 * Enter a parse tree produced by the {@code binary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterBinary_operation(TParser.Binary_operationContext ctx);
	/**
	 * Exit a parse tree produced by the {@code binary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitBinary_operation(TParser.Binary_operationContext ctx);
	/**
	 * Enter a parse tree produced by the {@code ternary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterTernary_operation(TParser.Ternary_operationContext ctx);
	/**
	 * Exit a parse tree produced by the {@code ternary_operation}
	 * labeled alternative in {@link TParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitTernary_operation(TParser.Ternary_operationContext ctx);
	/**
	 * Enter a parse tree produced by {@link TParser#func}.
	 * @param ctx the parse tree
	 */
	void enterFunc(TParser.FuncContext ctx);
	/**
	 * Exit a parse tree produced by {@link TParser#func}.
	 * @param ctx the parse tree
	 */
	void exitFunc(TParser.FuncContext ctx);
}