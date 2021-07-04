#include <stdio.h>
#include <stdlib.h>
#define vn 10
struct node;
typedef struct node* PNode;
struct node{
    int info;
    PNode next;
};
typedef struct node node;
struct stack{
    PNode top;
};
typedef struct stack* Pstack;
typedef struct stack stack;
Pstack createEmptyStack(){
    Pstack pastack=(Pstack)malloc(sizeof(stack));
    pastack->top=NULL;
    return pastack;
}
int isEmptyStack(Pstack pastack){
    return pastack->top==NULL;
}
void push_stack(Pstack pastack,int x){
    PNode new_node=(PNode)malloc(sizeof(node));
    new_node->info=x;
    new_node->next=pastack->top;
    pastack->top=new_node;
}
int pop_stack(Pstack pastack){
    if(isEmptyStack(pastack)) return-1;
    PNode p=pastack->top;
    pastack->top=pastack->top->next;
    free(p);
    return 1;
}
int peek_stack(Pstack pastack){
    return pastack->top->info;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Queue{
    int front,rear;
    int* element;
    int m;
};
typedef struct Queue* PQueue;
typedef struct Queue Queue;
PQueue createEmptyQueue(int m){
    PQueue paqueue=(PQueue)malloc(sizeof(Queue));
    paqueue->m=m;
    paqueue->front=0;
    paqueue->rear=0;
    paqueue->element=(int*)malloc(sizeof(int)*m);
    return paqueue;
}
int isEmptyQueue(PQueue paqueue){
    return (paqueue->front==paqueue->rear);
}
int isFullQueue(PQueue paqueue){
    int result=(paqueue->rear+1)%(paqueue->m);
    return result==paqueue->front;
}
void pop_Queue(PQueue paqueue){
    if(isEmptyQueue(paqueue)) return;
    paqueue->front=(paqueue->front+1)%(paqueue->m);
}
void push_Queue(PQueue paqueue,int x){
    if(isFullQueue(paqueue))
    {
        printf("out of space!\n");
        return;
    }
    paqueue->element[paqueue->rear]=x;
    paqueue->rear=(paqueue->rear+1)%(paqueue->m);
}
int peek_Queue(PQueue paqueue){
    if(isEmptyQueue(paqueue)) return -1;
    return paqueue->element[paqueue->front];
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct edgeNode;
typedef struct edgeNode* PedgeNode;
typedef struct edgeNode* edgeList;
struct edgeNode{
    int link_index;
    float weight;
    PedgeNode next;
};
typedef struct{
    char vertex;
    int mask;
    edgeList edgelist;
}VexNode;
typedef struct{
    int vexNum,arcsNum;
    VexNode vexs[vn];
}GraphList;
int search_index(GraphList*G, char a){
    for(int i=0;i<G->vexNum;i++){
        if(G->vexs[i].vertex==a) return i;
    }
    return -1;
}
void add_edge(GraphList*G,char a,char b,float weight){
    int start=search_index(G,a);
    PedgeNode p=G->vexs[start].edgelist;
    while(p!=NULL && p->next!=NULL) p=p->next;
    PedgeNode new_edge=(PedgeNode)malloc(sizeof(struct edgeNode));
    new_edge->link_index=search_index(G,b);
    new_edge->weight=weight;
    if(p!=NULL){
        new_edge->next=p->next;
        p->next=new_edge;
    }
    else{
        new_edge->next=p;
        p=new_edge;
    }
}
GraphList* createDGL(){
    GraphList* G=(GraphList*)malloc(sizeof(GraphList));
    printf("please input the vexNum and arcsNum\n");
    int inputNum1,inputNum2;
    scanf("%d %d",&G->vexNum,&G->arcsNum);
    getchar();
    printf("please input the nodes respectively\n");
    int i;
    for(i=0;i<G->vexNum;i++){
        scanf("%c",&G->vexs[i].vertex);
        getchar();
        G->vexs[i].mask=0;
        G->vexs[i].edgelist=NULL;
    }
    printf("please input the edges respectively\n");
    char startNode,endNode;
    float weight;
    for(i=0;i<G->arcsNum;i++){
        scanf("%c %c %d",&startNode,&endNode,&weight);
        getchar();
        add_edge(G,startNode,endNode,weight);
    }
    return G;
}
int firstAdjacent(GraphList* G, char x){
    int i=search_index(G,x);
    PedgeNode p=G->vexs[i].edgelist;
    if(p==NULL) return NULL;
    else return p->link_index;
}
int nextAdjacent(GraphList*G,char a,char b){
    int i=search_index(G,a);
    int j=search_index(G,b);
    PedgeNode p=G->vexs[i].edgelist;
    while(p!=NULL){
        if(p->link_index==j && p->next!=NULL) return p->next->link_index;
        p=p->next;
    }
    return NULL;
}
char firstVertex(GraphList* G){
    return G->vexs[0].vertex;
}
char nextVertex(GraphList* G,char v){
    for(int i=0;i<G->vexNum;i++){
        if(G->vexs[i].vertex=v) return G->vexs[i+1].vertex;
    }
    return NULL;
}
void dfs_digui(GraphList* G,char x){
    int i=search_index(G,x);
    printf("%c",G->vexs[i].vertex);
    G->vexs[i].mask=1;
    int j=firstAdjacent(G,x);
    while(j){
        dfs_digui(G,G->vexs[j].vertex);
        j=nextAdjacent(G,x,G->vexs[j].vertex);
    }
}
void dfs(GraphList*G ,char x){
    Pstack pastack=createEmptyStack();
    int i=search_index(G,x);
    printf("%c",G->vexs[i].vertex);
    G->vexs[i].mask=1;
    push_stack(pastack,i);
    int w=firstAdjacent(G,G->vexs[i].vertex);
    while(!isEmptyStack(pastack)){
        if(w==NULL){
            pop_stack(pastack);
            w=nextAdjacent(G,G->vexs[peek_stack(pastack)].vertex,w);
            continue;
        }
        if(G->vexs[w].mask==0){
            printf("%c",G->vexs[w].vertex);
            push_stack(pastack,w);
            G->vexs[w].mask==1;
            w=firstAdjacent(G,G->vexs[w].vertex);
        }
        else{
            while(w){//在无向图中有多少次能等于1，由于一条边存两次，所以执行次数为e
                if(G->vexs[w].mask!=0) w=nextAdjacent(G,G->vexs[peek_stack(pastack)].vertex,w);
                else break;
            }
        }
    }//由于不知道n和e谁大谁小，所以算法复杂度为O(n+e)，而邻接矩阵若全为0/无穷，那么算法复杂度为o(n*n)
}
void bfs(GraphList* G,char x){
    PQueue paqueue=createEmptyQueue(G->vexNum);
    int start=search_index(G,x);
    printf("%c",G->vexs[start].vertex);
    G->vexs[start].mask=1;
    push_Queue(paqueue,start);
    int j,k;
    while(!isEmptyQueue(paqueue)){//每个节点入队出队一次
        j=peek_Queue(paqueue);
        k=firstAdjacent(G,G->vexs[j].vertex);
        while(k){
            if(G->vexs[k].mask==0){
                printf("%c",G->vexs[k].vertex);
                G->vexs[k].mask=1;
                push_Queue(paqueue,k);
            }
            k=nextAdjacent(G,j,k);//共2e次，邻接矩阵n次
        }
        pop_Queue(paqueue);//共n次
    }
}//所以广度优先遍历邻接表为O(n+e)，邻接矩阵o(n*n)
void dft(GraphList* G){
    char s=firstVertex(G);
    int i;
    while(s){
        i=search_index(G,s);
        if(G->vexs[i].mask==0) dfs(G,s);//每次遍历可以产生一颗生成树
        s=nextVertex(G,s);
    }
}
void main(){
    GraphList* G=createDGL();
}

