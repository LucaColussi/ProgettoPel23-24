#include "../include/trie.hpp"  

    template <typename T>
    trie<T>::trie(){ // costruttore
        this->m_p = nullptr;
        this->m_l = nullptr;
        this->m_w = 0.0;
   }

    template <typename T>
    trie<T>::trie(double w){ // costruttore con peso
        this->m_p = nullptr;
        this->m_l = nullptr;
        this->m_w = w;
    }

    template <typename T>
    trie<T>::trie(trie<T> const& c){ //copy constructor
        //lui fa una trie<T> che chiama this e 
            this->m_p = c.m_p;
            T* label = new T(*c.m_l);
            this->m_l = label;
            this->m_c = c.m_c;
            this->m_c.setAllParent(this);
            this->m_w = c.m_w;
        //infine ritorna this   
    }

    template <typename T>
    trie<T>::trie(trie<T>&& m){ // move constructor
        //  forse devo fare move(m.m_c) e poi definire il move constructor di bag;
        this->m_c = move(m.m_c);
        this->m_l = m.m_l;
        this->m_p = m.m_p;
        this->m_w = m.m_w;
        m.m_l = nullptr;
        m.m_p = nullptr;
    }

    template <typename T>
    trie<T>::~trie(){   // distruttore
        if(this->m_l != nullptr){
            delete this->m_l;
            m_l = nullptr;
        }
        this->m_w = 0;
        this->m_p = nullptr;
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
    bag<trie<T>>& trie<T>::get_children(){
        if(this->m_c.getSize() == 0){
            throw parser_exception("Can't get children cause it's a leaf");
        } else{
            return this->m_c;
        }
    }

    template <typename T>
    bag<trie<T>> const& trie<T>::get_children() const{
        if(this->m_c.getSize() == 0){
            throw parser_exception("Can't get children cause it's a leaf");
        } else{
            return this->m_c;
        }
    }

