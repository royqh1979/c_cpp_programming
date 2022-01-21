#include <raylib.h>
#include <math.h>
#include <raymath.h>

int main(void)
{
	// ��ʼ��
	const int screenWidth = 640;
	const int screenHeight = 480;

	//���÷����
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	//��ʼ������
	InitWindow(screenWidth, screenHeight, "Sample");

	// ��ʼ�������
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 40.0f, 20.0f, 0.0f }; //�������λ��{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //�������λ��{x,y,z}
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //������Ϸ�����ʸ��


	camera.fovy = 70.0f; //�����Ұ���
	camera.projection = CAMERA_PERSPECTIVE; //����͸��ͶӰ

	//���ö���֡�ʣ�ˢ���ʣ�fps��Ϊ30֡/��
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	int angle=0; //�������ת�Ƕ�

	// ����Ϸѭ��
	while (!WindowShouldClose())    //�رմ��ڻ��߰�ESC��ʱ����true
	{

		double time = GetTime();
		// ÿ��ѭ������һ֡
		// �����Χ��y��ת��
		double cameraTime = time*0.3;
		camera.position.x = (float)cos(cameraTime)*40.0f;
		camera.position.z = (float)sin(cameraTime)*40.0f;

		BeginDrawing();

			ClearBackground(WHITE);
			//��������ӽǻ���3d����
			BeginMode3D(camera);
				//����ˮƽ������
				DrawGrid(100,5);
				//����Y��
				DrawLine3D(Vector3{0,100,0},Vector3{0,-100,0},BLACK);
				//����������
				DrawCube(Vector3{0,0,0},10,10,10,VIOLET);
				DrawCubeWires(Vector3{0,0,0},10,10,10,BLACK);
				//��������
				DrawSphere(Vector3{0,-40,0},10,RED);
				DrawSphereWires(Vector3{0,-40,0},10,10,10,BLACK);
			EndMode3D();
		EndDrawing();
	}

	//�رմ���
	CloseWindow();

	return 0;
}


