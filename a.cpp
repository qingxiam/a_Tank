#include <graphics.h>  // EasyX提供兼容接口
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// 设计的参数
#define WIDTH 800
#define HEIGHT 600
#define BLOCK_SIZE 30  
#define MAP_ROW 20
#define MAP_COL 26
#define ENEMY_NUM 10
#define LEVELS 5
#define MAX_LIFE 3
#define MAX_PROPS 2
#define PROP_DURATION 600

// 坦克速度
#define PLAYER_SPEED 3
#define PLAYER_BOOST_SPEED 8   // 调整为2.67倍速度，更合理
#define ENEMY_NORMAL_SPEED 2
#define ENEMY_FAST_SPEED 4

// 子弹速度
#define BULLET_SPEED 10

enum Direction { UP, DOWN, LEFT, RIGHT };

// 设计的关卡地图
const int level_maps[LEVELS][MAP_ROW][MAP_COL] = {
    // 关卡1：简单难度
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,1,2,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0},
        {0,0,2,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,2,0,0,0},
        {0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,2,0,0,1,0,0,0,1,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,0,0,0,1,0,0,0}
    },
    // 关卡2：中等难度
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0},
        {0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,2,0},
        {0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,1,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,2,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
        {0,0,0,1,1,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,0,0,0,1,0,0,0}
    },
    // 关卡3：困难难度
    {
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,0},
        {0,0,0,0,1,1,2,0,0,0,0,0,1,0,0,0,0,2,0,0,0,0,1,0,0,0},
        {0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0},
        {0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,0,1,0,0,0,0,0,0,2,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
        {0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,1,0},
        {0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0},
        {0,0,0,0,2,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,1,0,0},
        {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,5,1,0,0,0,0,0,0,0,0,1,0,0,0}
    },
    // 关卡4：挑战关卡 - 狭窄通道/保护基地/对称障碍物
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0},
        {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0},
        {0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,2,5,1,0,0,0,0,1,5,2,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0}
    },
    // 关卡5：BOSS关卡 - 红色大BOSS母体（随机地图）
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,2,0,0},
        {0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,1,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0},
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,1},
        {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,0,1,2,1,0,1,0,0,1,2,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,5,1,0,0,1,0,1,5,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0}
    }
};

struct Block {
    int x, y, type; // type: 1砖块 2铁块  5基地
    bool alive;
};
struct Tank {
    int x, y;
    Direction dir;
    COLORREF color;
    COLORREF originalColor; // 原始颜色
    bool isPlayer;
    int cooldown;
    int type; // 0普通 1快速 2高血量 3智能
    int hp;
    int speed;
    int prop_double; // 双发道具剩余帧
    int prop_invincible; // 无敌剩余帧
    int prop_speed; // 加速道具剩余帧
    int attackCooldown; // 攻击冷却时间
    int directionChangeCooldown; // 方向改变冷却时间
    int targetX, targetY; // 目标位置（用于智能敌人）
};
struct Bullet {
    int x, y;
    Direction dir;
    bool fromPlayer;
    bool alive;
};
struct Base {
    int x, y;
    bool alive;
};
struct Prop {
    int x, y, type; // 0加血 1无敌 2加速 3双发
    bool alive;
    int duration;
};
struct Explosion {
    int x, y, frame;
    bool alive;
};

struct Boss {
    int x, y;
    Direction dir;
    int hp;
    int maxHp;
    int speed;
    int moveCooldown;
    int spawnCooldown;
    int attackCooldown;
    bool alive;
    int size; // BOSS体积大小
    int immobileTimer; // 不移动计时器
};

std::vector<Block> blocks;
std::vector<Bullet> bullets;
std::vector<Tank> enemies;
std::vector<Prop> props;
std::vector<Explosion> explosions;
Tank player;
Base base;
Boss boss;
int currentLevel = 0;
int playerLife = MAX_LIFE;
int score = 0;

// 挑战关卡相关变量
int playerBullets = 0; // 玩家剩余子弹数
int totalEnemyHP = 0; // 敌人总血量
int bulletRefreshTimer = 0; // 子弹刷新计时器
bool isChallengeMode = false; // 是否为挑战模式
bool isBossLevel = false; // 是否为BOSS关卡

// ===================== 菜单与关卡选择 =====================
void showMenu() {
    setbkcolor(RGB(20, 20, 40)); // 深蓝色背景
    cleardevice();
    
    // 绘制渐变背景效果
    for (int i = 0; i < HEIGHT; i++) {
        int r = 20 + (i * 10) / HEIGHT;
        int g = 20 + (i * 15) / HEIGHT;
        int b = 40 + (i * 20) / HEIGHT;
        setlinecolor(RGB(r, g, b));
        line(0, i, WIDTH, i);
    }
    
    // 绘制装饰性边框
    setlinecolor(RGB(100, 150, 255));
    setlinestyle(PS_SOLID, 3);
    roundrect(50, 50, WIDTH - 50, HEIGHT - 50, 20, 20);
    
    // 主标题 - 带阴影效果
    settextstyle(80, 0, _T("黑体"));
    settextcolor(RGB(50, 50, 50));
    outtextxy(203, 103, _T("FC坦克大战"));
    settextcolor(RGB(100, 150, 255));
    outtextxy(200, 100, _T("FC坦克大战"));
    
    // 副标题
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(200, 200, 200));
    outtextxy(280, 200, _T("经典FC游戏重制版"));
    
    // 关卡选择说明
    settextstyle(30, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(250, 280, _T("选择关卡开始游戏"));
    
    // 关卡列表
    settextstyle(25, 0, _T("黑体"));
    
    // 关卡1-3
    settextcolor(RGB(100, 255, 100));
    outtextxy(200, 330, _T("关卡1: 新手训练 - 简单难度"));
    outtextxy(200, 360, _T("关卡2: 进阶挑战 - 中等难度"));
    outtextxy(200, 390, _T("关卡3: 高手试炼 - 困难难度"));
    
    // 特殊关卡
    settextcolor(RGB(255, 255, 0));
    outtextxy(200, 420, _T("关卡4: 挑战模式 - 子弹有限，需要策略！"));
    settextcolor(RGB(255, 100, 100));
    outtextxy(200, 450, _T("关卡5: BOSS关卡 - 红色大BOSS母体！"));
    
    // 操作说明
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(200, 200, 200));
    outtextxy(200, 500, _T("操作说明: WASD移动，空格射击，ESC退出"));
    outtextxy(200, 525, _T("H键 - 查看游戏说明"));
    
    // 闪烁提示
    static int blinkFrame = 0;
    blinkFrame = (blinkFrame + 1) % 60;
    if (blinkFrame < 30) {
        settextcolor(RGB(255, 255, 0));
        outtextxy(300, 550, _T("按数字键选择关卡"));
    }
    
    EndBatchDraw();
    BeginBatchDraw();
}

