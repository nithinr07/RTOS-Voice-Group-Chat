#pragma once


#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
struct Message {
    char name[100];
    uint8_t msg[1024];
};