/*============================================================================*
*                             FILE: A1.c                                      *
*                Skeleton code: Sasha Soraine, Kacie Beckett                  *
*           Edited by: [PLEASE ADD YOUR FULL NAME AND STUDENT ID HERE]        *
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
// The following set is for easy map rendering
// FILL OUT THIS SPACE FOR MAP ELEMENTS AND REMOVE THE PLACEHOLDER DEFINE
#define START_SPACE PLACEHOLDER_VALUE
#define END_SPACE PLACEHOLDER_VALUE
#define BLOCK_SPACE PLACEHOLDER_VALUE
#define PATH_SPACE PLACEHOLDER_VALUE
#define VISITED_SPACE PLACEHOLDER_VALUE

// The following directives are for map creation
#define EMPTY_SPACE ' '  // ASCII char 32
#define N 5

/*==========================================================*
*                   FUNCTION PROTOTYPES                     *
*==========================================================*/
/*----------------------------------------------------------*
*                    PROVIDED FUNCTIONS                     *
*----------------------------------------------------------*/
// Functions for map rendering and maintenance
void ClearMap(char MAP[][N]);
void RefreshMap(char MAP[][N]);
// Functions for printing/output readability
void LevelHeader(int LevelNum);

void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level03(char MAP[][N], int startRow, int startColumn);

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
/*----------------------------------------------------------*
*        SPACE FOR YOUR OWN CUSTOM HELPER FUNCTIONS         *
*----------------------------------------------------------*/


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
    }

    return 0;
}

/*==========================================================*
*                  FUNCTIONS TO COMPLETE                    *
*==========================================================*/
// Level 1 - Task 1
void PrintMap(char MAP[][N]) {}

// Level 1 - Task 2
void FillMap(char MAP[][N], int *startRow, int *startColumn, int *endRow,
             int *endColumn) {}

// Level 2
int SimpleDirections(char MAP[][N], int startRow, int startColumn, int endRow,
                     int endColumn) {}

// Level 3
void ClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn) {}

/*==========================================================*
*                    PROVIDED FUNCTIONS                     *
*==========================================================*/
void LevelHeader(int LevelNum) {
    printf("=================================\n");
    printf("Level %d:\n", LevelNum);
    printf("=================================\n");
}

void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(1);
    PrintMap(MAP);
    printf("The starting position is at MAP[%d][%d]\n", startRow, startColumn);
    printf("The ending position is at MAP[%d][%d]\n", endRow, endColumn);
}

void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(2);
    int steps = SimpleDirections(MAP, startRow, startColumn, endRow, endColumn);
    printf("SimpleDirections took %d steps to find the goal.\n\n", steps);
    PrintMap(MAP);
}

void Level03(char MAP[][N], int startRow, int startColumn) {
    LevelHeader(3);
    RefreshMap(MAP);
    ClosestFreeNeighbour(MAP, startRow, startColumn);
    PrintMap(MAP);
}

void ClearMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MAP[i][j] = EMPTY_SPACE;
        }
    }
}

/*Function to make printing out a "clean" map easier*/
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
*           LEVEL 3 ANSWERS             *
*--------------------------------------*/
// Q1.

// Q2.

/*--------------------------------------*
*           LEVEL 4 ANSWERS             *
*--------------------------------------*/
// Q1.

// Q2.