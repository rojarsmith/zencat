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
#ifdef CASE4_SLICE2
#include "case4_slice2.h"
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
#ifdef CASE4_SLICE2
    case4_slice2();
#endif

    return 0;
}