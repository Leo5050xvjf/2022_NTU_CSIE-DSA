#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define LEN 1000000

typedef struct d_stack{
    int cur_top;
    int max_top;
    long long *arr ;
}D_STACK;

void create_D_stack(D_STACK *ptr,int size);
int isEmpty(D_STACK *p);
int isFull(D_STACK*p);
long long pop(D_STACK *p);
void push(D_STACK *p,long long );
void printARR_D(D_STACK *p);

typedef struct c_stack{
    int cur_top;
    int max_top;
    char *arr ;
}C_STACK;
void create_C_stack(C_STACK *ptr,int size);
int isEmpty_c(C_STACK *p);
int isFull_c(C_STACK*p);
char pop_c(C_STACK *p);
void push_c(C_STACK *p,char);
void printARR(C_STACK *p);

void create_D_stack(D_STACK *ptr,int size){
    ptr->cur_top = -1;
    ptr->max_top = size-1;
    ptr->arr = (long long*) malloc(sizeof(long long) * size);
}
int isEmpty(D_STACK *p){
    if(p->cur_top == -1){return 1;}
    return -1;
}
int isFull(D_STACK*p){
    if(p->cur_top == p->max_top){return 1;}
    return -1;
}
long long pop(D_STACK *p){
    if (isEmpty(p) == -1){return p->arr[p->cur_top--];}
}
void push(D_STACK *p,long long i){
    if (isFull(p) == -1) {
        p->cur_top++;
        p->arr[p->cur_top] = i;
    }
}
void printARR_D(D_STACK *p){
    for (int i = 0 ;i < p->cur_top+1;i++){
        printf("%lld ",p->arr[i]);
    }
    printf("\n");
}



void create_C_stack(C_STACK *ptr,int size){
    ptr->cur_top = -1;
    ptr->max_top = size-1;
    ptr->arr = (char*) malloc(sizeof(char) * size);
}
int isEmpty_c(C_STACK *p){
    if(p->cur_top == -1){return 1;}
    return -1;
}
int isFull_c(C_STACK*p){
    if(p->cur_top == p->max_top){return 1;}
    return -1;
}
char pop_c(C_STACK *p){
    if (isEmpty_c(p) == -1){return p->arr[p->cur_top--];}
}
void push_c(C_STACK *p,char i){
    if (isFull_c(p) == -1){
        p->cur_top++;
        p->arr[p->cur_top] = i;
    }
}
void printARR(C_STACK *p){
    for (int i = 0 ;i < p->cur_top+1;i++){
        printf("%c ",p->arr[i]);
    }
    printf("\n");
}
long long calculate(char op, long long left,long long right ){
    if (op == '+'){return left+right;}
    else if (op == '-'){return left-right;}
    else if (op == '*'){return left*right;}
    else if (op == '/'){
        if (left==0){return 0;}
        else if((left > 0 && right > 0) || (left < 0 && right < 0)){
            return left/right;
        }
        else{return (left/right)-1;}
    }
    // else if (op == '/'){return (long long)floor((double)left/(double)right);}
    // else if (op == '/'){return floor((double)left/right);}
    // else if (op == '/'){
    //     if ((left == 0) || (right == 0)){ return 0;}
    //     //同號
    //     else if ((left*right)>0){
    //         return (left/right);
    //     } else{
    //         return (left/right)-1;
    //     }
    // }
    else{return left%right;}
}
//the divisor for / and % would not be zero during the process of the calculation;
//a > 0, b > 0, for all operations a%b.
int needCalOrNot(char new_op,char top_op){

    if ((new_op == ')')||(new_op =='='))return 1;
    else if(new_op == '('|| top_op == '(' ){return 0;}
    else if(new_op == '*' || new_op == '/' ||  new_op == '%'){
        if(top_op == '+' || top_op == '-'){return 0;}
        else return 1;
    }
    return 1;
}

int main() {

    char input[LEN];
    while (scanf("%s", input) != EOF) {
        //D_Stack 紀錄long long 的運算結果
        //C_Stack 紀錄operation
        D_STACK arr_D;
        C_STACK arr_C;
        create_D_stack(&(arr_D), LEN);
        create_C_stack(&(arr_C), LEN);

        int counter = 0;
        while (input[counter] != '\0') {
            //處理遇到數字的情況
            if (47 < input[counter] && input[counter] < 58){
                long long num = input[counter] - '0';
                while (47 < input[counter + 1] && input[counter + 1] < 58) {
                    long long t = input[counter + 1] - '0';
                    num = num * 10 + t;
                    counter++;
                }
                push(&(arr_D),num);
            }
            //處理遇到非數字的情況
            else {
                char arr_C_top ;
                if (arr_C.cur_top != -1){
                    arr_C_top = arr_C.arr[arr_C.cur_top];
                } else{arr_C_top = '(';}
                char new_C = input[counter];
                //若出現'=' 則最優先計算

                if (needCalOrNot(new_C, arr_C_top) == 0) { push_c(&(arr_C), new_C); }
                else {
                    while (arr_C.cur_top != -1 && needCalOrNot(new_C, arr_C.arr[arr_C.cur_top]) == 1 && arr_C.arr[arr_C.cur_top] != '(') {
                        long long right = pop(&(arr_D));
                        long long left = pop(&(arr_D));
                        long long ans = calculate(pop_c(&(arr_C)), left, right);
                        push(&(arr_D), ans);
                    }
                    //若 input[counter] 是 '=' 或 ')' 則不會放入stack_C

                    if ((new_C != ')')&&(new_C != '=')) { push_c(&(arr_C), new_C); }
                    if (arr_C.arr[arr_C.cur_top] == '(') { pop_c(&(arr_C)); }
                    if (new_C == '='){
                        printf( "Print: %lld\n", arr_D.arr[arr_D.cur_top]);
                    }

                }
            }
            counter++;
        }
        while(isEmpty_c(&(arr_C)) == -1){
            pop_c(&(arr_C));
        }
        while(isEmpty(&(arr_D)) == -1){
            pop(&(arr_D));
        }
        free(arr_D.arr);
        free(arr_C.arr);
    }
}


