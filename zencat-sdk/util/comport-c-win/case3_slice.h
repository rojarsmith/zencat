#ifndef CASE3_SLICE_INCLUDED
#define CASE3_SLICE_INCLUDED

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "rs232.h"
#include "atagent.h"

struct at_agent esp32;

void case3_slice();
void slicing_1();

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
    // unsigned char cmd2[] = "^+HTTPCLIENT:[0-9]*,\r\n$";
    // atag_cmd("^+HTTPCLIENT:[0-9]*,\r\n$");

    atag_send(atag_cmd("\r\n\r\n\r\n"));
    wait_response(2);
    atag_send(atag_cmd("\r\n\r\n\r\n"));
    wait_response(2);
    // unsigned char cmd2[] = (AT_SYSMSGFILTERCFG("1", "20", "3"));
    // atag_send(atag_cmd(cmd2));
    // wait_response(2);
    // // wait_response(10);
    // unsigned char cmd3[] = "^+HTTPCLIENT:[0-9]*,\r\n$";
    // atag_send(atag_cmd(cmd3));
    // wait_response(2);
    // atag_send(atag_cmd(""));
    // wait_response(10);
    // unsigned char cmd4[] = (AT_SYSMSGFILTERCFG("1", "0", "7"));
    // atag_send(atag_cmd(cmd4));
    // wait_response(2);
    // unsigned char cmd5[] = "\r\nOK\r\n";
    // atag_send(atag_cmd(cmd5));
    // wait_response(2);
    // atag_send(atag_cmd(""));
    // wait_response(10);
    // atag_send(atag_cmd("AT+SYSMSGFILTERCFG?"));
    // wait_response(10);
    // atag_send(atag_cmd("AT+SYSMSGFILTERCFG=0"));
    // wait_response(10);

    slicing_1();

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

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG=0"));
    wait_response(2);

    atag_send(atag_cmd(AT_SYSMSGFILTERCFG("1", "20", "3")));
    wait_response(2);

    atag_send(atag_cmd("^+HTTPCLIENT:[0-9]*,\r\n$"));
    wait_response(10);

    atag_send(atag_cmd(""));
    wait_response(10);

    atag_send(atag_cmd(AT_SYSMSGFILTERCFG("1", "0", "7")));
    wait_response(2);

    atag_send(atag_cmd("\r\nOK\r\n$"));
    wait_response(10);

    atag_send(atag_cmd(""));
    wait_response(10);

    atag_send(atag_cmd("AT+SYSMSGFILTERCFG?"));
    wait_response(10);

    atag_send(atag_cmd(AT_SYSMSGFILTER("1")));
    wait_response(2);

    atag_send(atag_cmd("ATE0"));
    wait_response(2);

    //
    FILE *file;
    unsigned char *data_raw;
    size_t data_len;

    file = fopen("file1", "ab");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    atag_send(atag_cmd(AT_HTTPCHEAD("0")));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPCHEAD("21")));
    wait_response(2);

    atag_send(atag_cmd(ARG_RANGE_BYTES("161", "183")));
    wait_response(2);

    atag_send(atag_cmd(AT_HTTPCLIENT("2,0,\"\",,,2")));
    data_raw = wait_response(20);
    data_len = 183 - 161 + 1;
    if (data_raw == NULL)
    {
        perror("No data");
        return;
    }

    size_t written = fwrite(data_raw, 1, data_len, file);
    if (written != data_len)
    {
        perror("Failed to write complete data to file");
    }

    fclose(file);

    /*
        File 2
    */
    atag_send(atag_cmd("\r\n\r\n\r\n"));
    wait_response(2);

    file = fopen("file2.png", "ab");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    // int sections = 386125 / 128;      // 3016
    // int tail = 386125 - (3016 * 128); // 77
    int sections = 386125 / 512;     // 754
    int tail = 386125 - (754 * 512); // 77

    for (int i = 0; i <= 754; i++)
    {
        atag_send(atag_cmd("ATE1"));
        wait_response(2);

        atag_send(atag_cmd(AT_HTTPCHEAD("0")));
        wait_response(2);

        int bs = 512 * i;
        int be = 512 * i + 512 - 1;
        if (i == 754)
        {
            be = 512 * i + tail - 1;
        }

        char rng[100];
        sprintf(rng, "Range: bytes= %d-%d", bs, be);
        int rng_len = strlen(rng);
        char rng_len_str[20];
        sprintf(rng_len_str, "AT+HTTPCHEAD=%d", rng_len);
        atag_send(atag_cmd(rng_len_str));
        wait_response(2);
        atag_send(atag_cmd(rng));
        wait_response(2);

        atag_send(atag_cmd("ATE0"));
        wait_response(2);
        atag_send(atag_cmd(AT_HTTPCLIENT("2,0,\"\",,,2")));
        data_raw = wait_response(20);
        data_len = be - bs + 1;
        if (data_raw == NULL)
        {
            perror("No data");
            data_raw = wait_response(200);
            if (data_raw == NULL)
            {
                perror("No data again");
                return;
            }
        }

        size_t written = fwrite(data_raw, 1, data_len, file);
        if (written != data_len)
        {
            perror("Failed to write complete data to file");
        }
    }

    fclose(file);

    while (1)
    {
        wait_response(2);
    }
}

void slicing_1()
{
    clock_t start;
    clock_t end;
    start = clock();

    unsigned char *file_name = "file_1.png";
    FILE *file;
    unsigned char *data_raw;
    size_t data_len;

    if (remove(file_name) == 0)
    {
        printf("Deleted successfully");
    }
    else
    {
        printf("Unable to delete the file");
    }

    int res_len = 386125;
    int sli_len = 2048;                        // 65536, 512
    int sections = res_len / sli_len;          // 5    , 754
    int tail = res_len - (sections * sli_len); // 58445, 77

    for (int i = 0; i <= sections; i++)
    {
        file = fopen(file_name, "ab");
        if (file == NULL)
        {
            perror("Failed to open file");
            return;
        }

        atag_send(atag_cmd("ATE1"));
        wait_response(2);

        atag_send(atag_cmd(AT_HTTPCHEAD("0")));
        wait_response(2);

        int bs = sli_len * i;
        int be = bs + sli_len - 1;
        if (i == sections)
        {
            be = sli_len * i + tail - 1;
        }
        char rng[100];
        sprintf(rng, "Range: bytes=%d-%d", bs, be);
        int rng_len = strlen(rng);
        char rng_len_str[30];
        sprintf(rng_len_str, "AT+HTTPCHEAD=%d", rng_len);
        int rng_len_str_len = strlen(rng_len_str);
        rng_len_str[rng_len_str_len] = '\r';
        rng_len_str[rng_len_str_len + 1] = '\n';
        rng_len_str[rng_len_str_len + 2] = '\0';
        atag_send(rng_len_str);
        wait_response(2);
        atag_send(rng);
        wait_response(2);
        atag_send(atag_cmd("AT+HTTPCHEAD?"));
        wait_response(2);

        atag_send(atag_cmd("ATE0"));
        wait_response(2);
        atag_send(atag_cmd(AT_HTTPCLIENT("2,0,\"\",,,2")));
        data_raw = wait_response(20);
        data_len = be - bs + 1;
        if (data_raw == NULL)
        {
            perror("No data");
        }

        size_t written = fwrite(data_raw, 1, data_len, file);

        fclose(file);
        int dbg = 0;
    }

    end = clock();
    double diff = end - start; // ms
    printf("Time: %f  ms", diff);
    printf(" %f  sec", diff / CLOCKS_PER_SEC);
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