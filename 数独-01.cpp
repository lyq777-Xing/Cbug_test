#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<math.h>
#include<time.h>
#include <windows.h>
#include<string.h>
#define MAX 999

struct Node{
	Node *up,*down,*left,*right,*colPtr,*rowPtr;//ָ��row,col��Ӧ���ж�����ж���
	int row_num;//��¼��������ר��(��1��ʼ)
	int col_elemCount;//��¼����Ԫ�ظ�������ר��
};

struct player{//�����Ϣ���
	int m ;
	int s ;
	char name[20];
	int level ;
	player* next;
};


int row_size=593;//����
int col_size = 324;//����
int result[81];//��Ž���е�ջ
int index = 0;//ջָ��
int sudoku[81] = {0};//�������
int time_start = 0;
int time_end = 0;//��ʼʱ��

void init(Node* head){

	head->left = head;
	head->right = head;
	head->up = head;
	head->down = head;

	for(int k = 0;k < row_size;k++){//�����ж���(ͷ��)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->up = head;
		newNode->down = head->down;
		newNode->left = newNode;
		newNode->right = newNode;
		newNode->down->up = newNode;
		head->down = newNode;
		newNode->row_num = row_size-k;
		newNode->col_elemCount = 0;//���ã���Ϊ��־
	}
}
void init_col(Node* head){
	/************��ʼ�����ж���*******************/
	for(int j = 0;j < col_size;j++){//�����ж���(ͷ��)
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->right = head->right;
		newNode->left = head;
		newNode->down = newNode;
		newNode->up = newNode;
		newNode->right->left = newNode;
		head->right = newNode;
		newNode->col_elemCount = 0;//��Ԫ�ظ�����ʼΪ0
	}
}

void link(Node* head,int** matrix){
	/***********������*************/
	Node *current_row, *current_col, *current;//��ǰ�ж���,��ǰ�ж���,��ǰ�ڵ�
	current_row = head;
	for(int row = 0;row<row_size;row++){
		current_row = current_row->down;
		current_col = head;
		for(int col = 0;col<col_size;col++){
			current_col = current_col->right;
			if(matrix[row][col] == 0)
				continue;
	
			/*****�����㣬�����к��ж���β�巨�������ж�������*****/
			Node* newNode = (Node*)malloc(sizeof(Node));
			newNode->colPtr = current_col;//���ýڵ��Ӧ���к���
			newNode->rowPtr = current_row;
			/**********��β��************/
			newNode->down = current_col;
			newNode->up = current_col->up;
			newNode->up->down = newNode;
			current_col->up = newNode;//���ӵ�ǰ�ڵ㵽��˫����β��
	
			if(current_row->col_elemCount == 0){//�е����Ϊ0,˵�����л�û��Ԫ�أ���˫���������ж����������(���ں��渲��)
				current_row->right = newNode;
	            newNode->left = newNode;
	            newNode->right = newNode;
	            current_row->col_elemCount++;//��Ϊ��־��Ϊ�˲����ж����������
			}
	
			current = current_row->right;
			newNode->left = current->left;
			/**********��β��************/
	        newNode->right = current;
	        newNode->left->right = newNode;
	        current->left = newNode;//���ӵ�ǰ�ڵ㵽��˫����β��
	
	        current_col->col_elemCount++;//����Ԫ�ؼ�1
		}
	}	
}

