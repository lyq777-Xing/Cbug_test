#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<math.h>
#include<time.h>
#include <windows.h>
#include<string.h>
#define MAX 999

struct Node{
	Node *up,*down,*left,*right,*colPtr,*rowPtr;//指向row,col对应的行对象和列对象
	int row_num;//记录行数，行专属(从1开始)
	int col_elemCount;//记录该列元素个数，列专属
};

struct player{//玩家信息结点
	int m ;
	int s ;
	char name[20];
	int level ;
	player* next;
};


int row_size=593;//行数
int col_size = 324;//列数
int result[81];//存放结果行的栈
int index = 0;//栈指针
int sudoku[81] = {0};//存放数独
int time_start = 0;
int time_end = 0;//起始时间

void init(Node* head){

	head->left = head;
	head->right = head;
	head->up = head;
	head->down = head;

	for(int k = 0;k < row_size;k++){//创建行对象(头插)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->up = head;
		newNode->down = head->down;
		newNode->left = newNode;
		newNode->right = newNode;
		newNode->down->up = newNode;
		head->down = newNode;
		newNode->row_num = row_size-k;
		newNode->col_elemCount = 0;//借用，作为标志
	}
}
void init_col(Node* head){
	/************初始化行列对象*******************/
	for(int j = 0;j < col_size;j++){//创建列对象(头插)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->right = head->right;
		newNode->left = head;
		newNode->down = newNode;
		newNode->up = newNode;
		newNode->right->left = newNode;
		head->right = newNode;
		newNode->col_elemCount = 0;//列元素个数初始为0
	}
}

void link(Node* head,int** matrix){
	/***********插入结点*************/
	Node *current_row, *current_col, *current;//当前行对象,当前列对象,当前节点
	current_row = head;
	for(int row = 0;row<row_size;row++){
		current_row = current_row->down;
		current_col = head;
		for(int col = 0;col<col_size;col++){
			current_col = current_col->right;
			if(matrix[row][col] == 0)
				continue;
	
			/*****插入结点，结点的行和列都用尾插法来与行列对象链接*****/
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->colPtr = current_col;//设置节点对应的行和列
			newNode->rowPtr = current_row;
			/**********列尾插************/
			newNode->down = current_col;
			newNode->up = current_col->up;
			newNode->up->down = newNode;
			current_col->up = newNode;//链接当前节点到列双向链尾端
	
			if(current_row->col_elemCount == 0){//行的这个为0,说明该行还没有元素，行双向链表不把行对象包含进来(便于后面覆盖)
				current_row->right = newNode;
	            newNode->left = newNode;
	            newNode->right = newNode;
	            current_row->col_elemCount++;//此为标志，为了不把行对象包含进来
			}
	
			current = current_row->right;
			newNode->left = current->left;
			/**********行尾插************/
	        newNode->right = current;
	        newNode->left->right = newNode;
	        current->left = newNode;//链接当前节点到行双向链尾端
	
	        current_col->col_elemCount++;//该列元素加1
		}
	}	
}

int** create_matrix()//将数独转换为01矩阵
{
    int** matrix = (int**)malloc(row_size*sizeof(int*));//申请二维数组空间

	for(int m=0;m<row_size;m++)
		matrix[m] = (int*)malloc(col_size*sizeof(int));

	for(int r=0;r<row_size;r++)
		for(int c=0;c<col_size;c++)
			matrix[r][c] = 0;//初始化
   
	int i = 0;
    for (int x = 0; x < 81; x++){
		int val = sudoku[x];
        if (val != 0){//有值则插入一行
            matrix[i][x] = 1;
            matrix[i][81 + x/9*9 + val -1] = 1;
            matrix[i][162 + x%9*9 + val -1] = 1;
            matrix[i][243 + (x/9/3*3+x%9/3)*9 +val -1] = 1;
            i++;

            continue;
        }

        for (int j = 0; j < 9; j++){//没值则插入9行（1~9）
            matrix[i][x] = 1;
            matrix[i][81 + x/9*9 + j] = 1;
            matrix[i][162 + x%9*9 +j] = 1;
            matrix[i][243 + (x/9/3*3+x%9/3)*9 +j] = 1;
            i++;
        }
    }

    return matrix;
}

void cover(Node* cRoot){//覆盖
    cRoot->left->right = cRoot->right;
    cRoot->right->left = cRoot->left;//删除该列对象，及该列结点的每行结点
    Node *i, *j;
    i = cRoot->down;
    while (i != cRoot)
    {
        j = i->right;
        while (j != i)
        {
            j->down->up = j->up;
            j->up->down = j->down;
            j->colPtr->col_elemCount--;
            j = j->right;
        }
        i = i->down;
    }
}

