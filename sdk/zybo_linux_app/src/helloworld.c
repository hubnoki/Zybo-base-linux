/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

int main(int argc, char **argv)
{
    printf("Hello World!\n");

    int fd;

    /* 1. GPIO900を使用可能にする */
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "900", 3);
    close(fd);

    /* 2. GPIO900を出力設定する */
    fd = open("/sys/class/gpio/gpio900/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    /* 3. GPIO900に1(High)を出力する */
    fd = open("/sys/class/gpio/gpio900/value", O_WRONLY);
    write(fd, "1", 1);

    while(1) {
        write(fd, "0", 1);
        usleep(1*1000*1000);
        write(fd, "1", 1);
        usleep(1*1000*1000);
    }

    /* 4. 使い終わったので閉じる */
    close(fd);

    return 0;
}


