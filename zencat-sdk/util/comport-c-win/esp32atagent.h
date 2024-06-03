#ifndef ESP32ATAGENT
#define ESP32ATAGENT

#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 64
#define RESP_BUF_SIZE 1024
#define RETRY_COUNT 3

/* AT Command */
#define AT_RST "AT+RST\r\n"
#define AT_CWMODE(arg) "AT+CWMODE=" arg "\r\n"
#define AT_CWJAP(NAME, PASSWORD) "AT+CWJAP=\"" NAME "\",\"" PASSWORD "\"\r\n"
#define AT_CIPSTA "AT+CIPSTA?\r\n"
#define AT_HTTPGETSIZE(url) "AT+HTTPGETSIZE=\"" url "\"\r\n"
#define AT_HTTPURLCFG "AT+HTTPURLCFG?\r\n"
#define AT_HTTPURLCFG_SET(len) "AT+HTTPURLCFG=" len "\r\n"
#define AT_HTTPCLIENT(arg) "AT+HTTPCLIENT=" arg "\r\n"

typedef enum
{
    IDLE,
    WAIT_RESPOSE
} Esp32Status;

typedef enum
{
    DOWNLOAD_READ_META,
    // DOWNLOAD_READ_SIZE,
    DOWNLOAD_READ_DATA
} DownloadStatus;

typedef enum
{
    ESP32_INITIAL_ERROR
} Esp32Error;

typedef struct
{
    char data_bits;
    char parity;
    char stop_bits;
    int flow_ctrl;
} UartMode;

typedef struct
{
    int com_port;
    int baud_rate;
    UartMode mode;
    Esp32Status esp32_status;
} Esp32AtAgent;

extern Esp32AtAgent esp32_at_agent;

int esp32_at_agent_set_config(
    int com_port,
    int baud_rate,
    char data_bits,
    char parity,
    char stop_bits,
    int flow_ctrl);

int esp32_at_agent_initial();

int esp32_at_agent_start_download();

int esp32_at_agent_receive(int remove_echo);

int esp32_at_agent_receive_tp();

unsigned char *esp32_at_agent_response();

int esp32_at_agent_response_status();

int esp32_at_agent_send_at(const char *cmd);

int extract_integer(const char *str);

int extract_integer2(const char *str);

int delay(int milli_second);

#endif