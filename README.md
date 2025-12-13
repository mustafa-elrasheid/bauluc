# Introduction
This is a compiler for a programming language called Baulu, that tried to achieve Python like syntax, with C like type safety, while trying to be memory safe without significant performance overhaults. It's a compiled language of course, that's why it has a compiler in the biggening.
I'm new to making a programming langauges. And this is a compiler that I'm making tries to achieve it's result by only using standard C++ and the standard library. that includes the frontend and the backend

# Tools
**necessery tools include:**
- makefile
- g++

**optional tools:**
- valgrind
- gdb

# The syntax
### Function declaration
the function declaration grammer uses the keyword `function` followed by an identifier, followed by paramters defined with `let TYPE IDENTIFIER` seperated by comas and all inside round brackets, followed by the return type if needed, then two colums. an example would be:
 - `function foo(let int x) => int:`
 - `function main() => int:`
 - `function is_prime(let int n) => bool`,
 - `function sleep(let int seconds):`

### Blocks and Nesting
this language uses the offside rule using tabs , you can also optionally use curly brackers for inline blocks, example:
```
if x:
return 0;
```
```
if x: {return 0;}
```
**note:** only use tabs for nesting

### conntrol flow
**return statements** `return expr;` 
**while statements** `while expr:` 
**if statements** `if expr:`

### Arithmetic Operations
These can be done in a statement that consist of an expression and a semicolon, as in`expr;`, the operation you can do are `=` for assignment, and [`+`,`-`,`/`,`*`,`%`] for Arithmetic

### Variable Declation
you need to use the `let` keyword followed by a type followed by an identifier, there is only `int`.
all veriables need to be assigned

# Future Additions
This project will doesn't gurantees backwards support compatabiliy for anything done with previous version of it.
iI you would like to add new stuff you can do a pull request or add and issue, and I would try to hopefully read all of them