#ifndef ATAGENT
#define ATAGENT

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
};

extern struct at_agent *at_ag;

int atag_set_config(struct at_agent *agent);

int atag_initial();

int atag_receive();

#endif