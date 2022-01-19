/*
见缝插圆
原程序使用EasyX，见 https://zhuanlan.zhihu.com/p/200744688
*/ 
#include <graphics.h>
#include <math.h>

//求两个点之间的距离
float dist2points(float x1,float y1,float x2,float y2){
	float result;
	result = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	return result;
} 

// 判断两个圆是否相交
/**
 * @brief 判断两个圆是否相交 
 * 
 * @param x1 第一个圆的圆心x坐标 
 * @param y1 第一个圆的圆心y坐标 
 * @param r1 第一个圆的半径 
 * @param x2 第二个圆的圆心新坐标 
 * @param y2 第二个圆的圆心y坐标 
 * @param r2 第二个圆的半径 
 * 
 * @return 非0表示两圆相交，0表示两圆不相交 
 **/
int is_two_circles_intersect(float x1,float y1,float r1,
	float x2, float y2, float r2){
	return (dist2points(x1,y1,x2,y2)<(r1+r2)); 
} 

//绘制填充黄色圆
void draw_circle1(float x,float y,float r){
	setcolor(BLACK);
	setfillcolor(YELLOW);
	fillellipse(x,y,r,r);
} 

//绘制随机填充颜色圆
void draw_circle2(float x, float y, float r){
	float h = random(360);
	COLORREF color = HSVtoRGB(h,0.6,0.8);
	setcolor(WHITE);
	setfillcolor(color);
	fillellipse(x,y,r,r);
}

//绘制随机填充颜色同心圆
void draw_circle3(float x, float y, float r){
	while(r>0){
		float h=random(360);
		COLORREF color = HSVtoRGB(h,0.6,0.8);
		setcolor(WHITE);
		setfillcolor(color);
		fillellipse(x,y,r,r);
		r-=5;
	}		
} 

//绘制随机线条颜色同心圆
void draw_circle4(float x, float y, float r){
	while (r>0){
		float h=random(360);
		COLORREF color = HSVtoRGB(h,0.9,0.8);
		setcolor(color);
		circle(x,y,r);
		r-=5;
	}
} 

int main(){
	int width = 600;
	int height = 600;
	
	int x_array[1000]; //所有圆心的x坐标
	int y_array[1000]; //所有圆心的y坐标
	int r_array[1000]; //所有圆的半径
	int rmin = 8; //圆的最小半径
	int rmax = 50; //圆的最大半径
	int circle_num = 0; //圆的个数 
	float x,y,r; //新增圆的圆心坐标、半径
	int is_new_circle_ok; //用于判断新生成 
	int i;
	int draw_mode = 3; //用于设定4种不同的绘制模式，开始设为3

	
	initgraph(width,height);
	setbkcolor(WHITE);
	cleardevice();
	randomize(); 
	
	while (circle_num < 1000) {
		//生成新圆 
		is_new_circle_ok = FALSE; 
		while (! is_new_circle_ok ) {
			if (kbhit()) {
				char input = getch();
				if (input == ' ') {
					circle_num = 0; 
					cleardevice();
					draw_mode++;
					draw_mode%=4; 
				}
			}
			
			//生成新圆 
			x = random(width);
			y = random(height);
			r = rmin;
			
			//检测新圆是否与旧圆相交 
			for (i=0;i<circle_num;i++) {
				if (is_two_circles_intersect(
					x_array[i],y_array[i],r_array[i],
					x,y,r
				)) {
					break;
				}
			}		
			if (i>=circle_num) {
				is_new_circle_ok = TRUE;
			}
		}
		//最大化新圆的半径
		is_new_circle_ok = FALSE;
		while ( ! is_new_circle_ok && r<rmax){
			r++;
			for (i=0;i<circle_num;i++) {
				if (is_two_circles_intersect(
					x_array[i],y_array[i],r_array[i],
					x,y,r
				)) {
					is_new_circle_ok = TRUE; 
					break;
				}
			}
		} 
		
		x_array[circle_num]=x; //把新圆的参数加到数组中
		y_array[circle_num]=y; 
		r_array[circle_num]=r;
		
		circle_num++; //圆的个数加1
		
		switch(draw_mode){
			case 0: draw_circle1(x,y,r);
				break;
			case 1: draw_circle2(x,y,r);
				break;
			case 2: draw_circle3(x,y,r);
				break;
			case 3: draw_circle4(x,y,r);
				break;
		} 
		
		delay(10);
		
	} 
	getch();
	closegraph();
	return 0;
}		
