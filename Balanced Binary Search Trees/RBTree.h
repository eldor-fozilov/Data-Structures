#include "Tree.h"

#define BLACK 0
#define RED 1
#define DOUBLE_BLACK 2

template <class keyT, class valT>
class RBTree_t : public Tree_t<keyT, valT> {
    size_t check_rb(Node_t<keyT, valT>* n) {
        // Checks RB tree property
        size_t b, br, bl;
        br = bl = b = 1;
        assert(n);
        if(n && n->right) {
            br = check_rb(n->right);
            assert(n->meta == BLACK || n->right->meta == BLACK);
        }
        if(n && n->left) {
            bl = check_rb(n->left);
            assert(n->meta == BLACK || n->left->meta == BLACK);
        }
        b = br;
        if(n == this->root && br != bl) {
            dump_subtree(this->root);
        }
        assert(n != this->root || br == bl);
        b = br;
        if(n->meta == BLACK) b += 1;
        if(n == this->root) assert(n->meta == BLACK);

        return b;
    }

    void balance(Node_t<keyT, valT>* n, Node_t<keyT, valT>* new_node) {
        // Make tree balanced
        Node_t<keyT, valT>* temp;

        if (n->left && n->left->left == new_node){
            temp = n->left;
            this->rotate(n, true);
            flip(temp, new_node);
        }
        else if (n->left && n->left->right == new_node){
            temp = n->left;
            this->rotate(n->left,false);
            this->rotate(n,true);
            flip(new_node, temp);

        }
        else if (n->right && n->right->right == new_node){
            temp = n->right;
            this->rotate(n,false);
            flip(temp, new_node);
        }
        else{
            temp = n->right;
            this->rotate(n->right, true);
            this->rotate(n, false);
            flip(new_node,temp);
        }
    }

    void flip(Node_t<keyT, valT>* n, Node_t<keyT, valT>* new_node) {
        // Flip the color of this node and all children
        if (n != this->root){
            if (n->meta == BLACK)
                n->meta = RED;
            else
                n->meta = BLACK;
        }
        else if (n->meta == RED)
            n->meta = BLACK;

        if (n->left && n->left != new_node){
            if (n->left->meta == BLACK)
                n->left->meta = RED;
            else
                n->left->meta = BLACK;
        }

        if (n->right && n->right != new_node){
            if (n->right->meta == BLACK)
                n->right->meta = RED;
            else
                n->right->meta = BLACK;
        }
        
    }

    void balance_RBTree(Node_t<keyT, valT>* new_node){
        if (this->root == new_node)
            new_node->meta = BLACK;
        
        else{
            new_node->meta = RED;

            if (new_node->parent->meta == BLACK)
                return;
            
            else{

                Node_t<keyT, valT> *uncle_node;
                if (new_node->parent->parent->left == new_node->parent)
                    uncle_node = new_node->parent->parent->right;
                else
                    uncle_node = new_node->parent->parent->left;

                if (uncle_node && uncle_node->meta == RED){
                    
                    flip(uncle_node->parent, new_node);
                    
                    if (uncle_node->parent != this->root)
                        balance_RBTree(uncle_node->parent);
                }
                
                else
                    balance(new_node->parent->parent, new_node);
            }
        }
    }

