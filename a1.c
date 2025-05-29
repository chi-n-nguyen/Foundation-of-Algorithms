/*============================================================================*
*                             FILE: A1.c                                      *
*                Skeleton code: Sasha Soraine, Kacie Beckett                  *
*                    Edited by: (Nhat) Chi Nguyen, 1492182                    *
*                  Course code: COMP10002 2025 Semester 1                     *
*============================================================================*/

#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

/*==========================================================*
*                 PREPROCESSOR DIRECTIVES                   *
*==========================================================*/
#define PLACEHOLDER_VALUE ' '
#define START_SPACE 'S'
#define END_SPACE 'E'
#define BLOCK_SPACE 'X'
#define PATH_SPACE '+'
#define VISITED_SPACE '*'
#define EMPTY_SPACE ' '  
#define N 5

// Global direction vectors for up, right, down, left 
static const int rowDir[4] = {-1, 0, 1, 0};
static const int colDir[4] = {0, 1, 0, -1};

/*==========================================================*
*                   FUNCTION PROTOTYPES                     *
*==========================================================*/
/*----------------------------------------------------------*
*                    PROVIDED FUNCTIONS                     *
*----------------------------------------------------------*/
void ClearMap(char MAP[][N]);
void RefreshMap(char MAP[][N]);
void LevelHeader(int LevelNum);

void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level03(char MAP[][N], int startRow, int startColumn);
void Level04(char MAP[][N], int startRow, int startColumn);

/*----------------------------------------------------------*
*                  FUNCTIONS TO COMPLETE                    *
*----------------------------------------------------------*/
// Render map
void PrintMap(char MAP[][N]);
// Map maintenance
void FillMap(char MAP[][N], int *startRow, int *startColumn, int *endRow,
             int *endColumn);
// Pathfinding algorithms
int SimpleDirections(char MAP[][N], int startRow, int startColumn, int endRow,
                     int endColumn);
void ClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn);
bool ImprovedPathfinding(char MAP[][N], int currentRow, int currentColumn);

/*----------------------------------------------------------*
*        SPACE FOR YOUR OWN CUSTOM HELPER FUNCTIONS         *
*----------------------------------------------------------*/
int IsValidMove(char MAP[][N], int row, int col);
bool AttemptMove(char MAP[][N], int *curRow, int *curCol,
                 int rowDir, int colDir, int *steps);
bool IsStuck(char MAP[][N], int curRow, int curCol);
bool FindPath(char MAP[][N], int curRow, int curCol, bool visited[][N]);

/**
 * @brief: Checks if possible to step into a position
 * @return: 1 if valid, 0 if invalid
*/
int IsValidMove(char MAP[][N], int row, int col) {
    if (row < 0 || row >= N || col < 0 || col >= N || MAP[row][col] == BLOCK_SPACE) {
        return 0;
    }

    return (MAP[row][col] == EMPTY_SPACE || MAP[row][col] == END_SPACE);
}

/**
 * @brief: Attempts to step one position and marks it as part of MAP
 * @return: True if can move, false if blocked
*/
bool AttemptMove(char MAP[][N], int *curRow, int *curCol,
                 int rowStep, int colStep, int *steps) {
    int nextRow = *curRow + rowStep;
    int nextCol = *curCol + colStep;

    if (!IsValidMove(MAP, nextRow, nextCol)) {
        return false;
    }

    *curRow = nextRow;
    *curCol = nextCol;

    if (MAP[nextRow][nextCol] == EMPTY_SPACE) { 
        MAP[nextRow][nextCol] = PATH_SPACE;
    }

    (*steps)++;
    return true;
}

/**
 * @brief: Checks all 4 neighbour positions (up, right, down, left) 
 * @return: True if stuck, false if at least 1 valid move exists
*/
bool IsStuck(char MAP[][N], int curRow, int curCol) {   
    for (int i = 0; i < 4; i++) {
        if (IsValidMove(MAP, curRow + rowDir[i], curCol + colDir[i])) {
            return false;
        }
    }

    return true;
}