void recover(Node* cRoot){//回溯
Node *i, *j;
    i = cRoot->up;
    while (i != cRoot)
    {
        j = i->left;
        while (j != i)
        {
            j->colPtr->col_elemCount++;
            j->down->up = j;
            j->up->down = j;
            j = j->left;
        }
        i = i->up;
    }
    cRoot->right->left = cRoot;
    cRoot->left->right = cRoot;

}

bool search(Node* head){
	if(head->right == head){
		return true;
	}
	Node *cRoot, *c;
    int minSize = MAX;//最少列元素个数
    for(c = head->right; c != head; c = c->right)//先选择列元素最少的列对象（提高效率）
    {
        if (c->col_elemCount < minSize)
        {
            minSize = c->col_elemCount;
            cRoot = c;
            if (minSize == 1)//1是最小
                break;
            if (minSize == 0)//有一列为空,失败
                return false;
        }
    }
    cover(cRoot);
    Node *current_row,*current;
    for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
    {
        result[index]=current_row->rowPtr->row_num;//将该行加入result中(行数)
		index++;

        for (current = current_row->right; current != current_row; current = current->right)
        {
            cover(current->colPtr);
        }
        if (search(head))//递归
            return true;
        for (current = current_row->left; current != current_row; current = current->left)
			recover(current->colPtr);
        index--;//发现该行不符合要求,出栈

    }
    recover(cRoot);
    return false;
}

int* to_sudoku(int** matrix){//01矩阵转化为数独
	int* done = (int*)malloc(81*sizeof(int));
	int temp[162]={0};

	for(int i = 0;i<81;i++){
	
		for(int j = 0;j<162;j++)
			temp[j] = matrix[result[i]-1][j];
		
	    int pos = 0,val = 0;
	
		for(int k = 0;k<81;k++){
			if(temp[k] == 1)
				break;
			pos++;
		}
	
		for(int m = 81;m<162;m++){
			if(temp[m] == 1)
				break;
			val++;
		}
		done[pos]=val%9+1;
	
	}

	return done;
}

void print(int* answer){//打印数独
	printf("┏━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┓\n");
	for(int i = 0;i<81;i++){
		if(answer[i]==0)
			printf("┃  ");
		else
			printf("┃ %d",answer[i]);
		if(i==80){
			printf("┃  ");
			printf("\n");
			printf("┗━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┛\n");
		}
		else if((i+1)%9==0){
				printf("┃  ");
				printf("\n");
				printf("┣━━╋━━╋━━╋━━╋━━╋━━╋━━╋━━╋━━┫\n");
		}
	}
}


bool isRight(int* flag,int x,int y,int val){//判断填入的数是否合法
	for(int i = 0;i<324;i++)
		if(flag[(x-1)*9+(y-1)] == 0 &&
		flag[81+(x-1)*9 + val -1] == 0 &&
		flag[162+(y-1)*9 + val -1] == 0 &&
		flag[243+((x-1)/3*3 + (y-1)/3+1)*9 +val -1] == 0 )
			return true;	
	return false;
}

void create_sudoku(){
	for(int j = 0;j<81;j++)
		sudoku[j] = 0;//初始化
	int flag[324]={0};
	int x,y,val;//行号、列号、值

	int i=0;
	while(i<17){//一个数独中有17个数及以上时有唯一解
		x = rand()%9+1;
		y = rand()%9+1;
		val = rand()%9+1;
		if(isRight(flag,x,y,val)){
			i++;
			sudoku[(x-1)*9+(y-1)]=val;
			flag[(x-1)*9+(y-1)] = 1;
			flag[81+(x-1)*9 + val -1] = 1;
			flag[162+(y-1)*9 + val -1] = 1;
			flag[243+((x-1)/3*3 + (y-1)/3+1)*9 +val -1] = 1;
		}
	}
}

void sudoku_level(int* answer,int count){//难度
	int x,y;//行号、列号
	int num=0;
	srand(time(NULL));
	for(int i = 0;i<81;i++)
		sudoku[i] = answer[i];
	while(num<(81-count)){//挖空
		x = rand()%9+1;
		y = rand()%9+1;
		if(sudoku[(x-1)*9+(y-1)] != 0){
			sudoku[(x-1)*9+(y-1)] = 0;
			num++;
		}
	}
}

