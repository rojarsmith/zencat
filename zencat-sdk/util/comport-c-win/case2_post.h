#ifndef CASE2_POST_INCLUDED
#define CASE2_POST_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
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

    esp32_at_agent_receive();
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive();
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
    esp32_at_agent_receive();
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    // esp32_at_agent_send_at(AT_RST);

    // esp32_at_agent_receive();
    // if (!esp32_at_agent_response_status())
    // {
    //     printf("response: %s", esp32_at_agent_response());
    // }

    esp32_at_agent_send_at(AT_CWMODE("1"));

    esp32_at_agent_receive();
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }

    esp32_at_agent_send_at(AT_CWJAP("NAME", "PASSWORD"));

    esp32_at_agent_receive();
    if (!esp32_at_agent_response_status())
    {
        printf("response: %s", esp32_at_agent_response());
    }
}

#endif