#ifndef ATAGENT
#define ATAGENT

#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 2048 // 4, 6: debug, 2048: ESP32
#define RESP_BUF_SIZE 2048
#define RETRY_COUNT 3

#define AT_RST "AT+RST"
#define AT_CWMODE(MODE) "AT+CWMODE=" MODE
#define AT_CWJAP(NAME, PASSWORD) "AT+CWJAP=\"" NAME "\",\"" PASSWORD "\""
#define AT_CIPSTA "AT+CIPSTA?"
#define AT_HTTPGETSIZE(URL) "AT+HTTPGETSIZE=\"" URL "\""
#define AT_HTTPURLCFG "AT+HTTPURLCFG?"
#define AT_HTTPURLCFG_SET(LEN) "AT+HTTPURLCFG=" LEN ""
#define AT_SYSMSGFILTERCFG(IDX, HREG, TREG) "AT+SYSMSGFILTERCFG=" IDX "," HREG "," TREG
#define AT_HTTPCLIENT(ARG) "AT+HTTPCLIENT=" ARG
#define AT_HTTPCHEAD(LEN) "AT+HTTPCHEAD=" LEN
#define ARG_RANGE_BYTES(BLEN, ELEN) "Range: bytes=" BLEN "-" ELEN
#define AT_SYSMSGFILTER(LEN) "AT+SYSMSGFILTER=" LEN

#define FILE_URL_1 "https://bitdove.net/assets/home_hero.png"
#define FILE_URL_1_LEN 40

enum hardware_type
{
    ESP32,
};

struct uart_mode
{
    char data_bits;
    char parity;
    char stop_bits;
    int flow_ctrl;
};

struct at_agent
{
    enum hardware_type h_type;
    int com_port;
    int baud_rate;
    struct uart_mode *u_mode;
    int msg_received;
};

extern struct at_agent *at_ag;

int atag_set_config(struct at_agent *agent);

int atag_initial();

int atag_receive(int delay_gain);

int atag_get_response_status();

unsigned char *atag_get_response();

unsigned char *atag_cmd(const char *cmd);

int atag_send(const char *cmd);

int extract_integer(const char *str);

#endif