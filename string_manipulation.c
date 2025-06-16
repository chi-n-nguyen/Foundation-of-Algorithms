#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    printf("=== STRING MANIPULATION DEMONSTRATION ===\n\n");
    
    // Original strings
    char greeting[50] = "Hello ";  // Fixed: needs array size
    char name[] = "Alice";
    char message[] = "Welcome to C programming!";
    
    printf("Original strings:\n");
    printf("greeting: \"%s\"\n", greeting);
    printf("name: \"%s\"\n", name);
    printf("message: \"%s\"\n", message);
    printf("\n");
    
    // 1. STRCAT DEMONSTRATION
    printf("=== STRCAT (Concatenation) ===\n");
    printf("Before strcat - greeting: \"%s\"\n", greeting);
    strcat(greeting, name);
    printf("After strcat(greeting, name): \"%s\"\n", greeting);
    printf("\n");
    
    // 2. STRCPY DEMONSTRATION  
    printf("=== STRCPY (Copy) ===\n");
    char copy_buffer[100];  // Fixed: needs array size
    printf("Before strcpy - copy_buffer: uninitialized\n");
    strcpy(copy_buffer, message);
    printf("After strcpy(copy_buffer, message): \"%s\"\n", copy_buffer);
    
    // Demonstrate that strcpy overwrites
    strcpy(copy_buffer, "New content");
    printf("After strcpy(copy_buffer, \"New content\"): \"%s\"\n", copy_buffer);
    printf("\n");
    
    // 3. STRDUP DEMONSTRATION
    printf("=== STRDUP (Duplicate) ===\n");
    char *duplicate = strdup(message);
    if (duplicate != NULL) {
        printf("Original message: \"%s\"\n", message);
        printf("After strdup: \"%s\"\n", duplicate);
        
        // Show they're independent - modify duplicate
        duplicate[0] = 'w';  // Change 'W' to 'w'
        printf("After modifying duplicate[0]: \"%s\"\n", duplicate);
        printf("Original message unchanged: \"%s\"\n", message);
        
        free(duplicate);  // Important: free the memory
        printf("Memory freed successfully\n");
    } else {
        printf("Memory allocation failed\n");
    }
    printf("\n");
    
    // 4. BONUS: More examples
    printf("=== BONUS EXAMPLES ===\n");
    
    // Multiple concatenations
    char sentence[200] = "C programming";
    strcat(sentence, " is");
    strcat(sentence, " awesome");
    strcat(sentence, "!");
    printf("Multiple strcat result: \"%s\"\n", sentence);
    
    // Chain of operations
    char source[] = "Learning ";
    char *dup_source = strdup(source);
    if (dup_source != NULL) {
        strcat(dup_source, "strings");  // This would crash - strdup gives exact size
        printf("This line won't execute due to buffer overflow\n");
        free(dup_source);
    }
    
    // Safe version of above
    char safe_buffer[50];
    strcpy(safe_buffer, source);
    strcat(safe_buffer, "strings safely!");
    printf("Safe concatenation: \"%s\"\n", safe_buffer);
    
    return 0;
}
