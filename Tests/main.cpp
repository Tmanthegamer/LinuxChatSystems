#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXMESSAGEDATA 	10
#define MAXCONNECTIONS 	1500
#define MAXMSGSIZE 		8192
#define MSG_COPY		040000

typedef struct
{
    long mesg_type; /* message type */
    int sockets[MAXCONNECTIONS]; /* Array of socket connections */
    int num_sockets; /* Number of sockets */
    char mesg_data[MAXMESSAGEDATA]; /* Small char array contains */
} Mesg;

int ReadMessage(int queue, Mesg* msg, long msg_type);
int ReadMessageCopy(int queue, Mesg* msg, long msg_type);
int RemoveClientFromList(int msgQueue, int client);
int SendMessage(int queue, Mesg* msg);
int AddClientToList(int msgQueue, int client);
int SendFinalMessage(int queue, Mesg* msg);
int RemoveQueue(int queue);
int OpenQueue(void);
int main(void);


int ReadMessage(int queue, Mesg* msg, long msg_type)
{
    int rc = msgrcv(queue, msg, sizeof(Mesg), msg_type,0);
    if(rc < 0)
    {
        perror("msgrcv failed:\n");
        return -1;
    }

    return 0;
}

int ReadMessageCopy(int queue, Mesg* msg, long msg_type)
{
    //int flags = MSG_COPY | IPC_NOWAIT;
    int rc = msgrcv(queue, msg, MAXMSGSIZE, msg_type, MSG_COPY | IPC_NOWAIT);
    if(rc < 0)
    {
        perror("instant return\n");
        return -1;
    }

    return 0;
}

int RemoveClientFromList(int msgQueue, int client)
{
    //need to read every message in the queue and fix it eventually.
    Mesg *oldMsg, *newMsg;

    oldMsg = (Mesg*)malloc(sizeof(Mesg));
    newMsg = (Mesg*)malloc(sizeof(Mesg));

    //printf("Reading message");
    if(ReadMessage(msgQueue, oldMsg, 0) < 0)
    {
        //printf("RemoveClientFromList: No more messages.\n");
        return -1;
    }
    //printf("[num:%d]\n", oldMsg.num_sockets);
    //printf("check message][");

    //msg here, modify it to add in another client
    if(oldMsg->num_sockets <= 0)
    {
        printf("RemoveClientFromList: num_sockets lower than zero...\n");
        return -1;
    }

    //printf("check message2\n");
    //Do not allow too many connections
    if(oldMsg->num_sockets >= MAXCONNECTIONS)
    {
        printf("RemoveClientFromList: num{%d} > %d...\n", oldMsg->num_sockets, MAXCONNECTIONS);
        return -1;
    }
    //printf("removing the client\n");

    //Add in the new client and increase the number of sockets.
    for(int i = 0; i < oldMsg->num_sockets; i++)
    {
        //printf("{%d}", i);
        if(oldMsg->sockets[i] != client)
        {
            newMsg->sockets[i] = oldMsg->sockets[i];
        }
    }

    newMsg->num_sockets = oldMsg->num_sockets - 1;
    newMsg->mesg_type = 1;
    memcpy(newMsg->mesg_data, oldMsg->mesg_data, sizeof(oldMsg->mesg_data));

    //printf("sending message");

    //Replace the old message
    if(SendMessage(msgQueue, newMsg) < 0)
    {
        printf("RemoveClientFromList: sendmessage failure.\n");
        return -1;
    }
    //printf("Donerino message");

    free(oldMsg);
    free(newMsg);

    return 0;
}

int SendMessage(int queue, Mesg* msg)
{

    /* This will keep trying to send messages the message queue if there are
        too many messages in the queue. */
    int rc = msgsnd(queue, msg, sizeof(Mesg), 0);

    if (rc < 0)
    {
        perror("msgsnd failed:\n");
        return -1;
    }

    return 0;
}

