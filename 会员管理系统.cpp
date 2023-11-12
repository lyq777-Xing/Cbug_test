#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct VIP             //定义结构体，存储会员账号、姓名、身份证号、联系方式、状态、余额 
{
	long no;
	char name[20];    //姓名、身份证号含有字符，使用char定义 
	char IDcard[19];
	char phone[12];   //联系电话有11位，超出了int的范围，使用字符串定义 
	int zhuangtai;	  //如果状态为1，则正常，若为0，则状态为挂失 
	int yue;
};
struct VIP s[50];     //定义结构体数组 
int n=0;              //n表示存入的会员数量 
 
void init();          //含义见main主函数 
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
 
         
void init()   //初始化（唯一的6位会员账号）
{
	int f=1; //1唯一   0不唯一 
	printf("请输入会员的账号,姓名,身份证号码,联系电话,余额,状态\n");
	printf("当输入会员号为0时停止输入\n");
	while(n<=50)
	{
		printf("请输入会员的账号:\n");
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
						printf("请输入唯一的6位会员账号\n"); 
					}
			
				}   
				if(f==1) 
					break;
			}
			else
			{
				printf("请输入6位会员账号\n");	
				printf("请重新输入会员的账号:\n");
				scanf("%d",&s[n].no);
			}
		}
		if(s[n].no==0)   
			break; 		//如果输入的账号为0，结束输入
		printf("请输入会员的姓名:\n");
		scanf("%s",s[n].name);
		printf("请输入会员的身份证号，联系电话，余额，状态:\n");
		scanf("%s%s%d%d",s[n].IDcard,s[n].phone,&s[n].yue,&s[n].zhuangtai);
		n++;
	}
}
 
void print()  //输出 
{
	//|和=组合起来，使输出结果时有表格样式
	//%t、空格使输出时数据对其 
	int j;
	printf("会员信息记录如下:\n");
	printf("     |===================================================================================|\n");
	printf("     |账号\t姓名\t        身份证号      \t  联系电话   \t余额\t  状态           |\n");
	printf("     |-----------------------------------------------------------------------------------|\n");
	for(j=0;j<n;j++)		
	{
	    printf("     |%ld\t%s\t  %s\t%s\t%d\t",s[j].no,s[j].name,s[j].IDcard,s[j].phone,s[j].yue);
	    if(s[j].zhuangtai==1)
	         printf("  正常           |\n");
	    else
	         printf("  挂失           |\n"); 
	    if(j<(n-1))
		printf("     |-----------------------------------------------------------------------------------|\n");
	}
	printf("     |===================================================================================|\n");
}
 
struct VIP gete()       //输入 
{
	struct VIP e;
	printf("请输入账号:\n");
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
					printf("请输入唯一的6位会员账号\n"); 
					scanf("%d",&e.no);
				}		
			}   
			if(f==1)   
				break;
		}
		else
		{
			printf("请输入6位会员账号\n");	
			printf("请重新输入会员的账号:\n");
			scanf("%d",&e.no);
		}
	}
	printf("请输入姓名:\n");
	scanf("%s",&e.name);
	printf("请输入会员的身份证号，联系电话，余额，状态:\n");
	scanf("%s%s%d%d",&e.IDcard,&e.phone,&e.yue,&e.zhuangtai);
	return e;
 
}
 
void add1()   //添加新的会员信息 
{
	struct VIP e = gete();
	int i; 
   	printf("请输入要插入的位置（1-%d）：\n",n);
	scanf("%d",&i); 
	while(i<1||i>n+1) 
	{
		printf("您输入的位置有误！\n");
		printf("请重新输入要插入的位置（1-%d）：\n",n);
		scanf("%d",&i);
	}
	for(int j=n-1;j>=i-1;j--) 
	{
		s[j+1]=s[j];
	}
	s[i-1]=e;
	n++; 
}
 
