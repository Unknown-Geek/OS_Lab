#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

// Message structure for receiving strings
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key_recv = ftok("progfile", 65);    // Key for receiving messages
    key_t key_send = ftok("progfile", 66);    // Key for sending messages
    
    // Create message queues
    int msgid_recv = msgget(key_recv, 0666 | IPC_CREAT);
    int msgid_send = msgget(key_send, 0666 | IPC_CREAT);
    
    struct msg_buffer message;
    char str1[100], str2[100], str3[100];
    char concatenated[300];
    
    // Receive first string
    msgrcv(msgid_recv, &message, sizeof(message), 1, 0);
    strcpy(str1, message.msg_text);
    printf("Received string 1: %s\n", str1);
    
    // Receive second string
    msgrcv(msgid_recv, &message, sizeof(message), 1, 0);
    strcpy(str2, message.msg_text);
    printf("Received string 2: %s\n", str2);
    
    // Receive third string
    msgrcv(msgid_recv, &message, sizeof(message), 1, 0);
    strcpy(str3, message.msg_text);
    printf("Received string 3: %s\n", str3);
    
    // Concatenate with spaces
    sprintf(concatenated, "%s %s %s", str1, str2, str3);
    printf("Concatenated: %s\n", concatenated);
    
    // Send concatenated string back
    message.msg_type = 1;
    strcpy(message.msg_text, concatenated);
    msgsnd(msgid_send, &message, sizeof(message), 0);
    
    return 0;
}