PURPOSE:
Arty is a natural language processing based shell.
It is intended for regular shell users who wish to have a smarter one.
It contains both a programming language interpreter and a natural language interpreter.
Thus, you can have the power of a regular shell, together with a playfull experience as Arty learns about you.

INSTALLATION:

USE ARTY AS DEFAULT SHELL:
add arty path to /etc/shells
then chsh -s path_to_arty <username>

SPECIFICATION OF THE LANGUAGE:

LANGUAGE:

keyword := 'if' + 'else' + 'for' + 'while' + 'elif' 
number := (['0'-'9']+)('.'['0'-'9']*)?('E'('+' + '-')?['0'-'9']*)?  	(ex: 1, 01, 0., 0.1, 1E1, 1E+01, 1E-2, 1.0E1)
identifier := (['a'-'z''A'- 'Z']+)['0'-'9']* 				(ex: myvar, myvar1, m02)
operator := '+' + '-' + '*' + '/' + '%'    

other tokens := ',' '(' ')' '{' '}' ';' '<-'

GRAMMAR:

Expr := Term operator Expr | Term
Term := number | identifier
Range := [number:number] | [number:number:number]
FuncCall := identifer Tupple
Tupple := '(' ')' | '(' TupleItems ')'
TuppleItems := Term | Term ',' TuppleItems
Assignement := identifer '<-' Term | identifer '<-' Func
Func := Tuple '->' Tuple Scope
Scope := '{' '}' | '{' Expr '}'

PRIMITIF TYPES:

number, function, type, range, enum