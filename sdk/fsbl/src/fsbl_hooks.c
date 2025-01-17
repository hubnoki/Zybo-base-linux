/******************************************************************************
*
* Copyright (C) 2012 - 2014 Xilinx, Inc.  All rights reserved.
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

/*****************************************************************************
*
* @file fsbl_hooks.c
*
* This file provides functions that serve as user hooks.  The user can add the
* additional functionality required into these routines.  This would help retain
* the normal FSBL flow unchanged.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 3.00a np   08/03/12 Initial release
* </pre>
*
* @note
*
******************************************************************************/


#include "fsbl.h"
#include "xstatus.h"
#include "fsbl_hooks.h"

#include "xiicps.h"
#include "xemacps.h"

/************************** Variable Definitions *****************************/


/************************** Function Prototypes ******************************/


/******************************************************************************
* This function is the hook which will be called  before the bitstream download.
* The user can add all the customized code required to be executed before the
* bitstream download to this routine.
*
* @param None
*
* @return
*		- XST_SUCCESS to indicate success
*		- XST_FAILURE.to indicate failure
*
****************************************************************************/
u32 FsblHookBeforeBitstreamDload(void)
{
	u32 Status;

	Status = XST_SUCCESS;

	/*
	 * User logic to be added here. Errors to be stored in the status variable
	 * and returned
	 */
	fsbl_printf(DEBUG_INFO,"In FsblHookBeforeBitstreamDload function \r\n");

	return (Status);
}

/******************************************************************************
* This function is the hook which will be called  after the bitstream download.
* The user can add all the customized code required to be executed after the
* bitstream download to this routine.
*
* @param None
*
* @return
*		- XST_SUCCESS to indicate success
*		- XST_FAILURE.to indicate failure
*
****************************************************************************/
u32 FsblHookAfterBitstreamDload(void)
{
	u32 Status;

	Status = XST_SUCCESS;

	/*
	 * User logic to be added here.
	 * Errors to be stored in the status variable and returned
	 */
	fsbl_printf(DEBUG_INFO, "In FsblHookAfterBitstreamDload function \r\n");

	return (Status);
}

/******************************************************************************
* This function is the hook which will be called  before the FSBL does a handoff
* to the application. The user can add all the customized code required to be
* executed before the handoff to this routine.
*
* @param None
*
* @return
*		- XST_SUCCESS to indicate success
*		- XST_FAILURE.to indicate failure
*
****************************************************************************/
u32 FsblHookBeforeHandoff(void)
{
	u32 Status;

	Status = XST_SUCCESS;

	/*
	 * User logic to be added here.
	 * Errors to be stored in the status variable and returned
	 */
	fsbl_printf(DEBUG_INFO,"In FsblHookBeforeHandoff function \r\n");


	/* Read Out MAC Address */
	{
		int Status;
		XIicPs Iic;
		XIicPs_Config *Iic_Config;
		XEmacPs Emac;
		XEmacPs_Config *Mac_Config;

		unsigned char mac_addr[6];
		int i = 0;

		fsbl_printf(DEBUG_GENERAL,"Look Up I2C Configuration\n\r");
		Iic_Config = XIicPs_LookupConfig(XPAR_PS7_I2C_0_DEVICE_ID);
		if(Iic_Config == NULL) {
			return XST_FAILURE;
		}

		fsbl_printf(DEBUG_GENERAL,"I2C Initialization\n\r");
		Status = XIicPs_CfgInitialize(&Iic, Iic_Config, Iic_Config->BaseAddress);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		fsbl_printf(DEBUG_GENERAL,"Set I2C Clock\n\r");
		XIicPs_SetSClk(&Iic, 200000);

		mac_addr[0] = 0xFA;

		fsbl_printf(DEBUG_GENERAL,"Set Memory Read Address\n\r");
		XIicPs_MasterSendPolled(&Iic, mac_addr, 1, 0x50);
		while(XIicPs_BusIsBusy(&Iic));
		fsbl_printf(DEBUG_GENERAL,"Get Mac Address\n\r");
		XIicPs_MasterRecvPolled(&Iic, mac_addr, 6, 0x50);
		while(XIicPs_BusIsBusy(&Iic));

		fsbl_printf(DEBUG_GENERAL,"MAC Addr: ");
		for(i = 0; i < 6; i++) {
			fsbl_printf(DEBUG_GENERAL,"%02x ", mac_addr[i]);
		}
		fsbl_printf(DEBUG_GENERAL,"\n\r");

		fsbl_printf(DEBUG_GENERAL,"Look Up Emac Configuration\n\r");
		Mac_Config = XEmacPs_LookupConfig(XPAR_PS7_ETHERNET_0_DEVICE_ID);
		if(Mac_Config == NULL) {
			return XST_FAILURE;
		}

		fsbl_printf(DEBUG_GENERAL,"Emac Initialization\n\r");
		Status = XEmacPs_CfgInitialize(&Emac, Mac_Config, Mac_Config->BaseAddress);
		if(Status != XST_SUCCESS){
			return XST_FAILURE;
		}

		fsbl_printf(DEBUG_GENERAL,"Set Emac MAC Address\n\r");
		Status = XEmacPs_SetMacAddress(&Emac, mac_addr, 1);
		if(Status != XST_SUCCESS){
			return XST_FAILURE;
		}

		fsbl_printf(DEBUG_GENERAL,"Verify Emac MAC Address\n\r");
		XEmacPs_GetMacAddress(&Emac, mac_addr, 1);
		if(Status != XST_SUCCESS){
			return XST_FAILURE;
		}
	}

	return (Status);
}


/******************************************************************************
* This function is the hook which will be called in case FSBL fall back
*
* @param None
*
* @return None
*
****************************************************************************/
void FsblHookFallback(void)
{
	/*
	 * User logic to be added here.
	 * Errors to be stored in the status variable and returned
	 */
	fsbl_printf(DEBUG_INFO,"In FsblHookFallback function \r\n");
	while(1);
}


