#include <stdio.h>
#include <stdlib.h>

/* Prohibited use of <string.h> questions */

/* QUESTION 5 SAMPLE EXAM 2025: Write a function that appends string "src" 
to the end of string "dst". Assume that "dst" is not NULL, and it has sufficient 
space to store any new character from "src". Can't use <string.h> 

Time complexity: O(m + n), m = length of the dst string, n = length of the src string */

/* Array index approach */
void my_str_cat(char *dst, char *src) {
    if (dst == NULL || src == NULL) {
        return; // defensive programming
    }
    int i = 0; 
    int j = 0;

    // find the end of the "dst" string
    while (dst[i] != '\0') {
        i++;
    }

    // copy "src" to the end of "dst"
    while (src[j] != '\0') {
        dst[i] = src[j];
        i++;
        j++;
    }

    // add null terminator, if don't add then undefined behavior 
    dst[i] = '\0';
}

/* Pointer arithmetic approach (More elegant) */
void my_str_cat(char *dst, char *src) {
        if (dst == NULL || src == NULL) {
        return; // defensive programming
    }

    while (*dst != '\0') {
        dst++; // move to the end of "dst"
    }

    while (*src != '\0') {
        *dst = *src; // copy character from "src" to "dst" (derefence it)
        dst++;
        src++;
    }

    *dst = '\0'; // add null terminator (dereference it)
}

/* QUESTION 2: STRLEN DUPE */
int my_strlen(char *str) {
    int length = 0;
    if (str == NULL) {
        return 0; // defensive programming
    }

    while (*str != '\0') {
        length++;
        str++;
    }

    return length;
}

/* QUESTION 3: STRCPY DUPE */
void my_strcpy(char *dst, char *src) {
    if (dst == NULL || src == NULL) {
        return; // defensive programming
    }

    while (*src != '\0') {
        *dst = *src; // copy character from "src" to "dst"
        dst++;
        src++;
    }

    *dst = '\0'; // add null terminator
} 

/* QUESTION 4: STRCMP DUPE */
int my_strcmp(char *str1, char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1; // defensive programming
    }

    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2; // return difference of first non-matching characters
        }
        str1++;
        str2++;
    }

    return *str1 - *str2; // if one string is shorter, return the difference
}


























































































































































































































