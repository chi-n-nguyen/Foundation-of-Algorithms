#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Some recursive questions, iteration is prohibited */

/* QUESTION 9 SAMPLE 2025: Recursive palindrome check */

int is_palindrome(char *str, int n) {
    if (str == NULL | n < 0) {
        return 0;
    }

    if (n <= 1) {
        return 1;
    }

    if (str[0] == str[n-1]) {
        is_palindrome(str + 1, n - 2);
    }

    return 0;
}

/* EXTRA! */

int array_sum(int arr[], int n) {
    if (n <= 0) {
        return 0; // base case: no elements left, sum is 0
    }

    return arr[n-1] + array_sum(arr, n - 1);
}

int power(int base, int exp) {
    if (exp < 0) {
        return 0;
    }

    if (exp == 0) {
        return 1;
    }

    return base * power(base, exp - 1);
}

// NOTE: strings end with a '\0'
int string_length(char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // base case: empty string
    }

    return 1 + string_length(str + 1); // count current char and recurse (pointer arithmetic)
}

int find_max(int arr[], int n) {
    if (n <= 0) {
        return -1; // Error case - invalid input
    }

    if (n == 1) {
        return arr[0]; // Base case: only one element
    }

    // Recursive case: compare last element with max of rest
    int max_of_rest = find_max(arr, n-1);
    return (arr[n-1] > max_of_rest) ? arr[n-1] : max_of_rest;
}

int digit_sum(int n) {
    // Base case: single digit
    if (n < 10) {
        return n;
    }
    
    // Recursive case: last digit + sum of remaining digits
    return (n % 10) + digit_sum(n / 10);
}

int binary_search(int arr[], int low, int high, int target) {
    // Base case: search space exhausted
    if (low > high) {
        return -1;
    }
    
    // Find middle index
    int mid = (low + high) / 2;
    
    // Target found
    if (arr[mid] == target) {
        return mid;
    }
    
    // Target is in left half
    if (arr[mid] > target) {
        return binary_search(arr, low, mid - 1, target);
    }
    
    // Target is in right half
    return binary_search(arr, mid + 1, high, target);
}

int count_char(char *str, char c) {
    // Base case: end of string
    if (*str == '\0') {
        return 0;
    }
    
    // Current character matches target
    if (*str == c) {
        return 1 + count_char(str + 1, c);
    }
    
    // Current character doesn't match
    return count_char(str + 1, c);
}

