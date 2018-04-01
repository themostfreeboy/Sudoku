#include <stdio.h>
#include <stdlib.h>

int data[9][9];

int ConvertNumToBit(int x)//��1-9����ת��Ϊ��Ӧ������λ��Ϊ1����������λ��Ϊ0����
{
	if(x<0)	x=-x;
	if(x==0)	return 0;
	if((x>=1)&&(x<=9))	return (1<<(x-1))&(0x01ff);
		else	return -1;//����
}

int CalcNumOfOne(int x)//�������������1�ĸ���
{
	int count=0;
	while(x>0)
	{
		x&=x-1;
		count++;
	}
	return count;
}

int JudgeRightOfOne(int x)//�ж϶�������������������һ����Ϊ0(Ϊ1)��Ӧ����(1-9)��������������ҵ�������1-9��ȡ��С����
{
	for(int i=0;i<9;i++)
	{
		if((x&(1<<i))!=0)
		{
			return (i+1);
		}
	}
	return 0;//�����Ϊ0��û��1
}

void UpdateOneData(int i,int j)//��data[i][j]Ϊ׼������i��:data[i][k]������j��:data[k][j]�����data[i][j]���ڵĹ�������data[i][j]����
{
	for(int k=0;k<9;k++)//����i��:data[i][k]
	{
		if(k==j)	continue;
		if(data[i][k]<0)
		{
			data[i][j]&=(~ConvertNumToBit(data[i][k]))&(0x01ff);
		}
	}
	for(int k=0;k<9;k++)//����j��:data[k][j]
	{
		if(k==i)	continue;
		if(data[k][j]<0)
		{
			data[i][j]&=(~ConvertNumToBit(data[k][j]))&(0x01ff);
		}
	}
	for(int k=(i/3)*3;k<=(i/3)*3+2;k++)//���data[i][j]���ڵĹ�
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

void UpdateMultiData(int i,int j)//��data[i][j]�����޸ĺ󣬶��������У������У����ڹ��ڵ����ݸ���
{
	for(int k=0;k<9;k++)//�޸ĵ�i��:data[i][k]����
	{
		if(k==j)	continue;
		if(data[i][k]>0)
		{
			data[i][k]&=(~ConvertNumToBit(data[i][j]))&(0x01ff);
		}
	}
	for(int k=0;k<9;k++)//�޸ĵ�j��:data[k][j]����
	{
		if(k==i)	continue;
		if(data[k][j]>0)
		{
			data[k][j]&=(~ConvertNumToBit(data[i][j]))&(0x01ff);
		}
	}
	for(int k=(i/3)*3;k<=(i/3)*3+2;k++)//�޸�data[i][j]���ڵĹ�����
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

void FindMinPos(int* min,int* pos)//��ǰ����ɨ�裬�ҳ���һ����������С��λ��
{
	*min=10;
	*pos=-1;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if((data[i][j]>=0)&&(CalcNumOfOne(data[i][j])<*min))//�˴�data[i][j]>=0�еȺ���Ϊ�˼���ĳλ���޷���1-9�����������������ݣ���ջ����Ҫ����(����˴�û�еȺţ����޷����ݣ�û�л��ݵ��ж�����)
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
	int position[1024];//��¼ѹջʱ�޸�Ԫ�ص�λ��
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
		printf("\"in.txt\"������!\n");
		system("pause");
		return 0;
	}
	err_write=fopen_s(&fp_write,"out.txt","w");
	if(err_write!=0)
	{
		fclose(fp_read);
		printf("\"out.txt\"����ռ�û����޷�����!\n");
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
				printf("��ȡ����������\n");
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
			if(data[i][j]>0)	UpdateOneData(i,j);//����data[i][j]����
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
			data[*pos/9][*pos%9]&=~ConvertNumToBit(tempnumber)&(0x01ff);//�Ƚ������Ŀ�����ȥ����ѹջ��������ջʱ�������ظ�ȥ�����ֿ�����
			s->push(*pos);
			data[*pos/9][*pos%9]=-tempnumber;
			UpdateMultiData(*pos/9,*pos%9);//ѹջ���ٸ������ݣ��Ա㵯ջʱ��Щ���µ����ݿ��Իָ�
		}
		else if(*min==0)//�ҵ���һ��λ�ã�ʹ����һ�θ�λ��������ʲô�����޷�����Ҫ�󣬻��ݣ���ջ
		{
			m:
			if(s->IsEmpty()==false)//ջ�ǿգ����ݣ���ջ
			{
				int poppos=s->pop();
				tempnumber=JudgeRightOfOne(data[poppos/9][poppos%9]);
				if(tempnumber==0)//��λ����Ȼ�޷������������������ݣ���ջ
				{
					goto m;
				}
				data[poppos/9][poppos%9]&=~ConvertNumToBit(tempnumber)&(0x01ff);//�Ƚ������Ŀ�����ȥ����ѹջ��������ջʱ�������ظ�ȥ�����ֿ�����
				s->push(poppos);
				data[poppos/9][poppos%9]=-tempnumber;
				UpdateMultiData(poppos/9,poppos%9);//ѹջ���ٸ������ݣ��Ա㵯ջʱ��Щ���µ����ݿ��Իָ�
			}
			else if(s->IsEmpty()==true)//ջ�ѿգ��������޽�
			{
				printf("�������޽�\n");
				fprintf(fp_write,"�������޽�\n");
				delete s;
				delete min;
				delete pos;
				fclose(fp_write);
				system("pause");
				return 0;
			}
		}
		else if(*min=10)//�����Ѿ�ȫ���������
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