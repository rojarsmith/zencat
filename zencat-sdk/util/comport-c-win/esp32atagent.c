#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include "rs232.h"
#include "esp32atagent.h"

Esp32AtAgent esp32_at_agent = {
    .com_port = 0,
    .baud_rate = 0,
    .mode.data_bits = '8',
    .mode.parity = 'N',
    .mode.stop_bits = '1',
    .mode.flow_ctrl = 0,
    .esp32_status = IDLE};

DownloadStatus download_status = DOWNLOAD_READ_META;

int resp_idx = 0;
int resp_fin = 0;
unsigned char send_buf[SEND_BUF_SIZE + 1] = {0};
int send_len = 0;
unsigned char recv_buf[RECV_BUF_SIZE + 1] = {0};
unsigned char resp_buf[RESP_BUF_SIZE + 1] = {0};

int esp32_at_agent_set_config(
    int com_port,
    int baud_rate,
    char data_bits,
    char parity,
    char stop_bits,
    int flow_ctrl)
{
    esp32_at_agent.com_port = com_port;
    esp32_at_agent.baud_rate = baud_rate;
    esp32_at_agent.mode.data_bits = data_bits;
    esp32_at_agent.mode.parity = parity;
    esp32_at_agent.mode.stop_bits = stop_bits;
    esp32_at_agent.mode.flow_ctrl = flow_ctrl;

    return 0;
}

int esp32_at_agent_initial()
{
    // esp32_at_agent.esp32_status = WAIT_RESPOSE;

    if (RS232_OpenComport(
            esp32_at_agent.com_port,
            esp32_at_agent.baud_rate,
            (char[]){
                esp32_at_agent.mode.data_bits,
                esp32_at_agent.mode.parity,
                esp32_at_agent.mode.stop_bits,
                esp32_at_agent.mode.flow_ctrl},
            0))
    {
        printf("Can not Open COM Port Success\n\n");
        return 0;
    }

    // esp32_at_agent.esp32_status = IDLE;

    return 0;
}

int esp32_at_agent_start_download()
{
    download_status = DOWNLOAD_READ_META;
}

int esp32_at_agent_receive(int remove_echo)
{
    resp_idx = 0;
    resp_fin = 1;
    int n = 0;
    int retry = 0;
    int brk = 0;

    while (retry <= RETRY_COUNT)
    {
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000); /* sleep for 0.1 Second */
#endif

        // [bug?]massive receive
        // `RS232_PollComport segmentation fault`
        n = RS232_PollComport(
            esp32_at_agent.com_port,
            recv_buf,
            RECV_BUF_SIZE);

        if (n > 0)
        {
            recv_buf[n] = 0; // always put a "null" at the end of a string!

            printf("\n[received]=====\n");
            printf("received %i bytes: %s\n", n, (char *)recv_buf);

            for (int i = 0; i <= RECV_BUF_SIZE; i++)
            {
                printf("[%02x]", recv_buf[i]);

                if (i == RECV_BUF_SIZE)
                {
                    printf("\n");
                }

                if (i < RECV_BUF_SIZE)
                {
                    resp_buf[resp_idx + i] = recv_buf[i];
                }

                if (i == RECV_BUF_SIZE)
                {
                    resp_idx += i;
                }

                if (i < RECV_BUF_SIZE &&
                    recv_buf[i] == '\0')
                {
                    resp_fin = 0;
                    brk = 1;
                }
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
            break;
        }
    }

    if (remove_echo == 1)
    {
        for (int i = 0; i < (RESP_BUF_SIZE - send_len); i++)
        {
            resp_buf[i] = resp_buf[i + send_len];
        }
    }

    // esp32_at_agent.esp32_status = IDLE;
    return 0;
}

unsigned char tk_buf[24 + 1] = {0};
int tk_buf_idx = 0;
int data_size = 0;
int resp_buf_idx = 0;
unsigned char val_head[] = "+HTTPCLIENT:";

