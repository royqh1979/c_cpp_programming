#include <graphics.h>
#include <mmsystem.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

#define WIDTH	1000	//���ڸ߶�
#define HEIGHT	700		//���ڿ��
#define RADIUS	25		//С��뾶
#define COLOR_NUM	5	//С����ɫ������Ŀ
color_t c = RED;

color_t colors[COLOR_NUM] = {RED,BLUE,GREEN,YELLOW,MAGENTA};

/**
 * @brief ������֮�����
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
 * @brief ����һ�������ļ�
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
 * ������
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
 * �켣��
 **/
class Path {
public :
	vector<Point> keyPoints; //��¼�켣�ϵĹؼ��㣬�ؼ���֮����ֱ������
	float sampleInterval; // �����������ɵĹ켣�ߣ����о��Ȳ����ļ��
	vector<Point> allPoints; //�����Բ�������õ��Ĳ�����
	
	/**
	 * @brief �Բ���������в������õ����еĲ�����
	 * 
	 **/
	void getAllPoints() {
		int i;
		//�Թؼ������������γɵĶ����߶ν��б���
		for (i=0;i<keyPoints.size()-1;i++) {
			float xd = keyPoints[i+1].x - keyPoints[i].x;
			float yd = keyPoints[i+1].y - keyPoints[i].y;
			float length = sqrt(xd*xd + yd*yd); //�����ؼ��㹹�ɵ��߶γ���
			
			int num = length / sampleInterval; // �����߶��ϵĲ��������
			for (int j=0;j<num;j++) {
				float x_sample = keyPoints[i].x + j*xd/num;
				float y_sample = keyPoints[i].y + j*yd/num;
				allPoints.push_back(Point(x_sample,y_sample));
			}
		}
		// �������һ���ؼ���
		allPoints.push_back(Point(keyPoints[i].x,keyPoints[i].y));
	}
	
	void draw() {
		setcolor(EGERGB(0,0,0)); 
		setfillcolor(EGERGB(0,0,0));
		//�����ؼ������������γɵ��߶�
		for (int i=0;i<keyPoints.size()-1;i++) {
			line(keyPoints[i].x,keyPoints[i].y,keyPoints[i+1].x,keyPoints[i+1].y);
		}
		//���в����㴦���ֱ�һ��СԲ��
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
 * С����
 **/
class Ball {
public:
	Point center; //Բ������
	float radius; //�뾶
	int colorId; //С�����ɫ��ţ�������colors������ȡ��ɫ
	int indexInPath; //С��λ����Path��allPoints�еĶ�Ӧ���
	int direction; // С���ƶ�����1���յ㣬-1����㣬0��ͣ
	
	
	void draw() {
		setcolor(colors[colorId]);
		setfillcolor(colors[colorId]);
		fillellipse(center.x,center.y,radius,radius);
	}
	
	/**
	 * @brief ��С���ƶ���path��allPoints�У�indexInPath��Ŵ���Ĺ켣��
	 * 
	 * @param path 
	 **/
	void moveToIndexInPath(Path path) {
		center = path.allPoints[indexInPath];
	}
	
	/**
	 * @brief ��ʼ��С�������λ����
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
	 * @brief ��С�����Ź켣Path�ƶ���ע�ⲻҪԽ��
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
 * ��̨��
 **/
class Cannon {
public:
	PIMAGE im;
	PIMAGE im_rotate;
	float x,y;
	
};
