#!/bin/bash

# If you want to use clangd for code completion or
# go-to-definition, you need compile_commands.json which includes 
# compile options for each source files.
# (In our project it is located in `build` directory.)
# It can be automatically generated by cmake if the option
# `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` is given.
# However, cmake generates compile options only for cpp file and
# compile options for header files are not generated.
# compdb (https://github.com/Sarcasm/compdb) provides a solution
# for the problem.
# It takes compile_commands.json and adds compile options
# for header files to it.

compdb -p build/ list > build/compile_commands.json.tmp
mv -f build/compile_commands.json.tmp build/compile_commands.json
