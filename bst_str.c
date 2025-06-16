/*============================================================================*
*                             FILE: bst_str.c                                 *
*            Simple binary search tree implementation for strings             *
*        Insertion, Deletion, In-order, Pre-order, Post-order Traversal       *
*   Source: https://www.geeksforgeeks.org/binary-search-tree-data-structure/  *
*============================================================================*/

/** 
 * used "const char" instead of just "char" for safety, read-only, and compiler optimisation 
 * strdup: malloc + string copying, return pointer to the newly allocated string 
 * strcpy: no malloc */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// BST node structure for strings
typedef struct string_node {
    char *data;
    struct string_node *left;
    struct string_node *right;
} string_node_t;

int get_string_height(string_node_t *root);
void print_string_level(string_node_t *root, int level);
bool contains_string(string_node_t *root, const char *data); 
string_node_t *create_string_node(const char *data);
string_node_t *insert_string(string_node_t *root, const char *data);
string_node_t *find_min_string(string_node_t *root);
string_node_t *find_max_string(string_node_t *root);
string_node_t *search_string(string_node_t *root, const char *data);
string_node_t *delete_string(string_node_t *root, const char *data);
void inorder_string_traversal(string_node_t *root);
void preorder_string_traversal(string_node_t *root);
void postorder_string_traversal(string_node_t *root);
void level_order_string_traversal(string_node_t *root);
int get_string_height(string_node_t *root);
void print_string_level(string_node_t *root, int level);
void print_string_tree(string_node_t *root, int space);
void free_string_tree(string_node_t *root);
int count_nodes(string_node_t *root);
bool contains_string(string_node_t *root, const char *data);
void get_sorted_strings(string_node_t *root, char **result, int *index);


