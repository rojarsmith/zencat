#ifndef AT_H
#define AT_H

#define SEND_BUF_SIZE 2048
#define RECV_BUF_SIZE 2048 // 4, 6: debug, 2048: ESP32
#define RESP_BUF_SIZE 2048
#define RETRY_COUNT 3

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
    char *cmd_current;
    char *cmd_previous;
};

extern struct at_agent *at_ag;

int atag_set_config(struct at_agent *agent);

int atag_initial();

int atag_close();

int atag_receive(int delay_gain);

int atag_receive_bin();

int atag_parse(const char *response);

int atag_send(const char *cmd);

int atag_send_bytes(unsigned char *cmd, int length);

int atag_get_response_status();

unsigned char *atag_get_response();

unsigned char *atag_cmd(const char *cmd, const char *pars);

int extract_integer(const char *str);

#endif