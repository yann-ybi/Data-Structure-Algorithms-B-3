#include "btree.h"

int main(int argc, char** argv){

    int degree = std::stoi(argv[1]);
    std::string infn_words = std::string(argv[2]);
    std::string infn_search = std::string(argv[3]);

    std::string outfn_dot = std::string(argv[4]);

    std::ifstream words(infn_words);
    std::string word;

    BT tree(degree);

    while (std::getline(words,word)){
        tree.insert(word);
    }

    std::ifstream searches(infn_search);

    while (std::getline(searches, word)){
        tree.search(word);
    }

    std::ofstream of(outfn_dot + ".dot");
    of << "digraph G {\n" << std::endl;

    tree.print_dot(of,tree.top_root);

    of << "\n}";

}