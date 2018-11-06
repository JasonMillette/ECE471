#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {           //allow user input from cli

    if (argc != 2) {                        //error checking user input
        printf("Enter a number of lines to print our\n");
        return 1;
    }

    for (int i=1; i <= atoi(argv[1]); i++) {            //run loop requested number of times
	printf("#%d: ECE471 This message is going to get old!\n", i);  //printing line number, and message
    }

	return 0;
}
