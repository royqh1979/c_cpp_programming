/**
 * 推箱子
 *  
 * 原程序使用EasyX，见https://zhuanlan.zhihu.com/p/244874347 
 **/
#include <graphics.h>

#define B_SIZE 60 // 方块大小
#define B_NUM 8 // 方块个数，一共8*8个方块

// 用于记录玩家位置的结构体 
struct Player {
	int i;
	int j;
};

Player player; //玩家位置全局变量

// 枚举类型，定义地图方块的可能类型 
enum ElementType {
	wall,target,box,empty,achieved,role
};

//地图方块数据 
ElementType level[B_NUM][B_NUM]={
	{wall,wall,wall,wall,wall,wall,wall,wall},
	{wall,wall,wall,target,box,empty,empty,wall},
	{wall,empty,empty,empty,empty,empty,empty,wall},
	{wall,empty,empty,empty,empty,empty,empty,wall},
	{wall,empty,empty,empty,empty,empty,empty,wall},
	{wall,role,empty,box,target,wall,wall,wall},
	{wall,empty,empty,empty,empty,wall,wall,wall},
	{wall,wall,wall,wall,wall,wall,wall,wall}
};

//位置个数，完成目标个数 
int target_num,achieved_num;

/**
 * @brief 初始化函数 
 * 
 **/
void startup(){
	//创建图形窗口 
	initgraph(B_NUM * B_SIZE,B_NUM*B_SIZE,INIT_NOFORCEEXIT); 	
	//使用灰色背景 
	setbkcolor(EGERGB(150,150,150));
	//使用非自动模式绘制
	//只有执行delay_fps/delay_ms函数时才将内容绘制到窗口上 
	setrendermode(RENDER_MANUAL);
	
	target_num = 0; // 目标个数，初始为0 
	
	// 对二维数组遍历 
	for (int i=0;i<B_NUM;i++){
		for (int j=0;j<B_NUM;j++){
			switch(level[i][j]) {
				case role:
					//找到地图中player起始位置 
					player.i = i;
					player.j = j;
					level[i][j] = empty; 
					break;
				case target:
				case achieved:
					target_num++;
			}
		}
	} 
} 

/**
 * @brief 绘制函数
 * 
 **/
void show(){
	int i,j;
	cleardevice();
	//绘制关卡地图 
	for (i=0;i<B_NUM;i++){
		for (j=0;j<B_NUM;j++){
			switch(level[i][j]){
				case empty:
					//是空地板，绘制灰色方块 
					setfillcolor(EGERGB(150,150,150));
					setcolor(EGERGB(150,150,150));
					bar(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					rectangle(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					break;
				case wall:
					//是墙，绘制淡红色方块 
					setfillcolor(EGERGB(155,0,0)); 
					setcolor(EGERGB(150,150,150));
					bar(j*B_SIZE,i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					rectangle(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					break;
				case box:
					//是箱子，绘制黄色方块 
					setfillcolor(EGERGB(250,250,0)); 
					setcolor(EGERGB(150,150,150));
					bar(j*B_SIZE,i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					rectangle(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					break;
				case target:
					//是目标，绘制白色小方块
					setfillcolor(RGB(250,250,250));
					bar((j+0.3)*B_SIZE,(i+0.3)*B_SIZE,(j+0.7)*B_SIZE,(i+0.7)*B_SIZE);
					break;
				case achieved:
					//是已到达目标的箱子，绘制黄色方块嵌套白色方块
					setcolor(EGERGB(150,150,150));
					setfillcolor(EGERGB(255,255,0));
					bar(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					rectangle(j*B_SIZE, i*B_SIZE,(j+1)*B_SIZE,(i+1)*B_SIZE);
					
					setfillcolor(EGERGB(250,250,250));
					bar((j+0.3)*B_SIZE,(i+0.3)*B_SIZE,(j+0.7)*B_SIZE,(i+0.7)*B_SIZE);
					break;
			}
		}
	} 
	//绘制小人（人脸图案） 
	i = player.i;
	j = player.j;
	//红色圆脸 
	setfillcolor(EGERGB(255,0,0));
	fillellipse((j+0.5)*B_SIZE, (i+0.5)*B_SIZE, 0.4*B_SIZE,0.4*B_SIZE);
	setfillcolor(EGERGB(80,80,80));
	//黑色眼睛 
	fillellipse((j+0.3)*B_SIZE, (i+0.45)*B_SIZE, 0.08*B_SIZE,0.08*B_SIZE);
	fillellipse((j+0.7)*B_SIZE, (i+0.45)*B_SIZE, 0.08*B_SIZE,0.08*B_SIZE);
	//黑色嘴巴 
	setlinewidth(3);
	setcolor(EGERGB(80,80,80));
	line((j+0.35)*B_SIZE,(i+0.7)*B_SIZE,(j+0.65)*B_SIZE,(i+0.7)*B_SIZE); 
	//如果已完成所有目标 
	if (achieved_num == target_num){
		setcolor(EGERGB(0,255,255));
		setfont(80,0,"宋体");
		xyprintf(80,200,"游戏胜利");
	}
} 

//更新游戏状态 
void update() {
	if (kbhit() && (achieved_num<target_num) ) {
		int ch = getch();
		if ( (ch == 'a')
			|| (ch == 's')
			|| (ch == 'd')
			|| (ch == 'w')) {
			//角色小人位置 
			Player goal = player;
			//小人面前的位置
			Player goal_next = player;
			switch(ch){
				case 'a':
					//向左移动 
					goal.j = player.j-1;
					goal_next.j = goal.j-1; 
					break;
				case 'd':
					//向右移动
					goal.j = player.j+1;
					goal_next.j = goal.j+1;
					break;
				case 's':
					//向下移动
					goal.i = player.i+1;
					goal_next.i = goal.i+1;
					break; 
				case 'w':
					//向上移动
					goal.i = player.i-1;
					goal_next.i = goal.i-1;
					break;				
			}
			
			//根据地图判断如何移动角色和更新
			if (level[goal.i][goal.j]==empty || level[goal.i][goal.j]==target) {
				//移动位置无墙或箱子
				player = goal;	
			} else if ( level[goal.i][goal.j]==box && level[goal_next.i][goal_next.j]==empty ) {
				//移动位置有箱子，且能推动
				player = goal;
				level[goal.i][goal.j]=empty;
				level[goal_next.i][goal_next.j]=box;
			} else if ( level[goal.i][goal.j]==box && level[goal_next.i][goal_next.j]==target ) {
				player = goal;
				level[goal.i][goal.j]=empty;
				level[goal_next.i][goal_next.j]=achieved;
			} else if ( level[goal.i][goal.j]==achieved && level[goal_next.i][goal_next.j]==empty ) {
				player = goal;
				level[goal.i][goal.j]=target;
				level[goal_next.i][goal_next.j]=box;
			} else if ( level[goal.i][goal.j]==achieved && level[goal_next.i][goal_next.j]==target ) {
				player = goal;
				level[goal.i][goal.j]=target;
				level[goal_next.i][goal_next.j]=achieved;
			}
			//计算已完成目标
			achieved_num = 0;
			for (int i;i<B_NUM;i++){
				for (int j=0;j<B_NUM;j++) {
					if (level[i][j]==achieved) {
						achieved_num++;
					}
				}
			} 	
		} 
	}
}

int main(){
	startup();
	while (is_run()){
		show();
		update();
		delay_fps(30);
	}
	closegraph();
	return 0;
}
