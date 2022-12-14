#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define LEN 200
#define ToTA 1
#define unit_test 0


typedef struct people{
    int group;
    int label;
    struct people *next;
    struct people *previous;
    struct people *super_link;
}People;
typedef struct bathroom{
    int isClose;
    int AllBathroomNum ;
    int labelOfBathroom;
    int numOfPeopleAll;
    int numOfGroup;
    int *numOfPeopleEachGroup;
    int *PreOrNext;
    struct people** AllHeadTail;
    struct bathroom* nextBathroom;
    struct bathroom* previousBathroom;
}Bathroom;
void freeBathroom(Bathroom*p){
    free(p->numOfPeopleEachGroup);
    free(p->AllHeadTail);
    free(p->PreOrNext);
}

struct bathroom* createBathroom(int bathroomLabel,int numOfGroup){
    Bathroom*p = (Bathroom*)calloc(1,sizeof(Bathroom));
    p->isClose = -1;
    p->AllBathroomNum = 0;
    p->labelOfBathroom = bathroomLabel;
    p->numOfPeopleAll = 0;
    p->numOfGroup = numOfGroup;
    p->numOfPeopleEachGroup = (int*)calloc(numOfGroup,sizeof(int));
    p->PreOrNext = (int*)calloc(((numOfGroup+1)*2),sizeof(int));
    p->AllHeadTail = (People**)calloc(((numOfGroup+1)*2),sizeof(People*));
    return p;
}
struct bathroom* createCircularBathroom(int numOfBathroom,int numOfGroup, struct bathroom**array){
//    printf("array address is %p\n",array);
    Bathroom*head = NULL;
    Bathroom*temp = NULL;

