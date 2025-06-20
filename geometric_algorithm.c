#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>

/* GEOMETRY QUESTIONS REFERENCING Q7 SAMPLE 2025 */

/* THE OG */
typedef struct {
    int ux, lx, uy, ly; // upper x, lower x, upper y, lower y
} rectangle_t;

int intersect(rectangle_t rect1, rectangle_t rect2) {
    return !(rect1.ux < rect2.lx || rect2.ux < rect1.lx ||
        rect1.uy < rect2.ly || rect2.uy < rect1.ly); // De Morgan's Law
}

/* EXTRA! */

/* Q1: CIRCLE INTERSECTION */

typedef struct {
    int origin_x, origin_y;
    int radius; 
} circle_t;

/* OOPS! ^ is XOR operator and sqrt will return a double type 
int circle_intersect(circle_t c1, circle_t c2) {
    int distance_of_centers = sqrt((c2.origin_x - c1.origin_x)^2 + (c2.origin_y - c1.origin_y)^2);
    int sum_of_radii = c1.radius + c2.radius;
    if (distance_of_centers <= sum_of_radii) {
        return 1; // Circles intersect
    } else {
        return 0; // Circles do not intersect
    }
} 
*/

int circle_intersect(circle_t c1, circle_t c2) {
    // Calculate distance between centers
    int dx = c2.origin_x - c1.origin_x;
    int dy = c2.origin_y - c1.origin_y;
    double distance_of_centers = sqrt(dx * dx + dy * dy); // type double due to sqrt()
    
    int sum_of_radii = c1.radius + c2.radius;
    
    return distance_of_centers <= sum_of_radii;
}

/* Q2: Rectangle Area Overlap */

int calculate_overlap_area(rectangle_t rect1, rectangle_t rect2) {
    // Check if rectangles intersect first
    if (rect1.ux < rect2.lx || rect1.lx > rect2.ux ||
        rect1.uy < rect2.ly || rect1.ly > rect2.uy) {
        return 0;  // No intersection
    }
    
    // Calculate intersection bounds
    int left = (rect1.lx > rect2.lx) ? rect1.lx : rect2.lx;    // max(lx1, lx2)
    int right = (rect1.ux < rect2.ux) ? rect1.ux : rect2.ux;   // min(ux1, ux2)
    int bottom = (rect1.ly > rect2.ly) ? rect1.ly : rect2.ly;  // max(ly1, ly2)
    int top = (rect1.uy < rect2.uy) ? rect1.uy : rect2.uy;     // min(uy1, uy2)
    
    // Calculate area
    int width = right - left;
    int height = top - bottom;
    
    return width * height;
}






























































































































































































































