#include <cstdio>
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <cstring>

using namespace std;

const int iavi = 16;//����һ����ֵ����������������ƽ�ƣ���ȡ���صĺ���λ�� 
const int LODATA = 200;//���г��ȣ�ȡ����һ�η��͵����ݵ�������� 
const int hokey = 650;//��ȡ���صĸ߶ȣ�ȡ���ڴ��ڣ������и�����Ļ�ֱ��ʸ��� 

HWND desktop,hd1,hd2;
RECT rectod,rect1,rect2;
int wos,hos;
char strTitle[255];

float wokey;
int position[88];

int noc;

struct nodec{
	int r;
	int g;
	int b;
};
nodec color[100];

HDC hdc;
int red,green,blue;
COLORREF clr;

HANDLE hCom;
DCB lpTest;
DWORD btsIO;

int ch;
int op,cl;
int data[LODATA];
char commu[1];

bool flags[88];


void pta (int a){
	cl++;
	if (cl >= LODATA){
		cl -= LODATA;
	}
	data[cl] = a;
}
void push(){
	if (op != cl){
		++op;
		if (op >= LODATA){
			op -= LODATA;
		}
		commu[0] = data[op];
		WriteFile(hCom,commu,strlen(commu),&btsIO,NULL);
		printf ("%d	",data[op]);
		Sleep(3);
		push();
	}
	else {
		Sleep(3);
		return;
	}
}
void effect1(int n){
	pta (90);
	push();
	Sleep(2);
	for (int i=1;i<=n;++i){
		pta (i);
		push();
		Sleep(5);
	}
	for (int i=n+1;i<=87;++i){
		pta (i);
		pta (i-n);
		push ();
		Sleep (5);
	}
	for (int i=88-n;i<= 87;++i){
		pta (i);
		push ();
		Sleep (5);
	}
	pta (91);
	push();
	Sleep(2);
}
//ע������̨����ȫ��Ӣ���Է�ֹ���ڱ��벻ͬ�������������� 

int main (){
	
	desktop = GetDesktopWindow ();
	GetWindowRect(desktop,&rectod);
	printf ("Desktop: %d * %d\n",rectod.right - rectod.left,rectod.bottom - rectod.top);
	//��ȡ����ֱ��� 
	
	hd1 = FindWindow(NULL,"Synthesia 10.1.3320");
	ShowWindow(hd1,SW_RESTORE);
	Sleep(100);
	SetForegroundWindow (hd1);
	GetWindowRect (hd1,&rect1);
	hos = rect1.bottom - rect1.top;
	wos = rect1.right - rect1.left;
	printf ("Syn: %d * %d\n",wos,hos);
	MoveWindow(hd1,0,0,wos,hos,false);
	//����Syn���� 
	
	wokey = (float)(wos-iavi)/89;
	for (int i=0;i<88;++i){
		position[i] = wokey * (i+1);
	} 
	printf ("Wide of key: %.2f\n",wokey);
	//����Syn�����м��̰���λ�� 
	
	GetConsoleTitle(strTitle, 255);
	hd2 = FindWindow("ConsoleWindowClass",strTitle);
	GetWindowRect (hd2,&rect2);
	MoveWindow (hd2,wos,0,rectod.right - wos,rectod.bottom,false);
	Sleep (100);
	SetForegroundWindow (hd2);
	//��������̨���� 
	
	freopen ("color.txt","r",stdin);
	scanf ("%d",&noc);
	for (int i=0;i<noc;++i){
		scanf ("%d%d%d",&color[i].r,&color[i].g,&color[i].b);
	}
	freopen ("CON","r",stdin);
	//��ȡ��ɫԤ�裨color.txt�� 
	printf ("%d colors got\nPress any key to continue\n",noc);
	
	while (!_kbhit()){
		Sleep (20);
	}
	Sleep (100);
	//���¼���������Կ�ʼ������Ļ����ʹ���ͨѶ 
	
	hdc = GetDC(hd1);
	
	hCom=CreateFile("COM3",//COM3���ڣ�������ͨ���豸��������ѯ�������ƣ� 
	GENERIC_READ|GENERIC_WRITE, //�������д
	0, //��ռ��ʽ
	0,
	OPEN_EXISTING, //�򿪶����Ǵ���
	0, //ͬ����ʽ
	0);
	//�򿪴��� 
	
	if (hCom != INVALID_HANDLE_VALUE){
		printf ("Serial connected\n");
		
        GetCommState(hCom,&lpTest);  //��ȡ��ǰ�Ĳ�������
        lpTest.BaudRate=CBR_19200;       //���ò�����
        lpTest.ByteSize=8;              //����λ��Ϊ8
        lpTest.Parity=NOPARITY;         //��У��
        lpTest.StopBits = ONESTOPBIT;   //1λֹͣλ
        SetCommState(hCom,&lpTest);  //����ͨ�Ų���
	}
	else {
		printf ("Serial connection failed\n");
		return 0;
	} 
	//��֤����״̬����δ���Ӵ������˳�������������� 
	
	while (1){
		if (_kbhit()){
			ch = _getch();
			if (ch == 27){
				break;
			}
			else {
				if (ch > 48 and ch < 58){
					pta (89);
					pta (color[ch-49].r);
					pta (color[ch-49].g);
					pta (color[ch-49].b);
					push();
					printf ("\nChange_Color:%d %d %d\n",color[ch-49].r,color[ch-49].g,color[ch-49].b);
				}
				else if (ch == 48){
					effect1(10);
					printf ("\nEffect1\n");
				}
				else if (ch == 45){
					//effect2();
					printf ("\nEffect2\n");
				}
				else if (ch == 61){
					//effect3();
					printf ("\nEffect3\n");
				}
			}
			
		}
		for (int i=0;i<88;++i){
			clr = GetPixel (hdc,position[i],hokey);
			red = GetRValue (clr);
			green = GetGValue (clr);
			blue = GetBValue (clr);
			if (green - blue - red > 0){
				if (flags[i] == false){
					pta (i+1);
					flags[i] = true;
				}
			}
			else if (flags[i] == true){
				pta (i+1);
				flags[i] = false;
			}
		}
		push();
		Sleep (3);
	}
	//�������岿�� 
	
	
	return 0;
} 
