/*
十字消除

原程序使用EasyX，见https://zhuanlan.zhihu.com/p/257805032
*/
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE 40 //小方块的长宽大小
#define COLOR_TYPE_NUM 9 //除了空白方块外，其他方块的颜色个数
#define GAME_RECORD_FILE (".\\game_record.dat")

//小方块结构体 
struct Block {
	int x,y; // x y 坐标
	int color_id; //对应的颜色下标
	int i,j; // 小方块在blocks数组中的下标	
};
//全局变量
 
int row_num;
int col_num; 
Block **blocks = NULL ; //动态二维数组指针，存储所有方块数据
COLORREF colors[COLOR_TYPE_NUM+1]; //方块儿颜色数组 
int	score; //得分数，即消去得方块个数
float max_time; //本关游戏总时长
float total_time; //减去扣分项后的游戏总时长
float remain_time; //剩余时间
clock_t start, finish; //计时变量
int level = 1 ; //当前关卡序号
int non_zero_block_count; //非空白区域的砖块个数 

/**
 * @brief 绘制提示框 
 * 
 * @param i 方块下标 
 * @param j 方块下标 
 * @param color 方块颜色 
 * @param isfill 是否填充 
 **/
void draw_block_hint(int i, int j, COLORREF color, int isfill){
	setcolor(color);
	setfillcolor(color);
	if (isfill==1) { 
		fillrect(blocks[i][j].x,blocks[i][j].y,
			blocks[i][j].x+BLOCK_SIZE,blocks[i][j].y+BLOCK_SIZE);		
	} else {
		rectangle(blocks[i][j].x,blocks[i][j].y,
			blocks[i][j].x+BLOCK_SIZE,blocks[i][j].y+BLOCK_SIZE);
	}
}

/**
 * @brief 保存最高分数到数据文件 
 * 
 * @param record_score 最高分 
 **/
void write_record_file(int record_score) {
	FILE *fp;
	fp = fopen(GAME_RECORD_FILE,"w");
	if (fp == NULL) {
		//文件打开失败 
		return;
	}
	fprintf(fp,"%d",record_score);
	fclose(fp);
} 

/**
 * @brief 从数据文件读入最高分数
 *
 *
 * @return
 **/
int read_record_file() {
	int record_score;
	FILE* fp;
	fp = fopen(GAME_RECORD_FILE,"r");
	if (fp==NULL){
		//文件打开失败 
		return 0;
	}
	fscanf(fp,"%d",&record_score);
	fclose(fp);
	return record_score;
}

/**
 * @brief 初始化函数
 *
 **/
void startup(){
	int i,j;
	start = clock(); //记录当前运行时刻
	
	//如果不是第1关，则先释放内存
	if (level>1) {
		for (i=0;i<row_num;i++)
			free(blocks[i]);
		free(blocks);
	}
	//根据是第几关，调整这一关对应的游戏画面大小
	row_num = 12 + level / 2;
	col_num = 20 + level;
	
	//动态内存分配
	blocks = (Block **)malloc(row_num * sizeof(Block *));
	for (i=0;i<row_num;i++)
		blocks[i]=(Block *)malloc(col_num*sizeof(Block));
	
	//本关限时
	max_time = 200 + level * 10;
	total_time = max_time;
	
	int width = BLOCK_SIZE * col_num;
	int height = BLOCK_SIZE * (row_num+3);
	initgraph(width,height);
	setrendermode(RENDER_MANUAL);
	setbkcolor(EGERGB(220,220,220));
	setlinewidth(2);
	cleardevice();
	randomize();
	//得分数，也就是消去的方块个数 
	score=0;
	//空白区域的砖块个数 
	non_zero_block_count = 0;	
	
	//预设颜色 
	colors[0] = EGERGB(220,220,220);
	for (i=1;i<=COLOR_TYPE_NUM;i++){
		colors[i]=HSVtoRGB((i-1)*40,0.6,0.8);
	}
	
	//初始化blocks数组
	for (i=0;i<row_num;i++){
		for (j=0;j<col_num;j++){
			//随机方块颜色，1~COLOR_TYPE_NUM 为彩色，其他为空白方块
			//空白方块的比例约占总数量的1/3 
			int t = random(int(COLOR_TYPE_NUM * 1.5));
			if (t<=COLOR_TYPE_NUM)
				blocks[i][j].color_id = t;
			else
				blocks[i][j].color_id = 0;
			//方块坐标 
			blocks[i][j].x = j*BLOCK_SIZE;
			blocks[i][j].y = i*BLOCK_SIZE;
			//方块在blocks数组中的下标 
			blocks[i][j].i = i;
			blocks[i][j].j = j;
			if (blocks[i][j].color_id !=0) 
				non_zero_block_count++;
				
			 
		}
	} 
	
}

/**
 * @brief 绘制函数 
 * 
 **/
