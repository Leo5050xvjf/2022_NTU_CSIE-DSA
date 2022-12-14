

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define finput 0






/** **************DLL結構 & 操作 ***************/
typedef struct pos_node{
    int pos;
    struct  pos_node* next;
    struct  pos_node* pre;
}P_Node;
typedef struct doubly_linked_listed{
    struct  pos_node* root;
    struct  pos_node* tail;
}DLL;
void insert_Dll(DLL*d,int setOfRoot){
    P_Node*new_p_node = malloc(sizeof(P_Node));
    new_p_node->pos = setOfRoot;
    new_p_node->pre = NULL;
    new_p_node->next = NULL;
    if(d->root == NULL){
        d->root = new_p_node;
        d->tail = new_p_node;
    }
    else{
        d->tail->next = new_p_node;
        new_p_node->pre  = d->tail;
        d->tail = new_p_node;
    }
}
void pop_dll(DLL*d,int setOfRoot){
    if(d->root == NULL){
        return;
    }
    if(d->root != d->tail){
        P_Node*del_node = d->tail;
        d->tail = d->tail->pre;
        d->tail->next = NULL;
        del_node->pre = NULL;
        free(del_node);

    }
    else{
        P_Node*del_node = d->tail;
        free(del_node);
    }
}











//建立Table:紀錄第i天梭需要的是哪一些boom k，也就是在offline時，先將所有command都看過，並且做出對應的整理。
/** **************offline的列表建立結構 & 操作 ***************/
typedef struct boom_num{
    int num;
    struct boom_num* next;
}Boom_NUM;
typedef struct table_node{
    char* command;
    int l_num,r_num,back_day,ans,visited,day;
    struct boom_num* boom_day_root;
    struct boom_num* boom_day_tail;
}T_Node;
typedef struct version_node{
    int version;
    int cur_set;
    int set_num;
    struct version_node* next;
    struct version_node* pre;
}V_Node;
typedef struct record_node{
    int cur_version;
    struct version_node* last_V_Node;
    struct version_node* root_V_Node;
}R_Node;
void instert_B_Node(T_Node*p,int day){
    Boom_NUM* n  = malloc(sizeof(Boom_NUM));
    assert(n!= NULL);
    n->next = NULL;
    n->num = day;
    if(p->boom_day_root == NULL){
        p->boom_day_root = n;
        p->boom_day_tail = n;
    }
    else{
        p->boom_day_tail->next = n;
        p->boom_day_tail = n;
    }
}
void init_T_Node(T_Node*p){
    p->visited = 0;
    p->ans = 0;
    p->day = 0;
    p->boom_day_root = NULL;
    p->boom_day_tail = NULL;
};
void find_correct_boom_day(int boom_day, int back_day, T_Node arr[],int state_arr[]){
    if(back_day == 0){
//        printf("back_day %d\n",back_day);
        instert_B_Node(&(arr[back_day]), boom_day);
        return;
    }
    else{
        if(back_day == state_arr[back_day]){
            if(arr[back_day].command[0] == 'm'){
//                printf("back_day %d\n",back_day);
                instert_B_Node(&(arr[back_day]), boom_day);
                return;
            }
            else{
                back_day = arr[back_day].back_day;
                find_correct_boom_day(boom_day,back_day,arr,state_arr);
            }
        }
        else{
            back_day = state_arr[back_day];
            find_correct_boom_day(boom_day,back_day,arr,state_arr);

        }
    }
}
//DLL相關函數


