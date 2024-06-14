#ifndef ATAGENT
#define ATAGENT

enum hardware_type
{
    ESP32,
};

struct at_agent
{
    enum hardware_type hardware;
};

int set_config_at_agent(struct at_agent *agent);

#endif