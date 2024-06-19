#include <stdio.h>
#include "rs232.h"
#include "atagent.h"

struct at_agent *at_ag;

unsigned char recv_buf[RECV_BUF_SIZE + 1] = {0};
unsigned char resp_buf[RESP_BUF_SIZE + 1] = {0};

int atag_set_config(struct at_agent *agent)
{
    at_ag = agent;

    return 0;
}

int atag_initial()
{
    if (RS232_OpenComport(
            at_ag->com_port,
            at_ag->baud_rate,
            (char[]){
                at_ag->u_mode->data_bits,
                at_ag->u_mode->parity,
                at_ag->u_mode->stop_bits,
                at_ag->u_mode->flow_ctrl},
            0))
    {
        printf("[SYS]Can not Open COM Port Success\n");
        return -1;
    }

    printf("[SYS]Open COM Port Success\n");

    return 0;
}

int atag_receive()
{
    int resp_idx = 0;
    int n = 0;
    int retry = 0;
    int brk = 0;
    at_ag->msg_received = 0;

    while (retry <= RETRY_COUNT)
    {
        n = RS232_PollComport(
            at_ag->com_port,
            recv_buf,
            RECV_BUF_SIZE);

        if (n > 0)
        {
            recv_buf[n] = 0; // always put a "null" at the end of a string!

            printf("[received] %i bytes: %s\n", n, (char *)recv_buf);

            for (int i = 0; i < n; i++)
            {
                printf("[%02x]", recv_buf[i]);
                if (i == (n - 1))
                {
                    printf("\n");
                }

                resp_buf[resp_idx] = recv_buf[i];
                if (resp_buf[resp_idx - 1] == '\r' &&
                    resp_buf[resp_idx] == '\n' &&
                    resp_idx != 1) // Special ACK: "\r\nready\r\n"
                {
                    resp_buf[resp_idx + 1] = 0;
                    brk = 1;
                }

                resp_idx++;
            }
        }
        else
        {
#ifdef _WIN32
            Sleep(1000);
#else
            usleep(1000000); /* sleep for 1 Second */
#endif
            retry++;
        }

        if (brk == 1)
        {
            at_ag->msg_received = 1;
            printf("[response] %i bytes: %s\n", resp_idx, (char *)resp_buf);
            break;
        }
    }

    if (brk == 0)
    {
        return -1;
    }

    return resp_idx;
}

int atag_get_response_status()
{
    return at_ag->msg_received;
}

unsigned char *atag_get_response()
{
    return resp_buf;
}