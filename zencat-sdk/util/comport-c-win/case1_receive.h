#ifndef CASE1_RECEIVE_INCLUDED
#define CASE1_RECEIVE_INCLUDED

#include <unistd.h>
#include <stdio.h>
#include "main.h"
#include "rs232.h"

void case1_receive();

void case1_receive()
{
    int state = 0;

    int com_port = COM_PORT;
    int baudrate = BAUD_RATE;
    char mode[] = {'8', 'N', '1', 0};

    int n = 0;

    unsigned char recv_buf[RECV_BUF_SIZE + 1] = {0};

    /* Open COM Port */
    if (RS232_OpenComport(com_port, baudrate, mode, 0))
    {
        printf("Can not Open COM Port Success\n\n");
        return;
    }

    /* Recv Data */
    while (1)
    {
        n = RS232_PollComport(com_port, recv_buf, RECV_BUF_SIZE);

        if (n > 0)
        {
            recv_buf[n] = 0; // always put a "null" at the end of a string!

            // for (i = 0; i < n; i++)
            // {
            //     if (recv_buf[i] < 32) // replace unreadable control-codes by dots
            //     {
            //         recv_buf[i] = '.';
            //     }
            // }

            printf("received %i bytes: %s\n", n, (char *)recv_buf);

            for (int i = 0; i < RECV_BUF_SIZE; i++)
            {
                printf("[%02x]", recv_buf[i]);

                if (i == RECV_BUF_SIZE)
                {
                    printf("\n");
                }
            }
        }
        usleep(100000); // sleep for 100 milliSeconds
    }

    /* Close COM Port */
    RS232_CloseComport(com_port);
    printf("\nClose COM Port\n");
}

#endif // CASE1_RECEIVE_INCLUDED