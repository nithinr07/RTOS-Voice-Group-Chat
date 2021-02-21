#pragma once
struct Message {
    char name[20];
    int msgtype; // 0 for group, 1 for personal
    char recipient_id[20];
    char group_id[20]; 
    unsigned long long timestamp;
    char msg[1000];
};

struct Init {
    char user_id[20];
    char group_id[20];
};