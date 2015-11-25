/*
 * Author: Jason Ernst
 * Date Started: Friday December 7th, 2007
 * Linked List Implementation in C
 * References:   http://vergil.chemistry.gatech.edu/resources/programming/c-tutorial/lists.html
 *               Data Structures, Algorithms & Software Principles in C
 *               Problem Solving & Design in C
 * Note: Head must be set to NULL initially or seg faults will occur!
 */

#include <stdlib.h>   /* malloc, NULL */
#include <stdio.h>    /* printf, scanf for testing */
#include <stdbool.h>
#include "ll.h"       /* structure definition for ll */

/*
 * Returns true if l1 < l2
 */
bool lessThan(llnode * l1, llnode *l2)
{
  //if l1 has fewer nodes than l2 it is less
  if(numNodes(l1) < numNodes(l2))
    return true;
  
  //if l1 has more nodes it is not less
  if(numNodes(l1) > numNodes(l2))
    return false;
  //if there are equal amounts of nodes we need to compare vertices
  else
  {
    while(l1 != NULL && l2 != NULL)
    {
      if(l1->data > l2->data)
        return false;
      l1 = l1->next;
      l2 = l2->next;
    }
    return true;
  }
}

/*
 * Function which allocates memory for a new node
 * and assigns data to it
 */
llnode * newNode(int data_in)
{
  llnode *tmp = NULL;
  
  if ((tmp = (llnode*)malloc(sizeof(*tmp))) == NULL)
  {
    printf("Error allocating memory for the LL node, program ending.");
    exit(0);
  }
  
  tmp->data = data_in;
  tmp->next = NULL;
  return tmp;
}

/*
 * Function which inserts a node at the start of the list
 */
void insert(llnode **head, int data_in)
{
  llnode *tmp = newNode(data_in);
  tmp->next = *head;
  *head = tmp; 
}

/*
 * Function to insert node at the end of the list
 */
void append(llnode **head, int data_in)
{  
  if((*head)==NULL)
  {    
    (*head) = newNode(data_in);
  }
  else
  {
    append(&(*head)->next, data_in);
  }
}

/*
 * Function to insert node at the end of the list
 * if it does not already exist
 */
void appendUnique(llnode **head, int data)
{
  if(findNode(*head, data) == NULL)
    append(head, data);
}

void insertUnique(llnode **head, int data)
{
  if(findNode(*head, data) == NULL)
    insert(head, data);
}

/*
 * Returns the amount of nodes in the list
 */
int numNodes(llnode *head)
{
  if(head == NULL)
    return 0;
  else
    return 1 + numNodes(head->next);
}

/*
 * Returns true if both lists have the same elements
 */
bool equal_list(llnode *l1, llnode * l2)
{ 
  llnode * l1tmp = l1;
   
  while(l1 != NULL)
  {
    if(findNode(l2, l1->data) == NULL)
      return false;
    else
      l1 = l1->next;
  }
  
  //used to preserve the head of l2;
  l1 = l1tmp;
  
  while(l2 != NULL)
  {
    if(findNode(l1, l2->data) == NULL)
      return false;
    else
      l2 = l2->next;
  }
  
  return true;
}

/*
 * Function to display the entire list
 */
void displayList(llnode *head)
{
  llnode *tmp;
  
  //Loop through entire list displaying each node separated by -> link
  while (head != NULL)
  {
    printf("%d --> ", head->data);
    tmp = head->next;
    head = tmp;
  }
  printf("NULL \n");
}

/* 
 * Function to display the entire list in set notation
 */
void displayListAsSet(llnode *head)
{
  llnode *tmp;
  
  printf("{");
  
  //Loop through the list separating output by commas
  while (head != NULL)
  {
    printf("%d", head->data);
    tmp = head->next;
    head = tmp;
    //only print comma if there is another item in the list
    if(head != NULL)
      printf(",");
  }
  
  printf("}\n");
}

/*
 * Function to free the entire list from memory
 */
void freeList(llnode *head)
{
  llnode *tmp;
  
  //Loop through entire list freeing each node
  while (head != NULL)
  {
    tmp = head->next;
    free(head);
    head = tmp;
  }
}

/*
 * Function to return a new copy of the list
 */
llnode * copyList(llnode *head)
{
  llnode *newNode=NULL;
  llnode *tmp = head;
  
  //Loop through the list appending the data from each node to
  //the end of the newNode list
  while (head != NULL)
  {
    append(&newNode, head->data);
    tmp = head->next;
    head = tmp;
  }
  
  return newNode; 
}

/*
 * Recursive function to find a node in the list and return it
 * returns null if not found
 */
llnode * findNode(llnode *head, int data)
{
  //Loop through the list looking for a match
  while (head != NULL)
  {
    if(head->data == data)
    {
      return head;
    }
    else
    {
      return findNode(head->next, data);
    }
  }
  return NULL;
}

/* Recursive functon to remove the node from the list
 * true if found, false if not
 */
bool removeNode(llnode **head, int data)
{  
  llnode *tmp;
  
  if( (*head) != NULL)
  {
    //head to be removed
    if( (*head)->data == data)
    {
      tmp = (*head)->next;
      free(*head);
      *head = tmp;
      return true;
    }
    //move to next item in list
    else
    {
      return removeNode(&(*head)->next, data );
    }
  }
  else
  {
    return false;
  }
}

/*
 * Main function for testing
 *
int main()
{
  //head must be set to null to prevent segfault
  llnode * head = NULL;
  
  insert(&head, 1);
  insert(&head, 2);
  insert(&head, 3);
  
  llnode * head1 = NULL;
  
  insert(&head1, 1);
  insert(&head1, 3);
  insert(&head1, 2);
  insert(&head1, 4);
  
  if(lessThan(head, head1))
    printf("L1 < L2\n");
  else
    printf("L1 > L2\n");
  
  printf("L1: ");
  displayListAsSet(head);

  printf("L2: ");
  displayListAsSet(head1);
  
  if(equal_list(head, head1))
    printf("EQUAL LISTS\n");
  else
    printf("UNEQUAL LISTS\n");
  return 0;
}*/