//更新和維護union set的方式要全部重新寫。
/**
 * void find_correct_boom_day(int boom_day, int back_day, T_Node arr[],int state_arr[]){
    if(back_day == 0){
//        printf("back_day %d\n",back_day);
        instert_B_Node(&(arr[back_day]), boom_day);
        return;
    }
    else{
        if(back_day == state_arr[back_day]){
            if(arr[back_day].command[0] == 'm'){
//                printf("back_day %d\n",back_day);
                instert_B_Node(&(arr[back_day]), boom_day);
                return;
            }
            else{
                back_day = arr[back_day].back_day;
                find_correct_boom_day(boom_day,back_day,arr,state_arr);
            }
        }
        else{
              back_day = state_arr[back_day];
              find_correct_boom_day(boom_day,back_day,arr,state_arr);

        }
    }
}
int findSET(int arr[],int a){
    if(arr[a] != a){
        arr[a] = findSET(arr,arr[a]);
    }
    return arr[a];

}
int merge(int arr[],int a,int b){
    int set_a = findSET(arr,a);
    int set_b = findSET(arr,b);
    if(set_a!=set_b){
        if(set_a<set_b){
            arr[set_b] = set_a;
        }
        else{arr[set_a] = set_b;}
        return 1;
    }
    return 0;
}
 int * copy_unionSet(const int arr[],int numOfSet){
    int * new_arr = malloc(sizeof(int) * numOfSet);
    assert(new_arr != NULL);
    for (int i = 0; i < numOfSet; ++i) {
        new_arr[i] = arr[i];
    }
    return new_arr;
}
 void jump_every_boom(T_Node Table[],int current_arr[],int current_day,int M){
//    會呼叫此函數表示已經確定需要用到new arr 否則根本不會呼叫此函數。
    int *new_arr = copy_unionSet(current_arr,M+1);
    while(current_day+1<M+1 &&(Table[current_day+1].command[0]!='b')){
        if(Table[current_day+1].command[0] == 'q'){
            Table[current_day+1].ans = new_arr[0];
        }
        else{
            int sameSetOrNot = merge(new_arr,Table[current_day+1].l_num,Table[current_day+1].r_num);
//            1代表不同set
            if(sameSetOrNot == 1){new_arr[0]-=1;}
//            此時得到更新後的new_arr，並且需要開始檢查此merge是否需要向下繼續遞迴。
            Boom_NUM*cur_node = Table[current_day+1].boom_day_root;
            while(cur_node != NULL){
                Table[cur_node->num].visited = 1;
                if((cur_node->num+1 < M+1) && (Table[cur_node->num+1].command[0] !='b')){
                    jump_every_boom(Table,new_arr,cur_node->num,M);
                }

                cur_node = cur_node->next;
            }
        }
        Table[current_day+1].visited = 1;
        current_day+=1;
    }
    free(new_arr);

}
 * */

