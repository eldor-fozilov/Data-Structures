#include "Tree.h"

template <class keyT, class valT>
class AVLTree_t : public Tree_t<keyT, valT> {
    long check_bf(Node_t<keyT, valT>* n) {
        // Checks balance factor values in tree.
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = check_bf(n->right);
        hl = check_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        assert(bf == n->meta);
        assert(bf < 2);
        assert(bf > -2);
        return h;
    }

    long calculate_bf(Node_t<keyT, valT>* n){ 
        return height_subtree(n->left) - height_subtree(n->right);}

    long height_subtree(Node_t<keyT, valT>* n){
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = height_subtree(n->right);
        hl = height_subtree(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        return h;
    }

    void balance(Node_t<keyT, valT>* a, Node_t<keyT, valT>* new_node) {
        // Make tree balanced
        if (a->meta > 1){
            if (a->left->left && this->search_subtree(a->left->left, new_node->key) == new_node)
                this->rotate(a, true);
            else{
                Node_t<keyT, valT>* temp = a->left;
                this->rotate(a->left, false);
                temp->meta = calculate_bf(temp);
                this->rotate(a,true);
            }
        }
        else if (a->meta < -1){
            
            if (a->right->right && this->search_subtree(a->right->right, new_node->key) == new_node)
                this->rotate(a, false);
            else{
                Node_t<keyT, valT>* temp = a->right;
                this->rotate(a->right, true);
                temp->meta = calculate_bf(temp);
                this->rotate(a,false);
            }
        }
    }

    void balance(Node_t<keyT, valT>* a){
        if (a->meta > 1){
            if (height_subtree(a->left->left) >= height_subtree(a->left->right))
                this->rotate(a, true);
            else{
                Node_t<keyT, valT>* temp = a->left;
                this->rotate(a->left, false);
                temp->meta = calculate_bf(temp);
                this->rotate(a,true);
            }
        }
        else if (a->meta < -1){
            
            if (height_subtree(a->right->right) >= height_subtree(a->right->left))
                this->rotate(a, false);
            else{
                Node_t<keyT, valT>* temp = a->right;
                this->rotate(a->right, true);
                temp->meta = calculate_bf(temp);
                this->rotate(a,false);
            }
        }
    }

    Node_t<keyT, valT>* insert_internal(keyT key, valT value) {
        // Helper function for insert()
        Node_t<keyT, valT> *new_node = Tree_t<keyT,valT>::insert_internal(key, value);
        Node_t<keyT, valT> *node = new_node;
        while (node){   
            node->meta = calculate_bf(node);
            if (node->meta > 1 || node->meta < -1){
                balance(node, new_node);
                continue;
            }
            node = node->parent;
        }
        return new_node;
    }

    void remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
        Tree_t<keyT, valT>::remove_node(n);

    }

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // we create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // we update the value, rather than making a new one.
        insert_internal(key,value);
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        if(!Tree_t<keyT, valT>::root) return false;
            Node_t<keyT, valT>* n = this->search_subtree(Tree_t<keyT, valT>::root, key);
            if (n && n->key == key) {

                Node_t<keyT, valT>* node_parent = n->parent;
                
                if(n->left && n->right) {
                Node_t<keyT, valT>* r = this->get_min(n->right);
                node_parent = r->parent;
                }

                remove_node(n);
                if (node_parent){
                    Node_t<keyT, valT>* temp = node_parent;
                    while (temp){   
                        temp->meta = calculate_bf(temp);
                        if (temp->meta > 1 || temp->meta < -1){
                            balance(temp);
                            continue;
                        }
                        temp = temp->parent;
                    }

                }
                return true;
            }

        else return false;
        
    }
};
