#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
#include <stdlib.h>
#define ll long long
#define mtwister 0
//============================================================================================================
#define UPPER_MASK		0x80000000
#define LOWER_MASK		0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000
#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef struct tagMTRand {
    unsigned long mt[STATE_VECTOR_LENGTH];
    int index;
} MTRand;

MTRand seedRand(unsigned long seed);
unsigned long genRandLong(MTRand* rand);
double genRand(MTRand* rand);


inline static void m_seedRand(MTRand* rand, unsigned long seed) {
    /* set initial seeds to mt[STATE_VECTOR_LENGTH] using the generator
     * from Line 25 of Table 1 in: Donald Knuth, "The Art of Computer
     * Programming," Vol. 2 (2nd Ed.) pp.102.
     */
    rand->mt[0] = seed & 0xffffffff;
    for(rand->index=1; rand->index<STATE_VECTOR_LENGTH; rand->index++) {
        rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffffffff;
    }
}

/**
* Creates a new random number generator from a given seed.
*/
MTRand seedRand(unsigned long seed) {
    MTRand rand;
    m_seedRand(&rand, seed);
    return rand;
}

/**
 * Generates a pseudo-randomly generated long.
 */
unsigned long genRandLong(MTRand* rand) {

    unsigned long y;
    static unsigned long mag[2] = {0x0, 0x9908b0df}; /* mag[x] = x * 0x9908b0df for x = 0,1 */
    if(rand->index >= STATE_VECTOR_LENGTH || rand->index < 0) {
        /* generate STATE_VECTOR_LENGTH words at a time */
        int kk;
        if(rand->index >= STATE_VECTOR_LENGTH+1 || rand->index < 0) {
            m_seedRand(rand, 4357);
        }
        for(kk=0; kk<STATE_VECTOR_LENGTH-STATE_VECTOR_M; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for(; kk<STATE_VECTOR_LENGTH-1; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+(STATE_VECTOR_M-STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rand->mt[STATE_VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[STATE_VECTOR_LENGTH-1] = rand->mt[STATE_VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
    y = rand->mt[rand->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_B;
    y ^= (y << 15) & TEMPERING_MASK_C;
    y ^= (y >> 18);
    return y;
}

/**
 * Generates a pseudo-randomly generated double in the range [0..1].
 */
double genRand(MTRand* rand) {
    return((double)genRandLong(rand) / (unsigned long)0xffffffff);
}


typedef struct node{
    ll val, sz, prior,sum, mx,se,cur_max_num,repl_k_tag;
    bool rev;
    struct node * l ,*r, *par;
}Node;
void init_node(Node* node,int val,unsigned long pri){
    node->val = val;
    node->sum = val;
    node->cur_max_num = 1;
    node->mx = val;
    node->se = INT_MIN;
    node->repl_k_tag = 0;
    node->rev = 0;
    node->sz = 1;
    node->prior = pri;
    node->l = NULL;
    node->r = NULL;
    node->par = NULL;
}
ll size(Node *n){
    return n ? n->sz : 0;
}
void update_size(Node ** t){
    if(*t){
        (*t)->sz = size((*t)->l)+size((*t)->r)+1;
    }
}
void update_parent(Node** n){
    if(!(*n)){ return;}
    if((*n)->l){(*n)->l->par = (*n);}
    if((*n)->r){(*n)->r->par = (*n);}
}
void lazy_rev_upd(Node ** n){
    if(!(*n) || !(*n)->rev){ return;}
    (*n)->rev = false;
    Node * temp= NULL;
    temp = (*n)->l;
    (*n)->l = (*n)->r;
    (*n)->r = temp;
    if((*n)->l){(*n)->l->rev^=true;}
    if((*n)->r){(*n)->r->rev^=true;}
}
void reset(Node ** n){
    if(!(*n)){ return;}
    (*n)->sum = (*n)->val;
    (*n)->mx = (*n)->val;
    (*n)->se  = INT_MIN;
}
void combine(Node ** t,Node * l,Node * r){
    if(!l){
        *t = r;
        return;
    }
    if(!r){
        *t = l;
        return;
    }
    (*t)->sum = l->sum+r->sum;
    if(l->mx > r->mx){
        if(l->se > r->mx){
            (*t)->se = l->se;
        }
        else{
            (*t)->se = r->mx;
        }
        (*t)->cur_max_num = l->cur_max_num;
        (*t)->mx = l->mx;
    }
    else if (l->mx < r->mx){
        if(r->se > l->mx){
            (*t)->se = r->se;
        }
        else{
            (*t)->se = l->mx;
        }
        (*t)->cur_max_num = r->cur_max_num;
        (*t)->mx = r->mx;
    }
    else{
        (*t)->mx = l->mx;
        if(l->se > r->se){
            (*t)->se = l->se;
        }
        else{
            (*t)->se = r->se;
        }
        (*t)->cur_max_num = l->cur_max_num + r->cur_max_num;
    }
};
void operation(Node ** t){
    if(!(*t)){ return;}
    reset(t);
    lazy_rev_upd(&((*t)->l));
    lazy_rev_upd(&((*t)->r));
    combine(t,(*t)->l,*t);
    combine(t,*t,(*t)->r);
}
void split(Node * t,Node ** l ,Node ** r,int k ,ll add){
    if(t == NULL){
        *l = NULL;
        *r = NULL;
        return;
    }
    lazy_rev_upd(&(t));
    ll idx = add + size(t->l);
    if(idx <= k){
        split(t->r,&(t->r),r,k,idx+1);
        *l = t;
    }
    else{
        split(t->l,l,&(t->l),k,add);
        *r = t;
    }
    update_parent(&(t));
    update_size(&(t));
    operation(&(t));
}
void merge(Node**t,Node *l,Node * r){
    lazy_rev_upd(&(l));
    lazy_rev_upd(&(r));
    if(!l){
        *t = r;
        return;

    }
    if(!r){
        *t = l;
        return;
    }
    if(l->prior > r->prior){
        merge(&(l->r),l->r,r),*t = l;
    }
    else{
        merge(&(r->l),l,r->l),*t = r;

    }
    update_parent(t);
    update_size(t);
    operation(t);
}
void swap_interval(Node * root,int l,int r,int x,int y){
    if(r > x){
        int temp = 0;
        temp = x;
        x = l;
        l = temp;
        temp = y;
        y = r;
        r = temp;
    }
    Node *left= NULL, *right= NULL, *mid= NULL,*left_block= NULL,*right_block= NULL;
    split(root,&(root),&(left_block),l-1,0);
    split(left_block,&(left_block),&(mid),r-l,0);
    split(mid,&(mid),&(right_block),x-r-2,0);
    split(right_block,&(right_block),&(right),y-x,0);

    merge(&(root),root,right_block);
    merge(&(root),root,mid);
    merge(&(root),root,left_block);
    merge(&(root),root,right);
}
Node * createNode(int val,unsigned long pri){
    Node* new_node = malloc(sizeof(Node));
    init_node(new_node,val,pri);
    return new_node;
}
void insert(Node ** root,int pos,int val,unsigned long pri){
    if(*root == NULL){
        Node* new_node = createNode(val,pri);
        *root = new_node;
        return;
    }
    Node * l = NULL,*r= NULL,*mid= NULL;
    mid = createNode(val,pri);
    split(*root,&(l),&(r),pos-1,0);
    merge(&l,l,mid);
    merge(root,l,r);
}
void erase(Node**root,int qL,int qR){
    Node*l = NULL,*r = NULL,*mid = NULL;
    split(*root,&(l),&(r),qL-1,0);
    split(r,&(mid),&(r),qR-qL,0);
    merge(root,l,r);
}
ll query(Node ** root,int qL,int qR){
    Node* l = NULL,*r = NULL,*mid = NULL;
    split(*root,&(l),&(r),qL-1,0);
    split(r,&(mid),&(r),qR-qL,0);
    ll answer = mid->sum;
    merge(&(r),mid,r);
    merge(root,l,r);
    return answer;
}
void reverse(Node** root,int qL,int qR){
    Node* l = NULL,* r = NULL,* mid = NULL;
    split(*root,&l,&r,qL-1,0);
    split(r,&mid,&r,qR-qL,0);

    mid->rev^=1;
    merge(&(r),mid,r);
    merge(root,l,r);
}

//-----------------------------------------------------------------------------------------------------------
void upd_merge(Node ** t){
    if(!(*t)->l && !(*t)->r ) {
        return;
    }


    if(!(*t)->l) {
        (*t)->sum =(*t)->r->sum;
        (*t)->mx = (*t)->r->mx;
        (*t)->se = (*t)->r->se;
        (*t)->cur_max_num = (*t)->r->cur_max_num;
        return;
    }


    if(!(*t)->r) {
        (*t)->sum =(*t)->l->sum;
        (*t)->mx = (*t)->l->mx;
        (*t)->se = (*t)->l->se;
        (*t)->cur_max_num = (*t)->l->cur_max_num;
        return;
    }


    (*t)->sum = (*t)->l->sum + (*t)->r->sum;



    if ((*t)->l->mx == (*t)->r->mx) {
        (*t)->mx = (*t)->l->mx;
        if((*t)->l->se > (*t)->r->se){
            (*t)->se = (*t)->l->se;
        }
        else{
            (*t)->se = (*t)->r->se;

        };
        (*t)->cur_max_num = (*t)->l->cur_max_num + (*t)->r->cur_max_num;
    }
    else{
        if ((*t)->l->mx > (*t)->r->mx) {
            (*t)->mx = (*t)->l->mx;
            if((*t)->l->se > (*t)->r->mx){
                (*t)->se = (*t)->l->se;
            }
            else{
                (*t)->se = (*t)->r->mx;

            }
            (*t)->cur_max_num = (*t)->l->cur_max_num;
        }
        else{
            (*t)->mx = (*t)->r->mx;
            if((*t)->l->mx > (*t)->r->se){
                (*t)->se = (*t)->l->mx ;
            }
            else{
                (*t)->se = (*t)->r->se;

            }
            (*t)->cur_max_num = (*t)->r->cur_max_num;
        };

    };
}
void push_max(Node ** t,ll v){
    if (!(*t) || v >= (*t)->mx) {
        return;
    }
    (*t)->sum -= (*t)->mx * (*t)->cur_max_num;
    (*t)->mx = v;
    (*t)->sum  += (*t)->mx * (*t)->cur_max_num;
}
void pushdown(Node ** t) {
    if (!(*t) ){return;}
    push_max(&((*t)->l), (*t)->mx);
    push_max(&((*t)->r), (*t)->mx);
}
void update_chmin(ll v,Node ** t){
    if(!(*t)){ return;}
    if((*t)->val > v){
        (*t)->repl_k_tag = 1;
    }
    if (v >= (*t)->mx) {
        return;
    }
    if (v > (*t)->se) {
        push_max(t, v);
        if((*t)->repl_k_tag == 1){
            (*t)->val = v;
            (*t)->repl_k_tag = 0;
        }
        return;
    }
    pushdown(t);
    update_chmin( v, &((*t)->l));
    update_chmin(v, &((*t)->r));
    if((*t)->repl_k_tag == 1){
        (*t)->val = v;
        (*t)->repl_k_tag = 0;
    }
    upd_merge(t);
}
void operation5(Node ** root, int qL, int qR, ll v){
    Node*l = NULL, *r = NULL, *mid = NULL;
    if((*root)->se == (-2147483648)){
        printf("wrong!");
    }

    split(*root, &l, &r, qL - 1,0);
    split(r, &mid, &r, qR - qL,0);
    update_chmin(v,&mid);
    merge(&r, mid, r);
    merge(root, l, r);
}
//-----------------------------------------------------------------------------------------------------------
void inorder(Node * cur,int op,ll depth){
    if(cur == NULL){ return;}
    operation(&(cur));
    if(op == 0){
        inorder(cur->l,op,depth+1);
        ll leaf = 0;
        if(cur->l == NULL && cur->r == NULL){
            leaf = 1;
        }
        if((leaf) && (cur->se != (-2147483648))){
            printf("***");
        }
        printf("%llu_%llu_%llu    ",cur->val,depth,leaf);



        inorder(cur->r,op,depth+1);
    }
    else{
        inorder(cur->l,op,depth+1);
        printf("%llu_%llu ",cur->mx,depth);
        inorder(cur->r,op,depth+1);
    }
}
void print_array(Node * root,int op,int depth){
    inorder(root,op,depth);
    printf("\n");
}


void test(){
    MTRand r = seedRand(1337);
    printf("%lu\n",genRandLong(&(r)));
}

int main(){

#if mtwister == 0
    Node * root = NULL;
    int N,Q,machine,operation,qL,qR,x,y,pos,val;
    scanf("%d %d",&(N),&(Q));
    unsigned long pri;
    MTRand r = seedRand(1337);
    for (int j = 0; j < N; ++j) {
        scanf("%d",&(machine));
        pri = genRandLong(&r);
//        printf("%lu\n",pri);
        insert(&(root),j,machine,pri);
    }
    for (int i = 0; i < Q; ++i) {
        scanf("%d",&(operation));
        if(operation == 6){
            scanf("%d %d",&(qL),&(qR));
            ll ans = query(&root,qL-1,qR-1);
            printf("%llu\n",ans);
        }
        else if(operation == 1){
            scanf("%d %d",&(pos),&(val));
            pri = genRandLong(&r);
//            printf("%lu\n",pri);
            insert(&(root),pos,val,pri);


        }
        else if(operation == 2){
            scanf("%d",&(pos));
            erase(&(root),pos-1,pos-1);
        }
        else if (operation == 3){
            scanf("%d %d",&(qL),&(qR));
            reverse(&(root),qL-1,qR-1);
        }
        else if(operation == 4){
            scanf("%d %d %d %d",&(qL),&(qR),&(x),&(y));
            swap_interval(root,qL-1,qR-1,x-1,y-1);
        }
        else{
            scanf("%d %d %d",&(qL),&(qR),&(val));
            operation5(&(root),qL-1,qR-1,val);
        }

    }


    return 0;
#else
    MTRand r = seedRand(1337);
    for (int i = 0; i < 1000; ++i) {
        printf("%lu\n ",genRandLong(&r));

    }
    return 0;
#endif
}

//6 4
//4 1 2 3 5 6
//4 4 6 1 3
//6 1 3
//6 4 6
//6 3 4

//6 6
//4 1 2 3 5 6
//4 4 6 1 3
//6 1 3
//6 4 6
//6 3 4
//2 1
//6 1 3


//6 9
//4 1 2 3 5 6
//4 4 6 1 3
//6 1 3
//6 4 6
//6 3 4
//2 1
//6 1 3
//1 1 10
//6 1 3
//6 4 6


//6 12
//4 1 2 3 5 6
//4 4 6 1 3
//6 1 3
//6 4 6
//6 3 4
//2 1
//6 1 3
//1 1 10
//6 1 3
//6 4 6
//4 5 5 6 6
//6 5 5
//6 6 6
