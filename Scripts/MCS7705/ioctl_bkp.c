/* INCLUDE FILE DECLARATIONS */
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <linux/ppdev.h>
#include <pthread.h>
#include <errno.h>

#define SET_DATA_DIR_IN		0x20
#define SET_DATA_DIR_OUT	0x0F

/* LOCAL VARIABLES DECLARATIONS */

inline unsigned long STR_TO_U8(const char *cp)
{
	int result = 0,value;
	int offset = 4;
	unsigned int base;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result + (value << offset);
		cp++;
		offset = offset - 4;
	}

	return result;
}

int main(int argc, char* argv[])
{

	char	dev[16];
	int	select;
	int	devfd;
	int 	ret;

	unsigned char	return_value;

	char		input_value[5];
	unsigned char	write_value;

	int		data_direction;

	while (1) {

		printf("Please input the port of MCS7705. (ex. /dev/parport0):");
		scanf("%s", dev);

		if (memcmp(dev, "/dev/parport", 12) != 0)
			printf("Wrong input!!\n");
		else
			break;
	}
	
	//memcpy(dev, "/dev/parport0", 14);

	devfd = open(dev, O_RDWR);
	
	if (devfd == -1) {
		printf("Can't open %s\n", dev);
		return 0;
	}

	while (1) {

		printf("\nPlease specify the operation of %s.\n", dev);
		printf("0  : Read  Status Register\n");
		printf("1  : Write Control Register\n");
		printf("2  : Read  Control Register output value\n");
		printf("3  : Data Pin write (Data direction setup to output)\n");
		printf("4  : Data Pin read  (Data direction setup to input)\n");
		printf("99 : Exit\n");
		printf(":");
		scanf("%d", &select);


 		if (select == 99) {
			return 0;
		}

		return_value = 0;
		write_value = 0;

		//Claim parport
		ret = ioctl(devfd, PPCLAIM);
		if (ret < 0) {
			printf("\nret = %d, errno = %d, Failed to claim the parport!!\n\r", ret, errno);
			continue;
		}

		if (select == 0) { //Read Status Register
			
			//Read Status
			ret = ioctl(devfd, PPRSTATUS, &return_value);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Read Status Register = 0x%X\n\r", return_value);
				printf("---Bit[7] nBUSY = %d\n", (return_value >> 7) & 0x01);
				printf("---Bit[6]  nACK = %d\n", (return_value >> 6) & 0x01);
				printf("---Bit[5]    PE = %d\n", (return_value >> 5) & 0x01);
				printf("---Bit[4]  SLCT = %d\n", (return_value >> 4) & 0x01);
				printf("---Bit[3] FAULT = %d\n", (return_value >> 3) & 0x01);
			}

		} else if (select == 1) { //Write Control Register

			//User input
			printf("Please input the value you want to write in hex format (ex. 0x5A):");
			scanf("%s", input_value);
			write_value = STR_TO_U8(input_value);

			//Change data direction
			data_direction = (write_value >> 5) & 0x01; //1:Input mode 0:Output mode
			ret = ioctl(devfd, PPDATADIR, &data_direction);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to change data direction!!\n\r", ret, errno);
				continue;
			}

			//Write Control
			ret = ioctl(devfd, PPWCONTROL, &write_value);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Write Control Register 0x%X success\n\r", write_value);
				printf("---Bit[5]     DIR = %d\n", (write_value >> 5) & 0x01);
				printf("---Bit[3] nSLCTIN = %d\n", (write_value >> 3) & 0x01);
				printf("---Bit[2]     INT = %d\n", (write_value >> 2) & 0x01);
				printf("---Bit[1] nAUTOFD = %d\n", (write_value >> 1) & 0x01);
				printf("---Bit[0] nSTROBE = %d\n", (write_value >> 0) & 0x01);
			}

			/*//Debug for Control Register------------------------------------------------------------------
			ret = ioctl(devfd, PPRCONTROL, &return_value);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Check Control Register = 0x%X\n\r", return_value);
			}
			*///--------------------------------------------------------------------------------------------

		} else if (select == 2) { //Read Control Register output value
			
			//Read Control
			ret = ioctl(devfd, PPRCONTROL, &return_value);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Read Control Register = 0x%X\n", return_value);
				printf("---Bit[5]     DIR = %d\n", (return_value >> 5) & 0x01);
				printf("---Bit[3] nSLCTIN = %d\n", (return_value >> 3) & 0x01);
				printf("---Bit[2]     INT = %d\n", (return_value >> 2) & 0x01);
				printf("---Bit[1] nAUTOFD = %d\n", (return_value >> 1) & 0x01);
				printf("---Bit[0] nSTROBE = %d\n", (return_value >> 0) & 0x01);
			}

		} else if (select == 3) { //Data Pin write (Data direction setup to output)

			while (1) {
				printf("\nPlease specify the operation of %s.\n", dev);
				printf("0  : Read  Data Pin output value\n");
				printf("1  : Write Data Pin\n");
				printf("99 : Back\n");
				printf(":");
				scanf("%d", &select);

				if (select == 99) {
					break;
				}

				//Change data direction 
				data_direction = 0; //Output mode
				ret = ioctl(devfd, PPDATADIR, &data_direction);
				if (ret < 0) {
					printf("\n---ret = %d, errno = %d, Failed to change data direction!!\n\r", ret, errno);
					continue;
				}

				/*//Debug for Control Register------------------------------------------------------------------
				ret = ioctl(devfd, PPRCONTROL, &return_value);
				if (ret < 0) {
					printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
				} else {
					printf("\n---Check Control Register = 0x%X\n\r", return_value);
				}

				if ((return_value & SET_DATA_DIR_IN))
					printf("\n---Can not setup Control Register DIR bit to output mode!!\n\r");
				*///--------------------------------------------------------------------------------------------

				if (select == 0) { //Read Data Pin output value
					//Read Data Register
					ret = ioctl(devfd, PPRDATA, &return_value);
					if (ret < 0) {
						printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
						continue;
					} else {
						printf("\n---Read Data Pin (D7-D0) = 0x%X = 2'b %d%d%d%d %d%d%d%d\n\r",
							return_value, (return_value >> 7) & 0x01, (return_value >> 6) & 0x01,
							(return_value >> 5) & 0x01, (return_value >> 4) & 0x01, (return_value >> 3) & 0x01,
							(return_value >> 2) & 0x01, (return_value >> 1) & 0x01, (return_value >> 0) & 0x01);
					}
				} else if (select == 1) {
					//User input
					printf("Please input the value you want to write in hex format (ex. 0x5A):");
					scanf("%s", input_value);
					write_value = STR_TO_U8(input_value);

					//Write Data
					ret = ioctl(devfd, PPWDATA, &write_value);
					if (ret < 0) {
						printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
						continue;
					} else {
						printf("\n---Write Data Pin 0x%X = 2'b %d%d%d%d %d%d%d%d success\n\r",
							write_value, (write_value >> 7) & 0x01, (write_value >> 6) & 0x01,
							(write_value >> 5) & 0x01, (write_value >> 4) & 0x01, (write_value >> 3) & 0x01,
							(write_value >> 2) & 0x01, (write_value >> 1) & 0x01, (write_value >> 0) & 0x01);
					}

					/*//Debug for data output------------------------------------------------------------------
					ret = ioctl(devfd, PPRDATA, &return_value);
					if (ret < 0) {
						printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
					} else {
						printf("\n---Check Data Pin (D7-D0) = 0x%X\n\r", return_value);
					}
					*///--------------------------------------------------------------------------------------------

				}
			}
		} else if (select == 4) { //Data Pin read  (Data direction setup to input)

			usleep(5000);

			//Change data direction 
			data_direction = 1; //Input mode
			ret = ioctl(devfd, PPDATADIR, &data_direction);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to change data direction!!\n\r", ret, errno);
				continue;
			}

			/*//Debug for Control Register------------------------------------------------------------------
			ret = ioctl(devfd, PPRCONTROL, &return_value);
			if (ret < 0) {
				printf("\nr---et = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Check Control Register = 0x%X\n\r", return_value);
			}

			if (!(return_value & SET_DATA_DIR_IN))
				printf("\n---Can not setup Control Register DIR bit to input mode!!\n\r");
			*///--------------------------------------------------------------------------------------------

			//Read Data Register
			ret = ioctl(devfd, PPRDATA, &return_value);
			if (ret < 0) {
				printf("\n---ret = %d, errno = %d, Failed to access parport register!!\n\r", ret, errno);
			} else {
				printf("\n---Read Data Pin (D7-D0) = 0x%X = 2'b %d%d%d%d %d%d%d%d\n\r",
					return_value, (return_value >> 7) & 0x01, (return_value >> 6) & 0x01,
					(return_value >> 5) & 0x01, (return_value >> 4) & 0x01, (return_value >> 3) & 0x01,
					(return_value >> 2) & 0x01, (return_value >> 1) & 0x01, (return_value >> 0) & 0x01);
			}
		}

		//Release parport
		ret = ioctl(devfd, PPRELEASE);
		if (ret < 0) {
			printf("\nret = %d, errno = %d, Failed to release the parport!!\n\r", ret, errno);
			continue;
		}	
	}

	printf("\n");

	return 0;
}
