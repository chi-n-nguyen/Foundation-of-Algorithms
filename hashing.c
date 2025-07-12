#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* =================================================================
 * COMPREHENSIVE HASHING TECHNIQUES DEMONSTRATION
 * 
 * This program implements three different hashing techniques:
 * 1. Separate Chaining (Hashing with Chaining)
 * 2. Open Addressing with Linear Probing (with Tombstone Deletion)
 * 3. Open Addressing with Quadratic Probing (with Tombstone Deletion)
 * 
 * Each technique handles collisions differently and demonstrates
 * various trade-offs in performance, memory usage, and implementation complexity.
 * ================================================================= */

#define HASH_TABLE_SIZE 11  // Prime number for better hash distribution
#define EMPTY_SLOT -1       // Marker for empty slots in open addressing
#define DELETED_SLOT -2     // Marker for deleted slots (tombstone)

/* =================================================================
 * TECHNIQUE 1: SEPARATE CHAINING
 * 
 * In separate chaining, each bucket contains a linked list of all
 * elements that hash to that bucket. This allows unlimited collisions
 * and provides simple insertion, deletion, and search operations.
 * ================================================================= */

/* Node structure for separate chaining hash table */
typedef struct chained_hash_node {
    int key_value;                        // The key being stored
    int associated_data;                  // Data associated with the key
    struct chained_hash_node *next_node;  // Pointer to next node in chain
} chained_hash_node_t;

/* Hash table structure for separate chaining */
typedef struct chained_hash_table {
    chained_hash_node_t **bucket_array;  // Array of pointers to linked lists
    int total_buckets;                    // Number of buckets in the table
    int total_elements;                   // Total number of elements stored
    double load_factor;                   // Current load factor (n/m)
} chained_hash_table_t;

/* =================================================================
 * TECHNIQUE 2 & 3: OPEN ADDRESSING WITH TOMBSTONE DELETION
 * 
 * Open addressing stores all elements directly in the hash table array.
 * When collisions occur, we probe for the next available slot.
 * Tombstone deletion uses lazy deletion to mark deleted slots.
 * ================================================================= */

/* Entry structure for open addressing hash tables */
typedef struct open_addressing_entry {
    int key_value;        // The key being stored
    int associated_data;  // Data associated with the key
    bool is_occupied;     // Flag indicating if slot is occupied
    bool is_tombstone;    // Flag for tombstone deletion (lazy deletion)
} open_addressing_entry_t;

/* Hash table structure for open addressing methods */
typedef struct open_addressing_table {
    open_addressing_entry_t *entry_array;  // Array of hash table entries
    int total_slots;                        // Total number of slots
    int occupied_slots;                     // Number of occupied slots
    int tombstone_slots;                    // Number of tombstone slots
    double load_factor;                     // Current load factor
} open_addressing_table_t;

/* =================================================================
 * UNIVERSAL HASH FUNCTION
 * 
 * Division method hash function that works for all techniques.
 * Handles negative keys and ensures good distribution.
 * ================================================================= */

int calculate_hash_index(int key_value, int table_size) {
    /* Ensure positive result even for negative keys using modular arithmetic */
    return (key_value % table_size + table_size) % table_size;
}

/* Calculate and display hash distribution statistics */
void display_hash_statistics(int *hash_counts, int table_size, int total_elements) {
    printf("Hash Distribution Statistics:\n");
    for (int i = 0; i < table_size; i++) {
        printf("  Index %2d: %d elements\n", i, hash_counts[i]);
    }
    printf("  Average elements per bucket: %.2f\n", (double)total_elements / table_size);
}

/* =================================================================
 * SEPARATE CHAINING IMPLEMENTATION
 * ================================================================= */

