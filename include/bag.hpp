/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */
#include <vector>
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
        for(int i = 0 ; i < (int)other.children.size(); i++){
            trie<T> * add = new trie<T>(*other.children[i]);
            this->children.push_back(add);
        }
    }
    
    bag<trie<T>>(bag<trie<T>>&& other){ // move constructor
        for(int i = 0; i < (int)other.children.size(); i++){
            this->children.push_back(other.children[i]);
        }
        other.children.clear();
    }

    bag<trie<T>>& operator=(bag<trie<T>>&& other){  //move assignment
        for(int i = 0; i < (int)this->children.size(); i++){
            delete this->children[i];
        }
        this->children.clear();
        for(int i = 0 ; i < (int)other.children.size(); i++){
            this->children.push_back(other.children[i]);
        }
        other.children.clear();
        return *this;
    }

    bag<trie<T>>& operator=(bag<trie<T>> const& other) { // copy assignment
        for(int i = 0; i < (int)this->children.size(); i++){
            delete this->children[i];
        }
        this->children.clear();
        for(int i = 0 ; i < (int)other.children.size(); i++){
            trie<T> * add = new trie<T>(*other.children[i]);
            this->children.push_back(add);
        }
        return *this;
    }

    bool operator==(bag<trie<T>> const& other) const {
        if(other.getSize() != this->getSize()){
            return false;
        }
        for(int i = 0; i < (int)this->children.size(); i++){
            if(this->children[i]->get_label() != nullptr && other.children[i]->get_label() != nullptr){ // possibilità di aggiungere piu casistiche nel caso andassero bene le label nullptr, in questo caso anche se entrambe nullptr quindi uguali da FALSE
                if(abs(this->children[i]->get_weight() - other.children[i]->get_weight()) > 1e-6 || *this->children[i]->get_label() != *other.children[i]->get_label() || this->children[i]->get_children() != other.children[i]->get_children()){
                    return false;
                }
            }
            else{
                return false;
            }    
        }
        return true;
    }

    bool operator!=(bag<trie<T>> const& other) const {
        return !(*this == other);
    }

    int getSize()const{     //get size of the children vector
        return this->children.size();
    }

    bool hasLabel(const T * label){      //find if label already present
        for(int i = 0; i < (int)this->children.size(); i++){
            if(*this->children[i]->get_label() == *label){
                return true;
            }
        }
        return false;
    }

    trie<T> * getWithLabel(const T label) const {       //return trie whit same label
        for(int i = 0; i < (int)this->children.size(); i++){
            if(*this->children[i]->get_label() == label){
                return this->children[i];
            }
        }
        return nullptr;
    }

    trie<T> * get(int i) const{
        return children[i];
    }

    int findIndex(trie<T> const * node) const {      //find at what index of the vector the child is
        for(int i = 0; i < (int)this->getSize(); i++){
            if(this->children[i] == node){
                return i;
            }
        }
    return -1;
    }

    void addChild(trie<T> const& c){ //add child al vector
        trie<T> * child = new trie<T>(c); // copy constructor, essendo che gli passo la c per valore
        if(children.size() == 0){
            this->children.push_back(child);
        }
        else{
            for(int i = 0; i < (int)children.size(); i++){
                if(*child->get_label() < *children[i]->get_label()){
                    this->children.insert(this->children.begin() + i, child);
                    return;
                }
            }
            this->children.push_back(child);
        }
    }

    void setAllParent(trie<T> * parent){
        for(int i = 0; i < (int)children.size(); i++){
            this->children[i]->set_parent(parent);
        }
    }
    
void reorder() {
    for (size_t i = 0; i < children.size() - 1; ++i) {
        for (size_t j = 0; j < children.size() - 1 - i; ++j) {
            if (*children[j]->get_label() > *children[j + 1]->get_label()) {
                trie<T>* temp = children[j];
                children[j] = children[j + 1];
                children[j + 1] = temp;
            }
        }
    }
}


    
    struct children_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = trie<T>;
        using pointer = trie<T>*;
        using reference = trie<T>&;

        children_iterator(trie<T>* tmp, vector<trie<T>*> childrenVec){
            this->m_ptr = tmp;
            this->children = childrenVec;
        }
        reference operator*() const{
            return *this->m_ptr;
        }
        pointer operator->() const{
            return this->m_ptr;
        }
        children_iterator& operator++(){
            for(int i = 0; i < (int)children.size(); i++){
                if(children[i] == this->m_ptr){
                    if(i != (int)children.size() -1){
                        this->m_ptr = children[i+1];
                        break;
                    } else{
                        this->m_ptr = nullptr;
                        break;
                    }
                } 
            }
            return *this;
        }
        children_iterator operator++(int){
            children_iterator tmp = *this;
            for(int i = 0; i < (int)children.size(); i++){
                if(children[i] == this->m_ptr){
                    if(i != (int)children.size() -1){
                        this->m_ptr = children[i+1];
                        break;
                    } else{
                        this->m_ptr = nullptr;
                        break;
                    }
                } 
            }
            return tmp;
        }
        bool operator==(children_iterator const& other) const{
            if(this->m_ptr == other.m_ptr){
                return true;
            }
            return false;
        }
        bool operator!=(children_iterator const& other) const{
            return !(*this == other);
        }

    private:
        trie<T>* m_ptr;
        vector<trie<T>*> children; 

    };

        struct const_children_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = const trie<T>;
        using pointer = const trie<T>*;
        using reference = const trie<T>&;

        const_children_iterator(trie<T>* const tmp, vector<trie<T>*> childrenVec){
            this->m_ptr = tmp;
            this->children = childrenVec;
        }
        reference operator*() const{
            return *this->m_ptr;
        }
        pointer operator->() const{
            return this->m_ptr;
        }
        const_children_iterator& operator++(){
            for(int i = 0; i < (int)children.size(); i++){
                if(children[i] == this->m_ptr){
                    if(i != (int)children.size() -1){
                        this->m_ptr = children[i+1];
                        break;
                    } else{
                        this->m_ptr = nullptr;
                        break;
                    }
                } 
            }
            return *this;
        }
        const_children_iterator operator++(int){
            const_children_iterator tmp = *this;
            for(int i = 0; i < (int)children.size(); i++){
                if(children[i] == this->m_ptr){
                    if(i != (int)children.size() -1){
                        this->m_ptr = children[i+1];
                        break;
                    } else{
                        this->m_ptr = nullptr;
                        break;
                    }
                } 
            }
            return tmp;
        }
        bool operator==(const_children_iterator const& other) const{
            if(this->m_ptr == other.m_ptr){
                return true;
            }
            return false;
        }
        bool operator!=(const_children_iterator const& other) const{
            return !(*this == other);
        }

    private:
        const trie<T>* m_ptr;
        vector<trie<T>*> children; 

    };

    children_iterator begin(){
        if(this->children.size() != 0){
            return children_iterator(this->children[0], this->children);
        }
        else{
            return children_iterator(nullptr, this->children);
        }
    }
    children_iterator end(){
        children_iterator ret(nullptr, this->children);
        return ret;
    }

    const_children_iterator begin() const {
        if(this->children.size() != 0){
            return const_children_iterator(this->children[0], this->children);
        }
        else{
            return const_children_iterator(nullptr, this->children);
        }
    }
    const_children_iterator end() const {
        const_children_iterator ret(nullptr, this->children);
        return ret;
    }
private:
    vector<trie<T>*> children; 
};