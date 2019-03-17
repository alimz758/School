#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>

#include <vector>
const int ALPHABET_SIZE=26;
using namespace std;

template <typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;
      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    // Trie node structure
    struct TrieNode
    {
        TrieNode *children[ALPHABET_SIZE];           //Your Trie class template must work for key strings containing any characters, not just A, C, G, T, and N
        vector<ValueType> values;//store the values, ValueType, in each TrieNode
    };
    TrieNode *root;
    //helper function for destructor; recursively delete nodes
    void clearTrie(TrieNode *current);
    struct TrieNode* getNode()
    {
        struct TrieNode* pNode = new TrieNode();
        for (int i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
        return pNode;
    }
    std::vector<ValueType> helper(TrieNode *cur, std::string key, bool exactMatch, vector<ValueType> temp) const
    {
        if(cur==NULL)
            return temp;
        if(key=="")
        {
            //push the values of the current child to the result
            for(int i=0; i<cur->values.size();i++)
                temp.push_back(cur->values[i]);
            return temp;
        }
            //recursively check all the children
        for(int i=0; i < ALPHABET_SIZE ; i++ )
        {
            if(exactMatch==true )
            {
                    //if the current char in key, it's ASCII value, is not the same is alphet
                if( key[0] -'A' == i )
                {
                    cur=cur->children[i];
                    temp=helper(cur, key.substr(1), true, temp);
                }
            }
            if(exactMatch==false)
            {
                if(int(key[0] -'A') == i && cur->children[i]!=NULL)
                    temp=helper(cur->children[i], key.substr(1), false, temp);
                    //if found a missmatch, pass in true so , for the next round it can't find a missmatch
                else if(int(key[0] -'A') != i && cur->children[i]!=NULL)
                    temp = helper(cur->children[i], key.substr(1), true, temp);
            }
        }
        return temp;
    }
    
};

template <typename ValueType>
void Trie<ValueType> ::clearTrie(TrieNode *current)
{
    if(current==NULL)
        return;
    for(int i=0; i<ALPHABET_SIZE; i++)
        clearTrie(current->children[i]);
    delete current;
    current=NULL;
}
//constructor implementation
template<typename ValueType>
Trie<ValueType>::Trie()
{
    // It should create a root trie node with no children and no values, and set the root pointer (if any) to point to this root node. This method must run in O(1) time.
    root= new TrieNode();
    for (int i=0; i < ALPHABET_SIZE; i++)
    {
        root->children[i]=NULL;
    }
}
//implementation of Destructor
template<typename ValueType>
Trie<ValueType>:: ~Trie()
{
    //The destructor must free all memory associated with trie. This method must run in O(N)
    // time where N is the number of nodes in the trie.
    clearTrie(root);//call the private function by passing the root node
}
//reset()
template<typename ValueType>
void Trie<ValueType>:: reset()
{
    //Your trie’s reset() method must free all of the memory associated with the current trie,
    //then allocate a new empty trie with a single root node (with no children or values). This
    //method must run in O(N) time where N is the number of nodes in the trie
    clearTrie(root);
    TrieNode  *root= new TrieNode();
    for (int i=0; i < ALPHABET_SIZE; i++)
    {
        root->children[i]=NULL;
    }
}
//insert()
template<typename ValueType>
void Trie<ValueType>:: insert(const std::string& key, const ValueType& value)
{
    string tempKey=key;
    TrieNode *cur= root;//a pointer that points to the root
    while( tempKey != "" )
    {
        if(cur->children[tempKey[0]-'A'] == NULL)
            cur->children[tempKey[0]-'A']= getNode();
        cur=cur->children[tempKey[0]-'A'];
        tempKey=tempKey.substr(1);
    }
    cur->values.push_back(value);
    // mark last node as leaf
}
//find()
template<typename ValueType>
std::vector<ValueType> Trie<ValueType>:: find(const std::string& key, bool exactMatchOnly) const
{
    vector<ValueType> tempVector={}; //to store all the values found within a match
    for (int i=0; i< ALPHABET_SIZE;i++)
    {
        if(i==int(key[0]-'A'))
            tempVector=helper(root->children[i], key.substr(1), exactMatchOnly, tempVector);
    }
     return tempVector;
}
#endif // TRIE_INCLUDED
