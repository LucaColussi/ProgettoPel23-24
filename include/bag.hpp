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
    bag<trie<T>>(){  // default constructor
    }

    ~bag<trie<T>>(){ // destructor
        for(auto elem : this->children){
            delete elem;
        }
        this->children.clear();
    }

    bag<trie<T>>(bag<trie<T>> const& other){ // copy constructor
        for(int i = 0 ; i < other.children.size(); i++){
            trie<T> * add = new trie<T>(*other.children[i]);
            this->children.push_back(add);
        }
    }
    
    bag<trie<T>>(bag<trie<T>>&& other){ // move constructor
        for(int i = 0; i < other.children.size(); i++){
            this->children.push_back(other.children[i]);
        }
        other.children.clear();
    }

    bag<trie<T>>& operator=(bag<trie<T>>&& other){  //move assignment
        for(int i = 0; i < this->children.size(); i++){
            delete this->children[i];
        }
        this->children.clear();
        for(int i = 0 ; i < other.children.size(); i++){
            this->children.push_back(other.children[i]);
        }
        other.children.clear();
        return *this;
    }

    bag<trie<T>>& operator=(bag<trie<T>> const& other) { // copy assignment
        for(int i = 0; i < this->children.size(); i++){
            delete this->children[i];
        }
        this->children.clear();
        for(int i = 0 ; i < other.children.size(); i++){
            trie<T> * add = new trie<T>(*other.children[i]);
            this->children.push_back(add);
        }
        return *this;
    }


    int getSize()const{
        return this->children.size();
    }
    void addChild(trie<T> const& c){
        trie<T> * child = new trie<T>(c); // copy constructor, essendo che gli passo la c per valore
        if(children.size() == 0){
            this->children.push_back(child);
        }else{
            for(int i = 0; i < children.size(); i++){
                if(*child->get_label() < *children[i]->get_label()){
                    this->children.insert(this->children.begin() + i, child);
                    return;
                }
            }
            this->children.push_back(child);
        }
    }

    void setAllParent(trie<T> * parent){
        for(int i = 0; i < children.size(); i++){
            this->children[i]->set_parent(parent);
        }
    }
    
private:
    std::vector<trie<T>*> children; 
};