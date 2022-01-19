/**
 * ӣ���� 
 * ԭ����ʹ��EasyX����https://zhuanlan.zhihu.com/p/260285711
 **/
#include <graphics.h>

#include <math.h>

#define WIDTH 800
#define HEIGHT 600

//����֦�ɺ͸�֦��ƫ��Ļ��� 
float offset_angle = PI /6; 

//��֦�ɱȸ�֦�ɱ�̵ı��� 
float shorten_rate = 0.65;

//�Ƿ���ʾ�����ɵĹ��̶��� 
int is_show_animation = 1;


/**
 * @brief ӳ�亯�� 
 * 
 * ��[input_min, input_max]��Χ�ڵ�input������
 * ӳ�䵽[output_min, output_max]��Χ
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
		output = output_min; // ��ֹinput_max - input_minΪ�� 
	else
		output = ( (input-input_min) * (output_max-output_min) 
			/ (input_max - input_min) ) + output_min; 
	return output;	
} 

/**
 * @brief ����min��max֮�������� 
 * 
 * @param min 
 * @param max 
 * 
 * @return 
 **/
float rand_between(float min, float max) {
	//���ɷ���[0,1)������ȷֲ�������� 
	float t = randomf();
	
	//ʹ��map_value��tӳ�䵽[min,max)������ 
	return map_value(t,0,1,min,max);	
}

/**
 * @brief ֦�����ɺͻ��� 
 * 
 * @param x_start	֦����ʼ������ 
 * @param y_start	֦����ʼ������ 
 * @param length	֦�ɳ��� 
 * @param angle 	֦�ɽǶ� 
 * @param thinckness 	֦��������� 
 * @param generation ֦�ɴ��������ڿ��Ƶݹ���� 
 **/
void branch(float x_start, float y_start, float length, 
	float angle, float thickness, int generation) {
	//��ǰ֦�ɵ��յ����� 
	float x_end, y_end;	
	x_end = x_start + length * cos(angle);
	y_end = y_start + length * sin(angle);
	
	//��֦��	
	
	//�趨��ǰ֦�ɿ�Ⱥ���ɫ 
	setlinewidth(thickness);	
	color_t color = HSVtoRGB(15,0.75,0.4+generation*0.05);
	setcolor(color); 
	
	line(x_start,y_start, x_end, y_end);
	
	int child_generation = generation + 1;
	float child_length = shorten_rate * length;
	
	if (child_length >= 2 && child_generation <=9) {
		//���ݹ����С��10���������ɳ����㹻��ʱ�� �ݹ����������
		
		float child_thinkness = thickness * 0.8;
		if (child_thinkness < 2) 
			child_thinkness = 2;
		
		//һ�����ʲ�����֦�ɲ��ݹ����
		if (randomf()<0.95) {
			// 95%���ʲ�������֦��
			float left_child_length = child_length * rand_between(0.9,1.1);
			branch(x_end,y_end,left_child_length,
				angle+offset_angle * rand_between(0.5,1),
				child_thinkness,child_generation
			);	 
		} 
		if (randomf()<0.95) {
			// 95%���ʲ�������֦��
			float right_child_length = child_length * rand_between(0.9,1.1);
			branch(x_end,y_end,right_child_length,
				angle-offset_angle * rand_between(0.5,1),
				child_thinkness, child_generation
			);
		}
		if (randomf()<0.85) {
			// 85%���ʲ�������֦��
			float middle_child_length = child_length * rand_between(0.8,1.1);
			branch(x_end,y_end,middle_child_length,
				angle+offset_angle/5*rand_between(-1,1),
				child_thinkness, child_generation
			);	 
		}
	} else {
		//����ӣ��
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
 * @brief ��ʼ�� 
 * 
 **/
void startup() {
	randomize(); //��ʼ������������� 
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
			//������ƶ�ʱ���������Ĳ��� 
			offset_angle = map_value(msg.x,0,WIDTH,PI/10,PI/4);
			shorten_rate = map_value(msg.y,0,HEIGHT,0.7,0.3);
		} else if(msg.is_down() && msg.is_left()) {
			//������������ʱ�����»����� 
			cleardevice();
			branch(WIDTH/2,HEIGHT,0.45*HEIGHT*shorten_rate,-PI/2,
				15*shorten_rate,1);
			delay_ms(0);
		} else if (msg.is_down() && msg.is_right()){
			//����������Ҽ�ʱ���л��Ƿ���ʾ��������
			is_show_animation = !is_show_animation; 
		}
	}
}

int main() {
	startup();
	while(is_run()) {
		update();
		delay_ms(1); // ��ֹCPU�˱�100%ռ�� 
	}
} 
