#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "price.h"
typedef long long ll;

typedef struct m_Heap{
    int number;
    ll stock;
    ll *arr;
    ll nowDay;
}HEAP;
typedef struct g_Heap{
    int number;
    HEAP **arr;
}G_HEAP;
HEAP* createHeap(ll stock,ll len){
    HEAP * h = malloc(sizeof(HEAP));
    assert(h);
    h->number = 0;
    h->stock = stock;
    h->arr = malloc(sizeof(ll)*len);
    h->nowDay = 1;
    return h;
}
void insert(HEAP*h,ll element){
    h->arr[h->number+1] = element;
    h->number+=1;
    h->nowDay+=1;
    int cur_pos = h->number;
    while (  ( cur_pos/2 ) >= 1 && h->arr[cur_pos/2] > h->arr[cur_pos] ){
        ll temp = h->arr[cur_pos];
        h->arr[cur_pos] = h->arr[cur_pos/2];
        h->arr[cur_pos/2] = temp;
        cur_pos /= 2;
    }
}
ll pop(HEAP* h){
    if(h->number == 0) {
        printf("error op\n");
        exit(0);
    }
    ll minimum = h->arr[1];
    h->arr[1] = h->arr[h->number];
    h->number--;
    int root = 1;
    while ( (root * 2) <= h->number || (root * 2 + 1) <= h->number) {
        int largest = root;
        if (root * 2 <= h->number && h->arr[root * 2] < h->arr[largest]) {
            largest = root * 2;
        }
        if (root * 2 + 1 <= h->number && h->arr[root * 2 + 1] < h->arr[largest]) {
            largest = root * 2 + 1;
        }
        if (largest == root) {
            break;
        } else {
            ll temp = h->arr[largest];
            h->arr[largest] = h->arr[root];
            h->arr[root] = temp;
            root = largest;
        }
    }
    return minimum;

}
void insertGlobalHeap(G_HEAP* g_heap,HEAP*h) {
    g_heap->arr[g_heap->number + 1] = h;
    g_heap->number += 1;
    int cur_pos = g_heap->number;
    while ((cur_pos / 2) >= 1 && g_heap->arr[cur_pos / 2]->arr[1] > g_heap->arr[cur_pos]->arr[1]){
        HEAP *temp = g_heap->arr[cur_pos];
        g_heap->arr[cur_pos] = g_heap->arr[cur_pos / 2];
        g_heap->arr[cur_pos / 2] = temp;
        cur_pos /= 2;
    }
}
void maintainGlobalHeap(G_HEAP*h){
    int root = 1;
    while ( (root * 2) <= h->number || (root * 2 + 1) <= h->number){
        int largest = root;
        if (root * 2 <= h->number && h->arr[root * 2]->arr[1] < h->arr[largest]->arr[1]) {
            largest = root * 2;
        }
        if (root * 2 + 1 <= h->number && h->arr[root * 2 + 1]->arr[1] < h->arr[largest]->arr[1]) {
            largest = root * 2 + 1;}
        if (largest == root){break;
        }
        else {
            HEAP* temp = h->arr[largest];
            h->arr[largest] = h->arr[root];
            h->arr[root] = temp;
            root = largest;
        }
    }
}
void showGlobalHeap(G_HEAP* h, int len){
    for (int i = 1; i < len+1; ++i) {
        printf("%llu ",h->arr[i]->arr[1]);
    }
    printf("\n\n");
}
void SWAP_HEAP(HEAP**arr,int a,int b){
    HEAP* temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
void showHeap(ll arr[],int len){
    for (int i = 1; i < len+1; ++i) {
        printf("%llu ",arr[i]);
    }
    printf("\n");
}
int main(){
    int A,Q,N;
    scanf("%d %d %d",&(A),&(Q),&(N));
    HEAP* BrianStock[A+1];
    ll brian_s = 0;
    for (int i = 0; i < A; ++i) {
        scanf("%llu",&(brian_s));
        BrianStock[i+1] = createHeap(brian_s,N+2);
        for (int j = 1; j < N+2; ++j) {
            insert(BrianStock[i+1],price(brian_s,j));;
        }
    }
    G_HEAP* g_heap = calloc(1,sizeof(G_HEAP));
    g_heap->arr = malloc(sizeof(HEAP*)*A+1);
    for (int k = 1; k < A+1; ++k) {
        insertGlobalHeap(g_heap,BrianStock[k]);
    }
    ll* BrianSorted = malloc(sizeof(ll)* 1000001);
    for (int l = 1; l < 1000001; ++l) {
        BrianSorted[l] = pop(g_heap->arr[1]);
        insert(g_heap->arr[1],price(g_heap->arr[1]->stock,g_heap->arr[1]->nowDay));
        maintainGlobalHeap(g_heap);
    }



    int i,k_th;
    for (int m = 0; m < Q; ++m) {
        scanf("%d %d",&(i),&(k_th));
        if(m == 0){
            if (i == 0){
                printf("%llu",BrianSorted[k_th]);
            }
        } else{
            if (i == 0){
                printf("\n%llu",BrianSorted[k_th]);
            }
        }
    }
    return 0;
}
/***
 *
 * //    HEAP*a = createHeap(10,10);
//    HEAP*b = createHeap(15,10);
//    int t_arr[5]  = {2,12,11,10,15  };
//    int t2_arr[5] = {7,8,9,11,10};
//    HEAP* arr[2] = {a,b};
//    printf("arr[0]: %llu arr[1]: %llu\n",arr[0]->stock,arr[1]->stock);
//    SWAP_HEAP(arr,0,1);
//    printf("arr[0]: %llu arr[1]: %llu\n",arr[0]->stock,arr[1]->stock);
//    for (int i = 1; i < 5+1; ++i) {
//        insert(a,t_arr[i-1]);
//        showHeap(a->arr,a->number);
//    }
//    printf("\n\n\n");
//    for (int i = 1; i < 5+1; ++i) {
//        insert(b,t2_arr[i-1]);
//        showHeap(b->arr,b->number);
//    }
//    G_HEAP global_heap;
//    global_heap.arr = malloc(sizeof(HEAP*)*2);
//    global_heap.number = 0;
//    insertGlobalHeap(&(global_heap),a);
//    insertGlobalHeap(&(global_heap),b);
//    showGlobalHeap(&(global_heap),2);
//    ll ans1 = pop(global_heap.arr[1]);
//    printf("%llu \n",ans1);
//    showGlobalHeap(&(global_heap),2);
//    maintainGlobalHeap(&(global_heap));
//    printf("stock is %llu\n",global_heap.arr[2]->stock);
//    showGlobalHeap(&(global_heap),2);


//    printf("\n");
//    for (int i = 1; i < 5+1; ++i) {
//        printf("root %llu \n",pop(a));
//        showHeap(a->arr,a->number);
//    }
 * ***/