#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "sensor_name.h"


char temp1[255];    //global variables for use in multiple functions
unsigned char buffer[17];
char i2c_device[]="/dev/i2c-1";
int j, wrt, fd, addr;

int read_temp(char *filename) {

        char rawInput[256], crc[256];       //strings for input
        FILE *W1;

	/* Your code here */
        W1 = fopen(SENSOR_NAME, "r");       //opens one wire device
        if (W1 < 0) {
            printf("error opening W1\n");
            return -1;
        }

        fscanf(W1, " %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s",crc , rawInput);    //assigns the wanted strings and ignores the rest

        if (strcmp(crc, "YES")) {       //checks the CRC
            printf("crc failed");
            return -2;
        }

        strcpy(temp1,rawInput+2);   //copies the rawinput removes the t= and assigns it to the global temp variable
        fclose(W1);
        return 0;
}

void display(char * temp) {
    //setting empty bytes to 0
    buffer[0] = 0x00;
    buffer[2] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = off;
    buffer[6] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x63;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;

    double temperature = (atoi(temp)/1000.0);   //converts temp to int for case checking

    for (int i=0; i < 3; i++) {         //loop to assign buffer for display
        if (i == 0) j = 1;              //connects character with buffer
        if (i == 1) j = 3;
        if (i == 2) j = 7;
        switch(temp[i]) {
            case '1' :
                buffer[j] = one;
                break;
             case '2' :
                buffer[j] = two;
                break;
            case '3' :
                buffer[j] = three;
                break;
            case '4' :
                buffer[j] = four;
                break;
            case '5' :
                buffer[j] = five;
                break;
            case '6' :
                buffer[j] = six;
                break;
            case '7' :
                buffer[j] = seven;
                break;
            case '8' :
                buffer[j] = eight;
                break;
            case '9' :
                buffer[j] = nine;
                break;
            case '0' :
                buffer[j] = zero;
                break;
            case '-' :
                buffer[j] = negative;
                break;
        }
        if ((temperature <= 100) && (j == 3)) {
            buffer[3] |= 0x80;  //adds decimal point
        }

        if ((i == 0) && (buffer[i] == '0')) {
            buffer[1] = off;    //leading zero check
        }

        if (temperature < 1) {  //two leading zero check
            buffer[1] = off;
            buffer[3] |= 0x80;
        }
    }

    fd = open(i2c_device, O_RDWR);      //opens and writes to i2c display
    if (fd < 0) {
        printf("Open error\n");
        return;
    }
    addr = ioctl(fd, I2C_SLAVE, address);
    if (addr < 0) {
        fprintf(stderr, "ioctl error\n");
        return;
    }

    wrt = write(fd, buffer, 17);
    if (wrt < 0) {
        printf("error in display\n");
        return;
    }

    close(fd);
}

int main(int argc, char **argv) {

        int error;

        //Opens I2C device
        fd = open(i2c_device, O_RDWR);
        if (fd < 0) {
            fprintf(stderr, "Open error\n");
            return 2;
        }

        addr = ioctl(fd, I2C_SLAVE, address);
        if (addr < 0) {
            fprintf(stderr, "ioctl error\n");
            return 3;
        }

        //turn on oscillator
        buffer[0] = oscillatorOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

        //Turn on display
        buffer[0] = displayOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

        //Set brightness
        buffer[0] = brightness15;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

	while(1) {

	    error = read_temp(SENSOR_NAME);     //gets value from temp sensor
            if (error < 0) printf("Error in reading from device.\n");
            else {
                display(temp1);     //displays temp on i2c display
            }
	    sleep(1);       //1 second delay
	}

	return 0;
}
