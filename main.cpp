#include <iostream>
#include <math.h>
#include <vector> 

template <typename T>
class SS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef std::vector<value_t*> pcontainer_t;
    typedef typename container_t::iterator iterator_t;
  
    class simple_search{
    public:
        iterator_t operator() (container_t a, pcontainer_t p, value_t v){
        return p.end();
        }
    };

    class post_order_print{
    public:
        void operator() (void) {
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
    typedef std::vector<value_t*> pcontainer_t;
    typedef typename container_t::iterator iterator_t;
  
    class binary_search{
    public:
        iterator_t operator() (container_t a, pcontainer_t p, value_t v){
            return p.end();
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
    typedef typename T::pcontainer_t pcontainer_t;

    container_t keys;
    pcontainer_t ptrs;
    std::size_t  order;
    int n;
    bool isLeaf;

    BNode(bool leaf):order(S){
        keys=container_t(order,0);
        ptrs=pcontainer_t(order,NULL);
        isLeaf = leaf;
    }

    ~BNode(void){}

    void insertNonfull (int val) {
        int i = keys.size() - 1;

        if (isLeaf) {
            while (i >= 0 && keys[i] > val) {
                keys[i+1] = keys[i];
                i--;
            }

            keys[i+1] = val;
            n += 1;
        } else {
            while (i >= 0 && keys[i] > val) {
                i--;
            }

            if (n == 2*order-1) {
                splitChild(i+1, ptrs[i+1]);

                if (keys[i+1] < val) {
                    i++;
                }
            }
            ptrs[i+1]->insertNonfull(val);
        }
    }

    void splitChild (int i, BNode *t) {
        BNode<T,S> *b = new BNode<T,S>(t->isLeaf);
        b->n = order - 1;

        for (int j = 0; j < order-1; j++) {
            b->keys[j] = t->keys[j+order];
        }

        if (!t->isLeaf) {
            for (int j = 0; j < order; j++) {
                b->ptrs[j] = t->ptrs[j+order];
            }
        }

        t->n = order - 1;

        for (int j = n; j >= i+1; j--) {
            ptrs[j+1] = ptrs[j];
        }

        ptrs[i+1] = b;

        for (int j = n-1; j >= i; j--) {
            keys[j+1] = keys[j];
        }

        keys[i] = t->keys[order-1];

        n += 1;
    }
};

template <typename T, int S>
class BTree {
public: 
    typedef typename T::value_t value_t;
    typedef typename T::functor_t functor_t;
    typedef typename T::print_t print_t;

    BNode<T,S>* root;
    print_t print;
    functor_t search;

    BTree(void):root(NULL) {

    }

    ~BTree(void) {}

    void insert(const value_t& val = 0) {
        // TODO :: INSERT
        if (root == NULL) {
            root = new BNode<T,S>(true);
            root->keys[0] = val;
            root->n = 1;
        } else {
            if (root->n == 2*S-1) {
                BNode<T,S> *t = new BNode<T,S>(false);
                t->ptrs[0] = root;
                t->splitChild(0, root);

                int i = 0;
                if (t->keys[0] < val) {
                    i++;
                }
                t->ptrs[i]->insertNonfull(val);

                root = t;
            } else {
                root->insertNonfull(val);
            }
        }
    }

    bool find(BNode<T,S> *bn = root, const value_t val = 0) const {
        // TODO :: SEARCH
        // search(x); inside each page
        int i = 0;
        while (i < bn->n && val > bn->keys[i]) {
            i++;
        }

        if (bn->keys[i] == val) {
            return true;
        }

        if (bn->isLeaf) {
            return false;
        }

        return find(bn->ptrs[i], val);
    }

    template <typename _T, int _S>
    friend std::ostream& operator<< (std::ostream& out, BTree<_T,_S> tree){
        tree.print();// (out)
        // IN PRE POST LEVEL ORDER
        return out;
    }
};

int main() {
    typedef BS_Traits<int> btrait_t;
    BTree<btrait_t,4> tree;
    tree.find(tree.root, 10);
    std::cout<<tree<< std::endl;


    typedef SS_Traits<float> strait_t;
    BTree<strait_t,10> stree; 
    std::cout<<stree<< std::endl;
}