/** **************Union Set ***************/
void init_UnionSet(R_Node UnionSetArr[], int numOfSet){
//    init root
    V_Node* ver_node = malloc(sizeof(V_Node));
    assert(ver_node != NULL);
    ver_node->cur_set = -1;
    ver_node->version = 0;
    ver_node->next = NULL;
    ver_node->pre = NULL;
    ver_node->set_num = numOfSet;
    UnionSetArr[0].cur_version = 0;
    UnionSetArr[0].root_V_Node = ver_node;
    UnionSetArr[0].last_V_Node = ver_node;
//init all set
    for (int i = 1; i < numOfSet + 1; ++i) {
        ver_node = malloc(sizeof(V_Node));
        assert(ver_node != NULL);
        ver_node->cur_set = i;
        ver_node->version = 0;
        ver_node->next = NULL;
        ver_node->pre = NULL;
        ver_node->set_num = 1;
        UnionSetArr[i].cur_version = 0;
        UnionSetArr[i].last_V_Node = ver_node;
        UnionSetArr[i].root_V_Node = ver_node;
    }
}
void copy_V_node(V_Node* new_node,V_Node*old_node){
    new_node->cur_set = old_node->cur_set;
//    new_node->version = old_node->version+1;
    new_node->set_num = old_node->set_num;
    new_node->next = NULL;
    new_node->pre = NULL;
}
void update_ver_node(R_Node UnionSetArr[], int set_root , int version){
    V_Node * new_v_node = malloc(sizeof(V_Node));
//    printf("new_ver: %d \n",version);
    new_v_node->version = version;
    copy_V_node(new_v_node,UnionSetArr[set_root].last_V_Node);
    UnionSetArr[set_root].last_V_Node->next = new_v_node;
    new_v_node->pre = UnionSetArr[set_root].last_V_Node;
    UnionSetArr[set_root].last_V_Node = new_v_node;
}
int findSet(R_Node UnionSetArr[], int set, int new_version, DLL*d){
    int rootOfSet = set;
    while(UnionSetArr[rootOfSet].last_V_Node->cur_set != rootOfSet){
        rootOfSet = UnionSetArr[rootOfSet].last_V_Node->cur_set;
    }
    if(UnionSetArr[rootOfSet].last_V_Node->version < new_version){
        update_ver_node(UnionSetArr, rootOfSet, new_version );
        insert_Dll(d,rootOfSet);
    }
    return rootOfSet;
}
//update UnionSetArr[0]
void update_num_of_set(R_Node UnionSetArr[]){
    V_Node * new_node = malloc(sizeof(V_Node));
    new_node->set_num = UnionSetArr[0].last_V_Node->set_num;
    new_node->pre = UnionSetArr[0].last_V_Node;
    new_node->version = UnionSetArr[0].last_V_Node->version+1;
    UnionSetArr[0].last_V_Node->next = new_node;
    UnionSetArr[0].last_V_Node = new_node;
}
void back_ver_node(R_Node UnionSetArr[], int set_root){
    V_Node*del_node = UnionSetArr[set_root].last_V_Node;
    UnionSetArr[set_root].last_V_Node = UnionSetArr[set_root].last_V_Node->pre;
    UnionSetArr[set_root].last_V_Node->next = NULL;
    del_node->pre = NULL;
    free(del_node);




//    V_Node* root = UnionSetArr[0].last_V_Node;
//    while (root!= NULL){
//        printf("%d-> ",root->set_num);
//        root = root->next;
//    }



}
void back_version(R_Node UnionSetArr[] ,DLL *d){
    if (d->root != NULL){
        P_Node* cur_pos = d->root;
        P_Node* next_pos = cur_pos->next;
        while(next_pos != NULL){
            back_ver_node(UnionSetArr,cur_pos->pos);
            free(cur_pos);
            cur_pos = next_pos;
            next_pos = cur_pos->next;
        }
        back_ver_node(UnionSetArr,cur_pos->pos);
        free(cur_pos);
    }


    V_Node* del_node = NULL;
    del_node = UnionSetArr[0].last_V_Node;
    UnionSetArr[0].last_V_Node = UnionSetArr[0].last_V_Node->pre;
    UnionSetArr[0].last_V_Node->next = NULL;
    del_node->pre = NULL;
//    free(d);
}
void merge(R_Node UnionSetArr[], int num_a, int num_b, int new_version, DLL*d){
    int root_a = 0,root_b = 0;
    root_a = findSet(UnionSetArr, num_a, new_version, d);
    root_b = findSet(UnionSetArr, num_b, new_version, d);
    if(root_a != root_b){
        UnionSetArr[0].last_V_Node->set_num-=1;
        if(UnionSetArr[root_a].last_V_Node->set_num >= UnionSetArr[root_b].last_V_Node->set_num){
            UnionSetArr[root_a].last_V_Node->set_num+=UnionSetArr[root_b].last_V_Node->set_num;
            UnionSetArr[root_b].last_V_Node->cur_set = root_a;
        }
        else{
            UnionSetArr[root_b].last_V_Node->set_num+=UnionSetArr[root_a].last_V_Node->set_num;
            UnionSetArr[root_a].last_V_Node->cur_set = root_b;
        }
//      由於一進去新版本就會先更新UnionSet[0]，也就是numOfSet的總數量，所以這裡不用再確認UnionSet[0]的版本
    }
}