    head =createBathroom(0,numOfGroup);
    temp = head;
    array[0] = head;
    for (int i = 1;i<numOfBathroom;i++){
        temp->nextBathroom = createBathroom(i,numOfGroup);
        temp->nextBathroom->previousBathroom = temp;
        array[i] = temp->nextBathroom;
        temp = temp->nextBathroom;
    }
    if (numOfBathroom>2){
        temp->nextBathroom =head;
        head->previousBathroom = temp;
    }
    return head;
}
People* createPeople(int group,int label){
    People*p = (People*)calloc(1,sizeof(People));
    p->group = group;
    p->label = label;
    p->next = NULL;
    p->previous = NULL;
    p->super_link = NULL;
    return p;
}
void enter(int group,int label ,int labelOfBathroom,Bathroom** BathroomArr){
    People* newPeople = createPeople(group, label);
    Bathroom*B = BathroomArr[labelOfBathroom];
    int numOfGroup = B->numOfGroup;
    if (B->isClose == 1){
//        printf("enter wrong operation!\n");
        return;
    }
    else{
        if (B->numOfPeopleAll == 0){
            B->AllHeadTail[group] = newPeople;
            B->AllHeadTail[group+numOfGroup+1] = newPeople;
            B->PreOrNext[group] = 1;
            B->PreOrNext[group+numOfGroup+1] = 1;

            B->numOfPeopleEachGroup[group] += 1;
            B->AllHeadTail[numOfGroup] = newPeople;
            B->AllHeadTail[numOfGroup*2+1] = newPeople;
            B->PreOrNext[numOfGroup] = 1;
            B->PreOrNext[numOfGroup*2+1] = 1;
        }
        else{
            if(B->numOfPeopleEachGroup[group] == 0){
                if (B->PreOrNext[numOfGroup*2+1]==1){
                    B->AllHeadTail[numOfGroup*2+1]->next = newPeople;
                    newPeople->previous = B->AllHeadTail[numOfGroup*2+1];
                    B->numOfPeopleEachGroup[group] += 1;
                    B->PreOrNext[group] = 1;
                    B->PreOrNext[group+numOfGroup+1] = 1;
                    B->AllHeadTail[group] = newPeople;
                    B->AllHeadTail[group+numOfGroup+1] = newPeople;
                    B->AllHeadTail[numOfGroup*2+1] = newPeople;
                }
                else{
                    B->AllHeadTail[numOfGroup*2+1]->previous = newPeople;
                    newPeople->next = B->AllHeadTail[numOfGroup*2+1];
                    B->numOfPeopleEachGroup[group] += 1;
                    B->PreOrNext[group] = -1;
                    B->PreOrNext[group+numOfGroup+1] = -1;

                    B->AllHeadTail[group] = newPeople;
                    B->AllHeadTail[group+numOfGroup+1] = newPeople;
                    B->AllHeadTail[numOfGroup*2+1] = newPeople;
                }
            }
            else{
                if (B->PreOrNext[group+numOfGroup+1] ==1){
                    if (B->AllHeadTail[group+numOfGroup+1]->super_link == NULL && B->AllHeadTail[group+numOfGroup+1]->next == NULL){
                        B->AllHeadTail[group+numOfGroup+1]->next = newPeople;
                        newPeople->previous = B->AllHeadTail[group + numOfGroup + 1];

                        B->AllHeadTail[numOfGroup*2+1] = newPeople;
                        B->AllHeadTail[group+numOfGroup+1] = newPeople;

                        B->numOfPeopleEachGroup[group] += 1;
                    }
                    else if (B->AllHeadTail[group+numOfGroup+1]->next != NULL){
                        newPeople->next = B->AllHeadTail[group + numOfGroup + 1]->next;
                        B->AllHeadTail[group+numOfGroup+1]->next->previous = newPeople;

                        B->AllHeadTail[group+numOfGroup+1]->next = newPeople;
                        newPeople->previous = B->AllHeadTail[group + numOfGroup + 1];

                        B->AllHeadTail[group+numOfGroup+1] = newPeople;
                        B->numOfPeopleEachGroup[group] += 1;
                    }
                    else{
                        People* temp = B->AllHeadTail[group+numOfGroup+1]->super_link;
                        B->AllHeadTail[group+numOfGroup+1]->super_link = NULL;
                        temp->super_link = NULL;

                        B->AllHeadTail[group+numOfGroup+1]->next = newPeople;
                        newPeople->previous =B->AllHeadTail[group + numOfGroup + 1];

                        newPeople->super_link = temp;
                        temp->super_link = newPeople;

                        B->AllHeadTail[group+numOfGroup+1] = newPeople;

                        B->numOfPeopleEachGroup[group] += 1;
                    }

                }
                else{
                    if (B->AllHeadTail[group+numOfGroup+1]->super_link == NULL && B->AllHeadTail[group+numOfGroup+1]->previous == NULL){
                        B->AllHeadTail[group+numOfGroup+1]->previous = newPeople;
                        newPeople->next = B->AllHeadTail[group + numOfGroup + 1];

                        B->AllHeadTail[numOfGroup*2+1] = newPeople;
                        B->AllHeadTail[group+numOfGroup+1] = newPeople;

                        B->numOfPeopleEachGroup[group] += 1;
                    }
                    else if (B->AllHeadTail[group+numOfGroup+1]->previous != NULL){
                        newPeople->previous = B->AllHeadTail[group + numOfGroup + 1]->previous;
                        B->AllHeadTail[group+numOfGroup+1]->previous->next = newPeople;

                        B->AllHeadTail[group+numOfGroup+1]->previous = newPeople;
                        newPeople->next = B->AllHeadTail[group + numOfGroup + 1];

                        B->AllHeadTail[group+numOfGroup+1] = newPeople;
                        B->numOfPeopleEachGroup[group] += 1;
                    }
                    else{
                        People* temp = B->AllHeadTail[group+numOfGroup+1]->super_link;
                        B->AllHeadTail[group+numOfGroup+1]->super_link = NULL;
                        temp->super_link = NULL;

                        B->AllHeadTail[group+numOfGroup+1]->previous = newPeople;
                        newPeople->next =B->AllHeadTail[group + numOfGroup + 1];

                        newPeople->super_link = temp;
                        temp->super_link = newPeople;

                        B->AllHeadTail[group+numOfGroup+1] = newPeople;

                        B->numOfPeopleEachGroup[group] += 1;
                    }

                }
            }
        }
        B->numOfPeopleAll+=1;
    }
}
void leave(int labelOfBathroom,Bathroom** BathroomArr){
    Bathroom*B = BathroomArr[labelOfBathroom];
    if (B->numOfPeopleAll == 0){
//        printf("leave wrong operation!\n");
        return;}
    else{
        if (B->numOfPeopleAll == 1){
            People* lastPeople = B->AllHeadTail[B->numOfGroup*2+1];
            int last_people_group = lastPeople->group;
            B->numOfPeopleEachGroup[last_people_group]-=1;
            B->numOfPeopleAll-=1;
            B->PreOrNext[B->numOfGroup] = 0;
            B->PreOrNext[B->numOfGroup*2+1] = 0;
            B->PreOrNext[last_people_group] = 0;
            B->PreOrNext[last_people_group+B->numOfGroup+1] = 0;
            B->AllHeadTail[B->numOfGroup] =NULL;
            B->AllHeadTail[B->numOfGroup*2+1] =NULL;

            B->AllHeadTail[last_people_group] =NULL;
            B->AllHeadTail[last_people_group+B->numOfGroup+1] =NULL;
//            free(lastPeople);
        }
        else{
            People * second_last_people = NULL;
            People* lastPeople = B->AllHeadTail[B->numOfGroup*2+1];
            int lastPeopleNextOrPre = 0;
            if (lastPeople->previous != NULL && B->PreOrNext[B->numOfGroup*2+1] == 1){
                second_last_people = lastPeople->previous;
                lastPeopleNextOrPre = 1;
            }
            else if (lastPeople->next != NULL && B->PreOrNext[B->numOfGroup*2+1] == -1){
                second_last_people = lastPeople->next;
                lastPeopleNextOrPre = -1;
            }
            else if (lastPeople->super_link != NULL){
                second_last_people = lastPeople->super_link;
                second_last_people->super_link = NULL;
                lastPeople->super_link = NULL;
                if (lastPeople->group == second_last_people->group){
                    B->PreOrNext[lastPeople->group+B->numOfGroup+1]*=-1;
                    B->PreOrNext[B->numOfGroup*2+1] = B->PreOrNext[lastPeople->group+B->numOfGroup+1];
                    if (B->PreOrNext[lastPeople->group+B->numOfGroup+1] == 1){
                        lastPeople->previous = second_last_people;
                        second_last_people->next = lastPeople;
                    }
                    else{
                        lastPeople->next = second_last_people;
                        second_last_people->previous = lastPeople;
                    }
                }
                else{
                    B->PreOrNext[lastPeople->group+B->numOfGroup+1]*=-1;
                    B->PreOrNext[lastPeople->group] = B->PreOrNext[lastPeople->group+B->numOfGroup+1];
                    B->PreOrNext[B->numOfGroup*2+1] = B->PreOrNext[lastPeople->group+B->numOfGroup+1];
                    if (B->PreOrNext[lastPeople->group+B->numOfGroup+1] == 1){
                        lastPeople->previous = second_last_people;
                        second_last_people->next = lastPeople;
                    }else{
                        lastPeople->next = second_last_people;
                        second_last_people->previous = lastPeople;
                    }
                }
                lastPeopleNextOrPre = B->PreOrNext[lastPeople->group+B->numOfGroup+1];
            }
            else{
                second_last_people = NULL;
            }
//            final pop the last one
            if (second_last_people != NULL){
                if (second_last_people->group ==lastPeople->group){
                    B->AllHeadTail[lastPeople->group+B->numOfGroup+1] = second_last_people;
                    B->AllHeadTail[B->numOfGroup*2+1] = second_last_people;

                    B->numOfPeopleEachGroup[lastPeople->group]-=1;
                    B->numOfPeopleAll-=1;

                    if (lastPeopleNextOrPre == 1){
                        second_last_people->next = NULL;
                        lastPeople->previous = NULL;
                    }else{
                        second_last_people->previous = NULL;
                        lastPeople->next = NULL;
                    }
//                free(lastPeople);

                }
                else{
                    B->AllHeadTail[B->numOfGroup*2+1] = second_last_people;
                    B->AllHeadTail[lastPeople->group] = NULL;
                    B->AllHeadTail[lastPeople->group+B->numOfGroup+1] = NULL;

                    B->numOfPeopleEachGroup[lastPeople->group]-=1;
                    B->numOfPeopleAll-=1;
                    B->PreOrNext[lastPeople->group] = 0;
                    B->PreOrNext[lastPeople->group+B->numOfGroup+1] = 0;

                    if (lastPeopleNextOrPre == 1){
                        second_last_people->next = NULL;
                        lastPeople->previous = NULL;
                    }else{
                        second_last_people->previous = NULL;
                        lastPeople->next = NULL;
                    }

//                free(lastPeople);
                }
                if (second_last_people->super_link != NULL){
                    People * new_second_last_people = second_last_people->super_link;
                    int new_lastPeopleNextOrPre = B->PreOrNext[B->numOfGroup*2+1];
                    if (new_lastPeopleNextOrPre == 1){
                        second_last_people->super_link = NULL;
                        new_second_last_people->super_link = NULL;

                        new_second_last_people->previous = second_last_people;
                        second_last_people->next = new_second_last_people;

                        B->PreOrNext[second_last_people->group+B->numOfGroup+1] = -1;
                        B->PreOrNext[B->numOfGroup*2+1] = -1;
                    }
                    else{
                        second_last_people->super_link = NULL;
                        new_second_last_people->super_link = NULL;

                        new_second_last_people->next = second_last_people;
                        second_last_people->previous = new_second_last_people;


                        B->PreOrNext[second_last_people->group+B->numOfGroup+1] = 1;
                        B->PreOrNext[B->numOfGroup*2+1] = 1;
                    }
                    if (second_last_people->group != new_second_last_people->group){
                        B->PreOrNext[second_last_people->group] = B->PreOrNext[second_last_people->group+B->numOfGroup+1] ;
                    }
                }
                if (B->numOfPeopleEachGroup[second_last_people->group] == 1){
                    B->PreOrNext[second_last_people->group] = B->PreOrNext[second_last_people->group+B->numOfGroup+1];
                }
            }
//          deal with end of SL Problem
//            free(lastPeople);
        }
    }
}
void go(int labelOfBathroom,Bathroom** BathroomArr){
    if (labelOfBathroom == 10){
        char test[10] = "here!";
    }
    Bathroom*B = BathroomArr[labelOfBathroom];
    if (B->numOfPeopleAll == 0){
//        printf("go wrong operation!\n");
        return;}
    else{
        People*HEAD = B->AllHeadTail[B->numOfGroup];
        if (B->numOfPeopleAll == 1){
            People* firstPeople = B->AllHeadTail[B->numOfGroup];
            int firstPeopleGroup = firstPeople->group;
            B->numOfPeopleEachGroup[firstPeopleGroup]-=1;
            B->numOfPeopleAll-=1;
            B->PreOrNext[B->numOfGroup] = 0;
            B->PreOrNext[B->numOfGroup*2+1] = 0;
            B->PreOrNext[firstPeopleGroup] = 0;
            B->PreOrNext[firstPeopleGroup + B->numOfGroup + 1] = 0;
            B->AllHeadTail[B->numOfGroup] =NULL;
            B->AllHeadTail[B->numOfGroup*2+1] =NULL;

            B->AllHeadTail[firstPeopleGroup] =NULL;
            B->AllHeadTail[firstPeopleGroup+B->numOfGroup+1] =NULL;
//            free(firstPeople);
        }
        else{
            People * second_people = NULL;
            People* firstPeople = B->AllHeadTail[B->numOfGroup];
            int firstPeople_nextOrPre = 0;
            if (B->PreOrNext[B->numOfGroup] ==1 && firstPeople->next != NULL){
                second_people = firstPeople->next;
                firstPeople_nextOrPre =1;
            }
            else if (B->PreOrNext[B->numOfGroup] ==-1&& firstPeople->previous != NULL){
                second_people = firstPeople->previous;
                firstPeople_nextOrPre = -1;
            }
            else if(firstPeople->super_link != NULL) {
                second_people = firstPeople->super_link;
                second_people->super_link = NULL;
                firstPeople->super_link = NULL;
                if (B->PreOrNext[firstPeople->group] == 1){
                    second_people->next = firstPeople;
                    firstPeople->previous = second_people;
                    if (firstPeople->group == second_people->group){
                        B->PreOrNext[B->numOfGroup] *=-1;
                        B->PreOrNext[firstPeople->group]*=-1;
                    }
                    else{
                        B->PreOrNext[B->numOfGroup] *=-1;
                        B->PreOrNext[firstPeople->group]*=-1;
                        B->PreOrNext[firstPeople->group+B->numOfGroup+1]*=-1;
                    }


                }
                else{
                    second_people->previous = firstPeople;
                    firstPeople->next = second_people;
                    if (firstPeople->group == second_people->group){
                        B->PreOrNext[B->numOfGroup] *=-1;
                        B->PreOrNext[firstPeople->group]*=-1;
                    }
                    else{
                        B->PreOrNext[B->numOfGroup] *=-1;
                        B->PreOrNext[firstPeople->group]*=-1;
                        B->PreOrNext[firstPeople->group+B->numOfGroup+1]*=-1;
                    }
                }
                firstPeople_nextOrPre = B->PreOrNext[B->numOfGroup];
            }
            else{
                second_people = NULL;
            }
//          pop the first people
            if (second_people != NULL){
                if (second_people->group == firstPeople->group){
                    B->AllHeadTail[firstPeople->group] = second_people;
                    B->AllHeadTail[B->numOfGroup] = second_people;

                    B->numOfPeopleEachGroup[firstPeople->group]-=1;
                    B->numOfPeopleAll-=1;
                    if (firstPeople_nextOrPre == 1){
                        second_people->previous=NULL;
                        firstPeople->next=NULL;
                    }
                    else{
                        second_people->next=NULL;
                        firstPeople->previous=NULL;
                    }
//                free(firstPeople);
                }
                else{

                    B->AllHeadTail[B->numOfGroup] = second_people;
                    B->AllHeadTail[firstPeople->group] = NULL;
                    B->AllHeadTail[firstPeople->group+B->numOfGroup+1] = NULL;

                    B->numOfPeopleEachGroup[firstPeople->group]-=1;
                    B->numOfPeopleAll-=1;

                    B->PreOrNext[firstPeople->group] = 0;
                    B->PreOrNext[firstPeople->group+B->numOfGroup+1] = 0;



                    if (firstPeople_nextOrPre == 1){
                        second_people->previous = NULL;
                        firstPeople->next = NULL;
                    }
                    else{
                        second_people->next = NULL;
                        firstPeople->previous = NULL;
                    }
//                free(firstPeople);
                }
//            deal with SL Problem
                if (second_people->super_link != NULL){
                    People * new_second_people = second_people->super_link;
                    int new_firstPeople_nextOrPre = B->PreOrNext[B->numOfGroup];
                    if (new_firstPeople_nextOrPre == 1){
                        second_people->super_link = NULL;
                        new_second_people->super_link = NULL;

                        new_second_people->next = second_people;
                        second_people->previous = new_second_people;

                        B->PreOrNext[B->numOfGroup] = -1;
                        B->PreOrNext[second_people->group] = -1;
                    }
                    else{
                        second_people->super_link = NULL;
                        new_second_people->super_link = NULL;

                        new_second_people->previous= second_people;
                        second_people->next = new_second_people;

                        B->PreOrNext[B->numOfGroup] = 1;
                        B->PreOrNext[second_people->group] = 1;
                    }
                    if(second_people->group != new_second_people->group){
                        B->PreOrNext[second_people->group+B->numOfGroup+1] = B->PreOrNext[second_people->group];
                    }
                }
                if (B->numOfPeopleEachGroup[second_people->group] == 1){
                    B->PreOrNext[second_people->group+B->numOfGroup+1] = B->PreOrNext[second_people->group];
                }
            }
//            free(firstPeople);

        }
    }
}
void close(int BathroomNum,Bathroom** BathroomARR){
    Bathroom* Head_B = BathroomARR[0];
    Bathroom* source_Bathroom = BathroomARR[BathroomNum];
    Bathroom* dest_Bathroom = NULL;
    if (source_Bathroom->isClose == 1){
//        printf("close wrong operation!\n");
        return;
    }
    else if (source_Bathroom->numOfPeopleAll == 0 || Head_B->AllBathroomNum == 1){
        source_Bathroom->isClose = 1;
//        freeBathroom(source_Bathroom);
    }
    else {

        if (Head_B->AllBathroomNum == 2) {
            if (source_Bathroom->nextBathroom != NULL) {
                dest_Bathroom = source_Bathroom->nextBathroom;
            } else {
                dest_Bathroom = source_Bathroom->previousBathroom;
            }
        }
        else { dest_Bathroom = source_Bathroom->previousBathroom;}
        assert(dest_Bathroom != NULL);
        while (source_Bathroom->numOfPeopleAll != 0) {

            if (dest_Bathroom->numOfPeopleAll == 0) {

                dest_Bathroom->isClose = -1;
                dest_Bathroom->numOfPeopleAll = source_Bathroom->numOfPeopleAll;
                dest_Bathroom->numOfGroup = source_Bathroom->numOfGroup;
                int ArrLen = source_Bathroom->numOfGroup + 1;
                People *people_temp = NULL;
                int preNext_temp = 0;
                for (int i = 0; i < ArrLen; i++) {
                    people_temp = source_Bathroom->AllHeadTail[i];
                    source_Bathroom->AllHeadTail[i] = source_Bathroom->AllHeadTail[i + source_Bathroom->numOfGroup + 1];
                    source_Bathroom->AllHeadTail[i + source_Bathroom->numOfGroup + 1] = people_temp;

                    preNext_temp = source_Bathroom->PreOrNext[i];
                    source_Bathroom->PreOrNext[i] = source_Bathroom->PreOrNext[i + source_Bathroom->numOfGroup + 1];
                    source_Bathroom->PreOrNext[i + source_Bathroom->numOfGroup + 1] = preNext_temp;
                    source_Bathroom->PreOrNext[i] *= -1;
                    source_Bathroom->PreOrNext[i + source_Bathroom->numOfGroup + 1] *= -1;
                }
                dest_Bathroom->numOfPeopleEachGroup = source_Bathroom->numOfPeopleEachGroup;

                dest_Bathroom->AllHeadTail = source_Bathroom->AllHeadTail;
                dest_Bathroom->PreOrNext = source_Bathroom->PreOrNext;

                source_Bathroom->numOfPeopleAll = 0;
                source_Bathroom->isClose = 1;
            }
            else {
                People *globalOrGroup_dest_tail = NULL;
                People *global_source_tail = source_Bathroom->AllHeadTail[source_Bathroom->numOfGroup * 2 + 1];
                int AllHead_index = source_Bathroom->numOfGroup;
                int AllTail_index = source_Bathroom->numOfGroup * 2 + 1;
                People*s_HEAD = source_Bathroom->AllHeadTail[AllHead_index];
                People*s_TAIL = source_Bathroom->AllHeadTail[AllTail_index];
                People*d_HEAD = dest_Bathroom->AllHeadTail[AllHead_index];
                People*d_TAIL = dest_Bathroom->AllHeadTail[AllTail_index];
                int group_head_index = global_source_tail->group;
                int group_tail_index = global_source_tail->group + source_Bathroom->numOfGroup + 1;
                int source_group_num = source_Bathroom->numOfPeopleEachGroup[group_head_index];
                int dest_group_num = dest_Bathroom->numOfPeopleEachGroup[group_head_index];

                People *dest_temp = NULL;
                People *source_temp = NULL;
                People *source_head = source_Bathroom->AllHeadTail[group_head_index];
                int dest_temp_PN = 0;
                int source_temp_PN = 0;
                int dest_tail_PN = dest_Bathroom->PreOrNext[group_tail_index] != 0 ?
                                   dest_Bathroom->PreOrNext[group_tail_index] : dest_Bathroom->PreOrNext[AllTail_index];
                int source_tail_PN = source_Bathroom->PreOrNext[group_tail_index];
                int source_head_PN = source_Bathroom->PreOrNext[group_head_index];
                globalOrGroup_dest_tail = dest_Bathroom->PreOrNext[group_tail_index] != 0 ?
                                          dest_Bathroom->AllHeadTail[group_tail_index] : dest_Bathroom->AllHeadTail[AllTail_index];

                if (source_group_num == 1) {
                    enter(global_source_tail->group, global_source_tail->label, dest_Bathroom->labelOfBathroom, BathroomARR);
                    leave(source_Bathroom->labelOfBathroom, BathroomARR);

                }
                else {
                    //if (globalOrGroup_dest_tail->super_link != NULL && (globalOrGroup_dest_tail->super_link->group) != globalOrGroup_dest_tail->group )
                    if (dest_tail_PN == 1 && globalOrGroup_dest_tail->next != NULL) {
                        dest_temp = globalOrGroup_dest_tail->next;
                        dest_temp_PN = dest_Bathroom->PreOrNext[dest_temp->group];
                        dest_temp->previous = NULL;
                        globalOrGroup_dest_tail->next = NULL;
                    }
                    else if (dest_tail_PN == -1 && globalOrGroup_dest_tail->previous != NULL) {
                        dest_temp = globalOrGroup_dest_tail->previous;
                        dest_temp_PN = dest_Bathroom->PreOrNext[dest_temp->group];
                        dest_temp->next = NULL;
                        globalOrGroup_dest_tail->previous = NULL;
                    }
                        //globalOrGroup_dest_tail->super_link != NULL 的情況，還有分成SL 在上或是 SL 在下
                    else if (globalOrGroup_dest_tail->super_link != NULL) {
                        dest_temp = globalOrGroup_dest_tail->super_link;
                        dest_temp_PN = dest_Bathroom->PreOrNext[dest_temp->group];
                        dest_temp->super_link = NULL;
                        globalOrGroup_dest_tail->super_link = NULL;
                    }
                    else { dest_temp = NULL;}


//                  以上已經把dest該段開的斷開了，但如果出現單顆的特例，要先處理SL
                    if(dest_Bathroom->numOfPeopleEachGroup[globalOrGroup_dest_tail->group] == 1){
                        if(globalOrGroup_dest_tail->super_link != NULL){
                            People* temp_people = globalOrGroup_dest_tail->super_link;
                            int temp_people_PN = dest_Bathroom->PreOrNext[temp_people->group+dest_Bathroom->numOfGroup+1];
                            temp_people->super_link = NULL;
                            globalOrGroup_dest_tail->super_link = NULL;
                            if(temp_people_PN == 1){
                                temp_people->next = globalOrGroup_dest_tail;
                                globalOrGroup_dest_tail->previous = temp_people;
                            }
                            else{
                                temp_people->previous = globalOrGroup_dest_tail;
                                globalOrGroup_dest_tail->next = temp_people;
                            }
                            dest_Bathroom->PreOrNext[globalOrGroup_dest_tail->group] = temp_people_PN;
                            dest_Bathroom->PreOrNext[globalOrGroup_dest_tail->group+dest_Bathroom->numOfGroup+1] = temp_people_PN;
                            dest_tail_PN*=-1;

                        }
                    }

                    if(globalOrGroup_dest_tail->super_link != NULL && globalOrGroup_dest_tail->super_link->group == globalOrGroup_dest_tail->group){
                        People* temp_people = globalOrGroup_dest_tail->super_link;
                        int temp_people_PN = dest_tail_PN*-1;
                        temp_people->super_link = NULL;
                        globalOrGroup_dest_tail->super_link = NULL;
                        if(temp_people_PN == 1){
                            temp_people->next = globalOrGroup_dest_tail;
                            globalOrGroup_dest_tail->previous = temp_people;
                        }
                        else{
                            temp_people->previous = globalOrGroup_dest_tail;
                            globalOrGroup_dest_tail->next = temp_people;
                        }
                        dest_Bathroom->PreOrNext[globalOrGroup_dest_tail->group+dest_Bathroom->numOfGroup+1] = temp_people_PN;
                        dest_tail_PN*=-1;
                    }


                    if (source_head_PN == 1 && source_head->previous != NULL) {
                        source_temp = source_head->previous;
                        source_temp_PN = source_Bathroom->PreOrNext[source_temp->group + source_Bathroom->numOfGroup + 1];
                        source_temp->next = NULL;
                        source_head->previous = NULL;
                    }
                    else if (source_head_PN == -1 && source_head->next != NULL) {
                        source_temp = source_head->next;
                        source_temp_PN = source_Bathroom->PreOrNext[source_temp->group + source_Bathroom->numOfGroup + 1];
                        source_temp->previous = NULL;
                        source_head->next = NULL;

                    }
                    else if (source_head->super_link != NULL && source_head->super_link->group != source_head->group) {
                        source_temp = source_head->super_link;
                        source_temp_PN = source_Bathroom->PreOrNext[source_temp->group + source_Bathroom->numOfGroup + 1];
                        source_temp->super_link = NULL;
                        source_head->super_link = NULL;
                    }
                    else { source_temp = NULL; }
                    if (source_head->super_link != NULL && (source_head->super_link->group ==source_head -> group)){
                        if (source_head_PN == 1){
                            source_head->super_link->next = source_head;
                            source_head->previous = source_head->super_link;
                            source_head->super_link->super_link = NULL;
                            source_head->super_link = NULL;
                            source_Bathroom->PreOrNext[source_head->group]*= -1;
                            source_head_PN *=-1;
                        }
                        else{
                            source_head->super_link->previous = source_head;
                            source_head->next = source_head->super_link;
                            source_head->super_link->super_link = NULL;
                            source_head->super_link = NULL;
                            source_Bathroom->PreOrNext[source_head->group]*= -1;
                            source_head_PN *=-1;
                        }
                    }
                    if (dest_temp != NULL){
//                        不同組也要處理SL 的問題!
//if (dest_temp->super_link != NULL && dest_temp->super_link->group == dest_temp->group)->幹嘛還要限制同組才處理??? 全部都處理才對???
                        if (dest_temp->super_link != NULL){
                            People*dest_temp_SL = dest_temp->super_link;
                            if (dest_temp_PN == 1){
                                dest_temp->previous = dest_temp_SL;
                                dest_temp_SL->next = dest_temp;

                            }else{
                                dest_temp->next = dest_temp_SL;
                                dest_temp_SL->previous = dest_temp;
                            }
                            dest_temp_SL->super_link = NULL;
                            dest_temp->super_link = NULL;
                            if(dest_temp->group != dest_temp_SL->group){
                                dest_Bathroom->PreOrNext[dest_temp->group+dest_Bathroom->numOfGroup+1] *=-1;
                            }
                            dest_Bathroom->PreOrNext[dest_temp->group]*=-1;
                            dest_temp_PN *=-1;
                        }
                    }
                    if (source_tail_PN == dest_tail_PN) {
                        global_source_tail->super_link = globalOrGroup_dest_tail;
                        globalOrGroup_dest_tail->super_link = global_source_tail;
                    }
                    else {
                        if (dest_tail_PN == 1) {
                            globalOrGroup_dest_tail->next = global_source_tail;
                            global_source_tail->previous = globalOrGroup_dest_tail;
                        } else {
                            globalOrGroup_dest_tail->previous = global_source_tail;
                            global_source_tail->next = globalOrGroup_dest_tail;
                        }
                    }
                    if (dest_temp != NULL) {
                        if (dest_temp_PN == source_head_PN) {
                            dest_temp->super_link = source_head;
                            source_head->super_link = dest_temp;
                        } else {
                            if (dest_temp_PN == 1) {
                                dest_temp->previous = source_head;
                                source_head->next = dest_temp;
                            } else {
                                dest_temp->next = source_head;
                                source_head->previous = dest_temp;
                            }
                        }
                    }
                    else{
                        dest_Bathroom->AllHeadTail[AllTail_index] = source_head;
                        dest_Bathroom->PreOrNext[AllTail_index] = (source_head_PN * -1);
                    }
                    if (dest_Bathroom->numOfPeopleEachGroup[group_head_index] == 0){
                        dest_Bathroom->AllHeadTail[group_head_index] = global_source_tail;
                        dest_Bathroom->PreOrNext[group_head_index] = source_tail_PN*-1;

                        dest_Bathroom->AllHeadTail[group_head_index+dest_Bathroom->numOfGroup+1] = source_head;
                        dest_Bathroom->PreOrNext[group_head_index+dest_Bathroom->numOfGroup+1] = source_head_PN*-1;
                    }

                    int changeNum = source_Bathroom->numOfPeopleEachGroup[global_source_tail->group];
                    source_Bathroom->numOfPeopleEachGroup[global_source_tail->group] = 0;
                    source_Bathroom->numOfPeopleAll -= changeNum;
                    dest_Bathroom->numOfPeopleAll += changeNum;
                    dest_Bathroom->numOfPeopleEachGroup[global_source_tail->group] += changeNum;
                    source_Bathroom->AllHeadTail[AllTail_index] = source_temp;
                    source_Bathroom->PreOrNext[AllTail_index] = source_temp_PN;

                    dest_Bathroom->AllHeadTail[group_tail_index] = source_head;
                    dest_Bathroom->PreOrNext[group_tail_index] = (source_head_PN * -1);

                }


                //每次搬過去，都要處理dest_bathroom雙尾SL_Problem 以及 source_Bathroom tail 的SL 情況
                People *new_dest_head = NULL;
                int new_dest_head_PN = 0;
                People *new_dest_tail = NULL;
                int new_dest_tail_PN = 0;
                if (dest_Bathroom->numOfPeopleAll != 0){
                    int a  = AllHead_index;
                    new_dest_head= dest_Bathroom->AllHeadTail[AllHead_index];
                    new_dest_head_PN= dest_Bathroom->PreOrNext[AllHead_index];
                    new_dest_tail = dest_Bathroom->AllHeadTail[AllTail_index];
                    new_dest_tail_PN= dest_Bathroom->PreOrNext[AllTail_index];
                }
                People *new_source_tail = NULL;
                int new_source_tail_PN = 0;
                if (source_Bathroom->numOfPeopleAll != 0){
                    new_source_tail = source_Bathroom->AllHeadTail[AllTail_index];
                    new_source_tail_PN = source_Bathroom->PreOrNext[AllTail_index];
                }
//              deal with source tail SL_Problem->source 最後是會搬空的，所以source tail可能為NULL
                if (new_source_tail!=NULL){
                    if(new_source_tail->super_link != NULL){
                        People*temp_new_source_tail = new_source_tail->super_link;
                        int temp_new_source_tail_PN = source_Bathroom->PreOrNext[temp_new_source_tail->group+source_Bathroom->numOfGroup+1];
                        temp_new_source_tail->super_link = NULL;
                        new_source_tail->super_link = NULL;
                        if(new_source_tail_PN == 1){
                            temp_new_source_tail->previous = new_source_tail;
                            new_source_tail->next = temp_new_source_tail;
                        }else{
                            temp_new_source_tail->next = new_source_tail;
                            new_source_tail->previous = temp_new_source_tail;
                        }
//                       更新PreNext
                        if(temp_new_source_tail->group != new_source_tail->group){
                            source_Bathroom->PreOrNext[new_source_tail->group] *=-1;
                        }
                        source_Bathroom->PreOrNext[new_source_tail->group+source_Bathroom->numOfGroup+1] *=-1;
                        source_Bathroom->PreOrNext[AllTail_index]*=-1;
                    }
                }
                if (new_dest_head !=NULL){
                    if(new_dest_head->super_link != NULL){
                        People*temp_new_dest_head = new_dest_head->super_link;
                        temp_new_dest_head->super_link = NULL;
                        new_dest_head->super_link = NULL;
                        if(new_dest_head_PN == 1){
                            temp_new_dest_head->next = new_dest_head;
                            new_dest_head->previous = temp_new_dest_head;
                        }else{
                            temp_new_dest_head->previous = new_dest_head;
                            new_dest_head->next = temp_new_dest_head;
                        }
                        //更新dest head PreNext
                        if(temp_new_dest_head->group != new_dest_head->group){
                            dest_Bathroom->PreOrNext[new_dest_head->group+dest_Bathroom->numOfGroup+1] *=-1;
                        }
                        dest_Bathroom->PreOrNext[new_dest_head->group]*=-1;
                        dest_Bathroom->PreOrNext[AllHead_index]*=-1;
                    }

                }
                if (new_dest_tail !=NULL){
                    if(new_dest_tail->super_link != NULL){
                        People*temp_new_dest_tail = new_dest_tail->super_link;
                        temp_new_dest_tail->super_link = NULL;
                        new_dest_tail->super_link = NULL;
                        if(new_dest_tail_PN == 1){
                            temp_new_dest_tail->previous = new_dest_tail;
                            new_dest_tail->next = temp_new_dest_tail;
                        }else{
                            temp_new_dest_tail->next = new_dest_tail;
                            new_dest_tail->previous = temp_new_dest_tail;
                        }
                        //更新dest tail PreNext
                        if(temp_new_dest_tail->group != new_dest_tail->group){
                            dest_Bathroom->PreOrNext[new_dest_tail->group] *=-1;
                        }
                        dest_Bathroom->PreOrNext[new_dest_tail->group+dest_Bathroom->numOfGroup+1] *=-1;
                        dest_Bathroom->PreOrNext[AllTail_index]*=-1;
                    }
                }
            }
        }
//      deal with SL Problem
        People*firstPeople = dest_Bathroom->AllHeadTail[dest_Bathroom->numOfGroup];
        People*SL_secondPeople = NULL;
        if(firstPeople->super_link != NULL){
            SL_secondPeople = firstPeople->super_link;
        }
        People*lastPeople = dest_Bathroom->AllHeadTail[dest_Bathroom->numOfGroup*2+1];
        People*SL_secondLastPeople = NULL;
        if(lastPeople->super_link != NULL){SL_secondLastPeople  = lastPeople->super_link;}
        if (SL_secondPeople != NULL){
            int firstPeople_PN = dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup];
            if (firstPeople_PN == 1){
                firstPeople->super_link = NULL;
                SL_secondPeople->super_link = NULL;
                SL_secondPeople->next = firstPeople;
                firstPeople->previous = SL_secondPeople;

                dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup] = -1;
                dest_Bathroom->PreOrNext[firstPeople->group] = -1;
            }
            else{
                firstPeople->super_link = NULL;
                SL_secondPeople->super_link = NULL;
                SL_secondPeople->previous = firstPeople;
                firstPeople->next = SL_secondPeople;

                dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup] = 1;
                dest_Bathroom->PreOrNext[firstPeople->group] = 1;
            }
            if(firstPeople->group != SL_secondPeople->group){
                dest_Bathroom->PreOrNext[firstPeople->group+dest_Bathroom->numOfGroup+1] =
                        dest_Bathroom->PreOrNext[firstPeople->group];
            }
        }
        if (SL_secondLastPeople != NULL){
            int lastPeople_PN = dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup*2+1];
            if(lastPeople_PN == 1){
                lastPeople->super_link = NULL;
                SL_secondLastPeople->super_link = NULL;
                SL_secondLastPeople->previous = lastPeople;
                lastPeople->next = SL_secondLastPeople;
                dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup*2+1] = -1;
                dest_Bathroom->PreOrNext[lastPeople->group+dest_Bathroom->numOfGroup+1] = -1;
            }
            else{
                lastPeople->super_link = NULL;
                SL_secondLastPeople->super_link = NULL;
                SL_secondLastPeople->next = lastPeople;
                lastPeople->previous = SL_secondLastPeople;

                dest_Bathroom->PreOrNext[dest_Bathroom->numOfGroup*2+1] = 1;
                dest_Bathroom->PreOrNext[lastPeople->group+dest_Bathroom->numOfGroup+1] = 1;
            }
            if (lastPeople->group != SL_secondLastPeople->group){
                dest_Bathroom->PreOrNext[lastPeople->group] = dest_Bathroom->PreOrNext[lastPeople->group+dest_Bathroom->numOfGroup+1];
            }
        }
    }
    if (BathroomARR[0]->AllBathroomNum > 3){
        Bathroom*pre_B = source_Bathroom->previousBathroom;
        Bathroom*next_B = source_Bathroom->nextBathroom;
        pre_B->nextBathroom = next_B;
        next_B->previousBathroom = pre_B;
        source_Bathroom->previousBathroom = NULL;
        source_Bathroom->nextBathroom = NULL;
    }
    else{
        if (BathroomARR[0]->AllBathroomNum == 3){
            Bathroom*pre_B = source_Bathroom->previousBathroom;
            Bathroom*next_B = source_Bathroom->nextBathroom;
            pre_B->nextBathroom = NULL;
            next_B->previousBathroom = NULL;
            source_Bathroom->previousBathroom = NULL;
            source_Bathroom->nextBathroom = NULL;
        }
        else if (BathroomARR[0]->AllBathroomNum == 2){
            if (source_Bathroom->previousBathroom != NULL){
                Bathroom*pre_B = source_Bathroom->previousBathroom;
                pre_B->nextBathroom = NULL;
                source_Bathroom->previousBathroom = NULL;
            } else {
                Bathroom*next_B = source_Bathroom->nextBathroom;
                next_B->previousBathroom = NULL;
                source_Bathroom->nextBathroom = NULL;
            }
        }
        else{
            source_Bathroom->isClose = 1;
        }
    }
    source_Bathroom->isClose = 1;
    BathroomARR[0]->AllBathroomNum-=1;
