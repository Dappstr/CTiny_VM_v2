# Intro:

This tiny VM is something I worked on over a few days as practice for getting into langdev. The VM works by manipulating a dynamic array of integers representing a stack.

# Syntax

`set(I, N)` where `I` is the index and `N` is the value to set the value at a certain index

`alloc(N)` where `N` is a value to allocate a certain amount of integers

`dealloc(N)` where `N` is a value to deallocate a certain amount of integers

`print` will print all the allocated values in the stack

`add` will print the value of all the values on the stack added to each other

`mult` will print the value of all the values on the stack multiplied by each other

To declare a variable (integers are the only type supported), you do `var x = 10` for example

To push a value or variable to the stack, you do `push N` where `N` can be either a variable or a number literal

To pop a value, you simply write `pop`
