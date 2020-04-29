#include <bits/stdc++.h>

#define BST_NODE_FLAGXX 1
#define B_NODE_FLAGXX 2

class BSTNodeTraits{
public:
    typedef int value_t;
};

class BSTNode : public BSTNodeTraits{
    public: 
    typedef typename  BSTNodeTraits::value_t value_t;

    value_t data;
    BSTNode **children;

    BSTNode(value_t data){
        children = new BSTNode*[2] {nullptr};
        this->data = data;
    }

    ~BSTNode(void) {}
};

class BSTNodeTraitsF: public BSTNodeTraits{
public:
    typedef int value_t;

    class functor{
        void operator()(){
            return;
        }
    };

    typedef functor functor_t;
};

class BSTNodeF : public BSTNodeTraitsF{
public: 
    typedef typename  BSTNodeTraitsF::value_t value_t;

    value_t data;
    BSTNode **children;

    BSTNodeF(value_t data){
        children = new BSTNode*[2] {nullptr};
    }

    ~BSTNodeF(void) {}

};

class BNodeTraits{
public:
    typedef int value_t;
    typedef std::vector<value_t> container_t;
};

class BNode : public BNodeTraits{
public: 
    typedef typename  BNodeTraits::value_t value_t;
    typedef typename  BNodeTraits::container_t container_t;

    container_t data;
    BNode **children;
    bool isFull;
    int n, order;
    BNode **parent;

    BNode(){
        children = new BNode*[4] {nullptr};
        isFull = false;
        n = 0;
        order = 3;
        parent = nullptr;
    }

    BNode(value_t &val){
        children = new BNode*[4] {nullptr};
        isFull = false;
        n = 1;
        order = 3;
        parent = nullptr;
        this->data.push_back(val);
    }

    int insert(const value_t &val) {
        data.push_back(val);
        std::sort(data.begin(), data.end());
        n++;
        
        if (n == order) {
            isFull = true;
        }

        int pos = 0;
        for (value_t value : data) {
            if (val > value) {
                pos++;
            }
        }
        return pos;
    }

    BNode *leftChild() {
        BNode *node = new BNode(data[0]);
        children[0] != nullptr ? node->children[0] = children[0] : NULL;
        children[1] != nullptr ? node->children[1] = children[1] : NULL;
        return node;
    }

    BNode *rightChild() {
        BNode *node = new BNode(data[2]);
        node->insert(this->data[3]); 
        children[2] != nullptr ? node->children[0] = children[2] : NULL;
        children[3] != nullptr ? node->children[1] = children[3] : NULL;
        return node;
    }

    ~BNode(void){}

};

template <typename Node>
struct NodeTraits{
    static const int  flag_type = 0;
};

template <>
struct NodeTraits< BSTNode >{
    static const int  flag_type = BST_NODE_FLAGXX;
};


template <> 
struct NodeTraits< BNode >{
    static const int  flag_type = B_NODE_FLAGXX;
};

template<typename  Node, int Flag>
struct TreeHelper{
    typedef Node  node_t;
    typedef typename node_t::value_t value_t;
  
    static void  insert (node_t** head, const value_t& val){
        std::cout << "el tipo de dato no es compatible" << std::endl;
    }

    static void  print (node_t** head){
    
    }
};

template<>
struct TreeHelper<BSTNode,BST_NODE_FLAGXX>{
    typedef BSTNode  node_t;
    typedef typename node_t::value_t value_t;
  
    static void  insert (node_t** head, const value_t& val){
        node_t **curr = head;
        node_t **next = head;
    
        if ((*head) == nullptr) {
            *head = new node_t(val);
        } else {
            while((*next) != nullptr) {
                curr = next;
                if ((*curr)->data > val) {
                    next = &((*curr)->children[0]);
                } else {
                    next = &((*curr)->children[1]);
                }
            }
            *next = new node_t(val);
        }  
        std::cout << "Insertando nodo BST hoja" << std::endl;
    }

