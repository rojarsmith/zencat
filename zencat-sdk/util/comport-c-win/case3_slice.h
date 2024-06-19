#ifndef CASE3_SLICE_INCLUDED
#define CASE3_SLICE_INCLUDED

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "rs232.h"
#include "atagent.h"

struct at_agent esp32;

void case3_slice();

void case3_slice()
{
    struct at_agent *e32 = &esp32;
    e32->h_type = ESP32;
    e32->com_port = COM_PORT;
    e32->baud_rate = BAUD_RATE;
    e32->u_mode = (struct uart_mode *)malloc(sizeof(struct uart_mode));
    e32->u_mode->data_bits = '8';
    e32->u_mode->parity = 'N';
    e32->u_mode->stop_bits = '1';
    e32->u_mode->flow_ctrl = 0;
    atag_set_config(e32);

    atag_initial();

    unsigned char *cmd = atag_cmd(AT_RST);
    printf("cmd: %s", cmd);
    atag_send(cmd);
    atag_receive(4);
    if (atag_get_response_status())
    {
        printf("response: %s", atag_get_response());
    }
    atag_receive(46);
    if (atag_get_response_status())
    {
        printf("response: %s", atag_get_response());
    }

    while (1)
    {
        // atag_receive();
        // if (atag_get_response_status())
        // {
        //     printf("response: %s", atag_get_response());
        // }
    }
}

#endif