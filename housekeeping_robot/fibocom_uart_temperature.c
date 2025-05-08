/******************************************************************************
Copyright (C) 2018-2019 AITOS.IO

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
����
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "fibofwk.h"
#include "fibo_info_interface.h"
#include "fibo_mdc_interface.h"
#include "fibo_mrc_interface.h"
#include "fibo_sim_interface.h"
#include "fibo_mcc_interface.h"
#include "fibo_print.h"
#include "fibo_aps_interface.h"
#include "fibo_atClient_interface.h"
#include "fibo_atDefs_interface.h"


#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>

typedef char CHAR;
typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef int SINT32;


// Uncomment following line if it's on target board
#define ON_TARGET

#ifdef ON_TARGET

// APIs defined on target board
//extern int ReadUART(void);

#else

int DemoReadUART(void) { return 0; }
#endif

#define SECONDS_BEYOND_UTC(timezone) ((timezone)*3600L)

volatile int g_uart_trace_exit_signal = 0;
struct sigaction g_uart_trace_sigaction;


static int g_uart_fd = -1;
static UINT8 g_uart_buf[1024];

#define TTY_DEVICE_NAME "/dev/ttyS0"

extern const char *g_ac_robot_address;


SINT32 CallUsdcTransfer(char * to_address, UINT32 value);

SINT32 OpenUART(int *fd_ptr, const CHAR* device_str)
{
    fibo_result_t fibo_ret = FIBO_OK;
    int tty_fd;

    if( fd_ptr == NULL || device_str == NULL)
    {
        printf("Arguments cannot be NULL.");
        return -1;
    }
    
    fibo_ret = fibo_aps_TryConnectService();

    if( fibo_ret == FIBO_UNAVAILABLE )
    {
        printf("fibo_aps_TryConnectService() returns FIBO_UNAVAILABLE");
        return -1;
    }
    else if( fibo_ret == FIBO_NOT_PERMITTED )
    {
        printf("fibo_aps_TryConnectService() returns FIBO_NOT_PERMITTED");
        return -1;
    }

    fibo_aps_SetUARTSwitch(0);


    if ((tty_fd = fibo_tty_Open(device_str, O_RDWR)) < 0)
    {
        printf("Open UART fail.");
        return -1;
    }


    fibo_tty_SetRaw(tty_fd, sizeof(g_uart_buf)/2, 5);

    fibo_tty_SetBaudRate(tty_fd, FIBO_TTY_SPEED_9600);

    fibo_tty_SetFraming(tty_fd, 'N', 8, 1);

    fibo_tty_SetFlowControl(tty_fd, FIBO_TTY_FLOW_CONTROL_NONE);

    *fd_ptr = tty_fd;

    return 0;
}

SINT32 CloseUART(int fd)
{

    fibo_tty_Close(fd);
    fibo_aps_DisconnectService();

    return 0;
}


SINT32 ReadUART(int fd, UINT8 *buf_ptr, UINT32 size)
{
    SINT32 read_count;
    SINT32 buf_offset = 0;

    while(buf_offset < size)
    {
        if( g_uart_trace_exit_signal == 1 )  return -1;
	
        read_count = read(fd, buf_ptr + buf_offset, size);

        if( read_count == -1)  return -1;

        buf_offset += read_count;

    }

    return 0;
}


SINT32 WriteUART(int fd, const UINT8 *buf_ptr, UINT32 size)
{
    SINT32 write_count;
    SINT32 buf_offset = 0;

    while(buf_offset < size)
    {
        if( g_uart_trace_exit_signal == 1 )  return -1;
	
        write_count = write(fd, buf_ptr + buf_offset, size);

        if( write_count == -1)  return -1;

        buf_offset += write_count;

    }

    return 0;
}



//CTRL-C:exit main process.
void uart_trace_handle_signal(int signum)
{
    g_uart_trace_exit_signal = 1;
    printf("uart_trace_handle_signal() is called.\n");
    fflush(stdout);
}



void showhex(char *pt, char *pd, int len)
{
			printf("%s: ", pt);

			for(int t=0; t < len; t++)
			{
				printf("%02x ", pd[t]);
			}
			printf("\n");
}
char g_countTemperature = 0;
char g_countTransfer = 0;
void checkBanlanceLoop();
#define SAMPLINGINTERVAL 1
#define TRANSFERINTERVAL 60/SAMPLINGINTERVAL*2
void *ReadUARTThreadEntry(void *arg)
{
    SINT32 result;
    int sig_result;
    
    SINT32 read_count;
    UINT8 poll_cmd[] = {0xA8, 0x01, 0xCC};
    UINT8 poll_response[] = {0xA8, 0x01, 0x33};

    #define POLL_STATUS_OK 0
    #define POLL_STATUS_NOT_READY 1
    #define STATUS_ERR_SIGINT 10
    #define STATUS_ERR_EXAUSTED_BUF 20
    #define STATUS_ERR_UART 30
    UINT8 poll_status = POLL_STATUS_NOT_READY;

    UINT8 query_cmd[] = {0xA8, 0x01, 0xAA};
    SINT32 uart_buf_offset;
    SINT32 poll_window_left_edge;
    SINT32 i;

    
    //signal-CTRL-C:exit main process.
    g_uart_trace_exit_signal = 0;
    //signal(SIGINT, uart_trace_handle_signal);
    g_uart_trace_sigaction.sa_handler = uart_trace_handle_signal;
    g_uart_trace_sigaction.sa_flags = 0;
    sig_result = sigaction(SIGINT, &g_uart_trace_sigaction, NULL);
    printf("sig_result = %d\n", sig_result);


    printf("OpenUART()\n");

    result = OpenUART(&g_uart_fd, TTY_DEVICE_NAME);
    if( result != 0 ) 
    {
        printf("OpenUART() fails.\n");
        return NULL;
    }


    // Polling for existence of thermal sensor
    uart_buf_offset = 0;
    poll_window_left_edge = 0;
    
    printf("Polling thermal sensor\n");
    
    // Loop for Sending Polling Command + Receive Polling Response
    char jumpCounter = 0;
    while(1)
    {
        // Send poll command
        printf("Sending Polling Command to UART\n");
        result = WriteUART(g_uart_fd, poll_cmd, sizeof(poll_cmd));
        if( result != 0 )
        {
            poll_status = STATUS_ERR_UART;
            break;
        }

        // Receive response from sensor with a slide window
        
        
        // Loop for reading out all data in UART buffer and search for Polling
        // Response pattern. The loop exits either no more data is in buffer or
        // a Polling Response is matched.

        while(1)
        {
            if( g_uart_trace_exit_signal == 1 )
            {
                poll_status = STATUS_ERR_SIGINT;
                break;
            }

            printf("Waiting for Polling Response\n");
            read_count = read(g_uart_fd, g_uart_buf + uart_buf_offset, 3);

            if( read_count == -1 )
            {
                poll_status = STATUS_ERR_UART;
				printf("\n   Uart errr\n");
                break;
            }

            if( read_count == 0 )
            {
                poll_status = POLL_STATUS_NOT_READY;
				break;
            }


            printf("Receive Polling Response: ");
            for( i = 0; i < read_count; i++)
            {
                printf("%02X ", g_uart_buf[uart_buf_offset + i]);
            }
            printf("\n\n");


            uart_buf_offset += read_count;

            if( uart_buf_offset - poll_window_left_edge >= 3 )
            {
                for( i = 0; i <= uart_buf_offset - poll_window_left_edge - 3; i++)
                {
                    if( memcmp(g_uart_buf + poll_window_left_edge + i, poll_response, 3) == 0)
                    {
                        poll_status = POLL_STATUS_OK;
                        printf("Polling Response is matched.\n");

                        // Clean up UART driver buffer if any data were still in.
                        // This makes sure following Query Response is aligned at frame edge.
                        read(g_uart_fd, g_uart_buf, sizeof(g_uart_buf));

                        break;
                    }
                }

                poll_window_left_edge += i;
            }

            if( poll_status == POLL_STATUS_OK )
            {
                break;
            }

            if( uart_buf_offset >= sizeof(g_uart_buf) - 3)
            {
                poll_status = STATUS_ERR_EXAUSTED_BUF;
                break;
            }

        }

        if( poll_status != POLL_STATUS_NOT_READY )
        {
            break;
        }

        // Interval between polling
        sleep(1);
    }

    if( poll_status == POLL_STATUS_OK )
    {
        while(1)
        {
            // Send query command
            //printf("Sending Query Command to UART\n");
            result = WriteUART(g_uart_fd, query_cmd, sizeof(query_cmd));
            if( result != 0 )
            {
                break;
            }

        
            // Receive temperature data from sensor
            // The data frame is:
            // |A8 01 XX NM YZ|
            // Where,
            // if XX == 0x00, the temperature is above 0 degree,
            // if XX == 0x80, the temperature is below 0 degree.
            // The temperature is represented in BCD: "NM.YZ" degree.

            result = ReadUART(g_uart_fd, g_uart_buf, 5);
            if( result != 0 )  break;
/*
            printf("Receive Query Response: ");
            for( i = 0; i < 5; i++)
            {
                printf("%02X ", g_uart_buf[i]);
            }
            printf("\n\n");
*/


			g_countTemperature++;

			if(g_countTransfer > 0) g_countTransfer--;
			else g_countTransfer = 0;
			printf("%02x ", g_uart_buf[3]);	
        	if(g_uart_buf[3] >= 0x40)
        	{
				if(g_countTemperature > 2)
				{
					g_countTemperature = 1;
					if(g_countTransfer == 0)
					{
						g_countTransfer = TRANSFERINTERVAL; // 2 minutes
						CallUsdcTransfer(g_ac_robot_address,10000); // 1usdc=10^6, 10000 = 0.01 usdc
					}
				}	
        	}
			else
			{
				g_countTemperature = 0;
			}



       
            // Sample Interval
            sleep(SAMPLINGINTERVAL);

        }
    }
    else
    {
        printf("Fail to read/write UART\n");
    }
    
    

    CloseUART(g_uart_fd);

    return NULL;
}




