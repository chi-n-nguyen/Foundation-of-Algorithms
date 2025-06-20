
/*
Singly Linked List: Insertion, Deletion, Traversal, & Memory Allocation 
Source: https://www.geeksforgeeks.org/singly-linked-list-tutorial/
*/

#include <stdio.h>
#include <stdlib.h> // for malloc

// Forward-declare the alias so we can use node_t inside the struct
typedef struct node node_t;

// Node structure for singly linked list
struct node {
    int     data;  // Stores the actual data
    node_t *next;  // Points to the next node
};

// List structure to maintain head and foot pointers
typedef struct {
    node_t *head;        // Points to first node
    node_t *foot;        // Points to last node
    int     total_nodes; // Keeps track of list size
} list_t;

// Create and initialize an empty list
list_t *make_empty_list(void) {
    list_t *list = malloc(sizeof *list);
    if (!list) {
        printf("Memory allocation failed for list\n");
        return NULL;
    }
    list->head = NULL;
    list->foot = NULL;
    list->total_nodes = 0;
    return list;
}

// Insert node at the head — O(1)
void insert_at_head(list_t *list, int value) {
    if (!list) {
        printf("Cannot insert into NULL list\n");
        return;
    }
    node_t *n = malloc(sizeof *n);
    if (!n) {
        printf("Memory allocation failed for node\n");
        return;
    }
    n->data = value;
    n->next = list->head;
    list->head = n;
    if (!list->foot)
        list->foot = n;
    list->total_nodes++;
    printf("Inserted %d at head successfully\n", value);
}

// Insert node at the foot — O(1) with tail pointer
void insert_at_foot(list_t *list, int value) {
    if (!list) {
        printf("Cannot insert into NULL list\n");
        return;
    }
    node_t *n = malloc(sizeof *n);
    if (!n) {
        printf("Memory allocation failed for node\n");
        return;
    }
    n->data = value;
    n->next = NULL;
    if (!list->head) {
        list->head = n;
        list->foot = n;
    } else {
        list->foot->next = n;
        list->foot = n;
    }
    list->total_nodes++;
    printf("Inserted %d at foot successfully\n", value);
}

// Delete first occurrence of specified value — O(n)
int delete_by_value(list_t *list, int value) {
    if (!list || !list->head) {
        printf("Cannot delete from empty or NULL list\n");
        return 0;
    }
    node_t *cur  = list->head;
    node_t *prev = NULL;
    while (cur && cur->data != value) {
        prev = cur;
        cur  = cur->next;
    }
    if (!cur) {
        printf("Value %d not found in list\n", value);
        return 0;
    }
    if (!prev) {
        list->head = cur->next;
        if (!list->head)
            list->foot = NULL;
    } else {
        prev->next = cur->next;
        if (cur == list->foot)
            list->foot = prev;
    }
    free(cur);
    list->total_nodes--;
    printf("Deleted %d successfully\n", value);
    return 1;
}

// Traverse and print all nodes in the list — O(n)
void traverse_and_print(list_t *list) {
    if (!list) {
        printf("Cannot traverse NULL list\n");
        return;
    }
    if (!list->head) {
        printf("List is empty (no nodes to display)\n");
        return;
    }
    printf("List contents (%d nodes): ", list->total_nodes);
    for (node_t *cur = list->head; cur; cur = cur->next) {
        printf("%d", cur->data);
        if (cur->next)
            printf(" -> ");
    }
    printf(" -> NULL\n");
}
