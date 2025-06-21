#include <stdio.h>
#include <stdlib.h>

// Node structure for doubly linked list
typedef struct node {
    int data;              // Stores the actual data
    struct node *next;     // Points to next node
    struct node *prev;     // Points to previous node
} node_t;

// List structure to maintain head and tail pointers
typedef struct {
    node_t *head;          // Points to first node
    node_t *tail;          // Points to last node
    int size;              // Keeps track of list size
} list_t;

// Create and initialize an empty doubly linked list
list_t* make_empty_list(void) {
    list_t *list = (list_t*)malloc(sizeof(list_t));
    
    if (list == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return list;
}

// Helper function to create a new node
node_t* create_node(int val) {
    node_t *node = (node_t*)malloc(sizeof(node_t));
    
    if (node == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    
    node->data = val;
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

// Insert node at the beginning - O(1)
void insert_head(list_t *list, int val) {
    if (list == NULL) {
        printf("Error: Cannot insert into NULL list\n");
        return;
    }
    
    node_t *node = create_node(val);
    if (node == NULL) {
        return;
    }
    
    // If list is empty
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    
    list->size++;
    printf("Inserted %d at head\n", val);
}

// Insert node at the end - O(1)
void insert_tail(list_t *list, int val) {
    if (list == NULL) {
        printf("Error: Cannot insert into NULL list\n");
        return;
    }
    
    node_t *node = create_node(val);
    if (node == NULL) {
        return;
    }
    
    // If list is empty
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    
    list->size++;
    printf("Inserted %d at tail\n", val);
}

// Insert node at specified position - O(n)
void insert_at(list_t *list, int val, int pos) {
    if (list == NULL) {
        printf("Error: Cannot insert into NULL list\n");
        return;
    }
    
    if (pos < 0 || pos > list->size) {
        printf("Error: Invalid position %d\n", pos);
        return;
    }
    
    // Handle insertion at head or tail
    if (pos == 0) {
        insert_head(list, val);
        return;
    }
    
    if (pos == list->size) {
        insert_tail(list, val);
        return;
    }
    
    node_t *node = create_node(val);
    if (node == NULL) {
        return;
    }
    
    // Find position (optimize by choosing shorter path)
    node_t *cur;
    int i;
    
    if (pos <= list->size / 2) {
        // Traverse from head
        cur = list->head;
        for (i = 0; i < pos; i++) {
            cur = cur->next;
        }
    } else {
        // Traverse from tail
        cur = list->tail;
        for (i = list->size - 1; i > pos; i--) {
            cur = cur->prev;
        }
    }
    
    // Insert before cur
    node->next = cur;
    node->prev = cur->prev;
    cur->prev->next = node;
    cur->prev = node;
    
    list->size++;
    printf("Inserted %d at position %d\n", val, pos);
}

// Delete from beginning - O(1)
void delete_head(list_t *list) {
    if (list == NULL || list->head == NULL) {
        printf("Error: Cannot delete from empty list\n");
        return;
    }
    
    node_t *node = list->head;
    int val = node->data;
    
    // If only one node
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = node->next;
        list->head->prev = NULL;
    }
    
    free(node);
    list->size--;
    printf("Deleted %d from head\n", val);
}

// Delete from end - O(1)
void delete_tail(list_t *list) {
    if (list == NULL || list->tail == NULL) {
        printf("Error: Cannot delete from empty list\n");
        return;
    }
    
    node_t *node = list->tail;
    int val = node->data;
    
    // If only one node
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = node->prev;
        list->tail->next = NULL;
    }
    
    free(node);
    list->size--;
    printf("Deleted %d from tail\n", val);
}

// Delete by value - O(n)
int delete_val(list_t *list, int val) {
    if (list == NULL || list->head == NULL) {
        printf("Error: Cannot delete from empty list\n");
        return 0;
    }
    
    node_t *cur = list->head;
    
    // Search for value
    while (cur != NULL && cur->data != val) {
        cur = cur->next;
    }
    
    // If not found
    if (cur == NULL) {
        printf("Value %d not found\n", val);
        return 0;
    }
    
    // Handle deletion based on position
    if (cur->prev == NULL) {
        delete_head(list);
    } else if (cur->next == NULL) {
        delete_tail(list);
    } else {
        // Delete middle node
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        free(cur);
        list->size--;
        printf("Deleted %d from middle\n", val);
    }
    
    return 1;
}

// Traverse forward - O(n)
void print_forward(list_t *list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    printf("Forward: ");
    node_t *cur = list->head;
    
    while (cur != NULL) {
        printf("%d", cur->data);
        if (cur->next != NULL) {
            printf(" <-> ");
        }
        cur = cur->next;
    }
    
    printf(" -> NULL (size: %d)\n", list->size);
}

// Traverse backward - O(n)
void print_backward(list_t *list) {
    if (list == NULL || list->tail == NULL) {
        printf("List is empty\n");
        return;
    }
    
    printf("Backward: ");
    node_t *cur = list->tail;
    
    while (cur != NULL) {
        printf("%d", cur->data);
        if (cur->prev != NULL) {
            printf(" <-> ");
        }
        cur = cur->prev;
    }
    
    printf(" -> NULL (size: %d)\n", list->size);
}

// Search for value - O(n)
int search(list_t *list, int val) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    
    node_t *cur = list->head;
    int pos = 0;
    
    while (cur != NULL) {
        if (cur->data == val) {
            printf("Value %d found at position %d\n", val, pos);
            return pos;
        }
        cur = cur->next;
        pos++;
    }
    
    printf("Value %d not found\n", val);
    return -1;
}

// Free entire list - O(n)
void free_list(list_t *list) {
    if (list == NULL) {
        return;
    }
    
    node_t *cur = list->head;
    node_t *next;
    int count = 0;
    
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
        count++;
    }
    
    free(list);
    printf("Freed list (%d nodes)\n", count);
}

// Demo function following sample exam style
int main(int argc, char *argv[]) {
    list_t *list = make_empty_list();
    
    if (list == NULL) {
        return 1;
    }
    
    printf("=== Doubly Linked List Demo ===\n");
    
    // Test insertions
    insert_head(list, 10);
    insert_head(list, 5);
    insert_tail(list, 15);
    insert_tail(list, 20);
    insert_at(list, 12, 2);
    
    // Test traversals
    print_forward(list);
    print_backward(list);
    
    // Test search
    search(list, 12);
    search(list, 99);
    
    // Test deletions
    delete_head(list);
    delete_tail(list);
    delete_val(list, 12);
    
    print_forward(list);
    
    // Cleanup
    free_list(list);
    
    return 0;
}
