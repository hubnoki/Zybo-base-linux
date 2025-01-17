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
// PS部GPIOのドライバヘッダをインクルード
#include "xgpiops.h"
/*************************************************/

int main()
{
/*************************************************/
	// PS部GPIOの設定の構造体
	XGpioPs_Config *ConfigPtr;
	// PS部GPIOのインスタンス
	XGpioPs Gpio;
/*************************************************/

	init_platform();

/*************************************************/
	// 対象のGPIOの設定を参照 (DEVICE IDはxparameters.hに記載されている)
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	// インスタンスを初期化
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
/*************************************************/

	print("Hello World\n\r");

/*************************************************/
	// MIO7を出力ポートに設定
	XGpioPs_SetDirectionPin(&Gpio, 7, 1);
	// MIO7の出力イネーブル
	XGpioPs_SetOutputEnablePin(&Gpio, 7, 1);

	*(unsigned int *)0x41200004 = 0;

	// LED点滅処理
	u32 CurrentLED = 0;
	while(1){
		// LED点滅間隔待ち
		for(int i = 0; i < 10000000; i++){;}

		// LED点灯状態反転
		if(CurrentLED)	*(unsigned int *)0x41200000 = 0xF;
		else			*(unsigned int *)0x41200000 = 0x0;
		XGpioPs_WritePin(&Gpio, 7, (CurrentLED ^= 1));
	}
/*************************************************/

    cleanup_platform();
    return 0;
}
