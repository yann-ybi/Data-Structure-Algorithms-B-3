#include "btree.h"

int main(int argc, char** argv){

    int degree = std::stoi(argv[1]); // we store the maximum number of keys desired for nodes of the tree
    std::string infn_words = std::string(argv[2]); // we store the name of the file of words to insert
    std::string infn_search = std::string(argv[3]); // we store the name of the file of the words to search for
    std::string outfn_dot = std::string(argv[4]); // we save the name desire for the output dot file

    // we convert tell computer that the string are files to open
    std::ifstream words(infn_words); 
    std::ifstream searches(infn_search);

    std::string word; // initialise a string for the words

    BT tree(degree); // construct the tree using the degree entered

    // for each line of the list of words file we call the insertion function for the word
    while (std::getline(words,word)){
        tree.insert(word); 
    }
    // for each line of words to search for we call the search funtion for the word
    while (std::getline(searches, word)){
        tree.search(word);
    }

    // we tell the computer a string is the name of the output file to create
    std::ofstream of(outfn_dot + ".dot");

    // we use the dot language on that output file
    of << "digraph G {\n" << std::endl; 
    tree.print_dot(of,tree.top_root); // call our function to print the graph on that output file with dot language
    of << "\n}";

}