/* Create a new separate chaining hash table */
chained_hash_table_t* create_chained_hash_table(int number_of_buckets) {
    chained_hash_table_t *new_table = malloc(sizeof(chained_hash_table_t));
    if (new_table == NULL) {
        printf("ERROR: Memory allocation failed for chained hash table\n");
        return NULL;
    }
    
    new_table->bucket_array = calloc(number_of_buckets, sizeof(chained_hash_node_t*));
    if (new_table->bucket_array == NULL) {
        printf("ERROR: Memory allocation failed for bucket array\n");
        free(new_table);
        return NULL;
    }
    
    new_table->total_buckets = number_of_buckets;
    new_table->total_elements = 0;
    new_table->load_factor = 0.0;
    
    return new_table;
}

/* Insert a key-value pair into separate chaining hash table */
bool insert_into_chained_table(chained_hash_table_t *hash_table, int key_value, int data_value) {
    if (hash_table == NULL) {
        printf("ERROR: Hash table is NULL\n");
        return false;
    }
    
    int bucket_index = calculate_hash_index(key_value, hash_table->total_buckets);
    
    /* Check if key already exists in the chain */
    chained_hash_node_t *current_node = hash_table->bucket_array[bucket_index];
    while (current_node != NULL) {
        if (current_node->key_value == key_value) {
            /* Key exists, update the data */
            current_node->associated_data = data_value;
            printf("  Updated existing key %d with new data %d\n", key_value, data_value);
            return true;
        }
        current_node = current_node->next_node;
    }
    
    /* Create new node and insert at head of chain for O(1) insertion */
    chained_hash_node_t *new_node = malloc(sizeof(chained_hash_node_t));
    if (new_node == NULL) {
        printf("ERROR: Memory allocation failed for new node\n");
        return false;
    }
    
    new_node->key_value = key_value;
    new_node->associated_data = data_value;
    new_node->next_node = hash_table->bucket_array[bucket_index];
    hash_table->bucket_array[bucket_index] = new_node;
    hash_table->total_elements++;
    
    /* Update load factor */
    hash_table->load_factor = (double)hash_table->total_elements / hash_table->total_buckets;
    
    return true;
}

/* Search for a key in separate chaining hash table */
bool search_in_chained_table(chained_hash_table_t *hash_table, int key_value, int *found_data) {
    if (hash_table == NULL) return false;
    
    int bucket_index = calculate_hash_index(key_value, hash_table->total_buckets);
    chained_hash_node_t *current_node = hash_table->bucket_array[bucket_index];
    
    int probe_count = 0;
    /* Traverse the chain looking for the key */
    while (current_node != NULL) {
        probe_count++;
        if (current_node->key_value == key_value) {
            if (found_data != NULL) {
                *found_data = current_node->associated_data;
            }
            printf("  Found key %d after %d probes in chain\n", key_value, probe_count);
            return true;
        }
        current_node = current_node->next_node;
    }
    
    printf("  Key %d not found after %d probes in chain\n", key_value, probe_count);
    return false;
}

/* Delete a key from separate chaining hash table */
bool delete_from_chained_table(chained_hash_table_t *hash_table, int key_value) {
    if (hash_table == NULL) return false;
    
    int bucket_index = calculate_hash_index(key_value, hash_table->total_buckets);
    chained_hash_node_t *current_node = hash_table->bucket_array[bucket_index];
    chained_hash_node_t *previous_node = NULL;
    
    /* Search for the node to delete */
    while (current_node != NULL) {
        if (current_node->key_value == key_value) {
            /* Found the node to delete */
            if (previous_node == NULL) {
                /* Deleting the head of the chain */
                hash_table->bucket_array[bucket_index] = current_node->next_node;
            } else {
                /* Deleting a middle or tail node */
                previous_node->next_node = current_node->next_node;
            }
            
            free(current_node);
            hash_table->total_elements--;
            
            /* Update load factor */
            hash_table->load_factor = (double)hash_table->total_elements / hash_table->total_buckets;
            
            printf("  Successfully deleted key %d from chain\n", key_value);
            return true;
        }
        
        previous_node = current_node;
        current_node = current_node->next_node;
    }
    
    printf("  Key %d not found for deletion in chain\n", key_value);
    return false;
}

