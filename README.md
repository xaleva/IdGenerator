# IdGenerator
This is a sample Id Generator (for demonstration only).

## Dependencies:
*  c++14 
*  qmake (for building project)
*  boost test library

Build
qmake
make


## Features:
* Support all numerical integral types
* Can change behaviour depending on settings
*  Thread safe

It can throw several exceptions:
*  Storage cannot be reached
*  In case of settings 'loop' parameter to false (default) and reaching the type limit it will throw an exception 'Out of limit exception'

Does not support custom storage for index

