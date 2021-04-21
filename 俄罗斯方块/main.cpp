#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<time.h>
#include<conio.h>
using namespace std;
int score=0;//ȫ�ֱ��� ����
int crank=0;//�ȼ�
#define BLOCK_COUNT 5 //���ַ���
#define BLOCK_WIDTH 5 //��
#define BLOCK_HEIGHT 5 //�߶� ��
#define UNIT_SIZE   20//С����ռ������
#define START_X 130
#define START_Y 30
#define UP 72  //���İ���˹��ֵ
#define DOWN 80
#define RIGHT 77//
#define LEFT 75
#define SPACE 32
int minx= 30;
int miny= 30;
typedef enum {//ö������
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT
}move_t;
typedef enum {//ö������
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t;
int speed=500;
int nextindex= -1;//��һ�����������
int blockindex= -1;//��ǰ���������
int color[BLOCK_COUNT]={//������ɫ
	GREEN,RED,CYAN,YELLOW,MAGENTA
};
int visit [30][15];//��������
int markcolor [30][15];//��ʾ��Ӧλ�õ���ɫ
int block [BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH]={//����ͼ��
	//l �ͷ���
	{0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},
	//L��
	{0,0,0,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,0,0,1,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,0,0,
	0,1,1,1,0,
	0,1,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,1,1,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,0,0,0},
	//����
	{0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,1,1,0,0,
	0,1,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},
	//T��
	{0,0,0,0,0,
	0,1,1,1,0,
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,1,0,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,0,1,0,
	0,0,1,1,0,
	0,0,0,1,0,
	0,0,0,0,0},
	//Z��
	{0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,1,1,0,0,
	0,0,1,1,0,
	0,0,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0},

	{0,0,0,0,0,
	0,0,1,0,0,
	0,1,1,0,0,
	0,1,0,0,0,
	0,0,0,0,0}	
};
//��ӭ����
void welcome(void){
	//��ʼ������
	initgraph(550,660);
	//���ô��ڱ���
	HWND window = GetHWnd();//��ȡ����
	//���ô��ڱ���
	SetWindowText(window,_T("����˹���� WDNMD NB"));
	//�����ı�������ʽ
	//�߶�  ���0(����Ĭ��) ����
	setfont(40,0,_T("΢���ź�"));
	setcolor(WHITE);//��ɫ
	outtextxy(205,200,_T("����˹����"));//����ָ��λ������ı�
	outtextxy(175,300,_T("��̴Ӷ���˹���鿪ʼ!"));
	Sleep(3000);//����3000���� 3��
}
//��ʼ����Ϸ����
void initgamescene(void){
	char str[16];//������ַ�������
	cleardevice();//�����Ļ
	//��������
	rectangle(27,27,336,635);//������(���ϵ�x,y  ���µ�x,y)
	rectangle(29,29,334,633);
	rectangle(370,50,515,195);
	setfont(24,0,_T("����"));//�����ı�������ʽ
	setcolor(LIGHTGRAY);//��ɫ
	outtextxy(405,215,_T("��һ��"));//����ָ��λ������ı�
	//����
	setcolor(RED);//��ɫ
	outtextxy(405,280,_T("����"));//����ָ��λ������ı�
	sprintf(str,"%d",score);//�ѷ���������ַ���������
	outtextxy(415,310,str);//�Ӳ���_T,������Ŀ���� �ַ���->ʹ�ö��ַ��ַ���
	outtextxy(405,375,_T("�ȼ�"));//����ָ��λ������ı�
	sprintf(str,"%d",crank);
	outtextxy(425,405,str);
	//����˵��
	setcolor(BLUE);
	outtextxy(390,475,"����˵��");
	outtextxy(390,500,"��:��ת");
	outtextxy(390,525,"��:�½�");
	outtextxy(390,550,"��:����");
	outtextxy(390,575,"��:����");
	outtextxy(390,600,"�ո�:��ͣ");
}
//�����������;
void clearblock(void){
	setcolor(BLACK);//��ɫ
	setfont(23,0,"����");
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//��
			int x=391+j*UNIT_SIZE;//��ʼ����+��*С����Ŀ��
			int y=71+i*UNIT_SIZE;//��ʼ����+��*С����Ŀ��
			outtextxy(x,y,"��");//ָ��λ���������
		}
	}
}
//����x ��������Ͻǵ�x����
//����y ��������Ͻ�����Ϸ�����ڵ�����,������Ϸ���򶥲��ľ���
void clearblock(int x,int y,block_dir_t dir){//�����������ķ���
	setcolor(BLACK);
	int id = blockindex * 4 + dir;
	y += START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j]==1){
				//�����÷����i��j��
				outtextxy(x+(20*j),y+i*20,"��");
			}
		}
	}
}
//���Ʒ�����ָ��λ�û���ָ�������ָ������ ( x,y �ĸ����� �ĸ�����)
void drawblock(int x,int y,int blokindex,block_dir_t dir){
	setcolor(color[blokindex]);//�����ɫ
	setfont(23,0,"����");
	int id = blockindex * 4 + dir;//��ǰ����*4+��ǰ����
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//"��"
			if(block[id][i][j]==1){//��������ÿ�ֿ�����ת4��*4,����=1�Ļ�ִ����һ��
				int x2= x + j * UNIT_SIZE;//��ʼ����+��*С����Ŀ��
				int y2= y + i * UNIT_SIZE;//��ʼ����+��*С����Ŀ��
				outtextxy(x2,y2,"��");//ָ��λ���������
			}
		}
	}
}
//�����Ͻ�������,������һ������
void drawblock(int x,int y){
	setcolor(color[nextindex]);//�����ɫ
	setfont(23,0,"����");
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//��
			if(block[nextindex*4][i][j]==1){//��������ÿ�ֿ�����ת4��*4,����=1�Ļ�ִ����һ��
				int x2=x+j*UNIT_SIZE;//��ʼ����+��*С����Ŀ��
				int y2=y+i*UNIT_SIZE;//��ʼ����+��*С����Ŀ��
				outtextxy(x2,y2,"��");//ָ��λ���������
			}
		}
	}
}
void nextblock(void){ 
	clearblock();//�����������;
	//���ѡ��һ��
	srand(time(NULL));//ʹ��ʱ�亯���ķ���ֵ����Ϊ�������
	nextindex=rand()%BLOCK_COUNT;//ʹ��������������1-4��ֵ
	drawblock(391,71);
}
//�����ָ��λ�ÿ�����ָ�������ƶ�,�ͷ���һ,����ͷ���0
int moveable(int x0,int y0,move_t movedir,block_dir_t blockdir){//�ж��ܲ�����ָ�������ƶ�
	//���㵱ǰ��������Ͻ���30*15����Ϸ���е�λ��(�ڶ�������)
	int x=(y0-miny)/UNIT_SIZE;
	int y=(x0-minx)/UNIT_SIZE;
	int id=blockindex*4+blockdir;
	int ret=1;
	if(movedir == MOVE_DOWN){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//�Ѿ�����ײ��������µ�λ�ô��ڷ�����
				if(block[id][i][j]==1 &&
					(x+i+1>=30 || visit[x+i+1][y+j]==1)){
						ret=0;
				}
			}
		}
	}else if(movedir == MOVE_LEFT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//�Ѿ��������һ������ҵ�λ�ô��ڷ�����
				if(block[id][i][j]==1 &&
					(y+j==0 || visit[x+i][y+j-1]==1)){
						ret=0;
				}
			}
		}
	}else if(movedir == MOVE_RIGHT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//�Ѿ�����������������λ�ô��ڷ�����
				if(block[id][i][j]==1 &&
					(y+j+1==15 || visit[x+i][y+j+1]==1)){
						ret=0;
				}
			}
		}
	}
	return ret;
}
void failCheck(void){//�����Ϸ�Ƿ����
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);//��ɫ
		setfont(45,0,"����");
		outtextxy(75,300,"GAME OVER!");
		Sleep(1000);
		system("pause");
		closegraph();//�ر�ͼ���
		exit(0);//����ֱ�ӽ���
	}
}
//�жϵ�ǰ�����Ƿ����ת��ָ��λ��
//ע���ʱû��ת���÷���!!!
int rotatable(int x,int y,block_dir_t dir){
	int xindex=(x-minx);
	int yindex=(y-miny);
	int id = blockindex * 4 + dir;//��ǰ����*4+��ǰ����
	if(!moveable(x,y,MOVE_DOWN,dir)){//  ����+!  ����x,y ����ת�� ���鵱ǰ����
		return 0;
	}
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1 && 
				(xindex+j<0 || yindex+j>=15 || visit [xindex+i][yindex+j]==1)){
					return 0;
			}
			return 1;
		}
	}
}
void wait(int interval){//��ʱ����
	//��������Ҳ���Լ�ʱ��Ӧ
	int count = interval/10;
	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return;
		}
	}
}
void mark(int x, int y,int blockindex,block_dir_t dir){
	int id = blockindex * 4 + dir;//��ǰ����*4+��ǰ����
	int x2 = (y-miny)/20;
	int y2 = (x-minx)/20;
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1){
				visit[x2+i][y2+j]=1;
				markcolor[x2+i][y2+j]=color[blockindex];
			}
		}
	}
}
void move(void){
	int x=START_X;
	int y=START_Y;
	int k=0;//�½�ֵ
	int curSpeed = speed;
	block_dir_t blockdir = BLOCK_UP;
	//�����Ϸ�Ƿ����
	failCheck();
	//�������½���
	while(1){
		if(kbhit()){
			int key=getch();
			if(key == SPACE){
				getch();//�û�û���ð����ͻῨ������
			}
		}
		//�����ǰ����
		clearblock(x,k,blockdir);
		if(kbhit()){
			int key=getch();
			if(key == UP){
				//��һ������= ���ڵķ���+1��4ȡ�����0;���ǵ�һ����
				block_dir_t nextdir = (block_dir_t)((blockdir+1)%4);
				if(rotatable(x,y+k,nextdir)){//+�ϸ�k,�����½�ֵ
					blockdir = nextdir;//��ǰ���������һ������
				}
			}else if(key == DOWN){
				curSpeed=50;
			}else if(key == LEFT){
				if(moveable(x,y+k+20,MOVE_LEFT,blockdir)){
				x-=20;//ͬ��
				}
			}else if(key == RIGHT){
				if(moveable(x,y+k+20,MOVE_RIGHT,blockdir)){
				x+=20;//x=x+20;
				}
			}
		}
		k+=20;
		//���Ƶ�ǰ����
		drawblock(x,y+k,blockindex,blockdir);
		//Sleep(curSpeed)//����Ҳ���Ե��ǻ��п���Ӱ������
		wait(curSpeed);//��ʱ���� 
		//����Ĺ̻�����,����ֹͣ��
		if(!moveable(x,y+k,MOVE_DOWN,blockdir)){
			mark(x,y+k,blockindex,blockdir);
			break;
		}
	}
}
void newblock(void){
	//ȷ������ʹ�õķ�������
	blockindex = nextindex;
	//���ƸմӶ����½��ķ���
	drawblock(START_X,START_Y);
	//���³��ַ�����ͣһ��,���û�ʶ��
	Sleep(100);//0.1��

	//���Ͻǻ�����һ������
	nextblock();
	//���齵��
	move();
}
//������x��,����������ж�����
void down(int x){
	for(int i=x;i>0;i--){
		//������i��,��j�еķ�������
		for(int j=0;j<15;j++){
			if(visit[i-1][j]){
				visit[i][j]=1;
				markcolor[i][j]=markcolor[i-1][j];
				setcolor(markcolor[i][j]);
				outtextxy(20*j+minx,20*i+miny,"��");
			}else {
				visit[i][j]=0;
				setcolor(BLACK);
				outtextxy(20*j+minx,20*i+miny,"��");
			}
		}
	}
	//�����ϵ���һ��(�����б�0����һ��)
	setcolor(BLACK);
	for(int j=0;j<15;j++){
		visit[0][j]=0;
		outtextxy(20*j+minx,miny,"��");
	}

}
//���²���,����lines��ʾ����������
void addScore(int lines){
	char str[32];
	setcolor(RED);
	score+=lines*10;
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}
//���µȼ�
void updategrade(void){
	//���µȼ�����ʾ
	//����50��һ��
	crank=score/50;
	char str[16];
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
	//�����ٶȵȼ�Խ���ٶ�Խ�� speedԽС
	speed=500-crank*100;
	if(speed <= 100){//�ٶȲ���С��100
		speed=100;
	}
}
//�������麯��
void check(void){
	int clearLines=0;//����������
	int i=0,j=0;//�ر�ע��forѭ����i,j��Ҫ���¶����int,���bug����������Сʱ
	for(i=29;i>=0;i--){//����
		//����i����û��
		for(j=0;j<15 && visit[i][j];j++);//����
		//ִ�е������������;
		//1,��i��û��,�п�λ ��ʱj<15
		//2.��i������,��ʱj>=15
		if(j>=15){
			//��ʱ,Ҫ����i����
			down(i);//������i��,��������Ķ�����
			i++;//��Ϊ������ѭ������i--,˵��������i++,ʹ�´�ѭ��ʱz�ڰ���һ�м��һ��
			clearLines++;
		}
	}
	//���·���
	addScore(clearLines);
	//���µȼ�(���µȼ���ʾ,�����ٶ�)
	updategrade();
}
int main(void){
	welcome();//ʹ�ú���
	initgamescene();//���Ͻǵ��·���
	//�����·���
	nextblock();
	Sleep(500);//���߰����ڳ��¸�����
	//��ʼ����������
	memset(visit,0,sizeof(visit));//memset���ڴ溯��(������,��ʼ��0,ռ���ֽ�)
	while(1){//ѭ���������
		newblock();
		//��������,�����·������ٶ�
		check();
	}
	system("pause");
	closegraph();//�ر�ͼ���
	return 0;
}
