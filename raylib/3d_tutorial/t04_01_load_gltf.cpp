#include <raylib.h>
#include <raymath.h>

int main(void)
{
	// ��ʼ��
	const int screenWidth = 640;
	const int screenHeight = 480;

	//���÷����
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetTraceLogLevel(LOG_DEBUG);

	//��ʼ������
	InitWindow(screenWidth, screenHeight, "Sample");

	// ��ʼ�������
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 3.0f, 3.0f, 3.0f }; //�������λ��{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //�������λ��{x,y,z}
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f }; //������Ϸ�����ʸ��
	camera.fovy = 40; //�����Ұ���
	camera.projection = CAMERA_PERSPECTIVE; //����͸��ͶӰ

	SetCameraMode(camera, CAMERA_FREE); //�����������ƶ�

	//����������
	Model model = LoadModel("rqq02.obj");

	//���ö���֡�ʣ�ˢ���ʣ�fps��Ϊ30֡/��
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	// ����Ϸѭ��
	while (!WindowShouldClose())    //�رմ��ڻ��߰�ESC��ʱ����true
	{
		UpdateCamera(&camera);
		BeginDrawing();
			ClearBackground(VIOLET);
			//��������ӽǻ���3d����
			BeginMode3D(camera);
				DrawModelWires(model,Vector3{0,0,0},1.0,BLACK);
			EndMode3D();
		EndDrawing();

		//�ͷŲ��ʺ���ͼ
	}
	//�ͷ�����
	UnloadModel(model);
	//�رմ���
	CloseWindow();

	return 0;
}
