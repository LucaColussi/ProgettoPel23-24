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
        T * l = new T(label);
        node.set_parent(&t);
        node.set_label(l);
        
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
        S(stream, t);
        return stream;
    }
     
