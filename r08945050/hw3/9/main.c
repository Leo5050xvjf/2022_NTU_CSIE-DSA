#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct stack{
    int counter;
    int* arr;
}STACK;
void push(STACK *S,int element){
    S->arr[S->counter] = element;
    S->counter+=1;
}
int pop(STACK *S){
    int num = S->arr[S->counter-1];
    S->counter-=1;
    return num;
}
STACK* createStack(int k){
    STACK* p = malloc(sizeof(STACK));
    assert(p!= NULL);
    p->arr = malloc(sizeof(int)*(2*k));
    assert(p->arr!= NULL);
    p->counter = 0;
    return p;
}
typedef struct node{
    int original_pos;
    unsigned long long hash_value;
}Node;
int * createDict(){
    int * dict = (int*)malloc(sizeof(int)*200);
    for (int i = 33; i <127 ; ++i) {
        // dict[i] = i-32;
        dict[i] = i-33;


    }
    return dict;
}
void createHashTable(Node* arr[],const char* str,int str_num,int l,unsigned long long q,int base,const int* dict){
    unsigned long long hash_val = 0,temp_hash_val = 0;
    for (int i = 0; i < l; ++i) {
        hash_val = (((hash_val*base)%q)+dict[str[i]])%q;
    }
    unsigned long long sub_term = 0,multi = 1;
    for (int j = l-1; j >= 0 ; --j) {
        sub_term = (dict[str[j]]*multi)%q;
        multi = (multi*base)%q;
        temp_hash_val = (hash_val >= sub_term) ? hash_val:hash_val+q;
        arr[j][str_num].hash_value = temp_hash_val-sub_term;
        arr[j][str_num].original_pos = str_num;
    }


}
int compare (const void * a, const void * b)
{
    Node *data_1 = (Node *)a;
    Node *data_2 = (Node *)b;
    if( data_1->hash_value == data_2->hash_value)
    {
        return 0 ;
    }
    else if( data_1->hash_value  < data_2->hash_value )
        return -1;
    else
        return 1;
}
int BinarySearch( Node* arr,int l,int r,unsigned long long x){

    if (r>=l){
        int mid = l+(r-l)/2;
        if(x == arr[mid].hash_value){
            return mid;
        }
        if(x > arr[mid].hash_value){
            return BinarySearch(arr,mid+1, r, x);
        }
        return BinarySearch(arr, l,mid-1, x);
    }
    return r;
}


Node* createNode(int ori_pos,unsigned long long HV ){
    Node* p = malloc(sizeof(Node));
    assert(p!= NULL);
    p->original_pos = ori_pos;
    p->hash_value = HV;
    return p;
}
void showOriPos(Node* arr[],int k,int l){
    for (int j = 0; j < k  ; ++j) {
        for (int i = 0; i < l; ++i) {
            printf("%d ",arr[i][j].original_pos);
        }
        printf("\n");
    }

}
void showHashVal(Node* arr[],int str_num,int l){
    for (int i = 0; i < l; ++i) {
        printf("%llu ",arr[i][str_num].hash_value);

    }
    printf("\n");
}