/*************小工具專區 *************/
void showLinkedList(Boom_NUM* root){
    if(root == NULL){
        printf("NULL");
    }
    else{
        while(root!=NULL){
            printf("%d ->",root->num);
            root = root->next;
        }
    }

}
void showUnionSet(R_Node UnionSetArr[],int numOfSet){
    printf("cur_num: %d\ncur_num_ver: %d\n",UnionSetArr[0].last_V_Node->set_num,UnionSetArr[0].last_V_Node->version);
    printf("set_ver:   ");
    for (int i = 1; i < numOfSet+1; ++i) {
        printf("%d_%d  ",UnionSetArr[i].last_V_Node->cur_set,UnionSetArr[i].last_V_Node->version);
    }
    printf("\n\n");
}
void showDLL(DLL*d){
    P_Node* root = d->root;
    printf("DLL: ");
    while(root!= NULL){
        printf("%d ",root->pos);
        root = root->next;
    }
    printf("\n");
}
void show_all_query(T_Node Table[],int M){
    for (int i = 1; i < M+1; ++i) {
        if(Table[i].ans != 0){
            printf("%d\n",Table[i].ans);
        }
    }
}
void show_all_query_to_file(T_Node Table[],int M,FILE*fp){
    for (int i = 1; i < M+1; ++i) {
        if(Table[i].ans != 0){
            fprintf(fp,"%d\n",Table[i].ans);
        }
    }
}
/** **************利用graph概念處理遞迴結構 ***************/

void jump(T_Node Table[],R_Node UnionSetArr[],int dest_day,int cur_ver,int M){
    update_num_of_set(UnionSetArr);
    DLL*d = malloc(sizeof(DLL));
    d->root = NULL;
    d->tail = NULL;
    while( (dest_day+1 < M+1 )&&(Table[dest_day+1].command[0]!= 'b' )){
        if(Table[dest_day+1].command[0] == 'q'){
            Table[dest_day+1].ans = UnionSetArr[0].last_V_Node->set_num;
        }
        else{
            merge(UnionSetArr,Table[dest_day+1].l_num,Table[dest_day+1].r_num,cur_ver,d);
            Boom_NUM* temp_boom_root = Table[dest_day+1].boom_day_root;
            while(temp_boom_root != NULL){
                jump(Table,UnionSetArr,temp_boom_root->num,cur_ver+1,M);
                temp_boom_root = temp_boom_root->next;
            }
        }
        Table[dest_day+1].visited = 1;
        dest_day = dest_day + 1;
    }
    back_version(UnionSetArr,d);
}

