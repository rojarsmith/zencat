#include <stdio.h>
#include <string.h>
#include "rs232.h"
#include "atagent.h"

struct at_agent *at_ag;

unsigned char send_buf[SEND_BUF_SIZE + 1] = {0};
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

int atag_receive(int delay_gain)
{
    printf("[begin]receive\n");

    int resp_idx = 0;
    int n = 0;
    int retry = 0;
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
                (i == (n - 1)) ? printf("\n") : 0;

                resp_buf[resp_idx] = recv_buf[i];
                resp_idx++;
            }
        }
        else
        {
#ifdef _WIN32
            Sleep(100 * delay_gain);
#else
            usleep(100000 * delay_gain); /* sleep for 0.1 * delay_gain Second */
#endif
            printf(".\n");
            retry++;
        }
    }

    if (resp_idx >= 1)
    {
        at_ag->msg_received = 1;
        resp_buf[resp_idx] = 0;
        printf("[response] %i bytes: %s\n", resp_idx, (char *)resp_buf);
        // Debug
        for (int i = 0; i <= resp_idx; i++)
        {
            printf("[%02x]", resp_buf[i]);
            (i == (resp_idx)) ? printf("\n") : 0;
        }
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

unsigned char *atag_cmd(const char *cmd)
{
    const char *suffix = "\r\n";
    size_t input_len = strlen(cmd);
    size_t suffix_len = strlen(suffix);

    char *result = (char *)malloc(input_len + suffix_len + 1);
    if (result == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, cmd);
    strcat(result, suffix);

    return result;
}

int atag_send(const char *cmd)
{
    int send_len = 0;
    send_len = strlen(cmd);

    strcpy(send_buf, cmd);
    RS232_cputs(at_ag->com_port, send_buf);

    if (strcmp(cmd, AT_CIPSTA) == 0)
    {
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000); // sleep for 0.2 Second
#endif
    }
    else
    {
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000); // sleep for 0.1 Second
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