    void check_delete(Node_t<keyT, valT>* n){
        
        if (this->root->meta == DOUBLE_BLACK){
                this->root->meta = BLACK;
                return;
        }

        Node_t<keyT, valT>* sibling_node;
        if (n->parent->left == n)
            sibling_node = n->parent->right;
        else
            sibling_node = n->parent->left;
                
        if (sibling_node && sibling_node->meta == BLACK){
            if ((sibling_node->right == nullptr || sibling_node->right->meta == BLACK) && (sibling_node->left == nullptr || sibling_node->left->meta == BLACK)){
                n->meta = BLACK;
                
                if (n->parent->meta == RED)
                    n->parent->meta = BLACK;
                else
                    n->parent->meta = DOUBLE_BLACK;

                sibling_node->meta = RED;
                if (n->parent->meta == DOUBLE_BLACK)
                    check_delete(n->parent);
            }
        
            else{
                if (n->parent->left == n){
                    short temp;
                    if ((sibling_node->right == nullptr || sibling_node->right->meta == BLACK) && (sibling_node->left && sibling_node->left->meta == RED)){
                        temp = sibling_node->left->meta;
                        sibling_node->left->meta = sibling_node->meta;
                        sibling_node->meta = temp;
                        this->rotate(sibling_node, true);
                        check_delete(n);
                        }
                    else if (sibling_node->right && sibling_node->right->meta == RED){
                        temp = sibling_node->meta;
                        sibling_node->meta = n->parent->meta;
                        n->parent->meta = temp;
                        this->rotate(n->parent, false);
                        n->meta = BLACK;
                        sibling_node->right->meta = BLACK;
                    }
                }
                else{
                    short temp;
                    if ((sibling_node->left == nullptr || sibling_node->left->meta == BLACK) && (sibling_node->right && sibling_node->right->meta == RED)){
                        temp = sibling_node->right->meta;
                        sibling_node->right->meta = sibling_node->meta;
                        sibling_node->meta = temp;
                        this->rotate(sibling_node, false);
                        check_delete(n);
                    }
                    else if (sibling_node->left && sibling_node->left->meta == RED){
                        temp = sibling_node->meta;
                        sibling_node->meta = n->parent->meta;
                        n->parent->meta = temp;
                        this->rotate(n->parent,true);
                        n->meta = BLACK;
                        sibling_node->left->meta = BLACK;
                    }                      
                }
            }
        }          
        
        else if (sibling_node && sibling_node->meta == RED){
            short temp = sibling_node->meta;
            sibling_node->meta = n->parent->meta;
            n->parent->meta = temp;
            if (n->parent->left == n)
                this->rotate(n->parent, false);
            else
                this->rotate(n->parent,true);
            
            check_delete(n);
        }
    }

    void remove_node(Node_t<keyT, valT>* n){
        
        if(n->left && n->right) {
                Node_t<keyT, valT>* r = this->get_min(n->right);
                n->key = r->key;
                n->value = r->value;
                remove_node(r);
            } else if (n->left) {
                n->left->parent = n->parent;
                
                if(n->parent) {
                    if(n == n->parent->left) n->parent->left = n->left;
                    else n->parent->right = n->left;
                }

                if(n == this->root) {
                    this->root = n->left;
                    n->left->parent = nullptr;
                }

                if (n->meta != RED){
                    if (n->left->meta == BLACK){
                        n->left->meta = DOUBLE_BLACK;
                        check_delete(n->left);
                    }
                    else
                        n->left->meta = BLACK;
                }

                delete n;
            } else if (n->right) {
                n->right->parent = n->parent;
                if(n->parent) {
                    if(n == n->parent->left) n->parent->left = n->right;
                    else n->parent->right = n->right;
                }
                if(n == this->root) {
                    this->root = n->right;
                    n->right->parent = nullptr;
                }

                if (n->meta != RED){
                    if (n->right->meta == BLACK){
                        n->right->meta = DOUBLE_BLACK;
                        check_delete(n->right);
                    }
                    else
                        n->right->meta = BLACK;
                }
                delete n;

            } else {

                if (n->meta == BLACK){
                    n->meta = DOUBLE_BLACK;
                    check_delete(n);
                }

                if(n->parent) {
                    if(n == n->parent->left) n->parent->left = nullptr;
                    else n->parent->right = nullptr;
                }
                if(n == this->root) this->root = nullptr;

                delete n;
            }
    }


public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // we create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // we update the value, rather than making a new one.
        bool check = true;
        if(this->root){
            Node_t<keyT, valT>* n = this->search_subtree(this->root, key);
            if (n && n->key == key)
                check = false;
        }
        Node_t<keyT, valT> *new_node = this->insert_internal(key, value);
        if (check)
            balance_RBTree(new_node);
    }


    
    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
         if(!this->root) return false;
            Node_t<keyT, valT>* n = this->search_subtree(this->root, key);
            if (n && n->key == key){
                remove_node(n);
                return true;
            }
            else return false;
    }

};
