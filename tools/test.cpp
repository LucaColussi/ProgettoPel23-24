#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/trie.cpp"

using namespace std;
namespace fs = std::filesystem;

#define WORKSPACE_PATH "/Users/lucacolussi/Uni/ProgettoPel23-24"

template <typename T>
trie<T> load_trie(string filePath) {
   trie<T> t;
   string fullPath = (string)WORKSPACE_PATH + "/tools/datasets/" + filePath;
   std::ifstream file(fullPath);

   if (!file.is_open()) {
      throw parser_exception("Could not open file: " + fullPath);
   }

   file >> t;
   file.close();
   return t;
}

template <typename T>
void parsing_should_throw_exception(string filePath, T type) {
   try {
      trie<T> t = load_trie<T>(filePath);
      assert(false);
   } catch (const parser_exception& e) {
      assert(true);
   }
}

template <typename T>
void parsing_should_complete(string filePath, T type) {
   try {
      trie<T> t = load_trie<T>(filePath);
      assert(true);
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

int count_error_files(const fs::path& directory, string type) {
   int count = 0;

   for (const auto& entry : fs::directory_iterator(directory)) {
      if (entry.is_regular_file() &&
          entry.path().filename().string().find("error") != std::string::npos &&
          entry.path().filename().string().find(type) != std::string::npos) {
         ++count;
      }
   }

   return count;
}

int count_non_error_files(const fs::path& directory, string type) {
   int count = 0;

   for (const auto& entry : fs::directory_iterator(directory)) {
      if (entry.is_regular_file() &&
          entry.path().filename().string().find("error") == std::string::npos &&
          entry.path().filename().string().find(type) != std::string::npos) {
         ++count;
      }
   }

   return count;
}

void test_parsing_validation() {
   int char_error_files = count_error_files(
       fs::path((string)WORKSPACE_PATH + "/tools/datasets"), "char");
   int string_error_files = count_error_files(
       fs::path((string)WORKSPACE_PATH + "/tools/datasets"), "string");

   int char_non_error_files = count_non_error_files(
       fs::path((string)WORKSPACE_PATH + "/tools/datasets"), "char");
   int string_non_error_files = count_non_error_files(
       fs::path((string)WORKSPACE_PATH + "/tools/datasets"), "string");

   // Should throw exceptions -- char
   for (int i = 1; i <= char_error_files; i++) {
      parsing_should_throw_exception("trie_char_error" + to_string(i) + ".tr",
                                     char());
   }
//    // Should throw exceptions -- string
   for (int i = 1; i <= string_error_files; i++) {
      parsing_should_throw_exception("trie_string_error" + to_string(i) + ".tr",
                                     string());
   }

   // Should complete -- char
   for (int i = 1; i <= char_non_error_files; i++) {
      parsing_should_complete("trie_char" + to_string(i) + ".tr", char());
   }

   // Should complete -- string
   for (int i = 1; i <= string_non_error_files; i++) {
      parsing_should_complete("trie_string" + to_string(i) + ".tr", string());
   }
}

void test_prefix_search() {
   try {
      trie<char> t = load_trie<char>("trie_char1.tr");

      vector<char> path = {'a', 'b', 'c'};
      assert(t[path] == *t.get_children().get(0));

      path = {'z'};
      assert(t[path] == t);

      path = {'z', 'b', 'e', 'c', 'b'};
      assert(
          t[path] ==
          *t.get_children().get(1)->get_children().get(1)->get_children().get(
              0));

      const trie<char> t1 = load_trie<char>("trie_char1.tr");

      path = {'a', 'b', 'c'};
      assert(t1[path] == *t.get_children().get(0));

      path = {'z'};
      assert(t1[path] == t);

      path = {'z', 'b', 'e', 'c', 'b'};
      assert(
          t1[path] ==
          *t.get_children().get(1)->get_children().get(1)->get_children().get(
              0));

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void testAddChild(){
    try
    {
        trie<char>  node;
        trie<char> child;
        child.set_parent(&node);
        child.set_weight(3.1);
        char * label = new char('a');
        child.set_label(label);
        node.add_child(child);
        node.get_children();
        assert(node.get_weight() == 0);
    }
    catch(const parser_exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

    void testCopyConstructor(){
        trie<char> root; 
        trie<char> firstChild(3.1);
        firstChild.set_parent(&root);
        char * label = new char('a');
        firstChild.set_label(label);

        trie<char> secondChild;
        secondChild.set_parent(&root);
        label = new char('b');
        secondChild.set_label(label);

        trie<char> thirdChild(2.9);
        thirdChild.set_parent(&secondChild);
        label = new char('c');
        thirdChild.set_label(label);

        secondChild.add_child(thirdChild);
        root.add_child(firstChild);
        root.add_child(secondChild);
    }

    void tryEdgeCases(){
        try{
            trie<char> child;
            trie<char> root;
            child.set_parent(&root);    
            char * label = new char('a');
            child.set_label(label);
            child.set_weight(3.1);
            root.add_child(child);
        }
            catch(const parser_exception& e)
        {

            std::cerr << e.what() << '\n';
        }
    }

    void testOperatorEqual(){
        try
         {
        trie<char> root; 
        trie<char> firstChild(3.1);
        firstChild.set_parent(&root);
        char * label = new char('a');
        firstChild.set_label(label);

        trie<char> secondChild;
        secondChild.set_parent(&root);
        label = new char('b');
        secondChild.set_label(label);

        trie<char> thirdChild(2.9);
        thirdChild.set_parent(&secondChild);
        label = new char('c');
        thirdChild.set_label(label);

        secondChild.add_child(thirdChild);
        root.add_child(firstChild);
        root.add_child(secondChild);

        trie<char> root2;
        trie<char> firstChild2(3.1);
        firstChild2.set_parent(&root2);
        label = new char('a');
        firstChild2.set_label(label);

        trie<char> secondChild2;
        secondChild2.set_parent(&root2);
        label = new char('b');
        secondChild2.set_label(label);

        trie<char> thirdChild2(2.9);
        thirdChild2.set_parent(&secondChild2);
        label = new char('c');
        thirdChild2.set_label(label);

        secondChild2.add_child(thirdChild2);
        root2.add_child(firstChild2);
        root2.add_child(secondChild2);

        assert(root == root2); // test case che ritorna true
         }
             catch(const parser_exception& e)
    {
        std::cerr << e.what() << '\n';
    }
        
    }

    void test_leaf_iterator() {
   try {
      trie<char> t = load_trie<char>("trie_char2.tr");
      trie<char>::leaf_iterator rootBegin(&t);

      trie<char>* child = t.get_children().get(0);
      trie<char>::leaf_iterator childBegin = child->begin();

      assert(rootBegin.get_leaf() == childBegin.get_leaf());

      trie<char>::leaf_iterator rootEnd = t.end();
      trie<char>::leaf_iterator childEnd = child->end();

      assert(childEnd.get_leaf() == *t.get_children().get(1));

      assert(
          (rootBegin++).get_leaf() ==
          *t.get_children().get(0)->get_children().get(0)->get_children().get(
              0));
      assert(
          (rootBegin).get_leaf() ==
          *t.get_children().get(0)->get_children().get(0)->get_children().get(
              1));
      assert((++rootBegin).get_leaf() == childEnd.get_leaf());
      rootBegin++;
      assert(rootBegin == rootEnd);

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_const_leaf_iterator() {
   try {
      const trie<char> t = load_trie<char>("trie_char2.tr");
      trie<char>::const_leaf_iterator rootBegin = t.begin();

      const trie<char>* child = t.get_children().get(0);
      trie<char>::const_leaf_iterator childBegin = child->begin();

      assert(rootBegin.get_leaf() == childBegin.get_leaf());

      trie<char>::const_leaf_iterator rootEnd = t.end();
      trie<char>::const_leaf_iterator childEnd = child->end();

      assert(childEnd.get_leaf() == *t.get_children().get(1));

      assert(
          (rootBegin++).get_leaf() ==
          *t.get_children().get(0)->get_children().get(0)->get_children().get(
              0));
      assert(
          (rootBegin).get_leaf() ==
          *t.get_children().get(0)->get_children().get(0)->get_children().get(
              1));
      assert((++rootBegin).get_leaf() == childEnd.get_leaf());
      rootBegin++;
      assert(rootBegin == rootEnd);

      for (auto it = t.begin(); it != t.end(); it++) {
      }

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_node_iterator() {
   try {
      trie<char> t = load_trie<char>("trie_char5.tr");
      trie<char>::node_iterator root = t.root();

      trie<char>* child =
          t.get_children().get(0)->get_children().get(0)->get_children().get(0);
      trie<char>::node_iterator childIT = trie<char>::node_iterator(child);

      assert(*child->get_label() == *(childIT++));
      assert(*t.get_children().get(0)->get_children().get(0)->get_label() ==
             *childIT);
      assert(*t.get_children().get(0)->get_label() == *(++childIT));

      assert(root == ++childIT);

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_const_node_iterator() {
   try {
      const trie<char> t = load_trie<char>("trie_char5.tr");
      trie<char>::const_node_iterator root = t.root();

      const trie<char>* child =
          t.get_children().get(0)->get_children().get(0)->get_children().get(0);
      trie<char>::const_node_iterator childIT =
          trie<char>::const_node_iterator(child);

      assert(*child->get_label() == *(childIT++));
      assert(*t.get_children().get(0)->get_children().get(0)->get_label() ==
             *childIT);
      assert(*t.get_children().get(0)->get_label() == *(++childIT));

      assert(root == ++childIT);

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_bag_iterator() {
   try {
      trie<char> t = load_trie<char>("trie_char1.tr");
      bag<trie<char>>::children_iterator it = t.get_children().begin();

      // Check first element - arrow operator
      assert(*it->get_label() == 'a');

      // Check first element - dereference operator
      assert(*(*it).get_label() == 'a');

      // Check if incrementing works
      it++;
      assert(*it->get_label() == 'b');

      // Check if .end() works
      it++;
      assert(it == t.get_children().end());

      // Check if ++it works
      it = t.get_children().begin();
      assert(*((++it)->get_label()) == 'b');

      // Check if it++ works
      it = t.get_children().begin();
      assert(*((it++)->get_label()) == 'a');
      assert(*((it)->get_label()) == 'b');

      // Check if sub-iterator works
      it = t.get_children().begin();
      bag<trie<char>>::children_iterator second_it = it->get_children().begin();
      assert(second_it == it->get_children().end());

      it++;
      second_it = it->get_children().begin();
      assert(*second_it->get_label() == 'b');

      second_it++;
      assert(*second_it->get_label() == 'c');

       char lb = 'd';
       second_it->set_label(&lb);
       assert(*second_it->get_label() == 'd');

       second_it++;
       assert(second_it == t.get_children().end());
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_const_bag_iterator() {
   try {
      const trie<char> t = load_trie<char>("trie_char1.tr");
      bag<trie<char>>::const_children_iterator it = t.get_children().begin();

      // Check first element - arrow operator
      assert(*it->get_label() == 'a');

      // Check first element - dereference operator
      assert(*(*it).get_label() == 'a');

      // Check if incrementing works
      it++;
      assert(*it->get_label() == 'b');

      // Check if .end() works
      it++;
      assert(it == t.get_children().end());

      // Check if ++it works
      it = t.get_children().begin();
      assert(*((++it)->get_label()) == 'b');

      // Check if it++ works
      it = t.get_children().begin();
      assert(*((it++)->get_label()) == 'a');
      assert(*((it)->get_label()) == 'b');

      // Check if sub-iterator works
      it = t.get_children().begin();
      bag<trie<char>>::const_children_iterator second_it =
          it->get_children().begin();
      assert(second_it == it->get_children().end());

      it++;
      second_it = it->get_children().begin();
      assert(*second_it->get_label() == 'b');

      second_it++;
      assert(*second_it->get_label() == 'c');

      second_it++;
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_max_function() {
   try {
      trie<char> t = load_trie<char>("trie_char5.tr");
      assert(t.max() == *t.get_children().get(1));

      trie<char> childNode = *t.get_children().get(0);
      assert(childNode.max() ==
             *childNode.get_children().get(0)->get_children().get(1));

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_ostream() {
   try {
      trie<char> t = load_trie<char>("trie_char1.tr");
      cout << t;
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_path_compression() {
   try {
      trie<string> t = load_trie<string>("compression/trie_string1.tr");
      trie<string> expectedResult =
      load_trie<string>("compression/result_1.tr");
      t.path_compress();
      assert(expectedResult == t);

      trie<int> t2 = load_trie<int>("compression/trie_int2.tr");
      trie<int> expectedResult2 = load_trie<int>("compression/result_2.tr");
      t2.path_compress();
      assert(expectedResult2 == t2);

      t2 = load_trie<int>("compression/trie_int3.tr");
      expectedResult2 = load_trie<int>("compression/result_3.tr");
      t2.path_compress();
      assert(expectedResult2 == t2);

      t2 = load_trie<int>("compression/trie_int4.tr");
      expectedResult2 = load_trie<int>("compression/result_4.tr");
      t2.path_compress();
      assert(expectedResult2 == t2);

      t2 = load_trie<int>("compression/trie_int5.tr");
      expectedResult2 = load_trie<int>("compression/result_5.tr");
      t2.path_compress();
      assert(expectedResult2 == t2);

      t2 = load_trie<int>("compression/trie_int6.tr");
      expectedResult2 = load_trie<int>("compression/result_6.tr");
      t2.path_compress();
      assert(expectedResult2 == t2);

   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

int main(){
    test_bag_iterator();
    test_const_bag_iterator();
    test_const_node_iterator();
    test_node_iterator();
    test_leaf_iterator();
    test_const_leaf_iterator();
    test_prefix_search();
    test_parsing_validation();
    test_max_function();
    test_ostream();
    test_path_compression();
    // testOperatorEqual();
    // tryEdgeCases();
    // testAddChild();
    // testCopyConstructor();
    return 0;
}
