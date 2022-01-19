#include <graphics.h>
#include <mmsystem.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

#define WIDTH	1000	//窗口高度
#define HEIGHT	700		//窗口宽度
#define RADIUS	25		//小球半径
#define COLOR_NUM	5	//小球颜色种类数目
color_t c = RED;

color_t colors[COLOR_NUM] = {RED,BLUE,GREEN,YELLOW,MAGENTA};

/**
 * @brief 求两点之间距离
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * 
 * @return 
 **/
float Distance(float x1,float y1, float x2, float y2){
	float xd = x2-x1;
	float yd = y2-y1;
	return sqrt(xd*xd+yd*yd);
}

/**
 * @brief 播放一次音乐文件
 * 
 * @param filename 
 **/
void PlayMusicOnce(const char* filename) {
	char cmdString[1000];
	sprintf(cmdString,"open %s alias tmpmusic", filename);
	mciSendString("close tmpmusic",NULL,0,NULL);
	mciSendString(cmdString,NULL,0,NULL);
	mciSendString("play tmpmusic",NULL,0,NULL);
	mciSendString("close tmpmusic",NULL,0,NULL);
}

/**
 * 顶点类
 **/
class Point {
public:
	float x,y;
	Point() {
		x=0;
		y=0;
	}
	Point(float ix,float iy) {
		x = ix;
		y = iy;
	}
};

/**
 * 轨迹类
 **/
class Path {
public :
	vector<Point> keyPoints; //记录轨迹上的关键点，关键点之间以直线相连
	float sampleInterval; // 对特征点连成的轨迹线，进行均匀采样的间隔
	vector<Point> allPoints; //所有以采样间隔得到的采样点
	
	/**
	 * @brief 以采样间隔进行采样，得到所有的采样点
	 * 
	 **/
	void getAllPoints() {
		int i;
		//对关键字依次连接形成的多条线段进行遍历
		for (i=0;i<keyPoints.size()-1;i++) {
			float xd = keyPoints[i+1].x - keyPoints[i].x;
			float yd = keyPoints[i+1].y - keyPoints[i].y;
			float length = sqrt(xd*xd + yd*yd); //两个关键点构成的线段长度
			
			int num = length / sampleInterval; // 这条线段上的采样点个数
			for (int j=0;j<num;j++) {
				float x_sample = keyPoints[i].x + j*xd/num;
				float y_sample = keyPoints[i].y + j*yd/num;
				allPoints.push_back(Point(x_sample,y_sample));
			}
		}
		// 还有最后一个关键点
		allPoints.push_back(Point(keyPoints[i].x,keyPoints[i].y));
	}
	
	void draw() {
		setcolor(EGERGB(0,0,0)); 
		setfillcolor(EGERGB(0,0,0));
		//画出关键点依次连接形成的线段
		for (int i=0;i<keyPoints.size()-1;i++) {
			line(keyPoints[i].x,keyPoints[i].y,keyPoints[i+1].x,keyPoints[i+1].y);
		}
		//所有采样点处，分别画一个小圆点
		for (int i=0;i<allPoints.size();i++) {
			fillellipse(allPoints[i].x,allPoints[i].y,2,2);
		}
	}
	~Path() {
		keyPoints.clear();
		allPoints.clear();
	}
};

/**
 * 小球类
 **/
class Ball {
public:
	Point center; //圆心坐标
	float radius; //半径
	int colorId; //小球的颜色序号，用于在colors数组中取颜色
	int indexInPath; //小球位置在Path的allPoints中的对应序号
	int direction; // 小球移动方向，1向终点，-1向起点，0暂停
	
	
	void draw() {
		setcolor(colors[colorId]);
		setfillcolor(colors[colorId]);
		fillellipse(center.x,center.y,radius,radius);
	}
	
	/**
	 * @brief 让小球移动到path的allPoints中，indexInPath序号处理的轨迹点
	 * 
	 * @param path 
	 **/
	void moveToIndexInPath(Path path) {
		center = path.allPoints[indexInPath];
	}
	
	/**
	 * @brief 初始化小球到最初的位置上
	 * 
	 * @param path 
	 **/
	void initiate(Path path) {
		radius = RADIUS;
		indexInPath	= 0;
		direction = 0;
		moveToIndexInPath(path);
		colorId = random(COLOR_NUM);
	}
	
	/**
	 * @brief 让小球沿着轨迹Path移动，注意不要越界
	 * 
	 * @param path 
	 **/
	void changeIndexByDirection(Path path) {
		if (direction == 1) {
			if (indexInPath+1 < path.allPoints.size()) 
				indexInPath++;
		} else if (direction == -1){
			if (indexInPath-1>=0) 
				indexInPath--;
		}
	}
};

/**
 * 炮台类
 **/
class Cannon {
public:
	PIMAGE im;
	PIMAGE im_rotate;
	float x,y;
	
};