/**
 * @brief: Recursive pathfinding with backtracking
 *         Base case: return true if on END_SPACE, false otherwise
 *         Recursive case: if position is EMPTY_SPACE, mark as VISITED_SPACE 
 *          recurse on first valid neighbour, if recursive call returns true,
 *          mark as PATH_SPACE and return true        
 * @return: True if found path to END_SPACE, false otherwise
*/
bool FindPath(char MAP[][N], int curRow, int curCol, bool visited[][N]) {
    if (MAP[curRow][curCol] == END_SPACE) {
        return true;
    }

    visited[curRow][curCol] = true;

    bool isStart = (MAP[curRow][curCol] == START_SPACE);

    if (!isStart && MAP[curRow][curCol] == EMPTY_SPACE) {
        MAP[curRow][curCol] = VISITED_SPACE;
    }

    for (int i = 0; i < 4; i++) {
        int nextRow = curRow + rowDir[i];
        int nextCol = curCol + colDir[i];

        if (nextRow >= 0 && nextRow < N && 
            nextCol >= 0 && nextCol < N &&
            MAP[nextRow][nextCol] != BLOCK_SPACE &&
            !visited[nextRow][nextCol]) {
            if (FindPath(MAP, nextRow, nextCol, visited)) {
                if (!isStart && MAP[curRow][curCol] == VISITED_SPACE) {
                    MAP[curRow][curCol] = PATH_SPACE;
                }

                return true;
            }
        }
    }

    return false;
}

/*==========================================================*
*                        CODE START                         *
*==========================================================*/
int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-level")) {
        printf(
            "You must run this program specifying the level to run as an "
            "argument\n");
        exit(EXIT_FAILURE);
    }
    int level = atoi(argv[2]);

    char MAP[N][N];
    int startRow, startColumn, endRow, endColumn;

    ClearMap(MAP);
    FillMap(MAP, &startRow, &startColumn, &endRow, &endColumn);

    if (level == 1) {
        Level01(MAP, startRow, startColumn, endRow, endColumn);
    } else if (level == 2) {
        Level02(MAP, startRow, startColumn, endRow, endColumn);
    } else if (level == 3) {
        Level03(MAP, startRow, startColumn);
    } else if (level == 4) {
        Level04(MAP, startRow, startColumn);
    }

    return 0;
}

/*==========================================================*
*                  FUNCTIONS TO COMPLETE                    *
*==========================================================*/
/**
 * @brief: Level 1 Task 2 - Prints out the map to the terminal screen
**/
void PrintMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) 
            printf("[%c]", MAP[i][j]);
        printf("\n");
    }
}

/** 
 * @brief: Level 1 Task 1 & 3 - Reads in the map data from the test file,
           saves the starting and ending locations
**/
void FillMap(char MAP[][N], int *startRow, int *startColumn, int *endRow,
             int *endColumn) {              
    int sRow, sCol, eRow, eCol, blocks;
    
    if (scanf("%d %d", &sRow, &sCol) != 2) {
        fprintf(stderr, "Error reading starting position\n");
        exit(EXIT_FAILURE);
    } 

    if (scanf("%d %d", &eRow, &eCol) != 2) {
        fprintf(stderr, "Error reading ending position\n");
        exit(EXIT_FAILURE);
    }

    if (scanf("%d", &blocks) != 1) {
        fprintf(stderr, "Error reading number of blocks\n");
        exit(EXIT_FAILURE);
    }
    
    *startRow = sRow;
    *startColumn = sCol;
    *endRow = eRow;
    *endColumn = eCol;
    
    for (int i = 0; i < blocks; i++) {
        int blockRow, blockCol;

        if (scanf("%d %d", &blockRow, &blockCol) != 2) {
            fprintf(stderr, "Error reading block #%d\n", i + 1);
            exit(EXIT_FAILURE);
        }

        if (blockRow >= 0 && blockRow < N && blockCol >= 0 && blockCol < N) {
            MAP[blockRow][blockCol] = BLOCK_SPACE;
        }
    }

    MAP[sRow][sCol] = START_SPACE;
    MAP[eRow][eCol] = END_SPACE;
}

/** 
 * @brief: Level 2 - Finds the correct row then the correct column
 *         Assumption: START_SPACE =/= END_SPACE
 *         Exception: Stuck at 0 step but (-0)==0, later specify in Level02
 * @return: Number of steps taken, positive if reached the end, negative if stuck
**/
int SimpleDirections(char MAP[][N], int startRow, int startColumn, int endRow,
                     int endColumn) {
    int curRow = startRow;
    int curCol = startColumn;
    int steps = 0;
    
    int rowStep = (endRow > curRow) ? 1 : -1;
    while (curRow != endRow) {
        if (AttemptMove(MAP, &curRow, &curCol, rowStep, 0, &steps)) {
            continue;
        }

        int colStep = (endColumn > curCol) ? 1 : -1;
        if (AttemptMove(MAP, &curRow, &curCol, 0, colStep, &steps)) {
            continue; 
        }

        return -steps;
    }

    int colStep = (endColumn > curCol) ? 1 : -1;
    while (curCol != endColumn) {
        if (AttemptMove(MAP, &curRow, &curCol, 0, colStep, &steps)) {
            continue;
        }

        if (AttemptMove(MAP, &curRow, &curCol, 1, 0, &steps) ||
            AttemptMove(MAP, &curRow, &curCol, -1, 0, &steps)) {
            continue;
        }
        return -steps;
    }  

    return steps;
}