void showHelp() {
    // 设置背景
    setbkcolor(RGB(20, 20, 40));
    cleardevice();
    
    // 绘制渐变背景
    for (int i = 0; i < HEIGHT; i++) {
        int r = 20 + (i * 10) / HEIGHT;
        int g = 20 + (i * 15) / HEIGHT;
        int b = 40 + (i * 20) / HEIGHT;
        setlinecolor(RGB(r, g, b));
        line(0, i, WIDTH, i);
    }
    
    // 绘制装饰性边框
    setlinecolor(RGB(100, 150, 255));
    setlinestyle(PS_SOLID, 3);
    roundrect(50, 50, WIDTH - 50, HEIGHT - 50, 20, 20);
    
    setbkmode(TRANSPARENT);
    
    // 标题
    settextstyle(60, 0, _T("黑体"));
    settextcolor(RGB(50, 50, 50));
    outtextxy(203, 73, _T("游戏说明"));
    settextcolor(RGB(100, 150, 255));
    outtextxy(200, 70, _T("游戏说明"));
    
    // 操作说明
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(100, 150, _T("【基本操作】"));
    
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(200, 200, 200));
    outtextxy(120, 180, _T("W/A/S/D - 坦克移动"));
    outtextxy(120, 205, _T("空格键 - 发射子弹"));
    outtextxy(120, 230, _T("ESC - 随时退出游戏"));
    
    // 游戏元素说明
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(100, 270, _T("【游戏元素】"));
    
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(200, 200, 200));
    outtextxy(120, 300, _T("蓝色坦克 - 玩家"));
    outtextxy(120, 325, _T("绿色坦克 - 普通敌人"));
    outtextxy(120, 350, _T("橙色坦克 - 高血量敌人"));
    outtextxy(120, 375, _T("黄色坦克 - 快速敌人"));
    outtextxy(120, 400, _T("紫色坦克 - 高攻击敌人"));
    outtextxy(120, 425, _T("红色方块 - 砖块（可破坏）"));
    outtextxy(120, 450, _T("灰色方块 - 铁块（不可破坏）"));
    outtextxy(120, 475, _T("黄色方块 - 基地（需要保护）"));
    
    // 道具说明
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(450, 150, _T("【道具系统】"));
    
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(255, 100, 100));
    outtextxy(470, 180, _T("红色道具 - 加血"));
    settextcolor(RGB(255, 255, 0));
    outtextxy(470, 205, _T("黄色道具 - 无敌"));
    settextcolor(RGB(0, 255, 255));
    outtextxy(470, 230, _T("青色道具 - 加速"));
    settextcolor(RGB(0, 128, 255));
    outtextxy(470, 255, _T("蓝色道具 - 双发"));
    
    // 关卡说明
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(450, 300, _T("【关卡特色】"));
    
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(100, 255, 100));
    outtextxy(470, 330, _T("关卡1-3: 经典模式"));
    settextcolor(RGB(255, 255, 0));
    outtextxy(470, 355, _T("关卡4: 挑战模式"));
    outtextxy(470, 380, _T("  子弹有限，需要策略"));
    settextcolor(RGB(255, 100, 100));
    outtextxy(470, 405, _T("关卡5: BOSS关卡"));
    outtextxy(470, 430, _T("  红色大BOSS，前40秒静止"));
    
    // 返回提示
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(WIDTH / 2 - 150, HEIGHT - 80, _T("按任意键返回主菜单"));
    
    // 闪烁提示
    static int blinkFrame = 0;
    blinkFrame = (blinkFrame + 1) % 60;
    if (blinkFrame < 30) {
        settextcolor(RGB(255, 255, 0));
        outtextxy(WIDTH / 2 - 100, HEIGHT - 50, _T("按任意键继续"));
    }
    
    FlushBatchDraw();
    
    // 等待任意按键
    while (1) {
        if (_kbhit()) {
            _getch();
            break;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000 ||
            GetAsyncKeyState(VK_RETURN) & 0x8000 ||
            GetAsyncKeyState('A') & 0x8000 ||
            GetAsyncKeyState('S') & 0x8000 ||
            GetAsyncKeyState('D') & 0x8000 ||
            GetAsyncKeyState('W') & 0x8000) {
            Sleep(100);
            break;
        }
        Sleep(50);
    }
}

int selectLevel() {
    while (1) {
        showMenu();
        
        // 检查按键
        if (GetAsyncKeyState('1') & 0x8000) return 0;
        if (GetAsyncKeyState('2') & 0x8000) return 1;
        if (GetAsyncKeyState('3') & 0x8000) return 2;
        if (GetAsyncKeyState('4') & 0x8000) return 3; // 挑战关卡
        if (GetAsyncKeyState('5') & 0x8000) return 4; // BOSS关卡
        if (GetAsyncKeyState('H') & 0x8000) { // 帮助键
            showHelp();
            continue;
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) exit(0);
        
        Sleep(50);
    }
}

// ===================== 地图与关卡初始化 =====================
void initMap(int level) {
    blocks.clear();
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            int t = level_maps[level][i][j];
            if (t == 0) continue;
            Block b = { j * BLOCK_SIZE, i * BLOCK_SIZE, t, true };
            blocks.push_back(b);
            if (t == 5) {
                base.x = j * BLOCK_SIZE;
                base.y = i * BLOCK_SIZE;
                base.alive = true;
            }
        }
    }
}

void drawBlocks() {
    for (auto& b : blocks) {
        if (!b.alive) continue;
        switch (b.type) {
        case 1: setfillcolor(RGB(200, 100, 50)); break; // 砖块
        case 2: setfillcolor(RGB(120, 120, 120)); break; // 铁块
        case 5: setfillcolor(RGB(255, 255, 0)); break; // 基地
        }
        fillroundrect(b.x, b.y, b.x + BLOCK_SIZE, b.y + BLOCK_SIZE, 10, 10);
        if (b.type == 5) {
            settextstyle(20, 0, _T("黑体"));
            settextcolor(BLACK);
            outtextxy(b.x + 5, b.y + 10, _T("基地"));
        }
    }
}

void drawBase() {
    if (base.alive) {
        setfillcolor(RGB(255, 255, 0));
        fillroundrect(base.x, base.y, base.x + BLOCK_SIZE, base.y + BLOCK_SIZE, 10, 10);
        settextstyle(20, 0, _T("黑体"));
        settextcolor(BLACK);
        outtextxy(base.x + 5, base.y + 10, _T("基地"));
    }
}

