#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    if(lower_than == NULL ) return NULL;
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;

    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new; 
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL) return;

    TreeNode *newNode = createTreeNode(key, value);
    if (newNode == NULL) return;

    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        return;
    }

    TreeNode *cur = tree->root;
    TreeNode *parent = NULL;

    while (cur != NULL) {
        parent = cur;

        if (is_equal(tree, key, cur->pair->key)) {
            free(newNode->pair);
            free(newNode);
            tree->current = cur;
            return;
        }

        if (tree->lower_than(key, cur->pair->key))
            cur = cur->left;
        else
            cur = cur->right;
    }

    newNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key))
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL) x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
if (tree == NULL || node == NULL) return;
    if (node->left == NULL || node->right == NULL) {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        TreeNode *parent = node->parent;
        if (parent == NULL) {
            tree->root = child;
        } else {
            if (parent->left == node) parent->left = child;
            else parent->right = child;
        }
        if (child != NULL) child->parent = parent;

        if (node->pair) free(node->pair);
        free(node);

        tree->current = (child != NULL) ? child : parent;
        return;
    }

    TreeNode *succ = minimum(node->right);
    Pair *tmp = node->pair;
    node->pair = succ->pair;
    succ->pair = tmp;
    removeNode(tree, succ);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || key == NULL) return NULL;
    TreeNode *cur = tree->root;
    TreeNode *last = NULL;

    while (cur != NULL) {
        last = cur;
        if (is_equal(tree, key, cur->pair->key)) {
            tree->current = cur;
            return cur->pair;
        }

        if (tree->lower_than(key, cur->pair->key))
            cur = cur->left;
        else
            cur = cur->right;
    }
    tree->current = last;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *M = minimum(tree->root);
    tree->current = M;
    return (M != NULL) ? M->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
