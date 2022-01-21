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
	camera.position = (Vector3){ 40.0f, 20.0f, 0.0f }; //�������λ��{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //�������λ��{x,y,z}
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //������Ϸ�����ʸ��
	camera.fovy = 40; //�����Ұ���
	camera.projection = CAMERA_PERSPECTIVE; //����͸��ͶӰ
	
	//����������
	Mesh mesh = GenMeshCube(5,5,15);
	
	//���ö���֡�ʣ�ˢ���ʣ�fps��Ϊ30֡/��
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	//��������
	double scale = 0.1;
	double step = 0.1;
	
	int colorHue = 0;
	// ����Ϸѭ��
	while (!WindowShouldClose())    //�رմ��ڻ��߰�ESC��ʱ����true
	{
		
		// ÿ��ѭ������һ֡
		double time = GetTime();
		scale+=step;
		if (scale>2.9) {
			step=-0.1;
		} else if (scale<0.2) {
			step=0.1;
		}
		
		colorHue++;
		colorHue%=360;
		
		//������ͼ
		Image checked = GenImageChecked(2,2,1,1,ColorFromHSV(colorHue,1,1),LIGHTGRAY);
		Texture2D texture = LoadTextureFromImage(checked);
		UnloadImage(checked);
		
		//������ͼ��������
		Material material=LoadMaterialDefault();
		material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;
		
		//�������α任����
		Matrix transform=MatrixScale(scale,scale,scale);
		
		BeginDrawing();		
			ClearBackground(WHITE);
			//��������ӽǻ���3d����
			BeginMode3D(camera);
				//��������
				DrawMesh(mesh,material,transform);			
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
