#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<time.h>
#include<conio.h>
using namespace std;
int score=0;//全局变量 分数
int crank=0;//等级
#define BLOCK_COUNT 5 //几种方块
#define BLOCK_WIDTH 5 //列
#define BLOCK_HEIGHT 5 //高度 行
#define UNIT_SIZE   20//小方块占的像素
#define START_X 130
#define START_Y 30
#define UP 72  //↑的阿克斯码值
#define DOWN 80
#define RIGHT 77//
#define LEFT 75
#define SPACE 32
int minx= 30;
int miny= 30;
typedef enum {//枚举类型
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT
}move_t;
typedef enum {//枚举类型
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t;
int speed=500;
int nextindex= -1;//下一个方块的种类
int blockindex= -1;//当前方块的种类
int color[BLOCK_COUNT]={//方块颜色
	GREEN,RED,CYAN,YELLOW,MAGENTA
};
int visit [30][15];//访问数组
int markcolor [30][15];//表示对应位置的颜色
int block [BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH]={//方块图像
	//l 型方块
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
	//L型
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
	//田型
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
	//T型
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
	//Z型
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
//欢迎界面
void welcome(void){
	//初始话画布
	initgraph(550,660);
	//设置窗口标题
	HWND window = GetHWnd();//获取窗口
	//设置窗口标题
	SetWindowText(window,_T("俄罗斯方块 WDNMD NB"));
	//设置文本字体样式
	//高度  宽度0(代表默认) 字体
	setfont(40,0,_T("微软雅黑"));
	setcolor(WHITE);//白色
	outtextxy(205,200,_T("俄罗斯方块"));//左上指定位置输出文本
	outtextxy(175,300,_T("编程从俄罗斯方块开始!"));
	Sleep(3000);//休眠3000毫秒 3秒
}
//初始化游戏场景
void initgamescene(void){
	char str[16];//定义个字符串数组
	cleardevice();//清除屏幕
	//画背景框
	rectangle(27,27,336,635);//画方块(左上的x,y  右下的x,y)
	rectangle(29,29,334,633);
	rectangle(370,50,515,195);
	setfont(24,0,_T("楷体"));//设置文本字体样式
	setcolor(LIGHTGRAY);//灰色
	outtextxy(405,215,_T("下一个"));//左上指定位置输出文本
	//分数
	setcolor(RED);//红色
	outtextxy(405,280,_T("分数"));//左上指定位置输出文本
	sprintf(str,"%d",score);//把分数输出到字符串数组里
	outtextxy(415,310,str);//加不了_T,更改项目属性 字符集->使用多字符字符集
	outtextxy(405,375,_T("等级"));//左上指定位置输出文本
	sprintf(str,"%d",crank);
	outtextxy(425,405,str);
	//操作说明
	setcolor(BLUE);
	outtextxy(390,475,"操作说明");
	outtextxy(390,500,"↑:旋转");
	outtextxy(390,525,"↓:下降");
	outtextxy(390,550,"←:左移");
	outtextxy(390,575,"→:右移");
	outtextxy(390,600,"空格:暂停");
}
//清除右上区域;
void clearblock(void){
	setcolor(BLACK);//黑色
	setfont(23,0,"楷体");
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//□
			int x=391+j*UNIT_SIZE;//起始点行+行*小方块的宽度
			int y=71+i*UNIT_SIZE;//起始点列+行*小方块的宽度
			outtextxy(x,y,"■");//指定位子输出方块
		}
	}
}
//参数x 方块的左上角的x坐标
//参数y 方块的左上角在游戏区域内的坐标,距离游戏区域顶部的距离
void clearblock(int x,int y,block_dir_t dir){//新清除主方块的方块
	setcolor(BLACK);
	int id = blockindex * 4 + dir;
	y += START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j]==1){
				//擦除该方块的i行j列
				outtextxy(x+(20*j),y+i*20,"■");
			}
		}
	}
}
//绘制方块在指定位置绘制指定方块的指定方向 ( x,y 哪个方块 哪个方向)
void drawblock(int x,int y,int blokindex,block_dir_t dir){
	setcolor(color[blokindex]);//随机颜色
	setfont(23,0,"楷体");
	int id = blockindex * 4 + dir;//当前方块*4+当前方向
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//"■"
			if(block[id][i][j]==1){//方块数组每种可以旋转4次*4,数组=1的话执行下一个
				int x2= x + j * UNIT_SIZE;//起始点行+行*小方块的宽度
				int y2= y + i * UNIT_SIZE;//起始点列+行*小方块的宽度
				outtextxy(x2,y2,"■");//指定位子输出方块
			}
		}
	}
}
//在右上角区域中,绘制下一个方块
void drawblock(int x,int y){
	setcolor(color[nextindex]);//随机颜色
	setfont(23,0,"楷体");
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			//□
			if(block[nextindex*4][i][j]==1){//方块数组每种可以旋转4次*4,数组=1的话执行下一个
				int x2=x+j*UNIT_SIZE;//起始点行+行*小方块的宽度
				int y2=y+i*UNIT_SIZE;//起始点列+行*小方块的宽度
				outtextxy(x2,y2,"■");//指定位子输出方块
			}
		}
	}
}
void nextblock(void){ 
	clearblock();//清除右上区域;
	//随机选择一种
	srand(time(NULL));//使用时间函数的返回值来作为随机种子
	nextindex=rand()%BLOCK_COUNT;//使用随机数生成随机1-4的值
	drawblock(391,71);
}
//如果在指定位置可以向指定方向移动,就返回一,否则就返回0
int moveable(int x0,int y0,move_t movedir,block_dir_t blockdir){//判断能不能往指定方向移动
	//计算当前方块的左上角在30*15的游戏区中的位置(第多少行列)
	int x=(y0-miny)/UNIT_SIZE;
	int y=(x0-minx)/UNIT_SIZE;
	int id=blockindex*4+blockdir;
	int ret=1;
	if(movedir == MOVE_DOWN){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//已经到达底部或者往下的位置存在方块了
				if(block[id][i][j]==1 &&
					(x+i+1>=30 || visit[x+i+1][y+j]==1)){
						ret=0;
				}
			}
		}
	}else if(movedir == MOVE_LEFT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//已经到达最右或者往右的位置存在方块了
				if(block[id][i][j]==1 &&
					(y+j==0 || visit[x+i][y+j-1]==1)){
						ret=0;
				}
			}
		}
	}else if(movedir == MOVE_RIGHT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){//已经到达最左或者往左的位置存在方块了
				if(block[id][i][j]==1 &&
					(y+j+1==15 || visit[x+i][y+j+1]==1)){
						ret=0;
				}
			}
		}
	}
	return ret;
}
void failCheck(void){//检查游戏是否结束
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);//白色
		setfont(45,0,"楷体");
		outtextxy(75,300,"GAME OVER!");
		Sleep(1000);
		system("pause");
		closegraph();//关闭图像库
		exit(0);//程序直接结束
	}
}
//判断当前方块是否可以转向到指定位置
//注意此时没有转到该方向!!!
int rotatable(int x,int y,block_dir_t dir){
	int xindex=(x-minx);
	int yindex=(y-miny);
	int id = blockindex * 4 + dir;//当前方块*4+当前方向
	if(!moveable(x,y,MOVE_DOWN,dir)){//  不能+!  坐标x,y 往下转动 方块当前方向
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
void wait(int interval){//延时数组
	//可以休眠也可以及时响应
	int count = interval/10;
	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return;
		}
	}
}
void mark(int x, int y,int blockindex,block_dir_t dir){
	int id = blockindex * 4 + dir;//当前方块*4+当前方向
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
	int k=0;//下降值
	int curSpeed = speed;
	block_dir_t blockdir = BLOCK_UP;
	//检查游戏是否结束
	failCheck();
	//持续向下降落
	while(1){
		if(kbhit()){
			int key=getch();
			if(key == SPACE){
				getch();//用户没有用按键就会卡在这里
			}
		}
		//清除当前方块
		clearblock(x,k,blockdir);
		if(kbhit()){
			int key=getch();
			if(key == UP){
				//下一个方向= 现在的方向+1和4取余就是0;就是第一方向
				block_dir_t nextdir = (block_dir_t)((blockdir+1)%4);
				if(rotatable(x,y+k,nextdir)){//+上个k,就是下降值
					blockdir = nextdir;//当前方向等于下一个方向
				}
			}else if(key == DOWN){
				curSpeed=50;
			}else if(key == LEFT){
				if(moveable(x,y+k+20,MOVE_LEFT,blockdir)){
				x-=20;//同下
				}
			}else if(key == RIGHT){
				if(moveable(x,y+k+20,MOVE_RIGHT,blockdir)){
				x+=20;//x=x+20;
				}
			}
		}
		k+=20;
		//绘制当前方块
		drawblock(x,y+k,blockindex,blockdir);
		//Sleep(curSpeed)//这样也可以但是会有卡顿影响性能
		wait(curSpeed);//延时处理 
		//方块的固话处理,到底停止等
		if(!moveable(x,y+k,MOVE_DOWN,blockdir)){
			mark(x,y+k,blockindex,blockdir);
			break;
		}
	}
}
void newblock(void){
	//确定即将使用的方块的类别
	blockindex = nextindex;
	//绘制刚从顶部下降的方块
	drawblock(START_X,START_Y);
	//让新出现方块暂停一会,让用户识别
	Sleep(100);//0.1秒

	//右上角绘制下一个方块
	nextblock();
	//方块降落
	move();
}
//消除第x行,并把上面的行都下移
void down(int x){
	for(int i=x;i>0;i--){
		//消除第i行,第j列的方格消除
		for(int j=0;j<15;j++){
			if(visit[i-1][j]){
				visit[i][j]=1;
				markcolor[i][j]=markcolor[i-1][j];
				setcolor(markcolor[i][j]);
				outtextxy(20*j+minx,20*i+miny,"■");
			}else {
				visit[i][j]=0;
				setcolor(BLACK);
				outtextxy(20*j+minx,20*i+miny,"■");
			}
		}
	}
	//清除最顶上的那一行(就是行表0的那一行)
	setcolor(BLACK);
	for(int j=0;j<15;j++){
		visit[0][j]=0;
		outtextxy(20*j+minx,miny,"■");
	}

}
//更新参数,参数lines表示消除的行数
void addScore(int lines){
	char str[32];
	setcolor(RED);
	score+=lines*10;
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}
//更新等级
void updategrade(void){
	//更新等级的提示
	//假设50分一级
	crank=score/50;
	char str[16];
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
	//更新速度等级越高速度越快 speed越小
	speed=500-crank*100;
	if(speed <= 100){//速度不能小于100
		speed=100;
	}
}
//消除方块函数
void check(void){
	int clearLines=0;//消除多少行
	int i=0,j=0;//特别注意for循环的i,j不要重新定义加int,这个bug我找了两个小时
	for(i=29;i>=0;i--){//麻了
		//检查第i行满没满
		for(j=0;j<15 && visit[i][j];j++);//麻了
		//执行到这有两种情况;
		//1,第i行没满,有空位 此时j<15
		//2.第i行满了,此时j>=15
		if(j>=15){
			//此时,要消第i行了
			down(i);//消除第i行,并吧下面的都下移
			i++;//因为最外层的循环中有i--,说以我们先i++,使下次循环时z在把这一行检查一下
			clearLines++;
		}
	}
	//更新分数
	addScore(clearLines);
	//更新等级(更新等级提示,更新速度)
	updategrade();
}
int main(void){
	welcome();//使用函数
	initgamescene();//右上角的新方块
	//产生新方块
	nextblock();
	Sleep(500);//休眠半秒在出下个方块
	//初始化访问数组
	memset(visit,0,sizeof(visit));//memset改内存函数(数组名,初始化0,占的字节)
	while(1){//循环输出方块
		newblock();
		//消除满行,并更新分数和速度
		check();
	}
	system("pause");
	closegraph();//关闭图像库
	return 0;
}
