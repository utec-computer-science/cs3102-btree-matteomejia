#include <iostream>
#include <vector> 
#include <math.h>

template <typename T>
class SS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;
  
    class simple_search{
    public:
        int operator() (container_t a, value_t v){
            return 0;
        }
    };

    class post_order_print{
    public:
        void operator() (void){
            std::cout << "post order" << std::endl;
        }
    };

    typedef simple_search functor_t;
    typedef post_order_print print_t;
};

template <typename T>
class BS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;
  
    class binary_search{
    public:
        int operator() (container_t a, value_t v){
            return 0;
        }
    };

    class pre_order_print{
    public:
        void operator() (void){
            std::cout << "pre order" << std::endl;
        }
    };

    typedef binary_search functor_t;
    typedef pre_order_print print_t;
};

template <typename T, int S> 
class BNode {
public:
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    //typedef typename T::iterador_t iterator_t;
    typedef std::vector< BNode<T,S>* > pcontainer_t;

    container_t keys;
    pcontainer_t ptrs;
    std::size_t  order;
    std::size_t n;
    bool isLeaf;

    BNode(bool leaf):order(S) {
        keys = container_t(order,0);
        ptrs = pcontainer_t(order,NULL);
        n = 0;
        isLeaf = leaf;
    }

    ~BNode(void) {}

    bool search(int val) {
        int i = 0;
        while (i < n && val > keys[i]) {
            i++;
        }

        if (keys[i] == val) {
            return true;
        }

        if (isLeaf) {
            return false;
        }

        return ptrs[i]->search(val);
    }

    void insertNonFull(T val) {
        int i = n - 1;

        if (isLeaf) {
            while (i >= 0 && keys[i] > val) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = val;
            n += 1;
        } else {
            while (i >= 0 && keys[i] > val) {
                i--;
            }

            if (ptrs[i + 1]->n == order) {
                splitChild(i + 1, ptrs[i + 1]);

                if (keys[i + 1] < val) {
                    i++;
                }
            }

            ptrs[i + 1]->insertNonFull(val);
        }
    }

    void splitChild(int i, BNode<T,S> *b) {
        BNode<T,S> *x = new BNode<T,S>(b->isLeaf);
        x->n = std::ceil(order/2) - 1;

        for (int j = 0; j < std::ceil(order/2) - 1; j++) {
            x->keys[j] = b->keys[j + x->n];
        }

        if (!b->isLeaf) {
            for (int j = 0; j < std::ceil(order/2); j++) {
                x->ptrs[j] = b->ptrs[j + x->n];
            }
        }

        b->n = std::ceil(order/2) - 1;

        for (int j = n; j >= i + 1; j--) {
            ptrs[j + 1] = ptrs[j]; 
        }

        ptrs[i + 1] = x;

        for (int j = n - 1; j >= i; j--) {
            keys[j + 1] = keys[j];
        }

        keys[i] = b->keys[std::ceil(order/2) - 1];

        n += 1;
            
    }

    std::ostream &printNode(std::ostream &out) {
        for (auto x : keys) {
            out << x << " ";
        }
        out << std::endl;
        return out;
    }
};

template <typename T, int S>
class BTree {
public: 
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    typedef typename T::functor_t functor_t;
    typedef typename T::print_t print_t;

    BNode<T,S>* root;
    //print_t print;
    functor_t search;

    BTree(void):root(NULL) {}

    ~BTree(void){}

    void insert(const value_t& val = 0){
        // TODO :: INSERT
        if (root == NULL) {
            root = new BNode<T,S>(true);
            root->keys[0] = val;
            root->n += 1;
        } else {
            if (root->n == root->order) {
                BNode<T,S> *b = new BNode<T,S>(false);

                b->ptrs[0] = root;
                b->splitChild(0, root);

                int i = 0;
                if (b->keys[0] < val) {
                    i++;
                }

                b->ptrs[i]->insertNonFull(val);
            }
        }
    }

    bool find(const value_t val = 0) const{
        if (root == NULL) {
            return false;
        } else {
            return root->search(val);
        }
    }

    std::ostream &printSingleLevel(std::ostream &out, BNode<T,S> *root, int level) {
        if (root == NULL) {
            return out;
        }
        if (level == 1) {
            root->printNode();
        } else if (level > 1) {
            for (auto x : root->ptrs) {
                printSingleLevel(out, x, level - 1);
            }
        }
        return out;
    }

    int height(BNode<T,S> *node) {
        if (node == NULL) {
            return 0;
        } else {
            std::floor(std::log((node->n + 1) / 2) / std::log(node->order))
        }
    }

    std::ostream &printLevels (std::ostream &out, BNode<T,S> *root) {
        int h = height(root);
        for (int i = 1; i <= h; i++) {
            printSingleLevel(out, root, i);
            out << std::endl;
        }
        return out;
    }

    std::ostream &print(std::ostream &out) {
        out << printLevels(out, root);
        return out;
    }

    template <typename _T, int _S>
    friend std::ostream& operator<<(std::ostream &out, BTree<_T,_S> tree){
        out = tree.print(out);// (out)
        // IN PRE POST LEVEL ORDER
        return out;
    }

};

int main() {
    typedef BS_Traits<int> btrait_t;
    BTree<btrait_t,4> tree;
    tree.find(10);
    std::cout<<tree<< std::endl;

    /*typedef SS_Traits<float> strait_t;
    BTree<strait_t,10> stree; 
    std::cout<<stree<< std::endl;*/
}