/* Display the separate chaining hash table */
void display_chained_table(chained_hash_table_t *hash_table) {
    if (hash_table == NULL) return;
    
    printf("\n=== SEPARATE CHAINING HASH TABLE ===\n");
    printf("Total buckets: %d, Total elements: %d, Load factor: %.2f\n", 
           hash_table->total_buckets, hash_table->total_elements, hash_table->load_factor);
    
    int hash_counts[HASH_TABLE_SIZE] = {0};
    
    for (int bucket_index = 0; bucket_index < hash_table->total_buckets; bucket_index++) {
        printf("Bucket[%2d]: ", bucket_index);
        
        chained_hash_node_t *current_node = hash_table->bucket_array[bucket_index];
        if (current_node == NULL) {
            printf("EMPTY\n");
        } else {
            int chain_length = 0;
            while (current_node != NULL) {
                printf("(%d,%d)", current_node->key_value, current_node->associated_data);
                current_node = current_node->next_node;
                chain_length++;
                if (current_node != NULL) {
                    printf(" -> ");
                }
            }
            printf(" -> NULL [Chain length: %d]\n", chain_length);
            hash_counts[bucket_index] = chain_length;
        }
    }
    
    display_hash_statistics(hash_counts, hash_table->total_buckets, hash_table->total_elements);
}

/* Free memory used by separate chaining hash table */
void free_chained_table(chained_hash_table_t *hash_table) {
    if (hash_table == NULL) return;
    
    for (int bucket_index = 0; bucket_index < hash_table->total_buckets; bucket_index++) {
        chained_hash_node_t *current_node = hash_table->bucket_array[bucket_index];
        while (current_node != NULL) {
            chained_hash_node_t *node_to_delete = current_node;
            current_node = current_node->next_node;
            free(node_to_delete);
        }
    }
    
    free(hash_table->bucket_array);
    free(hash_table);
}

/* =================================================================
 * LINEAR PROBING WITH TOMBSTONE DELETION
 * 
 * Linear probing resolves collisions by checking consecutive slots.
 * Tombstone deletion marks deleted slots to maintain probe sequences.
 * Formula: h(k, i) = (h(k) + i) mod m
 * ================================================================= */

/* Create a new linear probing hash table */
open_addressing_table_t* create_linear_probing_table(int table_size) {
    open_addressing_table_t *new_table = malloc(sizeof(open_addressing_table_t));
    if (new_table == NULL) {
        printf("ERROR: Memory allocation failed for linear probing table\n");
        return NULL;
    }
    
    new_table->entry_array = calloc(table_size, sizeof(open_addressing_entry_t));
    if (new_table->entry_array == NULL) {
        printf("ERROR: Memory allocation failed for entry array\n");
        free(new_table);
        return NULL;
    }
    
    new_table->total_slots = table_size;
    new_table->occupied_slots = 0;
    new_table->tombstone_slots = 0;
    new_table->load_factor = 0.0;
    
    return new_table;
}

