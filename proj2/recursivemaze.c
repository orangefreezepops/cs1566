
    /*choose starting/ending points of the maze
    //generate random number to determine which portion of the walls 
    //will get knocked out
    srand(time(0));
    int entrance = rand() % height;
    int exit = rand() % height;
    printf("entrance: %d\n", entrance);
    printf("exit: %d\n", exit);
    if (entrance % 2 == 0) {
        if (entrance == height - 1) {
            entrance -= 1;
            printf("entrance after correction1: %d\n", entrance);
        } else {
            entrance += 1;
            printf("entrance after correction2: %d\n", entrance);
        }
    }
    if (exit % 2 == 0) {
        if (exit == height - 1) {
            exit -= 1;
            printf("exit after correction1: %d\n", exit);
        } else {
            exit += 1;
            printf("exit after correction2: %d\n", exit);
        }
    }

    //set the positions to 0, signifying they're now open
    maze[entrance][0] = 0;
	maze[exit][width - 1] = 0;
    
    printMaze(maze);
	
	/*recursion didnt work bc I'm stupid
	int leftbound = 0;
	int upbound = 0;
	int rightbound = width - 1;
	int lowbound = height - 1;
    divide(leftbound, upbound, rightbound, lowbound);
	*/

/*
void divide(int leftbound, int upbound, int rightbound, int lowbound) {
	printf("Start recursion\n");

    //Base cases
    if (leftbound + 2 >= rightbound) {
		printf("failed: leftbound %d + 2 >= rightbound %d\n", leftbound, rightbound);
        return;
    }
    if (rightbound - 2 <= leftbound) {
		printf("failed: rightbound %d - 2 >= leftbound %d\n", rightbound, leftbound);
        return;
    }
    if (upbound + 2 >= lowbound) {
		printf("failed: upbound %d + 2 >= lowbound %d\n", upbound, lowbound);
        return;
    }
    if (lowbound - 2 <= upbound) {
		printf("failed: lowbound %d - 2 >= upbound %d\n", lowbound, upbound);
        return;
    }
	printf("beat base cases\n");
    //time_t t; //For random num gen
	srand(time(NULL));

    int r = 1;
    int c = 1;
    int i;
    int upwall = 0;
    int rightwall = 0;
    int downwall = 0;
    int leftwall = 0;

    while (c % 2 != 0) { //not even number. get random
        c = rand() % ((rightbound - 2) - (leftbound + 2) + 1) + leftbound + 2; //Get you cs from 2 - 14
    }
    //Choose r from 0-16
    while (r % 2 != 0) {
        r = rand() % ((lowbound - 2) - (upbound + 2) + 1) + upbound + 2; //Get you rs from 2 - 14
    }

    printf("r %i \n", r);
    printf("c %i \n", c);
    maze[r][c]; //start point
    for (i = leftbound; i < rightbound; i++) {
        if (maze[r][i] == 0) {
            maze[r][i] = 2; //Add wall through the r
            if (c < i) {
                rightwall++;
            } else {
                leftwall++;
            }
        }
    }
    for (i = upbound; i < lowbound; i++) {
        if (maze[i][c] == 0) {
            maze[i][c] = 2; //Add wall through the c
            if (r < i) {
                downwall++;
            } else {
                upwall++;
            }
        }
    }
    int keep = rand() % 5; //Random 0-4
	printf("keep %d\n", keep);
	printf("upwall %d\n", upwall);
	printf("downwall %d\n", downwall);
	printf("rightwall %d\n", rightwall);
	printf("leftwall %d\n", leftwall);
    int u_knock;
    int r_knock;
    int d_knock;
    int l_knock;
    switch (keep) {
       case 0:
			if (rightwall != 0) {
            r_knock = rand() % rightwall + 1;
            r_knock = r_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c + r_knock] = 0;
        }
        if (downwall != 0) {
            d_knock = rand() % downwall + 1;
            d_knock = d_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r + d_knock][c] = 0;
        }
        if (leftwall != 0) {
            l_knock = rand() % leftwall + 1;
            l_knock = l_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c - l_knock] = 0;
        }
		break;

		case 1:
        
		if (upwall != 0) {
            u_knock = rand() % upwall + 1;
            u_knock = u_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r - u_knock][c] = 0;
        }

        if (downwall != 0) {
            d_knock = rand() % downwall + 1;
            d_knock = d_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r + d_knock][c] = 0;
        }

        if (leftwall != 0) {
            l_knock = rand() % leftwall + 1;
            l_knock = l_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c - l_knock] = 0;
        }
		break;

		case 2:
		
        if (upwall != 0) {
            u_knock = rand() % upwall + 1;
            u_knock = u_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r - u_knock][c] = 0;
        }

        if (rightwall != 0) {
            r_knock = rand() % rightwall + 1;
            r_knock = r_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c + r_knock] = 0;
        }

        if (leftwall != 0) {
            l_knock = rand() % leftwall + 1;
            l_knock = l_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c - l_knock] = 0;
        }
		break;
    
		case 3:
        if (upwall != 0) {
            u_knock = rand() % upwall + 1;
            u_knock = u_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r - u_knock][c] = 0;
        }

        if (downwall != 0) {
            d_knock = rand() % downwall + 1;
            d_knock = d_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r + d_knock][c] = 0;
        }

        if (rightwall != 0) {
            r_knock = rand() % rightwall + 1;
            r_knock = r_knock * 2 - 1;
			printf("Set to 0\n");
            maze[r][c + r_knock] = 0;
        }
		break;
    }
	
	printf("recurse 1\n");
    divide(leftbound, upbound, c, r);
	printf("recurse 2\n");
    divide(c, upbound, rightbound, r);
	printf("recurse 3\n");
    divide(leftbound, r, c, lowbound); 
	printf("recurse 4\n");
    divide(c, r, rightbound, lowbound);
	
}
*/