bool receiver(int* player_res){//接收玩家答案
	for(int i=0;i<81;i++){
		scanf("%d",&player_res[i]);
		if(!(player_res[i]>=1&&player_res[i]<=9)){//0 表示玩家放弃
			fflush(stdin);
			return false;
		}
	}
	return true;
}

int get_time(){//获得当前时间秒
	time_t t;
	t=time(NULL);
	return t;
}

void ready(){
	print(sudoku);
	printf("你有5秒钟观察时间\n");
	for(int i = 0;i<5;i++){
		printf("●	");
		Sleep(1000);
	}
	printf("\n");
	printf("观察结束，计时开始，请开始作答。（输入除1~9外，视为放弃作答）\n");
	printf("==========================================================================\n");
}

bool judge(int* player_res,int* answer){//判断玩家答案
	for(int i = 0;i<81;i++)
		if(player_res[i] != answer[i])
			return false;
	return true;
}

void record(player info){//记录
	FILE* fp;
	int M = MAX,S = MAX,LEVEL = MAX;
	char NAME[20];
	char remove[100] = {"                                                      "};//用于记录长度固定化，方便更新记录
	//通过这种方法，可以直接在一个文件中更新数据，不必要全篇读—改—写，直接修改一行
	int c = 0;
	if((fp = fopen("record.txt","r+"))==NULL){//文件在cpp同目录下
		printf("文件不存在，保存失败！");//虽然会自动生成文件，but以防万一
		return;
	}
	setbuf(fp,NULL);//设置缓冲区
	rewind(fp);
	c = ftell(fp);//记录当前行的开头指针位置

	while(fscanf(fp,"%s %d:%d %d",NAME,&M,&S,&LEVEL) != EOF){

		if(!strcmp(NAME,info.name) && LEVEL == info.level){//strcmp比较相同返回0
			
			if(info.m<M || (info.m==M&&info.s<S)){//如果是新纪录，则更新
				fseek(fp,c,SEEK_SET);
				fputs(remove,fp);//覆盖旧记录
				fseek(fp,c,SEEK_SET);//回到该记录的开头位置
			    fprintf(fp,"%s %d:%d %d",info.name,info.m,info.s,info.level);//写入文件
				fflush(fp);//清除缓冲区
				return;
			}
			return;//不是新纪录就不插入
		}
		fscanf(fp,"\n");//读取换行
		c = ftell(fp);
	}

	fputs(remove,fp);//先覆盖固定长度的区域
	fseek(fp,c,SEEK_SET);//回到覆盖的区域首部
	fprintf(fp,"%s %d:%d %d",info.name,info.m,info.s,info.level);//在覆盖的区域内插入记录
	fseek(fp,0,SEEK_END);//指向尾部
	fprintf(fp,"\n");//插入换行符
	fclose(fp);
}


player* get_record(int level){//返回玩家记录的单向链表头结点
	FILE* fp;
	int M = MAX,S = MAX,LEVEL = MAX;
	char NAME[20];
	player* head = (player*)malloc(sizeof(player));
	head->next = NULL;
	if((fp = fopen("record.txt","r"))==NULL){
		printf("文件不存在！");
		system("pause");
		exit(1);
	}
	setbuf(fp,NULL);//设置缓冲区
	rewind(fp);
	while(fscanf(fp,"%s %d:%d %d",NAME,&M,&S,&LEVEL) != EOF){
		if(LEVEL == level){
			player* p = (player*)malloc(sizeof(player));//采用链表
			strcpy(p->name,NAME);
			p->m = M;
			p->s = S;
			p->next = head ->next;
			head->next = p;
		}	
	}
	fclose(fp);
	return head;
}

void order(player* head){//单链表排序
	player* p;
	player* q;
	int temp1;
	int temp2;
	char temp3[20];
	for(p = head->next;p != NULL;p = p->next)
		for(q = p->next;q != NULL;q = q->next)
			if(p->m>q->m||(p->m==q->m&&p->s>q->s)){//对换两个结点的内容
				temp1 = p->m;
				temp2 = p->s;
				strcpy(temp3,p->name);
				p->m = q->m;
				p->s = q->s;
				strcpy(p->name,q->name);
				q->m = temp1;
				q->s = temp2;
				strcpy(q->name,temp3);
			}
}

