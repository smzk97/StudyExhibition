#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <set>
#include <cstring>
#include <vector>
#define MAXSIZE 100
using namespace std;
set<string> set1,set2,set3,set4;      //宏定义set函数，用于缺考人员的查找
//计算姓名长度
int NameLength(char s[])
{
	int i=0;
	while (s[i] != '\0')
		i++;
	return i+1;
} 
//构造一个学生成绩存储结构体
typedef struct
{
	char name[MAXSIZE];
	char xh[MAXSIZE];
	int grade[MAXSIZE];
	int sum;
	int SchRank;
	int ClaRank;
}student;
//构造一个学生成绩变化情况结构体
typedef struct
{
	char name[MAXSIZE];
	char xh[MAXSIZE];
	int GradeSum1;
	int GradeSum2;
	int GradeChange;
	int rank;
}student1;
//构造一个顺序表来存储数据
typedef struct
{
	student data[MAXSIZE];
	int length;
}List;
//构造另外一个顺序表用来存储成绩变化数据
typedef struct
{
	student1 data[MAXSIZE];
	int length;
}List1;
//自定义排序，实现成绩降序排列
bool cmp(student a,student b)
{
	return a.sum > b.sum;
}
//自定义排序，实现成绩变化的降序排列
bool cmp1(student1 a, student1 b)
{
	return a.GradeChange > b.GradeChange;
}
//进行文件的读入操作,将数据存储到顺序表中
void ListWrite(List*& s, char* file)
{
	FILE* fp; int i = 0;
	//用于文件读入错误情形的反馈
	if ((fp = fopen(file, "r")) == NULL)
	{
		cout<<"文件读入错误";
		exit(0);
	}
	//文件读入成功，分块对数据进行录入
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
//进行文件的读入操作，针对文件只有姓名和学号等数据的文件
void ListWrite1(List*& s, char* file)
{
	FILE* fp;int i = 0;
	if ((fp = fopen(file, "r")) == NULL)
	{
		cout << "读入文件错误";
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
//对顺序表总分进行相应的排序
void ListSort1(List*& s)
{
	for (int i = 0;i < s->length;i++)         //计算总分
	{
		int save = 0;
		for (int j = 0;j < 6;j++)
			save += s->data[i].grade[j];
		s->data[i].sum = save;
	}
	sort(s->data, s->data + s->length, cmp);
}
//对顺序表总分、校排名，班排名进行排序
void ListSort2(List*& s)
{
	for (int i = 0;i < s->length;i++)         //计算总分
	{
		int save = 0;
		for (int j = 0;j < 6;j++)
			save += s->data[i].grade[j];
		s->data[i].sum = save;
	}
	sort(s->data, s->data + s->length, cmp); //对总分进行降序排序
	for (int i = 0;i < s->length;i++)        //对校排名进行排序
	{
		s->data[i].SchRank = i + 1;
		if (i)
		{
			if (s->data[i - 1].sum == s->data[i].sum)
				s->data[i].SchRank = s->data[i - 1].SchRank;     //如果分数相同，则排名也相同
		}
	}
	int sign1 = 0, sign2 = 0;
	for (int i = 0;i < s->length;i++)        //对班排名进行排序
	{
		if (s->data[i].xh[6] == '5')         //通过学号第7位对不同班进行区分
			s->data[i].ClaRank = ++sign1;
		else
			s->data[i].ClaRank = ++sign2;
		if (i)
		{
			if (s->data[i - 1].sum == s->data[i].sum && s->data[i].xh[6]==s->data[i-1].xh[6])   //如果分数相同，则排名也相同
				s->data[i].ClaRank = s->data[i - 1].ClaRank;
		}
	}

}
//将顺序表的数据输出到文档中
void DispList1(List* s, char* file)
{
	FILE* fp;
	if ((fp = fopen(file, "w")) == NULL)
	{
		cout << "读入文件错误";
		exit(0);
	}
	fprintf(fp, "   学生姓名                学号     \t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t总分\t\n");      //打印输出相关信息到文档中
	for (int j = 0;j < s->length;j++)
	{
		fprintf(fp, "    %s", s->data[j].name);           //打印姓名
		int mid = NameLength(s->data[j].name);
		for (int i = 0;i < 20 - mid;i++)
			fprintf(fp, " ");                             //进行格式上的对齐
		fprintf(fp, "%s", s->data[j].xh);
		for (int i = 0;i < 6;i++)
			fprintf(fp, "\t   %3d\t", s->data[j].grade[i]);  //打印各科成绩
		fprintf(fp, "\t%d", s->data[j].sum);                 //打印总分
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//将顺序表的数据输出到文档中，特别包括校排名以及班排名
void DispList2(List* s,char* file)
{
	FILE* fp;
	if((fp=fopen(file,"w"))==NULL)
	{
		cout<<"读入文件错误";
		exit(0);
	}
	fprintf(fp,"   学生姓名                学号     \t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t总分\t校排名\t班排名\n");
	for(int j=0;j<s->length;j++)
	{
		fprintf(fp,"    %s",s->data[j].name);          //打印姓名
		int mid=NameLength(s->data[j].name);
		for(int i=0;i<20-mid;i++)
			fprintf(fp," ");                           //进行格式上的对齐
		fprintf(fp,"%s",s->data[j].xh);
		for(int i=0;i<6;i++)
			fprintf(fp,"\t   %3d\t",s->data[j].grade[i]);   //打印各科成绩
		fprintf(fp,"\t%3d\t\t  %2d\t  %2d",s->data[j].sum,s->data[j].SchRank,s->data[j].ClaRank);   //打印总分、校排名，班排名
		fprintf(fp,"\n");
	}
	fclose(fp);
}
//将顺序表的数据输出到文档中，只包括姓名、学号、成绩
void DispList3(List* s, char* file)
{
	FILE* fp;
	if ((fp = fopen(file, "a+")) == NULL)
	{
		cout << "读入文件错误";
		exit(0);
	}
	for (int j = 0;j < s->length;j++)
	{
		fprintf(fp, "%s", s->data[j].name);             //打印姓名
		int mid = NameLength(s->data[j].name);
		for (int i = 0;i < 20 - mid;i++)
			fprintf(fp, " ");                          //进行格式上的对齐
		fprintf(fp, "%s", s->data[j].xh);
		for (int i = 0;i < 6;i++)
			fprintf(fp, "\t%3d", s->data[j].grade[i]);  //打印各科成绩
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//统计第一次校、班的各科平均分，最低分，最高分，不及格人数
void Include1(List* p1,char FileName1[],char FileName3[])
{
	/*save1用来存储全校所有人的各科的总分，save2，save3用来存储各班所有人各科的总分，sum2用来存全校各科的最高分，sum22，sum222用来存储各班各科的最高分
	sum3用来存全校各科的最低分，sum33，sum333用来存储各班各科的最低分，sum4用来存全校各科不及格人数，sum44，sum444用来存各班各科不及格人数             
	index1，index2用来统计各班的人数                                                                                                                                              */
	double save1[6] = { 0 }, save2[6] = { 0 }, save3[6] = { 0 };int sum2[6] = { 101,101,101,101,101,101 }, sum22[6] = { 101,101,101,101,101,101 }, sum222[6] = { 101,101,101,101,101,101 },
	sum3[6] = { 0 }, sum33[6] = { 0 }, sum333[6] = { 0 },sum4[6] = { 0 }, sum44[6] = { 0 }, sum444[6] = { 0 },index1=0,index2=0;
	for (int i = 0;i < p1->length;i++)
	{
		set1.insert(p1->data[i].name);               //将第一次成绩学生姓名存到set容器中
		if (p1->data[i].xh[6] == '5')
			index1++;                               //计算人工班人数
		else
			index2++;                               //计算人创班人数
		for (int j = 0;j < 6;j++)
		{
			save1[j] += p1->data[i].grade[j];        //计算全校总分
			if (sum2[j] > p1->data[i].grade[j])
				sum2[j] = p1->data[i].grade[j];     //计算全校各科最高分
			if (sum3[j] < p1->data[i].grade[j])
				sum3[j] = p1->data[i].grade[j];     //计算全校各科最低分
			if (p1->data[i].grade[j] < 60)
				sum4[j]++;                          //计算全校各科不及格人数
			if (p1->data[i].xh[6] == '5')          
			{
				set2.insert(p1->data[i].name);      //将人工班的学生姓名存到set容器中
				save2[j] += p1->data[i].grade[j];   //计算人工班各科总分
				if (sum22[j] > p1->data[i].grade[j])
					sum22[j] = p1->data[i].grade[j]; //计算人工班各科最高分
				if (sum33[j] < p1->data[i].grade[j])
					sum33[j] = p1->data[i].grade[j]; //计算人工班各科最低分
				if (p1->data[i].grade[j] < 60)
					sum44[j]++;                      //计算人工班不及格人数
			}
			else
			{
				set3.insert(p1->data[i].name);       //将人创班的学生姓名存到set容器中
				save3[j] += p1->data[i].grade[j];    //计算人创班各科总分
				if (sum222[j] > p1->data[i].grade[j])
					sum222[j] = p1->data[i].grade[j]; //计算人创班各科最高分
				if (sum333[j] < p1->data[i].grade[j])
					sum333[j] = p1->data[i].grade[j]; //计算人创班各科最低分
				if (p1->data[i].grade[j] < 60)
					sum444[j]++;                      //计算人创班不及格人数
			} 
		}
	}
	for (int i = 0;i < 6;i++)
		save1[i] /= p1->length, save2[i] /= index1, save3[i] /= index2;  //计算平均分
	//将计算好的数据输出到文件中
	FILE* fp1;
	if ((fp1 = fopen(FileName3, "a+")) == NULL)
	{
		cout << "读入文件错误";
		exit(0);
	}
	fprintf(fp1, "\t第一次考试全校统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n",save1[0], save1[1], save1[2], save1[3], save1[4], save1[5] );
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n",sum2[0], sum2[1], sum2[2], sum2[3], sum2[4], sum2[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n",sum3[0], sum3[1], sum3[2], sum3[3], sum3[4], sum3[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n",sum4[0], sum4[1], sum4[2], sum4[3], sum4[4], sum4[5]);
	fprintf(fp1, "\t人工智能班统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save2[0], save2[1], save2[2], save2[3], save2[4], save2[5]);
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum22[0], sum22[1], sum22[2], sum22[3], sum22[4], sum22[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum33[0], sum33[1], sum33[2], sum33[3], sum33[4], sum33[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n", sum44[0], sum44[1], sum44[2], sum44[3], sum44[4], sum44[5]);
	fprintf(fp1, "\t人创班统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save3[0], save3[1], save3[2], save3[3], save3[4], save3[5]);
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum222[0], sum222[1], sum222[2], sum222[3], sum222[4], sum222[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum333[0], sum333[1], sum333[2], sum333[3], sum333[4], sum333[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n\n", sum444[0], sum444[1], sum444[2], sum444[3], sum444[4], sum444[5]);
	fclose(fp1);
}
//统计第二次校、班的各科平均分，最低分，最高分，不及格人数，跟上一个函数雷同
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
		cout << "读入文件错误";
		exit(0);
	}
	fprintf(fp1, "\t第二次考试全校统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save1[0], save1[1], save1[2], save1[3], save1[4], save1[5]);
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum2[0], sum2[1], sum2[2], sum2[3], sum2[4], sum2[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum3[0], sum3[1], sum3[2], sum3[3], sum3[4], sum3[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum4[0], sum4[1], sum4[2], sum4[3], sum4[4], sum4[5]);
	fprintf(fp1, "\t缺考名单：");
	for (auto i : set1)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fprintf(fp1, "\t人工智能班统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save2[0], save2[1], save2[2], save2[3], save2[4], save2[5]);
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum22[0], sum22[1], sum22[2], sum22[3], sum22[4], sum22[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum33[0], sum33[1], sum33[2], sum33[3], sum33[4], sum33[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum44[0], sum44[1], sum44[2], sum44[3], sum44[4], sum44[5]);
	fprintf(fp1, "\t缺考名单：");
	for (auto i : set2)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fprintf(fp1, "\t人创班统计：\n");
	fprintf(fp1, "\t\t\t高等数学\t大学英语\t大学物理\t大学体育\t线性代数\t大学语文\t\n");
	fprintf(fp1, "\t平均分：%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f   \t%5.2f\n", save3[0], save3[1], save3[2], save3[3], save3[4], save3[5]);
	fprintf(fp1, "\t最低分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum222[0], sum222[1], sum222[2], sum222[3], sum222[4], sum222[5]);
	fprintf(fp1, "\t最高分：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum333[0], sum333[1], sum333[2], sum333[3], sum333[4], sum333[5]);
	fprintf(fp1, "\t不及格：%5d   \t%5d   \t%5d   \t%5d   \t%5d   \t%5d\n", sum444[0], sum444[1], sum444[2], sum444[3], sum444[4], sum444[5]);
	fprintf(fp1, "\t缺考名单：");
	for (auto i : set3)
		fprintf(fp1, "%s  ", i.c_str());
	fprintf(fp1, "\n\n");
	fclose(fp1);
}
//成绩补录
void GradeRecord(List* s1, List* s2, char FileName1[], char FileName2[])
{
	List* h = (List*)malloc(sizeof(List));h->length = 0;ListWrite(h, FileName1);     //将补录的成绩存储到循序表h中
	//用来对缺考人员的查找
	for (int i = 0;i < s1->length;i++)
		set4.insert(s1->data[i].name);
	for (int j = 0;j < s2->length;j++)
		set4.erase(s2->data[j].name);
	FILE* fp;
	if ((fp = fopen(FileName2, "w")) == NULL)
	{
		cout << "文件读入错误\n";
		exit(0);
	}
	//打印缺考人员名单
	fprintf(fp, "缺考人员名单：");
	for (auto i : set4)
		fprintf(fp,"%s  ", i.c_str());        //这里需要注意，%s传入的是char* 类型的数据，而set4里面的数据是string类型，需要通过c_str转化为c语言风格的字符串
	fprintf(fp, "\n补录成绩如下：\n");
	fclose(fp);
	DispList3(h, FileName2);                  //将补录完后的所有成绩重新打印
	free(h);
}
//功能一：按成绩总分由高到低显示所有记录
void f1()
{
	List* p = (List*)malloc(sizeof(List));p->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout<<"请输入要打开的文件名："; cin>>FileName1;
	cout<<"请输入要输出的文件名："; cin>>FileName2;
	ListWrite(p, FileName1);               //将打开的文件的数据存储到顺序表中
	ListSort1(p);DispList1(p, FileName2);free(p); //排序总分，输出数据，释放顺序表
}
//功能二：统计汇总（即两次考试中，校，班的各科平均分，最高分，最低分，不及格人数，第二次考试缺考名单等所有数据）
void f2()
{
	List* p1= (List *)malloc(sizeof(List));p1->length = 0;List* p2= (List *)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "请输入第一次成绩的文件名："; cin >> FileName1;
	cout << "请输入第二次成绩的文件名："; cin >> FileName2;
	cout << "请输入相应的输出文件名：";cin >> FileName3;
	//将打开的文件的数据存储到顺序表p1，p2中，统计汇总各项数据，再将数据进行输出，最后释放顺序表
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);Include1(p1, FileName1, FileName3);Include2(p2, FileName2, FileName3);free(p1);free(p2);
}
//功能三：查询数据（即输入姓名或学号可查询该生的所有成绩与排名，最好支持模糊查询，即输入“李”可查询所有名字中含有“李”字学生的成绩；）
void f3()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "请输入要查询的成绩的文件名：";cin >> FileName1;
	cout << "请输入要查询的名字关键词：";cin >> FileName2;
	cout << "请输入相应的输出文件名：";cin >> FileName3;
	ListWrite(p1, FileName1);ListSort2(p1);         //将打开的文件的数据存储到顺序表中，对总分、排名进行排序
	char* index1;char* index2;vector<int> sum(MAXSIZE);int index = 0;   //使用双指针进行字符串的子字符串的查找
	for (int i = 0;i < p1->length;i++)
	{
		index1 = FileName2;                        //使用变量index1记录用户输入的关键词
		index2 = p1->data[i].name;                 //使用变量index2记录文件中存在的姓名
		while (*index2 != '\0' && *index1 != '\0')
		{
			if (*index1 == *index2)                //如果关键词的某一个字符和姓名中的某一个字符相同，将两个字符都递增到下一个字符，直到遇到结束符'\0'
				index1++, index2++;
			else
				index2++;                          //如果不同，只需要指向姓名的字符指针index2向下移动
		}
		if (*index1 == '\0' && *index2 != '\0' || *index1 == '\0' && *index2 == '\0')     //如果找到子字符串，记录顺序表中data数组的下标，即第几个学生
			sum[index++] = i;
	}
	List* p3 = (List*)malloc(sizeof(List));p3->length = index;
	for (int i = 0;i < index;i++)
	{
		strcpy(p3->data[i].name, p1->data[sum[i]].name);strcpy(p3->data[i].xh, p1->data[sum[i]].xh);    //将找到的子字符串对应的姓名、学号、总分、成绩、班校排名赋值给顺序表p3
		p3->data[i].sum = p1->data[sum[i]].sum;p3->data[i].SchRank = p1->data[sum[i]].SchRank;p3->data[i].ClaRank = p1->data[sum[i]].ClaRank;
		for (int j = 0;j < 6;j++)
			p3->data[i].grade[j] = p1->data[sum[i]].grade[j];
	}
	DispList2(p3, FileName3);free(p1);free(p3);                 //将查询到的数据进行输出，释放顺序表p1、p3
}
//功能四：修改数据
void f4()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;List* p3 = (List*)malloc(sizeof(List));p3->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "请输入要修改的成绩的文件名：";cin >> FileName1;
	cout << "请输入更改后的数据的文件名：";cin >> FileName2;
	cout << "请输入相应的输出文件名：";cin >> FileName3;
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);        //将打开的文件的数据存储到顺序表中
	for (int i = 0;i < p2->length;i++)
	{
		for (int j = 0;j < p1->length;j++)
		{
			//通过学号查找到删除的数据的位置，对顺序表进行修改
			if (p1->data[j].xh[6] == p2->data[i].xh[6] && p1->data[j].xh[7]==p2->data[i].xh[7] && p1->data[j].xh[11]==p2->data[i].xh[11] && p1->data[j].xh[12]==p2->data[i].xh[12])
			{
				for (int k = 0;k < 6;k++)
					p1->data[j].grade[k] = p2->data[i].grade[k];
			}
		}
	}
	DispList3(p1, FileName3);free(p1);free(p2);        //输出数据到文档中，释放顺序表p1、p2
}
//功能五：成绩补录
void f5()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];char FileName4[MAXSIZE];
	cout << "请输入第一次成绩的文件名："; cin >> FileName1;
	cout << "请输入第二次成绩的文件名："; cin >> FileName2;
	cout << "请输入要补录的数据的文件名：";cin >> FileName3;
	cout << "请输入相应的输出文件名：";cin >> FileName4;
	//将打开的文件的数据存储到顺序表中，输出缺考人员名单，成绩进行补录，释放顺序表p1、p2
	ListWrite(p1, FileName1);ListWrite(p2, FileName2);GradeRecord(p1, p2, FileName3, FileName4);free(p1), free(p2);
}
//功能六：删除数据（按学号，或按姓名删除指定记录）
void f6()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];
	cout << "请输入要删除的成绩的文件名："; cin >> FileName1;
	cout << "请输入删除名单的文件名："; cin >> FileName2;
	cout << "请输入相应的输出的文件名：";cin >> FileName3;
	ListWrite(p1, FileName1);ListWrite1(p2, FileName2);   //将打开的文件的数据存储到顺序表中
	int index = 0;
	for (int i = 0;i < p1->length;i++)
	{
		int sign = 1;
		for (int j = 0;j < p2->length;j++)
		{
			//找到要删除的数据的位置
			if (p1->data[i].xh[6] == p2->data[j].xh[6] && p1->data[i].xh[7] == p2->data[j].xh[7] && p1->data[i].xh[11] == p2->data[j].xh[11] && p1->data[i].xh[12] == p2->data[j].xh[12])
			{
				sign = 0;
				break;
			}
		}
		if (sign)
		{
			p1->data[index] = p1->data[i];      //对顺序表p1重新赋值，实现删除功能
			index++;
		}
	}
	p1->length -= p2->length;
	DispList3(p1, FileName3);            //输出数据到文档，释放顺序表p1、p2
	free(p1);free(p2);
}
//功能七：能按成绩总分降序排列然后再赋与名次（班排名与校排名），分数相同名次相同。（如果有两个第二名，则没有第三名了）
void f7()
{
	List* p = (List*)malloc(sizeof(List));p->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout << "请输入要打开的文件名："; cin >> FileName1;
	cout << "请输入要输出的文件名："; cin >> FileName2;
	ListWrite(p, FileName1);                  //将打开的文件的数据存储到顺序表中
	ListSort2(p);DispList2(p, FileName2);free(p);  //排序完再输出数据
}
//功能八：分班导出数据，能看到学生最近考试成绩的总分升降情况，能按进步排名进行降序排列（相对上次考试）
void f8()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;List* p2 = (List*)malloc(sizeof(List));p2->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];char FileName3[MAXSIZE];char FileName4[MAXSIZE];
	cout << "请输入第一次成绩的文件名：";cin >> FileName1;
	cout << "请输入第二次成绩的文件名：";cin >> FileName2;
	cout << "请输入人工智能班文件名：";cin >> FileName3;
	cout << "请输入人创班文件名：";cin >> FileName4;
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
		cout << "文件读入失败";
		exit(0);
	}
	fprintf(fp, "人工智能班成绩变化统计：\n");
	fprintf(fp, "\t学生姓名                学号     \t第一次成绩总分\t    第二次成绩总分\t    成绩升降\t    成绩升降排名\n");
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
		cout << "文件读入失败";
		exit(0);
	}
	fprintf(fp1, "人创班成绩变化统计：\n");
	fprintf(fp1, "\t学生姓名                学号     \t第一次成绩总分\t    第二次成绩总分\t    成绩升降\t    成绩升降排名\n");
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
//功能九：查询挂科人员名单，优秀率(85分以上)名单
void f9()
{
	List* p1 = (List*)malloc(sizeof(List));p1->length = 0;
	char FileName1[MAXSIZE];char FileName2[MAXSIZE];
	cout << "请输入要查询的成绩的文件名：";cin >> FileName1;
	cout << "请输入相应的输出的文件名：";cin >> FileName2;
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
		cout << "文件读入失败";
		exit(0);
	}
	fprintf(fp, "\t挂科人员名单：\n");
	fprintf(fp, "\t高等数学：");
	for (auto i : set11[0])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学英语：");
	for (auto i : set11[1])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学物理：");
	for (auto i : set11[2])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学体育：");
	for (auto i : set11[3])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t线性代数：");
	for (auto i : set11[4])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学语文：");
	for (auto i : set11[5])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t优秀人员名单：\n");
	fprintf(fp, "\t高等数学：");
	for (auto i : set22[0])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学英语：");
	for (auto i : set22[1])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学物理：");
	for (auto i : set22[2])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学体育：");
	for (auto i : set22[3])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t线性代数：");
	for (auto i : set22[4])
		fprintf(fp, "%s   ", i.c_str());
	fprintf(fp, "\n");
	fprintf(fp, "\t大学语文：");
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
		system("cls");          //清屏
		cout << "========================================\n";
		cout << "||         学生成绩管理系统 V1        ||\n";
		cout << "========================================\n";
		cout << "|| 1.按成绩总分由高到低显示所有记录   ||\n";
		cout << "|| 2.统计汇总                         ||\n";
		cout << "|| 3.查询数据                         ||\n";
		cout << "|| 4.修改数据                         ||\n";
		cout << "|| 5.成绩补录                         ||\n";
		cout << "|| 6.删除数据                         ||\n";
		cout << "|| 7.名次排序                         ||\n";
		cout << "|| 8.导出数据                         ||\n";
		cout << "|| 9.查询挂科、优秀率情况             ||\n";
		cout << "|| 10.退出                            ||\n";
		cout << "========================================\n";
		cout << "请输入您的选择（1-10）：";cin >> sz;
		switch (sz)
		{
			case 1: { f1();cout << "成绩总分已成功排序,请点击相应文件查看结果\n";break; }
			case 2: { f2();cout << "成绩已统计完毕，请点击相应文件查看结果\n";break; }
			case 3: { f3();cout << "查询完毕，请点击相应文件查看结果\n";break ; }
			case 4: { f4();cout << "修改完毕，请点击相应文件查看结果\n";break; }
			case 5: { f5();cout << "补录完毕，请点击相应文件查看结果\n";break; }
			case 6: { f6();cout << "删除完毕，请点击相应文件查看结果\n";break; }
			case 7: { f7();cout << "排序完毕，请点击相应文件查看结果\n";break; }
			case 8: { f8();cout << "导出完毕，请点击相应文件查看结果\n";break; }
			case 9: { f9();cout << "查询完毕，请点击相应文件查看结果\n";break; }
			case 10:
			default:
			{
				cout << "您已成功退出，谢谢使用";flag = 0;break;
			}
		}
		Sleep(2000);       //延时2秒，让用户能看到反馈
	}
	return 0;
}
