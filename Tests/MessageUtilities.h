//
// Created by tyler on 29/02/16.
//

#ifndef MESSAGEQUEUE_MESSAGEUTILITIES_H
#define MESSAGEQUEUE_MESSAGEUTILITIES_H

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
#include "Message.h"

int ReadMessage(int queue, Mesg* msg, long msg_type);
int ReadMessageCopy(int queue, Mesg* msg, long msg_type);
int RemoveClientFromList(int msgQueue, int client);
int SendMessage(int queue, Mesg* msg);
int AddClientToList(int msgQueue, int client);
int SendFinalMessage(int queue, Mesg* msg);
int RemoveQueue(int queue);
int OpenQueue(void);

#endif //MESSAGEQUEUE_MESSAGEUTILITIES_H
