#include <stdio.h>
#include <stdlib.h>

int data[9][9];

int ConvertNumToBit(int x)//将1-9的数转化为对应二进制位上为1其它二进制位上为0的数
{
	if(x<0)	x=-x;
	if(x==0)	return 0;
	if((x>=1)&&(x<=9))	return (1<<(x-1))&(0x01ff);
		else	return -1;//有误
}

int CalcNumOfOne(int x)//计算二进制数中1的个数
{
	int count=0;
	while(x>0)
	{
		x&=x-1;
		count++;
	}
	return count;
}

int JudgeRightOfOne(int x)//判断二进制数从右往左数第一个不为0(为1)对应的数(1-9)，从右往左代表找到的数从1-9，取最小的数
{
	for(int i=0;i<9;i++)
	{
		if((x&(1<<i))!=0)
		{
			return (i+1);
		}
	}
	return 0;//这个数为0，没有1
}

void UpdateOneData(int i,int j)//以data[i][j]为准：检查第i行:data[i][k]；检查第j列:data[k][j]；检查data[i][j]所在的宫。更新data[i][j]数据
{
	for(int k=0;k<9;k++)//检查第i行:data[i][k]
	{
		if(k==j)	continue;
		if(data[i][k]<0)
		{
			data[i][j]&=(~ConvertNumToBit(data[i][k]))&(0x01ff);
		}
	}
	for(int k=0;k<9;k++)//检查第j列:data[k][j]
	{
		if(k==i)	continue;
		if(data[k][j]<0)
		{
			data[i][j]&=(~ConvertNumToBit(data[k][j]))&(0x01ff);
		}
	}
	for(int k=(i/3)*3;k<=(i/3)*3+2;k++)//检查data[i][j]所在的宫
	{
		for(int l=(j/3)*3;l<=(j/3)*3+2;l++)
		{
			if((k==i)&&(l==j))	continue;
			if(data[k][l]<0)
			{
				data[i][j]&=(~ConvertNumToBit(data[k][l]))&(0x01ff);
			}
		}
	}
}

void UpdateMultiData(int i,int j)//在data[i][j]数据修改后，对其所在行，所在列，所在宫内的数据更新
{
	for(int k=0;k<9;k++)//修改第i行:data[i][k]数据
	{
		if(k==j)	continue;
		if(data[i][k]>0)
		{
			data[i][k]&=(~ConvertNumToBit(data[i][j]))&(0x01ff);
		}
	}
	for(int k=0;k<9;k++)//修改第j列:data[k][j]数据
	{
		if(k==i)	continue;
		if(data[k][j]>0)
		{
			data[k][j]&=(~ConvertNumToBit(data[i][j]))&(0x01ff);
		}
	}
	for(int k=(i/3)*3;k<=(i/3)*3+2;k++)//修改data[i][j]所在的宫数据
	{
		for(int l=(j/3)*3;l<=(j/3)*3+2;l++)
		{
			if((k==i)&&(l==j))	continue;
			if(data[k][l]>0)
			{
				data[k][l]&=(~ConvertNumToBit(data[i][j]))&(0x01ff);
			}
		}
	}
	return;
}

void FindMinPos(int* min,int* pos)//从前往后扫描，找出第一个可填数最小的位置
{
	*min=10;
	*pos=-1;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if((data[i][j]>=0)&&(CalcNumOfOne(data[i][j])<*min))//此处data[i][j]>=0有等号是为了检测出某位置无法填1-9的所有数，进而回溯，弹栈的重要依据(如果此处没有等号，则无法回溯，没有回溯的判断依据)
			{
				*min=CalcNumOfOne(data[i][j]);
				*pos=i*9+j;
			}
		}
	}
}

class MyStack
{
public:
	MyStack();
	int top;
	int stackdata[1024][9][9];
	int position[1024];//记录压栈时修改元素的位置
	void push(int temp_pos);
	int pop();
	bool IsEmpty();
};

MyStack::MyStack()
{
	top=-1;
	for(int k=0;k<1024;k++)
	{
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				stackdata[k][i][j]=0;
			}
		}
		position[k]=-1;
	}
}

