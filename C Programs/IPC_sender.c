#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
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
    key_t snd_key = ftok("progfile", 65); // Key for sending messages
    key_t rcv_key = ftok("progfile", 66); // Key for receiving messages

    // Create or access message queues
    int msgid_send = msgget(snd_key, 0666 | IPC_CREAT);
    int msgid_rcv = msgget(rcv_key, 0666 | IPC_CREAT);

    message.msg_type = 1;

    // Get and send first string
    printf("Enter the first string : ");
    gets(message.msg_text);
    printf("\n");
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // Remove newline character
    msgsnd(msgid_send, &message, sizeof(message.msg_text), 0);

    // Get and send second string
    printf("Enter the second string : ");
    gets(message.msg_text);
    printf("\n");
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // Remove newline character
    msgsnd(msgid_send, &message, sizeof(message.msg_text), 0);

    // Get and send third string
    printf("Enter the third string : ");
    gets(message.msg_text);
    printf("\n");
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; // Remove newline character
    msgsnd(msgid_send, &message, sizeof(message.msg_text), 0);

    // Clear message buffer and receive concatenated string
    memset(message.msg_text, 0, sizeof(message.msg_text));
    msgrcv(msgid_rcv, &message, sizeof(message.msg_text), 1, 0);

    char str[100];
    strcpy(str, message.msg_text);

    printf("Received String : %s\n", message.msg_text);

    // Invert case of each character (uppercase to lowercase and vice versa)
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isupper(str[i]))
            str[i] = tolower(str[i]);
        else if (islower(str[i]))
            str[i] = toupper(str[i]);
    }

    printf("Final string : %s\n", str);

    return 0;
}