int esp32_at_agent_receive_tp()
{
    resp_buf_idx = 0;
    int n = 0;
    int n_idx = 0;
    int retry = 0;
    int brk = 0;

    while (retry <= RETRY_COUNT)
    {
        n = RS232_PollComport(
            esp32_at_agent.com_port,
            recv_buf,
            RECV_BUF_SIZE);

        if (n > 0)
        {
            recv_buf[n] = 0; // always put a "null" at the end of a string!

            printf("\n[received]=====\n");
            printf("received %i bytes: %s\n", n, (char *)recv_buf);

            while (n_idx < n)
            {
                if (download_status == DOWNLOAD_READ_META)
                {
                    int chk_len = (tk_buf_idx < (sizeof(val_head) - 2)) ? tk_buf_idx : (sizeof(val_head) - 2);
                    for (int i = 0; i < chk_len; i++)
                    {
                        if (tk_buf[i] != val_head[i])
                        {
                            return -888;
                        }
                    }

                    tk_buf[tk_buf_idx] = recv_buf[n_idx];
                    tk_buf[tk_buf_idx + 1] = '\0';

                    if (tk_buf[tk_buf_idx] == ',')
                    {
                        data_size = extract_integer2(tk_buf);
                        data_size += 2; // '\r','\n'
                        if (data_size == -1)
                        {
                            data_size = -1;
                        }
                        // data_size_bk = data_size;
                        printf("\ndata_size: %d\n", data_size);
                        tk_buf_idx = 0;
                        for (int i = 0; i < 24; i++)
                        {
                            tk_buf[i] = '\0';
                        }
                        download_status = DOWNLOAD_READ_DATA;
                        // n_idx++;
                        continue;
                    }
                    tk_buf_idx++;
                }
                else if (download_status == DOWNLOAD_READ_DATA)
                {
                    if (data_size > 0)
                    {
                        if (data_size > 2)
                        {
                            resp_buf[resp_buf_idx] = recv_buf[n_idx];
                        }
                        resp_buf_idx++;
                        data_size--;
                    }
                    else
                    {
                        // if (recv_buf[n_idx] == '\r')
                        // {
                        //     n_idx++;
                        //     continue;
                        // }
                        // else if (recv_buf[n_idx] != '\n')
                        // {
                        //     return -999;
                        // }

                        download_status = DOWNLOAD_READ_META;
                    }
                }

                n_idx++;
            }

            brk = 1;

            if (brk == 1)
            {
                for (int i = 0; i < n_idx; i++)
                {
                    printf("[%02x]", resp_buf[i]);
                }
                printf("\n");

                // resp_buf_idx = 0;
                break;
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
    }

    return resp_buf_idx;
}

unsigned char *esp32_at_agent_response()
{
    return resp_buf;
}

int esp32_at_agent_response_status()
{
    if (resp_fin == 0)
    {
        resp_fin = 1;
        return 0;
    }
    return 1;
}

int esp32_at_agent_send_at(const char *cmd)
{
    send_len = strlen(cmd);

    //     if (strcmp(cmd, AT_CIPSTA) == 0)
    //     {
    // #ifdef _WIN32
    //         Sleep(1000);
    // #else
    //         usleep(1000000); // sleep for 1 Second
    // #endif
    //     }

    esp32_at_agent.esp32_status = WAIT_RESPOSE;
    strcpy(send_buf, cmd);
    RS232_cputs(esp32_at_agent.com_port, send_buf);

    if (strcmp(cmd, AT_CIPSTA) == 0)
    {
#ifdef _WIN32
        Sleep(1000);
#else
        usleep(1000000); // sleep for 1 Second
#endif
    }

    return 0;
}

int extract_integer(const char *str)
{
    int value = 0;
    if (sscanf(str, "+HTTPGETSIZE:%d", &value) == 1)
    {
        return value;
    }
    return -1;
}

int extract_integer2(const char *str)
{
    int value = 0;
    if (sscanf(str, "+HTTPCLIENT:%d,", &value) == 1)
    {
        return value;
    }
    return -1;
}

int delay(int milli_second)
{
#ifdef _WIN32
    Sleep(milli_second / 1000);
    // Sleep(1000);
#else
    usleep(milli_second);
    // usleep(1000000); // sleep for 1 Second
#endif
}
