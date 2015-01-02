#ifndef _TTRTS_NET_H_
#define _TTRTS_NET_H_

#include <stdio.h>
#include <stdlib.h>

inline void error(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif