#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

//useful defines for addresses
#define address 0x70
#define oscillatorOn ((0x2 << 4) | (0x1))
#define displayOn ((0x8 << 4) | (0x1))
#define brightness15 ((0xE << 4) | (0xE))
#define off 0x00
#define G 0x6C
#define I 0x0

int main(int argc, char **argv) {

	int fd, addr, wrt;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];
        //setting last 7 bytes to empty
        buffer[10] = 0x00;
        buffer[11] = 0x00;
        buffer[12] = 0x00;
        buffer[13] = 0x00;
        buffer[14] = 0x00;
        buffer[15] = 0x00;
        buffer[16] = 0x00;

	/* Open i2c device */
        fd = open(i2c_device, O_RDWR);
        if (fd < 0) {
            fprintf(stderr, "Open error\n");    //error checking
            return 2;
        }

	/* Set slave address */
        addr = ioctl(fd, I2C_SLAVE, address);
        if (addr < 0) {
            fprintf(stderr, "ioctl error\n"); //error checking
            return 3;
        }

	/* Turn on oscillator */
        buffer[0] = oscillatorOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");  //error checking
            return 4;
        }

	/* Turn on Display, No Blink */
        buffer[0] = displayOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");  //error checking
            return 4;
        }

	/* Set Brightness */
        buffer[0] = brightness15;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");  //error checking
            return 4;
        }

        while (1) {
            //Displays ECE4
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = E;
            buffer[2] = 0x00;
            buffer[3] = C;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = E;
            buffer[8] = 0x00;
            buffer[9] = four;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);

            //Displays CE47
            buffer[0] = 0x00;
            buffer[1] = C;
            buffer[2] = 0x00;
            buffer[3] = E;
            buffer[4] = 0x00;
            buffer[5] = off;
            buffer[6] = 0x00;
            buffer[7] = four;
            buffer[8] = 0x00;
            buffer[9] = seven;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delay 1 second
            usleep(1000000);

            //Displays E471
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = E;
            buffer[2] = 0x00;
            buffer[3] = four;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = seven;
            buffer[8] = 0x00;
            buffer[9] = one;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);

            //Displays 471
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = four;
            buffer[2] = 0x00;
            buffer[3] = seven;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = one;
            buffer[8] = 0x00;
            buffer[9] = off;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);

            //Displays 471
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = seven;
            buffer[2] = 0x00;
            buffer[3] = one;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = off;
            buffer[8] = 0x00;
            buffer[9] = off;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);

            //Displays 471
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = one;
            buffer[2] = 0x00;
            buffer[3] = off;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = off;
            buffer[8] = 0x00;
            buffer[9] = off;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);

            //Displays nothing
            buffer[0] = 0x00;                   //display pointer offset
            buffer[1] = off;
            buffer[2] = 0x00;
            buffer[3] = off;
            buffer[4] = 0x00;
            buffer[5] = off;                    //center dots unused
            buffer[6] = 0x00;
            buffer[7] = off;
            buffer[8] = 0x00;
            buffer[9] = off;
            wrt = write(fd, buffer, 17);
            if (wrt < 0) {
                fprintf(stderr, "write error\n");  //error checking
                return 4;
            }
            //delays 1 second
            usleep(1000000);
        }

	/* Close device */
        close(fd);

	return 0;
}
