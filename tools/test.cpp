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
        assert(node->get_weight() == 0);
    }
    catch(const parser_exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(){
    testAddChild();
    return 0;
}