//    freeBathroom(source_Bathroom);
}

void showNumOfpeople(int*arr,int length){
    for (int i = 0; i < length; ++i) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}
void showInformation(int*preNext,People** peopleArr,int numOfGroupPlus1){

    printf("\n==================People==================\n");
    for (int j = 0; j < numOfGroupPlus1 ; ++j) {
        if (peopleArr[j] != NULL){
            printf("%d ",peopleArr[j]->label);
        } else{printf("- ");}

        if (j == numOfGroupPlus1 / 2 - 1){
            printf("\n");
        }
    }
    printf("\n==================PreOrNext==================\n");

    for (int i = 0; i < numOfGroupPlus1; ++i) {
        printf("%d ",preNext[i]);
        if (i == numOfGroupPlus1 / 2 - 1){
            printf("\n");
        }
    }


    printf("\n===========================================\n");

}
void showAllLine(Bathroom**BathroomArr,int numOfBathroom,int numOfGroup) {
    for (int i = 0; i < numOfBathroom; i++) {
//        printf("Bathroom[%d] : ",i);
        if (BathroomArr[i]->isClose == 1 || BathroomArr[i]->numOfPeopleAll == 0) {
//            if (BathroomArr[i]->isClose == 1){
//                printf("closed!\n");
//            }
//            else{
//
//            }
            if (i != numOfBathroom - 1) {
//                printf(" \n");
                printf("\n");
            } else {
//                printf(" ");
                printf("");
            }

        }
        else {

            People *p = BathroomArr[i]->AllHeadTail[numOfGroup];
            int counter = 0;
            int isNext = BathroomArr[i]->PreOrNext[numOfGroup];
            while (counter != (BathroomArr[i]->numOfPeopleAll - 1)) {
                while (p->next != NULL && isNext == 1) {

                    printf("%d ", p->label);
                    p = p->next;
                    counter += 1;
                }
                if (counter != BathroomArr[i]->numOfPeopleAll - 1 && p->next == NULL && isNext == 1) {
                    printf("%d ", p->label);
                    p = p->super_link;
                    isNext = -1;
                    counter += 1;
                }
                while (p->previous != NULL && isNext == -1) {
                    printf("%d ", p->label);
                    p = p->previous;
                    counter += 1;
                }
                if (counter != BathroomArr[i]->numOfPeopleAll - 1 && p->previous == NULL && isNext == -1) {
                    printf("%d ", p->label);
                    p = p->super_link;
                    isNext = 1;
                    counter += 1;
                }
            }
            if (i != numOfBathroom - 1) {
                printf("%d\n", p->label);
            }
            else {
                printf("%d", p->label);
            }
        }
    }
}
void writeALLLine(Bathroom**BathroomArr,int numOfBathroom,int numOfGroup,FILE*fp){
    for (int i = 0; i < numOfBathroom; i++) {
        if (BathroomArr[i]->isClose == 1 || BathroomArr[i]->numOfPeopleAll == 0) {
            if (i != numOfBathroom - 1) {
                fprintf(fp,"%s"," \n");
            } else {
                fprintf(fp,"%s"," ");
            }
        } else {
            People *p = BathroomArr[i]->AllHeadTail[numOfGroup];
            int counter = 0;
            int isNext = BathroomArr[i]->PreOrNext[numOfGroup];
            while (counter != (BathroomArr[i]->numOfPeopleAll - 1)) {
                while (p->next != NULL && isNext == 1) {

                    fprintf(fp,"%d ",p->label);
//                    printf("%d ", p->label);
                    p = p->next;
                    counter += 1;
                }
                if (counter != BathroomArr[i]->numOfPeopleAll - 1 && p->next == NULL && isNext == 1) {
                    fprintf(fp,"%d ",p->label);
//                    printf("%d ", p->label);
                    p = p->super_link;
                    isNext = -1;
                    counter += 1;
                }
                while (p->previous != NULL && isNext == -1) {
                    fprintf(fp,"%d ",p->label);
//                    printf("%d ", p->label);
//                    printf("%d ", p->label);
                    p = p->previous;
                    counter += 1;
                }
                if (counter != BathroomArr[i]->numOfPeopleAll - 1 && p->previous == NULL && isNext == -1) {
                    fprintf(fp,"%d ",p->label);
//                    printf("%d ", p->label);
                    p = p->super_link;
                    isNext = 1;
                    counter += 1;
                }
            }
            if (i != numOfBathroom - 1) {
                fprintf(fp,"%d\n",p->label);
//                printf("%d\n", p->label);
            }
            else {
                fprintf(fp,"%d",p->label);
//                printf("%d", p->label);
            }
        }
    }
}
int main() {
    char input[LEN];
    int numBathroom;
    int numCommandLine;
    int numGroup;
    fgets(input,LEN,stdin);
    sscanf(input,"%d %d %d",&(numBathroom),&(numCommandLine),&(numGroup));
    Bathroom* BathroomArr[numBathroom];
    Bathroom* Head = NULL;
    Head = createCircularBathroom(numBathroom,numGroup,&(BathroomArr[0]));
    Head->AllBathroomNum = numBathroom;
    char command[100] = {'\0'};
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    for (int i = 0 ;i<numCommandLine;i++){
        fgets(input,LEN,stdin);
        if (input[0] == 'e'){
            sscanf(input,"%s %d %d %d",command,&(num1),&(num2),&(num3));
            enter(num1,num2,num3,BathroomArr);
        }
        else if (input[0] == 'g'){
            sscanf(input,"%s %d",command,&(num1));
            go(num1,BathroomArr);
        }
        else if (input[0] == 'l'){
            sscanf(input,"%s %d",command,&(num1));
            leave(num1,BathroomArr);
        }
        else{
            sscanf(input,"%s %d",command,&(num1));
            close(num1,BathroomArr);
        }
    }
//    showInformation(BathroomArr[6]->PreOrNext,BathroomArr[6]->AllHeadTail,(numGroup+1)*2);
    showAllLine(BathroomArr,numBathroom,numGroup);

//    ====================================================================================================





//    ====================================================================================================
//    char input[LEN];
//    int numBathroom;
//    int numCommandLine;
//    int numGroup;
//#if unit_test == 0
//    for (int d = 1; d <7 ; ++d) {
//        printf("%d\n",d);
//        for (int j = 0; j < 60000; ++j) {
//
//            char fileLabel[10];
//            int numOfDir = d;
//            char fileroot[40];
//            sprintf(fileroot, "../sample%d/sample", numOfDir);
//            sprintf(fileLabel, "%d.txt", j);
//            strcat(fileroot, fileLabel);
//
//            char ansRoot[40];
//            sprintf(ansRoot, "../Ans%d/ans", numOfDir);
//            char ansLabel[10];
//            sprintf(ansLabel, "%d.txt", j);
//            strcat(ansRoot, ansLabel);
//            FILE *fp = fopen(fileroot, "r");
//            assert(fp != NULL);
//            fgets(input, 100, fp);
//            sscanf(input, "%d %d %d", &(numBathroom), &(numCommandLine), &(numGroup));
//            Bathroom *BathroomArr[numBathroom];
//            Bathroom *Head = NULL;
//            Head = createCircularBathroom(numBathroom, numGroup, &(BathroomArr[0]));
//            Head->AllBathroomNum = numBathroom;
//            char command[10] = {'\0'};
//            int num1 = 0;
//            int num2 = 0;
//            int num3 = 0;
//            for (int i = 0; i < numCommandLine; i++) {
//                fgets(input, 100, fp);
//                if (input[0] == 'e') {
//                    sscanf(input, "%s %d %d %d", command, &(num1), &(num2), &(num3));
//                    enter(num1, num2, num3, BathroomArr);
//                } else if (input[0] == 'g') {
//                    sscanf(input, "%s %d", command, &(num1));
//                    go(num1, BathroomArr);
//                } else if (input[0] == 'l') {
//                    sscanf(input, "%s %d", command, &(num1));
//                    leave(num1, BathroomArr);
//                } else {
//                    sscanf(input, "%s %d", command, &(num1));
//                    close(num1, BathroomArr);
//                }
//            }
////        showAllLine(BathroomArr, numBathroom, numGroup);
//
//            FILE *ans_fp = fopen(ansRoot, "w+");
//            assert(ans_fp != NULL);
//            writeALLLine(BathroomArr, numBathroom, numGroup, ans_fp);
//            printf("=============================sample %d finish!=========================================\n", j);
//            fclose(ans_fp);
//            fclose(fp);
//        }
//    }
//#endif


#if unit_test == 1
    char fileLabel[10] ;
//    char fileroot[40] = "../sample2/sample";
    char fileroot[40] = "../bug_sample/sample";
//    char fileroot[40] = "../sample";
    int j = 54820;
    sprintf(fileLabel,"%d",j);
    strcat(fileroot,fileLabel);
    strcat(fileroot,".txt");
//        printf(fileroot);
//        exit(0);
    FILE *fp = fopen(fileroot, "r");
    assert(fp != NULL);
    fgets(input, 50, fp);
    sscanf(input, "%d %d %d", &(numBathroom), &(numCommandLine), &(numGroup));
    Bathroom *BathroomArr[numBathroom];
    Bathroom *Head = NULL;
    Head = createCircularBathroom(numBathroom, numGroup, &(BathroomArr[0]));
    Head->AllBathroomNum = numBathroom;
    char command[10] = {'\0'};
    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    for (int i = 0; i < numCommandLine; i++) {
        fgets(input, 50, fp);
        if (input[0] == 'e') {
            sscanf(input, "%s %d %d %d", command, &(num1), &(num2), &(num3));
            enter(num1, num2, num3, BathroomArr);
            printf("\n======================= enter %d %d %d =========================\n",num1,num2,num3);
        } else if (input[0] == 'g') {
            sscanf(input, "%s %d", command, &(num1));
            go(num1, BathroomArr);
//            if(num1 == 10){
//                Bathroom*  b=BathroomArr[10];
//                People*p = b->AllHeadTail[6];
//                printf("here!\n");
//            }
            printf("\n======================= go %d =========================\n",num1);

        } else if (input[0] == 'l') {
            sscanf(input, "%s %d", command, &(num1));
            leave(num1, BathroomArr);
            printf("\n======================= leave %d =========================\n",num1);
        } else {
            sscanf(input, "%s %d", command, &(num1));
//            Bathroom*  b=BathroomArr[10];
//            People*p = b->AllHeadTail[6];
//            Bathroom*  b11=BathroomArr[11];
//            People*p11 = b11->AllHeadTail[6];
            close(num1, BathroomArr);
//            if(num1 == 11){
//                Bathroom*  b=BathroomArr[10];
//                People*p = b->AllHeadTail[6];
//
//            }
            printf("\n======================= close %d =========================\n",num1);
        }
//        showAllLine(BathroomArr,numBathroom,numGroup);

    }

//    Bathroom*  b=BathroomArr[10];
//    People*p = b->AllHeadTail[numGroup];
//    showAllLine(BathroomArr, numBathroom, numGroup);

//    printf("\n=============================sample %d finish!=========================================\n",j);

    FILE *ans_fp = fopen("../ans54819.txt", "w+");
    assert(ans_fp != NULL);
    writeALLLine(BathroomArr, numBathroom, numGroup, ans_fp);
    fclose(ans_fp);
    fclose(fp);
#endif
    return 0;
}





