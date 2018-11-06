#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "sensor_name.h"


static double read_temp(char *filename) {

	double result=0.0;
        char rawInput[256], crc[256];       //strings for input
        FILE *W1;

	/* Your code here */
        W1 = fopen(SENSOR_NAME, "r");       //opens one wire device
        if (W1 < 0) {
            printf("error opening W1\n");
            return -1000;
        }

        fscanf(W1, " %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s",crc , rawInput);    //assigns the wanted strings and ignores the rest

        if (strcmp(crc, "YES")) {       //checks the CRC
            printf("crc failed");
            return -1001;
        }

        result = (atoi(rawInput + 2)) / 1000.0;     //Converts string to degrees C

	return result;
}

int main(int argc, char **argv) {

	double temp1;

	while(1) {

		temp1=read_temp(SENSOR_NAME);
                if (temp1 <= -1000) printf("Error in reading from device.\n");
                else {
                    temp1 = (temp1 * (9.0/5)) + 32;     //convert C to F
                    printf("%.2lfF\n",temp1);
                }
		sleep(1);
	}

	return 0;
}
