#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <stdio.h>

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
	camera.position = (Vector3){ 20.0f, 30.0f, 20.0f }; //�������λ��{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //�������λ��{x,y,z}
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f }; //������Ϸ�����ʸ��
	camera.fovy = 40; //�����Ұ���
	camera.projection = CAMERA_PERSPECTIVE; //����͸��ͶӰ
	
	SetCameraMode(camera, CAMERA_FREE);
	
	Mesh mesh = GenMeshCube(5,15,5);
	//����������
	Model model = LoadModelFromMesh(mesh);
	
	Shader shader = LoadShader("test01.vs","test01.fs");
	
	
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
				DrawModel(model, Vector3{0.0f,0.0f,0.0f},1.0,WHITE);
			EndMode3D();
		EndDrawing();
		
		//�ͷŲ��ʺ���ͼ
	}
	UnloadShader(shader);
	UnloadMesh(mesh);
	//�ͷ�����
	UnloadModel(model);
	//�رմ���
	CloseWindow();
	
	return 0;
}
