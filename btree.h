#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <stack>

// class for nodes in our b-tree

class BT_Node {

    private:

        int num_keys; // current size of the node
        std::list <std::pair<std::string, int> > keys; // array of std pair no copied
        std::list <BT_Node*> children; // array of child pointers
        bool is_leaf; // node is a leaf or not
        
    public:

        BT_Node(); // constructor to create a node using the class

    friend class BT; // class b-tree has acces to everything in the class node
};

class BT {

    private:

        int degree;
    
    public:
        BT_Node* top_root; // pointer to the node root of the tree 
        BT_Node* root;
        std::stack <std::pair<BT_Node*, int> > path;

        BT(int d); // B-tree constructor 

        void bottom_up();

        //insert a key into a node  
        void insert(std::string k);

        void search(std::string k); 

        void print_dot(std::ofstream& of, BT_Node* print);
};
