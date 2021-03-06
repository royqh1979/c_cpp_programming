#include <raylib.h>
#include <math.h>
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
	Vector3 up = Vector3CrossProduct(Vector3Subtract(camera.target,camera.position),(Vector3){ 0.0f, 1.0f, 0.0f });
	camera.up = up; //相机正上方朝向矢量
	
	
	camera.fovy = 70.0f; //相机视野宽度
	camera.projection = CAMERA_PERSPECTIVE; //采用透视投影
	
	//设置动画帧率（刷新率，fps）为30帧/秒
	SetTargetFPS(30);
	//--------------------------------------------------------------------------------------
	int angle=0; //多边形旋转角度
	
	// 主游戏循环
	while (!WindowShouldClose())    //关闭窗口或者按ESC键时返回true
	{
		
		double time = GetTime(); 
		// 每次循环更新一帧
		// 摄像机围绕y轴转动
		double cameraTime = time*0.3;
		camera.position.x = (float)cos(cameraTime)*40.0f;
		camera.position.z = (float)sin(cameraTime)*40.0f;
		up = Vector3CrossProduct(Vector3Subtract(camera.target,camera.position),(Vector3){ 0.0f, 1.0f, 0.0f });
		
		camera.up = up; //相机正上方朝向矢量
		
		BeginDrawing();
		
			ClearBackground(WHITE);
			//以摄像机视角绘制3d内容
			BeginMode3D(camera);
				//绘制水平面网格
				DrawGrid(100,5);
				//绘制Y轴
				DrawLine3D(Vector3{0,100,0},Vector3{0,-100,0},BLACK);
				//绘制立方体
				DrawCube(Vector3{0,0,0},10,10,10,VIOLET);
				DrawCubeWires(Vector3{0,0,0},10,10,10,BLACK);
				//绘制球体
				DrawSphere(Vector3{0,-40,0},10,RED);
				DrawSphereWires(Vector3{0,-40,0},10,10,10,BLACK);				
			EndMode3D();
		EndDrawing();
	}
	
	//关闭窗口
	CloseWindow();
	
	return 0;
}
