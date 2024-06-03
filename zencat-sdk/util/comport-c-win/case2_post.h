#ifndef CASE2_POST_INCLUDED
#define CASE2_POST_INCLUDED

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "rs232.h"
#include "esp32atagent.h"

void case2_post();

void case2_post()
{
    esp32_at_agent_set_config(COM_PORT, BAUD_RATE, '8', 'N', '1', 0);

    esp32_at_agent_initial();

    /* Hot Initial */
    esp32_at_agent_send_at(AT_RST);

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    /* Cold Initial */
    // esp32_at_agent_receive();
    // if (!esp32_at_agent_response_status())
    // {
    //     printf("response: %s", esp32_at_agent_response());
    // }
    // esp32_at_agent_receive();
    // if (!esp32_at_agent_response_status())
    // {
    //     printf("response: %s", esp32_at_agent_response());
    // }
    // esp32_at_agent_receive();
    // if (!esp32_at_agent_response_status())
    // {
    //     printf("response: %s", esp32_at_agent_response());
    // }

    esp32_at_agent_send_at(AT_CWMODE("1"));

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    /* Response Nothing */
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }

    esp32_at_agent_send_at(AT_CWJAP(WIFI_USERNAME, WIFI_PASSWORD));

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }
    /* Response Nothing */
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }

    esp32_at_agent_send_at(AT_CIPSTA);

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }

    esp32_at_agent_send_at(AT_HTTPGETSIZE(FILE_URL_1));

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }
    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }

    int res_size = extract_integer(esp32_at_agent_response());
    printf("response: %d", res_size);

    esp32_at_agent_send_at(AT_HTTPURLCFG);

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    esp32_at_agent_send_at(AT_HTTPURLCFG_SET("40"));

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    esp32_at_agent_send_at(FILE_URL_1);

    esp32_at_agent_receive(0);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    esp32_at_agent_send_at(AT_HTTPURLCFG);

    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    esp32_at_agent_send_at(AT_HTTPCLIENT("2,0,\"\",,,2"));
    esp32_at_agent_receive(1);
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    else
    {
        printf("response: ...");
    }

    int total_len = 0;
    int try_cnt = 0;

    while (try_cnt <= 10)
    {
        delay(200000);
        int blk_len = esp32_at_agent_receive_tp();
        if (blk_len > 0)
        {
            printf("response blk_len: %d, ", blk_len);
            printf("response total_len: %d\n", total_len);      

            total_len += blk_len;
        }
        else
        {
            printf("response: ...");
            try_cnt++;
        }
    }
}

#endif