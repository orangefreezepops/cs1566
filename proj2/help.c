void fill_maze(int minX, int minY, int maxX, int maxY) {
    //Edge cases
    //Cant find a valid pillar
    if (minX + 2 >= maxX) {
        return;
    }
    if (maxX - 2 <= minX) {
        return;
    }
    if (minY + 2 >= maxY) {
        return;
    }
    if (maxY - 2 <= minY) {
        return;
    }

    time_t t; //For random num gen
    srand((unsigned) time(&t));

    int row = 1;
    int col = 1;
    int i;
    int num_North_Walls = 0;
    int num_East_Walls = 0;
    int num_South_Walls = 0;
    int num_West_Walls = 0;

    while (col % 2 != 0) { //not even number. get random
        col = rand() % ((maxX - 2) - (minX + 2) + 1) + minX + 2; //Get you cols from 2 - 14
    }
    //Choose row from 0-16
    while (row % 2 != 0) {
        row = rand() % ((maxY - 2) - (minY + 2) + 1) + minY + 2; //Get you rows from 2 - 14
    }

    printf("row %i \n", row);
    printf("col %i \n", col);
    maze[row][col]; //start point
    for (i = minX; i < maxX; i++) {
        if (maze[row][i] == 0) {
            maze[row][i] = 2; //Add wall through the row
            if (col < i) {
                num_East_Walls++;
            } else {
                num_West_Walls++;
            }
        }
    }
    for (i = minY; i < maxY; i++) {
        if (maze[i][col] == 0) {
            maze[i][col] = 2; //Add wall through the col
            if (row < i) {
                num_South_Walls++;
            } else {
                num_North_Walls++;
            }
        }
    }
    printf("North Walls %i \n", num_North_Walls);
    printf("West Walls %i \n", num_West_Walls);
    printf("South Walls %i \n", num_South_Walls);
    printf("East Walls %i \n", num_East_Walls);
    //Choose random wall to leave untouched.
    //0: North
    //1: East
    //2: South
    //3: West
    int untouched = rand() % 5; //Random 0-4
    int n_Remove;
    int e_Remove;
    int s_Remove;
    int w_Remove;
    if (untouched == 0) {
        //Dont touch N
        //Choose a random location in E,S,W to remove
        if (num_East_Walls != 0) {
            e_Remove = rand() % num_East_Walls + 1;
            e_Remove = e_Remove * 2 - 1;
            maze[row][col + e_Remove] = 0;
        }
        if (num_South_Walls != 0) {
            s_Remove = rand() % num_South_Walls + 1;
            s_Remove = s_Remove * 2 - 1;
            maze[row + s_Remove][col] = 0;
        }
        if (num_West_Walls != 0) {
            w_Remove = rand() % num_West_Walls + 1;
            w_Remove = w_Remove * 2 - 1;
            maze[row][col - w_Remove] = 0;
        }

    } else if (untouched == 1) {
        //Dont touch E
        //Choose a random location in N,S,W to remove
        if (num_North_Walls != 0) {
            n_Remove = rand() % num_North_Walls + 1;
            n_Remove = n_Remove * 2 - 1;
            maze[row - n_Remove][col] = 0;
        }

        if (num_South_Walls != 0) {
            s_Remove = rand() % num_South_Walls + 1;
            s_Remove = s_Remove * 2 - 1;
            maze[row + s_Remove][col] = 0;
        }

        if (num_West_Walls != 0) {
            w_Remove = rand() % num_West_Walls + 1;
            w_Remove = w_Remove * 2 - 1;
            maze[row][col - w_Remove] = 0;
        }

    } else if (untouched == 2) {
        //Dont touch S
        //Choose a random location in N,E,W to remove
        if (num_North_Walls != 0) {
            n_Remove = rand() % num_North_Walls + 1;
            n_Remove = n_Remove * 2 - 1;
            maze[row - n_Remove][col] = 0;
        }

        if (num_East_Walls != 0) {
            e_Remove = rand() % num_East_Walls + 1;
            e_Remove = e_Remove * 2 - 1;
            maze[row][col + e_Remove] = 0;
        }

        if (num_West_Walls != 0) {
            w_Remove = rand() % num_West_Walls + 1;
            w_Remove = w_Remove * 2 - 1;
            maze[row][col - w_Remove] = 0;
        }
    } else {
        //Dont touch W
        //Choose a random location in N,S,E to remove
        if (num_North_Walls != 0) {
            n_Remove = rand() % num_North_Walls + 1;
            n_Remove = n_Remove * 2 - 1;
            maze[row - n_Remove][col] = 0;
        }

        if (num_South_Walls != 0) {
            s_Remove = rand() % num_South_Walls + 1;
            s_Remove = s_Remove * 2 - 1;
            maze[row + s_Remove][col] = 0;
        }

        if (num_East_Walls != 0) {
            e_Remove = rand() % num_East_Walls + 1;
            e_Remove = e_Remove * 2 - 1;
            maze[row][col + e_Remove] = 0;
        }
    }
    //Recursion time
    //printf("minX %i\n", minX);
    //printf("maxX %i\n", maxX);
    //printf("minY %i\n", minY);
    //printf("maxY %i\n", maxY);

    fill_maze(minX, minY, col, row); //NW
    fill_maze(col, minY, maxX, row); //NE
    fill_maze(minX, row, col, maxY); //SW
    fill_maze(col, row, maxX, maxY); //SE
}