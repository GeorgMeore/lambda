# A lambda calculus interpreter.

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

The sytax analyzer (at least for now) is fairly simplistic.
All the names are exactly one character long and no spaces are allowed.
Lowercase letters are used for variables and uppercase for binding names.

### Example.

```
> a
- a
> ab
- ab
> x.x
- (1.1)
> (x.x)a
- (1.1)a
- a
> S=x.x
```
