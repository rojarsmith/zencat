#include <regex.h>
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

int atag_parse(const char *response)
{
    const char *pattern_ready =
        "("
        "\r\nready\r\n"
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
        // printf("[REG]Match at position %d\n", start + matchptr[0].rm_so);
        printf("[REG]Match at position %d - %d\n", matchptr[0].rm_so, matchptr[0].rm_eo);
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
