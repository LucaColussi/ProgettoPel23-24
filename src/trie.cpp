#include "../include/trie.hpp"  

template <typename T>
void trie<T>::set_weight(double w) {
    if(this->m_c.getSize() == 0){
      this->m_w = w;  
    }
    else{
        throw parser_exception("Can't set weight cause not a leaf")
    }
}
template <typename T>
double trie<T>::get_weight() const{
    if(this->m_c.getSize() == 0){
        return this->m_w;
    }else{
        return -1; // se non è una foglia quindi nel caso non abbia children
    }
}
// trie<char> * node = new trie<char>;
// double res = node->get_weight();

template <typename T>
void trie<T>::set_label(T* l) {
    if(this->m_p != nullptr){
        this->m_l = l;
    }
    else{
        throw parser_exception("Can't set label cause hasn't a parent")
    }
}
template <typename T>
T const* trie<T>::get_label() const{
        return this->m_l; // nessun if perchè nel momento che anche non avesse un parent
}                         // returna nullptr
// const char * res = node->get_label();

template <typename T>
T* trie<T>::get_label(){
    return this->m_l;
}

template <typename T>
void trie<T>::set_parent(trie<T>* p){
    this->m_p = p;
}

template <typename T>
trie<T> const* trie<T>::get_parent() const{
    return this->m_p;
}

template <typename T>
trie<T>* trie<T>::get_parent(){
    return this->m_p;
}

template <typename T>
void trie<T>::add_child(trie<T> const& c){
    this->m_c.
}


