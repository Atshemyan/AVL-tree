#include "TreeNode.hpp"
#include <iostream>
#include <initializer_list>

template <typename T>
class AVL
{
private:
    TreeNode<T> *root {};

    //helpers
    TreeNode<T>* helperInsert(TreeNode<T>*, T);
    bool helperSearch(TreeNode<T>*, T);
    void levelorderTraversalRecursive(TreeNode<T>*, int, std::function<void(T&)>);
    TreeNode<T>* leftRotate(TreeNode<T>*);
    TreeNode<T>* rightRotate(TreeNode<T>*);
    int getBalanceFactor(TreeNode<T>*);
    TreeNode<T>* helperRemove(TreeNode <T>*, T);

public:
    AVL() = default;
    AVL(const std::initializer_list<T>&);

    TreeNode<T> *getRoot() const;
    void insert(T);
    bool search(T);

    void preorderTraversal(TreeNode<T> *node, std::function<void(T&)>);
    void inorderTraversal(TreeNode<T> *node, std::function<void(T&)>);
    void postorderTraversal(TreeNode<T> *node, std::function<void(T&)>);
    void levelorderTraversal(TreeNode<T> *node, std::function<void(T&)>);
    void removeKey(T);

    int getHeight(TreeNode<T> *node);
    TreeNode<T>* getMin(TreeNode<T> *node);
    TreeNode<T>* getMax(TreeNode<T> *node);
    TreeNode<T>* getSuccessor(TreeNode<T> *node);
    TreeNode<T>* getPredecessor(TreeNode<T> *node);
};

template <typename T>
AVL<T>::AVL(const std::initializer_list<T>& l)
{
    for (T i : l)
    {
        insert(i);
    }
}

template <typename T>
TreeNode<T>* AVL<T>::getRoot() const
{
    return root;
}

template <typename T>
void AVL<T>::insert(T val)
{
    root = helperInsert(root, val);
}

template <typename T>
bool AVL<T>::search(T val)
{
    return helperSearch(root, val);
}

template <typename T>
TreeNode<T>* AVL<T>::getMax(TreeNode<T>* node)
{
    if (!node || !node->right)
    {
        return node;
    }
    return getMax(node->right);
}

template <typename T>
TreeNode<T>* AVL<T>::getMin(TreeNode<T>* node)
{
    if (!node ||!node->left)
    {
        return node;
    }
    return getMin(node->left);
}

template <typename T>
void AVL<T>::removeKey(T key)
{
    helperRemove(root, key);
}


template <typename T>
TreeNode<T>* AVL<T>::getSuccessor(TreeNode<T> *node)
{
    if (!node) return nullptr;
    if (node->right) return getMin(node->right);

    TreeNode<T> *succesor {};
    TreeNode<T> *ancestor {getRoot()};
    while (ancestor != node)
    {
        if (node->val < ancestor->val)
        {
            succesor = ancestor;
            ancestor = ancestor->left;
        }
        else
        {
            ancestor = ancestor->right;
        }
    }
    return succesor;
    
}

template <typename T>
TreeNode<T>* AVL<T>::getPredecessor(TreeNode<T> *node) // from minimums get maximum
{
    if (!node) return nullptr;
    if (node->left) return getMax(node->left);

    TreeNode<T> *predecessor {};
    TreeNode<T> *ancestor {getRoot()};
    while (ancestor != node)
    {
        predecessor = ancestor;
        ancestor = ancestor->left;
        
    }
    return predecessor;
}



template <typename T>
    int AVL<T>::getHeight(TreeNode<T> *node)
    {
        if (!node) {return -1;}

        return std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }


template <typename T>
void AVL<T>::levelorderTraversal(TreeNode<T> *node, std::function<void(T&)> functor)
{ 
    int height = getHeight(getRoot());
    for (int i = 0; i <= height; ++i)
    {
        levelorderTraversalRecursive(node, i, functor);
    }
   

}

template <typename T>
void AVL<T>::preorderTraversal(TreeNode<T> *node, std::function<void(T&)> functor)
{ 
    if (!node) {return;}
    functor(node->val);
    preorderTraversal(node->left, functor);
    preorderTraversal(node->right, functor);

}

