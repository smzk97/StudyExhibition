#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <set>
#include <cstring>
#include <vector>
#define MAXSIZE 100
using namespace std;
set<string> set1,set2,set3,set4;      //�궨��set����������ȱ����Ա�Ĳ���
//������������
int NameLength(char s[])
{
	int i=0;
	while (s[i] != '\0')
		i++;
	return i+1;
} 
//����һ��ѧ���ɼ��洢�ṹ��
typedef struct
{
	char name[MAXSIZE];
	char xh[MAXSIZE];
	int grade[MAXSIZE];
	int sum;
	int SchRank;
	int ClaRank;
}student;
//����һ��ѧ���ɼ��仯����ṹ��
typedef struct
{
	char name[MAXSIZE];
	char xh[MAXSIZE];
	int GradeSum1;
	int GradeSum2;
	int GradeChange;
	int rank;
}student1;
//����һ��˳������洢����
typedef struct
{
	student data[MAXSIZE];
	int length;
}List;
//��������һ��˳��������洢�ɼ��仯����
typedef struct
{
	student1 data[MAXSIZE];
	int length;
}List1;
//�Զ�������ʵ�ֳɼ���������
bool cmp(student a,student b)
{
	return a.sum > b.sum;
}
//�Զ�������ʵ�ֳɼ��仯�Ľ�������
bool cmp1(student1 a, student1 b)
{
	return a.GradeChange > b.GradeChange;
}
//�����ļ��Ķ������,�����ݴ洢��˳�����
void ListWrite(List*& s, char* file)
{
	FILE* fp; int i = 0;
	//�����ļ�����������εķ���
	if ((fp = fopen(file, "r")) == NULL)
	{
		cout<<"�ļ��������";
		exit(0);
	}
	//�ļ�����ɹ����ֿ�����ݽ���¼��
	while (!feof(fp))
	{
		fscanf(fp, "%s%s", s->data[i].name, s->data[i].xh);
		/*if (mid != 2)
			break;*/
		for (int j = 0; j < 6; j++)
		{
			fscanf(fp, "%d", &s->data[i].grade[j]);
			/*if (mid != 1)
				break;*/
		}
		i++;
	}
	s->length = i;
	fclose(fp);
}
//�����ļ��Ķ������������ļ�ֻ��������ѧ�ŵ����ݵ��ļ�
void ListWrite1(List*& s, char* file)
{
	FILE* fp;int i = 0;
	if ((fp = fopen(file, "r")) == NULL)
	{
		cout << "�����ļ�����";
		exit(0);
	}
	while (!feof(fp))
	{
		int mid=fscanf(fp, "%s%s", s->data[i].name, s->data[i].xh);
		if (mid != 2)
			break;
		i++;
	}
	s->length = i;
	fclose(fp);
}
//��˳����ֽܷ�����Ӧ������
void ListSort1(List*& s)
{
	for (int i = 0;i < s->length;i++)         //�����ܷ�
	{
		int save = 0;
		for (int j = 0;j < 6;j++)
			save += s->data[i].grade[j];
		s->data[i].sum = save;
	}
	sort(s->data, s->data + s->length, cmp);
}
//��˳����ܷ֡�У��������������������
void ListSort2(List*& s)
{
	for (int i = 0;i < s->length;i++)         //�����ܷ�
	{
		int save = 0;
		for (int j = 0;j < 6;j++)
			save += s->data[i].grade[j];
		s->data[i].sum = save;
	}
	sort(s->data, s->data + s->length, cmp); //���ֽܷ��н�������
	for (int i = 0;i < s->length;i++)        //��У������������
	{
		s->data[i].SchRank = i + 1;
		if (i)
		{
			if (s->data[i - 1].sum == s->data[i].sum)
				s->data[i].SchRank = s->data[i - 1].SchRank;     //���������ͬ��������Ҳ��ͬ
		}
	}
	int sign1 = 0, sign2 = 0;
	for (int i = 0;i < s->length;i++)        //�԰�������������
	{
		if (s->data[i].xh[6] == '5')         //ͨ��ѧ�ŵ�7λ�Բ�ͬ���������
			s->data[i].ClaRank = ++sign1;
		else
			s->data[i].ClaRank = ++sign2;
		if (i)
		{
			if (s->data[i - 1].sum == s->data[i].sum && s->data[i].xh[6]==s->data[i-1].xh[6])   //���������ͬ��������Ҳ��ͬ
				s->data[i].ClaRank = s->data[i - 1].ClaRank;
		}
	}

}
//��˳��������������ĵ���
void DispList1(List* s, char* file)
{
	FILE* fp;
	if ((fp = fopen(file, "w")) == NULL)
	{
		cout << "�����ļ�����";
		exit(0);
	}
	fprintf(fp, "   ѧ������                ѧ��     \t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t�ܷ�\t\n");      //��ӡ��������Ϣ���ĵ���
	for (int j = 0;j < s->length;j++)
	{
		fprintf(fp, "    %s", s->data[j].name);           //��ӡ����
		int mid = NameLength(s->data[j].name);
		for (int i = 0;i < 20 - mid;i++)
			fprintf(fp, " ");                             //���и�ʽ�ϵĶ���
		fprintf(fp, "%s", s->data[j].xh);
		for (int i = 0;i < 6;i++)
			fprintf(fp, "\t   %3d\t", s->data[j].grade[i]);  //��ӡ���Ƴɼ�
		fprintf(fp, "\t%d", s->data[j].sum);                 //��ӡ�ܷ�
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//��˳��������������ĵ��У��ر����У�����Լ�������
void DispList2(List* s,char* file)
{
	FILE* fp;
	if((fp=fopen(file,"w"))==NULL)
	{
		cout<<"�����ļ�����";
		exit(0);
	}
	fprintf(fp,"   ѧ������                ѧ��     \t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t�ܷ�\tУ����\t������\n");
	for(int j=0;j<s->length;j++)
	{
		fprintf(fp,"    %s",s->data[j].name);          //��ӡ����
		int mid=NameLength(s->data[j].name);
		for(int i=0;i<20-mid;i++)
			fprintf(fp," ");                           //���и�ʽ�ϵĶ���
		fprintf(fp,"%s",s->data[j].xh);
		for(int i=0;i<6;i++)
			fprintf(fp,"\t   %3d\t",s->data[j].grade[i]);   //��ӡ���Ƴɼ�
		fprintf(fp,"\t%3d\t\t  %2d\t  %2d",s->data[j].sum,s->data[j].SchRank,s->data[j].ClaRank);   //��ӡ�ܷ֡�У������������
		fprintf(fp,"\n");
	}
	fclose(fp);
}
//��˳��������������ĵ��У�ֻ����������ѧ�š��ɼ�
void DispList3(List* s, char* file)
{
	FILE* fp;
	if ((fp = fopen(file, "a+")) == NULL)
	{
		cout << "�����ļ�����";
		exit(0);
	}
	for (int j = 0;j < s->length;j++)
	{
		fprintf(fp, "%s", s->data[j].name);             //��ӡ����
		int mid = NameLength(s->data[j].name);
		for (int i = 0;i < 20 - mid;i++)
			fprintf(fp, " ");                          //���и�ʽ�ϵĶ���
		fprintf(fp, "%s", s->data[j].xh);
		for (int i = 0;i < 6;i++)
			fprintf(fp, "\t%3d", s->data[j].grade[i]);  //��ӡ���Ƴɼ�
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//ͳ�Ƶ�һ��У����ĸ���ƽ���֣���ͷ֣���߷֣�����������
void Include1(List* p1,char FileName1[],char FileName3[])
{
	/*save1�����洢ȫУ�����˵ĸ��Ƶ��ܷ֣�save2��save3�����洢���������˸��Ƶ��ܷ֣�sum2������ȫУ���Ƶ���߷֣�sum22��sum222�����洢������Ƶ���߷�
	sum3������ȫУ���Ƶ���ͷ֣�sum33��sum333�����洢������Ƶ���ͷ֣�sum4������ȫУ���Ʋ�����������sum44��sum444�����������Ʋ���������             
	index1��index2����ͳ�Ƹ��������                                                                                                                                              */
	double save1[6] = { 0 }, save2[6] = { 0 }, save3[6] = { 0 };int sum2[6] = { 101,101,101,101,101,101 }, sum22[6] = { 101,101,101,101,101,101 }, sum222[6] = { 101,101,101,101,101,101 },
	sum3[6] = { 0 }, sum33[6] = { 0 }, sum333[6] = { 0 },sum4[6] = { 0 }, sum44[6] = { 0 }, sum444[6] = { 0 },index1=0,index2=0;
	for (int i = 0;i < p1->length;i++)
	{
		set1.insert(p1->data[i].name);               //����һ�γɼ�ѧ�������浽set������
		if (p1->data[i].xh[6] == '5')
			index1++;                               //�����˹�������
		else
			index2++;                               //�����˴�������
		for (int j = 0;j < 6;j++)
		{
			save1[j] += p1->data[i].grade[j];        //����ȫУ�ܷ�
			if (sum2[j] > p1->data[i].grade[j])
				sum2[j] = p1->data[i].grade[j];     //����ȫУ������߷�
			if (sum3[j] < p1->data[i].grade[j])
				sum3[j] = p1->data[i].grade[j];     //����ȫУ������ͷ�
			if (p1->data[i].grade[j] < 60)
				sum4[j]++;                          //����ȫУ���Ʋ���������
			if (p1->data[i].xh[6] == '5')          
			{
				set2.insert(p1->data[i].name);      //���˹����ѧ�������浽set������
				save2[j] += p1->data[i].grade[j];   //�����˹�������ܷ�
				if (sum22[j] > p1->data[i].grade[j])
					sum22[j] = p1->data[i].grade[j]; //�����˹��������߷�
				if (sum33[j] < p1->data[i].grade[j])
					sum33[j] = p1->data[i].grade[j]; //�����˹��������ͷ�
				if (p1->data[i].grade[j] < 60)
					sum44[j]++;                      //�����˹��಻��������
			}
			else
			{
				set3.insert(p1->data[i].name);       //���˴����ѧ�������浽set������
				save3[j] += p1->data[i].grade[j];    //�����˴�������ܷ�
				if (sum222[j] > p1->data[i].grade[j])
					sum222[j] = p1->data[i].grade[j]; //�����˴��������߷�
				if (sum333[j] < p1->data[i].grade[j])
					sum333[j] = p1->data[i].grade[j]; //�����˴��������ͷ�
				if (p1->data[i].grade[j] < 60)
					sum444[j]++;                      //�����˴��಻��������
			} 
		}
	}
	for (int i = 0;i < 6;i++)
		save1[i] /= p1->length, save2[i] /= index1, save3[i] /= index2;  //����ƽ����
	//������õ�����������ļ���
	FILE* fp1;
	if ((fp1 = fopen(FileName3, "a+")) == NULL)
	{
		cout << "�����ļ�����";
		exit(0);
	}
	fprintf(fp1, "\t��һ�ο���ȫУͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n",save1[0], save1[1], save1[2], save1[3], save1[4], save1[5] );
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n",sum2[0], sum2[1], sum2[2], sum2[3], sum2[4], sum2[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n",sum3[0], sum3[1], sum3[2], sum3[3], sum3[4], sum3[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n",sum4[0], sum4[1], sum4[2], sum4[3], sum4[4], sum4[5]);
	fprintf(fp1, "\t�˹����ܰ�ͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save2[0], save2[1], save2[2], save2[3], save2[4], save2[5]);
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum22[0], sum22[1], sum22[2], sum22[3], sum22[4], sum22[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum33[0], sum33[1], sum33[2], sum33[3], sum33[4], sum33[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n", sum44[0], sum44[1], sum44[2], sum44[3], sum44[4], sum44[5]);
	fprintf(fp1, "\t�˴���ͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save3[0], save3[1], save3[2], save3[3], save3[4], save3[5]);
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum222[0], sum222[1], sum222[2], sum222[3], sum222[4], sum222[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum333[0], sum333[1], sum333[2], sum333[3], sum333[4], sum333[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n", sum444[0], sum444[1], sum444[2], sum444[3], sum444[4], sum444[5]);
	fclose(fp1);
}
//ͳ�Ƶڶ���У����ĸ���ƽ���֣���ͷ֣���߷֣�����������������һ��������ͬ
void Include2(List* p2, char FileName2[], char FileName3[])
{
	double save1[6] = { 0 }, save2[6] = { 0 }, save3[6] = { 0 };int sum2[6] = { 101,101,101,101,101,101 }, sum22[6] = { 101,101,101,101,101,101 }, sum222[6] = { 101,101,101,101,101,101 },
	sum3[6] = { 0 }, sum33[6] = { 0 }, sum333[6] = { 0 }, sum4[6] = { 0 }, sum44[6] = { 0 }, sum444[6] = { 0 }, index1 = 0, index2 = 0;
	for (int i = 0;i < p2->length;i++)
	{
		if (p2->data[i].xh[6] == '5')
			index1++;
		else
			index2++;
		set1.erase(p2->data[i].name);
		for (int j = 0;j < 6;j++)
		{
			save1[j] += p2->data[i].grade[j];
			if (sum2[j] > p2->data[i].grade[j])
				sum2[j] = p2->data[i].grade[j];
			if (sum3[j] < p2->data[i].grade[j])
				sum3[j] = p2->data[i].grade[j];
			if (p2->data[i].grade[j] < 60)
				sum4[j]++;
			if (p2->data[i].xh[6] == '5')
			{
				set2.erase(p2->data[i].name);
				save2[j] += p2->data[i].grade[j];
				if (sum22[j] > p2->data[i].grade[j])
					sum22[j] = p2->data[i].grade[j];
				if (sum33[j] < p2->data[i].grade[j])
					sum33[j] = p2->data[i].grade[j];
				if (p2->data[i].grade[j] < 60)
					sum44[j]++;
			}
			else
			{
				set3.erase(p2->data[i].name);
				save3[j] += p2->data[i].grade[j];
				if (sum222[j] > p2->data[i].grade[j])
					sum222[j] = p2->data[i].grade[j];
				if (sum333[j] < p2->data[i].grade[j])
					sum333[j] = p2->data[i].grade[j];
				if (p2->data[i].grade[j] < 60)
					sum444[j]++;
			}
		}
	}
	for (int i = 0;i < 6;i++)
		save1[i] /= p2->length, save2[i] /= index1, save3[i] /= index2;
	FILE* fp1;
	if ((fp1 = fopen(FileName3, "a+")) == NULL)
	{
		cout << "�����ļ�����";
		exit(0);
	}
	fprintf(fp1, "\t�ڶ��ο���ȫУͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save1[0], save1[1], save1[2], save1[3], save1[4], save1[5]);
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum2[0], sum2[1], sum2[2], sum2[3], sum2[4], sum2[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum3[0], sum3[1], sum3[2], sum3[3], sum3[4], sum3[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum4[0], sum4[1], sum4[2], sum4[3], sum4[4], sum4[5]);
	fprintf(fp1, "\tȱ��������");
	for (auto i : set1)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fprintf(fp1, "\t�˹����ܰ�ͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save2[0], save2[1], save2[2], save2[3], save2[4], save2[5]);
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum22[0], sum22[1], sum22[2], sum22[3], sum22[4], sum22[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum33[0], sum33[1], sum33[2], sum33[3], sum33[4], sum33[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum44[0], sum44[1], sum44[2], sum44[3], sum44[4], sum44[5]);
	fprintf(fp1, "\tȱ��������");
	for (auto i : set2)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fprintf(fp1, "\t�˴���ͳ�ƣ�\n");
	fprintf(fp1, "\t\t\t�ߵ���ѧ\t��ѧӢ��\t��ѧ����\t��ѧ����\t���Դ���\t��ѧ����\t\n");
	fprintf(fp1, "\tƽ���֣�%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save3[0], save3[1], save3[2], save3[3], save3[4], save3[5]);
	fprintf(fp1, "\t��ͷ֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum222[0], sum222[1], sum222[2], sum222[3], sum222[4], sum222[5]);
	fprintf(fp1, "\t��߷֣�%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum333[0], sum333[1], sum333[2], sum333[3], sum333[4], sum333[5]);
	fprintf(fp1, "\t������%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum444[0], sum444[1], sum444[2], sum444[3], sum444[4], sum444[5]);
	fprintf(fp1, "\tȱ��������");
	for (auto i : set3)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fclose(fp1);
}
//�ɼ���¼
void GradeRecord(List* s1, List* s2, char FileName1[], char FileName2[])
{
	List* h = (List*)malloc(sizeof(List));h->length = 0;ListWrite(h, FileName1);     //����¼�ĳɼ��洢��ѭ���h��
	//������ȱ����Ա�Ĳ���
	for (int i = 0;i < s1->length;i++)
		set4.insert(s1->data[i].name);
	for (int j = 0;j < s2->length;j++)
		set4.erase(s2->data[j].name);
	FILE* fp;
	if ((fp = fopen(FileName2, "w")) == NULL)
	{
		cout << "�ļ��������\n";
		exit(0);
	}
	//��ӡȱ����Ա����
	fprintf(fp, "ȱ����Ա������");
	for (auto i : set4)
		fprintf(fp,"%s  ", i.c_str());        //������Ҫע�⣬%s�������char* ���͵����ݣ���set4�����������string���ͣ���Ҫͨ��c_strת��Ϊc���Է����ַ���
	fprintf(fp, "\n��¼�ɼ����£�\n");
	fclose(fp);
	DispList3(h, FileName2);                  //����¼�������гɼ����´�ӡ
	free(h);
}
//����һ�����ɼ��ܷ��ɸߵ�����ʾ���м�¼
void f1()
{
	List* p = (List*)malloc(sizeof(List));p->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout<<"������Ҫ�򿪵��ļ�����"; cin>>FileName1;
	cout<<"������Ҫ������ļ�����"; cin>>FileName2;
	ListWrite(p, FileName1);               //���򿪵��ļ������ݴ洢��˳�����
	ListSort1(p);DispList1(p, FileName2);free(p); //�����ܷ֣�������ݣ��ͷ�˳���
}
//���ܶ���ͳ�ƻ��ܣ������ο����У�У����ĸ���ƽ���֣���߷֣���ͷ֣��������������ڶ��ο���ȱ���������������ݣ�
void f2()
{
	List* p1= (List *)malloc(sizeof(List));p1->length = 0;List* p2= (List *)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "�������һ�γɼ����ļ�����"; cin >> FileName1;
	cout << "������ڶ��γɼ����ļ�����"; cin >> FileName2;
	cout << "��������Ӧ������ļ�����";cin >> FileName3;
	//���򿪵��ļ������ݴ洢��˳���p1��p2�У�ͳ�ƻ��ܸ������ݣ��ٽ����ݽ������������ͷ�˳���
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);Include1(p1, FileName1, FileName3);Include2(p2, FileName2, FileName3);free(p1);free(p2);
}
//����������ѯ���ݣ�������������ѧ�ſɲ�ѯ���������гɼ������������֧��ģ����ѯ�������롰��ɲ�ѯ���������к��С����ѧ���ĳɼ�����
void f3()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "������Ҫ��ѯ�ĳɼ����ļ�����";cin >> FileName1;
	cout << "������Ҫ��ѯ�����ֹؼ��ʣ�";cin >> FileName2;
	cout << "��������Ӧ������ļ�����";cin >> FileName3;
	ListWrite(p1, FileName1);ListSort2(p1);         //���򿪵��ļ������ݴ洢��˳����У����ܷ֡�������������
	char* index1;char* index2;vector<int> sum(MAXSIZE);int index = 0;   //ʹ��˫ָ������ַ��������ַ����Ĳ���
	for (int i = 0;i < p1->length;i++)
	{
		index1 = FileName2;                        //ʹ�ñ���index1��¼�û�����Ĺؼ���
		index2 = p1->data[i].name;                 //ʹ�ñ���index2��¼�ļ��д��ڵ�����
		while (*index2 != '\0' && *index1 != '\0')
		{
			if (*index1 == *index2)                //����ؼ��ʵ�ĳһ���ַ��������е�ĳһ���ַ���ͬ���������ַ�����������һ���ַ���ֱ������������'\0'
				index1++, index2++;
			else
				index2++;                          //�����ͬ��ֻ��Ҫָ���������ַ�ָ��index2�����ƶ�
		}
		if (*index1 == '\0' && *index2 != '\0' || *index1 == '\0' && *index2 == '\0')     //����ҵ����ַ�������¼˳�����data������±꣬���ڼ���ѧ��
			sum[index++] = i;
	}
	List* p3 = (List*)malloc(sizeof(List));p3->length = index;
	for (int i = 0;i < index;i++)
	{
		strcpy(p3->data[i].name, p1->data[sum[i]].name);strcpy(p3->data[i].xh, p1->data[sum[i]].xh);    //���ҵ������ַ�����Ӧ��������ѧ�š��ܷ֡��ɼ�����У������ֵ��˳���p3
		p3->data[i].sum = p1->data[sum[i]].sum;p3->data[i].SchRank = p1->data[sum[i]].SchRank;p3->data[i].ClaRank = p1->data[sum[i]].ClaRank;
		for (int j = 0;j < 6;j++)
			p3->data[i].grade[j] = p1->data[sum[i]].grade[j];
	}
	DispList2(p3, FileName3);free(p1);free(p3);                 //����ѯ�������ݽ���������ͷ�˳���p1��p3
}
//�����ģ��޸�����
void f4()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;List* p3 = (List*)malloc(sizeof(List));p3->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "������Ҫ�޸ĵĳɼ����ļ�����";cin >> FileName1;
	cout << "��������ĺ�����ݵ��ļ�����";cin >> FileName2;
	cout << "��������Ӧ������ļ�����";cin >> FileName3;
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);        //���򿪵��ļ������ݴ洢��˳�����
	for (int i = 0;i < p2->length;i++)
	{
		for (int j = 0;j < p1->length;j++)
		{
			//ͨ��ѧ�Ų��ҵ�ɾ�������ݵ�λ�ã���˳�������޸�
			if (p1->data[j].xh[6] == p2->data[i].xh[6] && p1->data[j].xh[7]==p2->data[i].xh[7] && p1->data[j].xh[11]==p2->data[i].xh[11] && p1->data[j].xh[12]==p2->data[i].xh[12])
			{
				for (int k = 0;k < 6;k++)
					p1->data[j].grade[k] = p2->data[i].grade[k];
			}
		}
	}
	DispList3(p1, FileName3);free(p1);free(p2);        //������ݵ��ĵ��У��ͷ�˳���p1��p2
}
//�����壺�ɼ���¼
void f5()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];char FileName4[MAXSIZE];
	cout << "�������һ�γɼ����ļ�����"; cin >> FileName1;
	cout << "������ڶ��γɼ����ļ�����"; cin >> FileName2;
	cout << "������Ҫ��¼�����ݵ��ļ�����";cin >> FileName3;
	cout << "��������Ӧ������ļ�����";cin >> FileName4;
	//���򿪵��ļ������ݴ洢��˳����У����ȱ����Ա�������ɼ����в�¼���ͷ�˳���p1��p2
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);GradeRecord(p1, p2, FileName3, FileName4);free(p1), free(p2);
}
//��������ɾ�����ݣ���ѧ�ţ�������ɾ��ָ����¼��
void f6()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "������Ҫɾ���ĳɼ����ļ�����"; cin >> FileName1;
	cout << "������ɾ���������ļ�����"; cin >> FileName2;
	cout << "��������Ӧ��������ļ�����";cin >> FileName3;
	ListWrite(p1, FileName1);ListWrite1(p2, FileName2);   //���򿪵��ļ������ݴ洢��˳�����
	int index = 0;
	for (int i = 0;i < p1->length;i++)
	{
		int sign = 1;
		for (int j = 0;j < p2->length;j++)
		{
			//�ҵ�Ҫɾ�������ݵ�λ��
			if (p1->data[i].xh[6] == p2->data[j].xh[6] && p1->data[i].xh[7] == p2->data[j].xh[7] && p1->data[i].xh[11] == p2->data[j].xh[11] && p1->data[i].xh[12] == p2->data[j].xh[12])
			{
				sign = 0;
				break;
			}
		}
		if (sign)
		{
			p1->data[index] = p1->data[i];      //��˳���p1���¸�ֵ��ʵ��ɾ������
			index++;
		}
	}
	p1->length -= p2->length;
	DispList3(p1, FileName3);            //������ݵ��ĵ����ͷ�˳���p1��p2
	free(p1);free(p2);
}
//�����ߣ��ܰ��ɼ��ֽܷ�������Ȼ���ٸ������Σ���������У��������������ͬ������ͬ��������������ڶ�������û�е������ˣ�
void f7()
{
	List* p = (List*)malloc(sizeof(List));p->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout << "������Ҫ�򿪵��ļ�����"; cin >> FileName1;
	cout << "������Ҫ������ļ�����"; cin >> FileName2;
	ListWrite(p, FileName1);                  //���򿪵��ļ������ݴ洢��˳�����
	ListSort2(p);DispList2(p, FileName2);free(p);  //���������������
}
//���ܰˣ��ְർ�����ݣ��ܿ���ѧ��������Գɼ����ܷ�����������ܰ������������н������У�����ϴο��ԣ�
void f8()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];char FileName4[MAXSIZE];
	cout << "�������һ�γɼ����ļ�����";cin >> FileName1;
	cout << "������ڶ��γɼ����ļ�����";cin >> FileName2;
	cout << "�������˹����ܰ��ļ�����";cin >> FileName3;
	cout << "�������˴����ļ�����";cin >> FileName4;
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);
	ListSort1(p1);ListSort1(p2);
	List1* p3 = (List1*)malloc(sizeof(List1));p3->length = p1->length;int index = 0;
	for (int i = 0;i < p1->length;i++)
	{
		int sign = 0;
		for (int j = 0;j < p2->length;j++)
		{
			if (p1->data[i].xh[6] == p2->data[j].xh[6] && p1->data[i].xh[7]==p2->data[j].xh[7] && p1->data[i].xh[11] == p2->data[j].xh[11] && p1->data[i].xh[12] == p2->data[j].xh[12])
			{
				sign = 1;
				strcpy(p3->data[index].name, p1->data[i].name);strcpy(p3->data[index].xh, p1->data[i].xh);
				p3->data[index].GradeSum1 = p1->data[i].sum;p3->data[index].GradeSum2 = p2->data[j].sum;p3->data[index].GradeChange = p2->data[j].sum - p1->data[i].sum;
				index++;
			}
		}
		if (!sign)
		{
			strcpy(p3->data[index].name, p1->data[i].name);strcpy(p3->data[index].xh, p1->data[i].xh);
			p3->data[index].GradeSum1 = p1->data[i].sum;p3->data[index].GradeSum2 = 0;p3->data[index].GradeChange = - p1->data[i].sum;
			index++;
		}
	}
	sort(p3->data, p3->data + p3->length, cmp1);
	int index1 = 0, index2 = 0;
	for (int i = 0;i < p3->length;i++)
	{
		if(p3->data[i].xh[6]=='5')
			p3->data[i].rank = ++index1;
		else
			p3->data[i].rank = ++index2;
		if (i)
		{
			if (p3->data[i - 1].GradeChange == p3->data[i].GradeChange && p3->data[i-1].xh[6]==p3->data[i].xh[6])
				p3->data[i].rank = p3->data[i - 1].rank;
		}
	}
	FILE* fp;
	if ((fp = fopen(FileName3, "w")) == NULL)
	{
		cout << "�ļ�����ʧ��";
		exit(0);
	}
	fprintf(fp, "�˹����ܰ�ɼ��仯ͳ�ƣ�\n");
	fprintf(fp, "\tѧ������                ѧ��     \t��һ�γɼ��ܷ�\t    �ڶ��γɼ��ܷ�\t    �ɼ�����\t    �ɼ���������\n");
	for (int i = 0;i < p3->length;i++)
	{
		if (p3->data[i].xh[6] == '5')
		{
			fprintf(fp, "\t %s", p3->data[i].name);
			int mid = NameLength(p3->data[i].name);
			for (int i = 0;i < 20 - mid;i++)
				fprintf(fp, " ");
			fprintf(fp, "%s", p3->data[i].xh);
			fprintf(fp, "\t\t  %3d\t\t\t\t  %3d\t\t\t   %4d\t\t\t  %3d",p3->data[i].GradeSum1,p3->data[i].GradeSum2,p3->data[i].GradeChange,p3->data[i].rank);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	FILE* fp1;
	if ((fp1 = fopen(FileName4, "w")) == NULL)
	{
		cout << "�ļ�����ʧ��";
		exit(0);
	}
	fprintf(fp1, "�˴���ɼ��仯ͳ�ƣ�\n");
	fprintf(fp1, "\tѧ������                ѧ��     \t��һ�γɼ��ܷ�\t    �ڶ��γɼ��ܷ�\t    �ɼ�����\t    �ɼ���������\n");
	for (int i = 0;i < p3->length;i++)
	{
		if (p3->data[i].xh[6] == '6')
		{
			fprintf(fp1, "\t %s", p3->data[i].name);
			int mid = NameLength(p3->data[i].name);
			for (int i = 0;i < 20 - mid;i++)
				fprintf(fp1, " ");
			fprintf(fp1, "%s", p3->data[i].xh);
			fprintf(fp1, "\t\t  %3d\t\t\t\t  %3d\t\t\t   %4d\t\t\t  %3d", p3->data[i].GradeSum1, p3->data[i].GradeSum2, p3->data[i].GradeChange, p3->data[i].rank);
			fprintf(fp1, "\n");
		}
	}
	fclose(fp1);

}
//���ܾţ���ѯ�ҿ���Ա������������(85������)����
void f9()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout << "������Ҫ��ѯ�ĳɼ����ļ�����";cin >> FileName1;
	cout << "��������Ӧ��������ļ�����";cin >> FileName2;
	ListWrite(p1, FileName1);set<string> set11[6], set22[6];
	for (int i = 0;i < p1->length;i++)
	{
		for (int j = 0;j < 6;j++)
		{
			if (p1->data[i].grade[j] < 60)
				set11[j].insert(p1->data[i].name);
			if (p1->data[i].grade[j] > 85)
				set22[j].insert(p1->data[i].name);
		}
	}
	FILE* fp;
	if ((fp = fopen(FileName2, "w")) == NULL)
	{
		cout << "�ļ�����ʧ��";
		exit(0);
	}
	fprintf(fp, "\t�ҿ���Ա������\n");
	fprintf(fp, "\t�ߵ���ѧ��");
	for (auto i : set11[0])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧӢ�");
	for (auto i : set11[1])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ����");
	for (auto i : set11[2])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ������");
	for (auto i : set11[3])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t���Դ�����");
	for (auto i : set11[4])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ���ģ�");
	for (auto i : set11[5])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t������Ա������\n");
	fprintf(fp, "\t�ߵ���ѧ��");
	for (auto i : set22[0])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧӢ�");
	for (auto i : set22[1])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ����");
	for (auto i : set22[2])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ������");
	for (auto i : set22[3])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t���Դ�����");
	for (auto i : set22[4])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t��ѧ���ģ�");
	for (auto i : set22[5])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fclose(fp);
	free(p1);
}
int main()
{
	int flag = 1, sz;
	while (flag)
	{
		system("cls");          //����
		cout << "========================================\n";
		cout << "||         ѧ���ɼ�����ϵͳ V1        ||\n";
		cout << "========================================\n";
		cout << "|| 1.���ɼ��ܷ��ɸߵ�����ʾ���м�¼   ||\n";
		cout << "|| 2.ͳ�ƻ���                         ||\n";
		cout << "|| 3.��ѯ����                         ||\n";
		cout << "|| 4.�޸�����                         ||\n";
		cout << "|| 5.�ɼ���¼                         ||\n";
		cout << "|| 6.ɾ������                         ||\n";
		cout << "|| 7.��������                         ||\n";
		cout << "|| 8.��������                         ||\n";
		cout << "|| 9.��ѯ�ҿơ����������             ||\n";
		cout << "|| 10.�˳�                            ||\n";
		cout << "========================================\n";
		cout << "����������ѡ��1-10����";cin >> sz;
		switch (sz)
		{
			case 1: { f1();cout << "�ɼ��ܷ��ѳɹ�����,������Ӧ�ļ��鿴���\n";break; }
			case 2: { f2();cout << "�ɼ���ͳ����ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 3: { f3();cout << "��ѯ��ϣ�������Ӧ�ļ��鿴���\n";break ; }
			case 4: { f4();cout << "�޸���ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 5: { f5();cout << "��¼��ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 6: { f6();cout << "ɾ����ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 7: { f7();cout << "������ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 8: { f8();cout << "������ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 9: { f9();cout << "��ѯ��ϣ�������Ӧ�ļ��鿴���\n";break; }
			case 10:
			default:
			{
				cout << "���ѳɹ��˳���ллʹ��";flag = 0;break;
			}
		}
		Sleep(2000);       //��ʱ2�룬���û��ܿ�������
	}
	return 0;
}
