#include <stdio.h>
#include "rs232.h"
#include "atagent.h"

struct at_agent *at_ag;

unsigned char recv_buf[RECV_BUF_SIZE + 1] = {0};

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
    int n = 0;

    n = RS232_PollComport(
        at_ag->com_port,
        recv_buf,
        RECV_BUF_SIZE);

    if (n > 0)
    {
        recv_buf[n] = 0; // always put a "null" at the end of a string!

        printf("[received] %i bytes: %s\n", n, (char *)recv_buf);
    }
}