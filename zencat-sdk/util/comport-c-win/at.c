#include <regex.h>
#include <assert.h>
#include "rs232.h"
#include "at.h"

struct at_agent *at_ag;

char send_buf[SEND_BUF_SIZE + 1] = {0};
char recv_buf[RECV_BUF_SIZE + 1] = {0};
char resp_buf[RESP_BUF_SIZE + 1] = {0};

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

int atag_close()
{
    RS232_CloseComport(at_ag->com_port);
    printf("[SYS]Close COM Port Success\n");
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

        if ((resp_idx + n) >= RESP_BUF_SIZE)
        {
            printf("[received]exceed resp buff\n");
            break;
        }

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
            printf("[retry].\n");
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

        atag_parse(resp_buf);
    }

    return resp_idx;
}

int atag_receive_bin()
{
    printf("[begin]receive\n");

    int resp_idx = 0;
    int n = 0;
    int retry = 0;
    at_ag->msg_received = 0;

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

int atag_parse(const char *response)
{
    const char *pattern_ready =
        "("
        "\r\nOK\r\n"
        "|\r\nready\r\n"
        "|WIFI CONNECTED\r\n"
        ")";
    regex_t regex;
    regmatch_t matchptr[1];
    const size_t nmatch = 1;
    int ret;
    int start = 0;
    int len = strlen(response);

    ret = regcomp(&regex, pattern_ready, REG_EXTENDED);
    if (ret)
    {
        printf("Could not compile regex\n");
        exit(888);
    }

    int match_cnt = 0;

    while (start < len && regexec(&regex, response + start, nmatch, matchptr, 0) == 0)
    {
        printf("[REG]Match at position %d - %d\n", matchptr[0].rm_so, matchptr[0].rm_eo);
        printf("[REG]");
        for (int i = matchptr[0].rm_so; i < matchptr[0].rm_eo; i++)
        {
            printf("%c", response[i]);
        }
        printf("\n");
        start += matchptr[0].rm_eo;
        match_cnt++;
    }

    regfree(&regex);

    return match_cnt;
}

unsigned char *atag_cmd(const char *cmd, const char *pars)
{
    const char *suffix = "\r\n";
    size_t input_len = strlen(cmd);
    size_t suffix_len = strlen(suffix);
    size_t pars_len = strlen(pars);

    size_t malloc_len = 0;
    if (pars_len >= 1)
    {
        malloc_len = input_len + suffix_len + 1 + pars_len;
    }
    else
    {
        malloc_len = input_len + suffix_len + 1;
    }

    char *result = (char *)malloc(malloc_len);
    if (result == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, cmd);
    if (pars_len >= 1)
    {
        strcat(result, pars);
    }
    strcat(result, suffix);

    return result;
}

int atag_send(const char *cmd)
{
    int send_len = 0;
    send_len = strlen(cmd);

    strcpy(send_buf, cmd);
    RS232_cputs(at_ag->com_port, send_buf);

    if (strcmp(cmd, "") == 0)
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

int atag_send_bytes(unsigned char *cmd, int length)
{
    int poss[100];
    int poss_cnt = 0;
    char substr[] = "\r\n";
    int len_substr = strlen(substr);

    for (int i = 0; i <= length - len_substr; i++)
    {
        if (strncmp(&cmd[i], substr, len_substr) == 0)
        {
            poss[poss_cnt] = i;
            i += len_substr - 1;
            poss_cnt++;
        }
    }

    int status = 0;
    if (poss_cnt >= 1)
    {
        // int shift = 0;
        unsigned char tmp[200] = {0};

        // head
        if (poss[0] == 0)
        {
            // sprintf(tmp, "\r");
            // atag_send_bytes(tmp, 1);
            // sprintf(tmp, "\n");
            // atag_send_bytes(tmp, 1);
        }
        else if (poss[0] != 0)
        {
            status = RS232_SendBuf(at_ag->com_port, cmd, poss[0]);
        }

        for (int i = 0; i < poss_cnt; i++)
        {
            sprintf(tmp, "\r");
            RS232_SendBuf(at_ag->com_port, tmp, 1);
#ifdef _WIN32
            Sleep(200);
#else
            usleep(200000); // sleep for 0.1 Second
#endif
            sprintf(tmp, "\n");
            RS232_SendBuf(at_ag->com_port, tmp, 1);
#ifdef _WIN32
            Sleep(200);
#else
            usleep(200000); // sleep for 0.1 Second
#endif

            if (i < (poss_cnt - 1)) // not tail
            {
                status = RS232_SendBuf(at_ag->com_port, cmd + poss[i] + 2, poss[i + 1] - poss[i] - 2);
            }
            else if (i == (poss_cnt - 1)) // tail
            {
                status = RS232_SendBuf(at_ag->com_port, cmd + poss[i] + 2, length - poss[i] - 2);
            }
        }
    }
    else
    {
        status = RS232_SendBuf(at_ag->com_port, cmd, length);
    }

#ifdef _WIN32
    Sleep(200);
#else
    usleep(200000); // sleep for 0.1 Second
#endif

    return status;
}

int atag_get_response_status()
{
    return at_ag->msg_received;
}

unsigned char *atag_get_response()
{
    return resp_buf;
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
