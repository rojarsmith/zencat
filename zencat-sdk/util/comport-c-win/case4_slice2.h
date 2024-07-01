#ifndef CASE4_SLICE2_INCLUDED
#define CASE4_SLICE2_INCLUDED

#include <stdio.h>
#include <time.h>
#include "rs232.h"
#include "main.h"
#include "at.h"
#include "at_cmd.h"

struct at_agent esp32;

void case4_slice2();
void story_fetch_passthrough();

unsigned char *wait_response(int delay_gain);
unsigned char *wait_response_bin(int *length);

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

    clock_t start;
    clock_t end;
    start = clock();

    story_fetch_passthrough();

    end = clock();
    double diff = end - start; // ms
    printf("Time: %f  ms", diff);
    printf(" %f  sec", diff / CLOCKS_PER_SEC);

    atag_close();

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

    atag_send(atag_cmd(AT_RESTORE, ""));
    wait_response(2);
    wait_response(2);

    atag_send(atag_cmd(AT_CWMODE, "=1"));
    wait_response(2);

    char pars[200] = {0};
    sprintf(pars, "=\"%s\",\"%s\"", WIFI_USERNAME, WIFI_PASSWORD);
    atag_send(atag_cmd(AT_CWJAP, pars));
    wait_response(2);
    wait_response(50);

    sprintf(pars, "=\"%s\"", FILE_URL_10);
    atag_send(atag_cmd(AT_HTTPGETSIZE, pars));
    wait_response(2);
    int res_size = extract_integer(wait_response(20));
    printf("Res size: %d\n", res_size);

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG", "=1,18,3"));
    wait_response(2);

    unsigned char pars2[200] = {0};
    sprintf(pars2, "^+HTTPCGET:[0-9]*,\r\n$");
    atag_send_bytes(pars2, 23);
    // atag_send("^+HTTPCLIENT:[0-9]*,\r\n$");
    wait_response(2);

    // pars2[0] = '\r';
    // atag_send_bytes(pars2, 1);

    // atag_send(atag_cmd("", ""));
    // wait_response(2);

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG", "?"));
    wait_response(2);

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG", "=1,0,7"));
    wait_response(2);

    sprintf(pars2, "\r\nOK\r\n$");
    atag_send_bytes(pars2, 7);
    wait_response(2);

    // atag_send(atag_cmd("", ""));
    // wait_response(2);

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG", "?"));
    wait_response(2);

    atag_send(atag_cmd("AT+SYSMSGFILTER", "=1")); // Remove "OK"
    wait_response(2);

    atag_send(atag_cmd("ATE0", ""));
    wait_response(2);

    sprintf(pars, "=\"%s\",2048,2048,10000", FILE_URL_10);
    atag_send(atag_cmd("AT+HTTPCGET", pars));
    int bin_len_sum = 0;
    unsigned char *file_name = "file2_1.png";
    if (remove(file_name) == 0)
    {
        printf("Deleted successfully");
    }
    else
    {
        printf("Unable to delete the file");
    }
    while (bin_len_sum < res_size)
    {

        FILE *file;
        unsigned char *data_raw;
        // size_t data_len;

        file = fopen(file_name, "ab");
        if (file == NULL)
        {
            perror("Failed to open file");
            return;
        }

        int bin_len = 0;
        data_raw = wait_response_bin(&bin_len);
        printf("bin len: %d\n", bin_len);
        bin_len_sum += bin_len;
        printf("bin len sum: %d\n", bin_len_sum);

        size_t written = fwrite(data_raw, 1, bin_len, file);
        if (written != bin_len)
        {
            perror("Failed to write complete data to file");
        }

        fclose(file);
    }

    atag_send(atag_cmd("AT+SYSMSGFILTER", "=0"));
    wait_response(2);

    atag_send(atag_cmd("ATE1", ""));
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

unsigned char *wait_response_bin(int *length)
{
    *length = atag_receive_bin();
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