void show(){
	cleardevice();
	setcolor(EGERGB(255,255,255)); 
	//绘制所有小方块 
	for (int i=0;i<row_num;i++) {
		for (int j=0;j<col_num;j++){
			setfillcolor(colors[blocks[i][j].color_id]);
			bar(blocks[i][j].x, blocks[i][j].y,
				blocks[i][j].x+BLOCK_SIZE, blocks[i][j].y+BLOCK_SIZE);
		}	
	}
	//绘制剩余时间倒计时条
	setcolor(EGERGB(255,0,0));
	setfillcolor(EGERGB(255,0,0));
	bar(0,BLOCK_SIZE*(row_num+0.2),
		remain_time*BLOCK_SIZE*col_num/max_time, 
		BLOCK_SIZE*(row_num+0.8));
	
	//输出得分文字
	setcolor(EGERGB(0,0,0));
	setfont(22,0,"宋体");
	xyprintf(BLOCK_SIZE * (col_num/2 - 0.1), BLOCK_SIZE *(row_num+0.2),
		"%d",score);
	//输出游戏提示信息
	xyprintf(BLOCK_SIZE * (col_num/15.0), BLOCK_SIZE *(row_num+1.2), 
		"点击空白方块，其十字区域有两个或以上相同颜色方块则消除；不能消除扣时间");
	xyprintf(BLOCK_SIZE * (col_num/5.0), BLOCK_SIZE*(row_num+2.2), 
		"目前第 %d 关，时间结束前得分达到 %d 可进入下一关"
		,level,int(non_zero_block_count*0.9));
	//刷新 
	delay_ms(0);
} 

/**
 * @brief 和输入无关的更新 
 * 
 **/
void update_without_input(){
	// 倒计时减少
	// 获取当前时刻 
	finish = clock(); 
	//  计算本关游戏已运行时间
	double duration = (double)(finish -start) / CLOCKS_PER_SEC;
	//计算游戏剩余时间 
	remain_time = total_time - duration;
	
	//如果已超时 
	if (remain_time <= 0 ){
		if (score > read_record_file()){
			write_record_file(score);
			
			show();
			setcolor(EGERGB(255,0,0));
			setfont(100,0,"黑体");
			xyprintf(BLOCK_SIZE * (col_num/30.0), BLOCK_SIZE *(row_num/3.0),
				"恭喜打破得分记录！");
			delay_ms(2000);
		}
		//如果消除掉了90%的非空白方块，进入下一关 
		if (score>=int(non_zero_block_count*0.9)) 
			level++; 
		startup();
	}
}

/**
 * @brief 和输入有关的更新 
 * 
 **/
void update_with_input() {
	if (remain_time <= 0) {
		//时间已到，停止操作 
		return;
	}	
	int clicked = 0;
	mouse_msg m,temp_m; 
	while (mousemsg()) {
		temp_m=getmouse();
		if (temp_m.is_up() and temp_m.is_left()) {
			clicked = 1;
			m=temp_m;
		}
	}
	if ( clicked ) {
		//当按下鼠标左键时
		//获得点击的小方块的下标
		int clicked_i = int(m.y)/BLOCK_SIZE;
		int clicked_j = int(m.x)/BLOCK_SIZE;
		//点到的是提示部分，不处理 
		if (clicked_i>=row_num)
			return;
		//如果当前点击的不是空白方块，不需要处理
		if (blocks[clicked_i][clicked_j].color_id!=0)
			return;
		
		show();
		 
		draw_block_hint(clicked_i,clicked_j,EGERGB(100,100,100),1);
		
		//存储上下左右四个方向找到的第一个不是空白的方块 
		Block four_blocks[4];
		
		// 向上找 
		four_blocks[0].color_id = 0; 
		for (int search=1;clicked_i-search >= 0; search++){
		    //找到了 
            if (blocks[clicked_i-search][clicked_j].color_id!=0) {
                four_blocks[0]=blocks[clicked_i-search][clicked_j];
                break;
            }
        } 
        //向下找 
        four_blocks[1].color_id = 0;
        for (int search=1;clicked_i+search < row_num; search++) {                                       
            //找到了
            if (blocks[clicked_i+search][clicked_j].color_id!=0){
                four_blocks[1]=blocks[clicked_i+search][clicked_j];
                break;
            }                 
        }
        //向左找
		four_blocks[2].color_id = 0; 
        for (int search=1;clicked_j - search >= 0; search++){
            if (blocks[clicked_i][clicked_j-search].color_id!=0) {
				four_blocks[2] = blocks[clicked_i][clicked_j-search];
				break;
			}                
        }
        //向右找
		four_blocks[3].color_id = 0;
		for (int search=1;clicked_j + search < col_num; search++) {
			if (blocks[clicked_i][clicked_j+search].color_id!=0) {
				four_blocks[3] = blocks[clicked_i][clicked_j+search];
				break;
			}
		}
		
		// 统计four_blocks的四个小方块，有没有同样颜色数目大于等于2的
		int color_statistics[COLOR_TYPE_NUM+1]={0};
		// 点击的方块是否合适（能够消除），缺省为否 
		int is_bad_click = 1;
		for (int i=1;i<COLOR_TYPE_NUM+1;i++) {
			for (int j=0;j<4;j++){
				if (four_blocks[j].color_id == i) 
					color_statistics[i]++;
			}
			if (color_statistics[i]>=2) { 
				//如果这种颜色的方块数不止1个 
				is_bad_click = 0; 
				//把对应十字区域要消除的方块颜色改成空白 
				for (int j=0;j<4;j++) {
					if (four_blocks[j].color_id == i) {
						draw_block_hint(four_blocks[j].i,four_blocks[j].j,EGERGB(0,0,0),0);
						blocks[four_blocks[j].i][four_blocks[j].j].color_id=0;
					}
				}
				score += color_statistics[i]; 
			}
		}
		//错误点击，减去十秒钟时间 
		if (is_bad_click==1)
			total_time -= 10;
		delay_ms(300);
	}	
} 

int main() {
	startup();
	while (is_run()) {
		show();
		update_without_input();
		update_with_input();
		delay_fps(30);
	}
	closegraph();
	return 0;
}
