/*
见缝插针 游戏
原程序使用EasyX，见 https://zhuanlan.zhihu.com/p/195049797 
*/
#include <graphics.h>
#include <math.h>

int main(){
	int width=800;
	int height=600;
	
	float line_length=160;//针的长度
	float x_end,y_end; //针的终点位置坐标（针的起始位置为圆心）
	float rotate_speed = PI/360;
	int line_num = 0; //在旋转的针的个数
	float angles[1000]; //存储所有针的旋转角度，最多1000根针
	int score = 0; //得分
	
	initgraph(width,height);
	setbkcolor(WHITE);
	setlinewidth(3);
	setfillcolor(WHITE);
	setrendermode(RENDER_MANUAL);
	while (is_run()){
		cleardevice();
		setcolor(BLACK); //设置针的颜色为黑色
		line(0,height/2,line_length,height/2); //绘制左侧发射区域的一根针
		for (int i=0;i<line_num;i++){ //对所有旋转针进行遍历			
			angles[i] += rotate_speed; //增加角度
			if (angles[i] > 2*PI)
				angles[i]-=2*PI;
			x_end = line_length*cos(-angles[i])+width/2; //计算针的末端坐标
			y_end = line_length*sin(-angles[i])+height/2; 
			setcolor(BLUE);
			if (i==line_num-1) {
				setcolor(RED);
			} 
			line(width/2,height/2,x_end,y_end); //绘制一根针 
		}
	
		if (kbhit() && rotate_speed!=0) {//如果按键，并且旋转速度不等于0
			char input = getch(); 
			if (input== ' ') { //如果为空格键
				line_num++; //针的个数加一 
				angles[line_num-1]=2*PI; //新加针的初始角度 
				x_end = line_length * cos(-angles[line_num-1])+width/2; //新加针的末端坐标 
				y_end = line_length * sin(-angles[line_num-1])+height/2; 
				line(width/2,height/2,x_end,y_end);
				for (int i=0;i<line_num-1;i++){
					//拿新增加的针和之前所有的针比较
					//如果两根针之间的角度相近，认为碰撞，游戏结束
					if (fabs(angles[line_num-1]-angles[i])<PI/60) {
						rotate_speed = 0;
						score = -1;
						break;
					} 
				} 
				score = score + 1;
			}
		}
		setfillcolor(HSVtoRGB(0,line_num/60.0,0.8)); // 中间的圆盘颜色，针越多，颜色越鲜艳
		setcolor(HSVtoRGB(0,line_num/60.0,0.8));
		fillellipse(width/2,height/2,60,60); //绘制中间的圆盘
		//输出得分 
		setcolor(DARKGRAY);
		xyprintf(65,200,"%d",score);
		delay_fps(60);		
	}
	closegraph();
	return 0;
}	 
		
