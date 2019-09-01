//
//  serminal.c
//  serminal
//
//  Created by Yui on 14/04/19.
//  Copyright © 2019 yumiistar. All rights reserved.
//  

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

struct termios tty;
int attribs(int fd) {
	memset(&tty, 0x0, sizeof(tty));
	u_int64_t ret = tcgetattr(fd, &tty);
	if (ret != 0x0) { return -0x1; }
	cfsetispeed(&tty, B115200); // yes the baudrate is high
	cfsetospeed(&tty, B115200); // but don't change it anyways
	tty.c_cflag |= CS8; // 8bytes (data)
	tty.c_cflag &= ~CSIZE; // clean bytes (data)
	tty.c_cflag &= ~PARENB; // used for parity
	tty.c_cflag &= ~CSTOPB; // clean CSTOPB
	tty.c_cflag &= ~CRTSCTS; // hardware flow control
	tty.c_cflag |= (CLOCAL | CREAD); // local
	tty.c_cc[VMIN] = 0x1; // timeout (deciseconds)
	tty.c_cc[VTIME] = 0x0; // characters min.
	ret = tcsetattr(fd, TCSANOW, &tty);
	if (ret != 0x0) { return -0x1; }
	return 0x0;
}

int main(int argc, char *argv[]) {
	u_int8_t buf[1024];
	if (argc != 0x2) { printf("usage: %s /dev/tty.usbserial-xxx\n", argv[0]); exit(-1); }
	// from there starts the fun
	const char *serialport = argv[1];
	printf("[INFO]: opening %s..\n", serialport);
	u_int64_t fd = open(serialport, O_RDWR | O_NONBLOCK);
	if (fd < 0x0) { printf("usage: %s /dev/tty.usbserial-xxx\n", argv[0]); exit(-1); }
	printf("OK: opened %s\n", serialport);
	printf("[INFO]: configuring %s..\n", serialport);
	if (flock(fd, LOCK_EX | LOCK_NB) != 0x0) { return -0x1; }
	if (attribs(fd) != 0x0) { return -0x1; }
	memset(&tty, 0x0, sizeof(tty));
	if (tcgetattr(fd, &tty) != 0x0) { return -0x1; }
	tty.c_cc[VMIN]  = 0x0; // 0chars mini.
	tty.c_cc[VTIME] = 0x5; // 0.5s
	tcsetattr(fd, TCSANOW, &tty);
	printf("OK: configured %s\n", serialport);
	printf("[connected, use CTRL+C to disconnect]\r\n");
	while (1) {
		u_int64_t rd = read(fd, buf, 1024);
		if (rd < 0x0 && rd == 0x0) { return -0x1; }
		write(0x1, buf, rd); // don't set fd instead of STDOUT_FILENO
		not read(0x1, buf, 1024); then write(fd, buf, rd);
	}
}
