/*
贪食蛇
原代码使用EasyX，见 https://zhuanlan.zhihu.com/p/211062266
*/
#include <graphics.h>

#define BLOCK_SIZE 20 //每个小格子的长宽大小
#define HEIGHT 30 // 高度上一共30个小格子
#define WIDTH 40 // 宽度上一共40个小格子

//全局变量定义
int blocks[HEIGHT][WIDTH] = {0}; // 记录游戏数据
char move_direction; //小蛇移动方向
int food_i,food_j ; //食物的位置
int is_failure = 0; // 是否游戏失败

/*
移动小蛇及相关处理函数 
*/
void move_snake() {
	int i,j;
	for (i=0;i<HEIGHT;i++){
		for (j=0;j<WIDTH;j++){
			if (blocks[i][j]>0) //是小蛇身体
				blocks[i][j]++; 
		}
	}
	
	int old_tail_i, old_tail_j, old_head_i, old_head_j;  //旧蛇头、蛇尾坐标
	 
	int max = 0;
	 
	for (i=0;i<HEIGHT;i++){
		for (j=0;j<WIDTH;j++){
			if (max<blocks[i][j]){ //旧蛇头位置的值最大 
				max = blocks[i][j];
				old_tail_i = i;
				old_tail_j = j;
			}
			if (2==blocks[i][j]){ //旧蛇头位置的值为2 
				old_head_i = i;
				old_head_j = j;
			}
		}
	}
	
	int new_head_i = old_head_i; //新蛇头位置
	int new_head_j = old_head_j;
	
	switch(move_direction){
		case 'w': // 向上移动
			new_head_i = old_head_i - 1;
			break;
		case 's': //向下移动 
			new_head_i = old_head_i + 1;
			break;
		case 'a': //向左移动
			new_head_j = old_head_j - 1;
			break;
		case 'd': //向右移动
			new_head_j = old_head_j + 1; 
			break;
	} 
	
	//如果蛇头超出边界，或者碰到蛇身，游戏失败
	if ( (new_head_i >= HEIGHT) || (new_head_i < 0) 
		|| ( new_head_j>=WIDTH) || (new_head_j <0 )
		|| (blocks[new_head_i][new_head_j]>0) ) {
			is_failure = 1; //游戏失败
			return; 
	}
	
	blocks[new_head_i][new_head_j] = 1 ; //新蛇头位置数值为1
	if ( (new_head_i == food_i) && (new_head_j == food_j)) { 
		//如果新蛇头正好碰到食物
	 	food_i = random(HEIGHT-5)+2; //重新随机食物位置 
	 	food_j = random(WIDTH-5)+2;
		//不对旧蛇尾处理，相当于蛇的长度+1		
	} else {
		//没有碰到食物
		blocks[old_tail_i][old_tail_j]=0; 
	}
} 

void start_up() {// 程序初始化函数 

	initgraph(WIDTH*BLOCK_SIZE,HEIGHT*BLOCK_SIZE); 
	randomize(); //初始化随机数发生器
	setcolor(EGERGB(200,200,200));
	setrendermode(RENDER_MANUAL); //设置为非自动刷新 (只有delay/delay_fps/getch 才刷新屏幕） 
	 
	blocks[HEIGHT/2][WIDTH/2] = 1; //画面中间画蛇头
	for (int i=1;i<=4;i++){ //向左依次4个蛇身，数值依次为2，3，4，5 
		blocks[HEIGHT/2][WIDTH/2-i]=i+1;
	} 
	move_direction = 'd'; //初始向右移动
	food_i = random(HEIGHT-5)+2; //随机初始化食物位置
	food_j = random(WIDTH-5)+2; 
}

void show() { //绘制函数 
	color_t color;
	cleardevice();
	for (int i=0;i<HEIGHT;i++){
		for (int j=0;j<WIDTH;j++){
			if (blocks[i][j]) {
				//蛇身
				color = HSVtoRGB(blocks[i][j]*10,0.9,1);
			} else {
				//普通格子 
				color = EGERGB(150,150,150);
			}
			setfillcolor(color);
			//i是行(y)，j是列(x) 
			bar(j*BLOCK_SIZE,i*BLOCK_SIZE,(j+1)*BLOCK_SIZE,(i+1)*BLOCK_SIZE);
		}
	} 
	//食物 
	setfillcolor(GREEN);
	bar(food_j*BLOCK_SIZE,food_i*BLOCK_SIZE,(food_j+1)*BLOCK_SIZE,(food_i+1)*BLOCK_SIZE);
	if (is_failure){
		//如果游戏失败
		setcolor(EGERGB(255,0,0));
		setfont(80,0,"宋体");
		xyprintf(240,220,"游戏失败"); 
	} 
}

void update_without_input(){
	//与输入无关的更新函数
	if (is_failure)
		return;
	static int wait_index = 1; 
	wait_index++ ;
	
	if (wait_index == 10) { //每10帧 执行一次，用这个控制小蛇移动速度 
		move_snake();
		wait_index = 1;
	}	 
}

void update_with_input(){
	if (kbhit()) {
		char input = getch(); 
		switch (input){
			case 'w':
			case 'd':
			case 's':
			case 'a': 
				move_direction=input;
		}
	}
}

int main(){
	start_up();
	while (is_run()){
		show();
		update_without_input();
		update_with_input();
		delay_fps(60);
	}
	closegraph();
	return 0;
}