template <typename T>
void AVL<T>::inorderTraversal(TreeNode<T> *node, std::function<void(T&)> functor)
{
    if (!node) {return;}
    inorderTraversal(node->left, functor);
    functor(node->val);
    inorderTraversal(node->right, functor);

}

template <typename T>
void AVL<T>::postorderTraversal(TreeNode<T> *node, std::function<void(T&)> functor)
{
    if (!node) {return;}
    preorderTraversal(node->left, functor);
    preorderTraversal(node->right, functor);
    functor(node->val);
}

//helpers realizations
 
template <typename T>
int AVL<T>::getBalanceFactor(TreeNode<T> *node)
{
    return getHeight(node->left) - getHeight(node->right);
}
template <typename T>
TreeNode<T>* AVL<T>::helperRemove(TreeNode <T>* node, T key)
{
    if (!node) return nullptr;
    if (key > node->val)
    {
        node->right = helperRemove(node->right, key);
    }
    else if (key < node->val)
    {
        node->left = helperRemove(node->left, key);
    }
    else
    {
        if (!node->left)
        {
            TreeNode<T> *tmp = node->right;
            delete node;
            return tmp;
        }
        if (!node->right)
        {
            TreeNode<T> *tmp = node->left;
            delete node;
            return tmp;
        }

        //if we have 2 childs

        TreeNode<T>* tmpNode = getMax(node->left);
        node->val = tmpNode->val;
        helperRemove(node->left, tmpNode->val);
    }

    if (getBalanceFactor(node) > 1 && getBalanceFactor(node->left) >= 0) //LL
    {
        return rightRotate(node);
    }

    if (getBalanceFactor(node) > 1 && getBalanceFactor(node->left) < 0) //LR
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (getBalanceFactor(node) < -1 && getBalanceFactor(node->right) <= 0) //RR
    {
        return leftRotate(node);
    }

    if (getBalanceFactor(node) < -1 && getBalanceFactor(node->right) > 0) //RL
    {
        return leftRotate(node);
    }
    return node;
}

template <typename T>
TreeNode<T> *AVL<T>::helperInsert(TreeNode<T>* node, T val)
{
    if (!node) {return new TreeNode<T>(val);}

    if (val > node->val)
    {
        node->right = helperInsert(node->right, val);
    }
    else
    {
        node->left = helperInsert(node->left, val);
    }

    int bf = getBalanceFactor(node);

    //checking cases  RR RL LL LR
    if (bf < -1 && val < node->right->val)  //RL
    {
        std::cout << "LOG: RL case rotating \n";
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    if (bf < -1 && val > node->right->val) //RR
    {
        std::cout << "LOG: RR case rotating \n";
        return leftRotate(node);
    }

    if (bf > 1 && val > node->left->val) //LR
    {
        std::cout << "LOG: LR case rotating \n";
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (bf > 1 && val < node->left->val) //LL
    {
        std::cout << "LOG: LL case rotating \n";
        return rightRotate(node);
    }
    return node;
}

template <typename T>
bool AVL<T>::helperSearch(TreeNode<T> *node, T val)
{
    if (!node) {return false;}
    if (node->val == val)
    {
        return true;
    }

    if (val > node->val)
    {
        return helperSearch(node->right, val);
    }

    else
    {
        return helperSearch(node->left, val);
    }
    
}

template <typename T>
TreeNode<T>* AVL<T>::leftRotate(TreeNode<T> *z)
{
    TreeNode<T> *y = z->right;
    z->right = y->left;
    y->left = z;
    return y;
}

template <typename T>
TreeNode<T>* AVL<T>::rightRotate(TreeNode<T> *z)
{
    TreeNode<T> *y = z->left;
    z->left = y->right;
    y->right = z;
    return y;
}


template <typename T>
void AVL<T>::levelorderTraversalRecursive(TreeNode<T> *node, int level, std::function<void(T&)> functor)
{
    if (!node || level < 0)
        return;

    if (level == 0)
    {
        functor(node->val);
    }
    else
    {
        levelorderTraversalRecursive(node->left, level - 1, functor);
        levelorderTraversalRecursive(node->right, level - 1, functor);
    }
}