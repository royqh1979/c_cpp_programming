/*
自由落体小球 
原程序使用EasyX实现，来自https://zhuanlan.zhihu.com/p/186255141 
*/
#include <graphics.h>

int main(){
	float x,y,vx,vy,g;  
	

	// 小球的坐标 
	x=100;  
	y=200; 
	
	// 小球的速度分量 
	vx=8;  
	vy=0; 
	
	// 小球y方向加速度 
	g = 0.5;	

	// 创建绘图窗口，宽600， 高600
	// INIT_NOFORCEEXIT 表示按下关闭按钮后程序继续运行 
	initgraph(600,600,INIT_NOFORCEEXIT); 
	
	// 设置更新模式为手动，以避免屏幕闪烁 
	// (执行delay_fps/delay_ms函数时才将内容绘制到屏幕上)
	setrendermode(RENDER_MANUAL); 
	
	//设置图形填充颜色为白色 
	setfillcolor(WHITE);
	
	//当没有按下绘图窗口的关闭按钮时，is_run()返回1 
	while(is_run()){
		// 清除之前绘制的内容
		cleardevice(); 
		// 利用加速度g更新vy速度
		vy += g; 
		//更新小球坐标
		x += vx;  
		y += vy; 
		
		if (y>=580) { 
			//当碰到地面时
			//注意本例中窗口顶部y坐标为0，底部y坐标为600 
			
			// x方向速度受阻尼影响变小
			vx = 0.98*vx; 
			
			// y方向反弹，速度改变方向，并受阻尼影响变小
			vy = -0.95*vy; 
			
			//防止小球越过地面
			y = 580;  
		} 
		if (x>=580) {
			//碰到右边墙时
			//小球x方向反弹 
			vx=-vx; 
			x=580; 
		} 
		if (x<=20){ 
		    //碰到左边墙 
		    //小球x方向反弹
			vx=-vx; 
			x=20; 			
		} 
		//绘制小球 
		fillellipse(x,y,20,20);
		/* 
		控制绘制帧率，每1/60秒绘制一帧； 
		一帧绘制完后，剩下的时间等待 
		*/
		delay_fps(60);		
	}
	//关闭和清理绘图窗口 
	closegraph();
	return 0;
} 
