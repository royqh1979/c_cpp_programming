/*
别碰白块
原程序使用EasyX实现，来自https://zhuanlan.zhihu.com/p/186282897
*/

#include <graphics.h>
#include <string.h>
#include <stdio.h>

int main(){
	float width, height, gravity; // 游戏画面大小、重力加速度 
	float ball_x, ball_y,ball_vy,radius; //小球圆心坐标、y方向速度、半径大小 
	float rect_left_x, rect_top_y, rect_width, rect_height, rect_vx; // 方块障碍物的相关参数
	int score = 0; //得分
	int is_ball_on_floor = TRUE; //小球是否在地面上，避免重复起跳
	 
	width = 600; //游戏画面宽度
	height = 400; //游戏画面高度
	gravity = 0.6; // 重力加速度
	
	radius = 20; //小球半径 
	ball_x = width / 4; //小球x位置
	ball_y = height - radius; //小球y位置
	ball_vy = 0; //小球初始y速度为0
	
	rect_height = 100; //方块高度
	rect_width = 20; //方块宽度 
	rect_left_x = width*3/4; //方块坐标x坐标
	rect_top_y = height -rect_height; //方块顶部y坐标
	rect_vx = -3; //方块x方向速度 

	initgraph(width,height); //创建绘图窗口 
	setfillcolor(WHITE);
	setrendermode(RENDER_MANUAL);
	while(is_run()){
		if (kbhit()){
			char input = getch();
			if (input == ' ' && is_ball_on_floor) { //当按下空格键且小球在地面时
				ball_vy=-17; // 给小球一个向上的速度
				is_ball_on_floor = FALSE; //小球不在地面了 
			}
		}
		//小球
		ball_vy +=  gravity; //在重力作用下小球y方向速度发生变化 
		ball_y += ball_vy; //更新小球y坐标
		if (ball_y >= height-radius) { //如果小球落到地面上
			ball_vy=0; //y速度为0;
			ball_y=height-radius; // 小球y位置为地面;
			is_ball_on_floor = TRUE; 
		} 
		//方块 
		rect_left_x+=rect_vx; // 方块移动
		if (rect_left_x<=0) { //如果方块跑到最坐标 
			rect_left_x = width; //在最右边重新出现 
			score+=1; //得分+1
			rect_height = random((int)height/4)+height/4; //设置随机高度 
			rect_vx = randomf() * 7 - 7;
			if ((rect_vx > -0.5) && (rect_vx<=0))
				rect_vx=-0.5;
			if ((rect_vx < 0.5) && (rect_vx>=0))
				rect_vx=0.5;
		}
		if ((rect_left_x<=ball_x+radius)
			&& (rect_left_x+rect_width >= ball_x-radius)
			&& (height-rect_height<=ball_y+radius)){
			delay(500); // 慢动作效果
			score = 0; //得分清零 
		}
		
		cleardevice();  // 清空画面 
		fillellipse(ball_x,ball_y,radius,radius); // 绘制小球		
			
		bar(rect_left_x,rect_top_y,rect_left_x+rect_width,rect_top_y+rect_height);
		
		xyprintf(50,30,"%d",score);
		delay_fps(30);		
	} 
	closegraph();
	return 0;
}
