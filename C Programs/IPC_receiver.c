#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// Message structure for IPC communication
struct msg_buffer
{
    long msg_type;      // Message type identifier
    char msg_text[100]; // Message content
} message;

int main()
{
    // Generate unique keys for message queues
    key_t snd_key = ftok("progfile", 66); // Key for sending messages
    key_t rcv_key = ftok("progfile", 65); // Key for receiving messages

    // Create or access message queues
    int msgid_snd = msgget(snd_key, 0666 | IPC_CREAT);
    int msgid_rcv = msgget(rcv_key, 0666 | IPC_CREAT);

    char str1[100], str2[100], str3[100], concatenated[100];

    // Receive first string
    msgrcv(msgid_rcv, &message, sizeof(message.msg_text), 1, 0);
    strcpy(str1, message.msg_text);
    printf("Received string 1: %s\n", str1);

    // Receive second string
    msgrcv(msgid_rcv, &message, sizeof(message.msg_text), 1, 0);
    strcpy(str2, message.msg_text);
    printf("Received string 2: %s\n", str2);

    // Receive third string
    msgrcv(msgid_rcv, &message, sizeof(message.msg_text), 1, 0);
    strcpy(str3, message.msg_text);
    printf("Received string 3: %s\n", str3);

    // Concatenate all three strings with spaces between them
    sprintf(concatenated, "%s %s %s", str1, str2, str3);
    printf("Concatenated: %s\n", concatenated);

    // Send the concatenated string back
    message.msg_type = 1;
    strcpy(message.msg_text, concatenated);
    msgsnd(msgid_snd, &message, sizeof(message.msg_text), 0);

    return 0;
}
