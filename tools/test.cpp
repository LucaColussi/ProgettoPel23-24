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

int main(){
    test_prefix_search();
    test_parsing_validation();
    testOperatorEqual();
    tryEdgeCases();
    testAddChild();
    testCopyConstructor();
    return 0;
}
