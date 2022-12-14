#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define LEN 1000
#define P2Booster 0
#define P3_1 0
#if P2Booster == 1
typedef struct stack{
    int num;
    int current_top;
    int *arr;
}Stack;
void createStack(Stack*s,int len){
    s->num = 0;
    s->current_top = -1;
    int *arr = (int*)calloc(LEN,sizeof(int));
    s->arr = arr;
}
int pop(Stack*s){
    if (s->current_top == -1){
        return 0;
    } else {
        int ans = s->arr[s->current_top];
        s->arr[s->current_top] = 0;
        s->current_top-=1;
        return ans;
    }
}
void push(Stack*s,int num){
    if(s->current_top == 999){
        printf("full!\n");
        return;
    }
    s->arr[s->current_top+1] = num;
    s->current_top+=1;
}
#endif

#if P2Booster == 1
//    Stack s;
//    createStack(&(s),LEN);
//    Stack*p = &(s);
//    int record_arr[LEN] = {0};
//    int input_num = 0 ;
//
//    scanf("%d",&(input_num));
//    while ( input_num!= -1){
//        if(record_arr[input_num] == 0){
//            record_arr[input_num] = 1;
//            push(p,input_num);
//        }
//        else{
//            if(input_num == p->arr[p->current_top]){
//                int temp = pop(p);
//                record_arr[temp] = 0;
//            }
//            else{
//                printf("input %d is invalid\n",input_num);
//                break;
//            }
//        }
//        scanf("%d",&(input_num));
//    }
#endif
#if P3_1 == 0
typedef struct node {
    int value;
    struct node* next;
}Node;

Node *createNode(int num){
    Node* n = NULL;
    n = (Node*)calloc(1,sizeof(Node));
    assert(n != NULL);
    n->next = NULL;
    n->value = num;
    return n;
}
//create singly linked list
Node* createSLL(int numOfNode){
    if (numOfNode == 0){
        return NULL;
    }
    Node*head = createNode(0);
    Node *temp = head;
    for (int i = 1; i < numOfNode; i++) {
        temp->next = createNode(i);
        temp = temp->next;
    }
    return head;
}

#endif

int main(){
    int numOfNode = 0;
    scanf("%d",&(numOfNode));
    Node *head = createSLL(numOfNode);
    Node *currentNode = head;
    Node *middleNode = head;
    int midNodeID = 1;
    int counter = 1;
    int update = 2*midNodeID+1;
    if (head == NULL){
        printf("%s\n","NULL");
        return 0;
    }
    while(currentNode->next != NULL){
        counter+=1;
        currentNode = currentNode->next;
        if(counter == update){
            middleNode = middleNode->next;
            midNodeID+=1;
            update = midNodeID*2+1;
        }
    }
    printf("ans is %d \n",midNodeID);
    return 0;
}
