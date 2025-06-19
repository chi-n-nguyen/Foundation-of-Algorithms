/* Array processing questions similar to MST Q2 & 2025 Sample Exam */
/* Nested loop strategy -> O(n^2) time complexity */
/* Space complexity: O(1), no additional array needed */

/* J LOOP PROBLEM
Q: Why j = i + 1 Works for Some Questions?
A: From the sample exam, you can see that j = i + 1 works for most_frequent because:
For most_frequent: You want the maximum frequency. The first occurrence of each element gets the correct count, 
and later occurrences getting undercounted doesn't matter since you're tracking the maximum.
For least_frequent: You want the minimum frequency. Later occurrences getting undercounted makes them appear falsely unique, 
breaking the logic.

=> Pattern recognition
j = i + 1 works when:
- Seeking maximum values (most_frequent, highest frequency)
- Processing only first occurrences matters
- Undercounting later duplicates doesn't affect the result
j = i + 1 fails when:
- Seeking minimum values (least_frequent)
- Needing accurate counts for ALL positions (sum_unique_elements)
- Later occurrences must be properly identified as duplicates
*/

/* Sure fire approach:
for (j = 0; j < n; j++) {
    if (j != i && array[i] == array[j]) {
        count++;
    }
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_COUNT_YET -1

/* QUESTION 1: SECOND MOST FREQUENT ELEMENT */
int second_most_frequent(int array[], int n) {
    int i, j, count, max_count = NO_COUNT_YET, second_max_count = NO_COUNT_YET;
    int max_index, second_max_index;

    for (i = 0; i < n; i++) {
        count = 1; // for each element, there's already 1 of it
        for (j = i + 1; j < n; j++) {
            if (array[i] == array[j]) {
                count++;
            }
        }

        // update max and second max counts
        if (count > max_count) {
            second_max_count = max_count; // from previous iteration
            second_max_count = max_index; 
            max_count = count;
            max_index = i;
        } else if (count > second_max_count && count < max_count) {
            second_max_count = count;
            second_max_index = i;
        } else if (count == max_count && array[i] < array[max_index]) {
            // tie break
            second_max_count = max_count;
            second_max_index = max_index;
            max_count = i; 
        }
    }

    // if all elements have the same frequency then return the smollest
    if (second_max_count == NO_COUNT_YET) {
        int min_element = array[0];
        for (i = 1; i < n; i++) {
            if (array[i] < min_element) {
                min_element = array[i];
            }
        }
        return min_element;
    }

    return array[second_max_index];
}


/* QUESTION 2: FIND UNIQUE ELEMENT ~ count == 1 */
int find_unique_element(int array[], int n) {
    int i, j, count;
    
    for (i = 0; i < n; i++) {
        count = 1;
        for (j = i+1; j < n; j++) {
            if (array[i] == array[j]) {
                count++;
            }
        }
        
        // If element appears exactly once, return it
        if (count == 1) {
            return array[i];
        }
    }
    
    // Should never reach here given problem constraints
    return -1;
}

/* QUESTION 3: LEAST FREQUENT ELEMENT */
/* Be careful with bound checks for reversed logic - J LOOP PROBLEM */

/* BUGGED!!
int least_frequent(int array[], int n) {
    int i, j, count, min_count = NO_COUNT_YET, min_index = NO_COUNT_YET;
    for (i = 0; i < n; i++) {
        count = 1;
        for (j = i + 1; j < n; j++) {
            if (array[i] == array[j]) {
                count++;
            }
        }
        if (min_count == NO_COUNT_YET || count < min_count) { // 1st iteration: count can never be < min_count (= NO_COUNT_YET)
            min_count = count;
            min_index = i;
        } else if (count == min_count && array[i] < array[min_index]) {
                min_index = i;
        }
    }
    return array[min_index];
}
*/

int least_frequent(int array[], int n) {
    int i, j, count, min_count = NO_COUNT_YET, min_index;
    
    for (i = 0; i < n; i++) {
        count = 1;
        // Check ALL other positions, not just forward ones
        for (j = 0; j < n; j++) {
            if (j != i && array[i] == array[j]) {
                count++;
            }
        }
        
        if (min_count == NO_COUNT_YET || count < min_count) {
            min_count = count;
            min_index = i;
        } else if (count == min_count && array[i] < array[min_index]) {
            min_index = i;
        }
    }
    return array[min_index];
}


/* QUESTION 4: SUM OF ALL UNIQUE ELEMENTS, J LOOP PROBLEM */
int sum_unique_elements(int array[], int n) {
    int i, j, count, sum = 0;
    for (i = 0; i < n; i++) {
        count = 1;
        for (j = 0; j < n; j++) {
            if (j != i && array[i] == array[j]) {
                count++;
            }
        }

        if (count == 1) {
            sum += array[i];
        }
    }
    return sum;
}





























































































































































































































