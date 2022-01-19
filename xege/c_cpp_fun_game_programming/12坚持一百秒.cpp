/**
 * 坚持一百秒
 *  源程序使用EasyX实现，见https://zhuanlan.zhihu.com/p/262949768 
 **/ 
 

#include <graphics.h>
#include <stdio.h>
#include <mmsystem.h>
#include <time.h>
#include <math.h>
#define WIDTH	560	//画面宽度 
#define HEIGHT	800	//画面高度 
#define MaxBulletNum	200 //最多子弹个数

/**
 * @brief 精确延时函数 
 * 
 * @param ms 毫秒数 
 **/ 
void sleep(DWORD ms) {
  static DWORD oldtime = GetTickCount();
  while (GetTickCount() - oldtime < ms)
    Sleep(1);
  oldtime = GetTickCount();
} 

void PlayMusicOnce(const char* filename) {
  char cmd_str[1000];
  sprintf(cmd_str,"open %s alias tmpmusic",filename);
  //关闭之前播放的音乐 
  mciSendString("close tmpmusic",NULL,0,NULL);
  //打开音乐文件 
  mciSendString(cmd_str,NULL,0,NULL);
  //播放文件 
  mciSendString("play tmpmusic",NULL,0,NULL);
}

class Rocket {
public:
  PIMAGE im_rocket; //火箭图像
  PIMAGE im_blowup; //爆炸图像
  float x,y; //火箭坐标
  float width,height; //火箭图片的宽度、高度
  int live_time; //火箭存活时间 
  int life; //火箭有几条命
  
  /**
  * @brief 显示火箭相关信息 
  * 
  **/
  void draw() {
    //窗口左上角显示life个火箭图片，表示火箭生命数
    for (int i=0;i<life;i++) {
      //使用带alpha（透明）通道贴图 
      putimage_withalpha(NULL,im_rocket,i*width*0.9,0);
    }
    //
    //窗口正上方显示坚持了多少秒
    setbkmode(TRANSPARENT); //文字背景透明
    setcolor(WHITE); 
    setfont(40,0,"黑体");
    xyprintf(WIDTH*0.85, 20, "%d秒", live_time); 
    
    if (life>0) 
      putimage_withalpha(NULL,im_rocket,x-width/2,y-height/2);
    else
      putimage_withalpha(NULL,im_blowup,x-width/2,y-height/2);
	}
	
	/**
   * @brief 更新火箭位置坐标
   * 
   * @param mx 
   * @param my 
   **/
	void update(float mx,float my) {
    x=mx;
    y=my;
  }
  
  /**
   * @brief 当火箭减命时的操作
   * 
   **/
  void updateWhenLifeLost() {
    //播放爆炸音效
    PlayMusicOnce("musics\\explode.mp3");
    //生命减少
    life--;
  }
};

/**
 * 定义子弹类
 **/
class Bullet {
public:
  PIMAGE im_bullet; //子弹图像
  float x,y; //子弹坐标
  float vx,vy; //子弹速度
  float radius; //子弹球体半径
  
  /**
   * @brief 显示子弹
   * 
   **/
  void draw() {
    putimage_withalpha(NULL,im_bullet,x-radius,y-radius);
  }
  
  /**
   * @brief 更新子弹的位置、速度
   * 
   **/
  void update() {
    x += vx;
    y += vy;
    if (x<=0 || x>=WIDTH)
      vx = -vx;
    if (y<=0 || y>=HEIGHT)
      vy = -vy;
  }
  
  /**
   * @brief 判断子弹是否和火箭碰撞
   * 
   * @param rocket 火箭
   * 
   * @return 1表示发生碰撞；0表示没有碰撞
   **/
  int isCollideRocket(Rocket rocket) {
    float distance_x = abs(rocket.x - x);
    float distance_y = abs(rocket.y - y);
    if (distance_x < rocket.width/2 && distance_y < rocket.height/2)
      return 1;
    else
      return 0;
  }
};

/**
 * 智能飞碟类，由Bullet类派生出来
 **/
class SmartUFO: public Bullet {
public:
  /**
   * @brief 让飞碟的速度瞄向目标火箭
   * 
   * @param targetRocket 
   **/
  void updateVelocityForTarget(Rocket targetRocket) {
    float scalar = randomf() + 1; //速度大小有一定的随机性
    if (targetRocket.x>x) // 目标在飞碟左边，飞碟向右飞
      vx = scalar;
    else if (targetRocket.x<x) // 目标在飞碟右边，飞碟向左飞
      vx = -scalar;
    if (targetRocket.y>y) // 目标在飞碟下方，飞碟向下飞 
      vy = scalar;
    else if (targetRocket.y<y) //目标在飞碟上方，飞碟向上飞
      vy = -scalar;
  }
};

