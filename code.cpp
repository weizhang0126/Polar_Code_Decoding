#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//function
float f(float a,float b);
float g(float a,float b,int c);
int *decode(float *llr,int *CBR,int N,int n);
int mod(int a,int b);
int main(){
    int CBR[8]={0,0,0,1,0,1,1,1};
    float llr[8]={4.5,-4.4,2.8,4.2,-7.4,9.0,8.9,-4.4};
    int *u;
    u=decode(llr,CBR,8,3);
    for(int i=0;i<8;i++){
        printf("%d ",u[i]);
    }
}
//SC--->decoding
//odd -> 1 3 5 7 operate function g and function mod
//even ->0 2 4 6 operate function g and function f
int *decode(float *llr,int *CBR,int N,int n){
//the index of 0 is the most unusual for operation
//we need to count the numbers for function mod and function f
//Set A represents the numbers of function mod
//Set B represents the numbers of function f
int A[N]={};
int B[N]={};
for(int i=0;i<N;i++){
    int j=i+1;
    int layer=0;
    //If N=8 ---> there exists 1->1 3->2 5->1 ...
    while(j%2==0){
        j/=2;
        layer++;
    }
    A[i]=layer;
}
B[0]=0;
for(int i=1;i<N;i++){
    int j=i;
    int layer=0;
    //If N=8 ---> 2->1 4->2 6->1 ...
    while(j%2==0){
        j/=2;
        layer++;
    } 
    B[i]=layer;
}   
//Set P[N][n+1] represents the value of llr
//Set C[N][n+1] represents the value of partial sum
double P[N][n+1];
int C[N][n+1];
for(int i=0;i<N;i++){
    P[i][n]=llr[i];
}
int h=N;
for(int j=n-1;j>=0;j--){
    for(int i=0;i<h/2;i++){
        P[i][j]=f(P[i][j+1],P[i+h/2][j+1]);
    }
    h/=2;
}
if(!CBR[0])C[0][0]=0;
else C[0][0]=(int)P[0][0]>0?0:1;
//Subscripts start at 1
for(int k=1;k<N;k++){
if(k%2==1){//odd function g + function mod
if(!CBR[k])C[k][0]=0;//Freeze bits
else{//Information bits
    float temp=g(P[k-1][1],P[k][1],C[k-1][0])>0?0:1;
    C[k][0]=(int)temp;
}
//update the partial sum
int num1=A[k];
for(int j=0;j<num1;j++){
    int num2=pow(2,j);
    for(int i=0;i<num2;i++){
        C[k-i-num2][j+1]=mod(C[k-i-num2][j],C[k-i][j]);
        C[k-i][j+1]=C[k-i][j];
    }
}
}
else{//even function g + function f
//g
int num3=B[k];
int j=num3;
int num4=pow(2,num3);
for(int i=0;i<num4;i++){
    int index=k-num4+i;
    P[k+i][j]=g(P[index][j+1],P[k+i][j+1],C[index][j]);
}
//f
for(int y=j;y>0;y--){
    int num5=pow(2,y-1);
    for(int x=0;x<num5;x++){
        P[k+x][y-1]=f(P[k+x][y],P[k+num5+x][y]);
    }
}
if(!CBR[k])C[k][0]=0;//Freeze bits
else{//Information bits
    float temp=P[k][0]>0?0:1;
    C[k][0]=(int)temp;
}
}
}
//return the results
int *u_d;
u_d=(int *)malloc(sizeof(int)*N);
for(int i=0;i<N;i++){
    u_d[i]=C[i][0];
}
return u_d;
}
//implement of function
float f(float a,float b){
    float c=a*b>0?1:-1;
    c=fmin(fabs(a),fabs(b))*c;
    return c;
}
float g(float a,float b,int c){
    return (1-2*c)*a+b;
}
int mod(int a,int b){
    return a==b?0:1;
}
