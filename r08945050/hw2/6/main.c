#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define ToTA 1

typedef struct labelNode{
    int label;
    struct labelNode* next;
}LabelNode;
typedef struct city{
    struct city*s_previous;
    struct city*r_previous;
    LabelNode* neighbor_city_head;
    LabelNode* lastNode;
    int label;
}City;


City* createCity(int label){
    City* c =(City*) calloc(1,sizeof(City));
    assert(c != NULL);
    c->s_previous = NULL;
    c->r_previous = NULL;
    c->label = label;
    c->neighbor_city_head = NULL;
    c->lastNode = NULL;
    return c;
}
City** createCityArr(int num){
    City** arr = (City**)calloc(num+1,sizeof(City*));
    for (int i = 1; i < num+1; ++i) {
        arr[i] = createCity(i);
    }
    return arr;
}
LabelNode* createLabelNode(int label){
    LabelNode* p = (LabelNode*)calloc(1,sizeof(LabelNode));
    p->label = label;
    p->next = NULL;
    return p;
}

void addCityLabel(int city_a,int city_b,City**CityArr){
    City* A = CityArr[city_a];
    City* B = CityArr[city_b];
    LabelNode* label_a = createLabelNode(city_a);
    LabelNode* label_b = createLabelNode(city_b);
    if (A->lastNode == NULL){
        A->neighbor_city_head = label_b;
        A->lastNode = label_b;
    } else{
        A->lastNode->next = label_b;
        A->lastNode = label_b;
    }
    if(B->lastNode == NULL){
        B->neighbor_city_head = label_a;
        B->lastNode = label_a;
    } else{
        B->lastNode->next = label_a;
        B->lastNode = label_a;
    }
}

void DFScreateTree(City** CityArr,City*C,int parentIndex,int SorR){
//    printf("%d\n",parentIndex);
    if (SorR ==0){
        LabelNode* neighbor_city_head = C->neighbor_city_head;
        while (neighbor_city_head != NULL){
            if (neighbor_city_head->label != parentIndex){
                CityArr[neighbor_city_head->label]->s_previous = C;
                DFScreateTree(CityArr,CityArr[neighbor_city_head->label],C->label,SorR);
            }
            neighbor_city_head = neighbor_city_head->next;
        }
    } else{
        LabelNode* neighbor_city_head = C->neighbor_city_head;
        while (neighbor_city_head != NULL){
            if (neighbor_city_head->label != parentIndex){
                CityArr[neighbor_city_head->label]->r_previous = C;
                DFScreateTree(CityArr,CityArr[neighbor_city_head->label],C->label,SorR);
            }
            neighbor_city_head = neighbor_city_head->next;
        }
    }

}

