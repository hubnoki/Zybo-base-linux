/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

/*************************************************/
// PS��GPIO�̃h���C�o�w�b�_���C���N���[�h
#include "xgpiops.h"
/*************************************************/

int main()
{
/*************************************************/
	// PS��GPIO�̐ݒ�̍\����
	XGpioPs_Config *ConfigPtr;
	// PS��GPIO�̃C���X�^���X
	XGpioPs Gpio;
/*************************************************/

	init_platform();

/*************************************************/
	// �Ώۂ�GPIO�̐ݒ���Q�� (DEVICE ID��xparameters.h�ɋL�ڂ���Ă���)
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	// �C���X�^���X��������
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
/*************************************************/

	print("Hello World\n\r");

/*************************************************/
	// MIO7���o�̓|�[�g�ɐݒ�
	XGpioPs_SetDirectionPin(&Gpio, 7, 1);
	// MIO7�̏o�̓C�l�[�u��
	XGpioPs_SetOutputEnablePin(&Gpio, 7, 1);

	// LED�_�ŏ���
	u32 CurrentLED = 0;
	while(1){
		// LED�_�ŊԊu�҂�
		for(int i = 0; i < 10000000; i++){;}

		// LED�_����Ԕ��]
		XGpioPs_WritePin(&Gpio, 7, (CurrentLED ^= 1));
	}
/*************************************************/

    cleanup_platform();
    return 0;
}