int AddClientToList(int msgQueue, int client)
{
    //need to read every message in the queue and fix it eventually.
    Mesg *oldMsg, *newMsg;

    oldMsg = (Mesg*)malloc(sizeof(Mesg));
    newMsg = (Mesg*)malloc(sizeof(Mesg));

    //printf("Reading message");
    if(ReadMessage(msgQueue, oldMsg, 0) < 0)
    {
        //printf("AddClientToList: No more messages.\n");
        return -1;
    }
    //printf("[num:%d]\n", oldMsg.num_sockets);
    //printf("check message][");

    //msg here, modify it to add in another client
    if(oldMsg->num_sockets <= 0)
    {
        printf("AddClientToList: num_sockets lower than zero...\n");
        return -1;
    }

    //printf("check message2\n");
    //Do not allow too many connections
    if(oldMsg->num_sockets >= MAXCONNECTIONS)
    {
        printf("AddClientToList: num{%d} > %d...\n", oldMsg->num_sockets, MAXCONNECTIONS);
        return -1;
    }
    //printf("adding new client\n");

    //Add in the new client and increase the number of sockets.
    for(int i = 0; i < oldMsg->num_sockets; i++)
    {
        //printf("{%d}", i);
        newMsg->sockets[i] = oldMsg->sockets[i];
    }

    newMsg->sockets[oldMsg->num_sockets] = client;
    newMsg->num_sockets = oldMsg->num_sockets + 1;
    newMsg->mesg_type = 1;
    memcpy(newMsg->mesg_data, oldMsg->mesg_data, sizeof(oldMsg->mesg_data));

    //printf("sending message");

    //Replace the old message
    if(SendMessage(msgQueue, newMsg) < 0)
    {
        printf("AddClientToList: sendmessage failure.\n");
        return -1;
    }
    //printf("Donerino message");

    free(oldMsg);
    free(newMsg);

    return 0;
}

int SendFinalMessage(int queue, Mesg* msg)
{
    msg->mesg_data[0] = '\0';
    msg->mesg_data[1] = '\0';
    return SendMessage(queue, msg);
}

int RemoveQueue(int queue)
{
    int rc;
    rc= msgctl(queue, IPC_RMID, NULL);
    if (rc < 0)
    {
        return 1;
    }
    return 0;
}

int OpenQueue(void)
{
    key_t key;

    key = ftok("Info", 'a');
    int msgQueue = msgget(key, 0644|IPC_CREAT);

    if (msgQueue < 0) {
        return -1;
    }

    return msgQueue;
}

int main(void)
{
    int msgQueue = 0;

    if((msgQueue = OpenQueue()) < 0)
    {
        RemoveQueue(msgQueue);
        return 1;
    }


    Mesg *msg, *rcv;
    int i = 0;

    msg = (Mesg*)malloc(sizeof(Mesg));
    rcv = (Mesg*)malloc(sizeof(Mesg));

    msg->mesg_type = 1;
    for(i = 0; i < 200; i++)
    {
        msg->sockets[i] = i+i;
        msg->num_sockets = i + 1;
    }
    sprintf(msg->mesg_data, "%s", "Hello.");
    printf("[num:%d][message:%s]\n", msg->num_sockets, msg->mesg_data);


    //int ReadMessage(int queue, Mesg* msg, long msg_type)
    if(SendMessage(msgQueue, msg) < 0)
    {
        printf("Sendmessage failure\n");
        RemoveQueue(msgQueue);
        return 1;
    }

    printf("Sent message to queue.\n");

    //test stuff in this if statement below
    int newClient = 123;
    if(AddClientToList(msgQueue, newClient) < 0)
    {
        printf("\nUnable to add a new client.\n");
        RemoveQueue(msgQueue);
        return 1;
    }

    printf("Added client to list.\n");

    Mesg *temp = (Mesg*)malloc(sizeof(Mesg));

    if(ReadMessageCopy(msgQueue, temp, 0))
    {
        printf("Copy failure.\n");
    }

    printf("Info about the copied message...\n");
    printf("[type:%ld][num_sockets:%d][message:%s]\n", temp->mesg_type, temp->num_sockets, temp->mesg_data);

    free(temp);


    if(ReadMessage(msgQueue, rcv, 0) < 0)
    {
        printf("Readmessage failure\n");
    }

    printf("Info about the message...\n");
    printf("[type:%ld][num_sockets:%d][message:%s]\n", rcv->mesg_type, rcv->num_sockets, rcv->mesg_data);

    for(i = 0; i < rcv->num_sockets; i++)
    {
        printf("<index:%d><socket:%d>\n", i, rcv->sockets[i]);
    }
    free(msg);
    free(rcv);

    RemoveQueue(msgQueue);

    return 0;
}











