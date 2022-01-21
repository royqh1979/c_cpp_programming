#include <raylib.h>
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
	camera.position = (Vector3){ 5.0f, 50.0f, 7.0f }; //�������λ��{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //�������λ��{x,y,z}
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //������Ϸ�����ʸ��
	camera.fovy = 40; //�����Ұ���
	camera.projection = CAMERA_PERSPECTIVE; //����͸��ͶӰ
	
	//����������
	Mesh mesh = GenMeshCube(5,5,15);
	
	//���ö���֡�ʣ�ˢ���ʣ�fps��Ϊ30֡/��
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	double angle;
	
	int colorHue = 0;
	// ����Ϸѭ��
	while (!WindowShouldClose())    //�رմ��ڻ��߰�ESC��ʱ����true
	{
		
		// ÿ��ѭ������һ֡
		double time = GetTime();
		angle = time*0.3;
		
		colorHue++;
		colorHue%=360;
		
		//������ͼ
		Image checked = GenImageChecked(2,2,1,1,ColorFromHSV(colorHue,1,1),LIGHTGRAY);
		Texture2D texture = LoadTextureFromImage(checked);
		UnloadImage(checked);
		
		//������ͼ��������
		Material material=LoadMaterialDefault();
		material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;
		
		//�ƶ�ԭ����ϵ��ԭ�㵽{-10,0,0}����ԭ����y��ͨ��������ϵ��{10,0,0}
		Matrix translate1Transform=MatrixTranslate(-10,0,0);
		//��ԭ����y����ת������������ϵ���ƾ���{10,0,0}������ת
		Matrix rotateTransform=MatrixRotateY(angle);
		
		//����������ˣ��õ����ϱ任����
		Matrix complexTransform =
			MatrixMultiply(translate1Transform,rotateTransform);
		
		BeginDrawing();
		ClearBackground(WHITE);
		//��������ӽǻ���3d����
		BeginMode3D(camera);
		DrawGrid(10,5);
		DrawLine3D(Vector3{0,100,0},Vector3{0,-100,0},BLACK);
		//��������
		DrawMesh(mesh,material,complexTransform);
		EndMode3D();
		EndDrawing();
		
		//�ͷŲ��ʺ���ͼ
		UnloadMaterial(material);
		UnloadTexture(texture);
	}
	//�ͷ�����
	UnloadMesh(mesh);
	//�رմ���
	CloseWindow();
	
	return 0;
}
