#include "MessageUtilities.h"

int main(void)
{
    int msgQueue = 0;   //Message queue to contain the list of sockets.
    Mesg *msg, *rcv;    //Messages for sending and receiving initially.
    int i = 0;          //Index used for the "for" loops

    if((msgQueue = OpenQueue()) < 0)
    {
        RemoveQueue(msgQueue);
        return 1;
    }

    msg = (Mesg*)malloc(sizeof(Mesg));
    rcv = (Mesg*)malloc(sizeof(Mesg));

    //Assign mock sockets to the initial message.
    msg->mesg_type = 1;
    for(i = 0; i < 200; i++)
    {
        msg->sockets[i] = i+i;
        msg->num_sockets = i + 1;
    }
    sprintf(msg->mesg_data, "%s", "Hello.");
    //printf("[num:%d][message:%s]\n", msg->num_sockets, msg->mesg_data);

    //Send the first message containing all the sockets.
    if(SendMessage(msgQueue, msg) < 0)
    {
        printf("Sendmessage failure\n");
        RemoveQueue(msgQueue);
        free(msg);
        free(rcv);

        return 1;
    }
    printf("Sent message to queue.\n");


#if 0
    //This section will add a socket to the message.
    //It removes the message, rearranges the message and replaces it.
    int newClient = 123;
    if(AddClientToList(msgQueue, newClient) < 0)
    {
        printf("\nUnable to add a new client.\n");
        free(msg);
        free(rcv);
        RemoveQueue(msgQueue);

        return 1;
    }
    printf("Added client to list.\n");

#endif


#if 0
    //This works, this is an example of reading only a copy of the message
    //without removing it from the message queue.
    Mesg *temp = (Mesg*)malloc(sizeof(Mesg));

    if(ReadMessageCopy(msgQueue, temp, 0))
    {
        printf("Copy failure.\n");
    }

    printf("Info about the copied message...\n");
    printf("[type:%ld][num_sockets:%d][message:%s]\n", temp->mesg_type, temp->num_sockets, temp->mesg_data);

    free(temp);

#endif


#if 0
    //This removes a client's socket from the message and
    //replace it with the new message
    if((i = RemoveClientFromList(msgQueue, 4)) < 0) //Client remove successfully
    {
        if(i == -2)
            printf("Client not found.\n");
        else
            printf("Critical error.\n");

        free(msg);
        free(rcv);
        RemoveQueue(msgQueue);
        return 1;
    }
    printf("Removed client from list.\n");

#endif


#if 0
    //This fails to remove a client's socket from the message and
    //replace it with the new message
    if((i = RemoveClientFromList(msgQueue, 3)) < 0) //Intended failure here.
    {
        if(i == -2)
        {
            printf("Client not found.\n");
        }
        else
        {
            printf("Critical error.\n");
            free(msg);
            free(rcv);
            RemoveQueue(msgQueue);
            return 1;
        }

    }
    printf("Test success client from list.\n");

#endif


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











