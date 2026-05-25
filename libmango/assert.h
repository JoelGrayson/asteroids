// Note: this file is not used. Instead, the built-in assert.h is used from libmango

#ifndef ASSERT_H
#define ASSERT_H

#include "printf.h"

// Use a macro so that it expands so that __LINE__ and __FUNCTION__ work and are local the line
#define assert(expr) \
    if (!(expr)) { \
        printf("Assert failed at line %d in %s\n", __LINE__, __FUNCTION__); \
    }


#endif
