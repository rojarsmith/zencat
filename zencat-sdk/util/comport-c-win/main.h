#ifndef MAIN
#define MAIN

#if DEV_MODE
#include "main_dev.h"
#endif

#ifndef WIFI_USERNAME
#define WIFI_USERNAME "USERNAME"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "PASSWORD"
#endif

#define COM_PORT (9 - 1)
#define BAUD_RATE 115200
#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 5

#define FILE_URL_1 "https://bitdove.net/assets/home_hero.png"
#define FILE_URL_1_LEN 40
#define FILE_URL_2 "https://bitdove.net/logo/logo_full.svg"

/* AT Command */
#define AT_RST "AT+RST\r\n"

#endif