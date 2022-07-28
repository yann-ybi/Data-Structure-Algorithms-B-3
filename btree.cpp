#include "btree.h"

// node constructor
BT_Node::BT_Node(){
    is_leaf = false;
    num_keys = 0;
} 

// B-tree constructor
BT::BT(int d) 
{   degree = d;
    top_root = NULL;
}

void BT::bottom_up(){

    // create a new node
    BT_Node* new_node = new BT_Node;

    // if the node on top of the stack of the path made during insertion has no children (is a leaf)
    if (path.top().first->children.empty()){
        new_node->is_leaf = true; // new node is a leaf
        path.top().first->is_leaf = true; // make sure that childless node is set as a leaf 
    }

    // push last key of old node into new node
    new_node->keys.push_back(path.top().first->keys.back()); new_node->num_keys++;
    
    // if the node on top of the stack of the path made during insertion is not a leaf
    if (!path.top().first->is_leaf){
        new_node->children.push_front(path.top().first->children.back()); // give new node the upright child of his brother
        path.top().first->children.pop_back(); // remove the children from the brother of the new node
        new_node->children.push_front(path.top().first->children.back()); // give new node the upright of his brother
        path.top().first->children.pop_back();  // remove the children from the brother of the new node
    }

    // remove that last key from old node that was given to new node his brother
    path.top().first->keys.pop_back(); path.top().first->num_keys--; 

    std::pair <std:: string, int>dad_key = path.top().first->keys.back(); // save the parent key from old child

    // remove the parent key from old child
    path.top().first->keys.pop_back(); path.top().first->num_keys--; 

    if (path.size() == 1){  // special case for top root node split  
    
        // we create parent node as well 
        top_root = new BT_Node;
       
        top_root->children.push_back(path.top().first); // new top root have the old split node as child
        
        path.pop(); // remove old split node from path
        top_root->children.push_back(new_node); // new top root point to new node (brother of old node)  !
        top_root->keys.push_back(dad_key); top_root->num_keys++;  // dad key go to the parent node and we increment the number of keys it has

        path.push(std::make_pair(top_root, 0)); // add that top root parent to the path
    }
    else {

        path.pop(); // remove old child from path   // dad is now at the top of the stack 

        // we get to the saved position where the key from the child going to the parent should go 
        // which is the same position as the position where the inserted key could've gone 
        std::list<std::pair<std::string,int> >::iterator it = path.top().first->keys.begin();
        std::advance(it,path.top().second);

        path.top().first->keys.insert(it, dad_key); path.top().first->num_keys++; // insert the parent key from old child 
    
        // add memory addreess of new node into parent children pointers
        std::list<BT_Node*>::iterator itc = path.top().first->children.begin();
        std::advance(itc,path.top().second+1);
        path.top().first->children.insert(itc, new_node);
    }

    // if parent doesn;t have the right degree
    if (path.top().first->num_keys > degree)
        bottom_up(); // recursively call the function with parent until all ancestors have right degree   
}

//insert a key into a node  
void BT::insert(std::string k) {

    if (top_root == NULL){ // if our b-tree is empty

        top_root = new BT_Node();  // root point to a new dynamically created node
        root = top_root; // node tracer starts at root
        top_root->keys.push_back(std::make_pair(k,1));  // we add the key k to the our keys array
        top_root->num_keys = 1;  // update the number of keys
        top_root->is_leaf = true; // iset leaf as true
    } 
    else {
        // iterate through the keys of the node while saving its index with num_it
        int num_it = -1; std::list<std::pair<std::string,int> >::iterator it = root->keys.begin();
        for (; it != root->keys.end(); it++){
            num_it++;
            if (k.compare(it->first) == 0){ // if k to insert is the same as a key word of a node
                it->second++; // increment the number of that key repeat counter 
                return; // exit from the insertion funtion
            } 
            else if (k.compare(it->first) < 0){ // else if k is less than a key, k position is the index of that key 
                break; // so we break out of the loop
            }
            else if (num_it == root->num_keys-1) { // if we reach the last key of the node and k without breaking
                // k postion is the index after the last key 
                num_it++; // so we increment the index
            }
        }
        // push the node we've iterated through and the position of k inside of that node in out path stack for out bottom_up function
        path.push(std::make_pair(root, num_it));

        // if that node is a leaf
        if (root->is_leaf){
            if (it == root->keys.end() && k.compare(it->first) > 0 ){ // if its position is after the last key of the node
                // we pushback k after that last key with its repeat counter at 1
                root->keys.push_back(std::make_pair(k,1)); root->num_keys++; // increment the number of keys counter of that node
            }
            else{ // otherwise we just insert it at key iterator position where we broke out of the loop
                root->keys.insert(it,std::make_pair(k,1)); root->num_keys++;
            }

            if (root->num_keys > degree){ // after insertion if the number of keys is too high
                bottom_up();   // we call our tree self balancing function 
            }
        }
        else { // if the node isn't a leaf
            // we get to the right child for k based on the position of the k in the node keys num_it
            std::list<BT_Node*>::iterator itc = root->children.begin();  
            std::advance(itc,num_it);
            // the root becomes that child
            root = (*itc);
            insert(k); //  and we recursively call our insertion function
        }
    }
    root = top_root; // root points back to top of tree for future insertions
    while (!path.empty()) // we make sure to empty our path stack for future insertions
        path.pop();
    return;
}

