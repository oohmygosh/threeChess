#include <graphics.h>


#define SCREEN_WIDTH  300
#define SCREEN_HEIGHT 300

// 对行和列以及斜对角进行判断,是否可以连成线
#define ONE_ROW        (map[0][0] != 0 && map[1][0] != 0 && map[2][0] != 0) && \
                    ((map[0][0] == map[1][0]) && (map[1][0] == map[2][0]))        //第一排

#define TWO_ROW        (map[0][1] != 0 && map[1][1] != 0 && map[2][1] != 0) && \
                    ((map[0][1] == map[1][1]) && (map[1][1] == map[2][1]))        //第二排

#define THREE_ROW   (map[0][2] != 0 && map[1][2] != 0 && map[2][2] != 0) && \
                    ((map[0][2] == map[1][2]) && (map[1][2] == map[2][2]))        //第三排

#define ONE_COLUMN        (map[0][0] != 0 && map[0][1] != 0 && map[0][2] != 0) && \
                        ((map[0][0] == map[0][1]) && (map[0][1] == map[0][2]))    //第一列

#define TWO_COLUMN        (map[1][0] != 0 && map[1][1] != 0 && map[1][2] != 0) && \
                        ((map[1][0] == map[1][1]) && (map[1][1] == map[1][2]))    //第二列

#define THREE_COLUMN    (map[2][0] != 0 && map[2][1] != 0 && map[2][2] != 0) && \
                        ((map[2][0] == map[2][1]) && (map[2][1] == map[2][2]))    //第三列

#define LEFT_OBLIQUE        (map[0][0] != 0 && map[1][1] != 0 && map[2][2] != 0) && \
                            ((map[0][0] == map[1][1]) && (map[1][1] == map[2][2]))    //左斜角

#define RIGHT_OBLIQUE        (map[2][0] != 0 && map[1][1] != 0 && map[0][2] != 0) && \
                            ((map[2][0] == map[1][1]) && (map[1][1] == map[0][2]))    //右斜角

// 棋盘地图, 0代表空位置
int map[3][3] =
        {
                0, 0, 0,
                0, 0, 0,
                0, 0, 0
        };

int x = 0, y = 0;    //当前下棋的坐标
int camp = 1;        //阵营(自己玩的图案) -1为O图形, 1为X图形
bool resWin[3];        //判断赢得是哪一条线
int bestPosition[] = {0, 0};
int currentDepth = 0;

// 加载资源以及初始化棋盘
void initChessboard();

// 检查鼠标事件
void detectMouseMsg(MOUSEMSG &mouse, IMAGE *img);

// 检查游戏是否结束
bool checkEnd();

// 重置游戏
void resetGame();

int isEnd();

int evaluate();

int maxMinSearch(int depth);

int main() {
    IMAGE img[2];
    MOUSEMSG mouse{};

    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);            //初始化界面

    loadimage(&img[0], _T("./x.png"), 90, 90, true);    //加载资源图
    loadimage(&img[1], _T("./o.png"), 90, 90, true);

    initChessboard();    //初始化棋盘

    while (true) {
        // 检测鼠标位置, 走棋
        detectMouseMsg(mouse, img);
        if (checkEnd())        //判断是否结束游戏了
        {
            Sleep(500);
            cleardevice();        //清除窗口
            initChessboard();
            resetGame();        //重置游戏的参数
        }
    }
}

void initChessboard() {
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 1);

    // 画棋盘
    for (int i = 1; i < 3; i++) {
        line(0, i * 100, 300, i * 100);
        line(i * 100, 0, i * 100, 300);
    }
    line(300, 0, 300, 300);
}

void computerPlay(IMAGE *img) {
    maxMinSearch(currentDepth);
    map[bestPosition[0]][bestPosition[1]] = -1;
    putimage(bestPosition[0] * 100 + 5, bestPosition[1] * 100 + 5, &img[0]);
    currentDepth++;
    camp = 1;
}

