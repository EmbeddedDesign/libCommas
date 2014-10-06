/*
Copyright 2014 SAIFE, Inc.
https://saifeinc.com/

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * Authors: Breck Boven, John Curtis, Stephen Harper
**/

#include "serialunix.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>

#ifndef port
    #define port  "/dev/ttyUSB0"
#endif

#ifndef BAUD_RATE
 #define BAUD_RATE 19200
#endif

#if(BAUD_RATE == 1200)
    #define BAUD B1200
#endif
#if(BAUD_RATE == 2400)
    #define BAUD B2400
#endif
#if(BAUD_RATE == 9600)
    #define BAUD B9600
#endif
#if(BAUD_RATE == 14400)
    #define BAUD B14400
#endif
#if(BAUD_RATE == 19200)
    #define BAUD B19200
#endif
#if(BAUD_RATE == 28800)
    #define BAUD B28800
#endif
#if(BAUD_RATE == 38400)
    #define BAUD B38400
#endif

int fd;

/**
 * serialunix constructor
 * calls open_serial()
 */
serialunix::serialunix()
{
  open_serial();
}

/**
 * Read() attempts to read "max_read" bytes from serial and put the result at "buffer"
 */
uint8_t* serialunix::Read(uint8_t* buffer, const uint8_t max_read)
{
    memset(buffer, 0, max_read);
	read(fd, buffer, max_read);
    usleep(1000000);
    // Currently prints the read buffer every .5 seconds
    printf("\nBR READ BUFF:\n%s\n",buffer );
}

/**
 * Write() attempts to write "length" bytes of data at "buffer" to the serial port
 */
void serialunix::Write(const uint8_t* buffer, const uint8_t length)
{
	write(fd, buffer, length); 
    // printf("BR SEND BUFF:\n%s\n",buffer); 
}

/**
 * Close() closes the serial connection
 * Currently does nothing but its implementation is required by the interface
 */
void serialunix::Close()
{

}

/**
 * set_interface_attribs() sets the serial interface attributes
 */
int serialunix::set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
        return -1;
    }

    /* Set the Baud Rate */
    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);
    
    /* Set the Character Size */
    tty.c_cflag &= ~CSIZE; /* Mask the character size bits */
    tty.c_cflag |= CS8;    /* Select 8 data bits */     // 8-bit chars

    /* Set to 8N1 */
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    /* Ignore Breaks */
    tty.c_iflag &= ~IGNBRK;

    /* Disable hardware flow control */
    tty.c_cflag &= ~CRTSCTS;

    /* Disable software flow control */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Probalably just want raw ouput */
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    /* Save the Settings NOW */
    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * open_serial() attemps to open a serial connection
 */
void serialunix::open_serial(void)
{
    fd = open( port, O_RDWR | O_NOCTTY| O_NDELAY);
	if (fd == -1)
	{
		// perror("open_port: Unable to open a connection - ");
	}
	else
    {
        fcntl(fd, F_SETFL, FNDELAY);
    }
    // 19200 baud
	set_interface_attribs (fd, BAUD, 0);
}