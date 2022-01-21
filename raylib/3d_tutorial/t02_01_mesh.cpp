#include <raylib.h>
#include <raymath.h>

int main(void)
{
	// 初始化
	const int screenWidth = 640;
	const int screenHeight = 480;
	
	//启用反锯齿
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	
	//初始化窗口
	InitWindow(screenWidth, screenHeight, "Sample");
	
	// 初始化摄像机
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 40.0f, 20.0f, 0.0f }; //相机所在位置{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //相机朝向位置{x,y,z}
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //相机正上方朝向矢量
	camera.fovy = 40; //相机视野宽度
	camera.projection = CAMERA_PERSPECTIVE; //采用透视投影
	
	//创建几何体
	Mesh mesh = GenMeshCube(5,5,15);
	
	//设置动画帧率（刷新率，fps）为30帧/秒
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	int angle=0; //多边形旋转角度
	
	int colorHue = 0;
	// 主游戏循环
	while (!WindowShouldClose())    //关闭窗口或者按ESC键时返回true
	{
		
		// 每次循环更新一帧
		double time = GetTime();
		double rotateAngle = time*0.3;
		
		colorHue++;
		colorHue%=360;
		
		//创建贴图
		Image checked = GenImageChecked(2,2,1,1,ColorFromHSV(colorHue,1,1),LIGHTGRAY);
		Texture2D texture = LoadTextureFromImage(checked);
		UnloadImage(checked);
		
		//基于贴图创建材质
		Material material=LoadMaterialDefault();
		material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;
		
		//创建几何变换矩阵
		Matrix transform =MatrixRotateY(rotateAngle);
		
		BeginDrawing();		
			ClearBackground(WHITE);
			//以摄像机视角绘制3d内容
			BeginMode3D(camera);
				//绘制网格
				DrawMesh(mesh,material,transform);			
			EndMode3D();
		EndDrawing();
		
		//释放材质和贴图
		UnloadMaterial(material);
		UnloadTexture(texture);
	}
	//释放网格
	UnloadMesh(mesh);
	//关闭窗口
	CloseWindow();
	
	return 0;
}