/* Insert using linear probing with tombstone awareness */
bool insert_linear_probing(open_addressing_table_t *hash_table, int key_value, int data_value) {
    if (hash_table == NULL) return false;
    
    /* Check if table is effectively full (occupied + tombstones) */
    if (hash_table->occupied_slots + hash_table->tombstone_slots >= hash_table->total_slots) {
        printf("ERROR: Hash table is full, cannot insert key %d\n", key_value);
        return false;
    }
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int current_probe_index = original_index;
    int probe_count = 0;
    int first_tombstone_index = -1;
    
    /* Linear probing: check consecutive slots */
    while (probe_count < hash_table->total_slots) {
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            /* Use first available slot (prefer tombstone if found) */
            int insert_index = (first_tombstone_index != -1) ? first_tombstone_index : current_probe_index;
            open_addressing_entry_t *insert_entry = &hash_table->entry_array[insert_index];
            
            insert_entry->key_value = key_value;
            insert_entry->associated_data = data_value;
            insert_entry->is_occupied = true;
            
            if (insert_entry->is_tombstone) {
                insert_entry->is_tombstone = false;
                hash_table->tombstone_slots--;
            }
            
            hash_table->occupied_slots++;
            hash_table->load_factor = (double)hash_table->occupied_slots / hash_table->total_slots;
            
            printf("  Inserted key %d at index %d after %d probes\n", key_value, insert_index, probe_count + 1);
            return true;
        }
        
        /* Found tombstone slot - remember first one */
        if (current_entry->is_tombstone && first_tombstone_index == -1) {
            first_tombstone_index = current_probe_index;
        }
        
        /* Key already exists and is not deleted */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            current_entry->associated_data = data_value;
            printf("  Updated existing key %d with new data %d\n", key_value, data_value);
            return true;
        }
        
        /* Move to next slot (linear probing) */
        current_probe_index = (current_probe_index + 1) % hash_table->total_slots;
        probe_count++;
    }
    
    printf("ERROR: Could not insert key %d after probing all slots\n", key_value);
    return false;
}

/* Search using linear probing with tombstone awareness */
bool search_linear_probing(open_addressing_table_t *hash_table, int key_value, int *found_data) {
    if (hash_table == NULL) return false;
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int current_probe_index = original_index;
    int probe_count = 0;
    
    /* Linear probing search */
    while (probe_count < hash_table->total_slots) {
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot that was never occupied - key doesn't exist */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("  Key %d not found after %d probes (empty slot reached)\n", key_value, probe_count + 1);
            return false;
        }
        
        /* Found the key and it's not a tombstone */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            if (found_data != NULL) {
                *found_data = current_entry->associated_data;
            }
            printf("  Found key %d at index %d after %d probes\n", key_value, current_probe_index, probe_count + 1);
            return true;
        }
        
        /* Move to next slot */
        current_probe_index = (current_probe_index + 1) % hash_table->total_slots;
        probe_count++;
    }
    
    printf("  Key %d not found after %d probes (table fully searched)\n", key_value, probe_count);
    return false;
}

/* Delete using linear probing with tombstone deletion */
bool delete_linear_probing(open_addressing_table_t *hash_table, int key_value) {
    if (hash_table == NULL) return false;
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int current_probe_index = original_index;
    int probe_count = 0;
    
    /* Linear probing search for deletion */
    while (probe_count < hash_table->total_slots) {
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot - key doesn't exist */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("  Key %d not found for deletion (empty slot reached)\n", key_value);
            return false;
        }
        
        /* Found the key and it's not already a tombstone */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            
            /* Mark as tombstone (lazy deletion) */
            current_entry->is_tombstone = true;
            hash_table->occupied_slots--;
            hash_table->tombstone_slots++;
            hash_table->load_factor = (double)hash_table->occupied_slots / hash_table->total_slots;
            
            printf("  Successfully marked key %d as tombstone at index %d\n", key_value, current_probe_index);
            return true;
        }
        
        /* Move to next slot */
        current_probe_index = (current_probe_index + 1) % hash_table->total_slots;
        probe_count++;
    }
    
    printf("  Key %d not found for deletion after full search\n", key_value);
    return false;
}

/* =================================================================
 * QUADRATIC PROBING WITH TOMBSTONE DELETION
 * 
 * Quadratic probing reduces primary clustering by using quadratic intervals.
 * Tombstone deletion maintains probe sequences for deleted elements.
 * Formula: h(k, i) = (h(k) + i²) mod m
 * ================================================================= */

/* Create a new quadratic probing hash table */
open_addressing_table_t* create_quadratic_probing_table(int table_size) {
    /* Same structure as linear probing, different probing strategy */
    return create_linear_probing_table(table_size);
}

