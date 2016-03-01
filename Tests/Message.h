//
// Created by tyler on 29/02/16.
//

#ifndef MESSAGEQUEUE_MESSAGE_H
#define MESSAGEQUEUE_MESSAGE_H

#define MAXMESSAGEDATA 	10
#define MAXCONNECTIONS 	1500
#define MAXMSGSIZE 		8192
#define MSG_COPY		040000
#define READMSG         (MSG_COPY | IPC_NOWAIT)
#define REMOVEMSG       0

typedef struct
{
    long    mesg_type;                  /* Message type */
    int     sockets[MAXCONNECTIONS];    /* Array of socket connections */
    int     num_sockets;                /* Number of sockets */
    char    mesg_data[MAXMESSAGEDATA];  /* Small char array contains */
} Mesg;

#endif //MESSAGEQUEUE_MESSAGE_H
