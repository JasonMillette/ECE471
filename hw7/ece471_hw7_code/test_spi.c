#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

int main(int argc, char **argv) {

        int spi_fd;
        int lsb_mode = 0;
        int mode=SPI_MODE_0;
        int result;
        struct spi_ioc_transfer spi;
        unsigned char data_out[LENGTH] = { 0x1, 0x2, 0x3};
        unsigned char data_in[LENGTH];

	/* Open SPI device */
        spi_fd = open("/dev/spidev0.0", O_RDWR);        //open with read and write privelages
        if (spi_fd < 0) {
            printf("Error opening SPI device\n");
            return -1;
        }

	/* Set SPI Mode_0 */
        result = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);     //setup SPI
        if (result < 0) {
            printf("Error in setting SPI mode 0\n");
            return -1;
        }
        result = ioctl(spi_fd, SPI_IOC_WR_LSB_FIRST, &lsb_mode);
        if (result < 0) {
            printf("Error \n");
            return -1;
        }

        // clearing transmit buffer
        memset(&spi, 0, sizeof(struct spi_ioc_transfer));   //clear buffer to elimante potential errors

        //Setup full-duplex transfer of 3 bytes
        spi.tx_buf = (unsigned long)&data_out;
        spi.rx_buf = (unsigned long)&data_in;
        spi.len = LENGTH;
        spi.delay_usecs = 0;
        spi.speed_hz = 100000;
        spi.bits_per_word = 8;
        spi.cs_change = 0;

	/* Loop forever printing the CH0 and CH1 Voltages 	*/
	/* Once per second.					*/
        while (1) {


            data_out[2] = 0x00;         //setting for channel 0
            data_out[1] = 0x80;
            data_out[0] = 0x01;

            result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);   //gets data from ADC
            if (result < 0) {
                printf("Error in reading from CH1\n");
                return -1;
            }

            int value = data_in[2] + ((data_in[1] & 0x3) << 8);     //converts from LSB
            float vin = value * 3.3 /1024;                          //converts to voltage
            printf("CH0=%f\n",vin);                                 //prints voltage

            data_out[2] = 0x00;         //setting for channel 1
            data_out[1] = 0x90;
            data_out[0] = 0x01;

            result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);   //gets data from ADC
            if (result < 0) {
                printf("Error in reading from CH1\n");
                return -1;
            }

            value = data_in[2] + ((data_in[1] & 0x3) << 8);     //converts from LSB
            vin = value * 3.3 /1024;                          //converts to voltage
            printf("CH1=%f\n",vin);                                 //prints voltage
            usleep(1000000);                                        //1 second delay
        }


	return 0;
}
