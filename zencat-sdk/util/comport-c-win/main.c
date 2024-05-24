#include <unistd.h>
#include "main.h"

#ifdef CASE1_RECEIVE
#include "case1_receive.h"
#endif
#ifdef CASE2_POST
#include "case2_post.h"
#endif

int main()
{
#ifdef CASE1_RECEIVE
    case1_receive();
#endif
#ifdef CASE2_POST
    case2_post();
#endif

    return 0;
}