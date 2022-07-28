# <p align="center"> UNIVERSITY OF RHODE ISLAND </p>
# <p align="center"> CSC212 FINAL PROJECT: B-TREE IMPLEMENTATION</p>
<p> Professor Christian R Esteves </p>
<p> By Yann Youbi </p>

# Introduction

This is a repository for source code of the implementation of a b-tree, a self balancing tree data structure fully presented in this [report](https://docs.google.com/document/d/1U6YQNCGhB913AHjNxEAv0Uc0Uue7i_7pCujP0fb6XwA/edit?usp=sharing) and illustrated with these [slides](https://docs.google.com/presentation/d/1f7UmGo35Y4HxlUk8YhF094GzNUajxF-z5FUNUBeX5og/edit?usp=sharing)

# How to use the source code?

The source code is written in C++ and should be compiled using an Integrated Development Environment (IDE)
An IDE is a software application for easier software development. 
Compiling is calling a function called a compiler to translate the computer code written into a another language. It is mainly used to create an executable program. 

# How to compile the code

The source code contains three files that can be in the same folder and after accesing the folder of the file with: cd name of folder.
It can be compiles using:
g++ -std=c++11 -Wall main.cpp btree.cpp -o compiledFileName
- <p> compiledFileName: name desired for the compiled file. </p>
  
### example: g++ -std=c++11 -Wall main.cpp btree.cpp -o btree

# How to run the code?
  
The code should be run with: ./compiledFileName degree fname1 fname2 fname3
- compiledFileName: The name of the compiled filed
- degree: a numer for the degree of the tree we want to create (number of maximum keys in a node)
- fname1: the name of a file containing the list of words to insert in the tree
- fname2: the name of a file containing the list of words to search in the tree
- fname3: the name of the output dot file we generate fom the list of words.

### example: ./btree 5 thousands.txt search.txt tree 

# What happens we run the code?
  
Once you run the code, it will create DOT file that once processed with the right extention will show a b-tree with your list of words inserted followed by the number of times they've tried to be inserted.
In the terminal, it should tell you whether or not your words entered in the search file are present in the tree and if present then the number of times they are repeated. 
If the tree is empty, it should annouce it to you as well.
  
 
  
