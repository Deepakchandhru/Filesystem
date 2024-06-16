#include <iostream>
using namespace std;

struct Node_s {
    int key;
    string path;
    int freq;
    Node_s* left;
    Node_s* right;

    Node_s(int k,string p) : key(k), path(p), left(nullptr), right(nullptr), freq(1) {}
};

Node_s* rotateRight(Node_s* y) {
    Node_s* x = y->left;
    Node_s* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;
    return x;
}

// Helper function to perform a left rotation (Zag)
Node_s* rotateLeft(Node_s* x) {
    Node_s* y = x->right;
    Node_s* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;
    return y;
}

Node_s* splay(Node_s* root, int key, int freq) {
    if (root == nullptr || (root->key == key && ++root->freq)) {
        return root;
    }

    if ((root->freq < freq) || (root->freq == freq && key < root->key)) {
        if (root->left == nullptr)
            return root;

        if ((root->left->freq < freq) || (root->left->freq == freq && key < root->left->key)) {
            root->left->left = splay(root->left->left, key, freq);
            root = rotateRight(root);
        } else if ((root->left->freq < freq) || (root->left->freq == freq && key > root->left->key)) {
            root->left->right = splay(root->left->right, key, freq);
            if (root->left->right != nullptr)
                root->left = rotateLeft(root->left);
        }
        return (root->left == nullptr) ? root : rotateRight(root);
    } else {
        if (root->right == nullptr)
            return root;

        if ((root->right->freq < freq) || (root->right->freq == freq && key > root->right->key)) {
            root->right->right = splay(root->right->right, key, freq);
            root = rotateLeft(root);
        } else if ((root->right->freq < freq) || (root->right->freq == freq && key < root->right->key)) {
            root->right->left = splay(root->right->left, key, freq);
            if (root->right->left != nullptr)
                root->right = rotateRight(root->right);
        }
        return (root->right == nullptr) ? root : rotateLeft(root);
    }
}

// Helper function to insert a key into the splay tree
Node_s* insert(Node_s* root, int key,string path) {
    if (root == nullptr)
        return new Node_s(key,path);

    // Perform insertion as in a normal BST
    root = splay(root, key,root->freq);
    if (key < root->key) {
        Node_s* newNode = new Node_s(key,path);
        newNode->left = root->left;
        newNode->right = root;
        root->left = nullptr;
        root = newNode;
    }
    else if (key > root->key) {
        Node_s* newNode = new Node_s(key,path);
        newNode->right = root->right;
        newNode->left = root;
        root->right = nullptr;
        root = newNode;
    }
    return root;
}

// Helper function to search for a key in the splay tree
Node_s* search(Node_s* root, int key) {
    if (root == nullptr || root->key == key){
        int freq =0;
        if(root!=nullptr)
            freq = root->freq;
        return splay(root, key,freq);
    }

    if (key < root->key && root->left != nullptr)
        root->left = search(root->left, key);
    else if (key > root->key && root->right != nullptr)
        root->right = search(root->right, key);

    return splay(root, key,root->freq);
}
