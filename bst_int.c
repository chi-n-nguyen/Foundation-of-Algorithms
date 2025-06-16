/*============================================================================*
*                             FILE: bst_int.c                                 *
*            Simple binary search tree implementation for integers            *
*        Insertion, Deletion, In-order, Pre-order, Post-order Traversal       *
*   Source: https://www.geeksforgeeks.org/binary-search-tree-data-structure/  *
*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief stuct node_t representing a node storing integer data
 */
typedef struct node {
    int data;
    struct node *left;
    struct node *right;
} node_t;

/**
 * @brief allocate memory and initialise a new BST node
 * @param data the integer value to store in the node's data field
 * @return pointer to a heap-allocated node_t with its data set to data,
 *         both child pointers (L and R) initialised to NULL,
 *         or NULL if malloc fails
 * @note O(1)
*/
node_t *create_node(int data) {
    node_t *new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        return NULL;
    }

    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL; 

    return new_node; // ensure always return the newly allocated node 
}

/**
 * @brief insert a new integer into the BST
 *        recursively finds the correct position, if already present, no insertion is performed 
 * @param root pointer to the root of the BST (or subtree)
 * @param data integer value to insert
 * @return pointer to the root of the modified BST
 * @note O(log n) average, O(n) worst case
*/
node_t *insert(node_t *root, int data) {
    // base case
    if (root == NULL) {
        return create_node(data);
    }

    // recursive cases
    if (data < root->data) {
        root->left = insert(root->left, data);
        } else if (data > root->data) {
            root->right = insert(root->right, data);
    }

    // always return root!
    return root;
}

/** 
 * @brief search for a value in the BST
 * @param root pointer to the root of the BST (or subtree)
 * @param data integer value to search for
 * @return pointer to the node containing data, NULL if not found
 * @note O(log n) average, O(n) worst case
*/
node_t *search(node_t *root, int data) {
    // base case
    if (root == NULL || root->data == data) {
        return root;
    }

    // recursive case
    if (data < root->data) {
        return search(root->left, data);
    } else {
        return search(root->right, data);
    }
}

/**
 * @brief Find the node with the minimum value in a BST.
 * @param root Pointer to the root of the BST (or subtree).
 * @return Pointer to the node with the smallest data value, or NULL if tree is empty.
 * @note Time Complexity: O(h) where h is the tree height (O(log n) avg, O(n) worst)
 */
static node_t *find_min(node_t *root) {
    while (root && root->left) // while there is still a root (not NULL)
        root = root->left;
    return root;
}

/**
 * @brief delete a value from the BST
 *  Handle 3 cases:
 *  1. Node without children (leaf) -> simply free it
 *  2. Node with 1 child -> replace node with its child 
 *  3. Node with 2 children -> replace node's data with its in-order successor, 
 *     then deletes the successor node
 * @param root pointer to the root of the BST (or subtree)
 * @param data the integer value to delete
 * @return pointer to the root of the modified BST
 * @note O(log n) average, O(n) worst case
*/
node_t *delete_node(node_t *root, int data) {
    // base case: delete 
    if (root == NULL) {
        return NULL;
    }

    // recursive cases
    if (data < root->data) {
        root->left = delete_node(root->left, data); // go left if smol
    } else if (data > root->data) {
        root->right = delete_node(root->right, data); // go right if big
    } else {
        // case 1 & 2: 0 or 1 child
        if (root->left == NULL) { // no left child
            node_t *temp = root->right; // could be NULL (leaf) or single child
            free(root); // free this node
            return temp; // no further deletion here
        }

        if (root->right == NULL) {
            node_t *temp = root->left; // could be NULL (leaf) or single child
            free(root); // free this node
            return temp; // end recursion for this branch
        }

        // case 3: 2 children 
        node_t *successor = find_min(root->right);
        root->data = successor->data;
        root->right = delete_node(root->right, successor->data);
    }

    return root;
 }

 /**
 * @brief In-order traversal: left subtree, node, right subtree.
 * @param root Pointer to the root of the BST (or subtree).
 * @note Time Complexity: O(n)
 */
void inorder(node_t *root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

/**
 * @brief Pre-order traversal: node, left subtree, right subtree.
 * @param root Pointer to the root of the BST (or subtree).
 * @note Time Complexity: O(n)
 */
void preorder(node_t *root) {
    if (root) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

/**
 * @brief Post-order traversal: left subtree, right subtree, node.
 * @param root Pointer to the root of the BST (or subtree).
 * @note Time Complexity: O(n)
 */
void postorder(node_t *root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}