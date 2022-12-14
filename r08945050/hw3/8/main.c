#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#define LEN 10000001


char * shortestPalindrome(const char*charArr,long long length){
    char *p = (char*)malloc((length*2+2)*sizeof(char));
    assert(p!= NULL);
    p[length] = ' ';
    for (int i = 0; i < length; ++i) {
        p[i] = charArr[i];
        p[length+i+1] = charArr[length-i-1];}
    p[length*2+1] ='\0';

    return p;

}

long long * getTable(char*str){
    long long length = strlen(str);
    long long *table = (long long *)calloc(length,sizeof(long long));
    assert(table!=NULL);
    long long id = 0;
    for (int i = 1; i < length; ++i) {
        while (id > 0 && (str[id]!=str[i])){
            id = table[id-1];}
        if(str[id] == str[i]){id++;}
        table[i] =id;
    }
    return table;
}

void showTable(long long *arr,int l){
    for (int i = 0; i < l; ++i) {
        printf("%llu ",arr[i]);
    }
    printf("\n");
}


void showAns(char*str,char* r_str,int flag,long long P_num,long long str_len){
    printf("%llu\n",str_len-P_num);
    if(flag == 0){
        r_str[str_len-P_num] = '\0';
        printf("%s",r_str);
        printf("%s",str);
    }
    else if(flag==1){
        printf("%s",str);
        printf("%s",&(r_str[P_num]));
    }
    else{
        char temp_c = r_str[str_len-P_num];
        r_str[str_len-P_num] = '\0';
        printf("%s",r_str);
        printf("%s\n",str);
        r_str[str_len-P_num] = temp_c;
        printf("%s",str);
        printf("%s",&(r_str[P_num]));
    }
}
char * strrev_(const char*str,int len){
    char* p = (char*)calloc(len+1,sizeof(char));
    assert(p!=NULL);
    len--;
    int c = 0;
    for (int i = len; i >= 0 ; --i) {
        p[c] = str[i];
        c++;
    }
    p[len+1] = '\0';
    return p;
}
int main() {

    char *str = NULL,*r_str= NULL,*first= NULL;
    char * second = NULL;
    str = (char*)malloc(LEN*sizeof(char));
    assert(str!=NULL);
//    str[0] ='\0';

    int counter = 0;
    scanf("%s",str);
    counter = strlen(str);
//    printf("%d\n",sizeof(*str));

//    printf("%d\n",counter);
//    printf("%d\n",sizeof(str));


    if(counter == 0){
        printf("0\n");
    }
    else{
        first = shortestPalindrome(str,counter);
        r_str = strrev_(str,counter);
        second = shortestPalindrome(r_str,counter);
        long long* Table1 = getTable(first),*Table2 = getTable(second);
//        showTable(Table1,strlen(first));
//        showTable(Table2,strlen(first));
        long long table_len = strlen(first);
        long long PalindromeNum;
        if(Table1[table_len -1] > Table2[table_len -1]){
            PalindromeNum = Table1[table_len -1];
            showAns(str,r_str,0,PalindromeNum,counter);
        }
        else if(Table1[table_len -1] < Table2[table_len -1]){
            PalindromeNum = Table2[table_len -1];
            showAns(str,r_str,1,PalindromeNum,counter);
        }
        else{
            PalindromeNum = Table2[table_len -1];
            if(PalindromeNum == counter){
                printf("0\n");
                printf("%s",str);
            } else{
                showAns(str,r_str,-1,PalindromeNum,counter);
            }
        }
        free(str);
        free(r_str);
        free(Table1);
        free(Table2);
    }



    return 0;
}

