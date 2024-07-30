//
// Created by Lxiunneg on 2023/11/28.
//

#ifndef HASHMAP_LINKLIST_H
#define HASHMAP_LINKLIST_H

#define bool int
#define OK 1
#define NO 0


#include "malloc.h"
#include "stdio.h"

typedef int ElemType;

typedef struct LinkNode{
    ElemType data; // The data of head-node is the length of linklist
    struct LinkNode* next;
}LinkNode;

struct LinkNode* Init(){
    //DEFAULT CONSTRUCTOR
    //ReturnValue: The address of the head node
    struct LinkNode* newNode = (struct LinkNode*)malloc(sizeof(LinkNode));
    newNode->data = 0; //The length of the initialized linked list is zero
    newNode->next = NULL;
    return newNode;
}

void DestroyList(struct LinkNode* head){
    //What the function does: Free up the memory space of the linked list
    //Parameter: The header node of the linked list

    struct LinkNode *pre = head,*p = pre->next;
    while(p!=NULL){
        free(pre);
        pre = p;
        p = p->next;
    }
    free(pre);
}


bool isListEmpty(struct LinkNode* head){
    //What the function does: Detect if a linked list is empty
    //Parameter: The head node of the linked list
    //Return value: bool: true -> null false -> non-null

    return head->data;
}

int ListLength(struct LinkNode* head){
    //What the function does: Detect if a linked list is empty
    //Parameter: The head node of the linked list
    //Return Value: The length of the linked list


    return head->data;
}

void DispList(struct LinkNode* head){
    //What the function does: Outputs the elements of the linked list
    //Parameter: The head node of the linked list

    if (!isListEmpty(head)){
        //Linked lists are not allowed to be empty
        printf("The List is empty!\n");
        return;
    }

    int len = ListLength(head);
    struct LinkNode* curNode = head->next;

    printf("[");
    for(int i=0;i<len-1;i++){
        printf("%d -> ",curNode->data);
        curNode = curNode->next;
    }
    printf("%d]\n",curNode->data);

    return;
}

bool getElem(struct LinkNode* head,int index,int* elem){
    //What the function does:Get an element with the rank of index
    //Parameter:The head node of the linked list, the rank, the element obtained
    //Return value: OK:obtained, NO:not obtained

    if(!isListEmpty(head)){
        //Linked lists are not allowed to be empty
        printf("The List is Empty!\n");
        return NO;
    }

    if(index+1 > ListLength(head) || index < 0){
        //The rank must be within the range of the linked list
        printf("The index is out of range!\n");
        return NO;
    }

    struct LinkNode* curNode = head->next;

    for(int i=0;i<index;i++) curNode = curNode->next; //Moves the  pointer to the rank node

    *elem = curNode->data;

    return OK;
}

int localElem(struct LinkNode* head,int elem){
    //What the function does:Locate the rank of the element in the linked list
    //Parameter:The head node of the linked list, The element you want to position
    //Return value: -1:Element not found, other:The rank of the element

    int index = 0;
    struct LinkNode* curNode = head->next;

    while(curNode != NULL && curNode->data != elem){
        index++;
        curNode = curNode->next;
    }

    return curNode == NULL ? -1 : index;
}

bool ListInsert(struct LinkNode* head,int index,int elem){
    //What the function does:Insert the element elem at the rank of index
    //Parameter:The head node of the linked list,The rank you want to insert,The element you want to insert
    //Return value: OK:Successfully inserted,NO:Insertion failed

    if(head == NULL){
        //Check if the linked list exists
        printf("The List is not exist!\n");
        return NO;
    }

    if(!isListEmpty(head)){
        //Linked lists are not allowed to be empty
        printf("The List is Empty!\n");
        return NO;
    }

    if(index < 0 || index >= ListLength(head)) {
        //The rank must be within the range of the linked list
        printf("The index is out of range!\n");
        return NO;
    }

    head->data++;//Linked list length +1
    struct LinkNode* curNode = head->next;
    struct LinkNode* newNode = (struct LinkNode*) malloc(sizeof (struct LinkNode));
    newNode->data = elem;

    if(index == 0){//Inserted in the head
        newNode->next = head->next;
        head->next = newNode;
        return OK;
    }

    for(int i=0;i<index-1;i++) curNode = curNode->next;//Move to the previous position of the position you want to insert

    //It must be written like this, otherwise it will result in the loss of the linked list
    newNode->next = curNode->next;
    curNode->next = newNode;

    return OK;

}

bool ListDelete(struct LinkNode* head,int index,int* elem){
    //What the function does:Delete the element elem at the rank of index
    //Parameter:The head node of the linked list,The rank you want to delete,The element you have deleted
    //Return value: OK:Successfully Deleted,NO:Delete failed

    if(head == NULL){
        //Check if the linked list exists
        printf("The List is not exist!\n");
        return NO;
    }

    if(!isListEmpty(head)){
        //Linked lists are not allowed to be empty
        printf("The List is Empty!\n");
        return NO;
    }

    if(index < 0 || index >= ListLength(head)) {
        //The rank must be within the range of the linked list
        printf("The index is out of range!\n");
        return NO;
    }

    head->data--;//Linked list length -1
    struct LinkNode* curNode = head->next;
    struct LinkNode* delNode;

    if(index == 0){
        delNode = head->next;
        head->next = delNode->next;
        delNode->next = NULL;
        free(delNode);
        return OK;
    }

    for(int i=0;i<index-1;i++) curNode = curNode->next;//Move to the previous position of the position you want to delete

    delNode = curNode->next;

    curNode->next = delNode->next;
    delNode->next = NULL;

    free(delNode);//Free up memory

    return OK;
}

void createListFront(struct LinkNode* head,ElemType* array,int len){
    //What the function does:Use header interpolation to convert an array into a linked list. Note: Arrays are reversed.
    //Parameter:The head node of the linked list, the array to be converted, and the length of the array

    struct LinkNode* newNode;
    head->data = len;
    for(int i=0;i<len;i++){
        newNode = (struct LinkNode*)malloc(sizeof(struct LinkNode));
        newNode->data = array[i];
        newNode->next = head->next;
        head->next = newNode;
    }

}

void createListBack(struct LinkNode* head,ElemType* array,int len){
    //What the function does:Use header interpolation to convert an array into a linked list.
    //Parameter:The head node of the linked list, the array to be converted, and the length of the array


    struct LinkNode* newNode,*tail;
    tail = head;
    head->data = len;
    for(int i=0;i<len;i++){
        newNode = (struct LinkNode*)malloc(sizeof(struct LinkNode));
        newNode->data = array[i];
        tail->next = newNode;
        tail = tail->next;
        tail->next = NULL;
    }
}
#endif //HASHMAP_LINKLIST_H