// ===================== 玩家与敌人初始化 =====================
void initPlayer() {
    player.x = (MAP_COL / 2 - 1) * BLOCK_SIZE;
    player.y = (MAP_ROW - 2) * BLOCK_SIZE;
    player.dir = UP;
    player.color = RGB(0, 128, 255); // 彩色玩家坦克 - 蓝色
    player.originalColor = RGB(0, 128, 255);
    player.isPlayer = true;
    player.cooldown = 0;
    player.type = 0;
    player.hp = 1;
    player.speed = PLAYER_SPEED;
    player.prop_double = 0;
    player.prop_invincible = 0;
    player.prop_speed = 0;
}
void initEnemies() {
    enemies.clear();
    totalEnemyHP = 0; // 重置敌人总血量
    
    for (int i = 0; i < ENEMY_NUM; i++) {
        Tank e;
        e.x = (i % 3) * 8 * BLOCK_SIZE + BLOCK_SIZE;
        e.y = BLOCK_SIZE;
        e.dir = DOWN;
        e.isPlayer = false;
        e.cooldown = rand() % 30;
        
        // 根据概率生成不同类型的敌人，越强大的敌人概率越低
        int randNum = rand() % 100;
        if (randNum < 60) {
            e.type = 0; // 60%概率生成绿色弱敌人（数量最多）
        } else if (randNum < 80) {
            e.type = 1; // 20%概率生成橙色高血量敌人
        } else if (randNum < 90) {
            e.type = 2; // 10%概率生成黄色灵活敌人
        } else {
            e.type = 3; // 10%概率生成紫色高攻击敌人
        }
        
        // 根据类型设置不同的属性
        switch (e.type) {
        case 0: // 绿色弱敌人（数量最多）
            e.color = RGB(0, 255, 0); 
            e.hp = 1; 
            e.speed = ENEMY_NORMAL_SPEED;
            e.attackCooldown = 150 + rand() % 50; // 攻击频率最低
            e.directionChangeCooldown = 60 + rand() % 40; // 方向改变频率较低
            break;
        case 1: // 橙色高血量敌人
            e.color = RGB(255, 128, 0); 
            e.hp = 4; // 血量最厚
            e.speed = ENEMY_NORMAL_SPEED;
            e.attackCooldown = 120 + rand() % 60; // 攻击频率较低
            e.directionChangeCooldown = 50 + rand() % 30; // 方向改变频率较低
            break;
        case 2: // 黄色灵活敌人
            e.color = RGB(255, 255, 0); 
            e.hp = 1; 
            e.speed = ENEMY_FAST_SPEED; // 速度最快
            e.attackCooldown = 100 + rand() % 40; // 攻击频率中等
            e.directionChangeCooldown = 15 + rand() % 15; // 方向改变最频繁（最灵活）
            break;
        case 3: // 紫色高攻击敌人
            e.color = RGB(128, 0, 255); 
            e.hp = 2; 
            e.speed = ENEMY_NORMAL_SPEED;
            e.attackCooldown = 50 + rand() % 30; // 攻击频率最高
            e.directionChangeCooldown = 40 + rand() % 25; // 方向改变频率中等
            break;
        }
        
        e.prop_double = 0;
        e.prop_invincible = 0;
        e.prop_speed = 0;
        e.originalColor = e.color;
        enemies.push_back(e);
        totalEnemyHP += e.hp; // 累加敌人血量
    }
    
    // 如果是挑战模式，设置玩家子弹数量等于敌人总血量
    if (isChallengeMode) {
        playerBullets = totalEnemyHP;
        bulletRefreshTimer = 0;
    }
}

// ===================== BOSS初始化 =====================
void initBoss() {
    boss.x = (MAP_COL / 2 - 2) * BLOCK_SIZE; // 居中位置
    boss.y = 2 * BLOCK_SIZE; // 上方位置
    boss.dir = DOWN;
    boss.hp = 20; // 血量很厚
    boss.maxHp = 20;
    boss.speed = 1; // 移动很慢
    boss.moveCooldown = 0;
    boss.spawnCooldown = 0;
    boss.attackCooldown = 0;
    boss.alive = true;
    boss.size = 2; // 2x2大小
    boss.immobileTimer = 800; // 前40秒不移动 (800帧 = 40秒 * 20fps)
}

// ===================== 精美化坦克与子弹 =====================
void drawTank(const Tank& t) {
    // 绘制坦克主体
    if (t.isPlayer) {
        // 玩家坦克使用彩色渐变效果
        static int colorFrame = 0;
        colorFrame = (colorFrame + 1) % 360;
        
        // 创建彩虹色渐变效果
        int r = (int)(128 + 127 * sin(colorFrame * 3.14159 / 180));
        int g = (int)(128 + 127 * sin((colorFrame + 120) * 3.14159 / 180));
        int b = (int)(128 + 127 * sin((colorFrame + 240) * 3.14159 / 180));
        
        // 确保颜色值在有效范围内
        r = max(0, min(255, r));
        g = max(0, min(255, g));
        b = max(0, min(255, b));
        
        setfillcolor(RGB(r, g, b));
    } else {
        setfillcolor(t.color);
    }
    fillroundrect(t.x, t.y, t.x + BLOCK_SIZE, t.y + BLOCK_SIZE, 10, 10);
    
    // 绘制坦克炮管
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3);
    int cx = t.x + BLOCK_SIZE / 2;
    int cy = t.y + BLOCK_SIZE / 2;
    switch (t.dir) {
    case UP: line(cx, cy, cx, t.y); break;
    case DOWN: line(cx, cy, cx, t.y + BLOCK_SIZE); break;
    case LEFT: line(cx, cy, t.x, cy); break;
    case RIGHT: line(cx, cy, t.x + BLOCK_SIZE, cy); break;
    }
    setlinestyle(PS_SOLID, 1);
    
    // 如果是玩家坦克且有buff，绘制buff光环效果
    if (t.isPlayer && (t.prop_invincible > 0 || t.prop_speed > 0 || t.prop_double > 0)) {
        // 绘制外圈光环 - 使用彩色效果
        static int haloFrame = 0;
        haloFrame = (haloFrame + 2) % 360;
        
        int r = (int)(128 + 127 * sin(haloFrame * 3.14159 / 180));
        int g = (int)(128 + 127 * sin((haloFrame + 120) * 3.14159 / 180));
        int b = (int)(128 + 127 * sin((haloFrame + 240) * 3.14159 / 180));
        
        r = max(0, min(255, r));
        g = max(0, min(255, g));
        b = max(0, min(255, b));
        
        setlinecolor(RGB(r, g, b));
        setlinestyle(PS_SOLID, 2);
        int radius = BLOCK_SIZE / 2 + 8;
        circle(cx, cy, radius);
        
        // 如果有多个buff，绘制多层光环
        if (t.prop_invincible > 0 && t.prop_speed > 0) {
            setlinecolor(RGB(255, 255, 0)); // 黄色
            circle(cx, cy, radius + 4);
        }
        if (t.prop_double > 0) {
            setlinecolor(RGB(0, 128, 255)); // 蓝色
            circle(cx, cy, radius + 8);
        }
    }
    
    // 如果是敌人且血量大于1，显示血量条
    if (!t.isPlayer && t.hp > 1) {
        int barWidth = BLOCK_SIZE - 4;
        int barHeight = 4;
        int barX = t.x + 2;
        int barY = t.y - 8;
        
        // 血量条背景
        setfillcolor(RGB(100, 100, 100));
        fillroundrect(barX, barY, barX + barWidth, barY + barHeight, 2, 2);
        
        // 血量条
        int maxHp;
        if (t.type == 1) maxHp = 4; // 橙色敌人最大血量4
        else if (t.type == 3) maxHp = 2; // 紫色敌人最大血量2
        else maxHp = 1; // 其他敌人最大血量1
        int currentHpWidth = (barWidth * t.hp) / maxHp;
        if (currentHpWidth > 0) {
            setfillcolor(RGB(255, 0, 0));
            fillroundrect(barX, barY, barX + currentHpWidth, barY + barHeight, 2, 2);
        }
    }
}

// ===================== BOSS绘制 =====================
void drawBoss() {
    if (!boss.alive) return;
    
    // 绘制BOSS主体（2x2大小）
    setfillcolor(RGB(255, 0, 0)); // 红色
    fillroundrect(boss.x, boss.y, boss.x + BLOCK_SIZE * boss.size, boss.y + BLOCK_SIZE * boss.size, 15, 15);
    
    // 绘制BOSS边框
    setlinecolor(RGB(200, 0, 0));
    setlinestyle(PS_SOLID, 3);
    roundrect(boss.x, boss.y, boss.x + BLOCK_SIZE * boss.size, boss.y + BLOCK_SIZE * boss.size, 15, 15);
    
    // 绘制BOSS炮管
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 4);
    int cx = boss.x + BLOCK_SIZE * boss.size / 2;
    int cy = boss.y + BLOCK_SIZE * boss.size / 2;
    switch (boss.dir) {
    case UP: line(cx, cy, cx, boss.y); break;
    case DOWN: line(cx, cy, cx, boss.y + BLOCK_SIZE * boss.size); break;
    case LEFT: line(cx, cy, boss.x, cy); break;
    case RIGHT: line(cx, cy, boss.x + BLOCK_SIZE * boss.size, cy); break;
    }
    setlinestyle(PS_SOLID, 1);
    
    // 绘制BOSS血量条
    int barWidth = BLOCK_SIZE * boss.size - 4;
    int barHeight = 6;
    int barX = boss.x + 2;
    int barY = boss.y - 12;
    
    // 血量条背景
    setfillcolor(RGB(100, 100, 100));
    fillroundrect(barX, barY, barX + barWidth, barY + barHeight, 3, 3);
    
    // 血量条
    int currentHpWidth = (barWidth * boss.hp) / boss.maxHp;
    if (currentHpWidth > 0) {
        setfillcolor(RGB(255, 0, 0));
        fillroundrect(barX, barY, barX + currentHpWidth, barY + barHeight, 3, 3);
    }
    
    // 绘制BOSS标识
    settextstyle(16, 0, _T("黑体"));
    settextcolor(WHITE);
    outtextxy(boss.x + 5, boss.y + 5, _T("BOSS"));
}

