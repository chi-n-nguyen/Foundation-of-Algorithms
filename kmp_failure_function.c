#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to construct the failure function array using F[0] = -1 convention
// This follows the approach from the sample exam for COMP10002
void construct_failure_function(char *pattern, int pattern_length, int *failure_array) {
    
    // Set the first value to -1 as per failure function convention
    // This handles the special case when mismatch occurs at first character
    failure_array[0] = -1;
    
    // For single character patterns, return early
    if (pattern_length == 1) {
        return;
    }
    
    // Second value is always 0 since single character has no proper prefix/suffix
    failure_array[1] = 0;
    
    // Start processing from the third character (index 2)
    int current_index = 2;
    int prefix_length = 0; // Length of current prefix being considered
    
    while (current_index < pattern_length) {
        
        // If characters match, we found a longer prefix-suffix match
        if (pattern[current_index - 1] == pattern[prefix_length]) {
            prefix_length++;
            failure_array[current_index] = prefix_length;
            current_index++;
        }
        // If there is a mismatch between characters
        else {
            if (prefix_length > 0) {
                // Use failure function to find next candidate prefix length
                // This avoids redundant comparisons by utilizing previously computed information
                prefix_length = failure_array[prefix_length];
                // Note: We don't increment current_index here to recheck the same character
            }
            else {
                // No matching prefix found, set failure value to 0
                // and move to next character
                failure_array[current_index] = 0;
                current_index++;
            }
        }
    }
}

// Modified KMP search function using failure function approach
// Returns dynamically allocated array of all match positions
int* kmp_search_with_failure_function(char *pattern, char *text, int *result_count) {
    int text_length = strlen(text);
    int pattern_length = strlen(pattern);
    
    // Initialize result count to 0
    *result_count = 0;

    // Allocate memory for failure function array
    int *failure_array = (int *)malloc(sizeof(int) * pattern_length);
    if (failure_array == NULL) {
        printf("Memory allocation failed for failure function array\n");
        return NULL;
    }

    // Preprocess the pattern to build failure function array
    construct_failure_function(pattern, pattern_length, failure_array);

    // Allocate initial memory for results array (will resize if needed)
    int results_capacity = 10; // Initial capacity
    int *results_array = (int *)malloc(sizeof(int) * results_capacity);
    if (results_array == NULL) {
        printf("Memory allocation failed for results array\n");
        free(failure_array);
        return NULL;
    }

    // text_index: pointer for traversing the text string
    // pattern_index: pointer for traversing the pattern string
    int text_index = 0;
    int pattern_index = 0;

    // Continue searching until we've examined the entire text
    while (text_index < text_length) {

        // If current characters match, advance both pointers
        if (text[text_index] == pattern[pattern_index]) {
            text_index++;
            pattern_index++;

            // If we've matched the entire pattern, we found an occurrence
            if (pattern_index == pattern_length) {
                // Calculate the starting position of the match
                int match_position = text_index - pattern_index;
                
                // Resize results array if needed
                if (*result_count >= results_capacity) {
                    results_capacity *= 2; // Double the capacity
                    int *temp = (int *)realloc(results_array, sizeof(int) * results_capacity);
                    if (temp == NULL) {
                        printf("Memory reallocation failed\n");
                        free(failure_array);
                        free(results_array);
                        return NULL;
                    }
                    results_array = temp;
                }
                
                // Store the match position and increment count
                results_array[*result_count] = match_position;
                (*result_count)++;

                // Use failure function to determine how much we can skip
                // Handle the special case where pattern_index equals pattern_length
                if (pattern_length > 1) {
                    pattern_index = failure_array[pattern_index - 1];
                } else {
                    pattern_index = 0;
                }
            }
        }
        // If there is a mismatch between current characters
        else {
            // If we had some partial match, use failure function to skip redundant comparisons
            if (pattern_index > 0) {
                pattern_index = failure_array[pattern_index - 1];
            }
            // If no partial match (or F[0] = -1 case), advance text pointer
            else {
                text_index++;
            }
        }
    }

    // Clean up failure function array memory
    free(failure_array);

    // Resize results array to exact size needed (optional optimization)
    if (*result_count > 0 && *result_count < results_capacity) {
        int *final_results = (int *)realloc(results_array, sizeof(int) * (*result_count));
        if (final_results != NULL) {
            results_array = final_results;
        }
    }

    return results_array;
}

// Helper function to print failure function array for debugging
void print_failure_function(char *pattern, int *failure_array, int pattern_length) {
    printf("Pattern: ");
    for (int i = 0; i < pattern_length; i++) {
        printf("%c ", pattern[i]);
    }
    printf("\n");
    
    printf("F values: ");
    for (int i = 0; i < pattern_length; i++) {
        printf("%d ", failure_array[i]);
    }
    printf("\n");
}

// Main function to demonstrate the failure function approach
int main() {
    // Sample text and pattern for testing (from sample exam)
    char *text = "aabaacaadaabaaba";
    char *pattern = "aaba";

    printf("Text: %s\n", text);
    printf("Pattern: %s\n", pattern);
    
    // Test with the sample exam pattern as well
    char *exam_pattern = "apple's app store";
    int *exam_failure = (int *)malloc(sizeof(int) * strlen(exam_pattern));
    construct_failure_function(exam_pattern, strlen(exam_pattern), exam_failure);
    
    printf("\nSample exam pattern analysis:\n");
    print_failure_function(exam_pattern, exam_failure, strlen(exam_pattern));
    free(exam_failure);
    
    printf("\nSearch results:\n");
    int match_count = 0;
    int *match_positions = kmp_search_with_failure_function(pattern, text, &match_count);
    
    if (match_positions != NULL && match_count > 0) {
        printf("Pattern found at %d position(s):\n", match_count);
        for (int i = 0; i < match_count; i++) {
            printf("Position %d: %d\n", i + 1, match_positions[i]);
        }
        
        // Clean up dynamically allocated memory
        free(match_positions);
    } else {
        printf("Pattern not found in text\n");
    }

    return 0;
}


































































































































































































































