//Jason Millette && Glenn Lumbert
//ECE471 HW4
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void export(char *pin);
void direction(char *pin, char *direction);
void value(char *pin, char *IO);

int main(int argc, char **argv) {

    export("18");
    direction("18", "out");

    while(1) {
    value("18","1");    //sets GPIOs
    usleep(500000);     //delay
    value("18","0");    //sets GPIOs
    usleep(500000);
    }
    return 0;
}


void export(char *pin) {
    //exports GPIO pin
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "\tError enabling\n");
    }

    write(fd, pin, 2);
    close(fd);
    return;
}

void direction(char *pin, char *direction) {
    //sets the direction of users choice for user choice of GPIO pin 
    int fd;
    char *directory = "/sys/class/gpio/gpio";   //strings for open
    char *name = "/direction";
    char *filename;
    filename = malloc(strlen(directory) + strlen(name) + strlen(direction) + 1); //allocating memory for filename
    strcat(filename, directory);
    strcat(filename, pin);
    strcat(filename, name);     //putting all the strings together
    fd = open(filename, O_WRONLY);
    write(fd, direction, strlen(direction));
    close(fd);
    free(filename);       //free memory from malloc
    return;
}

void value(char *pin, char *IO) {
    //sets IO of users pin
    int fd;
    char *directory = "/sys/class/gpio/gpio";
    char *name = "/value";
    char *filename;
    filename = malloc(strlen(directory) + strlen(name) + strlen(IO) + 1); //allocating memory for filename
    strcat(filename, directory);
    strcat(filename, pin);
    strcat(filename, name);     //putting all the strings together
    fd = open(filename, O_WRONLY);
    write(fd, IO, strlen(IO));
    close(fd);
    free(filename);       //free memory from malloc
    return;
}
