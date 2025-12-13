This directory contains the implementation of the compiler frontend.

**lexer:** doesn't include anything
**grammer:** includes lexer
**parser:** includes lexer and grammer

external utils for some error reporting have been placed outside this dir, but they can be easily removed to make the code self inclusive.
