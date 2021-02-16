#pragma once
struct Message {
    char name[100];
    int msgtype; // 0 for group, 1 for personal
    int recipient_id;
    // int group_id;
    // unsigned long long timestamp;
    char msg[1000];
};

struct Init {
    int id;
};