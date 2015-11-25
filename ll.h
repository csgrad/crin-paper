#ifndef LL_H
#define LL_H

#include <stdbool.h>

/*
 * Author: Jason Ernst
 * Date Started: Friday December 7th, 2007
 * Linked List Implementation in C
 */

typedef struct list_node
{
  int data;
  struct list_node *next;
} llnode;

void insert(llnode **head, int data);
void append(llnode **head, int data);
void appendUnique(llnode **head, int data);
void insertUnique(llnode **head, int data);
bool equal_list(llnode *l1, llnode * l2);
bool lessThan(llnode * l1, llnode *l2);
void displayList(llnode *head);
void displayListAsSet(llnode *head);
void freeList(llnode *head);

llnode * findNode(llnode *head, int data);
llnode * copyList(llnode *head);

int numNodes(llnode *head);

bool removeNode(llnode **head, int data);
 
#endif