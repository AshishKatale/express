## Express
> ### A simple arithmetic expression evaluator with operator precedence using Recursive descent Pratt parsing.

```bash
$ ./express -h
Usage:
    -h        show this help message
    -i        start interactive repl mode
    -t  -l    show lexer tokens
    -a        show ast for expression
```


> #### Evaluate an expression


```bash
$ ./express '4 + 3 * 11 - 13 % (-4) ^ 2'
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.000000
```


> #### Show AST (Abstract Synatx Tree) an expression


```bash
$ ./express -a '4 + 3 * 11 - 13 % (-4) ^ 2'

AST:
Binary(
       Binary(
              Numeric(4.000000),
              ADD,
              Binary(
                     Numeric(3.000000),
                     MUL,
                     Numeric(11.000000)
              )
       ),
       SUB,
       Binary(
              Numeric(13.000000),
              MOD,
              Binary(
                     Unary(
                            NEGATIVE,
                            Numeric(4.000000)
                     ),
                     POW,
                     Numeric(2.000000)
              )
       )
)
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.000000
```


> #### Show generated lexer tokens for expression


```bash
$ ./express -l '4 + 3 * 11 - 13 % (-4) ^ 2'

Tokens [15]: [
    Token { type=NUM, token='4' }
    Token { type=PLUS, token='+' }
    Token { type=NUM, token='3' }
    Token { type=STAR, token='*' }
    Token { type=NUM, token='11' }
    Token { type=MINUS, token='-' }
    Token { type=NUM, token='13' }
    Token { type=MOD, token='%' }
    Token { type=LPAREN, token='(' }
    Token { type=MINUS, token='-' }
    Token { type=NUM, token='4' }
    Token { type=RPAREN, token=')' }
    Token { type=CARET, token='^' }
    Token { type=NUM, token='2' }
    Token { type=EOF, token='' }
]
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.000000
```


> #### Start interactive repl


```bash
$ ./express -i
Usage:
    \q     quit this repl
    \h     show this help message
    \r     toggle show result
    \a     toggle show AST
    \l     toggle show lexer tokens

expression >> 1 + 3
1 + 3 = 4.000000

expression >> \a
Show AST: on

expression >> 1 + 3

AST:
Binary(
       Numeric(1.000000),
       ADD,
       Numeric(3.000000)
)
1 + 3 = 4.000000

expression >>

```