// Create a new string node
string_node_t *create_string_node(const char *data) {
    string_node_t *new_node = (string_node_t *)malloc(sizeof(string_node_t));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    
    // Allocate memory for string and copy
    new_node->data = (char *)malloc(strlen(data) + 1);
    if (new_node->data == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        free(new_node);
        exit(1);
    }
    
    strcpy(new_node->data, data);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Insert a string into BST (lexicographic comparison)
string_node_t *insert_string(string_node_t *root, const char *data) {
    // Base case: empty tree
    if (root == NULL) {
        return create_string_node(data);
    }
    
    // Compare strings lexicographically
    int cmp = strcmp(data, root->data);
    
    if (cmp < 0) {
        root->left = insert_string(root->left, data);
    } else if (cmp > 0) {
        root->right = insert_string(root->right, data);
    }
    // If cmp == 0, ignore duplicates
    
    return root;
}

// Find minimum string node (leftmost node)
string_node_t *find_min_string(string_node_t *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Find maximum string node (rightmost node)
string_node_t *find_max_string(string_node_t *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

// Search for a string in BST
string_node_t *search_string(string_node_t *root, const char *data) {
    // Base case: empty tree or found
    if (root == NULL) {
        return NULL;
    }
    
    int cmp = strcmp(data, root->data);
    
    if (cmp == 0) {
        return root;  // Found
    } else if (cmp < 0) {
        return search_string(root->left, data);
    } else {
        return search_string(root->right, data);
    }
}

// Delete a string node from BST
string_node_t *delete_string(string_node_t *root, const char *data) {
    // Base case: empty tree
    if (root == NULL) {
        return root;
    }
    
    int cmp = strcmp(data, root->data);
    
    // Find the node to delete
    if (cmp < 0) {
        root->left = delete_string(root->left, data);
    } else if (cmp > 0) {
        root->right = delete_string(root->right, data);
    } else {
        // Node to be deleted found
        
        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root->data);
            free(root);
            return NULL;
        }
        
        // Case 2: Node has one child
        else if (root->left == NULL) {
            string_node_t *temp = root->right;
            free(root->data);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            string_node_t *temp = root->left;
            free(root->data);
            free(root);
            return temp;
        }
        
        // Case 3: Node has two children
        else {
            // Find inorder successor (smallest in right subtree)
            string_node_t *temp = find_min_string(root->right);
            
            // Free current data and copy successor's data
            free(root->data);
            root->data = (char *)malloc(strlen(temp->data) + 1);
            strcpy(root->data, temp->data);
            
            // Delete the inorder successor
            root->right = delete_string(root->right, temp->data);
        }
    }
    return root;
}

// STRING BST TRAVERSAL METHODS

// In-order traversal: Left -> Root -> Right (gives alphabetically sorted order)
void inorder_string_traversal(string_node_t *root) {
    if (root != NULL) {
        inorder_string_traversal(root->left);
        printf("\"%s\" ", root->data);
        inorder_string_traversal(root->right);
    }
}

// Pre-order traversal: Root -> Left -> Right
void preorder_string_traversal(string_node_t *root) {
    if (root != NULL) {
        printf("\"%s\" ", root->data);
        preorder_string_traversal(root->left);
        preorder_string_traversal(root->right);
    }
}

// Post-order traversal: Left -> Right -> Root
void postorder_string_traversal(string_node_t *root) {
    if (root != NULL) {
        postorder_string_traversal(root->left);
        postorder_string_traversal(root->right);
        printf("\"%s\" ", root->data);
    }
}

// Level-order (breadth-first) traversal
void level_order_string_traversal(string_node_t *root) {
    if (root == NULL) return;
    
    int height = get_string_height(root);
    for (int i = 1; i <= height; i++) {
        print_string_level(root, i);
    }
}

// Helper function to get height of string tree
int get_string_height(string_node_t *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height = get_string_height(root->left);
    int right_height = get_string_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

// Helper function to print strings at a given level
void print_string_level(string_node_t *root, int level) {
    if (root == NULL) return;
    if (level == 1) {
        printf("\"%s\" ", root->data);
    } else if (level > 1) {
        print_string_level(root->left, level - 1);
        print_string_level(root->right, level - 1);
    }
}

// Print tree structure (visual representation)
void print_string_tree(string_node_t *root, int space) {
    const int COUNT = 15;
    if (root == NULL) return;
    
    space += COUNT;
    
    // Print right child first
    print_string_tree(root->right, space);
    
    // Print current node
    printf("\n");
    for (int i = COUNT; i < space; i++) {
        printf(" ");
    }
    printf("%s\n", root->data);
    
    // Print left child
    print_string_tree(root->left, space);
}

// Free all nodes in the string tree
void free_string_tree(string_node_t *root) {
    if (root != NULL) {
        free_string_tree(root->left);
        free_string_tree(root->right);
        free(root->data);  // Free the string
        free(root);        // Free the node
    }
}

// Count total nodes in tree
int count_nodes(string_node_t *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Check if tree contains a specific string
bool contains_string(string_node_t *root, const char *data) {
    return search_string(root, data) != NULL;
}

// Get all strings in alphabetical order
void get_sorted_strings(string_node_t *root, char **result, int *index) {
    if (root != NULL) {
        get_sorted_strings(root->left, result, index);
        result[*index] = (char *)malloc(strlen(root->data) + 1);
        strcpy(result[*index], root->data);
        (*index)++;
        get_sorted_strings(root->right, result, index);
    }
}

// MAIN FUNCTION - DEMONSTRATION
int main(int argc, char *argv[]) {
    string_node_t *root = NULL;
    
    // Sample exam strings + additional examples
    const char *strings[] = {
        "apple", "banana", "pear", "watermelon", "cherry", 
        "algorithms are fun", "zebra", "cat", "dog", "elephant"
    };
    int n = sizeof(strings) / sizeof(strings[0]);
    
    // Insert strings
    printf("=== STRING BST INSERTION ===\n");
    for (int i = 0; i < n; i++) {
        root = insert_string(root, strings[i]);
        printf("Inserted: \"%s\"\n", strings[i]);
    }
    
    // Print tree structure
    printf("\n=== TREE STRUCTURE ===\n");
    print_string_tree(root, 0);
    
    // Demonstrate traversals
    printf("\n=== TRAVERSALS ===\n");
    printf("In-order (alphabetical):   ");
    inorder_string_traversal(root);
    printf("\n");
    
    printf("Pre-order:                 ");
    preorder_string_traversal(root);
    printf("\n");
    
    printf("Post-order:                ");
    postorder_string_traversal(root);
    printf("\n");
    
    printf("Level-order:               ");
    level_order_string_traversal(root);
    printf("\n");
    
    // Demonstrate search
    printf("\n=== SEARCH OPERATIONS ===\n");
    const char *search_strings[] = {"apple", "xyz", "cherry", "algorithms are fun"};
    for (int i = 0; i < 4; i++) {
        string_node_t *found = search_string(root, search_strings[i]);
        printf("Search \"%s\": %s\n", search_strings[i], 
               found ? "Found" : "Not Found");
    }
    
    // Demonstrate min/max
    printf("\n=== MIN/MAX ===\n");
    string_node_t *min_node = find_min_string(root);
    string_node_t *max_node = find_max_string(root);
    printf("Alphabetically first: \"%s\"\n", min_node ? min_node->data : "None");
    printf("Alphabetically last:  \"%s\"\n", max_node ? max_node->data : "None");
    
    // Show count
    printf("\nTotal nodes: %d\n", count_nodes(root));
    
    // Demonstrate deletion
    printf("\n=== DELETION ===\n");
    const char *delete_strings[] = {"cat", "banana", "apple"}; // leaf, one child, two children
    
    for (int i = 0; i < 3; i++) {
        printf("Before deleting \"%s\": ", delete_strings[i]);
        inorder_string_traversal(root);
        printf("\n");
        
        root = delete_string(root, delete_strings[i]);
        
        printf("After deleting \"%s\":  ", delete_strings[i]);
        inorder_string_traversal(root);
        printf("\n\n");
    }
    
    // Get sorted array of remaining strings
    printf("=== SORTED STRING ARRAY ===\n");
    int remaining_count = count_nodes(root);
    char **sorted_strings = (char **)malloc(remaining_count * sizeof(char *));
    int index = 0;
    get_sorted_strings(root, sorted_strings, &index);
    
    printf("Remaining strings in alphabetical order:\n");
    for (int i = 0; i < remaining_count; i++) {
        printf("%d. \"%s\"\n", i + 1, sorted_strings[i]);
        free(sorted_strings[i]);  // Free individual strings
    }
    free(sorted_strings);  // Free array
    
    // Clean up memory
    free_string_tree(root);
    
    return 0;
}
