#include <iostream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

// defining the struct element for the linked list
struct LLNode {
    int data;
    LLNode *next;
};

// defining the class for linked list
class linkedlist{
    // the head and tail pointer to the linked list
    LLNode *list;
    LLNode *tail;

public:
    // constructor
    linkedlist(){
        // the first element is a dummy head
        LLNode *head = new LLNode();
        head->data = 0;
        head->next = NULL;
        list = head;
        tail = head;
    }

    // insert element x into the list
    void insert(int x) {
        //TODO: write code to insert element x to the end of the linkedlist
        LLNode *insert_node = new LLNode();
        insert_node->data = x;
        insert_node->next = NULL;
        tail->next = insert_node;
        tail = insert_node;
    }

    // remove the first occurrence of element x from the list
    void remove(int x) {
        // TODO : write code to remove element x from the list. Also print
        // printf("%d is NOT in the list\n",x); if x is not in the list
        if(!list->next)
            printf("%d is NOT in the list\n",x);

        LLNode *head = list->next;

        if (head->data == x){
            list->next = head->next;
            return;
        }

        LLNode *last = head;
        for (LLNode *iter = head->next; iter; iter = iter->next){
            if (iter->data == x)
            {
                if (iter == tail) tail = last;
                last->next = iter->next;
                return;
            }
            last = iter;
        }

        printf("%d is NOT in the list\n",x);
    }

    void print() {
        LLNode *p = list->next;

        while(p != NULL){
            cout << p->data << ", ";
            p = p->next;
        }

        cout << endl;
    }
};

int main(){
    linkedlist myList;

    myList.insert(2);

myList.insert(1);

myList.insert(3);

myList.insert(4);

myList.print();

myList.remove(3);

myList.print();

//TODO: write your own tests for the code

myList.insert(5);

myList.print();

myList.remove(5);

myList.print();

myList.insert(1);

myList.print();

myList.remove(2);

myList.print();

myList.remove(5);

myList.remove(4);

myList.print();

myList.remove(1);

myList.print();

    //TODO: write your own tests for the code

    return 0;
}