/* Insert using quadratic probing with tombstone awareness */
bool insert_quadratic_probing(open_addressing_table_t *hash_table, int key_value, int data_value) {
    if (hash_table == NULL) return false;
    
    /* Check if table is effectively full */
    if (hash_table->occupied_slots + hash_table->tombstone_slots >= hash_table->total_slots) {
        printf("ERROR: Hash table is full, cannot insert key %d\n", key_value);
        return false;
    }
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int probe_attempt = 0;
    int first_tombstone_index = -1;
    
    /* Quadratic probing: check slots at quadratic intervals */
    while (probe_attempt < hash_table->total_slots) {
        int quadratic_offset = probe_attempt * probe_attempt;
        int current_probe_index = (original_index + quadratic_offset) % hash_table->total_slots;
        
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            /* Use first available slot (prefer tombstone if found) */
            int insert_index = (first_tombstone_index != -1) ? first_tombstone_index : current_probe_index;
            open_addressing_entry_t *insert_entry = &hash_table->entry_array[insert_index];
            
            insert_entry->key_value = key_value;
            insert_entry->associated_data = data_value;
            insert_entry->is_occupied = true;
            
            if (insert_entry->is_tombstone) {
                insert_entry->is_tombstone = false;
                hash_table->tombstone_slots--;
            }
            
            hash_table->occupied_slots++;
            hash_table->load_factor = (double)hash_table->occupied_slots / hash_table->total_slots;
            
            printf("  Inserted key %d at index %d after %d quadratic probes\n", key_value, insert_index, probe_attempt + 1);
            return true;
        }
        
        /* Found tombstone slot - remember first one */
        if (current_entry->is_tombstone && first_tombstone_index == -1) {
            first_tombstone_index = current_probe_index;
        }
        
        /* Key already exists and is not deleted */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            current_entry->associated_data = data_value;
            printf("  Updated existing key %d with new data %d\n", key_value, data_value);
            return true;
        }
        
        probe_attempt++;
    }
    
    printf("ERROR: Could not insert key %d after quadratic probing\n", key_value);
    return false;
}

/* Search using quadratic probing with tombstone awareness */
bool search_quadratic_probing(open_addressing_table_t *hash_table, int key_value, int *found_data) {
    if (hash_table == NULL) return false;
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int probe_attempt = 0;
    
    /* Quadratic probing search */
    while (probe_attempt < hash_table->total_slots) {
        int quadratic_offset = probe_attempt * probe_attempt;
        int current_probe_index = (original_index + quadratic_offset) % hash_table->total_slots;
        
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot that was never occupied - key doesn't exist */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("  Key %d not found after %d quadratic probes (empty slot reached)\n", key_value, probe_attempt + 1);
            return false;
        }
        
        /* Found the key and it's not a tombstone */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            if (found_data != NULL) {
                *found_data = current_entry->associated_data;
            }
            printf("  Found key %d at index %d after %d quadratic probes\n", key_value, current_probe_index, probe_attempt + 1);
            return true;
        }
        
        probe_attempt++;
    }
    
    printf("  Key %d not found after %d quadratic probes (table fully searched)\n", key_value, probe_attempt);
    return false;
}

/* Delete using quadratic probing with tombstone deletion */
bool delete_quadratic_probing(open_addressing_table_t *hash_table, int key_value) {
    if (hash_table == NULL) return false;
    
    int original_index = calculate_hash_index(key_value, hash_table->total_slots);
    int probe_attempt = 0;
    
    /* Quadratic probing search for deletion */
    while (probe_attempt < hash_table->total_slots) {
        int quadratic_offset = probe_attempt * probe_attempt;
        int current_probe_index = (original_index + quadratic_offset) % hash_table->total_slots;
        
        open_addressing_entry_t *current_entry = &hash_table->entry_array[current_probe_index];
        
        /* Found empty slot - key doesn't exist */
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("  Key %d not found for deletion (empty slot reached)\n", key_value);
            return false;
        }
        
        /* Found the key and it's not already a tombstone */
        if (current_entry->is_occupied && 
            current_entry->key_value == key_value && 
            !current_entry->is_tombstone) {
            
            /* Mark as tombstone (lazy deletion) */
            current_entry->is_tombstone = true;
            hash_table->occupied_slots--;
            hash_table->tombstone_slots++;
            hash_table->load_factor = (double)hash_table->occupied_slots / hash_table->total_slots;
            
            printf("  Successfully marked key %d as tombstone at index %d\n", key_value, current_probe_index);
            return true;
        }
        
        probe_attempt++;
    }
    
    printf("  Key %d not found for deletion after full quadratic search\n", key_value);
    return false;
}

