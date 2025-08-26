## Express
> ### A simple arithmetic expression evaluator with operator precedence using Recursive descent Pratt parsing.

```bash
$ ./express -h
Usage:
    -h          show this help message
    -p <num>    set precision for output numbers
    -i          start interactive repl mode
    -t / -l     show lexer tokens
    -a          show ast for expression
```


> #### Evaluate an expression


```bash
$ ./express '4 + 3 * 11 - 13 % (-4) ^ 2'
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.0
```


> #### Set precision for result


```bash
$ ./express -p 3 '4 + 3 * 11 - 13 % (-4) ^ 2'
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.000
```


> #### Show AST (Abstract Synatx Tree) for expression


```bash
$ ./express -a '4 + 3 * 11 - 13 % (-4) ^ 2'

AST:
Binary(
       Binary(
              Numeric(4.0),
              ADD,
              Binary(
                     Numeric(3.0),
                     MUL,
                     Numeric(11.0)
              )
       ),
       SUB,
       Binary(
              Numeric(13.0),
              MOD,
              Binary(
                     Unary(
                            NEGATIVE,
                            Numeric(4.0)
                     ),
                     POW,
                     Numeric(2.0)
              )
       )
)
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.0
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
4 + 3 * 11 - 13 % (-4) ^ 2 = 24.0
```


> #### Start interactive repl


```bash
$ ./express -i
Usage:
    \?     show this help message
    \h     alternative to \?
    \q     quit this repl
    \r     toggle show result
    \a     toggle show AST
    \l     toggle show lexer tokens
    \p     set precision for numbers

expression >> 1 + 3
1 + 3 = 4.0

expression >> \a
Show AST: on

expression >> 1 + 3

AST:
Binary(
       Numeric(1.0),
       ADD,
       Numeric(3.0)
)
1 + 3 = 4.0

expression >> \p 4
Precision set: 4

expression >> 1 + 3

AST:
Binary(
       Numeric(1.0000),
       ADD,
       Numeric(3.0000)
)
1 + 3 = 4.0000

expression >>

```