void findbyno()     //根据会员账号查找会员信息 
{
	int num,i;
	int found=1;
	char choice;
    while(1)
	{
        printf("请输入要查找的会员的账号：\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
        	
        	if(s[i].no==num)
			{
					found=0;
					printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
					if(s[i].zhuangtai==1)
	                   printf("正常\n");
	                else
	                   printf("挂失!\n"); 
			}
		}
        if(found==1)
		{
			printf("未找到!\n");
		}
		found=1;	
		printf("是否继续查询(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void findbyname()    //根据会员姓名查找会员信息 
{
		char name[20];
		char choice;
		int found=1;
		int i;
		while(1)
		{
		printf("输入要查询会员的姓名：\n");
		scanf("%s",name);
		getchar();
		for(i=0;i<n&&found==1;i++)
		{
			if(strcmp(s[i].name,name)==0)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                printf("正常\n");
	            else
	                printf("挂失!\n"); 
			}
		}
		if(found==1)
		{
			printf("未找到!\n");
		}
		found=1;	
		printf("是否继续查询(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void findbyIDcard()   //根据身份证号查找会员信息 
{
    char num[19];
    int i;
	int found=1;
	char choice;
    while(1)
	{
        printf("请输入要查找的会员的身份证号：\n");
        scanf("%s",num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
            if(strcmp(s[i].IDcard,num)==0)
			{
					found=0;
					printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
					if(s[i].zhuangtai==1)
	                   printf("正常\n");
	                else
	                   printf("挂失!\n"); 
			}
		}
        if(found==1)
		{
			printf("未找到!\n");
		}
		found=1;	
		printf("是否继续查询(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}		
}
 
void findbyphone()    //根据联系电话查找会员信息 
{
    char num[12];
    int i;
	int found=1;
	char choice;
    while(1)
	{
        printf("请输入要查找的会员的联系电话：\n");
        scanf("%s",num);
        getchar();
        for(i=0;i<n&&found==1;i++)
        {
            if(strcmp(s[i].phone,num)==0)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                printf("正常\n");
	            else
	                printf("挂失!\n"); 
			}
		}
        if(found==1)
		{
			printf("未找到!\n");
		}
		found=1;	
		printf("是否继续查询(Y/N)\n");
		choice=getchar();
		if(choice=='N'|| choice=='n')
			break;
	}		
}
 
void mod()     //修改已有的会员信息 
{
    int num,i;
	int found=1;
	char choice;
	int opt;
	while(1)
	{
		printf("请输入要修改的会员的账号(会员号无法修改)：\n");
		scanf("%d",&num);
		getchar();
		for (i=0;i<n&&found==1;i++)
		{
			if(s[i].no==num)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                 printf("正常\n");
	            else
	                 printf("挂失\n"); 
				printf("--请选择--1.修改身份证号 2.修改联系电话 3.修改状态\n");
				scanf("%d",&opt);
				getchar();
				switch(opt)
				{
				case 1:
					printf("请输入身份证号新值:\n");  
					scanf("%s",s[i].IDcard);   
					getchar(); 
					break;
				case 2:
					printf("请输入联系电话新值:\n");  
					scanf("%s",s[i].phone);     
					getchar(); 
					break;
				case 3:
					printf("请输入状态新值:\n");  
					scanf("%d",&s[i].zhuangtai);   
					getchar(); 
					break;				
				}
			}
		}
		if(found==1)
		{
			printf("未找到!\n");
		}	
		found=1;
		printf("是否继续修改(Y or N)\n");
		choice=getchar();
		getchar();
		if(choice=='N'|| choice=='n')
			 break;
		}
}
 
void del()     //删除已有会员信息 
{
    int num,i,j;
	int found=1;
	char choice;
    while(1)
	{
        printf("请输入要删除的会员的账号：\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)   //5  1-5      
        {
			if(s[i].no==num)
			{
				found=0;
				printf("你要删除的会员信息为:\n");
				printf("会员账号     姓名      身份证号      联系电话       余额        状态\n"); 
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	                 printf("正常\n");
	            else
	                 printf("挂失!\n"); 
				for(j=i;j<n-1;j++)
				{
					s[j]=s[j+1];
				}
				n--;				
			}
		}
		if(found==1)
		{
			printf("未找到!\n");
		}	
		found=1;
		printf("是否继续（Y/N）\n");
		choice=getchar();
		getchar();
		if(choice=='N'|| choice=='n')
			break;
	}
}
void transaction()     //消费或充值 
{
	int num,i,a,b;
	int found=1;
	char choice;
    while(1)
	{
        printf("请输入会员的账号：\n");
        scanf("%d",&num);
        getchar();
        for(i=0;i<n&&found==1;i++)
            if(s[i].no==num)
			{
				found=0;
				printf("%ld\t%s\t%d\t%d\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
				if(s[i].zhuangtai==1)
	            	printf("正常\n");
	            else
	                printf("挂失!\n"); 
	            printf("请选择类型：1.消费  2.充值\n");
	            scanf("%d",&a);
	        	getchar();
	            printf("请输入金额：\n");
	            scanf("%d",&b); 
	            getchar();
	            if(a==1)
	            {
	            	s[i].yue=s[i].yue-b;
					printf("%ld\t%s\t%s\t%s\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue);
	                if(s[i].zhuangtai==1)
	                   printf("正常\n");
	                else
	                   printf("挂失!\n");
				}
				else
				{
					s[i].yue=s[i].yue+b;
					printf("%ld\t%s\t%s\t%s\t%d\t",s[i].no,s[i].name,s[i].IDcard,s[i].phone,s[i].yue); 
					if(s[i].zhuangtai==1)
	                   printf("正常\n");
	                else
	                   printf("挂失!\n");
				}       
			}
        if(found==1)
		{
			printf("未找到!\n");
		}
		found=1;	
		printf("是否继续(Y/N)\n");
		choice=getchar();
		getchar(); 
		if(choice=='N'|| choice=='n')
			break;
	}
}
 
void sort()     //根据会员账号从小到大排序 
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
	printf("\n排序后的会员信息:\n");
}
	
int main()
{
	int opt;
	printf("     |*============================================*|\n");
	printf("     |*             欢迎使用会员管理系统           *|\n");
	printf("     |*============================================*|\n");
	printf("     |*    1.初始化                                *|\n");
	printf("     |*    2.添加新的会员记录                      *|\n");
	printf("     |*    3.修改已有会员记录/挂失                 *|\n");
	printf("     |*    4.根据账号查询已有会员记录              *|\n");
	printf("     |*    5.根据姓名查询已有会员记录              *|\n");         
	printf("     |*    6.根据身份证号查询已有会员记录          *|\n");
	printf("     |*    7.根据联系电话查询已有会员记录          *|\n");
	printf("     |*    8.删除已有会员记录                      *|\n");
	printf("     |*    9.根据会员账号排序并显示                *|\n");
	printf("     |*    10.会员消费或充值                       *|\n"); 
	printf("     |*    0.退出会员信息管理系统                  *|\n");
	printf("     |*============================================*|\n");
	while(1)
	{
		printf("请输入要完成的功能：0-10:");
		scanf("%d",&opt);
		getchar();    //吸收回车 
		switch(opt)
		{
			case 1:init();print();break;          //初始化 
			case 2:add1();print();break;          //添加 
			case 3:mod();print();break;           //修改 
			case 4:findbyno();print();break;      //根据账号查询 
			case 5:findbyname();print();break;    //根据姓名查询 
			case 6:findbyIDcard();print();break;  //根据身份证号查询 
			case 7:findbyphone();print();break;   //根据联系电话查询 
			case 8:del();print();break;           //删除 
			case 9:sort();print();break;          //排序 
			case 10:transaction();print();break;  //消费/充值 
			case 0:system("cls");exit(0);break;   //退出 
			default:printf("请输入0-10中的数!\n");break;
		}
	}
}