void drawBullets() {
    setfillcolor(YELLOW);
    for (auto& b : bullets) {
        if (b.alive)
            solidcircle(b.x, b.y, 6);
    }
}
void drawExplosion(int x, int y, int frame) {
    setfillcolor(RGB(255, 128, 0));
    solidcircle(x, y, frame * 8);
}

// ===================== BOSS AI =====================
void bossAI() {
    if (!boss.alive) return;
    
    // 更新不移动计时器
    if (boss.immobileTimer > 0) {
        boss.immobileTimer--;
    }
    
    // BOSS移动逻辑（频率很低）
    if (boss.moveCooldown <= 0) {
        // 随机改变方向
        boss.dir = (Direction)(rand() % 4);
        boss.moveCooldown = 200 + rand() % 300; // 移动频率很低
    } else {
        boss.moveCooldown--;
    }
    
    // BOSS移动（前40秒不移动）
    if (boss.immobileTimer <= 0) {
        int nx = boss.x, ny = boss.y;
        switch (boss.dir) {
        case UP: ny -= boss.speed; break;
        case DOWN: ny += boss.speed; break;
        case LEFT: nx -= boss.speed; break;
        case RIGHT: nx += boss.speed; break;
        }
        
        // 检查移动是否有效（考虑BOSS大小）
        bool canMove = true;
        for (int i = 0; i < boss.size; i++) {
            for (int j = 0; j < boss.size; j++) {
                int checkX = nx + i * BLOCK_SIZE;
                int checkY = ny + j * BLOCK_SIZE;
                if (checkX < 0 || checkX >= WIDTH - BLOCK_SIZE || 
                    checkY < 0 || checkY >= HEIGHT - BLOCK_SIZE) {
                    canMove = false;
                    break;
                }
            }
        }
        
        if (canMove) {
            boss.x = nx;
            boss.y = ny;
        }
    }
    
    // BOSS攻击逻辑
    if (boss.attackCooldown <= 0) {
        int cx = boss.x + BLOCK_SIZE * boss.size / 2;
        int cy = boss.y + BLOCK_SIZE * boss.size / 2;
        bullets.push_back({ cx, cy, boss.dir, false, true });
        boss.attackCooldown = 150 + rand() % 100; // 攻击频率较低
    } else {
        boss.attackCooldown--;
    }
    
    // BOSS生成小坦克逻辑
    if (boss.spawnCooldown <= 0) {
        // 在BOSS周围生成小坦克
        for (int i = 0; i < 2; i++) { // 一次生成2个小坦克
            Tank e;
            e.x = boss.x + (rand() % 3 - 1) * BLOCK_SIZE * 2;
            e.y = boss.y + (rand() % 3 - 1) * BLOCK_SIZE * 2;
            e.dir = DOWN;
            e.isPlayer = false;
            e.cooldown = rand() % 30;
            
            // 随机生成不同类型的敌人
            int randNum = rand() % 100;
            if (randNum < 60) {
                e.type = 0; // 绿色弱敌人
            } else if (randNum < 80) {
                e.type = 1; // 橙色高血量敌人
            } else if (randNum < 90) {
                e.type = 2; // 黄色灵活敌人
            } else {
                e.type = 3; // 紫色高攻击敌人
            }
            
            // 设置敌人属性
            switch (e.type) {
            case 0: // 绿色弱敌人
                e.color = RGB(0, 255, 0); 
                e.hp = 1; 
                e.speed = ENEMY_NORMAL_SPEED;
                e.attackCooldown = 150 + rand() % 50;
                e.directionChangeCooldown = 60 + rand() % 40;
                break;
            case 1: // 橙色高血量敌人
                e.color = RGB(255, 128, 0); 
                e.hp = 4; 
                e.speed = ENEMY_NORMAL_SPEED;
                e.attackCooldown = 120 + rand() % 60;
                e.directionChangeCooldown = 50 + rand() % 30;
                break;
            case 2: // 黄色灵活敌人
                e.color = RGB(255, 255, 0); 
                e.hp = 1; 
                e.speed = ENEMY_FAST_SPEED;
                e.attackCooldown = 100 + rand() % 40;
                e.directionChangeCooldown = 15 + rand() % 15;
                break;
            case 3: // 紫色高攻击敌人
                e.color = RGB(128, 0, 255); 
                e.hp = 2; 
                e.speed = ENEMY_NORMAL_SPEED;
                e.attackCooldown = 50 + rand() % 30;
                e.directionChangeCooldown = 40 + rand() % 25;
                break;
            }
            
            e.prop_double = 0;
            e.prop_invincible = 0;
            e.prop_speed = 0;
            e.originalColor = e.color;
            enemies.push_back(e);
        }
        
        boss.spawnCooldown = 800 + rand() % 400; // 生成频率较低
    } else {
        boss.spawnCooldown--;
    }
}

