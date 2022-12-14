#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct disjointSet{
    int root;
    int numOfElement;
} DisjointSet;

DisjointSet ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {
    int ret = 0;
    int mask = (1<<24)-1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    // TODO: Initialize a set with hash value
    int h = hash(s);
    ds[h].root = h;
    ds[h].numOfElement = 1;
}

inline int static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
    return i;
}

int find_set(const char* s) {
    int i = init(s);
//    int i = hash(s);
//   這邊要再把樹高壓低，每次查找都要順便維護樹高->input 為string 要維護樹高感覺需要多考慮input s 如何找到下個 parent的string 或是需更改input的內容
//   TODO
//    if(ds[i].root != i){
//        ds[i].root = find_set()
//    }
    while(ds[i].root != i){
        i = ds[i].root;
    }
    // TODO: Implement your find algorithm here
    return i;
}

void group(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
//    printf("findSet-> %s: %d %s: %d\n",ra,a,rb,b);
//    printf("elements-> %s: %d %s: %d\n",ra,ds[a].numOfElement,rb,ds[b].numOfElement);
    if(a != b){
        if(ds[a].numOfElement >= ds[b].numOfElement){
//            printf("%s -> %s\n\n",rb,ra);
            ds[b].root = ds[a].root;
            ds[a].numOfElement+=ds[b].numOfElement;
        }
        else{
//            printf("%s -> %s\n\n",ra,rb);
            ds[a].root = ds[b].root;
            ds[b].numOfElement+=ds[a].numOfElement;
        }
    }



    // TODO: Implement your union algorithm here
}

bool same_set(const char*a, const char* b) {
    // TODO: Implement your algorithm here
    return find_set(a) == find_set(b);
}

int main() {
    int N = 0;
    char op[10] = {'\0'};
    char name1[13]= {'\0'};
    char name2[13]= {'\0'};
    scanf("%d",&(N));
    for (int i = 0; i < N; ++i) {
        scanf("%s %s %s",op,name1,name2);
        if(op[0] == 'g'){
            group(name1,name2);
        } else{
            if(same_set(name1,name2)){
                printf("Positive\n");
            }
            else{
                printf("Negative\n");
            }
        }
    }
    return 0;
}

//5
//g a b
//g c d
//g d b
//t a d
//t d c

//5
//g a b
//g c d
//t a b
//t a c
//t c d

