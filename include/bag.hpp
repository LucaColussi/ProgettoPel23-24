/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */
using namespace std;
template <typename T>
struct trie;

template <typename T>
struct bag;
template <typename T>
struct bag<trie<T>>{

public:
    int getSize()const{
        return this->children.size();
    }
    void addChild(trie<T> const& c){
        trie<T> child(c); // copy constructor, essendo che gli passo la c per valore
        if(children.size() == 0){
            this->children.push_back(child);
        }else{
            for(int i = 0; i < children.size(); i++){
                if(*child.get_label() < *children[i].get_label()){
                    this->children.insert(this->children.begin() + i, child);
                    return;
                }
            }
            this->children.push_back(child);
        }
    }
    
private:
    std::vector<trie<T>> children; 
};