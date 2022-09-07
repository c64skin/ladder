#include "ladder.h"
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>

static void waittcs(unsigned int tcs)
{
    struct timeval tv;
    tv.tv_sec = tcs / CLK_TCK;
    tv.tv_usec = (tcs % CLK_TCK) * (1000000L / CLK_TCK);
    select(0,(fd_set *)0,(fd_set *)0,(fd_set *)0,&tv);
}

static struct tms tmsbuf;
static clock_t last, diff;

/* set 'em */
static void setct(short msec)
{
    diff = ((clock_t)CLK_TCK * msec) / 1000 + 1;
    last = times(&tmsbuf);
}

void ctplay(void)
{
    static short speeds[HISPEED] = {125,100,80,60,40};
    setct(speeds[speed]);
}

/* for killed lad & hookas */
void ctnplay(void)
{
    setct(25);
}

void waitct(void)
{
    clock_t now = times(&tmsbuf);
    
    last += diff;
    if( now >= last )
        last = now;
    else
        waittcs((unsigned int)(last - now));
}
