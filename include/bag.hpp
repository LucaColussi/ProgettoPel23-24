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
    
private:
    std::vector<trie<T>> children; 
};