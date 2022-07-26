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

        BT_Node(){
            is_leaf = false;
            num_keys = 0;
        }; // constructor to create a node using the class

    friend class BT; // class b-tree has acces to everything in the class node
};

class BT {

    private:

        
        int degree;
    
    public:
        BT_Node* top_root; // pointer to the node root of the tree 
        BT_Node* root;
        std::stack <std::pair<BT_Node*, int> > path;

        BT(int d) // B-tree constructor
        {   degree = d;
            top_root = NULL;
        }; 

        //~BT(); // Destructor

        void Bottom_up(){

            // search for parent node having old node address pointer 
                // store parent and old node in memory

            // create a new node
            BT_Node* new_node = new BT_Node;

            // new node is a leaf
            if (path.top().first->children.empty()){
                new_node->is_leaf = true;
                path.top().first->is_leaf = true;
            }
            // push last key of old node into new node
            new_node->keys.push_back(path.top().first->keys.back()); new_node->num_keys++;

            // remove that last key from old node (new child key)
            path.top().first->keys.pop_back(); path.top().first->num_keys--; 

            std::pair <std:: string, int>dad_key = path.top().first->keys.back(); // save the parent key from old child

            // remove the parent key from old child
            path.top().first->keys.pop_back(); path.top().first->num_keys--; 

            if (path.size() == 1){
                
                // special case for root node 
                // create parent node as well
                // root points to that parent node 
                top_root = new BT_Node;

                top_root->children.push_back(path.top().first); // new top root point to old child
                
                path.pop(); // remove old child from path
                top_root->children.push_back(new_node); // new top root point to new child  !!!!!!!!!!!!
                top_root->keys.push_back(dad_key); top_root->num_keys++;

                path.push(std::make_pair(top_root, 0)); // add new top root to the path
            }
            else { // remove old node
    
                path.pop(); // remove old child from path
            
                // dad is now at the top of the stack 

                std::list<std::pair<std::string,int> >::iterator it = path.top().first->keys.begin();
                std::advance(it,path.top().second);

                path.top().first->keys.insert(it, dad_key); path.top().first->num_keys++; // insert the parent key from old child 
            
                // add pointer to new node into parent children pointers
                std::list<BT_Node*>::iterator itc = path.top().first->children.begin();
                std::advance(itc,path.top().second+1);
                path.top().first->children.insert(itc, new_node);
            }
            // check if parent has right degree
            if (path.top().first->num_keys > degree)
                Bottom_up();
                // recursive with old node = parent until all ancestors have right degree   
        };

        //insert a key into a node  
        void Insert(std::string k) {

            if (top_root == NULL){ // if our b-tree is empty

                top_root = new BT_Node();  // root point to a new dynamically created node
                root = top_root;
                top_root->keys.push_back(std::make_pair(k, 1));  // we add the key k to the our keys array
                top_root->num_keys = 1;  // update the number of keys
                top_root->is_leaf = true;
            } 
            else {
                int num_it = -1; std::list<std::pair<std::string,int> >::iterator it = root->keys.begin();
                
                for (; it != root->keys.end(); it++){
                    num_it++;
                    if (k.compare(it->first) == 0){
                        it->second++;
                        return;
                    } else if (k.compare(it->first) < 0){
                        break;
                    }
                    else if (num_it == root->num_keys-1) {
                        num_it++;
                    }
                }
                path.push(std::make_pair(root, num_it));
                if (root->is_leaf){
                    if (it == root->keys.end() && k.compare(it->first) > 0 ){
                        root->keys.push_back(std::make_pair(k,1)); root->num_keys++;
                    }
                    else{
                        root->keys.insert(it,std::make_pair(k,1)); root->num_keys++;
                    }

                    if (root->num_keys > degree){
                        Bottom_up();   // does all the spliting etc...
                    }
                }
                else {
                    std::list<BT_Node*>::iterator itc = root->children.begin();
                    std::advance(itc,num_it);
                    root = (*itc);
                    Insert(k);
                }
            }
            root = top_root; // root points back to top of tree cuz insertion done
            while (!path.empty())
                path.pop();
            return;
        };

        void Search(std::string k){
            
            if (top_root != NULL){ 

                int num_it = -1; std::list<std::pair<std::string,int> >::iterator it = root->keys.begin();
                for (; it != root->keys.end(); it++){
                    
                    num_it++;
                    if (k.compare(it->first) == 0){ //
                        std::cout << "The word " << it->first << " is repeated " << it->second << " times";
                        return;
                    } else if (k.compare(it->first) < 0){
                        break;
                    }
                    else if (num_it == root->num_keys-1) {
                        num_it++;
                        break;
                    }
                }
            
                if (root->is_leaf){
                    std::cout << "Word not found";
                    return;
                }
                else {
                    // go to the right child and recurse
                    std::list<BT_Node*>::iterator itc = root->children.begin();
                    std::advance(itc,num_it);
                    root = (*itc);
                    Search(k);
                } 
            } else {std::cout << "Empty tree" << std::endl;}

        }; 
        void print_dot(std::ofstream& of, BT_Node* print){
            std::string str_dad = "";
            std::string str_kid = "";
            if(top_root->is_leaf){
                for(std::list<std::pair<std::string,int> >::iterator it = print->keys.begin(); it != print->keys.end(); it++){
                    str_dad += (*it).first;
                    str_dad += " ";
                }
                of << str_dad;
                std::cout << "baby";
            }
            else if (print != NULL && !print->is_leaf){
                // loop trough the keys of top_root and print them
                for(std::list<std::pair<std::string,int> >::iterator it = print->keys.begin(); it != print->keys.end(); it++){
                    str_dad += (*it).first;
                    str_dad += " ";
                }
                std::cout << "baby";
            }
            if(!print->is_leaf){
                std::cout << "babyyyy";
                for(std::list<BT_Node*>::iterator itc = print->children.begin(); itc != print->children.end(); itc++){
                    for(std::list<std::pair<std::string,int> >::iterator it = (*itc)->keys.begin(); it != (*itc)->keys.end(); it++){
                        str_kid += (*it).first;
                        str_kid += " ";                         
                    }
                    of << str_dad << "-> " << str_kid << "; \n"; str_kid = "";
                    print_dot(of, *itc); // kid becomes dad
                }             
            } 
        }
};

int main(){
    BT tree(2);
    tree.Insert("yann");
    tree.Insert("patricia");
    tree.Insert("ariel");
    tree.Insert("justin");
    tree.Insert("maman");
    tree.Insert("linda");

    tree.Search("yann");


    BT_Node* print = tree.top_root;

    std::ofstream of("btree_default.dot");
    of << "diagram G {\n" << std::endl;

    tree.print_dot(of,print);

    of << "\n}";
    return 0;

}