    static void print (node_t** head){
        std::stack<node_t*> nodes;
        node_t *curr = *head;
        while(curr != nullptr) {
            std::cout << "Node: " << curr->data << std::endl;
            if (curr->children[1] != nullptr) {
                nodes.push(curr->children[1]);
            }

            if (curr->children[0] != nullptr) {
                curr = curr->children[0];
            } else {
                if (!nodes.empty()) {
                    curr = nodes.top(); 
                    nodes.pop();
                } else {
                    curr = nullptr;
                }
            } 
        }
    }
};

template<>
struct TreeHelper<BNode,B_NODE_FLAGXX> {
    typedef BNode  node_t;
    typedef typename node_t::value_t value_t;
    typedef typename node_t::container_t container_t;
  
    static int find (const container_t& container, const value_t& val) {
        int i = 0;
        for (auto value : container) {
            if (value < val) 
                i++; 
        }
        return i;
    }

    static void  insert (node_t** head, const value_t& val) {
        node_t *curr = *head;
        node_t *next = *head;
        
        if ((*head) == nullptr) {
            *head = new node_t;
            (*head)->insert(val);
        } else {
            while(next != nullptr) {
                curr = next;
                next = curr->children[find(curr->data, val)];
            }
      
            if (!curr->isFull) {
                curr->insert(val);
            } else {
                curr->insert(val);

                node_t *left = curr->leftChild();
                node_t *right = curr->rightChild();
      
                if (curr->parent == nullptr) {
                    node_t *parent = new node_t;
                    parent->insert(curr->data[1]);
                    parent->children[0] = left;
                    parent->children[1] = right;
                    *head = &*parent;
                    left->parent = head;
                    right->parent = head;
                } else {
                    node_t *parent = *(curr->parent);
                    parent->children[parent->insert(curr->data[1]) + 1] = left;
                    parent->children[parent->insert(curr->data[1])] = right;  
                }
            } 
        }
        std::cout << "Insertando para un nodo B" << std::endl;
    }

    static void  print (node_t** head){
        node_t *curr = *head;
        container_t container = curr->data;
        BNode **children = curr->children;
        std::cout << "Node: ";

        if (container.size() > 1) {
            for (int i = 0; i < container.size() - 1; i++) {
                std::cout << container[i] << ", ";
            }
            std::cout << container[container.size() - 1];
        } else {
            std::cout << container[0];
        }

        std::cout << std::endl;
        for (int i = 0; i < 4; i++) {
            if (children[i] != nullptr) {
                print(&(*head)->children[i]);
            }
        }
    }
};

template <typename Node>
class Tree{
public:
    typedef Node node_t;
    typedef typename node_t::value_t value_t;

    node_t* root;

    Tree(void):root(nullptr){
        std::cout << NodeTraits<node_t>::flag_type << std::endl;
    }

    void insert(const value_t& val){
        add<NodeTraits<node_t>::flag_type>(&root,val);
    }
  
    void print() {
        show<NodeTraits<node_t>::flag_type>(&root);
    }

    template <int Flag> 
    void add(node_t**, const value_t&);

    template <int Flag>
    void show(node_t**);

    ~Tree(void){}
};

template<typename Node> template <int Flag>
void Tree<Node>::add(
    typename Tree<Node>::node_t ** root, 
    const typename Tree<Node>::value_t & val){
    TreeHelper<Tree<Node>::node_t,Flag>::insert(root,val);
}

template<typename Node> template <int Flag>
void Tree<Node>::show(typename Tree<Node>::node_t **root) {
    TreeHelper<Tree<Node>::node_t,Flag>::print(root);
}

int main() {
    typedef BSTNode bst_int_node;
    typedef BNode b_int_node;
    typedef Tree<bst_int_node> bst_tree;
    typedef Tree<b_int_node> b_tree;

    bst_tree bst;
    b_tree bt;

    bst.insert(10);
    bst.insert(15);
    bst.insert(5);
    bst.insert(25);
    bst.insert(20);
    std::cout << "Imprimiendo BST: " << std::endl;
    bst.print();

    bt.insert(10);
    bt.insert(15);
    bt.insert(5);
    bt.insert(25);
    bt.insert(20);
    std::cout << "Imprimiendo BT: " << std::endl;
    bt.print();
}