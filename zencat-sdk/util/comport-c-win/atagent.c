#include <stdio.h>
#include "atagent.h"

struct at_agent *at_ag;

int atag_set_config(struct at_agent *agent)
{
    at_ag = agent;

    return 0;
}
