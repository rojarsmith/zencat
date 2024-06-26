#ifndef CASE4_SLICE2_INCLUDED
#define CASE4_SLICE2_INCLUDED

#include <stdio.h>
#include "rs232.h"
#include "at.h"
#include "at_cmd.h"

struct at_agent esp32;

void case4_slice2();
void story_fetch_passthrough();

unsigned char *wait_response(int delay_gain);

void case4_slice2()
{
    printf("[case4_slice2]start.\n");

    struct at_agent *e32 = &esp32;
    e32->h_type = ESP32;
    e32->com_port = COM_PORT;
    e32->baud_rate = BAUD_RATE;
    e32->u_mode = (struct uart_mode *)malloc(sizeof(struct uart_mode));
    e32->u_mode->data_bits = '8';
    e32->u_mode->parity = 'N';
    e32->u_mode->stop_bits = '1';
    e32->u_mode->flow_ctrl = 0;
    e32->cmd_current = NULL;
    e32->cmd_previous = NULL;
    atag_set_config(e32);

    atag_initial();

    story_fetch_passthrough();

    return;

    while (1)
    {
        int resp_l = atag_receive(2);
        printf("[response] len %d .\n", resp_l);
    }
}

void story_fetch_passthrough()
{
    char *cmd = atag_cmd(AT_RESTORE, "?=");
    printf("[cmd]%s", cmd);

    atag_send(atag_cmd("\r\n\r\n\r\n", ""));
    wait_response(2);

    // atag_send(atag_cmd(AT_RESTORE));
    wait_response(2);

    // atag_send(atag_cmd(AT_CWMODE("1")));
    wait_response(2);

    int dbg = 0;
}

unsigned char *wait_response(int delay_gain)
{
    atag_receive(delay_gain);
    if (atag_get_response_status())
    {
        printf("response: %s", atag_get_response());
        return atag_get_response();
    }
    else
    {
        printf("response: ...");
    }

    return NULL;
}

#endif