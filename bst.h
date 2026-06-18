#ifndef BST_H
#define BST_H

#include <vector>
#include "voter.h"
using namespace std;

/* ---------------- [bst.h] Binary Search Tree Voter Ledger ---------------- */
struct BSTNode
{
    Voter v;
    BSTNode *left = nullptr;
    BSTNode *right = nullptr;
    BSTNode(Voter val) : v(val) {}
};

class VoterBST
{
    BSTNode *root = nullptr;
    BSTNode *insert(BSTNode *node, Voter v)
    {
        if (!node)
            return new BSTNode(v);
        if (v.voterId < node->v.voterId)
            node->left = insert(node->left, v);
        else if (v.voterId > node->v.voterId)
            node->right = insert(node->right, v);
        return node;
    }
    BSTNode *search(BSTNode *node, int id)
    {
        if (!node || node->v.voterId == id)
            return node;
        return id < node->v.voterId ? search(node->left, id) : search(node->right, id);
    }
    void inorder(BSTNode *node, vector<Voter> &out)
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
        BSTNode *n = search(root, id);
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
