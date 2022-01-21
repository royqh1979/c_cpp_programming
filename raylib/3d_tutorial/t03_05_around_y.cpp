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
	camera.position = (Vector3){ 5.0f, 50.0f, 7.0f }; //相机所在位置{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //相机朝向位置{x,y,z}
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; //相机正上方朝向矢量
	camera.fovy = 40; //相机视野宽度
	camera.projection = CAMERA_PERSPECTIVE; //采用透视投影
	
	//创建几何体
	Mesh mesh = GenMeshCube(5,5,15);
	
	//设置动画帧率（刷新率，fps）为30帧/秒
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	double angle;
	
	int colorHue = 0;
	// 主游戏循环
	while (!WindowShouldClose())    //关闭窗口或者按ESC键时返回true
	{
		
		// 每次循环更新一帧
		double time = GetTime();
		angle = time*0.3;
		
		colorHue++;
		colorHue%=360;
		
		//创建贴图
		Image checked = GenImageChecked(2,2,1,1,ColorFromHSV(colorHue,1,1),LIGHTGRAY);
		Texture2D texture = LoadTextureFromImage(checked);
		UnloadImage(checked);
		
		//基于贴图创建材质
		Material material=LoadMaterialDefault();
		material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;
		
		//移动原坐标系的原点到{-10,0,0}，即原来的y轴通过新坐标系的{10,0,0}
		Matrix translate1Transform=MatrixTranslate(-10,0,0);
		//绕原来的y轴旋转，即在新坐标系下绕经过{10,0,0}的轴旋转
		Matrix rotateTransform=MatrixRotateY(angle);
		
		//两个矩阵相乘，得到复合变换矩阵
		Matrix complexTransform =
			MatrixMultiply(translate1Transform,rotateTransform);
		
		BeginDrawing();
		ClearBackground(WHITE);
		//以摄像机视角绘制3d内容
		BeginMode3D(camera);
		DrawGrid(10,5);
		DrawLine3D(Vector3{0,100,0},Vector3{0,-100,0},BLACK);
		//绘制网格
		DrawMesh(mesh,material,complexTransform);
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
