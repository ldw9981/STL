

#include<stdio.h>
#include "List.h"


int main()
{
    int a = 5, b = 15, c = 43, d = 23, e = 12, f = 66, g = 99, h = 65, i = 20, j = 8;
    int pos = 1, insertMidValue = 500, deletePos = 4;

    DoubleLinkedList list;

    //Create a linked list
    printf("Insert nodes at tail: %d, %d, %d\n", a, b, c);
    list.Insert_at_tail(a);
    list.Insert_at_tail(b);
    list.Insert_at_tail(c);


    //print the list forward
    list.PrintLinkedListForward();

    printf("Insert node: %d at middle. Position: %d\n", insertMidValue, pos);

    list.InsertAfterPos(insertMidValue, pos);


    //print the list forward
    list.PrintLinkedListForward();


    printf("Delete item of position number %d", deletePos);
    list.DeleteNode(deletePos);

    //print the list forward
    list.PrintLinkedListForward();

    printf("Insert nodes at front: %d\n", d);
    list.Insert_at_head(d);

    //print the list forward
    list.PrintLinkedListForward();

    printf("Insert nodes at tail: %d\n", e);
    list.Insert_at_tail(e);

    //print the list forward
    list.PrintLinkedListForward();

    printf("Insert nodes at front: %d\n", f);
    list.Insert_at_head(f);

    //print the list forward
    list.PrintLinkedListForward();

    printf("Delete first node of list\n");
    list.DeleteNode(1);

    //print the list forward
    list.PrintLinkedListForward();

    printf("Insert nodes at tail: %d, %d, %d, %d\n", g, h, i, j);
    list.Insert_at_tail(g);
    list.Insert_at_tail(h);
    list.Insert_at_tail(i);
    list.Insert_at_tail(j);

    //print the list forward
    list.PrintLinkedListForward();

    //print the list backward
    list.PrintLinkedListBackward();

    //Searching the value in the list
    int value = h;
    if (list.IsValueAvailable(value) == true)
        printf("%d found in the list.\n\n", value);
    else
        printf("%d not found in the list.\n\n", value);

    return 0;
}