#ifndef AVL_H
#define AVL_H

#include <vector>
#include <algorithm>
#include "voter.h"
using namespace std;

/* ------------- [avl.h] Self-Balancing AVL Voter Ledger ------------- */
struct AVLNode
{
    Voter v;
    AVLNode *left = nullptr;
    AVLNode *right = nullptr;
    int height = 1;
    AVLNode(Voter val) : v(val) {}
};

class VoterAVL
{
    AVLNode *root = nullptr;
    int height(AVLNode *n) { return n ? n->height : 0; }
    int balanceFactor(AVLNode *n) { return n ? height(n->left) - height(n->right) : 0; }
    AVLNode *rotateRight(AVLNode *y)
    {
        AVLNode *x = y->left;
        y->left = x->right;
        x->right = y;
        y->height = 1 + max(height(y->left), height(y->right));
        x->height = 1 + max(height(x->left), height(x->right));
        return x;
    }
    AVLNode *rotateLeft(AVLNode *x)
    {
        AVLNode *y = x->right;
        x->right = y->left;
        y->left = x;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }
    AVLNode *insert(AVLNode *node, Voter v)
    {
        if (!node)
            return new AVLNode(v);
        if (v.voterId < node->v.voterId)
            node->left = insert(node->left, v);
        else if (v.voterId > node->v.voterId)
            node->right = insert(node->right, v);
        else
            return node;
        node->height = 1 + max(height(node->left), height(node->right));
        int bf = balanceFactor(node);
        if (bf > 1 && v.voterId < node->left->v.voterId)
            return rotateRight(node);
        if (bf < -1 && v.voterId > node->right->v.voterId)
            return rotateLeft(node);
        if (bf > 1 && v.voterId > node->left->v.voterId)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1 && v.voterId < node->right->v.voterId)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    AVLNode *search(AVLNode *node, int id)
    {
        if (!node || node->v.voterId == id)
            return node;
        return id < node->v.voterId ? search(node->left, id) : search(node->right, id);
    }
    void inorder(AVLNode *node, vector<Voter> &out)
    {
        if (!node)
            return;
        inorder(node->left, out);
        out.push_back(node->v);
        inorder(node->right, out);
    }

public:
    void insert(Voter v) { root = insert(root, v); }
    Voter *find(int id)
    {
        AVLNode *n = search(root, id);
        return n ? &n->v : nullptr;
    }
    vector<Voter> getAll()
    {
        vector<Voter> out;
        inorder(root, out);
        return out;
    }
};

#endif
