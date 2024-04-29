parser grammar TParser;

options {
	tokenVocab = TLexer;
}

root: expr EOF;

expr: Id                            # idential
    | Int                           # value
    | Float                         # value 
    | func                          # function
    | OpenPar expr ClosePar         # brackets_operation
    | Minus expr                    # unary_operation
    | Plus expr                     # unary_operation
    | expr Mul expr                 # binary_operation
    | expr Div expr                 # binary_operation
    | expr Plus expr                # binary_operation
    | expr Minus expr               # binary_operation
    | expr LEQ expr                 # binary_operation
    | expr GEQ expr                 # binary_operation
    | expr LT expr                  # binary_operation
    | expr GT expr                  # binary_operation
    | expr Equal expr               # binary_operation
    | expr And expr                 # binary_operation
    | expr Or expr                  # binary_operation
    | expr Qmark expr Colon expr    # ternary_operation
    ;

func: Id OpenPar expr (Comma expr)* ClosePar;
