.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <ctype.h>

// Message structure for sending strings
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key_send = ftok("progfile", 65);    // Key for sending messages
    key_t key_recv = ftok("progfile", 66);    // Key for receiving messages
    
    // Create message queues
    int msgid_send = msgget(key_send, 0666 | IPC_CREAT);
    int msgid_recv = msgget(key_recv, 0666 | IPC_CREAT);
    
    struct msg_buffer message;
    message.msg_type = 1;
    
    // Send first string
    printf("Enter first string: ");
    fgets(message.msg_text, 100, stdin);
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0';  // Remove newline
    msgsnd(msgid_send, &message, sizeof(message), 0);
    
    // Send second string
    printf("Enter second string: ");
    fgets(message.msg_text, 100, stdin);
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0';  // Remove newline
    msgsnd(msgid_send, &message, sizeof(message), 0);
    
    // Send third string
    printf("Enter third string: ");
    fgets(message.msg_text, 100, stdin);
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0';  // Remove newline
    msgsnd(msgid_send, &message, sizeof(message), 0);
    
    // Receive concatenated message
    msgrcv(msgid_recv, &message, sizeof(message), 1, 0);
    
    // Flip the case of characters
    for(int i = 0; message.msg_text[i] != '\0'; i++) {
        if(isupper(message.msg_text[i]))
            message.msg_text[i] = tolower(message.msg_text[i]);
        else if(islower(message.msg_text[i]))
            message.msg_text[i] = toupper(message.msg_text[i]);
    }
    
    // Print the result
    printf("Final string with flipped case: %s\n", message.msg_text);
    
    return 0;
}