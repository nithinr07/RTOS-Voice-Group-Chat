#pragma once
struct Message {
    char name[100];
    int msgtype; // 0 for group, 1 for personal
    int recipient;
    char msg[1000];
};

struct Init {
    char name[100];
    int number;
};