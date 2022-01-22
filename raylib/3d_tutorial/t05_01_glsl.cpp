#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <stdio.h>

int main(void)
{
	// 初始化
	const int screenWidth = 640;
	const int screenHeight = 480;
	
	//启用反锯齿
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetTraceLogLevel(LOG_DEBUG);
	
	//初始化窗口
	InitWindow(screenWidth, screenHeight, "Sample");
	
	// 初始化摄像机
	Camera3D camera = { 0 };
	camera.position = (Vector3){ 20.0f, 30.0f, 20.0f }; //相机所在位置{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //相机朝向位置{x,y,z}
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f }; //相机正上方朝向矢量
	camera.fovy = 40; //相机视野宽度
	camera.projection = CAMERA_PERSPECTIVE; //采用透视投影
	
	SetCameraMode(camera, CAMERA_FREE);
	
	Mesh mesh = GenMeshCube(5,15,5);
	//创建几何体
	Model model = LoadModelFromMesh(mesh);
	
	Shader shader = LoadShader("test01.vs","test01.fs");
	
	
	//设置动画帧率（刷新率，fps）为30帧/秒
	SetTargetFPS(30);
	
	//--------------------------------------------------------------------------------------
	// 主游戏循环
	while (!WindowShouldClose())    //关闭窗口或者按ESC键时返回true
	{
		UpdateCamera(&camera);
		BeginDrawing();
			ClearBackground(VIOLET);
			//以摄像机视角绘制3d内容
			BeginMode3D(camera);
				DrawModel(model, Vector3{0.0f,0.0f,0.0f},1.0,WHITE);
			EndMode3D();
		EndDrawing();
		
		//释放材质和贴图
	}
	UnloadShader(shader);
	UnloadMesh(mesh);
	//释放网格
	UnloadModel(model);
	//关闭窗口
	CloseWindow();
	
	return 0;
}
