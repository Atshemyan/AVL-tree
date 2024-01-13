template <typename T>
struct TreeNode {
    T val;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(T x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(T x, TreeNode<T>* left, TreeNode<T>* right) : val(x), left(left), right(right) {}
};