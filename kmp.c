#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

/**
 *  KMP algorithm, improved from the naive pattern search algorithm, time complexity O(m+n) 
 * 	Source: https://www.geeksforgeeks.org/dsa/kmp-algorithm-for-pattern-searching/ 
 * */

/* function to construct the LPS array */
void construct_lps_array(char *pattern, int pattern_length, int *lps_array) { // pointer of type int/array of ints
	int prefix_length = 0;
	lps_array[0] = 0; // always true

	int current_index = 1; // starting from the 2nd character

	while (current_index < pattern_length) {
		// if characrers match, we found a longer prefix suffix match
		if (pattern[current_index] == pattern[prefix_length]) {
			prefix_length++;
			lps_array[current_index] = prefix_length;
			current_index++;
		} else {  // if there is a mismatch between characters
			if (prefix_length != 0) {
				// update prefix_length to the previous LPS value to avoid redundant comparisons
				prefix_length = lps_array[prefix_length - 1];
			} else {
				// mo matching prefixing found, set LPS value to 0 
				// move on to the next character
				lps_array[current_index] = 0;
				current_index++;
			}
		}
	}
}

// modified KMP search function that returns an array of all match positions
// returns dynamically allocated array, and stores count in result_count parameter
int* kmp_search(char *pattern, char *text, int *result_count) {
    int text_length = strlen(text);
    int pattern_length = strlen(pattern);
    
    // initialise result count to 0
    *result_count = 0;

    // allocate memory for LPS array to store preprocessing results
    int *lps_array = (int *)malloc(sizeof(int) * pattern_length);
    if (lps_array == NULL) {
        printf("Memory allocation failed for LPS array\n");
        return NULL;
    }

    // preprocess the pattern to build LPS array
    construct_lps_array(pattern, pattern_length, lps_array);

    // allocate initial memory for results array (will resize if needed)
    int results_capacity = 10;  // initial capacity
    int *results_array = (int *)malloc(sizeof(int) * results_capacity);
    if (results_array == NULL) {
        printf("Memory allocation failed for results array\n");
        free(lps_array);
        return NULL;
    }

    // text_index: pointer for traversing the text string
    // pattern_index: pointer for traversing the pattern string
    int text_index = 0;
    int pattern_index = 0;

    // continue searching until we've examined the entire text
    while (text_index < text_length) {

        // if current characters match, advance both pointers
        if (text[text_index] == pattern[pattern_index]) {
            text_index++;
            pattern_index++;

            // if we've matched the entire pattern, we found an occurrence
            if (pattern_index == pattern_length) {
                // calculate the starting position of the match
                int match_position = text_index - pattern_index;
                
                // resize results array if needed
                if (*result_count >= results_capacity) {
                    results_capacity *= 2;  // double the capacity
                    int *temp = (int *)realloc(results_array, sizeof(int) * results_capacity);
                    if (temp == NULL) {
                        printf("Memory reallocation failed\n");
                        free(lps_array);
                        free(results_array);
                        return NULL;
                    }
                    results_array = temp;
                }
                
                // store the match position and increment count
                results_array[*result_count] = match_position;
                (*result_count)++;

                // use LPS value to determine how much we can skip
                // this avoids checking characters we know will match
                pattern_index = lps_array[pattern_index - 1];
            }
        }
        // if there is a mismatch between current characters
        else {
            // if we had some partial match, use LPS to skip redundant comparisons
            if (pattern_index != 0) {
                pattern_index = lps_array[pattern_index - 1];
            }
            // if no partial match, just advance text pointer
            else {
                text_index++;
            }
        }
    }

    // clean up LPS array memory
    free(lps_array);

    // resize results array to exact size needed (optional optimisation)
    if (*result_count > 0 && *result_count < results_capacity) {
        int *final_results = (int *)realloc(results_array, sizeof(int) * (*result_count));
        if (final_results != NULL) {
            results_array = final_results;
        }
    }

    return results_array;
}

// main function to demonstrate the modified KMP algorithm
int main() {
    // sample text and pattern for testing (from sample exam)
    char *text = "aabaacaadaabaaba";
    char *pattern = "aaba";

    printf("Text: %s\n", text);
    printf("Pattern: %s\n", pattern);
    
    int match_count = 0;
    int *match_positions = kmp_search(pattern, text, &match_count);
    
    if (match_positions != NULL && match_count > 0) {
        printf("Pattern found at %d position(s):\n", match_count);
        for (int i = 0; i < match_count; i++) {
            printf("Position %d: %d\n", i + 1, match_positions[i]);
        }
        
        // clean up dynamically allocated memory
        free(match_positions);
    } else {
        printf("Pattern not found in text\n");
    }

    return 0;
}

































































































































































































































