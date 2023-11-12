#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct VIP             //����ṹ�壬�洢��Ա�˺š����������֤�š���ϵ��ʽ��״̬����� 
{
	long no;
	char name[20];    //���������֤�ź����ַ���ʹ��char���� 
	char IDcard[19];
	char phone[12];   //��ϵ�绰��11λ��������int�ķ�Χ��ʹ���ַ������� 
	int zhuangtai;	  //���״̬Ϊ1������������Ϊ0����״̬Ϊ��ʧ 
	int yue;
};
struct VIP s[50];     //����ṹ������ 
int n=0;              //n��ʾ����Ļ�Ա���� 
 
void init();          //�����main������ 
void print();           
struct VIP gete();
void add1();             
void findbyno();       
void findbyname();
void findbyIDcard();
void findbyphone();     
void mod();            
void del();             
void sort();  
void transaction();
 
         
void init()   //��ʼ����Ψһ��6λ��Ա�˺ţ�
{
	int f=1; //1Ψһ   0��Ψһ 
	printf("�������Ա���˺�,����,���֤����,��ϵ�绰,���,״̬\n");
	printf("�������Ա��Ϊ0ʱֹͣ����\n");
	while(n<=50)
	{
		printf("�������Ա���˺�:\n");
		scanf("%ld",&s[n].no);
		while(s[n].no!=0)
		{
			if(s[n].no/1000000==0 && s[n].no/100000!=0)
			{
				int i;
				for(i=0;i<n&&f==1;i++)
				{
					if(s[n].no==s[i].no)
					{
						f=0;
						printf("������Ψһ��6λ��Ա�˺�\n"); 
					}
			
				}   
				if(f==1) 
					break;
			}
			else
			{
				printf("������6λ��Ա�˺�\n");	
				printf("�����������Ա���˺�:\n");
				scanf("%d",&s[n].no);
			}
		}
		if(s[n].no==0)   
			break; 		//���������˺�Ϊ0����������
		printf("�������Ա������:\n");
		scanf("%s",s[n].name);
		printf("�������Ա�����֤�ţ���ϵ�绰����״̬:\n");
		scanf("%s%s%d%d",s[n].IDcard,s[n].phone,&s[n].yue,&s[n].zhuangtai);
		n++;
	}
}
 
void print()  //��� 
{
	//|��=���������ʹ������ʱ�б����ʽ
	//%t���ո�ʹ���ʱ���ݶ��� 
	int j;
	printf("��Ա��Ϣ��¼����:\n");
	printf("     |===================================================================================|\n");
	printf("     |�˺�\t����\t        ���֤��      \t  ��ϵ�绰   \t���\t  ״̬           |\n");
	printf("     |-----------------------------------------------------------------------------------|\n");
	for(j=0;j<n;j++)		
	{
	    printf("     |%ld\t%s\t  %s\t%s\t%d\t",s[j].no,s[j].name,s[j].IDcard,s[j].phone,s[j].yue);
	    if(s[j].zhuangtai==1)
	         printf("  ����           |\n");
	    else
	         printf("  ��ʧ           |\n"); 
	    if(j<(n-1))
		printf("     |-----------------------------------------------------------------------------------|\n");
	}
	printf("     |===================================================================================|\n");
}
 
struct VIP gete()       //���� 
{
	struct VIP e;
	printf("�������˺�:\n");
	scanf("%d",&e.no);
	int f=1;
	while(e.no)
	{	
		if(e.no/1000000==0 && e.no/100000!=0)
		{
			int i;
			for(i=0;i<n&&f==1;i++)
			{
				if(e.no==s[i].no)
				{
					f=0;
					printf("������Ψһ��6λ��Ա�˺�\n"); 
					scanf("%d",&e.no);
				}		
			}   
			if(f==1)   
				break;
		}
		else
		{
			printf("������6λ��Ա�˺�\n");	
			printf("�����������Ա���˺�:\n");
			scanf("%d",&e.no);
		}
	}
	printf("����������:\n");
	scanf("%s",&e.name);
	printf("�������Ա�����֤�ţ���ϵ�绰����״̬:\n");
	scanf("%s%s%d%d",&e.IDcard,&e.phone,&e.yue,&e.zhuangtai);
	return e;
 
}
 
