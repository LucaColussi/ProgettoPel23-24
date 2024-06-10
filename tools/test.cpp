#include "../src/trie.cpp"  
using namespace std;
#include <iostream>
#include <cassert>

void testAddChild(){
    try
    {
        trie<char> * node = new trie<char>;
        trie<char> * child = new trie<char>;
        child->set_parent(node);
        child->set_weight(3.1);
        char * label = new char('a');
        child->set_label(label);
        node->add_child(*child);
        node->get_children();
        assert(node->get_weight() == 0);
    }
    catch(const parser_exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

    void testCopyConstructor(){
        trie<char> root; 
        trie<char> firstChild(3.1);
        firstChild.set_parent(&root);
        char * label = new char('a');
        firstChild.set_label(label);

        trie<char> secondChild;
        secondChild.set_parent(&root);
        label = new char('b');
        secondChild.set_label(label);

        trie<char> thirdChild(2.9);
        thirdChild.set_parent(&secondChild);
        label = new char('c');
        thirdChild.set_label(label);

        secondChild.add_child(thirdChild);
        root.add_child(firstChild);
        root.add_child(secondChild);
    }

int main(){
    //testAddChild();
    testCopyConstructor();
    return 0;
}

/*
children = { //root
    a 3.1 children = {},    // firstChild
    b children = {          //secondChild
        c 2.9 children = {}     //thirdChild
  }
} 
*/