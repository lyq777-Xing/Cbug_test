#include<stdio.h>
#include<math.h>
void line(int array[][9],int *p,int n);
void row(int array[][9],int *p,int n);
int judgeNum(int *p);
int SudokuPosition(int x,int y);
void SquireNine(int array[][9],int *p,int x,int y);
void FillBlank(int array[][9],int *p,int x,int y);
int Prepare(int Array[][9]);
 
int main()
{
	int NumAlone[9][9],mArray[9];
	int i,j,m,k;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			scanf("%d",&NumAlone[i][j]);
		}
		mArray[i]=0;
	}
	//line(NumAlone,mArray,1);
	//row(NumAlone,mArray,3);
	//SquireNine(NumAlone,mArray,1,3);
	//FillBlank(NumAlone,mArray,1,3);
	while(Prepare(NumAlone)){
		for(i=0;i<9;i++){
			for(j=0;j<9;j++){
				if(NumAlone[i][j]==0){
					for(m=0;m<9;m++){
						mArray[m]=0;
					}
					line(NumAlone,mArray,i);
					row(NumAlone,mArray,j);
					SquireNine(NumAlone,mArray,i,j);
					FillBlank(NumAlone,mArray,i,j);
				}
			}
		}
	}
	printf("\n\n");
	for(m=0;m<9;m++){
		for(k=0;k<9;k++){
			printf("%d ",NumAlone[m][k]);
		}
		printf("\n");
	}
 
	return 0;
}
 
void line(int array[][9],int *p,int n)
{
	for(int i=0;i<9;i++){
		p[array[n][i]-1] = 1;
	}
}
 
void row(int array[][9],int *p,int n)
{
	for(int i=0;i<9;i++){
		p[array[i][n]-1] = 1;
	}
}
 
void SquireNine(int array[][9],int *p,int x,int y)
{
	int i,j,xy,a,b;
	xy=SudokuPosition(x,y);
	a=xy/10-1;b=xy%10-1;
	for(i=a;i<a+3;i++){
		for(j=b;j<b+3;j++){
			p[array[i][j]-1] = 1;
		}
	}
}
 
int judgeNum(int *p)
{
	int flag=0,k;
	for(int i=0;i<9;i++){
		if(p[i]==0){
			flag++;
			k=i+1;
		}
	}
	if(flag==1){
		return k;
	}else{
		return 0;
	}
}
 
int SudokuPosition(int x,int y)
{
	int i,j,a,b,result;
	for(i=1;i<=7;i+=3){
		for(j=1;j<=7;j+=3){
			a=abs(x-i);
			b=abs(y-j);
			if(pow(a,2) + pow(b,2) <= 2){
				result=i*10+j;
				break;
			}
		}
	}
	return result;
}
 
void FillBlank(int array[][9],int *p,int x,int y)
{
	array[x][y]=judgeNum(p);
}
 
int Prepare(int Array[][9])
{
	int i,j,flag=0;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(Array[i][j]==0){
				flag=1;
			}
		}
	}
	return flag;
}
