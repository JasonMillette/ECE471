#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "i2c-bitbang.h"

#define B   0x7C 
#define E   0x79
#define R   0x50
#define I   0x06
#define S   0x6D
#define G   0x6F
#define U   0x3E 
#define D   0x5E
int main(int argc, char **argv) {

	int result;
	unsigned char buffer[17];


	/*******************************************/
	/* Init the GPIO lines we are going to use */
	/* to bitbang the i2c                      */
	/*******************************************/
	result=i2c_init_gpios(SDA_GPIO,SCL_GPIO);
	if (result<0) {
		exit(1);
	}


	/****************************************/
	/****************************************/
	/* Start the display			*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;
	/* Command 0x20 = SYSTEM SETUP  (See p30 of manual) */
	/* Action  0x01 = Turn on oscillator */
        buffer[1]= 0x20 | 0x01;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error starting display (enable oscilator)!\n");
                return -1;
        }

	/****************************************/
	/****************************************/
	/* Turn display on, no blinking		*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;

	/* Command = 0x80 Setup*/
	/* Action = Blinking off, display on */
        buffer[1]= 0x80 | 0x1;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error starting display (turning on)!\n");
                return -1;
        }



	/****************************************/
	/****************************************/
	/* Set Brightness			*/
	/****************************************/
	/****************************************/

	/* Address of device, and we want to write */
        buffer[0]= 0x70<<1 | 0x0;

	/* Command = 0xe0 Dimming */
	/* Action = Brightness = 10/16 */
        buffer[1]= 0xe0 | 0xa;

	/* Write the two bytes in our buffer */
	result=write_i2c(buffer,2);
        if ( result<0 ) {
                fprintf(stderr,"Error setting brightness!\n");
                return -1;
        }


	while(1) {

		/* Address of device, and we want to write */
		buffer[0]= 0x70<<1 | 0x0;

		/* Command 0x0, address = 0x0 */
		buffer[1]=0x0;

		/* Turn on BEER*/

                buffer[2]=B;
                buffer[4]=E;
                buffer[6]=0x00;
                buffer[8]=E;
                buffer[10]=R;

		result=write_i2c(buffer,17);

		usleep(500000);


		/* Address of device, and we want to write */

		buffer[0]= 0xe0;


		/* Command 0x0, address = 0x0 */

		buffer[1]=0x0;

		/* Turn on IS */
                buffer[2]=0x00;
                buffer[4]=I;
                buffer[6]=0x00;
                buffer[8]=S;
                buffer[10]=0x00;

		result=write_i2c(buffer,17);

		usleep(500000);
		/* Address of device, and we want to write */

		buffer[0]= 0xe0;


		/* Command 0x0, address = 0x0 */

		buffer[1]=0x0;

		/* Turn on GUD*/
                buffer[2]=G;
                buffer[4]=U;
                buffer[6]=0x00;
                buffer[8]=D;
                buffer[10]=0x00;

		result=write_i2c(buffer,17);

		usleep(500000);

	}

	return 0;

}
