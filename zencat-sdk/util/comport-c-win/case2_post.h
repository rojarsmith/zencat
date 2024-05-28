#ifndef CASE2_POST_INCLUDED
#define CASE2_POST_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include "main.h"
#include "rs232.h"
#include "esp32atagent.h"

void case2_post();

// void sendAT(const char *cmd, unsigned char *buff, int com_port);

void case2_post()
{
    // esp32_status = IDLE;

    // int com_port = COM_PORT;
    // int baudrate = BAUDRATE;
    // char mode[] = {'8', 'N', '1', 0};
    esp32_at_agent_set_config(COM_PORT, BAUD_RATE, '8', 'N', '1', 0);

    // int n = 0;

    // unsigned char send_buf[SEND_BUF_SIZE + 1] = {0};
    // unsigned char recv_buf[RECV_BUF_SIZE + 1] = {0};

    /* Open COM Port */
    //     if (RS232_OpenComport(com_port, baudrate, mode, 0))
    //     {
    //         printf("Can not Open COM Port Success\n\n");
    //         return;
    //     }
    esp32_at_agent_initial();

    //     // strcpy(send_buf, AT_RST);
    //     // RS232_cputs(com_port, send_buf);
    //     // esp32_status = WAIT_RESPOSE;
    //     sendAT((const char *)AT_RST, send_buf, com_port);

    //     while (WAIT_RESPOSE)
    //     {
    //         n = RS232_PollComport(com_port, recv_buf, RECV_BUF_SIZE);

    //         if (n > 0)
    //         {
    //             recv_buf[n] = 0; // always put a "null" at the end of a string!

    //             printf("received %i bytes: %s\n", n, (char *)recv_buf);

    //             for (int i = 0; i <= RECV_BUF_SIZE; i++)
    //             {
    //                 printf("[%02x]", recv_buf[i]);

    //                 if (i == RECV_BUF_SIZE)
    //                 {
    //                     printf("\n");
    //                 }
    //             }
    //         }

    //         if (recv_buf[n - 2] == '\r' && recv_buf[n - 1] == '\n')
    //         {
    //             esp32_status = IDLE;
    //         }

    // #ifdef _WIN32
    //         Sleep(1000);
    // #else
    //         usleep(1000000); /* sleep for 1 Second */
    // #endif
    //     }
    esp32_at_agent_receive();
    if (esp32_at_agent_response_status() == 1)
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive();
    if (esp32_at_agent_response_status() == 1)
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive();
    if (esp32_at_agent_response_status() == 1)
    {
        printf("response: %s", esp32_at_agent_response());
    }
}

// void sendAT(const char *cmd, unsigned char *buff, int com_port)
// {
//     strcpy(buff, cmd);
//     RS232_cputs(com_port, buff);
//     esp32_status = WAIT_RESPOSE;
// }

#endif