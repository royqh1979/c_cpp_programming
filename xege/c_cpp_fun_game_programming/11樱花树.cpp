/**
 * 樱花树 
 * 原程序使用EasyX，见https://zhuanlan.zhihu.com/p/260285711
 **/
#include <graphics.h>

#include <math.h>

#define WIDTH 800
#define HEIGHT 600

//左右枝干和父枝干偏离的弧度 
float offset_angle = PI /6; 

//子枝干比父枝干变短的倍数 
float shorten_rate = 0.65;

//是否显示树生成的过程动画 
int is_show_animation = 1;


/**
 * @brief 映射函数 
 * 
 * 把[input_min, input_max]范围内的input变量，
 * 映射到[output_min, output_max]范围
 * 
 * @param input 
 * @param input_min 
 * @param input_max 
 * @param output_min 
 * @param output_max 
 * 
 * @return 
 **/
float map_value(float input, float input_min, float input_max, 
	float output_min, float output_max) {
	float output;
	if (fabs(input-input_min)<0.000001) 
		output = output_min; // 防止input_max - input_min为零 
	else
		output = ( (input-input_min) * (output_max-output_min) 
			/ (input_max - input_min) ) + output_min; 
	return output;	
} 

/**
 * @brief 生成min到max之间的随机数 
 * 
 * @param min 
 * @param max 
 * 
 * @return 
 **/
float rand_between(float min, float max) {
	//生成服从[0,1)区间均匀分布的随机数 
	float t = randomf();
	
	//使用map_value将t映射到[min,max)区间上 
	return map_value(t,0,1,min,max);	
}

/**
 * @brief 枝干生成和绘制 
 * 
 * @param x_start	枝干起始横坐标 
 * @param y_start	枝干起始纵坐标 
 * @param length	枝干长度 
 * @param angle 	枝干角度 
 * @param thinckness 	枝干线条宽度 
 * @param generation 枝干代数，用于控制递归结束 
 **/
void branch(float x_start, float y_start, float length, 
	float angle, float thickness, int generation) {
	//当前枝干的终点坐标 
	float x_end, y_end;	
	x_end = x_start + length * cos(angle);
	y_end = y_start + length * sin(angle);
	
	//画枝干	
	
	//设定当前枝干宽度和颜色 
	setlinewidth(thickness);	
	color_t color = HSVtoRGB(15,0.75,0.4+generation*0.05);
	setcolor(color); 
	
	line(x_start,y_start, x_end, y_end);
	
	int child_generation = generation + 1;
	float child_length = shorten_rate * length;
	
	if (child_length >= 2 && child_generation <=9) {
		//当递归层数小于10，且子树干长度足够长时， 递归绘制子树干
		
		float child_thinkness = thickness * 0.8;
		if (child_thinkness < 2) 
			child_thinkness = 2;
		
		//一定概率产生子枝干并递归绘制
		if (randomf()<0.95) {
			// 95%概率产生左子枝干
			float left_child_length = child_length * rand_between(0.9,1.1);
			branch(x_end,y_end,left_child_length,
				angle+offset_angle * rand_between(0.5,1),
				child_thinkness,child_generation
			);	 
		} 
		if (randomf()<0.95) {
			// 95%概率产生右子枝干
			float right_child_length = child_length * rand_between(0.9,1.1);
			branch(x_end,y_end,right_child_length,
				angle-offset_angle * rand_between(0.5,1),
				child_thinkness, child_generation
			);
		}
		if (randomf()<0.85) {
			// 85%概率产生中子枝干
			float middle_child_length = child_length * rand_between(0.8,1.1);
			branch(x_end,y_end,middle_child_length,
				angle+offset_angle/5*rand_between(-1,1),
				child_thinkness, child_generation
			);	 
		}
	} else {
		//绘制樱花
		setlinewidth(1);
		color_t color = HSVtoRGB(rand_between(300,350),rand_between(0.2,0.3),1);
		setfillcolor(color);
		if (child_length<=4) {
			fillellipse(x_end,y_end,2,2);
		} else {
			fillellipse(x_end,y_end,child_length/2,child_length/2);
		}	 
	}	 
	
	if (is_show_animation) {
		delay_ms(1);
	}
}

/**
 * @brief 初始化 
 * 
 **/
void startup() {
	randomize(); //初始化随机数发生器 
	initgraph(WIDTH,HEIGHT);
	setbkcolor(EGERGB(255,255,255));
	cleardevice();
	setrendermode(RENDER_MANUAL);
	branch(WIDTH/2,HEIGHT,0.45 * HEIGHT * shorten_rate,-PI/2,
		15*shorten_rate,1);
}

void update() {
	while (mousemsg()) {
		mouse_msg msg = getmouse();
		if (msg.is_move()) {
			//当鼠标移动时，设置树的参数 
			offset_angle = map_value(msg.x,0,WIDTH,PI/10,PI/4);
			shorten_rate = map_value(msg.y,0,HEIGHT,0.7,0.3);
		} else if(msg.is_down() && msg.is_left()) {
			//当按下鼠标左键时，重新绘制树 
			cleardevice();
			branch(WIDTH/2,HEIGHT,0.45*HEIGHT*shorten_rate,-PI/2,
				15*shorten_rate,1);
			delay_ms(0);
		} else if (msg.is_down() && msg.is_right()){
			//当按下鼠标右键时，切换是否显示动画过程
			is_show_animation = !is_show_animation; 
		}
	}
}

int main() {
	startup();
	while(is_run()) {
		update();
		delay_ms(1); // 防止CPU核被100%占用 
	}
} 