PIMAGE im_bk,im_bullet, im_rocket, im_blowup,im_UFO;
Bullet bullet[MaxBulletNum]; //定义子弹对象数组
Rocket rocket; //定义火箭对象
SmartUFO ufo; //定义飞碟对象
int bullet_num = 0; // 已有子弹个数

/**
 * @brief 初始化函数
 * 
 **/
void startup() {
  //打开背景音乐并循环播放
  mciSendString("open musics\\game_music.mp3 alias bkmusic",NULL,0,NULL);
  mciSendString("play bkmusic repeat", NULL, 0, NULL); 
  
  initgraph(WIDTH,HEIGHT);
  setrendermode(RENDER_MANUAL);
  
  randomize();
  //导入背景图片
  im_bk = newimage();
  getimage(im_bk,"pics\\background.png");
  //导入子弹图片
  im_bullet = newimage();
  getimage(im_bullet, "pics\\bullet.png");
  //导入火箭图片
  im_rocket = newimage();
  getimage(im_rocket,"pics\\rocket.png");
  //导入爆炸图片
  im_blowup = newimage();
  getimage(im_blowup,"pics\\blowup.png");
  //导入飞碟图片
  im_UFO = newimage();
  getimage(im_UFO,"pics\\ufo.png");
  //初始化火箭
  rocket.im_rocket = im_rocket;
  rocket.im_blowup = im_blowup;
  rocket.width = getwidth(im_rocket);
  rocket.height = getheight(im_rocket);
  rocket.life = 5;
  rocket.x = WIDTH / 2;
  rocket.y = HEIGHT - rocket.height;
  
  //初始化飞碟成员变量
  ufo.x = WIDTH/2;
  ufo.y = 10;
  ufo.im_bullet = im_UFO;
  ufo.radius = getwidth(im_UFO)/2;
  ufo.updateVelocityForTarget(rocket);
}

/**
 * @brief 绘制函数
 * 
 **/
void show(){
  putimage(0,0,im_bk); 
  ufo.draw();
  for (int i=0;i<bullet_num;i++) {
    bullet[i].draw();
  }
  rocket.draw();
}

void updateWithoutInput() {
  if (rocket.life<=0)
    return;
  
  static int last_second = 0;
  static int now_second = 0;
  static clock_t start = clock();
  clock_t now = clock(); 
  
  //计算程序目前一共运行了多少秒
  now_second = ( (int)(now-start) / CLOCKS_PER_SEC);
  rocket.live_time = now_second;
  if (now_second == last_second+1) 
    ufo.updateVelocityForTarget(rocket);
    
  if (now_second == last_second+2) {
    last_second = now_second;
    if (bullet_num<MaxBulletNum) {
      bullet[bullet_num].x = WIDTH / 2;
      bullet[bullet_num].y = 10;
      float angle = (randomf()-0.5)*0.9*PI;
      float scalar = 2*randomf() + 2;
      bullet[bullet_num].vx = scalar * sin(angle);
      bullet[bullet_num].vy = scalar * cos(angle);
      bullet[bullet_num].im_bullet = im_bullet;
      bullet[bullet_num].radius = getwidth(im_bullet)/2;
      bullet_num++;
    }
  }
  
  for (int i=0;i<bullet_num;i++) {
    bullet[i].update();
    if (bullet[i].isCollideRocket(rocket)) {
      rocket.updateWhenLifeLost();
      bullet[i].x = 5;
      bullet[i].y = 5;
      break;
    }
  }
  
  ufo.update();
  if (ufo.isCollideRocket(rocket)) {
    rocket.updateWhenLifeLost();
    ufo.x = 5;
    ufo.y = 5;
  }
}

void updateWithInput() {
  if (rocket.life <= 0) 
    return ;
  mouse_msg msg;
  while (mousemsg()) {
    msg = getmouse();
    if (msg.is_move()) 
      rocket.update(msg.x, msg.y);
  }
}

int main() {
  startup();
  for(;is_run();delay_fps(30)) {
    updateWithInput();
    updateWithoutInput();
    show();
  }
  return 0;
}