// ===================== 子弹、碰撞、AI、操作 =====================
void moveBullets() {
    for (auto& b : bullets) {
        if (!b.alive) continue;
        switch (b.dir) {
        case UP: b.y -= BULLET_SPEED; break;
        case DOWN: b.y += BULLET_SPEED; break;
        case LEFT: b.x -= BULLET_SPEED; break;
        case RIGHT: b.x += BULLET_SPEED; break;
        }
        if (b.x < 0 || b.x > WIDTH || b.y < 0 || b.y > HEIGHT)
            b.alive = false;
    }
}
void bulletCollision() {
    // 首先处理子弹与地图块、坦克的碰撞
    for (auto& b : bullets) {
        if (!b.alive) continue;
        // 碰砖块/铁块/基地 - 改进的圆形子弹碰撞检测
        for (auto& bl : blocks) {
            if (!bl.alive) continue;
            
            // 计算子弹中心到方块最近点的距离
            int bulletRadius = 6; // 子弹半径
            int closestX = max(bl.x, min(b.x, bl.x + BLOCK_SIZE));
            int closestY = max(bl.y, min(b.y, bl.y + BLOCK_SIZE));
            
            // 计算距离的平方
            int distanceX = b.x - closestX;
            int distanceY = b.y - closestY;
            int distanceSquared = distanceX * distanceX + distanceY * distanceY;
            
            // 如果距离小于子弹半径，则发生碰撞
            if (distanceSquared <= bulletRadius * bulletRadius) {
                if (bl.type == 1) bl.alive = false; // 砖块可破坏
                if (bl.type == 5) base.alive = false; // 基地被打
                b.alive = false;
                break; // 子弹已经碰撞，不需要检查其他方块
            }
        }
        // 敌人被打 - 改进的圆形子弹碰撞检测
        if (b.fromPlayer) {
            for (auto& e : enemies) {
                if (e.x < 0) continue; // 跳过已死亡的敌人
                
                // 计算子弹中心到坦克最近点的距离
                int bulletRadius = 6; // 子弹半径
                int closestX = max(e.x, min(b.x, e.x + BLOCK_SIZE));
                int closestY = max(e.y, min(b.y, e.y + BLOCK_SIZE));
                
                // 计算距离的平方
                int distanceX = b.x - closestX;
                int distanceY = b.y - closestY;
                int distanceSquared = distanceX * distanceX + distanceY * distanceY;
                
                // 如果距离小于子弹半径，则发生碰撞
                if (distanceSquared <= bulletRadius * bulletRadius) {
                    if (e.prop_invincible <= 0) {
                        e.hp--;
                        if (e.hp <= 0) {
                            e.x = -1000;
                            score += 100; // 击败敌人加100分
                            mciSendString(L"play resource/enemy-bomb.wav", 0, 0, 0); // 播放敌人爆炸音效
                            explosions.push_back({ e.x + BLOCK_SIZE / 2, e.y + BLOCK_SIZE / 2, 0, true });
                        }
                    }
                    b.alive = false;
                    break; // 子弹已经碰撞，不需要检查其他敌人
                }
            }
            
            // BOSS被打 - 改进的圆形子弹碰撞检测
            if (boss.alive) {
                // 计算子弹中心到BOSS最近点的距离
                int bulletRadius = 6; // 子弹半径
                int closestX = max(boss.x, min(b.x, boss.x + BLOCK_SIZE * boss.size));
                int closestY = max(boss.y, min(b.y, boss.y + BLOCK_SIZE * boss.size));
                
                // 计算距离的平方
                int distanceX = b.x - closestX;
                int distanceY = b.y - closestY;
                int distanceSquared = distanceX * distanceX + distanceY * distanceY;
                
                // 如果距离小于子弹半径，则发生碰撞
                if (distanceSquared <= bulletRadius * bulletRadius) {
                    boss.hp--;
                    if (boss.hp <= 0) {
                        boss.alive = false;
                        score += 1000; // 击败BOSS加1000分
                        mciSendString(L"play resource/enemy-bomb.wav", 0, 0, 0); // 播放BOSS爆炸音效
                        explosions.push_back({ boss.x + BLOCK_SIZE * boss.size / 2, boss.y + BLOCK_SIZE * boss.size / 2, 0, true });
                    }
                    b.alive = false;
                }
            }
        }
        // 玩家被打 - 改进的圆形子弹碰撞检测
        if (!b.fromPlayer && player.prop_invincible <= 0) {
            // 计算子弹中心到玩家坦克最近点的距离
            int bulletRadius = 6; // 子弹半径
            int closestX = max(player.x, min(b.x, player.x + BLOCK_SIZE));
            int closestY = max(player.y, min(b.y, player.y + BLOCK_SIZE));
            
            // 计算距离的平方
            int distanceX = b.x - closestX;
            int distanceY = b.y - closestY;
            int distanceSquared = distanceX * distanceX + distanceY * distanceY;
            
            // 如果距离小于子弹半径，则发生碰撞
            if (distanceSquared <= bulletRadius * bulletRadius) {
                player.x = -1000;
                b.alive = false;
                mciSendString(L"play resource/player_bomb.wav", 0, 0, 0); // 播放玩家爆炸音效
                explosions.push_back({ player.x + BLOCK_SIZE / 2, player.y + BLOCK_SIZE / 2, 0, true });
            }
        }
    }

    // 处理子弹之间的碰撞
    for (size_t i = 0; i < bullets.size(); ++i) {
        if (!bullets[i].alive) continue;
        for (size_t j = i + 1; j < bullets.size(); ++j) {
            if (!bullets[j].alive) continue;
            if (bullets[i].fromPlayer != bullets[j].fromPlayer) {
                int dx = bullets[i].x - bullets[j].x;
                int dy = bullets[i].y - bullets[j].y;
                int distanceSquared = dx * dx + dy * dy;
                int collisionRadius = 12; // 假设子弹半径为 6，两倍半径作为碰撞检测距离
                if (distanceSquared < collisionRadius * collisionRadius) {
                    bullets[i].alive = false;
                    bullets[j].alive = false;
                }
            }
        }
    }
}

