#include "../include/trie.hpp"  

template <typename T>
   trie<T>::trie(){ // costruttore
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = 0.0;
   }

   template <typename T>
   trie<T>::trie(double w){
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = w;
   }



template <typename T>
void trie<T>::set_weight(double w) {
    if(this->m_c.getSize() == 0){
      this->m_w = w;  
    }
    else{
        throw parser_exception("Can't set weight cause not a leaf");
    }
}
template <typename T>
double trie<T>::get_weight() const{
    if(this->m_c.getSize() == 0){
        return this->m_w;
    }else{
        return 0; // se non è una foglia quindi nel caso abbia children
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
        throw parser_exception("Can't set label cause hasn't a parent");
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
    this->m_w = 0;
    if(c.get_label() == nullptr){
        throw parser_exception("Can't add child cause it hasn't a label");
    }
    if(c.get_parent() == nullptr){
        throw parser_exception("Can't add child cause hasn't used set_parent");
    }
    this->m_c.addChild(c);
}

template <typename T>
trie<T>::trie(trie<T> const& c){ //copy constructor
    //lui fa una trie<T> che chiama this e 
        this->m_p = c.m_p;
        T* label = new T(*c.m_l);
        this->m_l = label;
        this->m_c = c.m_c;
        this->m_w = c.m_w;
    //infine ritorna this   
}

//nodoPadre->add_child(nodoFiglio);
