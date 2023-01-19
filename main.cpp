#include <algorithm>

struct Node {
 public:
    explicit Node(int value) {
        this->value = value;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
    Node() {
        height = 0;
        left = nullptr;
        right = nullptr;
        value = 0;
    }
    int height;
    Node *left;
    Node *right;
    int value;
};

class AVLTree {
 public:
    AVLTree() {
        size_ = 0;
        root_ = nullptr;
    }

    int getHeight() {
        return getHeight(root_);
    }

    int getHeight(Node *node) {
        if (node == nullptr) {
            return 0;
        }
        auto left_tree_height = getHeight(node->left);
        auto right_tree_height = getHeight(node->right);
        return std::max(left_tree_height, right_tree_height) + 1;
    }

    static int calculateHeight(Node *node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    Node *leftRotate(Node *node) {
        Node *right_child = node->right;
        Node *left_child_from_right = right_child->left;
        // bila pizda
        right_child->left = node;
        node->right = left_child_from_right;  // swap
        auto left_tree_height = calculateHeight(node->left);
        auto right_tree_height = calculateHeight(node->right);
        node->height = std::max(left_tree_height, right_tree_height) + 1;  // recalculate
        left_tree_height = calculateHeight(right_child->left);
        right_tree_height = calculateHeight(right_child->right);
        right_child->height = std::max(left_tree_height, right_tree_height) + 1;  // recalculate
        return right_child;
    }

    // if right subtree null check
    Node *rightRotate(Node *node) {
        Node *left_child = node->left;
        Node *right_child_from_left = left_child->right;
        left_child->right = node;            // bil krash
        node->left = right_child_from_left;  // swap
        auto left_tree_height = calculateHeight(node->left);
        auto right_tree_height = calculateHeight(node->right);
        node->height = std::max(left_tree_height, right_tree_height) + 1;  // recalc
        left_tree_height = calculateHeight(left_child->left);
        right_tree_height = calculateHeight(left_child->right);
        left_child->height = std::max(left_tree_height, right_tree_height) + 1;  // recalc
        return left_child;
    }

    Node *insert(Node *node, int value) {
        if (node == nullptr) {  // the root is null
            auto new_node = new Node(value);
            root_ = new_node;
            size_++;
            return root_;
        }
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node;
        }
        // пересчитаем balance factors и если надо сбалансируем поворотами для поддер
        auto left_tree_height = calculateHeight(node->left);
        auto right_tree_height = calculateHeight(node->right);
        node->height = std::max(left_tree_height, right_tree_height) + 1;
        int balance_factor = findBalanceFactor(node);
        if (balance_factor > 1) {
            if (value < node->left->value) {
                return rightRotate(node);
            } else if (value > node->left->value) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        if (balance_factor < -1) {
            if (value > node->right->value) {
                return leftRotate(node);
            } else if (value < node->right->value) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        return node;
    }

    static int findBalanceFactor(Node *node) {
        if (node == nullptr) {
            return 0;
        }
        auto left = calculateHeight(node->left);
        auto right = calculateHeight(node->right);
        return left - right;
    }

    void insert(int value) {
        root_ = insert(root_, value);
        // size_++;
    }

    Node *findMinimum(Node *node) {
        Node *cur_min = node;
        while (cur_min->left != nullptr) {
            cur_min = cur_min->left;
        }
        return cur_min;
    }

    Node *erase(Node *node, int value) {
        if (node == nullptr) {
            return node;
        }
        if (value < node->value) {
            node->left = erase(node->left, value);
        } else if (value > node->value) {
            node->right = erase(node->right, value);
        } else {
            if ((node->left == nullptr) || (node->right == nullptr)) {
                Node *copy = (node->left ? node->left : node->right);
                if (copy != nullptr) {
                    *node = *copy;
                } else {
                    copy = node;
                    node = nullptr;
                }
                size_--;
                delete copy;
            } else {
                Node *copy = findMinimum(node->right);
                node->value = copy->value;
                node->right = erase(node->right, copy->value);
            }
        }
        if (node == nullptr) {
            return node;
        }
        // пересчитаем balance factors и если надо сбалансируем поворотами для поддер
        auto left_tree_height = calculateHeight(node->left);
        auto right_tree_height = calculateHeight(node->right);
        node->height = std::max(left_tree_height, right_tree_height) + 1;
        int balance_factor = findBalanceFactor(node);
        if (balance_factor < -1) {
            if (findBalanceFactor(node->right) <= 0) {
                return leftRotate(node);
            } else {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        if (balance_factor > 1) {
            if (findBalanceFactor(node->left) >= 0) {
                return rightRotate(node);
            } else {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        return node;
    }

    void erase(int value) {
        root_ = erase(root_, value);
    }

    int *find(Node *node, int value) {
        if (node == nullptr) {
            return nullptr;
        }
        if (value > node->value) {
            return find(node->right, value);
        } else if (value < node->value) {
            return find(node->left, value);
        } else if (value == node->value) {
            return &node->value;
        }
        return nullptr;
    }

    int *find(int value) {
        return find(root_, value);
    }

    void traversal(size_t &pos, Node *node, int *arr) {
        if (node) {
            traversal(pos, node->left, arr);
            arr[pos++] = node->value;
            traversal(pos, node->right, arr);
        } else {
            return;
        }
    }

    int *traversal() {
        int *arr = new int[size_];  // массив вершин
        size_t pos = 0;
        traversal(pos, root_, arr);
        return arr;
    }

    int *lowerBound(int value) {
        Node *cur = root_;
        Node *res = nullptr;
        while (cur != nullptr) {
            if (cur->value >= value) {
                res = cur;
                cur = cur->left;
            } else {
                cur = cur->right;  // sdvig
            }
        }
        return (res ? &res->value : nullptr);
    }

    bool empty() {
        return size_ == 0;  // hehe
    }

    Node *getRoot() {
        return root_;  // hehe
    }

    int getSize() {
        return size_;
    }

    ~AVLTree() {
        deleteNode(root_);
    }

 private:
    void deleteNode(Node *node) {
        if (node != nullptr) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
        }
    }

    int size_;
    Node *root_;
};