int main() {
#if finput == 0
    int N = 0, M = 0;
    scanf("%d %d", &(N), &(M));
    T_Node Table[M + 1];
    int closest_state[M + 1];
    closest_state[0] = 0;
    int temp_state = 0;
//Table[0] 需要紀錄所有需要回到起始點的boom，但他並沒有command 所以另外處理
//TODO
    Table[0].boom_day_root = NULL;
    Table[0].boom_day_tail = NULL;
    Table[0].ans = 0;

    for (int i = 1; i < M + 1; ++i) {
        init_T_Node(&(Table[i]));
        Table[i].command = malloc(6);
        scanf("%s", Table[i].command);
        if (Table[i].command[0] == 'm') {
            scanf("%d %d", &(Table[i].l_num), &(Table[i].r_num));
            temp_state = i;
            closest_state[i] = i;
        } else if (Table[i].command[0] == 'b') {
            int day = 0;
            scanf("%d", &(day));
            Table[i].back_day = day;
            find_correct_boom_day(i, day, Table, closest_state);
            temp_state = i;
            closest_state[i] = i;
        } else {
            closest_state[i] = temp_state;
        }
    }
    R_Node UnionSetArr[N + 1];
    init_UnionSet(UnionSetArr, N);
    int cur_ver = 0;
    if(Table[0].boom_day_root!= NULL){
        Boom_NUM* temp_b_node = Table[0].boom_day_root;
        while(temp_b_node != NULL){
            jump(Table,UnionSetArr,temp_b_node->num,cur_ver+1,M);
            temp_b_node = temp_b_node->next;
        }
    }
    Table[0].visited = 1;

    DLL * d = malloc(sizeof(DLL));
    d->tail = NULL;
    d->root = NULL;
    for (int j = 1; j < M+1; ++j) {
        if(Table[j].visited == 0){
            if(Table[j].command[0] == 'q'){
                Table[j].ans = UnionSetArr[0].last_V_Node->set_num;
            }
            else if (Table[j].command[0] == 'm'){
                merge(UnionSetArr,Table[j].l_num,Table[j].r_num,cur_ver,d);
                Boom_NUM* temp_boom_root = Table[j].boom_day_root;
                while(temp_boom_root != NULL){
                    jump(Table,UnionSetArr,temp_boom_root->num,cur_ver+1,M);
                    temp_boom_root = temp_boom_root->next;
                }
            }
            Table[j].visited = 1;
        }
    }
    show_all_query(Table,M);


#else
    int N = 0, M = 0;
//    FILE * fp = fopen("../")


    for (int k = 0; k < 10000; ++k) {
        char p1[200] = "../sample0/sample";
        char p2[10];
        char p3[10] = ".txt";
        sprintf(p2,"%d",k);
        strcat(p1,p2);
        strcat(p1,p3);
        FILE* fp =  fopen(p1,"r");
        assert(fp!=NULL);

        fscanf(fp,"%d %d", &(N), &(M));
        T_Node Table[M + 1];
        int closest_state[M + 1];
        closest_state[0] = 0;
        int temp_state = 0;
//Table[0] 需要紀錄所有需要回到起始點的boom，但他並沒有command 所以另外處理
        Table[0].boom_day_root = NULL;
        Table[0].boom_day_tail = NULL;
        Table[0].ans = 0;

        for (int i = 1; i < M + 1; ++i) {
            init_T_Node(&(Table[i]));
            Table[i].command = malloc(6);
            fscanf(fp,"%s", Table[i].command);
            if (Table[i].command[0] == 'm') {
                fscanf(fp,"%d %d", &(Table[i].l_num), &(Table[i].r_num));
                temp_state = i;
                closest_state[i] = i;
            } else if (Table[i].command[0] == 'b') {
                int day = 0;
                fscanf(fp,"%d", &(day));
                Table[i].back_day = day;
                find_correct_boom_day(i, day, Table, closest_state);
                temp_state = i;
                closest_state[i] = i;
            } else {closest_state[i] = temp_state;}
        }
        R_Node UnionSetArr[N + 1];
        init_UnionSet(UnionSetArr, N);
//        exit(0);
        int cur_ver = 0;
        if(Table[0].boom_day_root!= NULL){
            Boom_NUM* temp_b_node = Table[0].boom_day_root;
            while(temp_b_node != NULL){
                jump(Table,UnionSetArr,temp_b_node->num,cur_ver+1,M);
                temp_b_node = temp_b_node->next;
            }
        }
        Table[0].visited = 1;

        DLL * d = malloc(sizeof(DLL));
        d->tail = NULL;
        d->root = NULL;
        for (int j = 1; j < M+1; ++j) {
            if(Table[j].visited == 0){
                if(Table[j].command[0] == 'q'){
                    Table[j].ans = UnionSetArr[0].last_V_Node->set_num;
                }
                else if (Table[j].command[0] == 'm'){
                    merge(UnionSetArr,Table[j].l_num,Table[j].r_num,cur_ver,d);
                    Boom_NUM* temp_boom_root = Table[j].boom_day_root;
                    while(temp_boom_root != NULL){
                        jump(Table,UnionSetArr,temp_boom_root->num,cur_ver+1,M);
                        temp_boom_root = temp_boom_root->next;
                    }
                }
                Table[j].visited = 1;
            }
        }
//        show_all_query(Table,M);
        char w_p1[200] = "../My_Ans/ans";
        char w_p2[10];
        char w_p3[10] = ".txt";
        sprintf(w_p2,"%d",k);
        strcat(w_p1,w_p2);
        strcat(w_p1,w_p3);
        FILE* w_fp =  fopen(w_p1,"w");
//        show_all_query(Table,M);
        show_all_query_to_file(Table,M,w_fp);
        fclose(fp);
        fclose(w_fp);
    }



#endif
    return 0;
}