/** 
 * @brief: Level 3 - Recursively finds and marks a path from START_SPACE to END_SPACE
 *         Attempts one move per call in order: up, right, down, left
 *         Base case: return if on END_SPACE
 *         Recursive case: if a neighbour is END_SPACE, recurse,
 *         if EMPTY_SPACE, mark as PATH_SPACE, recurse 
 *         Restore START_SPACE if no path found
 */
void ClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn) {
    if (MAP[currentRow][currentColumn] == END_SPACE) {
        return;
    }

    bool isStart = (MAP[currentRow][currentColumn] == START_SPACE);

    for (int dir = 0; dir < 4; dir++) {
        int nextRow = currentRow + rowDir[dir];
        int nextCol = currentColumn + colDir[dir];

        if (nextRow < 0 || nextRow >= N || nextCol < 0 || nextCol >= N) {
            continue;
        }

        if (MAP[nextRow][nextCol] == END_SPACE) {
            ClosestFreeNeighbour(MAP, nextRow, nextCol);
            if (isStart) {
                MAP[currentRow][currentColumn] = START_SPACE;
            } 

            return;
        }

        if (MAP[nextRow][nextCol] == EMPTY_SPACE) {
            MAP[nextRow][nextCol] = PATH_SPACE;
            if (!isStart) {
                MAP[currentRow][currentColumn] = PATH_SPACE;
            }

            ClosestFreeNeighbour(MAP, nextRow, nextCol);
            if (isStart) {
                MAP[currentRow][currentColumn] = START_SPACE;
            }

            return;
        }
    }

    if (isStart) {
        MAP[currentRow][currentColumn] = START_SPACE;
    }
}

/** 
 * @brief: Level 4 Bonus - Implements depth-first backtracking from START_SPACE to END_SPACE
 * @return: True if path found, false otherwise
 */
bool ImprovedPathfinding(char MAP[][N], int currentRow, int currentColumn) {
    bool visited[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = false;
        }
    }

    if (FindPath(MAP, currentRow, currentColumn, visited)) {
        return true;
    } else {
        printf("No path found\n");
        return false;
    }
}

/*==========================================================*
*                    PROVIDED FUNCTIONS                     *
*==========================================================*/
void LevelHeader(int LevelNum) {
    printf("=================================\n");
    printf("Level %d:\n", LevelNum);
    printf("=================================\n");
}
// Level01
void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(1);
    PrintMap(MAP);
    printf("The starting position is at MAP[%d][%d]\n", startRow, startColumn);
    printf("The ending position is at MAP[%d][%d]\n", endRow, endColumn);
}

// Level02
void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(2);
    int steps = SimpleDirections(MAP, startRow, startColumn, endRow, endColumn);

    if (steps > 0) {
        printf("SimpleDirections took %d steps to find the goal.\n\n", steps);
    } else if (steps == 0) {
        if(IsStuck(MAP, startRow, startColumn)) {
            printf("SimpleDirections took %d steps and got stuck.\n\n", steps);
        } else {
            printf("SimpleDirections took %d steps to find the goal.\n\n", -steps);
        }
    } else {
        printf("SimpleDirections took %d steps and got stuck.\n\n", -steps);
    }

    PrintMap(MAP);
}

// Level03
void Level03(char MAP[][N], int startRow, int startColumn) {
    LevelHeader(3);
    RefreshMap(MAP);
    ClosestFreeNeighbour(MAP, startRow, startColumn);
    PrintMap(MAP);
}

// Level04
void Level04(char MAP[][N], int startRow, int startColumn) {
    LevelHeader(4);
    RefreshMap(MAP);
    ImprovedPathfinding(MAP, startRow, startColumn);
    PrintMap(MAP);
}

// Function to clear the map
void ClearMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MAP[i][j] = EMPTY_SPACE;
        }
    }
}