/* =================================================================
 * DISPLAY FUNCTIONS FOR OPEN ADDRESSING TABLES
 * ================================================================= */

/* Display linear probing hash table with tombstone information */
void display_linear_probing_table(open_addressing_table_t *hash_table) {
    if (hash_table == NULL) return;
    
    printf("\n=== LINEAR PROBING HASH TABLE (with Tombstone Deletion) ===\n");
    printf("Total slots: %d, Occupied: %d, Tombstones: %d, Load factor: %.2f\n", 
           hash_table->total_slots, hash_table->occupied_slots, 
           hash_table->tombstone_slots, hash_table->load_factor);
    
    for (int slot_index = 0; slot_index < hash_table->total_slots; slot_index++) {
        open_addressing_entry_t *current_entry = &hash_table->entry_array[slot_index];
        
        printf("Slot[%2d]: ", slot_index);
        
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("EMPTY\n");
        } else if (current_entry->is_tombstone) {
            printf("TOMBSTONE (was key=%d, data=%d)\n", 
                   current_entry->key_value, current_entry->associated_data);
        } else {
            printf("OCCUPIED (%d,%d)\n", current_entry->key_value, current_entry->associated_data);
        }
    }
}

/* Display quadratic probing hash table with tombstone information */
void display_quadratic_probing_table(open_addressing_table_t *hash_table) {
    if (hash_table == NULL) return;
    
    printf("\n=== QUADRATIC PROBING HASH TABLE (with Tombstone Deletion) ===\n");
    printf("Total slots: %d, Occupied: %d, Tombstones: %d, Load factor: %.2f\n", 
           hash_table->total_slots, hash_table->occupied_slots, 
           hash_table->tombstone_slots, hash_table->load_factor);
    
    for (int slot_index = 0; slot_index < hash_table->total_slots; slot_index++) {
        open_addressing_entry_t *current_entry = &hash_table->entry_array[slot_index];
        
        printf("Slot[%2d]: ", slot_index);
        
        if (!current_entry->is_occupied && !current_entry->is_tombstone) {
            printf("EMPTY\n");
        } else if (current_entry->is_tombstone) {
            printf("TOMBSTONE (was key=%d, data=%d)\n", 
                   current_entry->key_value, current_entry->associated_data);
        } else {
            printf("OCCUPIED (%d,%d)\n", current_entry->key_value, current_entry->associated_data);
        }
    }
}

/* Free memory used by open addressing tables */
void free_open_addressing_table(open_addressing_table_t *hash_table) {
    if (hash_table == NULL) return;
    
    free(hash_table->entry_array);
    free(hash_table);
}

/* =================================================================
 * COMPREHENSIVE TESTING AND DEMONSTRATION
 * ================================================================= */

