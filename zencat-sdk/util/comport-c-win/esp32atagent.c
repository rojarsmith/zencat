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

int esp32_at_agent_receive(int remove_echo)
{
    resp_idx = 0;
    resp_fin = 1;
    int n = 0;
    int retry = 0;
    int brk = 0;

    while (retry <= RETRY_COUNT)
    {
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
