/**
 * ���һ����
 *  Դ����ʹ��EasyXʵ�֣���https://zhuanlan.zhihu.com/p/262949768 
 **/ 
 

#include <graphics.h>
#include <stdio.h>
#include <mmsystem.h>
#include <time.h>
#include <math.h>
#define WIDTH	560	//������ 
#define HEIGHT	800	//����߶� 
#define MaxBulletNum	200 //����ӵ�����

/**
 * @brief ��ȷ��ʱ���� 
 * 
 * @param ms ������ 
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
  //�ر�֮ǰ���ŵ����� 
  mciSendString("close tmpmusic",NULL,0,NULL);
  //�������ļ� 
  mciSendString(cmd_str,NULL,0,NULL);
  //�����ļ� 
  mciSendString("play tmpmusic",NULL,0,NULL);
}

class Rocket {
public:
  PIMAGE im_rocket; //���ͼ��
  PIMAGE im_blowup; //��ըͼ��
  float x,y; //�������
  float width,height; //���ͼƬ�Ŀ�ȡ��߶�
  int live_time; //������ʱ�� 
  int life; //����м�����
  
  /**
  * @brief ��ʾ��������Ϣ 
  * 
  **/
  void draw() {
    //�������Ͻ���ʾlife�����ͼƬ����ʾ���������
    for (int i=0;i<life;i++) {
      //ʹ�ô�alpha��͸����ͨ����ͼ 
      putimage_withalpha(NULL,im_rocket,i*width*0.9,0);
    }
    //
    //�������Ϸ���ʾ����˶�����
    setbkmode(TRANSPARENT); //���ֱ���͸��
    setcolor(WHITE); 
    setfont(40,0,"����");
    xyprintf(WIDTH*0.85, 20, "%d��", live_time); 
    
    if (life>0) 
      putimage_withalpha(NULL,im_rocket,x-width/2,y-height/2);
    else
      putimage_withalpha(NULL,im_blowup,x-width/2,y-height/2);
	}
	
	/**
   * @brief ���»��λ������
   * 
   * @param mx 
   * @param my 
   **/
	void update(float mx,float my) {
    x=mx;
    y=my;
  }
  
  /**
   * @brief ���������ʱ�Ĳ���
   * 
   **/
  void updateWhenLifeLost() {
    //���ű�ը��Ч
    PlayMusicOnce("musics\\explode.mp3");
    //��������
    life--;
  }
};

/**
 * �����ӵ���
 **/
class Bullet {
public:
  PIMAGE im_bullet; //�ӵ�ͼ��
  float x,y; //�ӵ�����
  float vx,vy; //�ӵ��ٶ�
  float radius; //�ӵ�����뾶
  
  /**
   * @brief ��ʾ�ӵ�
   * 
   **/
  void draw() {
    putimage_withalpha(NULL,im_bullet,x-radius,y-radius);
  }
  
  /**
   * @brief �����ӵ���λ�á��ٶ�
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
   * @brief �ж��ӵ��Ƿ�ͻ����ײ
   * 
   * @param rocket ���
   * 
   * @return 1��ʾ������ײ��0��ʾû����ײ
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
 * ���ܷɵ��࣬��Bullet����������
 **/
class SmartUFO: public Bullet {
public:
  /**
   * @brief �÷ɵ����ٶ�����Ŀ����
   * 
   * @param targetRocket 
   **/
  void updateVelocityForTarget(Rocket targetRocket) {
    float scalar = randomf() + 1; //�ٶȴ�С��һ���������
    if (targetRocket.x>x) // Ŀ���ڷɵ���ߣ��ɵ����ҷ�
      vx = scalar;
    else if (targetRocket.x<x) // Ŀ���ڷɵ��ұߣ��ɵ������
      vx = -scalar;
    if (targetRocket.y>y) // Ŀ���ڷɵ��·����ɵ����·� 
      vy = scalar;
    else if (targetRocket.y<y) //Ŀ���ڷɵ��Ϸ����ɵ����Ϸ�
      vy = -scalar;
  }
};

PIMAGE im_bk,im_bullet, im_rocket, im_blowup,im_UFO;
Bullet bullet[MaxBulletNum]; //�����ӵ���������
Rocket rocket; //����������
SmartUFO ufo; //����ɵ�����
int bullet_num = 0; // �����ӵ�����

/**
 * @brief ��ʼ������
 * 
 **/
void startup() {
  //�򿪱������ֲ�ѭ������
  mciSendString("open musics\\game_music.mp3 alias bkmusic",NULL,0,NULL);
  mciSendString("play bkmusic repeat", NULL, 0, NULL); 
  
  initgraph(WIDTH,HEIGHT);
  setrendermode(RENDER_MANUAL);
  
  randomize();
  //���뱳��ͼƬ
  im_bk = newimage();
  getimage(im_bk,"pics\\background.png");
  //�����ӵ�ͼƬ
  im_bullet = newimage();
  getimage(im_bullet, "pics\\bullet.png");
  //������ͼƬ
  im_rocket = newimage();
  getimage(im_rocket,"pics\\rocket.png");
  //���뱬ըͼƬ
  im_blowup = newimage();
  getimage(im_blowup,"pics\\blowup.png");
  //����ɵ�ͼƬ
  im_UFO = newimage();
  getimage(im_UFO,"pics\\ufo.png");
  //��ʼ�����
  rocket.im_rocket = im_rocket;
  rocket.im_blowup = im_blowup;
  rocket.width = getwidth(im_rocket);
  rocket.height = getheight(im_rocket);
  rocket.life = 5;
  rocket.x = WIDTH / 2;
  rocket.y = HEIGHT - rocket.height;
  
  //��ʼ���ɵ���Ա����
  ufo.x = WIDTH/2;
  ufo.y = 10;
  ufo.im_bullet = im_UFO;
  ufo.radius = getwidth(im_UFO)/2;
  ufo.updateVelocityForTarget(rocket);
}

/**
 * @brief ���ƺ���
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
  
  //�������Ŀǰһ�������˶�����
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
