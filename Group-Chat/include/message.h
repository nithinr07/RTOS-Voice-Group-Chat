#pragma once
struct Message {
    char name[100];
    int msgtype; // 0 for group, 1 for personal
    int recipient_id;
    char msg[1000];
};

struct Init {
    int id;
};