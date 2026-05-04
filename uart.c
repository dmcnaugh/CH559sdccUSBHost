
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "uart.h"
#include "USBHost.h"

#define MAJOR 0x02
#define MINOR 0x01
#define PATCH 0x00

const unsigned char version[] = { 0xAA, 0x55, 'V', 'E', 'R', MAJOR, MINOR, PATCH };

uint8_t __xdata uartRxBuff[64];
uint8_t rxPos = 0;

extern void resetInit();
extern void checkDeviceStatus();
extern void showDeviceInfo();

void processUart(){
	register unsigned char c;

    while(RI){
            RI=0;
			c = SBUF;
            // uartRxBuff[rxPos] = SBUF;
			// DEBUG_OUT("UART: '%c' [%02X]", uartRxBuff[rxPos], uartRxBuff[rxPos])
			// if (uartRxBuff[rxPos]!='\n') uartRxBuff[rxPos] += 0x80;
            if (c =='\n' || rxPos >= 4){
                // for (uint8_t i = 0; i < rxPos; i ++ )
                //     {
                //         printf( "0x%02X ",uartRxBuff[i]);
                //     }
                //     printf("\n");
				c = *uartRxBuff;
                if(c==('I' + 0x80)) {
					putchar('U');
					checkDeviceStatus();
					putchar('V');
					putchar('\n');
				}
                else if(c==('D' + 0x80)) {
					putchar('E');
					showDeviceInfo();
					putchar('\n');
				}
                else if(c==('V' + 0x80)) {
					putchar('W');
					putchar(MAJOR + 0x80);
					putchar(MINOR + 0x80);
					putchar(PATCH + 0x80);
					putchar('\n');
				}
                else if(c==('R' + 0x80)){
					resetInit();
				}
                else if(c==('L' + 0x80)){
                //if(uartRxBuff[1]==0x61)LED=0;
                //if(uartRxBuff[1]==0x73)LED=1;
                // if(uartRxBuff[1]=='b')runBootloader();
					// sendProtocolMSG(MSG_TYPE_ERROR,0, uartRxBuff[1], 0, 0xEE, 0);
					// setHIDDeviceReport(0, uartRxBuff[1] & 0x1f);
					setHIDkbLeds(uartRxBuff[1] & 0x1f);
                }
            	rxPos=0;
				*uartRxBuff = 0;
            }else{
				uartRxBuff[rxPos] = c;
				rxPos++;
            }
        }
}

void sendProtocolMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer){
    unsigned short i;

	return;

    putchar(0xFE);	
	putchar(length);
	putchar((unsigned char)(length>>8));
	putchar(msgtype);
	putchar(type);
	putchar(device);
	putchar(endpoint);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	for (i = 0; i < length; i++)
	{
		putchar(msgbuffer[i]);
	}
	putchar('\n');
}

void sendHidPollMSG(unsigned char msgtype, unsigned short length, unsigned char type, unsigned char device, unsigned char endpoint, unsigned char __xdata *msgbuffer,unsigned char idVendorL,unsigned char idVendorH,unsigned char idProductL,unsigned char idProductH){
    unsigned short i;
    unsigned short a = 0xFE;

	if ((msgtype == 4) && (type == 6)) {
		putchar(a);	
		// putchar(length);
		// putchar((unsigned char)(length>>8));
		// putchar(msgtype);
		// putchar(type);
		// putchar(device);
		// putchar(endpoint);
		// putchar(idVendorL);
		// putchar(idVendorH);
		// putchar(idProductL);
		// putchar(idProductH);
		for (i = 0; i < length; i++)
		{
			a += msgbuffer[i];
		}
		a += '\n';

		msgbuffer[1] = 256 - (a % 256); // Zero total checksum in spare byte
		
		for (i = 0; i < length; i++)
		{
			putchar(msgbuffer[i]);
		}
		putchar('\n');
	}
}