bool canMove(int x, int y, bool isPlayer = false) {
    if (x < 0 || x + BLOCK_SIZE > WIDTH || y < 0 || y + BLOCK_SIZE > HEIGHT)
        return false;
    
    // 检查与方块的碰撞
    for (auto& bl : blocks) {
        if (bl.alive && bl.type != 4 && bl.type != 3 &&
            x + BLOCK_SIZE > bl.x && x < bl.x + BLOCK_SIZE &&
            y + BLOCK_SIZE > bl.y && y < bl.y + BLOCK_SIZE)
            return false;
    }
    
    // 检查与坦克的碰撞
    if (isPlayer) {
        // 玩家坦克检查与敌人的碰撞
        for (auto& e : enemies) {
            if (e.x < 0) continue; // 跳过已死亡的敌人
            if (x + BLOCK_SIZE > e.x && x < e.x + BLOCK_SIZE &&
                y + BLOCK_SIZE > e.y && y < e.y + BLOCK_SIZE)
                return false;
        }
    } else {
        // 敌人坦克只检查与玩家的碰撞，允许敌人之间重叠
        if (player.x >= 0 && // 玩家还活着
            x + BLOCK_SIZE > player.x && x < player.x + BLOCK_SIZE &&
            y + BLOCK_SIZE > player.y && y < player.y + BLOCK_SIZE)
            return false;
    }
    
    return true;
}
void enemyAI() {
    for (auto& e : enemies) {
        if (e.x < 0) continue;
        
        // 更新冷却时间
        if (e.directionChangeCooldown > 0) e.directionChangeCooldown--;
        if (e.attackCooldown > 0) e.attackCooldown--;
        
        // 根据敌人类型执行不同的AI行为
        switch (e.type) {
        case 0: // 红色普通敌人 - 基础行为
            if (e.directionChangeCooldown <= 0) {
                e.dir = (Direction)(rand() % 4);
                e.directionChangeCooldown = 30 + rand() % 20;
            }
            break;
            
        case 1: // 橙色快速敌人 - 攻击性强
            if (e.directionChangeCooldown <= 0) {
                // 有30%概率朝向玩家方向
                if (rand() % 100 < 30) {
                    if (player.x < e.x) e.dir = LEFT;
                    else if (player.x > e.x) e.dir = RIGHT;
                    else if (player.y < e.y) e.dir = UP;
                    else e.dir = DOWN;
                } else {
                    e.dir = (Direction)(rand() % 4);
                }
                e.directionChangeCooldown = 15 + rand() % 15;
            }
            break;
            
        case 2: // 紫色高血量敌人 - 缓慢但坚韧
            if (e.directionChangeCooldown <= 0) {
                e.dir = (Direction)(rand() % 4);
                e.directionChangeCooldown = 50 + rand() % 30;
            }
            break;
            
        case 3: // 青色智能敌人 - 追踪玩家
            if (e.directionChangeCooldown <= 0) {
                // 计算到玩家的方向
                int dx = player.x - e.x;
                int dy = player.y - e.y;
                
                // 优先选择距离玩家更近的方向
                if (abs(dx) > abs(dy)) {
                    e.dir = (dx > 0) ? RIGHT : LEFT;
                } else {
                    e.dir = (dy > 0) ? DOWN : UP;
                }
                e.directionChangeCooldown = 20 + rand() % 20;
            }
            break;
        }
        
        // 移动逻辑
        int nx = e.x, ny = e.y;
        switch (e.dir) {
        case UP: ny -= e.speed; break;
        case DOWN: ny += e.speed; break;
        case LEFT: nx -= e.speed; break;
        case RIGHT: nx += e.speed; break;
        }
        
        // 检查移动是否有效
        if (canMove(nx, ny, false)) {
            e.x = nx; e.y = ny;
        } else {
            // 如果移动失败，随机改变方向
            e.dir = (Direction)(rand() % 4);
        }
        
        // 攻击逻辑
        if (e.attackCooldown <= 0) {
            bullets.push_back({ e.x + BLOCK_SIZE / 2, e.y + BLOCK_SIZE / 2, e.dir, false, true });
            // 重置攻击冷却，增加随机性
            switch (e.type) {
            case 0: e.attackCooldown = 100 + rand() % 50; break; // 红色普通敌人
            case 1: e.attackCooldown = 80 + rand() % 40; break;  // 橙色快速敌人
            case 2: e.attackCooldown = 120 + rand() % 60; break; // 紫色高血量敌人
            case 3: e.attackCooldown = 90 + rand() % 40; break;  // 青色智能敌人
            }
        }
    }
}
void playerControl() {
    int nx = player.x, ny = player.y;
    if (GetAsyncKeyState('W') & 0x8000) { player.dir = UP; ny -= player.speed; }
    else if (GetAsyncKeyState('S') & 0x8000) { player.dir = DOWN; ny += player.speed; }
    else if (GetAsyncKeyState('A') & 0x8000) { player.dir = LEFT; nx -= player.speed; }
    else if (GetAsyncKeyState('D') & 0x8000) { player.dir = RIGHT; nx += player.speed; }
    if (canMove(nx, ny, true)) {
        player.x = nx; player.y = ny;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (player.cooldown == 0) {
            // 挑战模式下检查子弹数量
            if (isChallengeMode && playerBullets <= 0) {
                // 子弹不足，不发射
            } else {
                bullets.push_back({ player.x + BLOCK_SIZE / 2, player.y + BLOCK_SIZE / 2, player.dir, true, true });
                if (player.prop_double > 0) {
                    switch (player.dir) {
                    case UP:
                        bullets.push_back({ player.x + BLOCK_SIZE / 4, player.y, player.dir, true, true });
                        bullets.push_back({ player.x + BLOCK_SIZE * 3 / 4, player.y, player.dir, true, true });
                        break;
                    case DOWN:
                        bullets.push_back({ player.x + BLOCK_SIZE / 4, player.y + BLOCK_SIZE, player.dir, true, true });
                        bullets.push_back({ player.x + BLOCK_SIZE * 3 / 4, player.y + BLOCK_SIZE, player.dir, true, true });
                        break;
                    case LEFT:
                        bullets.push_back({ player.x, player.y + BLOCK_SIZE / 4, player.dir, true, true });
                        bullets.push_back({ player.x, player.y + BLOCK_SIZE * 3 / 4, player.dir, true, true });
                        break;
                    case RIGHT:
                        bullets.push_back({ player.x + BLOCK_SIZE, player.y + BLOCK_SIZE / 4, player.dir, true, true });
                        bullets.push_back({ player.x + BLOCK_SIZE, player.y + BLOCK_SIZE * 3 / 4, player.dir, true, true });
                        break;
                    }
                }
                player.cooldown = 20;
                
                // 挑战模式下减少子弹数量
                if (isChallengeMode) {
                    playerBullets--;
                }
                
                // 音效播放机制：立即停止当前音效并播放新音效
                mciSendString(L"stop resource/shot.wav", 0, 0, 0); // 停止当前播放的音效
                mciSendString(L"play resource/shot.wav", 0, 0, 0); // 立即播放新的射击音效
            }
        }
    }
    if (player.cooldown > 0) player.cooldown--;
}

// ===================== 游戏判定与主循环 =====================
bool checkBaseHit() {
    for (auto& e : enemies) {
        if (e.x >= 0 && e.x < base.x + BLOCK_SIZE && e.x + BLOCK_SIZE > base.x &&
            e.y >= 0 && e.y < base.y + BLOCK_SIZE && e.y + BLOCK_SIZE > base.y)
            return true;
    }
    return false;
}
int checkGameOver() {
    bool enemyAlive = false;
    for (auto& e : enemies) if (e.x >= 0) enemyAlive = true;
    
    // 挑战模式下检查子弹耗尽
    if (isChallengeMode && playerBullets <= 0 && enemyAlive) {
        return -3; // 子弹耗尽
    }
    
    if (player.x < 0) {
        playerLife--;
        if (playerLife <= 0) {
            mciSendString(L"play resource/fail.wav", 0, 0, 0); // 播放失败音效
            return -1; // 玩家死
        }
        else {
            initPlayer();
        }
    }
    if (!base.alive || checkBaseHit()) return -2; // 基地被毁
    
    // BOSS关卡特殊逻辑
    if (isBossLevel) {
        if (!boss.alive && !enemyAlive) return 1; // BOSS和敌人都死了，胜利
        if (!boss.alive) return 1; // 只有BOSS死了也算胜利
    } else {
        if (!enemyAlive) return 1;   // 敌人全死
    }
    
    return 0; // 游戏继续
}


void showResult(int result) {
    // 设置背景
    setbkcolor(RGB(20, 20, 40));
    cleardevice();
    
    // 绘制渐变背景
    for (int i = 0; i < HEIGHT; i++) {
        int r = 20 + (i * 10) / HEIGHT;
        int g = 20 + (i * 15) / HEIGHT;
        int b = 40 + (i * 20) / HEIGHT;
        setlinecolor(RGB(r, g, b));
        line(0, i, WIDTH, i);
    }
    
    // 绘制装饰性边框
    setlinecolor(RGB(100, 150, 255));
    setlinestyle(PS_SOLID, 3);
    roundrect(50, 50, WIDTH - 50, HEIGHT - 50, 20, 20);
    
    setbkmode(TRANSPARENT);
    
    // 根据结果显示不同的标题和颜色
    if (result == 1) {
        // 胜利界面
        settextstyle(80, 0, _T("黑体"));
        settextcolor(RGB(50, 50, 50));
        outtextxy(WIDTH / 2 - 123, HEIGHT / 2 - 43, _T("胜利！"));
        settextcolor(RGB(100, 255, 100));
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 40, _T("胜利！"));
        
        // 胜利动画效果
        for (int i = 0; i < 10; i++) {
            setfillcolor(RGB(100, 255, 100));
            solidcircle(100 + i * 70, HEIGHT / 2 + 80, 15);
            solidcircle(100 + i * 70, HEIGHT / 2 + 100, 15);
        }
        
        settextstyle(25, 0, _T("黑体"));
        settextcolor(RGB(255, 255, 255));
        outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 50, _T("恭喜通关！即将进入下一关..."));
        
        Sleep(3000);
    }
    else {
        // 失败界面
        settextstyle(80, 0, _T("黑体"));
        settextcolor(RGB(50, 50, 50));
        outtextxy(WIDTH / 2 - 123, HEIGHT / 2 - 43, _T("失败"));
        settextcolor(RGB(255, 100, 100));
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 40, _T("失败"));
        
        // 显示失败原因
        settextstyle(30, 0, _T("黑体"));
        settextcolor(RGB(255, 200, 200));
        if (result == -1)
            outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("生命值耗尽！"));
        else if (result == -2)
            outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("基地被摧毁！"));
        else if (result == -3)
            outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("子弹耗尽！"));
        
        settextstyle(25, 0, _T("黑体"));
        settextcolor(RGB(255, 255, 255));
        outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 80, _T("按任意键返回主菜单"));
        
        // 闪烁提示
        static int blinkFrame = 0;
        blinkFrame = (blinkFrame + 1) % 60;
        if (blinkFrame < 30) {
            settextcolor(RGB(255, 255, 0));
            outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 120, _T("按任意键继续"));
        }
        
        FlushBatchDraw();
        
        // 清空输入缓冲区
        while (_kbhit()) _getch();
        
        // 等待任意按键
        while (1) {
            if (_kbhit()) {
                _getch();
                break;
            }
            if (GetAsyncKeyState(VK_SPACE) & 0x8000 ||
                GetAsyncKeyState(VK_RETURN) & 0x8000 ||
                GetAsyncKeyState('A') & 0x8000 ||
                GetAsyncKeyState('S') & 0x8000 ||
                GetAsyncKeyState('D') & 0x8000 ||
                GetAsyncKeyState('W') & 0x8000) {
                Sleep(100);
                break;
            }
            Sleep(50);
        }
    }
}


