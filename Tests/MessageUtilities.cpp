//
// Created by tyler on 29/02/16.
//

#include "MessageUtilities.h"
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

/**
 *
 *
 * @returns     -1 on critical error
 *              -2 when the client was unable to be added.
 */
int RemoveClientFromList(int msgQueue, int client)
{
    //need to read every message in the queue and fix it eventually.
    Mesg *oldMsg, *newMsg;
    bool found = false;
    int offset = 0;

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
        if(oldMsg->sockets[i] == client)
        {
            found = true;
            offset++;
        }

        newMsg->sockets[i] = oldMsg->sockets[i + offset];
    }

    if(!found)
    {
        printf("Client was not found.\n");
        if(SendMessage(msgQueue, oldMsg) < 0)
        {
            printf("RemoveClientFromList: Critical sendmessage failure.\n");
            return -1;
        }
        return -2;
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