// Function to make printing out a "clean" map easier
void RefreshMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char currentNode = MAP[i][j];
            if (!(currentNode == START_SPACE || currentNode == END_SPACE ||
                  currentNode == BLOCK_SPACE)) {
                MAP[i][j] = EMPTY_SPACE;
            }
        }
    }
}

/*==========================================================*
*                     WRITTEN ANSWERS                       *
*==========================================================*/
/*--------------------------------------*
*           LEVEL 2 ANSWERS             *
*--------------------------------------*/
/* Q1. Cases where the naive pathfinding algorithm will not reach the end:
- When the starting position is completely surrounded by blocks on all 4 sides, making it
impossible to take any step. The algorithm gets stuck at step 0.
Example: Start at (1,1) with blocks at (0,1), (1,0), (1,2), (2,1) -> stuck immediately

- When both the one-step move toward the goal in the row direction and
the one step move toward the goal in the column direction are blocked by
adjacent blocks, the algorithm gets stuck at step 0 even though 
a longer route exists.
Example: N=5, Start at (2,2), End at (3,3) with blocks at (2,3) and (3,2) 
-> both first-row move (down) and first-column move (right) fail immediately 
even though a longer route exists S->(2,1)->(3,1)->(3,2)->E

- In maps with deadends and need to backtrack (e.g.: get blocked during the column search), 
the algorithm will stop at the first deadend and never go back moves to explore 
alternative paths.
Example: N=5, Start at (0,0), End at (4,4) with blocks at (1,1), (2,2), (3,3), (4,3)
-> stuck at step 7, even though another route exists S->(0,1)->(0,2)->(0,3)->(0,4)
->(1,4)->(2,4)->(3,4)->E with 8 steps. 
*/ 

/* Q2. Efficiency on larger maps:
Time complexity: T = O(delta row + delta column) = O(N-1+N-1) = O(2N-2) = O(N)
for average and worst case, where delta row is the absolute difference of endRow and startRow, 
and delta column is the absolute difference of endColumn and startColumn. For example,
a 100x100 map (N=100) has worst case = 200 steps.

Strengths: The algorithm runs fast on large empty maps. It stops when encounter a block, 
which prevents any wasted work.

Weaknesses: Fails with dense and complex blocks despite valid paths existing (examples in Q1).
It also lacks backtracking ability, so a single wrong turn sends it 
to a deadend with no way to recover. When there are more than one paths to the goal, 
the algorithm is likely to choose a path that is not shortest or not most efficient. 

So, while efficient for simple, empty maps, the naive algorithm trades completeness
for speed and as a result unsuitable and inefficient for maze-like maps 
that requires systematic pathfinding. 
*/

/*--------------------------------------*
*           LEVEL 4 ANSWERS              
*--------------------------------------*/
/* Q1. The Closest Neighbour process will NOT always work on every map, because:
- This greedy algorithm always commits to the first valid neighbour in the fixed order of
up, right, down left without considering the actual location of END_SPACE. 
This means that it has no sense of direction and might waste resource walking into
deadends or paths that do not lead to END_SPACE. 

- The fixed checking order creates a bias in pathfinding pattern that can be 
inefficient for certain map layouts. The spec doesn't limit the map layout, 
so the algorithm is likely to fail on certain maps.
Example of a map that ClosestFreeNeighbour fails: N=5, S at (0,0),
E at (2,2), blocks at (1,2), (1,3) (4,3) -> stuck at (4,4) - step 8

- Once the current position is surrounded by PATH_SPACE, BLOCK_SPACE
or the map boundary, the algorithm has no memory of the previous PATH_SPACE and 
as a result does not have the backtracking ability to explore other paths 
that might reach END_SPACE. 
*/

/* Q2. My suggestion for improvement in pseudocode:

procedure ImprovedPathfinding
    initialise visited array to track explored positions, set false for all coordinates  
    call recursive helper function FindPath to find path
    if FindPath returns true then
        return true
    else
        return false and output "No path found"
end procedure

procedure FindPath
    if current position is END_SPACE then 
        return true
    endif
    
    mark current position as visited   
    remember if this is the start position
    
    if current position is not start position and is empty then
        mark as visited
    endif
    
    for dir from 0 to 3 do
        next row <- current row + rowDir[dir]
        next col <- current col + colDir[dir]
        
        if next position is not out of bound and not visited then
            recurse from first valid neighbour using FindPath
            if path found then 
                mark current position as part of the solution path
                endif
                return true
            endif
        endif
    endfor
    
    return false if no valid path
end procedure
*/