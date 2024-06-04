/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */
using namespace std;
template <typename T>
struct bag{
public:
    int getSize(){
        return this->children.size();
    }
    void addChild(trie<T> const& c){
        trie<T> child(c); // copy constructor, essendo che gli passo la c per valore
        this->children.push_back(child);
    }
    
private:
    std::vector<trie<T>> children; 
};