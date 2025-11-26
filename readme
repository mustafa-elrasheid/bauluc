# Intro
I'm new to making a programming langauges
this is a compiler for a programming language that I'm making
the only libraries that get used should are the standard libraries give for input and output

# Tools
* makefile
* g++

# additions
this project will not support backwards compatabiliy for anything done with previous version of it.
if you would like to add new stuff you can do a pull request and i would try to hopefully read all of them

# code base
first, there are headers and cpp files. 
## headers
no header file can include headers from a higher level or same level, only lower levels.
### bottom level headers
* bytecode (backend)
* depednance (backend)
* lexer (frontend)
### top headers:
* grammer (frontend): includes lexer 
* vm (backend): includes bytecode and dependance 
### top top headers:
* parser: uses grammer and lexer (frontend)
## cpps
they can only include one header except for main.cpp