#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<queue>
#include<iostream>
#include <search.h>

#define CITYNUM 20000
#define CITYNAME 100
#define MAXLINE 500
#define PI acos(-1.0)
//#define PI 3.14159265358979323846

using namespace std;

bool check[CITYNUM];

typedef struct Node;
typedef struct Data;
typedef struct QueueData;

typedef struct Node {
	int num;
	double weight;
	Node* Next;
}Node;

typedef struct Data {
	char* Name;
	double longitude;
	double latitude;
}Data;

typedef struct QueueData {
	int num;
	int hops;
}QueueData;

int CompareName(const void * a, const void * b) {
	Data* l = *(Data**)a;
	Data* r = *(Data**)b;
	return strcmp(l->Name, r->Name);
}

Data* newData(char* name) {
	Data* tmp = (Data*)malloc(sizeof(Data));
	tmp->Name = (char*)malloc(CITYNAME);
	strcpy(tmp->Name, name);

	return tmp;
}

Node* newNode(int num, double weight) {
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->num = num;
	tmp->weight = weight;
	tmp->Next = NULL;
	return tmp;
}

QueueData* newQueueData(int num, int hops) {
	QueueData* tmp = (QueueData*)malloc(sizeof(QueueData));
	tmp->num = num;
	tmp->hops = hops;
	return tmp;
}

void initNodeArray(Node* NA[]) {
	int i;
	for (i = 0; i < CITYNUM; i++)
		NA[i] = NULL;
}

void initBoolArray(bool ba[]) {
	for (int i = 0; i < CITYNUM; i++)
		ba[i] = false;
}

double deg2rad(double deg) {
	return (double)(deg * PI / (double)180);
}

double rad2deg(double rad) {
	return (double)(rad * (double)180 / PI);
}
// 매개변수는 첫번째 지점의 위도(lat1), 경도(lon1), 두번째 지점의 위도(lat2), 경도(lon2) 순서이다.
double calDistance(double lat1, double lon1, double lat2, double lon2)
{
	double theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2))
		+ cos(deg2rad(lat1))
		* cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	dist = dist * 1.609344; // 단위 mile 에서 km 변환.
	dist = dist * 1000.0; // 단위 km 에서 m 로 변환
	return dist;
}

int binarySearch(Data* arr[], int l, int r, char* key) {
	if (r < l)
		return -1;
	else {
		int mid = (l + r) / 2;
		//printf("%dmid : %s\n", mid,arr[mid]->Name);
		if (strcmp(arr[mid]->Name, key) == 0)
			return mid;
		else if (strcmp(arr[mid]->Name, key) > 0)
			return binarySearch(arr, l, mid - 1, key);
		else
			return binarySearch(arr, mid + 1, r, key);
	}
}

void BFS_Print_tenHops(Data* data[],Node* node[],int start) {
	queue<QueueData*> q;
	QueueData* qd = newQueueData(start, 0);
	int i = 0;
	q.push(qd);
	check[start] = true;
	while (!q.empty()) {
		QueueData* tmp = q.front();
		q.pop();
		printf("%d - %d Hops : ", i, tmp->hops);
		printf("%s  %f  %f\n", data[tmp->num]->Name, data[tmp->num]->latitude, data[tmp->num]->longitude);
		Node* nTmp = node[tmp->num];
		while (nTmp != NULL) {
			if (tmp->hops + 1 > 10)
				break;
			if (!check[nTmp->num]) {
				check[nTmp->num] = true;
				qd = newQueueData(nTmp->num, tmp->hops + 1);
				q.push(qd);
			}
			nTmp = nTmp->Next;
		}
		free(tmp);
		i++
	}
}

void DFS(Data* data[], Node* node[], int i, int &c) {
	check[i] = true;
	printf("%d %s  %f  %f\n",c, data[i]->Name, data[i]->latitude, data[i]->longitude);
	c++;
	Node* p = node[i];
	while (p != NULL) {
		i = p->num;
		if (!check[i])
			DFS(data, node, i, c);
		p = p->Next;
	}
}

