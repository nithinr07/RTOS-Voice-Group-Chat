#pragma once
struct Message {
    char name[100];
    char recipient[100];
    char msg[1000];
};

struct Init {
    char recipient[100];
};