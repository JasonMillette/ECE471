#include <stdio.h>

int main(int argc, char **argv) {           //allow user input from cli

    for (int i=0; i < 15; i++) {            //run loop 15 times
	printf("#%2d: ECE471 This message is going to get old!\n", i);  //printing line number sized to 2 spaces to keep nice format, and message
    }
	return 0;
}
