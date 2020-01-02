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

#define REG(address) *(volatile unsigned int*)(address)

int main(int argc, char **argv)
{
    printf("Hello World!\n");

    int address;    /* GPIOレジスタへの仮想アドレス(ユーザ空間) */
    int fd;

    /* メモリアクセス用のデバイスファイルを開く */
    if ((fd = open("/dev/uio0", O_RDWR | O_SYNC)) < 0) {
        perror("open");
        return -1;
    }

    /* ARM(CPU)から見た物理アドレス → 仮想アドレスへのマッピング */
    address = (int)mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    REG(address + 0x04) = 0x0; // GPIO_TRI register

    while(1) {
        /* Set LEDs(PL GPIO) as High */
        REG(address) = 0x0F;
        usleep(1*1000*1000);
        /* Set LEDs(PL GPIO) as Low */
        REG(address) = 0x00;
        usleep(1*1000*1000);
    }

    /* 使い終わったリソースを解放する */
    munmap((void*)address, 0x1000);
    close(fd);

    return 0;
}