void MyStack::push(int temp_pos)
{
	top++;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			stackdata[top][i][j]=data[i][j];
		}
	}
	position[top]=temp_pos;
}

int MyStack::pop()
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			data[i][j]=stackdata[top][i][j];
		}
	}
	return position[top--];
}

bool MyStack::IsEmpty()
{
	return top==-1?true:false;
}

int main()
{
	errno_t err_read,err_write;
	FILE *fp_read=NULL,*fp_write=NULL;
	err_read=fopen_s(&fp_read,"in.txt","r");
	if(err_read!=0)
	{
		printf("\"in.txt\"不存在!\n");
		system("pause");
		return 0;
	}
	err_write=fopen_s(&fp_write,"out.txt","w");
	if(err_write!=0)
	{
		fclose(fp_read);
		printf("\"out.txt\"正被占用或者无法创建!\n");
		system("pause");
		return 0;
	}
	int i=0,j=0;
	int tempnumber=0;
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			fscanf(fp_read,"%d",&tempnumber);
			if((tempnumber>=1)&&(tempnumber<=9))
			{
				data[i][j]=-tempnumber;
			}
			else if(tempnumber==0)
			{
				data[i][j]=0x01ff;
			}
			else
			{
				printf("读取的数据有误\n");
				fclose(fp_read);
				fclose(fp_write);
				system("pause");
				return 0;
			}
		}
	}
	fclose(fp_read);
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(data[i][j]>0)	UpdateOneData(i,j);//更新data[i][j]数据
		}
	}
	MyStack *s=new MyStack();
	int* min=new int();
	int* pos=new int();
	while(1)
	{
		FindMinPos(min,pos);
		if((*min!=0)&&(*min!=10))
		{
			tempnumber=JudgeRightOfOne(data[*pos/9][*pos%9]);
			data[*pos/9][*pos%9]&=~ConvertNumToBit(tempnumber)&(0x01ff);//先将该数的可能性去掉再压栈，这样弹栈时不会再重复去试这种可能性
			s->push(*pos);
			data[*pos/9][*pos%9]=-tempnumber;
			UpdateMultiData(*pos/9,*pos%9);//压栈后再更新数据，以便弹栈时这些更新的数据可以恢复
		}
		else if(*min==0)//找到了一个位置，使得下一次该位置无论填什么数均无法满足要求，回溯，弹栈
		{
			m:
			if(s->IsEmpty()==false)//栈非空，回溯，弹栈
			{
				int poppos=s->pop();
				tempnumber=JudgeRightOfOne(data[poppos/9][poppos%9]);
				if(tempnumber==0)//该位置仍然无法继续填数，继续回溯，弹栈
				{
					goto m;
				}
				data[poppos/9][poppos%9]&=~ConvertNumToBit(tempnumber)&(0x01ff);//先将该数的可能性去掉再压栈，这样弹栈时不会再重复去试这种可能性
				s->push(poppos);
				data[poppos/9][poppos%9]=-tempnumber;
				UpdateMultiData(poppos/9,poppos%9);//压栈后再更新数据，以便弹栈时这些更新的数据可以恢复
			}
			else if(s->IsEmpty()==true)//栈已空，该数独无解
			{
				printf("该数独无解\n");
				fprintf(fp_write,"该数独无解\n");
				delete s;
				delete min;
				delete pos;
				fclose(fp_write);
				system("pause");
				return 0;
			}
		}
		else if(*min=10)//数独已经全部填入完成
		{
			for(i=0;i<9;i++)
			{
				for(j=0;j<9;j++)
				{
					printf("%d ",-data[i][j]);
					fprintf(fp_write,"%d ",-data[i][j]);
				}
				printf("\n");
				fprintf(fp_write,"\n");
			}
			delete s;
			delete min;
			delete pos;
			fclose(fp_write);
			system("pause");
			return 0;
		}
	}
	delete s;
	delete min;
	delete pos;
	fclose(fp_write);
	system("pause");
	return 0;
}