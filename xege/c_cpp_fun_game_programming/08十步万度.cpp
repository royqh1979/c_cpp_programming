/*
第8章 十步万度
原程序使用EasyX，见 https://zhuanlan.zhihu.com/p/217887383 
*/
#include <graphics.h>
#include <math.h>

//小圆圈结构体定义 
struct Round {
	float x,y; //小圆的圆心坐标
	float r; //小圆的半径
	int angle_num;	//对应的角度，可取值为0/1/2/3，分别代表向右/向上/向左/向下 
};

Round rounds[5][5]; 
int step; //还剩下的操作步数
int score; //得分，也就是一共旋转了多少度 

/* 对二维数组中第i行j列的小圆圈，顺时针旋转 */

/**
 * @brief 对二维数组中第i行j列的小圆圈，顺时针旋转 
 * 
  * 
 * @param i 
 * @param j 
 * @return 
 **/
void rotate_round(int i,int j){
	rounds[i][j].angle_num -= 1; //值减1
	if (rounds[i][j].angle_num<0)
		rounds[i][j].angle_num = 3;
	score += 90; 
}

/**
 * @brief 获得当前圆指向的下一个圆圈序号
 * 
 * 存储在数组int indexes[2]中，下一个圆序号页存储在这个数组中
 * 
 * @param indexes 当前圆序号；函数返回时，该数组保存下一个圆的序号 
 * @return 如果有下一个指向的圆，返回1；如果指向边界了，返回0 
 **/
int get_next_indexes(int indexes[2]){
	int i = indexes[0]; // 当前圆的i,j序号 
	int j = indexes[1];
	
	// 根据当前圆圈的角度，获得下一个小圆圈的序号 
	if (rounds[i][j].angle_num == 0){
		//指向右边的小圆圈 
		j++;
	} else if (rounds[i][j].angle_num == 3) {
		//指向下边的小圆圈
		i++;  
	} else if (rounds[i][j].angle_num == 2) {
		//指向左边的小圆圈
		j--; 
	} else if (rounds[i][j].angle_num == 1) {
		//指向上边的小圆圈 
		i--;
	}
	
	indexes[0] = i;
	indexes[1] = j;
	
	if ((i>=0) && (i<5) && (j>=0) && (j<5)) 
		return 1;
	else
		return 0;
}

/**
 * @brief 程序启动和初始化 
 * 
 **/
void startup() {
	//初始化函数
	initgraph(600,700);
	setbkcolor(EGERGB(50,50,50)); 
	setlinewidth(3);
	setrendermode(RENDER_MANUAL);
	cleardevice();
	step = 10; 
	score = 0;
	for (int i=0;i<5;i++){
		for (int j=0;j<5;j++){
			// 设定小圆坐标 
			rounds[i][j].x = 100 + j * 100; 
			rounds[i][j].y = 200 + i * 100; 
			//设定小圆半径 
			rounds[i][j].r = 30;  
			//设定小圆初始指向 向上; 
			rounds[i][j].angle_num = 1 ;			
		}
	}
}

/**
 * @brief 绘制函数 
 * 
 **/ 
void show(){
	float angle; 
	cleardevice();
	
	for (int i=0;i<5;i++){
		for (int j=0;j<5;j++){
			//设置圆圈为白灰色 
			setcolor(EGERGB(200,200,200));
			//画小圆圈 
			circle(rounds[i][j].x,rounds[i][j].y,rounds[i][j].r);  
			
			//画红色指示线 
			setcolor(RED);
			angle = rounds[i][j].angle_num*PI/2;
			line(
				rounds[i][j].x,rounds[i][j].y,
				rounds[i][j].x + rounds[i][j].r * cos(-angle),
				rounds[i][j].y + rounds[i][j].r * sin(-angle) 
			);
		}
	}
	
	setcolor(WHITE);
	setfont(50,0,"宋体");
	xyprintf(150,30,"%d 步 %d 度",step,score);
	setfont(20,0,"宋体");
	xyprintf(15,100,"点击一个圆圈，其指针顺时针旋转90度之后 指向的指针依次旋转");
}

/**
 * @brief 更新函数 
 * 
 **/
void update(){
	mouse_msg m; 
	int mouse_clicked = 0;
	while (mousemsg()){
		m = getmouse();
		if (m.is_up() && m.is_left()) {
			mouse_clicked = 1;
		}
	}
	if (mouse_clicked && (step>0)) {
		step--;
		//获得当前点击圆的序号
		int clicked_i = int(m.y - 150)/100;  
		int clicked_j = int(m.x - 50) /100;
		if ((clicked_i<0) || (clicked_i>=5)
			||(clicked_j<0) || (clicked_j>=5)) {
				return;
		}
		rotate_round(clicked_i,clicked_j);
		show();
		delay_ms(500);
		int indexes[2]={clicked_i,clicked_j};
		while (get_next_indexes(indexes)){
			rotate_round(indexes[0],indexes[1]);
			show();
			delay_ms(500);
		}
	}
}

int main(){
	startup();
	while(is_run()){
		show();
		update();
		delay_ms(1);
	}
	closegraph();
	return 0;
}
