/*
    Doubly Linked List implementation by C programming Language
    Operations: create list, insert item to tail, insert item to first, insert item to middle,
        delete any item according to position,
        print list forward order, print list reverse order
    Programmed by: Hasan Abdullah
    Contact: http://hellohasan.com/
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct linked_list
{
    int number;
    struct linked_list* next;
    struct linked_list* previous;
};

typedef struct linked_list node;
node* head = NULL, * tail = NULL;

node* getNewNode(int val);
void insert_at_head(int value);
void insert_at_tail(int value);
void insertAfterPos(int value, int position);
void deleteNode(int position);
void deleteNode(node* pNode);
void printLinkedListForward();
void printLinkedListBackward();
bool isValueAvailable(int value);
node* findNodeByPosition(int position);

/*
    User defined functions
*/

//create a new node and returns to caller
node* getNewNode(int val)
{
    node* temp_node = (node*)malloc(sizeof(node));
    if (temp_node != nullptr)
    {
        temp_node->number = val;
        temp_node->next = NULL;
        temp_node->previous = NULL;
    }
    return temp_node;
}

//Insert a node at front of the list. This node will be the new head
void insert_at_head(int value)
{
    node* newNode = getNewNode(value);

    if (head == NULL) //For the 1st element
    {
        //For now, newNode is the only one node of list
        //So it it is head and also tail
        head = newNode;
        tail = newNode;
        return;
    }

    //newNode will be the NEW HEAD of list.
    //So it'll point to head as 'next node'
    newNode->next = head;
    head->previous = newNode; //before, the previous node of head was NULL. but now newNode

    head = newNode; //update the global node 'head' by newNode
}

//Insert a node after last node
void insert_at_tail(int value)
{
    node* newNode = getNewNode(value);

    if (head == NULL) //For the 1st element
    {
        head = newNode;
        tail = newNode;
        return;
    }

    //'tail' is a global node. 'newNode' will be the next node of tail.
    //finally newNode will be the 'tail' node of the list
    tail->next = newNode;
    newNode->previous = tail; //'newNode' point 'tail' node as previous node

    tail = newNode; // update the global node 'tail' by newNode.
}


// delete any node of list according to position
void deleteNode(int position)
{
    node* pFindNode = findNodeByPosition(position);
    deleteNode(pFindNode);
}

// print the list in forward order
void printLinkedListForward()
{
    printf("\nYour updated linked list in FORWARD ORDER:\n");

    node* myList;
    myList = head;

    while (1)
    {
        if (head == NULL || tail == NULL) break;

        printf("%d ", myList->number);

        if (myList == tail) break;

        myList = myList->next;
    }
    puts("\n");
}

// print the list in reverse order
void printLinkedListBackward()
{
    printf("\nYour full linked list in REVERSE ORDER:\n");

    node* myList;
    myList = tail;

    while (1)
    {
        if (head == NULL || tail == NULL) break;

        printf("%d ", myList->number);

        if (myList->previous == NULL) break;

        myList = myList->previous;
    }
    puts("\n");
}

//This function search the value in the list
bool isValueAvailable(int value)
{
    node* p = head;
    while (p != NULL)
    {
        if (p->number == value)
        {
            return true;
        }
        else
        {
            p = p->next;
        }
    }
    return 0;
}

node* findNodeByData(int data)
{
    node* pNode = head;
    while (pNode != nullptr)
    {
        if (pNode->number == data)
        {
            return pNode;
        }
        else
        {
            pNode = pNode->next;
        }
    }
    return nullptr;
}

// 첫번째 노드가 0이다
node* findNodeByPosition(int position)
{
    int positionCount = 0;
    node* pNode = head;
    while (pNode != nullptr)
    {
        if (position == positionCount)
        {
            return pNode;
        }
        else
        {
            pNode = pNode->next;
            positionCount++;
        }
    }
    return nullptr;
}



void deleteNode(node* pNode)
{
    if (pNode == nullptr)
    {
        printf("deleteNode(nullptr)\n");
        return;
    }

    if (pNode == head)
    {
        head = pNode->next;     // 지우려는 노드가 head이면 다음노드로 head설정
    }
    else if (pNode == tail)
    {
        tail = pNode->previous; // 지우려는 노드가 tail이면 이전 노드로 tail설정
    }

    // 포인터 정리
    if (pNode->previous != nullptr)
    {
        pNode->previous->next = pNode->next;
    }

    if (pNode->next != nullptr)
    {
        pNode->next->previous = pNode->previous;
    }

    //할당 해제
    free(pNode);
}

void insertAfterNode(int data, node* pNode)
{
    if (pNode == nullptr)
    {
        printf("insertNodeAfter(nullptr)\n");
        return;
    }

    node* pNewNode = getNewNode(data);
    pNewNode->number = data;
    pNewNode->previous = pNode;
    pNewNode->next = pNode->next;

    pNode->next = pNewNode;

    if (pNode == tail)
    {
        tail = pNewNode;
    }
}

void insertAfterPos(int data, int position)
{
    node* pFindNode = findNodeByPosition(position);
    if (pFindNode == nullptr)
    {
        printf("insertAfterPos()  pFindNode==nullptr\n");
        return;
    }
    insertAfterNode(data, pFindNode);
}


void deleteNodeByData(int data)
{
    node* pFindNode = findNodeByData(data);
    if (pFindNode == nullptr)
    {
        printf("no data: %d\n", data);
        return;
    }
    deleteNode(pFindNode);
}


int main()
{
    int a = 5, b = 15, c = 43, d = 23, e = 12, f = 66, g = 99, h = 65, i = 20, j = 8;
    int pos = 1, insertMidValue = 500, deletePos = 4;

    //Create a linked list
    printf("Insert nodes at tail: %d, %d, %d\n", a, b, c);
    insert_at_tail(a);
    insert_at_tail(b);
    insert_at_tail(c);


    //print the list forward
    printLinkedListForward();

    printf("Insert node: %d at middle. Position: %d\n", insertMidValue, pos);

    insertAfterPos(insertMidValue, pos);


    //print the list forward
    printLinkedListForward();


    printf("Delete item of position number %d", deletePos);
    deleteNode(deletePos);

    //print the list forward
    printLinkedListForward();

    printf("Insert nodes at front: %d\n", d);
    insert_at_head(d);

    //print the list forward
    printLinkedListForward();

    printf("Insert nodes at tail: %d\n", e);
    insert_at_tail(e);

    //print the list forward
    printLinkedListForward();

    printf("Insert nodes at front: %d\n", f);
    insert_at_head(f);

    //print the list forward
    printLinkedListForward();

    printf("Delete first node of list\n");
    deleteNode(1);

    //print the list forward
    printLinkedListForward();

    printf("Insert nodes at tail: %d, %d, %d, %d\n", g, h, i, j);
    insert_at_tail(g);
    insert_at_tail(h);
    insert_at_tail(i);
    insert_at_tail(j);

    //print the list forward
    printLinkedListForward();

    //print the list backward
    printLinkedListBackward();

    //Searching the value in the list
    int value = h;
    if (isValueAvailable(value) == true)
        printf("%d found in the list.\n\n", value);
    else
        printf("%d not found in the list.\n\n", value);

    return 0;
}