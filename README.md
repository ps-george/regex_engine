# regex_engine
Regex engine written in C++.

Specification
-------

Implement a basic regex engine using C++. The
requirements are to create a program with the
following features:

- The program is built using the command `make bin/regex_engine` (in the directory `c`).

- The program takes as an input argument an ASCII regular expression,
  which can contain the following constructs:

  - Character literals (but not character ranges).
  
    - Character literals only include the alphabetic letters, numbers, and underscore,
      so each literal must match `[a-zA-Z0-9_]`.
    
    - The dot construct will not appear, as it is syntactic sugar for a character
      range containing all characters.

  - One-or-more (but not zero-or-more)

  - Grouping

  - Alternation

  Neither the regular expression nor input strings will contain whitespace.

- The program should read a sequence of input lines from stdin,
  and apply the regular expression to the line.

- Each input line should result in a corresponding output on stdout,
  which consists of either:

  - `Match` if the regular expression matches the _whole_ line

  - `NoMatch` if the regular expression does not match

Efficiency is not important (within reason!). So test inputs
are designed to have reasonable run-time, even if there is
a worst-case exponential execution time in the regex engine.

Example
-------

An example session would be (with input and output interleaved):

````
$ make bin/regex_engine
$ bin/regex_engine "ab+"
a
NoMatch
ab
Match
abb
Match
abab
NoMatch
````