void showScore() {
    // 设置背景
    setbkcolor(RGB(20, 20, 40));
    cleardevice();
    
    // 绘制渐变背景
    for (int i = 0; i < HEIGHT; i++) {
        int r = 20 + (i * 10) / HEIGHT;
        int g = 20 + (i * 15) / HEIGHT;
        int b = 40 + (i * 20) / HEIGHT;
        setlinecolor(RGB(r, g, b));
        line(0, i, WIDTH, i);
    }
    
    // 绘制装饰性边框
    setlinecolor(RGB(100, 150, 255));
    setlinestyle(PS_SOLID, 3);
    roundrect(50, 50, WIDTH - 50, HEIGHT - 50, 20, 20);
    
    setbkmode(TRANSPARENT);
    
    // 主标题
    settextstyle(70, 0, _T("黑体"));
    settextcolor(RGB(50, 50, 50));
    outtextxy(WIDTH / 2 - 123, HEIGHT / 2 - 143, _T("游戏结束"));
    settextcolor(RGB(255, 215, 0)); // 金色
    outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 140, _T("游戏结束"));
    
    // 显示最终得分
    settextstyle(40, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    TCHAR buf[128];
    swprintf_s(buf, 128, _T("最终得分: %d"), score);
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 - 60, buf);
    
    // 绘制分数装饰
    setfillcolor(RGB(255, 215, 0));
    for (int i = 0; i < 5; i++) {
        solidcircle(150 + i * 120, HEIGHT / 2 - 20, 8);
    }
    
    // 根据游戏结束原因显示不同的信息
    settextstyle(25, 0, _T("黑体"));
    if (playerLife <= 0) {
        settextcolor(RGB(255, 100, 100));
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("生命值耗尽！"));
    } else if (!base.alive) {
        settextcolor(RGB(255, 100, 100));
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("基地被摧毁！"));
    } else {
        settextcolor(RGB(100, 255, 100));
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 30, _T("恭喜通关所有关卡！"));
        
        // 通关庆祝效果
        for (int i = 0; i < 8; i++) {
            setfillcolor(RGB(100, 255, 100));
            solidcircle(200 + i * 80, HEIGHT / 2 + 80, 12);
        }
    }
    
    // 游戏统计信息
    settextstyle(20, 0, _T("黑体"));
    settextcolor(RGB(200, 200, 200));
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 100, _T("感谢游玩FC坦克大战！"));
    
    // 提示按ESC返回主界面
    settextstyle(25, 0, _T("黑体"));
    settextcolor(RGB(255, 255, 255));
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 140, _T("按 ESC 退回主界面"));
    
    // 闪烁提示
    static int blinkFrame = 0;
    blinkFrame = (blinkFrame + 1) % 60;
    if (blinkFrame < 30) {
        settextcolor(RGB(255, 255, 0));
        outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 170, _T("按ESC键继续"));
    }
    
    FlushBatchDraw();
    
    // 等待ESC键
    while (1) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            Sleep(100);
            break;
        }
        Sleep(50);
    }
}

// ===================== 道具系统 =====================
void spawnProp() {
    if (props.size() >= MAX_PROPS) return;
    if (rand() % 400 == 0) {
        int px = (rand() % MAP_COL) * BLOCK_SIZE;
        int py = (rand() % (MAP_ROW - 2)) * BLOCK_SIZE;
        int t = rand() % 4;
        props.push_back({ px, py, t, true, PROP_DURATION });
    }
}
void drawProps() {
    for (auto& p : props) {
        if (!p.alive) continue;
        switch (p.type) {
        case 0: setfillcolor(RGB(255, 0, 0)); break; // 加血
        case 1: setfillcolor(RGB(255, 255, 0)); break; // 无敌
        case 2: 
            // 加速道具闪烁效果
            if ((p.duration / 10) % 2 == 0) {
                setfillcolor(RGB(0, 255, 255)); // 青色
            } else {
                setfillcolor(RGB(255, 255, 0)); // 黄色
            }
            break; // 加速
        case 3: setfillcolor(RGB(0, 128, 255)); break; // 双发
        }
        fillcircle(p.x + BLOCK_SIZE / 2, p.y + BLOCK_SIZE / 2, 16);
        settextstyle(12, 0, _T("黑体"));
        settextcolor(WHITE);
        switch (p.type) {
        case 0: outtextxy(p.x + BLOCK_SIZE / 2 - 6, p.y + BLOCK_SIZE / 2 - 6, _T("加血")); break;
        case 1: outtextxy(p.x + BLOCK_SIZE / 2 - 6, p.y + BLOCK_SIZE / 2 - 6, _T("无敌")); break;
        case 2: outtextxy(p.x + BLOCK_SIZE / 2 - 6, p.y + BLOCK_SIZE / 2 - 6, _T("加速")); break;
        case 3: outtextxy(p.x + BLOCK_SIZE / 2 - 6, p.y + BLOCK_SIZE / 2 - 6, _T("双发")); break;
        }
    }
}
void propCollision() {
    for (auto& p : props) {
        if (!p.alive) continue;
        if (player.x + BLOCK_SIZE > p.x && player.x < p.x + BLOCK_SIZE &&
            player.y + BLOCK_SIZE > p.y && player.y < p.y + BLOCK_SIZE) {
            switch (p.type) {
            case 0: 
                playerLife = min(playerLife + 1, MAX_LIFE); 
                break;
            case 1: 
                player.prop_invincible = PROP_DURATION; 
                player.color = RGB(255, 255, 0); // 无敌时变黄色
                break;
            case 2: 
                player.prop_speed = PROP_DURATION;
                player.speed = PLAYER_BOOST_SPEED; 
                player.color = RGB(0, 255, 255); // 加速时变青色
                break;
            case 3: 
                player.prop_double = PROP_DURATION; 
                player.color = RGB(0, 128, 255); // 双发时变蓝色
                break;
            }
            p.alive = false;
            mciSendString(L"play resource/pick.wav", 0, 0, 0); // 播放拾取道具音效
        }
    }
}
void updateProps() {
    for (auto& p : props) {
        if (p.alive) p.duration--;
        if (p.duration <= 0) p.alive = false;
    }
    if (player.prop_invincible > 0) player.prop_invincible--;
    if (player.prop_double > 0) player.prop_double--;
    if (player.prop_speed > 0) player.prop_speed--;
    
    // 检查buff是否结束，恢复原始颜色
    if (player.prop_invincible <= 0 && player.prop_double <= 0 && player.prop_speed <= 0) {
        player.color = player.originalColor;
    }
    
    // 恢复正常速度
    if (player.prop_speed <= 0 && player.speed > PLAYER_SPEED) {
        player.speed = PLAYER_SPEED;
    }
}