int** create_matrix()//������ת��Ϊ01����
{
    int** matrix = (int**)malloc(row_size*sizeof(int*));//�����ά����ռ�

	for(int m=0;m<row_size;m++)
		matrix[m] = (int*)malloc(col_size*sizeof(int));

	for(int r=0;r<row_size;r++)
		for(int c=0;c<col_size;c++)
			matrix[r][c] = 0;//��ʼ��
   
	int i = 0;
    for (int x = 0; x < 81; x++){
		int val = sudoku[x];
        if (val != 0){//��ֵ�����һ��
            matrix[i][x] = 1;
            matrix[i][81 + x/9*9 + val -1] = 1;
            matrix[i][162 + x%9*9 + val -1] = 1;
            matrix[i][243 + (x/9/3*3+x%9/3)*9 +val -1] = 1;
            i++;

            continue;
        }

        for (int j = 0; j < 9; j++){//ûֵ�����9�У�1~9��
            matrix[i][x] = 1;
            matrix[i][81 + x/9*9 + j] = 1;
            matrix[i][162 + x%9*9 +j] = 1;
            matrix[i][243 + (x/9/3*3+x%9/3)*9 +j] = 1;
            i++;
        }
    }

    return matrix;
}

void cover(Node* cRoot){//����
    cRoot->left->right = cRoot->right;
    cRoot->right->left = cRoot->left;//ɾ�����ж��󣬼����н���ÿ�н��
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

void recover(Node* cRoot){//����
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
    int minSize = MAX;//������Ԫ�ظ���
    for(c = head->right; c != head; c = c->right)//��ѡ����Ԫ�����ٵ��ж������Ч�ʣ�
    {
        if (c->col_elemCount < minSize)
        {
            minSize = c->col_elemCount;
            cRoot = c;
            if (minSize == 1)//1����С
                break;
            if (minSize == 0)//��һ��Ϊ��,ʧ��
                return false;
        }
    }
    cover(cRoot);
    Node *current_row,*current;
    for (current_row = cRoot->down; current_row != cRoot; current_row = current_row->down)
    {
        result[index]=current_row->rowPtr->row_num;//�����м���result��(����)
		index++;

        for (current = current_row->right; current != current_row; current = current->right)
        {
            cover(current->colPtr);
        }
        if (search(head))//�ݹ�
            return true;
        for (current = current_row->left; current != current_row; current = current->left)
			recover(current->colPtr);
        index--;//���ָ��в�����Ҫ��,��ջ

    }
    recover(cRoot);
    return false;
}

int* to_sudoku(int** matrix){//01����ת��Ϊ����
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

void print(int* answer){//��ӡ����
	printf("�������ש����ש����ש����ש����ש����ש����ש����ש�����\n");
	for(int i = 0;i<81;i++){
		if(answer[i]==0)
			printf("��  ");
		else
			printf("�� %d",answer[i]);
		if(i==80){
			printf("��  ");
			printf("\n");
			printf("�������ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ�����\n");
		}
		else if((i+1)%9==0){
				printf("��  ");
				printf("\n");
				printf("�ǩ����贈���贈���贈���贈���贈���贈���贈���贈����\n");
		}
	}
}


bool isRight(int* flag,int x,int y,int val){//�ж���������Ƿ�Ϸ�
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
		sudoku[j] = 0;//��ʼ��
	int flag[324]={0};
	int x,y,val;//�кš��кš�ֵ

	int i=0;
	while(i<17){//һ����������17����������ʱ��Ψһ��
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

void sudoku_level(int* answer,int count){//�Ѷ�
	int x,y;//�кš��к�
	int num=0;
	srand(time(NULL));
	for(int i = 0;i<81;i++)
		sudoku[i] = answer[i];
	while(num<(81-count)){//�ڿ�
		x = rand()%9+1;
		y = rand()%9+1;
		if(sudoku[(x-1)*9+(y-1)] != 0){
			sudoku[(x-1)*9+(y-1)] = 0;
			num++;
		}
	}
}

bool receiver(int* player_res){//������Ҵ�
	for(int i=0;i<81;i++){
		scanf("%d",&player_res[i]);
		if(!(player_res[i]>=1&&player_res[i]<=9)){//0 ��ʾ��ҷ���
			fflush(stdin);
			return false;
		}
	}
	return true;
}

int get_time(){//��õ�ǰʱ����
	time_t t;
	t=time(NULL);
	return t;
}

void ready(){
	print(sudoku);
	printf("����5���ӹ۲�ʱ��\n");
	for(int i = 0;i<5;i++){
		printf("��	");
		Sleep(1000);
	}
	printf("\n");
	printf("�۲��������ʱ��ʼ���뿪ʼ���𡣣������1~9�⣬��Ϊ��������\n");
	printf("==========================================================================\n");
}

bool judge(int* player_res,int* answer){//�ж���Ҵ�
	for(int i = 0;i<81;i++)
		if(player_res[i] != answer[i])
			return false;
	return true;
}

void record(player info){//��¼
	FILE* fp;
	int M = MAX,S = MAX,LEVEL = MAX;
	char NAME[20];
	char remove[100] = {"                                                      "};//���ڼ�¼���ȹ̶�����������¼�¼
	//ͨ�����ַ���������ֱ����һ���ļ��и������ݣ�����Ҫȫƪ�����ġ�д��ֱ���޸�һ��
	int c = 0;
	if((fp = fopen("record.txt","r+"))==NULL){//�ļ���cppͬĿ¼��
		printf("�ļ������ڣ�����ʧ�ܣ�");//��Ȼ���Զ������ļ���but�Է���һ
		return;
	}
	setbuf(fp,NULL);//���û�����
	rewind(fp);
	c = ftell(fp);//��¼��ǰ�еĿ�ͷָ��λ��

	while(fscanf(fp,"%s %d:%d %d",NAME,&M,&S,&LEVEL) != EOF){

		if(!strcmp(NAME,info.name) && LEVEL == info.level){//strcmp�Ƚ���ͬ����0
			
			if(info.m<M || (info.m==M&&info.s<S)){//������¼�¼�������
				fseek(fp,c,SEEK_SET);
				fputs(remove,fp);//���Ǿɼ�¼
				fseek(fp,c,SEEK_SET);//�ص��ü�¼�Ŀ�ͷλ��
			    fprintf(fp,"%s %d:%d %d",info.name,info.m,info.s,info.level);//д���ļ�
				fflush(fp);//���������
				return;
			}
			return;//�����¼�¼�Ͳ�����
		}
		fscanf(fp,"\n");//��ȡ����
		c = ftell(fp);
	}

	fputs(remove,fp);//�ȸ��ǹ̶����ȵ�����
	fseek(fp,c,SEEK_SET);//�ص����ǵ������ײ�
	fprintf(fp,"%s %d:%d %d",info.name,info.m,info.s,info.level);//�ڸ��ǵ������ڲ����¼
	fseek(fp,0,SEEK_END);//ָ��β��
	fprintf(fp,"\n");//���뻻�з�
	fclose(fp);
}


player* get_record(int level){//������Ҽ�¼�ĵ�������ͷ���
	FILE* fp;
	int M = MAX,S = MAX,LEVEL = MAX;
	char NAME[20];
	player* head = (player*)malloc(sizeof(player));
	head->next = NULL;
	if((fp = fopen("record.txt","r"))==NULL){
		printf("�ļ������ڣ�");
		system("pause");
		exit(1);
	}
	setbuf(fp,NULL);//���û�����
	rewind(fp);
	while(fscanf(fp,"%s %d:%d %d",NAME,&M,&S,&LEVEL) != EOF){
		if(LEVEL == level){
			player* p = (player*)malloc(sizeof(player));//��������
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

void order(player* head){//����������
	player* p;
	player* q;
	int temp1;
	int temp2;
	char temp3[20];
	for(p = head->next;p != NULL;p = p->next)
		for(q = p->next;q != NULL;q = q->next)
			if(p->m>q->m||(p->m==q->m&&p->s>q->s)){//�Ի�������������
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

void show(player* easy,player* normal,player* hard){//�������
	int no=1;
	player* p1 = easy->next;
	player* p2 = normal->next;
	player* p3 = hard->next;
	printf("==========================================================================\n");
	printf("\t\t   ��\t\t\t  һ��\t\t\t   ����\n");
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

void re_init(Node* head){//���³�ʼ���к��ж���
	Node* p;
	for(p = head->down;p!=head;p=p->down)
		p->col_elemCount = 0;
	
	if(head->right == head){//����н⣬���ж������search�б�ɾ������Ҫ���������ж���
		init_col(head);
		return;
		}
	
	for(p = head->right;p!=head;p=p->right){//�޽�ʱ,��ָ��ж���
		p->col_elemCount = 0;
		p->down = p;
		p->up = p;
	}
}

int main(){
	int player_res[81]={0};
	int choice;
	int** matrix;//���������01����
	int* answer;//��Ŵ�
	player* easy;//�����Ѷ�����
	player* normal;//���Ѷ�����
	player* hard;//�����Ѷ�����
	player info ;//�����Ϣ
	
	Node* head =(Node*)malloc(sizeof(Node));
	init(head);init_col(head);//��ʼ�����ж��󣬽���ʮ��˫��ѭ������
	srand(time(NULL));
	bool flag = false;//���޽�
	while(true){
	
		while(!flag){
			flag=true;
	
			create_sudoku();//����17������������17����������Ψһ�⣩
	
			matrix = create_matrix();//�õ���������01����
	
			link(head,matrix);//��01����ת��Ϊʮ��˫��ѭ������ת��Ϊ��׼�������⣩
	
			if(!search(head))//�õ����У�result��
				flag = false;//��������޽⣬�����¹��������������˲��ԣ��޽�ĸ��ʴ��Ϊ1/1000��
			index = 0;//��ʼ��resultջ
			
			re_init(head);//���³�ʼ��ʮ������
		}
	
	flag = false;
	answer = to_sudoku(matrix);//�õ���
	
	
	printf("                               ����\n");
	printf("==========================================================================\n\n");
	printf("1.��ʼ��Ϸ\n");
	printf("2.�鿴����\n");
	printf("3.�˳�\n\n");
	printf("==========================================================================\n");
	printf("��ѡ��:");
	scanf("%d",&choice);
	switch(choice){
	case 1:
			int option;
			printf("�����:");
			scanf("%s",&info.name);
			if(strlen(info.name)>20){
				printf("����̫����\n");
				break;
			}
			printf("��ѡ����Ϸ�Ѷ�:  1.��\t2.һ��\t3.����\n");
			scanf("%d",&option);
			printf("\n");
			switch(option){
			case 1:
				sudoku_level(answer,40);//�ڿմ�
				ready();
				time_start = get_time();
				if(!receiver(player_res)){
					printf("\n���ѷ�������!\t��ȷ��Ϊ:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 1;
			
				if(judge(player_res,answer)){
					printf("�ش���ȷ!\t��ʱ:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("\n�ش����!\t��ȷ��Ϊ:\n\n");
					fflush(stdin);
					print(answer);
				}
				break;
			case 2:
				sudoku_level(answer,35);
				time_start = get_time();
				ready();
				if(!receiver(player_res)){
					printf("\n���ѷ�������!\t��ȷ��Ϊ:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 2;
				if(judge(player_res,answer)){
					printf("�ش���ȷ!\t��ʱ:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("�ش����!\t��ȷ��Ϊ:\n\n");
					fflush(stdin);
					print(answer);
				}
				break;
			case 3:
				sudoku_level(answer,30);
				time_start = get_time();
				ready();
				if(!receiver(player_res)){
					printf("\n���ѷ�������!\t��ȷ��Ϊ:\n\n");
					print(answer);
					break;
				}
				time_end = get_time();
				info.m = (time_end-time_start)/60;
				info.s = (time_end-time_start)%60;
				info.level = 3;
				if(judge(player_res,answer)){
					printf("�ش���ȷ!\t��ʱ:  %d:%d\n",info.m,info.s);
					record(info);
				}
				else {
					printf("�ش����!\t��ȷ��Ϊ:\n\n");
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
		printf("\n�ݰ�~\n\n");
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


