# IdGenerator
This is a sample Id Generator (for demonstration only).

Dependencies:
1.  c++14 
2.  qmake (for building project)
3.  boost test library

Build
qmake
make


Features:
1. Support all numerical integral types
2. Can change behaviour depending on settings
3.  Thread safe

It can throw several exceptions:
1.  Storage cannot be reached
2.  In case of settings 'loop' parameter to false (default) and reaching the type limit it will throw an exception 'Out of limit exception'

Does not support custom storage for index