void show(player* easy,player* normal,player* hard){//输出排行
	int no=1;
	player* p1 = easy->next;
	player* p2 = normal->next;
	player* p3 = hard->next;
	printf("==========================================================================\n");
	printf("\t\t   简单\t\t\t  一般\t\t\t   困难\n");
	while(p1 != NULL || p2 != NULL || p3 != NULL){
		printf("NO.%d",no++);
		if(p1 != NULL){
			printf("\t\t%s\t%d:%d\t",p1->name,p1->m,p1->s);
			p1 = p1->next;
		}
		if(p2 != NULL){
		    printf("\t%s\t%d:%d\t",p2->name,p2->m,p2->s);
			p2 = p2->next;
		}
		if(p3 != NULL){
			printf("\t%s\t%d:%d\t",p3->name,p3->m,p3->s);
			p3 = p3->next;
		}
		printf("\n");
	}
}

void re_init(Node* head){//重新初始化行和列对象
	Node* p;
	for(p = head->down;p!=head;p=p->down)
		p->col_elemCount = 0;
	
	if(head->right == head){//如果有解，则列对象会在search中被删除，需要重新链接列对象
		init_col(head);
		return;
		}
	
	for(p = head->right;p!=head;p=p->right){//无解时,则恢复列对象
		p->col_elemCount = 0;
		p->down = p;
		p->up = p;
	}
}

int main(){
	int player_res[81]={0};
	int choice;
	int** matrix;//存放数独的01矩阵
	int* answer;//存放答案
	player* easy;//容易难度排行
	player* normal;//简单难度排行
	player* hard;//困难难度排行
	player info ;//玩家信息
	
	Node* head =(Node*)malloc(sizeof(Node));
	init(head);init_col(head);//初始化行列对象，建立十字双向循环链表
	srand(time(NULL));
	bool flag = false;//有无解
	while(true){
	
		while(!flag){
			flag=true;
	
			create_sudoku();//创建17个数的数独（17个数及以上唯一解）
	
			matrix = create_matrix();//得到该数独的01矩阵
	
			link(head,matrix);//将01矩阵转化为十字双向循环链表（转化为精准覆盖问题）
	
			if(!search(head))//得到答案行（result）
				flag = false;//如果不幸无解，则重新构建数独（经本人测试，无解的概率大概为1/1000）
			index = 0;//初始化result栈
			
			re_init(head);//重新初始化十字链表
		}
	
	flag = false;
	answer = to_sudoku(matrix);//得到答案
	
	
	printf("                               数独\n");
	printf("==========================================================================\n\n");
	printf("1.开始游戏\n");
	printf("2.查看排名\n");
	printf("3.退出\n\n");
	printf("==========================================================================\n");
	printf("请选择:");
	scanf("%d",&choice);
	switch(choice){
	case 1:
			int option;
			printf("玩家名:");
			scanf("%s",&info.name);
			if(strlen(info.name)>20){
				printf("名字太长！\n");
				break;
			}
			printf("请选择游戏难度:  1.简单\t2.一般\t3.困难\n");
			scanf("%d",&option);
			printf("\n");
			switch(option){
			case 1:
				sudoku_level(answer,40);//挖空答案
				ready();
				time_start = get_time();
				if(!receiver(player_res)){
					printf("\n您已放弃作答!\t正确答案为:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 1;
			
				if(judge(player_res,answer)){
					printf("回答正确!\t用时:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("\n回答错误!\t正确答案为:\n\n");
					fflush(stdin);
					print(answer);
				}
				break;
			case 2:
				sudoku_level(answer,35);
				time_start = get_time();
				ready();
				if(!receiver(player_res)){
					printf("\n您已放弃作答!\t正确答案为:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 2;
				if(judge(player_res,answer)){
					printf("回答正确!\t用时:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("回答错误!\t正确答案为:\n\n");
					fflush(stdin);
					print(answer);
				}
				break;
			case 3:
				sudoku_level(answer,30);
				time_start = get_time();
				ready();
				if(!receiver(player_res)){
					printf("\n您已放弃作答!\t正确答案为:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 3;
				if(judge(player_res,answer)){
					printf("回答正确!\t用时:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("回答错误!\t正确答案为:\n\n");
					fflush(stdin);
					print(answer);
				}
				break;
			default:
				printf("no option!\n");
				fflush(stdin);
				break;
			}
			break;
	case 2:
		easy = get_record(1);
		normal = get_record(2);
		hard = get_record(3);
		order(easy);
		order(normal);
		order(hard);
		show(easy,normal,hard);
		break;
	case 3:
		printf("\n拜拜~\n\n");
		exit(0);
	default:
		printf("no option!\n");
		fflush(stdin);
		break;
	}
	system("pause");
	system("cls");
	}
}