void insertNode(Node *&root, int r, double w) {
	//NULL인 경우
	if (root == NULL) {
		root = newNode(r, w);
	}
	else {
		Node* tmp = root;
		while (tmp->Next != NULL)
			tmp = tmp->Next;
		tmp->Next = newNode(r, w);
	}
}

/*	parsing
int main ()
{
char str[] ="- This, a sample string.";
char * pch;
printf ("Splitting string \"%s\" into tokens:\n",str);
pch = strtok (str," ,.-");
while (pch != NULL)
{
printf ("%s\n",pch);
pch = strtok (NULL, " ,.-");
}
return 0;
}

Splitting string "- This, a sample string." into tokens:
This
a
sample
string
*/

int main()
{
	FILE* fi1=fopen("alabama.txt","r");
	Data* dList[CITYNUM];
	char* cLine = (char*)malloc(MAXLINE);
	char* pch;

	//Data Input
	int i = 0;
	while (fgets(cLine, MAXLINE, fi1) != NULL) {
		char *end;
		//printf("%s\n", cLine);
		pch = strtok(cLine, "	");
		dList[i] = newData(pch);
		//printf("aa");
		pch = strtok(NULL, "	");
		dList[i]->longitude = strtod(pch, &end);
		pch = strtok(NULL, "	");
		dList[i]->latitude = strtod(pch, &end);
		//printf("%s %f %f\n", dList[i]->Name, dList[i]->longitude, dList[i]->latitude);
		i++;
	}
	printf("i=%d\n", i);

	//sorting for binary search
	qsort(dList, i, sizeof(Data*), CompareName);

/*	for (int a = 0; a < i; a++) {
		printf("%s %f %f\n", dList[a]->Name, dList[a]->longitude, dList[a]->latitude);
	}*/
	//13528

	//printf("res = %d\n", strcmp(dList[13528]->Name, "Westfield Number 1 School (historical)"));
	
	FILE* fi2 = fopen("roadList2.txt", "r");
	Node* nList[CITYNUM];
	int l, r;
	double w=0;
	/*char q[] = "Marion County - Rankin Fite Airport";
	int t = binarySearch(dList, 0, i - 1, q);
	printf("\n%d\n", t);
	//for (int a = 9200; a < 9300; a++)
	//	printf("%d : %s\n", a, dList[a]->Name);*/

	//Make Adjacency list
	initNodeArray(nList);
	while (fgets(cLine, MAXLINE, fi2) != NULL) {
		char* end;
		pch = strtok(cLine, "	");
		//printf("%s | ", pch);
		l = binarySearch(dList, 0, i - 1, pch);
		pch = strtok(NULL, "	");
		pch[strlen(pch) - 1] = 0;
		//printf("%s\n", pch);
		r = binarySearch(dList, 0, i - 1, pch);
		//printf("%d %d\n", l, r);
		//printf("%f %f %f %f", dList[l]->latitude, dList[l]->longitude, dList[r]->latitude, dList[r]->longitude);
		w = calDistance(dList[l]->latitude, dList[l]->longitude, dList[r]->latitude, dList[r]->longitude);
		//printf("%d ", l);
		//printf("%d ", r);
		//printf("%f\n", w);
		insertNode(nList[l], r, w);
		insertNode(nList[r], l, w);
	}

	initBoolArray(check);
	printf("Enter the CityName to display the CityName within 10 hops of the City : ");
	gets_s(cLine, CITYNAME);
	BFS_Print_tenHops(dList, nList, binarySearch(dList, 0, i - 1, cLine));

	initBoolArray(check);
	printf("Enter the CityName for DFS : ");
	gets_s(cLine, CITYNAME);
	int c = 0;
	DFS(dList, nList, binarySearch(dList, 0, i - 1, cLine), c);
	//14378

	free(cLine);
	for (int a = 0; a < i; a++)
		free(dList[a]);
	for (int a = 0; a < i; a++) {
		if (nList)
			free(nList[a]);
	}
	fclose(fi1);
	fclose(fi2);
	return 0;
}

