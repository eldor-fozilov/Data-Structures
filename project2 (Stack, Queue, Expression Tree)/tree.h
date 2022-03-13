#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"

using namespace std;

/*
 * Recursive binary tree structure for building expression tree
 *
 * Constructors: Tree(char, Tree*, Tree*) and Tree(string, Tree*, Tree*)
 *      pointers will be automatically set to NULL if omitted
 *
 * Destructor: recursively destruct whole tree
 *
 */


struct Tree {
    string expr;
    Tree* left;
    Tree* right;

    explicit Tree(char h, Tree* l = NULL, Tree* r = NULL) {
        expr = string(1, h);
        left = l;
        right = r;
    }

    explicit Tree(const string& h = string(), Tree* l = NULL, Tree* r = NULL) {
        expr = h;
        left = l;
        right = r;
    }

    ~Tree() {
        delete left;
        delete right;
    }

    int size() {
        int size = 1;
        if (left != NULL) { size += left->size(); }
        if (right != NULL) { size += right->size(); }
        return size;
    }

    void print();

    double eval();
};

// Implementation


Tree* root = 0;

void Tree::print() {

    Queue<Tree*> q;
    string s = "(";
    if (root == 0){
        s += ')';
        cout << s << '\n' ;
        return;
    }
    q.push(root);
    int size = root->size();

    while (!q.empty() && size > 0) {
        Tree* currentnode = q.front();

        if(currentnode == NULL) 
            s += "null";
        else{
            s += currentnode->expr;
            size--;
        }

        if (currentnode && currentnode->left)
            q.push(currentnode->left);

        if (currentnode && currentnode->right)
            q.push(currentnode->right);

        if(currentnode && !currentnode->right) q.push(NULL);
        if(currentnode && !currentnode->left) q.push(NULL);
        q.pop();
        if(size > 0) s += ',';
    }

    while (!q.empty())
    {
        q.pop();
    }
    
    cout << s << ")\n";

}

double compute(double num1, double num2,  const string& oper){
   
    if (oper == "*")
        return num1 * num2;
    else if (oper == "/")
        return num1 / num2;
    else if (oper == "-")
        return num1 - num2;
    else
        return num1 + num2;

}

double calc(Tree* node){
    
    double num;
    if (node && stringstream(node->expr) >> num)
        return num;
    
    if (node && !(stringstream(node->expr) >> num)){
        double left_num = calc(node->left);
        double right_num = calc(node->right);
        return compute(left_num, right_num, node->expr);
    }

    return 0;

}


double Tree::eval() {
    return calc(root);
}

Tree* build_expression_tree(const string& postfix) {
    
    Stack<Tree*> S;
    stringstream str;
    
    if (postfix == "")
        return root;
    
    str << postfix;

    string s;
    double num;

    while (!str.eof()) {
        Tree* newnode = new Tree;
        str >> s;
        newnode->expr = s;

        if (stringstream(s) >> num) {
            S.push(newnode);
        }
        else {
            newnode->right = S.top();
            S.pop();
            newnode->left = S.top();
            S.pop();
            S.push(newnode);            
        }
        
        if (str.eof())
            root = newnode;
        
    }

    return root;
}

#endif //TREE_H
