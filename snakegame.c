#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "getch.c"
#include <time.h>
#define max_n 100
pthread_t painter,controller;
struct {
        char map[max_n][max_n];
        int r[max_n],c[max_n],pre[max_n],l,rows,colums,hi,ri,speed,dir;
}gs;
int randomex(int m){
        return rand()%m;
}
int congratulation(){
        if(randomex(100)<3){
                int r,c,num;
                do{
                        num=randomex(100);
                        c=num%gs.colums;
                        r=num%gs.rows;
                }while(gs.map[r][c]=='*');
                gs.map[r][c]='-';
        }
}
void turn(){
        int hi=gs.hi , ri=gs.ri , hr=gs.r[hi] , hc=gs.c[hi] , rr=gs.r[ri] , rc=gs.c[ri],nr=hr,nc=hc;
        if(gs.dir==0){ 
                nr--;
        }else if(gs.dir==1){
                nr++;
        }else if(gs.dir==2){
                nc--;
        }else if(gs.dir==3){
                nc++;
        }else{
                return;
        }
        if((nr>=0 && nr<gs.rows && nc>=0 && nc<gs.colums) && (gs.map[nr][nc]!='*')){
                if(gs.map[nr][nc]=='-'){
                        gs.r[gs.l]=nr;
                        gs.c[gs.l]=nc;
                        gs.pre[hi]=gs.l;
                        gs.hi=gs.l++;
                }else{
                        gs.map[nr][nc]='*';
                        gs.map[rr][rc]=' ';
                        gs.r[ri]=nr;
                        gs.c[ri]=nc;
                        gs.pre[hi]=ri;
                        gs.hi=ri;
                        gs.ri=gs.pre[ri];
                }
       }else{
                printf("------------------------gameover!---------------------------\n");
                pthread_exit(NULL);
                pthread_kill(controller,NULL);
       }
}
void* draw(void *p){
        int i,j;
        while(10){
                system("clear");
                for(j=0;j<gs.colums+2;j++){
                        printf("-");
                }
                printf("\n");
                for(i=0;i<gs.rows;i++){
                        printf("|");
                        for(j=0;j<gs.colums;j++){
                                printf("%c",gs.map[i][j]);
                        }
                        printf("|\n");
                }
                for(j=0;j<gs.colums+2;j++){
                        printf("-");
                }
                printf("\nrank:%d\n",gs.l);
                system("date");
                usleep(gs.speed*100000);
                turn();
                congratulation();
        }
}
void init(){
        int i;
        gs.hi=gs.ri=gs.dir=gs.l=0;
        gs.speed=1;
        gs.rows=gs.colums=30;
        gs.c[gs.l]=gs.r[gs.l]=10;
        gs.pre[gs.hi]=gs.pre[gs.ri]=gs.l++;
        for(i=0;i<gs.rows*gs.colums;i++){
                gs.map[i/gs.colums][i%gs.colums]=' ';
        }
        for(i=0;i<gs.l;i++){
                gs.map[gs.r[i]][gs.c[i]]='*';
        }
}
void *control(void * p){
        char ch=' ';
        do{
                ch=getch();
                if(ch=='w' && gs.dir!=1){
                        gs.dir=0;
                }else if(ch=='s' && gs.dir!=0){
                        gs.dir=1;
                }else if(ch=='a' && gs.dir!=3){
                        gs.dir=2;
                }else if(ch=='d' && gs.dir!=2){
                        gs.dir=3;
                }
        }while(ch!='q');
        exit(0);
}
int main(){
        init();
        srand(100);
        pthread_create(&painter,NULL,(void*)draw,NULL);
        pthread_create(&controller,NULL,(void*)control,NULL);
        pthread_join(painter,NULL);
        return 0;
}
