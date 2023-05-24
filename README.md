# A lambda calculus interpreter.

## Why?

Just for fun!

## Building.

To build this thing you will need `gcc`, `libc` and plan9's `mk`.

```
$ git clone REPO lambda
$ cd lambda
$ mk
```

## Running.

Type `./lambda` to start REPL.

## Syntax.

The sytax (at least for now) is fairly simplistic. No spaces are allowed.

`Term ::= Variable | Name | Lambda | Application | '(' Term ')'`

A term is either a `Variable` or a `Lambda` or an `Application`.
You can also use parentheses for grouping.

`Variable ::= a | b | ... | z`

Variables are lowercase english letters.

`Application ::= Application Term`

Application is just two terms written side by side.
Application is left associative as usual.

`Lambda ::= Variable '.' Term`

Lambdas are defined as a 'Variable' and a `Term` separated by `.`.
There is no `λ` or `\` at the beginning.

`Name ::= A | B | ... | Z`

Uppercase english letters can be used to create aliases for terms.
During the syntactical analysis phase they are just replaced
by the aliased term.

`Binding ::= Name '=' Term`

You can provide aliases for terms.
`Term` is not be evaluated at the time of aliasing, only the syntactical analysis takes place.

### Some examples:

```
> a
- a
> ab
- ab
> x.x
- (1.1)
> x.xa
- (1.1a)
> (x.x)a
- (1.1)a
- a
> S=x.x
```
