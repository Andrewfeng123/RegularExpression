# RegularExpression
Parser from regular expression to NFA

## Why am I Making This
This project comes from the inspiration I got from taking the course CSC240, Intro to the Theory of Computation, at University of Toronto. I decided to make something practical after reading the proofs about the equivalence of regular expressions and finite state automaton.

## Result
The parser can take in any valid regular expression involving (,),\*,., or just juxtuposition in place. The alphabet the regular expression is over is configurated in the constructor of the Parser class. The notation is definitely not standard in regular expression libraries, but I wanted it to resemble the course content.

Here's a demo of the proram.

![demo](https://user-images.githubusercontent.com/19532712/78835767-bf647380-79be-11ea-8d8c-c31a91436157.PNG)
