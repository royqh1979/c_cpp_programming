#include <raylib.h>
#include <raymath.h>

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
	camera.position = (Vector3){ 3.0f, 3.0f, 3.0f }; //相机所在位置{x,y,z}
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; //相机朝向位置{x,y,z}
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f }; //相机正上方朝向矢量
	camera.fovy = 40; //相机视野宽度
	camera.projection = CAMERA_PERSPECTIVE; //采用透视投影

	SetCameraMode(camera, CAMERA_FREE); //相机可用鼠标移动

	//创建几何体
	Model model = LoadModel("rqq02.obj");

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
				DrawModelWires(model,Vector3{0,0,0},1.0,BLACK);
			EndMode3D();
		EndDrawing();

		//释放材质和贴图
	}
	//释放网格
	UnloadModel(model);
	//关闭窗口
	CloseWindow();

	return 0;
}
