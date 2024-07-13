#include "../include/trie.hpp"  
//#include "trie.hpp"

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
        T * label;
        if(c.m_l == nullptr) {
            label = nullptr;
        } else{
            label = new T(*c.m_l);
        }
        this->m_l = label;
        this->m_c = c.m_c;
        this->m_c.setAllParent(this);
        this->m_w = c.m_w;
        //infine ritorna this   
    }

    template <typename T>
    trie<T>::trie(trie<T>&& m){ // move constructor
        //  forse devo fare move(m.m_c) e poi definire il move constructor di bag;
        this->m_c = std::move(m.m_c);
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
        if(this->m_l != nullptr){
            delete this->m_l;
        }
        T * label;
        if(l == nullptr) {
            label = nullptr;
        } else{
            label = new T(*l);
        }
        this->m_l = label;

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
        if(this->m_c.hasLabel(c.get_label())){
            throw parser_exception("Can't add child cause is already present an equal label");
        }
        this->m_c.addChild(c);
    }

    template <typename T>
    bag<trie<T>>& trie<T>::get_children(){
            return this->m_c;
    }

    template <typename T>
    bag<trie<T>> const& trie<T>::get_children() const{
            return this->m_c;
    }

    template <typename T>
    trie<T>& trie<T>::operator=(trie<T> const& other) { // copy assignment
        this->m_c = other.m_c;
        this->m_c.setAllParent(this);
        this->m_w = other.m_w;
        return *this;
    }

    template <typename T>
    trie<T>& trie<T>::operator=(trie<T>&& other) { // move assignment
        this->m_c = std::move(other.m_c);
        this->m_c.setAllParent(this);
        this->m_w = other.m_w;
        return *this;   
    }

    template <typename T>
    bool trie<T>::operator==(trie<T> const& other) const {
        if(this->m_c == other.m_c && this->m_w == other.m_w){
            return true;
        }
        return false;
    }

    template <typename T>
    bool trie<T>::operator!=(trie<T> const& other) const {
        return !(*this == other);
    }

    string getNextWord(std::istream& stream, char limit){ 
        char c;
        string result;
        do{
            if(stream.eof() || stream.peek() == EOF){
                throw parser_exception("unexpected end of file");
            }
            stream>>c;  //mette nel chaar carattere corrente e va avanti di uno
            result.push_back(c);
        }while(stream.peek() != ' ' && stream.peek() != '\t' && stream.peek() != '\n' && stream.peek() != limit);

        return result;
    }

    template <typename T>
    void R(std::istream& stream, trie<T>& t){
        T label;
        stream>>label;
        if(stream.fail()) throw parser_exception("expected label, got something else");
        trie<T> node;
        node.set_parent(&t);
        node.set_label(&label);
        
        S(stream, node);

        t.add_child(node);
        char c;
        stream>>c;
        if(c == ','){
            R(stream, t);
        }
        else{
            stream.unget();
        }
    }

    template <typename T>
    void F(std::istream& stream, trie<T>& t){
        if(getNextWord(stream, '{') != "=" ){
            throw parser_exception("excepted '=' after children keyword, got something else");
        }
        char c;
        stream>>c;
        if(c != '{'){
            throw parser_exception("excepted '{' after '=', got something else");
        }
        R(stream,t);

        stream>>c;
        if(c != '}'){
            throw parser_exception("excepted '}' after '{', got something else");
        }
    }

    template <typename T>
    void H(std::istream& stream, trie<T>& t){
        if(getNextWord(stream, '=') != "children" ){
            throw parser_exception("excepted children keyword after weigth, got something else");
        }
        if(getNextWord(stream, '{') != "=" ){
            throw parser_exception("excepted '=' after children keyword, got something else");
        }
        char c;
        stream>>c;
        if(c != '{'){
            throw parser_exception("excepted '{' after '=', got something else");
        }
        stream>>c;
        if(c != '}'){
            throw parser_exception("excepted '}' after '{', got something else");
        }
    }


    template <typename T>
    void S(std::istream& stream, trie<T>& t){
    
    string word = getNextWord(stream, '=');
        if(word == "children"){
            F(stream, t);
        }
        else{
            try{
                double num = stod(word); // se non funziona throwa stod un exception
                t.set_weight(num);
            }
             catch (std::invalid_argument&) {
                // Se std::stod non riesce a convertire la stringa
                throw parser_exception("expected a double as first argument, got something else");
            }

            H(stream, t);
        }
    }


    template <typename T>
    std::istream& operator>>(std::istream& stream, trie<T>& t){

        trie<T> tr;     // mi assicuro che le trie che mi passa sia vuota
        S(stream, tr);
        t = tr;
        return stream;
    }

    template <typename T>
    trie<T>& trie<T>::operator[](std::vector<T> const& vec){
        trie<T> * res = this;
        for(int i = 0; i < (int)vec.size(); i++){
            if(res->m_c.getWithLabel(vec[i]) != nullptr){
                res = res->m_c.getWithLabel(vec[i]);
            }
        }
        return *res;
    }

    template <typename T>
    trie<T> const& trie<T>::operator[](std::vector<T> const& vec) const {
        if(vec.size() == 0) return *this;
        
        const trie<T> * res = this;     //mi faccio una copia del nodo da scorrere presente in this
        for(int i = 0; i < (int)vec.size(); i++){
            if(res->m_c.getWithLabel(vec[i]) != nullptr){   //penso io debba controllare che res abbia figli 
                res = res->m_c.getWithLabel(vec[i]);
            }
        }
        return *res;
    }

    /* node iterators */
    template <typename T>
    trie<T>::node_iterator::node_iterator(trie<T>* start){
        this->m_ptr = start;
    }
    template <typename T>
    typename trie<T>::node_iterator::reference trie<T>::node_iterator::operator*() const{
        return *this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::node_iterator::pointer trie<T>::node_iterator::operator->() const{
        return this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::node_iterator& trie<T>::node_iterator::operator++(){
        this->m_ptr = this->m_ptr->m_p;
        return *this;
    }

    template <typename T>
    typename trie<T>::node_iterator trie<T>::node_iterator::operator++(int){
        node_iterator tmp = *this;
        this->m_ptr = this->m_ptr->m_p;
        return tmp;
    }

    template <typename T>
    bool trie<T>::node_iterator::operator==(node_iterator const& other) const {
        return (this->m_ptr == other.m_ptr);      
    }

    template <typename T>
    bool trie<T>::node_iterator::operator!=(node_iterator const& other) const {
        return !(*this == other);
    }


    /*const node iterators */
    template <typename T>
    trie<T>::const_node_iterator::const_node_iterator(trie<T> const* ptr){
        this->m_ptr = ptr;
    }
    template <typename T>
    typename trie<T>::const_node_iterator::reference trie<T>::const_node_iterator::operator*() const{
        return *this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::const_node_iterator::pointer trie<T>::const_node_iterator::operator->() const{
        return this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::const_node_iterator& trie<T>::const_node_iterator::operator++(){
        this->m_ptr = this->m_ptr->m_p;
        return *this;
    }

    template <typename T>
    typename trie<T>::const_node_iterator trie<T>::const_node_iterator::operator++(int){
        const_node_iterator tmp = *this;
        this->m_ptr = this->m_ptr->m_p;
        return tmp;
    }

    template <typename T>
    bool trie<T>::const_node_iterator::operator==(const_node_iterator const& other) const {
        return (this->m_ptr == other.m_ptr);      
    }

    template <typename T>
    bool trie<T>::const_node_iterator::operator!=(const_node_iterator const& other) const {
        return !(*this == other);
    }

    /* leaf iterators */ 
    template <typename T>
    trie<T> * findLeaf(trie<T> * start){
        if(start == nullptr) return nullptr;
        while(start->get_children().getSize() != 0){
            start = start->get_children().get(0);
        }
        return start;
    }

    template <typename T>
    trie<T> * findNextLeaf(trie<T> * node){
        if(node->get_parent() == nullptr) return nullptr;
        int i = node->get_parent()->get_children().findIndex(node);

        i++;    // se non ha fratelli a destra
        if(i > node->get_parent()->get_children().getSize() - 1){       //size = 2 - 1   i = 1
            return findNextLeaf(node->get_parent());
        }
        //gli passo il fratello a destra
        return findLeaf(node->get_parent()->get_children().get(i)); 
    }

    template <typename T>
    trie<T>::leaf_iterator::leaf_iterator(trie<T> * start){
        this->m_ptr = findLeaf(start);
    }

    template <typename T>
    typename trie<T>::leaf_iterator::reference trie<T>::leaf_iterator::operator*() const{
        return *this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::leaf_iterator::pointer trie<T>::leaf_iterator::operator->() const{
        return this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::leaf_iterator& trie<T>::leaf_iterator::operator++(){
        this->m_ptr = findNextLeaf(this->m_ptr);
        return *this;
    }

    template <typename T>
    typename trie<T>::leaf_iterator trie<T>::leaf_iterator::operator++(int){
        leaf_iterator tmp = *this;
        this->m_ptr = findNextLeaf(this->m_ptr);
        return tmp;
    }

    template <typename T>
    bool trie<T>::leaf_iterator::operator==(leaf_iterator const& other) const{
        return this->m_ptr == other.m_ptr;
    }

    template <typename T>
    bool trie<T>::leaf_iterator::operator!=(leaf_iterator const& other) const{
        return !(*this == other);
    }
    
    template <typename T>
   trie<T>::leaf_iterator::operator trie<T>::node_iterator() const{
        return node_iterator(this->m_ptr);
    }
    
    template <typename T>
    trie<T>& trie<T>::leaf_iterator::get_leaf() const{
        return *this->m_ptr;
    }


    /* leaf const iterators */ 
    template <typename T>
    trie<T> const * findLeaf(trie<T> const * start){
        if(start == nullptr) return nullptr;
        while(start->get_children().getSize() != 0){
            start = start->get_children().get(0);
        }
        return start;
    }

    template <typename T>
    trie<T> const * findNextLeaf(trie<T> const * node){
        if(node->get_parent() == nullptr) return nullptr;
        int i = node->get_parent()->get_children().findIndex(node);

        i++;
        if(i > node->get_parent()->get_children().getSize() - 1){       //size = 2 - 1   i = 1
            return findNextLeaf(node->get_parent());
        }
        
        return findLeaf(node->get_parent()->get_children().get(i));
    }

    template <typename T>
    trie<T>::const_leaf_iterator::const_leaf_iterator(trie<T> const * ptr){
        this->m_ptr = findLeaf(ptr);
    }

    template <typename T>
    typename trie<T>::const_leaf_iterator::reference trie<T>::const_leaf_iterator::operator*() const{
        return *this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::const_leaf_iterator::pointer trie<T>::const_leaf_iterator::operator->() const{
        return this->m_ptr->m_l;
    }

    template <typename T>
    typename trie<T>::const_leaf_iterator& trie<T>::const_leaf_iterator::operator++(){
        this->m_ptr = findNextLeaf(this->m_ptr);
        return *this;
    }

    template <typename T>
    typename trie<T>::const_leaf_iterator trie<T>::const_leaf_iterator::operator++(int){
        const_leaf_iterator tmp = *this;
        this->m_ptr = findNextLeaf(this->m_ptr);
        return tmp;
    }

    template <typename T>
    bool trie<T>::const_leaf_iterator::operator==(const_leaf_iterator const& other) const{
        return this->m_ptr == other.m_ptr;
    }

    template <typename T>
    bool trie<T>::const_leaf_iterator::operator!=(const_leaf_iterator const& other) const{
        return !(*this == other);
    }
    
    template <typename T>
   trie<T>::const_leaf_iterator::operator trie<T>::const_node_iterator() const{
        return const_node_iterator(this->m_ptr);
    }
    
    template <typename T>
    trie<T> const& trie<T>::const_leaf_iterator::get_leaf() const{
        return *this->m_ptr;
    }


    template <typename T>   
    typename trie<T>::leaf_iterator trie<T>::begin(){
        return leaf_iterator(this);
    }

    template <typename T>   
    typename trie<T>::const_leaf_iterator trie<T>::begin() const{
        return const_leaf_iterator(this);
    }

    template <typename T>   
    typename trie<T>::node_iterator trie<T>::root(){
        trie<T> * tmp = this;
        while(tmp->get_parent() != nullptr){
            tmp = tmp->get_parent();
        }
        node_iterator ret(tmp);
        return ret;
    }

    template <typename T>   
    typename trie<T>::const_node_iterator trie<T>::root() const{
        trie<T> const * tmp = this;
        while(tmp->get_parent() != nullptr){
            tmp = tmp->get_parent();
        }
        const_node_iterator ret(tmp);
        return ret;
    }

    template <typename T>
    typename trie<T>::leaf_iterator trie<T>::end(){ 
        trie<T> * tmp = findNextLeaf(this);
        leaf_iterator ret(tmp);
        return ret;
    }

    template <typename T>
    typename trie<T>::const_leaf_iterator trie<T>::end() const { 
        trie<T> const * tmp = findNextLeaf(this);
        const_leaf_iterator ret(tmp);
        return ret;
    }

    template <typename T>
    trie<T>& trie<T>::max(){
        auto max = this->begin();
        for(auto it = this->begin(); it != this->end(); it++){
            if(it.get_leaf().get_weight() > max.get_leaf().get_weight()){
                max = it; 
            }
        }
        return max.get_leaf();
    }

    template <typename T>
    const trie<T>& trie<T>::max() const {
        auto max = this->begin();
        for(auto it = this->begin(); it != this->end(); it++){
            if(it.get_leaf().get_weight() > max.get_leaf().get_weight()){
                max = it; 
            }
        }
        return max.get_leaf();
    }

    template <typename T>
    void printTrie(std::ostream& stream, trie<T> const& root){
        if(root.get_parent() != nullptr){
                stream<<*root.get_label()<<" ";
            }
        if(root.get_children().getSize() == 0){
            stream<<root.get_weight();
            stream<<" children = {}";
        }
        else{
            stream<<"children = {";
            for(auto it = root.get_children().begin(); it != root.get_children().end(); it++){
                printTrie(stream, *it);
                auto tmp = it;
                tmp++;
                if(tmp != root.get_children().end()){
                    stream<<",";
                }
            }
            stream<<"}";
        }
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, trie<T> const& root){
        printTrie(stream, root);
        return stream;
    }

    template <typename T>
     void trie<T>::path_compress() {
        trie<T> * tmp = this;
        if(tmp->get_children().getSize() == 0){
            return;
        }

        if(tmp->get_children().getSize() == 1){
            if(tmp->get_parent() == nullptr){
                return;
            }
            trie<T> node = *tmp->get_children().get(0);       // creo una nuova trie uguale al primo e unico figlio
            T label = *tmp->get_label() + *node.get_label();  // mergio le due label
            node.set_parent(tmp->get_parent());               // setto il parent del vecchio nodo alla trie
            *tmp = node;
            tmp->set_label(&label);
            tmp->get_parent()->get_children().reorder();
            tmp->path_compress();
        }

        else{
            for(auto it = tmp->get_children().begin(); it != tmp->get_children().end(); it++){
                it->path_compress();
            }
        }
     }