void BT::search(std::string k){
    
    if (top_root != NULL){  // if the tree is not empty

        // iterate through the keys of the node while storing its index with num_it
        int num_it = -1; std::list<std::pair<std::string,int> >::iterator it = root->keys.begin(); 
        for (; it != root->keys.end(); it++){
            num_it++;
            if (k.compare(it->first) == 0){ // if we found the key 
                // we print that it was found and the number of times it was found
                std::cout << "The word " << it->first << " is repeated " << it->second << " times" << std::endl; 
                return; // exit from the search funtion
            } else if (k.compare(it->first) < 0){ // else if k is less than a key, k position is the index of that key 
                break; // we stop the iteration
            }
            else if (num_it == root->num_keys-1) { // if we reach the last key of the node and k without breaking
                // k postion is the index after the last key 
                num_it++; // increment the index
            }
        }
    
        if (root->is_leaf){ // if we reach the leaf without finding the key
            std::cout << "The word " << k << " is not found" << std::endl; // print the key was not found
            return; // exit from the function
        }
        else { // if no we go to the right child based of num_it and we call the search recursively
            std::list<BT_Node*>::iterator itc = root->children.begin();
            std::advance(itc,num_it);
            root = (*itc); // the root becomes that right child
            search(k);
        } 
    } else {std::cout << "The tree is empty tree, check your list of word's file" << std::endl;} // if tree is empty we print the tree is empty
    root = top_root; // me make sure root tracer points back to the top_root for future use
}

void BT::print_dot(std::ofstream& of, BT_Node* print){
    
    std::string str_dad = "\""; // string to concatenate all the keys of the dad keys 
    std::string str_kid = "\""; // string to concatenate all the keys of a child keys

    // if the top_root is a leaf then there is only one node in our tree
    if(top_root->is_leaf){
        // iterate through the node's keys
        for(std::list<std::pair<std::string,int> >::iterator it = print->keys.begin(); it != print->keys.end(); it++){
            // at each iteration
            str_dad += (*it).first; // we add dad's key word into the string
            str_dad += ":"; // we add a column
            str_dad += std::to_string((*it).second); // we add the number of times the word was repeated
            str_dad += " "; // we add some space for the next key of the node
        }
        of << str_dad + "\""; // we print the string with all its keys in our output file
    }
    else if (print != NULL && !print->is_leaf){ // if there is a node and the node is not a leaf
        // iterate through the node's keys
        for(std::list<std::pair<std::string,int> >::iterator it = print->keys.begin(); it != print->keys.end(); it++){
            str_dad += (*it).first; // we add dad's key word into the string
            str_dad += ":"; // we add a column
            str_dad += std::to_string((*it).second); // we add the number of times the word was repeated
            str_dad += " "; // we add some space for the next key fo the node
        }
    }
    if(!print->is_leaf){ // if the node is not a leaf
        // we iterate through the node's children 
        for(std::list<BT_Node*>::iterator itc = print->children.begin(); itc != print->children.end(); itc++){
            // for each child we loop through its keys we iterate through all its keys
            for(std::list<std::pair<std::string,int> >::iterator it = (*itc)->keys.begin(); it != (*itc)->keys.end(); it++){
                str_kid += (*it).first; // we add child's key word into the string
                str_kid += ":"; // we add a column
                str_kid += std::to_string((*it).second); // we add the number of times the word was repeated
                str_kid += " ";   // we add some space for the next key fo the node                     
            }
            // for each child of the parent we print the parent followed by its child
            of << str_dad << "\"" << "->" << str_kid << "\"" << "; \n"; 
            str_kid = "\""; // we make sure to empty the child string for the next child
            print_dot(of, *itc); // kid becomes dad
        }             
    } 
}
