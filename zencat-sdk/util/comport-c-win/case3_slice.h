#ifndef CASE3_SLICE_INCLUDED
#define CASE3_SLICE_INCLUDED

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "rs232.h"
#include "atagent.h"

struct at_agent esp32;

void case3_slice();
unsigned char *wait_response(int delay_gain);

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

    // Debug
    // printf("cmd: %s", AT_HTTPURLCFG_SET("40"));
    // atag_send(atag_cmd(AT_HTTPURLCFG_SET("40")));

    unsigned char *cmd = atag_cmd(AT_RST);
    printf("cmd: %s", cmd);
    atag_send(cmd);
    wait_response(4);
    wait_response(46);

    atag_send(atag_cmd(AT_CWMODE("1")));
    wait_response(2);
    /* Response Nothing */
    wait_response(0);

    atag_send(atag_cmd(AT_CWJAP(WIFI_USERNAME, WIFI_PASSWORD)));
    wait_response(2);
    wait_response(10);

    atag_send(atag_cmd(AT_CIPSTA));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPGETSIZE(FILE_URL_1)));
    wait_response(10);
    int res_size = extract_integer(wait_response(10));
    printf("size: %d\n", res_size);

    atag_send(atag_cmd(AT_HTTPURLCFG_SET("40")));
    wait_response(2);

    atag_send(atag_cmd(FILE_URL_1));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPURLCFG));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPCHEAD("0")));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPCHEAD("21")));
    wait_response(2);

    atag_send(atag_cmd(ARG_RANGE_BYTES("161", "183")));
    wait_response(2);

    // atag_send(atag_cmd(AT_SYSMSGFILTERCFG("1","","")));
    atag_send(atag_cmd(AT_HTTPCLIENT("2,0,\"\",,,2")));
    wait_response(2);

    while (1)
    {
        wait_response(2);
    }
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

    return 0;
}

#endif