// ===================== 爆炸动画 =====================

void drawExplosions() {
    for (auto& e : explosions) {
        if (!e.alive) continue;
        setfillcolor(RGB(255, 128, 0));
        solidcircle(e.x, e.y, e.frame * 8);
    }
}
void updateExplosions() {
    for (auto& e : explosions) {
        if (!e.alive) continue;
        e.frame++;
        if (e.frame > 5) e.alive = false;
    }
}

// ===================== 玩家多命与界面信息 =====================
void drawInfo() {
    settextstyle(20, 0, _T("黑体"));
    settextcolor(WHITE);
    TCHAR buf[128];
    
    // 左上角基本信息
    if (isChallengeMode) {
        swprintf_s(buf, 128, _T("分数:%d  关卡:%d  敌人:%d  生命:%d  子弹:%d"), score, currentLevel + 1, (int)enemies.size(), playerLife, playerBullets);
    } else {
        swprintf_s(buf, 128, _T("分数:%d  关卡:%d  敌人:%d  生命:%d"), score, currentLevel + 1, (int)enemies.size(), playerLife);
    }
    outtextxy(10, 10, buf);
    
    // 右上角buff状态显示
    int buffY = 10;
    int buffX = WIDTH - 200;
    
    if (player.prop_invincible > 0) {
        settextcolor(RGB(255, 255, 0)); // 黄色
        swprintf_s(buf, 128, _T("无敌: %d"), player.prop_invincible / 20); // 转换为秒
        outtextxy(buffX, buffY, buf);
        buffY += 25;
    }
    
    if (player.prop_speed > 0) {
        settextcolor(RGB(0, 255, 255)); // 青色
        swprintf_s(buf, 128, _T("加速: %d"), player.prop_speed / 20); // 转换为秒
        outtextxy(buffX, buffY, buf);
        buffY += 25;
    }
    
    if (player.prop_double > 0) {
        settextcolor(RGB(0, 128, 255)); // 蓝色
        swprintf_s(buf, 128, _T("双发: %d"), player.prop_double / 20); // 转换为秒
        outtextxy(buffX, buffY, buf);
        buffY += 25;
    }
    
    // BOSS关卡显示BOSS血量
    if (isBossLevel && boss.alive) {
        settextstyle(18, 0, _T("黑体"));
        settextcolor(RGB(255, 0, 0));
        swprintf_s(buf, 128, _T("BOSS血量: %d/%d"), boss.hp, boss.maxHp);
        outtextxy(WIDTH / 2 - 80, 10, buf);
        
        // 显示不移动倒计时
        if (boss.immobileTimer > 0) {
            settextcolor(RGB(255, 255, 0)); // 黄色
            int remainingSeconds = boss.immobileTimer / 20; // 转换为秒
            swprintf_s(buf, 128, _T("BOSS静止: %d秒"), remainingSeconds);
            outtextxy(WIDTH / 2 - 80, 35, buf);
        }
    }
    
    // 恢复白色
    settextcolor(WHITE);
}

int main() {
    srand((unsigned)time(0));
    initgraph(WIDTH, HEIGHT);
    BeginBatchDraw(); // 开始双缓冲绘图
    while (1) {
        currentLevel = selectLevel();
        
        // 检查是否为挑战模式或BOSS关卡
        isChallengeMode = (currentLevel == 3);
        isBossLevel = (currentLevel == 4);
        
        initMap(currentLevel);
        initPlayer();
        initEnemies();
        if (isBossLevel) {
            initBoss();
        }

        while (1) {
            // 检测ESC键退出
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                // 显示退出确认界面
                cleardevice();
                setbkcolor(RGB(20, 20, 40));
                
                // 绘制渐变背景
                for (int i = 0; i < HEIGHT; i++) {
                    int r = 20 + (i * 10) / HEIGHT;
                    int g = 20 + (i * 15) / HEIGHT;
                    int b = 40 + (i * 20) / HEIGHT;
                    setlinecolor(RGB(r, g, b));
                    line(0, i, WIDTH, i);
                }
                
                setbkmode(TRANSPARENT);
                settextstyle(40, 0, _T("黑体"));
                settextcolor(RGB(255, 255, 255));
                outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 50, _T("确认退出游戏？"));
                
                settextstyle(25, 0, _T("黑体"));
                settextcolor(RGB(200, 200, 200));
                outtextxy(WIDTH / 2 - 150, HEIGHT / 2, _T("按ESC确认退出，按其他键继续游戏"));
                
                FlushBatchDraw();
                
                // 等待用户选择
                while (1) {
                    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                        Sleep(100);
                        score = 0;
                        playerLife = MAX_LIFE;
                        goto exit_game; // 退出到主菜单
                    }
                    // 检查其他按键继续游戏
                    if (GetAsyncKeyState(VK_SPACE) & 0x8000 ||
                        GetAsyncKeyState(VK_RETURN) & 0x8000 ||
                        GetAsyncKeyState('A') & 0x8000 ||
                        GetAsyncKeyState('S') & 0x8000 ||
                        GetAsyncKeyState('D') & 0x8000 ||
                        GetAsyncKeyState('W') & 0x8000) {
                        Sleep(100);
                        break; // 继续游戏
                    }
                    Sleep(50);
                }
            }
            
            cleardevice();
            drawBlocks();
            drawBase();
            drawTank(player);
            for (auto& e : enemies) drawTank(e);
            if (isBossLevel) {
                drawBoss();
            }
            drawBullets();
            drawProps();
            drawExplosions();
            drawInfo();

            moveBullets();
            bulletCollision();
            enemyAI();
            if (isBossLevel) {
                bossAI();
            }
            playerControl();
            spawnProp();
            propCollision();
            updateProps();
            updateExplosions();
            
            // 挑战模式子弹刷新逻辑（每10秒刷新一颗子弹）
            if (isChallengeMode) {
                bulletRefreshTimer++;
                if (bulletRefreshTimer >= 500) { // 500帧 = 10秒 (20fps * 10)
                    playerBullets++;
                    bulletRefreshTimer = 0;
                }
            }

            int result = checkGameOver();
            if (result != 0) {
                showResult(result);
                if (result == 1) {
                    currentLevel++;
                    if (currentLevel >= LEVELS) {
                        showScore();
                        break;
                    }
                    initMap(currentLevel);
                    initPlayer();
                    initEnemies();
                }
                else {
                    score = 0;
                    playerLife = MAX_LIFE;
                    break;
                }
            }

            EndBatchDraw(); // 结束双缓冲绘图，将内存中的画面复制到屏幕上
            Sleep(20);
            BeginBatchDraw(); // 开始下一轮双缓冲绘图
        }
        
        exit_game: // 退出标签
        ; // 空语句，标签后需要语句
    }
    EndBatchDraw(); // 结束双缓冲绘图
    closegraph();
    return 0;
}