void add1()   //����µĻ�Ա��Ϣ 
{
	struct VIP e = gete();
	int i; 
   	printf("������Ҫ�����λ�ã�1-%d����\n",n);
	scanf("%d",&i); 
	while(i<1||i>n+1) 
	{
		printf("�������λ������\n");
		printf("����������Ҫ�����λ�ã�1-%d����\n",n);
		scanf("%d",&i);
	}
	for(int j=n-1;j>=i-1;j--) 
	{
		s[j+1]=s[j];
	}
	s[i-1]=e;
	n++; 
}
 
void findbyno()     //���ݻ�Ա�˺Ų��һ�Ա��Ϣ 
{
	int num,i;
	int found=1;
	char choice;
    while(1)
	{
        printf("������Ҫ���ҵĻ�Ա���˺ţ�\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
        	
        	if(s[i].no==num)
			{
					found=0;
					printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
					if(s[i].zhuangtai==1)
	                   printf("����\n");
	                else
	                   printf("��ʧ!\n"); 
			}
		}
        if(found==1)
		{
			printf("δ�ҵ�!\n");
		}
		found=1;	
		printf("�Ƿ������ѯ(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void findbyname()    //���ݻ�Ա�������һ�Ա��Ϣ 
{
		char name[20];
		char choice;
		int found=1;
		int i;
		while(1)
		{
		printf("����Ҫ��ѯ��Ա��������\n");
		scanf("%s",name);
		getchar();
		for(i=0;i<n&&found==1;i++)
		{
			if(strcmp(s[i].name,name)==0)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                printf("����\n");
	            else
	                printf("��ʧ!\n"); 
			}
		}
		if(found==1)
		{
			printf("δ�ҵ�!\n");
		}
		found=1;	
		printf("�Ƿ������ѯ(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void findbyIDcard()   //�������֤�Ų��һ�Ա��Ϣ 
{
    char num[19];
    int i;
	int found=1;
	char choice;
    while(1)
	{
        printf("������Ҫ���ҵĻ�Ա�����֤�ţ�\n");
        scanf("%s",num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
            if(strcmp(s[i].IDcard,num)==0)
			{
					found=0;
					printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
					if(s[i].zhuangtai==1)
	                   printf("����\n");
	                else
	                   printf("��ʧ!\n"); 
			}
		}
        if(found==1)
		{
			printf("δ�ҵ�!\n");
		}
		found=1;	
		printf("�Ƿ������ѯ(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}		
}
 
void findbyphone()    //������ϵ�绰���һ�Ա��Ϣ 
{
    char num[12];
    int i;
	int found=1;
	char choice;
    while(1)
	{
        printf("������Ҫ���ҵĻ�Ա����ϵ�绰��\n");
        scanf("%s",num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
            if(strcmp(s[i].phone,num)==0)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                printf("����\n");
	            else
	                printf("��ʧ!\n"); 
			}
		}
        if(found==1)
		{
			printf("δ�ҵ�!\n");
		}
		found=1;	
		printf("�Ƿ������ѯ(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}		
}
 
void mod()     //�޸����еĻ�Ա��Ϣ 
{
    int num,i;
	int found=1;
	char choice;
	int opt;
	while(1)
	{
		printf("������Ҫ�޸ĵĻ�Ա���˺�(��Ա���޷��޸�)��\n");
		scanf("%d",&num);
		getchar();
		for (i=0;i<n&&found==1;i++)
		{
			if(s[i].no==num)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                 printf("����\n");
	            else
	                 printf("��ʧ\n"); 
				printf("--��ѡ��--1.�޸����֤�� 2.�޸���ϵ�绰 3.�޸�״̬\n");
				scanf("%d",&opt);
				getchar();
				switch(opt)
				{
				case 1:
					printf("���������֤����ֵ:\n");  
					scanf("%s",s[i].IDcard);   
					getchar(); 
					break;
				case 2:
					printf("��������ϵ�绰��ֵ:\n");  
					scanf("%s",s[i].phone);     
					getchar(); 
					break;
				case 3:
					printf("������״̬��ֵ:\n");  
					scanf("%d",&s[i].zhuangtai);   
					getchar(); 
					break;				
				}
			}
		}
		if(found==1)
		{
			printf("δ�ҵ�!\n");
		}	
		found=1;
		printf("�Ƿ�����޸�(Y or N)\n");
		choice=getchar();
		getchar();
		if(choice=='N'|| choice=='n')
			 break;
		}
}
 
void del()     //ɾ�����л�Ա��Ϣ 
{
    int num,i,j;
	int found=1;
	char choice;
    while(1)
	{
        printf("������Ҫɾ���Ļ�Ա���˺ţ�\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)   //5  1-5      
        {
			if(s[i].no==num)
			{
				found=0;
				printf("��Ҫɾ���Ļ�Ա��ϢΪ:\n");
				printf("��Ա�˺�     ����      ���֤��      ��ϵ�绰       ���        ״̬\n"); 
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                 printf("����\n");
	            else
	                 printf("��ʧ!\n"); 
				for(j=i;j<n-1;j++)
				{
					s[j]=s[j+1];
				}
				n--;				
			}
		}
		if(found==1)
		{
			printf("δ�ҵ�!\n");
		}	
		found=1;
		printf("�Ƿ������Y/N��\n");
		choice=getchar();
		getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
void transaction()     //���ѻ��ֵ 
{
	int num,i,a,b;
	int found=1;
	char choice;
    while(1)
	{
        printf("�������Ա���˺ţ�\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)
            if(s[i].no==num)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	            	printf("����\n");
	            else
	                printf("��ʧ!\n"); 
	            printf("��ѡ�����ͣ�1.����  2.��ֵ\n");
	            scanf("%d",&a);
	        	getchar();
	            printf("�������\n");
	            scanf("%d",&b); 
	            getchar();
	            if(a==1)
	            {
	            	s[i].yue=s[i].yue-b;
					printf("%ld\t%s\t%s\t%s\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
	                if(s[i].zhuangtai==1)
	                   printf("����\n");
	                else
	                   printf("��ʧ!\n");
				}
				else
				{
					s[i].yue=s[i].yue+b;
					printf("%ld\t%s\t%s\t%s\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue); 
					if(s[i].zhuangtai==1)
	                   printf("����\n");
	                else
	                   printf("��ʧ!\n");
				}       
			}
        if(found==1)
		{
			printf("δ�ҵ�!\n");
		}
		found=1;	
		printf("�Ƿ����(Y/N)\n");
		choice=getchar();
		getchar(); 
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void sort()     //���ݻ�Ա�˺Ŵ�С�������� 
{
	int i,j;
	struct VIP temp;
	for(i=0;i<n;i++) 
	{
		for (j=i+1;j<n;j++) 
		{
			if (s[i].no>s[j].no) 
			{ 
				temp=s[i]; 
				s[i]=s[j]; 
				s[j]=temp; 
			}
		}
	}
	printf("\n�����Ļ�Ա��Ϣ:\n");
}
	
int main()
{
	int opt;
	printf("     |*============================================*|\n");
	printf("     |*             ��ӭʹ�û�Ա����ϵͳ           *|\n");
	printf("     |*============================================*|\n");
	printf("     |*    1.��ʼ��                                *|\n");
	printf("     |*    2.����µĻ�Ա��¼                      *|\n");
	printf("     |*    3.�޸����л�Ա��¼/��ʧ                 *|\n");
	printf("     |*    4.�����˺Ų�ѯ���л�Ա��¼              *|\n");
	printf("     |*    5.����������ѯ���л�Ա��¼              *|\n");         
	printf("     |*    6.�������֤�Ų�ѯ���л�Ա��¼          *|\n");
	printf("     |*    7.������ϵ�绰��ѯ���л�Ա��¼          *|\n");
	printf("     |*    8.ɾ�����л�Ա��¼                      *|\n");
	printf("     |*    9.���ݻ�Ա�˺�������ʾ                *|\n");
	printf("     |*    10.��Ա���ѻ��ֵ                       *|\n"); 
	printf("     |*    0.�˳���Ա��Ϣ����ϵͳ                  *|\n");
	printf("     |*============================================*|\n");
	while(1)
	{
		printf("������Ҫ��ɵĹ��ܣ�0-10:");
		scanf("%d",&opt);
		getchar();    //���ջس� 
		switch(opt)
		{
			case 1:init();print();break;          //��ʼ�� 
			case 2:add1();print();break;          //��� 
			case 3:mod();print();break;           //�޸� 
			case 4:findbyno();print();break;      //�����˺Ų�ѯ 
			case 5:findbyname();print();break;    //����������ѯ 
			case 6:findbyIDcard();print();break;  //�������֤�Ų�ѯ 
			case 7:findbyphone();print();break;   //������ϵ�绰��ѯ 
			case 8:del();print();break;           //ɾ�� 
			case 9:sort();print();break;          //���� 
			case 10:transaction();print();break;  //����/��ֵ 
			case 0:system("cls");exit(0);break;   //�˳� 
			default:printf("������0-10�е���!\n");break;
		}
	}
}