void detectMouseMsg(MOUSEMSG &mouse, IMAGE *img) {
    int front;
    mouse = GetMouseMsg();        // 获取鼠标信息

    //判断鼠标是否左键按下
    if (camp == 1 && mouse.uMsg == WM_LBUTTONDOWN) {
        // 计算坐标
        x = mouse.x / 100;
        y = mouse.y / 100;

        front = map[x][y];        //记录当前位置的值
        if (front == 0) //0代表空位, 即可以在当前位置下棋
        {
            map[x][y] = camp;        //鼠标点击的区域
            putimage(x * 100 + 5, y * 100 + 5, &img[camp == 1 ? camp : 0]);
            (camp == 1) ? camp = -1 : camp = 1;    //交换阵营
            currentDepth++;
        }
    } else if (camp == -1)
        computerPlay(img);
}

bool checkEnd() {
    setlinecolor(YELLOW);        //设置线的颜色
    setlinestyle(PS_SOLID, 5);    //设置线的风格

    if ((resWin[0] = ONE_ROW) || (resWin[1] = TWO_ROW) || (resWin[2] = THREE_ROW)) {
        if (resWin[0])
            line(5, 45, 295, 45);
        else if (resWin[1])
            line(5, 155, 295, 155);
        else if (resWin[2])
            line(5, 245, 295, 245);

        outtextxy(140, 140, "你赢了!");
        Sleep(100);
        return true;
    } else if ((resWin[0] = ONE_COLUMN) || (resWin[1] = TWO_COLUMN) || (resWin[2] = THREE_COLUMN)) {
        if (resWin[0])
            line(45, 5, 45, 295);
        else if (resWin[1])
            line(155, 5, 155, 295);
        else if (resWin[2])
            line(245, 5, 245, 295);

        outtextxy(140, 140, "你赢了!");
        Sleep(100);
        return true;
    } else if ((resWin[0] = LEFT_OBLIQUE) || (resWin[1] = RIGHT_OBLIQUE)) {
        if (resWin[0])
            line(5, 5, 295, 295);
        else if (resWin[1])
            line(295, 5, 5, 295);

        outtextxy(140, 140, "你赢了!");
        Sleep(100);
        return true;
    }

    // 判断是否还有空位
    for (auto &i: map)
        for (int j: i)
            if (j == 0) return false;

    outtextxy(140, 140, "平局!");
    Sleep(70);
    return true;
}

int int_max = 999999;
int int_min = -1000000;

int isEnd() {
    int count;
    // 判断行
    for (auto &k: map) {
        count = 0;
        for (int l: k)
            count += l;
        if (count == 3 || count == -3)
            return count / 3;
    }
    // 判断列
    for (int k = 0; k < 3; ++k) {
        count = 0;
        for (auto &l: map)
            count += l[k];
        if (count == 3 || count == -3)
            return count / 3;
    }

    count = map[0][0] + map[1][1] + map[2][2];
    if (count == 3 || count == -3)
        return count / 3;
    count = map[0][2] + map[1][1] + map[2][0];
    if (count == 3 || count == -3)
        return count / 3;
    return 0;
}

int evaluate() {
    int value = isEnd();
    if (value == 1) return int_max;
    if (value == -1) return int_min;
}

int maxMinSearch(int depth) {
    int nextValue;
    int value = 0;
    if (camp == 1) value = int_min;
    if (camp == -1) value = int_max;
    if (isEnd() != 0) {
        return evaluate();
    }
    if (depth == 9) {
        value = evaluate();
        return value;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == 0) {
                if (camp == 1) {
                    map[i][j] = 1;
                    camp = -1;
                    nextValue = maxMinSearch(depth + 1);
                    camp = 1;
                    if (value < nextValue) {
                        value = nextValue;
                        if (depth == currentDepth) {
                            bestPosition[0] = i;
                            bestPosition[1] = j;
                        }
                    }

                } else if (camp == -1) {
                    map[i][j] = -1;
                    camp = 1;
                    nextValue = maxMinSearch(depth + 1);
                    camp = -1;
                    if (value > nextValue) {
                        value = nextValue;
                        if (depth == currentDepth) {
                            bestPosition[0] = i;
                            bestPosition[1] = j;
                        }

                    }
                }
                map[i][j] = 0;
            }

        }
    }

    return value;
}


void resetGame() {
    // 擦除界面,重置map值
    setfillcolor(BLACK);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            map[i][j] = 0;
            solidrectangle(i * 100 + 5, j * 100 + 5, i * 100 + 95, j * 100 + 95);
        }
    }
    bestPosition[0] = 0;
    bestPosition[1] = 0;
    camp = 1;
    currentDepth = 0;
}