/* Test all three hashing techniques with comprehensive scenarios */
void demonstrate_all_hashing_techniques() {
    printf("=================================================================\n");
    printf("COMPREHENSIVE HASHING TECHNIQUES DEMONSTRATION\n");
    printf("Featuring: Separate Chaining, Linear Probing, Quadratic Probing\n");
    printf("All with proper collision handling and tombstone deletion\n");
    printf("=================================================================\n");
    
    /* Test data: keys and their associated values */
    int test_keys[] = {10, 22, 31, 4, 15, 28, 17, 88, 59, 23, 46, 13, 35};
    int test_values[] = {100, 220, 310, 40, 150, 280, 170, 880, 590, 230, 460, 130, 350};
    int number_of_test_items = sizeof(test_keys) / sizeof(test_keys[0]);
    
    printf("Test dataset (%d items): ", number_of_test_items);
    for (int i = 0; i < number_of_test_items; i++) {
        printf("(%d,%d) ", test_keys[i], test_values[i]);
    }
    printf("\n");
    
    /* =================================================================
     * TEST SEPARATE CHAINING
     * ================================================================= */
    
    printf("\n=== TESTING SEPARATE CHAINING ===\n");
    chained_hash_table_t *chained_table = create_chained_hash_table(HASH_TABLE_SIZE);
    
    if (chained_table == NULL) {
        printf("ERROR: Failed to create chained hash table\n");
        return;
    }
    
    /* Insert all test data */
    printf("\n--- Insertion Phase ---\n");
    for (int i = 0; i < number_of_test_items; i++) {
        bool insertion_success = insert_into_chained_table(chained_table, test_keys[i], test_values[i]);
        printf("Insert key %d: %s\n", test_keys[i], insertion_success ? "SUCCESS" : "FAILED");
    }
    
    display_chained_table(chained_table);
    
    /* Test search operations */
    printf("\n--- Search Phase ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_in_chained_table(chained_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* Test deletion */
    printf("\n--- Deletion Phase ---\n");
    for (int i = 1; i < 4; i++) {
        bool deletion_result = delete_from_chained_table(chained_table, test_keys[i]);
        printf("Delete key %d: %s\n", test_keys[i], deletion_result ? "SUCCESS" : "FAILED");
    }
    
    display_chained_table(chained_table);
    
    /* Test search after deletion */
    printf("\n--- Search After Deletion ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_in_chained_table(chained_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* =================================================================
     * TEST LINEAR PROBING WITH TOMBSTONE DELETION
     * ================================================================= */
    
    printf("\n=== TESTING LINEAR PROBING WITH TOMBSTONE DELETION ===\n");
    open_addressing_table_t *linear_table = create_linear_probing_table(HASH_TABLE_SIZE);
    
    if (linear_table == NULL) {
        printf("ERROR: Failed to create linear probing table\n");
        free_chained_table(chained_table);
        return;
    }
    
    /* Insert all test data */
    printf("\n--- Insertion Phase ---\n");
    for (int i = 0; i < number_of_test_items; i++) {
        bool insertion_success = insert_linear_probing(linear_table, test_keys[i], test_values[i]);
        printf("Insert key %d: %s\n", test_keys[i], insertion_success ? "SUCCESS" : "FAILED");
    }
    
    display_linear_probing_table(linear_table);
    
    /* Test search operations */
    printf("\n--- Search Phase ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_linear_probing(linear_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* Test tombstone deletion */
    printf("\n--- Tombstone Deletion Phase ---\n");
    for (int i = 1; i < 4; i++) {
        bool deletion_result = delete_linear_probing(linear_table, test_keys[i]);
        printf("Delete key %d: %s\n", test_keys[i], deletion_result ? "SUCCESS" : "FAILED");
    }
    
    display_linear_probing_table(linear_table);
    
    /* Test search after tombstone deletion */
    printf("\n--- Search After Tombstone Deletion ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_linear_probing(linear_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* Test insertion into tombstone slots */
    printf("\n--- Insertion into Tombstone Slots ---\n");
    int new_keys[] = {99, 77, 55};
    int new_values[] = {990, 770, 550};
    for (int i = 0; i < 3; i++) {
        bool insertion_success = insert_linear_probing(linear_table, new_keys[i], new_values[i]);
        printf("Insert key %d: %s\n", new_keys[i], insertion_success ? "SUCCESS" : "FAILED");
    }
    
    display_linear_probing_table(linear_table);
    
    /* =================================================================
     * TEST QUADRATIC PROBING WITH TOMBSTONE DELETION
     * ================================================================= */
    
    printf("\n=== TESTING QUADRATIC PROBING WITH TOMBSTONE DELETION ===\n");
    open_addressing_table_t *quadratic_table = create_quadratic_probing_table(HASH_TABLE_SIZE);
    
    if (quadratic_table == NULL) {
        printf("ERROR: Failed to create quadratic probing table\n");
        free_chained_table(chained_table);
        free_open_addressing_table(linear_table);
        return;
    }
    
    /* Insert all test data */
    printf("\n--- Insertion Phase ---\n");
    for (int i = 0; i < number_of_test_items; i++) {
        bool insertion_success = insert_quadratic_probing(quadratic_table, test_keys[i], test_values[i]);
        printf("Insert key %d: %s\n", test_keys[i], insertion_success ? "SUCCESS" : "FAILED");
    }
    
    display_quadratic_probing_table(quadratic_table);
    
    /* Test search operations */
    printf("\n--- Search Phase ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_quadratic_probing(quadratic_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* Test tombstone deletion */
    printf("\n--- Tombstone Deletion Phase ---\n");
    for (int i = 1; i < 4; i++) {
        bool deletion_result = delete_quadratic_probing(quadratic_table, test_keys[i]);
        printf("Delete key %d: %s\n", test_keys[i], deletion_result ? "SUCCESS" : "FAILED");
    }
    
    display_quadratic_probing_table(quadratic_table);
    
    /* Test search after tombstone deletion */
    printf("\n--- Search After Tombstone Deletion ---\n");
    for (int i = 0; i < 5; i++) {
        int found_data;
        bool search_result = search_quadratic_probing(quadratic_table, test_keys[i], &found_data);
        printf("Search key %d: %s", test_keys[i], search_result ? "FOUND" : "NOT FOUND");
        if (search_result) {
            printf(" (data=%d)", found_data);
        }
        printf("\n");
    }
    
    /* Test insertion into tombstone slots */
    printf("\n--- Insertion into Tombstone Slots ---\n");
    for (int i = 0; i < 3; i++) {
        bool insertion_success = insert_quadratic_probing(quadratic_table, new_keys[i], new_values[i]);
        printf("Insert key %d: %s\n", new_keys[i], insertion_success ? "SUCCESS" : "FAILED");
    }
    
    display_quadratic_probing_table(quadratic_table);
    
    /* =================================================================
     * CLEANUP AND SUMMARY
     * ================================================================= */
    
    printf("\n--- Memory Cleanup ---\n");
    free_chained_table(chained_table);
    free_open_addressing_table(linear_table);
    free_open_addressing_table(quadratic_table);
    printf("All memory successfully freed.\n");
    
    printf("\n=================================================================\n");
    printf("DEMONSTRATION COMPLETED SUCCESSFULLY\n");
    printf("=================================================================\n");
    printf("Summary of Techniques Demonstrated:\n");
    printf("1. Separate Chaining: Linked lists handle unlimited collisions\n");
    printf("2. Linear Probing: Sequential search with tombstone deletion\n");
    printf("3. Quadratic Probing: Quadratic intervals reduce clustering\n");
    printf("=================================================================\n");
}

/* =================================================================
 * MAIN FUNCTION
 * ================================================================= */

int main() {
    printf("Starting comprehensive hashing techniques demonstration...\n");
    printf("This program demonstrates three collision resolution methods:\n");
    printf("- Separate Chaining (unlimited collisions via linked lists)\n");
    printf("- Linear Probing with Tombstone Deletion (sequential probing)\n");
    printf("- Quadratic Probing with Tombstone Deletion (quadratic intervals)\n\n");
    
    demonstrate_all_hashing_techniques();
    
    printf("\nProgram execution completed successfully.\n");
    return 0;
} 