#include <stdio.h>
#include<stdbool.h>
#include <string.h>
#include<assert.h>
#include<stdlib.h>
#define ARRLEN  1024





int BigOrNot(char *up,char *low){
    /*左邊大return 1
     * 右邊大return -1
     * 相等 return 0
     * */
    while(*(up) == '0'){up++;}
    while(*(low) == '0'){low++;}
    if (strlen(up) < strlen(low)){return -1;}
    if (strlen(up) > strlen(low)){return 1;}

    for (int i = 0;i<strlen(low);i++){
        if (*(up+i) == *(low+i)){continue;}
        if (*(up+i) > *(low+i)){return 1;}
        return -1;
    }
    return 0;
}
void BigSUB(char *big,char*small,char*arr_ans){

    int len_big =strlen(big);
    int len_small =strlen(small);
    char temp_small[ARRLEN] = {'\0'};
    char temp_big[ARRLEN] = {'\0'};
    for (int i = 0;i<len_big;i++){arr_ans[i] = '0';}
    /*以上完成
     * 1. big small 長度對齊
     * 2. arr_ans[] 長度對齊且內容初始化為'0'
     * */
    if (len_small < len_big){
        for(int i = 0;i<len_big-len_small;i++){
            temp_small[i] = '0';}
        strcat(temp_small,small);
    } else{stpcpy(temp_small,small);}
    for (int i = 0;i<len_big;i++){temp_big[i] =*(big+i);}
    /*以上完成兩長度對其，且順序為我們習慣的
     * EX : bigger is   2312222
     *      smaller is  0002345
     *      arr_ans is  0000000
     * */
    for(int i = 0;i<len_big;i++){
        if(*(temp_big+len_big-1-i) >= *(temp_small+len_big-1-i)){
            *(arr_ans+len_big-1-i) = (*(temp_big+len_big-1-i) - *(temp_small+len_big-1-i))+'0';
        } else{
            int counter = 1;
            while(*(temp_big+len_big-1-i-counter) == '0'){
                temp_big[len_big-1-i-counter] = '9';
                counter++;
            }
            *(temp_big+len_big-1-i-counter) = (*(temp_big+len_big-1-i-counter)-'1')+'0';
            *(arr_ans+len_big-1-i) = ((*(temp_big+len_big-1-i)+10)-*(temp_small+len_big-1-i))+'0';
        }
    }
    /*以上完成對齊的 bigger smaller 相減 且 放入 arr_ans*/
}
char* BigMOD(char*bigger,char*smaller){
    /**還未處理左小右大的情況，應在外面輸入以前就處理好**/
    int len_small =strlen(smaller);
    int totla_length = strlen(bigger);
    char slice_bigger[ARRLEN] = {'\0'};
    for (int i = 0;i<len_small;i++){slice_bigger[i] = bigger[i];}
    char *current_ptr = &(bigger[len_small]);
    char *slice_ptr = &(slice_bigger[len_small]);
    int counter = totla_length-len_small;
    /*current_ptr 紀錄 bigger 目前只到哪裡，是為了等等要給slice_ptr加數字，已經起始在，正要加入的那格
     * slice_ptr 指著目前指到哪，是為了等等要在尾巴加數字，已經起始在正要加入的那格
     * */
    /*如果 BigOrNot(a,b) 回傳1 代表 a>b*/
//    int len_slice_bigger = strlen(slice_bigger);
    /*因為slicebigger 是會變得（隨著加入數字會變），所以每次要做都要紀錄*/
    char *ans_ptr = NULL;
    do{
        while (BigOrNot(slice_bigger,smaller) != 1 && (counter != 0)) {
            *(slice_ptr) = *(current_ptr);
            slice_ptr++;
            current_ptr++;
            counter--;}
        char temp_ans_[ARRLEN] = {'\0'};
        char* slice_p = slice_bigger;
        char* temp_p = temp_ans_;
        while((BigOrNot(slice_p,smaller ) != -1 )){
            BigSUB(slice_p,smaller,temp_p);
            strcpy(slice_p,temp_p);
            memset(temp_p,'\0',ARRLEN);
        }
        ans_ptr = slice_p;
    }while (counter!=0);

    while(*(ans_ptr) =='0'){ans_ptr++;}
    if (strlen(ans_ptr) == 0){return "0";}
    char *ans_ = malloc(sizeof(char) * ARRLEN);
    int i =0;
    while(*(ans_ptr+i) != '\0'){
        ans_[i] = *(ans_ptr+i);
        i++;
    }
    return ans_;
}
void BigGCD(char*bigger,char*smaller,char* arr_){

    if ((BigOrNot(bigger,smaller) == 0) || (BigOrNot(smaller,"0") == 0)){
//        char *ans = malloc(sizeof(char) * ARRLEN);
        int i = 0;
        while(*(bigger+i) != '\0'){
            arr_[i] = *(bigger+i);
            i++;
        }
        return;
    }
    BigGCD(smaller,BigMOD(bigger,smaller),arr_);
}
int main(void) {

    char arr_a[ARRLEN] = {'\0'};
    char arr_b[ARRLEN] = {'\0'};
    while(scanf("%s%s",&(arr_a[0]),&(arr_b[0])) != EOF){
       // if (strlen(buf) == 1){讀到空行則跳出
       //     continue;}

        char *ptr_a = NULL;
        char *ptr_b = NULL;
        ptr_a = arr_a;
        ptr_b = arr_b;

        if (BigOrNot(ptr_a,ptr_b) == -1){
            char *temp_ptr = NULL;
            temp_ptr = ptr_a;
            ptr_a = ptr_b;
            ptr_b = temp_ptr;
        }
        char arr_[ARRLEN] = {'\0'};
        BigGCD(ptr_a,ptr_b,arr_);
        // printf("%s\n",arr_);
        int i = 0;
        while (arr_[i] != '\0'){
            printf("%c",arr_[i]);
            i++;
        }
        printf("\n");
    }
    return 0;
}