int main() {
    int k = 0,l = 0,flag = 0;
    unsigned long long q =196241960000000000;
    scanf("%d %d %d",&(k),&(l),&(flag));
    char *stringArr[k],*str = NULL;
    int * dict = createDict();

    Node* NodeTable[l];
    Node*p = NULL;
    for (int j = 0; j < l; ++j) {
        p = malloc(sizeof(Node)*k);
        assert(p!= NULL);
        NodeTable[j] = p;
    }
    for (int i = 0; i < k; ++i) {
        str = malloc(sizeof(char)*(l+1));
        assert(str!= NULL);
        scanf("%s",str);
        stringArr[i] = str;
        createHashTable(NodeTable,stringArr[i],i,l,q,94,dict);
    }

    unsigned long long ori_hashvalueTable[l][k];
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < l; ++j) {
            ori_hashvalueTable[j][i] = NodeTable[j][i].hash_value;
        }
    }

    /**
     * NodeTable[j]: 遮住第j個字元位置的string
     * NodeTable[j][s]: 遮住第j個字元位置的第s個string 的hash value
     * */
    /*
    接著將NodeTable的每個row, 對於 hash value進行排序，以利於後續的二元搜尋
    */
    for (int m = 0; m < l; ++m) {
        qsort(NodeTable[m],k,sizeof(Node),compare);
    }

    if(flag == 1){
        int* record_visited = calloc(k,sizeof(int));
        assert(record_visited!=NULL);
        STACK* record_stack = createStack(k);
        unsigned long long total_match_num = 0;
        for (int i1 = 0; i1 < k; ++i1) {
            if(record_visited[i1] == 0 && l > 1){
                /*total_same_string_num 是在避免2個完全相同的string 會重複計算，例如 "abcc" , "abcc" 兩者不論遮住哪個字元都會有相同的Hash Value， 因此要記得扣掉重複計算的部分。
                */
                unsigned long long total_same_string_num = 0,partial_same_string_num = 0;
                for (int i = 0; i < 2; ++i) {
                    int HV_pos = BinarySearch(NodeTable[i],0,k-1,ori_hashvalueTable[i][i1]);

                    int temp_up = HV_pos-1,temp_down = HV_pos+1;
                    unsigned long long  HV = ori_hashvalueTable[i][i1];
                    int str_pos = NodeTable[i][HV_pos].original_pos;
                    if(record_visited[str_pos] < 2){
                        push(record_stack,str_pos);
                        record_visited[str_pos]+=1;
                    }

                    while(temp_up >= 0 && (NodeTable[i][temp_up].hash_value==HV)){
                        str_pos = NodeTable[i][temp_up].original_pos;
                        if(record_visited[str_pos] < 2){
                            push(record_stack,str_pos);
                            record_visited[str_pos]+=1;
                        }
                        temp_up--;
                    }
                    while (temp_down < k  && (NodeTable[i][temp_down].hash_value == HV)){
                        str_pos = NodeTable[i][temp_down].original_pos;
                        if(record_visited[str_pos] < 2){
                            push(record_stack,str_pos);
                            record_visited[str_pos]+=1;
                        }
                        temp_down++;
                    }
                }
                while (record_stack->counter != 0 ){
                    int sameHV_string_pos = pop(record_stack);
                    if (record_visited[sameHV_string_pos] == 2){
                        total_same_string_num+=1;
                    }
                    else{record_visited[sameHV_string_pos] = 0;}
                }
                total_same_string_num/=2;
                for (int j = 0; j < l; ++j) {

                    int HV_pos = BinarySearch(NodeTable[j],0,k-1,ori_hashvalueTable[j][i1]);
                    if(record_visited[NodeTable[j][HV_pos].original_pos] < 2){
                        partial_same_string_num+=1;
                    }
                    int temp_up = HV_pos-1,temp_down = HV_pos+1;
                    unsigned long long  HV = ori_hashvalueTable[j][i1];

                    while(temp_up >= 0 && (NodeTable[j][temp_up].hash_value==HV)){
                        int str_pos = NodeTable[j][temp_up].original_pos;
                        if(record_visited[str_pos] < 2){
                            partial_same_string_num+=1;
                        }
                        temp_up--;
                    }
                    while (temp_down < k  && (NodeTable[j][temp_down].hash_value == HV)){
                        int str_pos = NodeTable[j][temp_down].original_pos;
                        if(record_visited[str_pos] < 2){

                            partial_same_string_num+=1;
                        }
                        temp_down++;
                    }
                }
                total_match_num+= (total_same_string_num*partial_same_string_num);
                total_match_num+=( ( total_same_string_num*(total_same_string_num-1) )/ 2);

            }
            else if (record_visited[i1] == 0 && l ==  1){
                unsigned long long total_same_string_num = 0;
                int HV_pos = BinarySearch(NodeTable[0],0,k-1,ori_hashvalueTable[0][i1]);
                int temp_up = HV_pos-1,temp_down = HV_pos+1;
                unsigned long long  HV = NodeTable[0][HV_pos].hash_value;
                while(temp_up >= 0 && (NodeTable[0][temp_up].hash_value==HV)){
                    int ori_pos = NodeTable[0][temp_up].original_pos;
                    if(record_visited[ori_pos]  == 0){
                        record_visited[ori_pos]+=1;
                        push(record_stack,ori_pos);
                        total_same_string_num+=1;
                    }
                    temp_up--;
                }
                while(temp_down >= 0 && (NodeTable[0][temp_down].hash_value==HV)){
                    int ori_pos = NodeTable[0][temp_down].original_pos;
                    if(record_visited[ori_pos]  == 0){
                        record_visited[ori_pos]+=1;
                        push(record_stack,ori_pos);
                        total_same_string_num+=1;
                    }
                    temp_down++;
                }
                total_same_string_num+=1;
                push(record_stack,NodeTable[0][HV_pos].original_pos);
                while(record_stack->counter != 0){
                    record_visited[pop(record_stack)] = 2;
                }
                total_match_num+=( ( (unsigned long long )total_same_string_num*((unsigned long long )total_same_string_num-1) )/ 2);
            }



        }
        if(total_match_num == 0){
            printf("No");
        } else{
            printf("Yes\n");
            printf("%llu",total_match_num);
        }

    }
    else {

        unsigned long long total_match_num = 0;
        for (int i1 = 0; i1 < k; ++i1) {
            unsigned long long total_same_string_num = 0, partial_same_string_num = 0;
            for (int i = 0; i < l; ++i) {
                int HV_pos = BinarySearch(NodeTable[i], 0, k - 1, ori_hashvalueTable[i][i1]);
                int temp_up = HV_pos - 1, temp_down = HV_pos + 1;
                unsigned long long HV = ori_hashvalueTable[i][i1];

                if (temp_up >= 0 && (NodeTable[i][temp_up].hash_value == HV)) {
                    printf("%s\n","Yes");
                    printf("%d %d", NodeTable[i][temp_up].original_pos, NodeTable[i][HV_pos].original_pos);
                    return 0;
                }

                if (temp_down < k && (NodeTable[i][temp_down].hash_value == HV)) {
                    printf("%s\n","Yes");
                    printf("%d %d", NodeTable[i][temp_down].original_pos, NodeTable[i][HV_pos].original_pos);
                    return 0;
                }
            }
        }
        printf("%s","No");
        return 0;
    }
    return 0;
}
