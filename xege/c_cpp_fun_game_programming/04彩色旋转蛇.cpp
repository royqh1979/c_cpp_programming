/**
彩色旋转蛇

原程序为EasyX，见https://zhuanlan.zhihu.com/p/188259883
*/
#include <graphics.h>

int main() {
    //创建绘图窗口
    initgraph(800, 600);
    
    
    //背景色为灰色
    setbkcolor(EGERGB(128, 128, 128));
    //清除屏幕内容
    cleardevice();
    //初始化随机数发生器
    randomize();

    int center_x, center_y; //圆心坐标
    int radius; //圆半径
    int offset; // 同一半径各组扇形之间的角度偏移量
    int total_offset; // 不同半径之间的角度偏移量

    

    while (is_run()) {
        for (center_x = 100; center_x < 800; center_x += 200) {
            for (center_y = 100; center_y < 600; center_y += 200) {
                total_offset = 0;
                float h = random(180);
                // 用色调1生成的颜色1
                COLORREF color1 = HSVtoRGB(h, 0.9, 0.8);
                // 用色调2生成的颜色2
                COLORREF color2 = HSVtoRGB(h + 180, 0.9, 0.8);
                for (radius = 100; radius > 0; radius = radius - 20) {
                    //半径从大到小绘制
                    for (int i = 0; i < 20; i++) {
                        //绕着旋转一周绘制扇形区域

                        //各组扇形之间偏移的角度
                        offset = i * 180 / 10 + total_offset ;

                        setfillcolor(color1);
                        setcolor(color1);
                        sector(center_x, center_y, offset, (2 * 180 / 60 + offset), radius, radius);

                        setfillcolor(WHITE);
                        setcolor(WHITE);
                        sector(center_x, center_y, (2 * 180 / 60 + offset),
                               (3 * 180 / 60 + offset), radius, radius);

                        setfillcolor(color2);
                        setcolor(color2);
                        sector(center_x, center_y, (3 * 180 / 60 + offset),
                               (5 * 180 / 60 + offset), radius, radius);

                        setfillcolor(BLACK);
                        setcolor(BLACK);
                        sector(center_x, center_y,
                               (5 * 180 / 60 + offset),
                               (6 * 180 / 60 + offset), radius, radius);
                    }
                    total_offset += 180 / 20; //不同半径间角度偏移量为180/20
                }
            }
        }
        getch();
    }
    closegraph();
}
