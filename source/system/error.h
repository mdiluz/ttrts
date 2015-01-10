#ifndef _TTRTS_ERROR_H_
#define _TTRTS_ERROR_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

//======================================================================================================================
// Error functions

// For local fatal errors
inline void fatal_error(const char *msg)
{
    std::cerr<<msg<<std::endl;
    exit(1);
}

// For system fatal errors (ie. functions that set errno)
inline void fatal_perror(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif