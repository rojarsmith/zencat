#include <unistd.h>
#include "main.h"

#ifdef CASE1_RECEIVE
#include "case1_receive.h"
#endif
#ifdef CASE2_POST
#include "case2_post.h"
#endif
#ifdef CASE3_SLICE
#include "case3_slice.h"
#endif

int main()
{
#ifdef CASE1_RECEIVE
    case1_receive();
#endif
#ifdef CASE2_POST
    case2_post();
#endif
#ifdef CASE3_SLICE
    case3_slice();
#endif

    return 0;
}