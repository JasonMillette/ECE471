#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#define address 0x70
#define oscillatorOn ((0x2 << 4) | (0x1))
#define displayOn ((0x8 << 4) | (0x1))
#define brightness15 ((0xE << 4) | (0xE))

int main(int argc, char **argv) {

	int fd, addr, wrt;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */
        fd = open(i2c_device, O_RDWR);
        if (fd < 0) {
            fprintf(stderr, "Open error\n");
            return 2;
        }

	/* Set slave address */
        addr = ioctl(fd, I2C_SLAVE, address);
        if (addr < 0) {
            fprintf(stderr, "ioctl error\n");
            return 3;
        }

	/* Turn on oscillator */
        buffer[0] = oscillatorOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

	/* Turn on Display, No Blink */
        buffer[0] = displayOn;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

	/* Set Brightness */
        buffer[0] = brightness15;
        wrt = write(fd, buffer, 1);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

	/* Write 1s to all Display Columns */
        buffer[0] = 0x0;
        for (int i = 0; i < 16; i++) buffer[1+i] = 0xff;
        wrt = write(fd, buffer, 17);
        if (wrt < 0) {
            fprintf(stderr, "write error\n");
            return 4;
        }

	/* Close device */
        close(fd);

	return 0;
}