void showPath(City*C,int SorR){
    if(SorR == 0){
        while (C != NULL){
            printf("%d ",C->label);
            C = C->s_previous;
        }
    }
    else{
        while (C != NULL){
            printf("%d ",C->label);
            C = C->r_previous;
        }
    };
}
void show_record_arr(int*record_arr,int numOfCity){
    printf("\n");
    for (int i = 1; i < numOfCity+1; ++i) {
        printf("%d ",record_arr[i]);

    }
    printf("\n");
}
void showAns(City*C ,int S ,int R,int Capital,int Resort,int*record_arr,int*temp_record_arr){
//    如果S 和 R 相等，那麼答案就只會出現在Capital  = Resort 的地方，完全不用浪費時間算。
    if (S == R){
        printf("%d",Capital);
    }
    else{

        City* temp_S = C;
        City* temp_R = C;
        if(record_arr[temp_S->label]!=0){
            printf("%d",record_arr[temp_S->label]);
        }
        else{
            int counter = 0;
            temp_record_arr[counter] =temp_S->label;
            counter +=1;
            int root = 0;
            while ((temp_S->s_previous != NULL && temp_R->r_previous != NULL) ){
                if(temp_S->s_previous->label != temp_R->r_previous->label){
                    break;
                } else{
                    if(record_arr[temp_S->s_previous->label] != 0){
                        root = record_arr[temp_S->s_previous->label];
                        break;
                    }
                    temp_S = temp_S->s_previous;
                    temp_R = temp_R->r_previous;
                    temp_record_arr[counter] = temp_S->label;
                    counter+=1;
                }
            }
            if (root == 0){
                root = temp_S->label;
            }
            for (int i = 0; i < counter; ++i) {
                record_arr[temp_record_arr[i]] = root;
            }
            printf("%d",root);

        }

    };
}
int main() {


#if ToTA == 0
    FILE *fp = fopen("../sample2.txt","r");
    assert(fp != NULL);
    char input[30] ;
    int numOfCity,numOfQuery,Capital,Resort;
    fscanf(fp, "%d %d %d %d", &(numOfCity), &(numOfQuery), &(Capital), &(Resort));
    City ** CityArr = createCityArr(numOfCity+1);
    int city_a,city_b,S = 0,R = 1;

    for (int i = 0; i < numOfCity-1; ++i) {
        fscanf(fp, "%d %d", &(city_a), &(city_b));
        addCityLabel(city_a,city_b,CityArr);
    }
    DFScreateTree(CityArr,CityArr[Capital],-1,0);
    if(Capital !=Resort){
        DFScreateTree(CityArr,CityArr[Resort],-1,1);}
    else{
        R = S;
    }
    int query = 0;
    int record_arr[(numOfCity+1)] , temp_record_arr[(numOfCity+1)];
    memset(record_arr,0,sizeof(record_arr));
    memset(temp_record_arr,0,sizeof(temp_record_arr));
    for (int j = 0; j < numOfQuery; ++j) {
        fscanf(fp, "%d", &(query));
        printf("query is %d\n",query);
        showAns(CityArr[query],S,R,Capital,Resort,record_arr,temp_record_arr);
        show_record_arr(record_arr,numOfCity);

        memset(temp_record_arr,0,numOfCity+1);
        if(j != numOfQuery-1){
            printf("\n");
        }
    }
    return 0;
#endif
#if ToTA == 1
    int numOfCity,numOfQuery,Capital,Resort;
    scanf("%d %d %d %d", &(numOfCity), &(numOfQuery), &(Capital), &(Resort));
    City ** CityArr = createCityArr(numOfCity+1);
    int city_a,city_b,S = 0,R = 1;
    for (int i = 0; i < numOfCity-1; ++i) {
        scanf("%d %d", &(city_a), &(city_b));
        addCityLabel(city_a,city_b,CityArr);
    }
    DFScreateTree(CityArr,CityArr[Capital],-1,0);
    if(Capital !=Resort){
        DFScreateTree(CityArr,CityArr[Resort],-1,1);}
    else{R = S;}
    int query = 0;
//    int *record_arr = calloc(numOfCity+1,(numOfCity+1)*sizeof(int));
//    int *temp_record_arr = calloc(numOfCity+1,(numOfCity+1)*sizeof(int));
    int record_arr[(numOfCity+1)] , temp_record_arr[(numOfCity+1)];
    memset(record_arr,0,sizeof(record_arr));
    memset(temp_record_arr,0,sizeof(temp_record_arr));
//    printf("num of  city is %d\n",numOfCity);
//    for (int k = 0; k < numOfCity+1; ++k) {
//        printf("%d ",record_arr[k]);
//
//    }
//    printf("\n");
//    for (int p = 0; p < numOfCity + 1; ++p) {
//        printf("%d ",temp_record_arr[p]);
//    }
//    printf("\n");
    for (int j = 0; j < numOfQuery; ++j) {
        scanf("%d", &(query));
        showAns(CityArr[query],S,R,Capital,Resort,record_arr,temp_record_arr);
//        memset(temp_record_arr,0,numOfCity+1);
//        show_record_arr(record_arr,numOfCity);
        if(j != numOfQuery-1){
            printf("\n");
        }
    }
    return 0;
#endif




}
