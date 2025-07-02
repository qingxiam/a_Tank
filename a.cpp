#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define WIDTH 800
#define HEIGHT 600
#define BLOCK_SIZE 40
#define MAP_ROW 15
#define MAP_COL 20
#define ENEMY_NUM 5
#define LEVELS 3
#define MAX_LIFE 3
#define MAX_PROPS 2
#define PROP_DURATION 600

enum Direction { UP, DOWN, LEFT, RIGHT };

// 0空地 1砖块 2铁块 3水 4草 5基地
const int level_maps[LEVELS][MAP_ROW][MAP_COL] = {
    // 关卡1
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5}
    },
    // 关卡2
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5}
    },
    // 关卡3
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5}
    }
};

struct Block {
    int x, y, type; // type: 1砖块 2铁块 3水 4草 5基地
    bool alive;
};
struct Tank {
    int x, y;
    Direction dir;
    COLORREF color;
    bool isPlayer;
    int cooldown;
    int type; // 0普通 1快速 2高血量 3智能
    int hp;
    int speed;
    int prop_double; // 双发道具剩余帧
    int prop_invincible; // 无敌剩余帧
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

std::vector<Block> blocks;
std::vector<Bullet> bullets;
std::vector<Tank> enemies;
std::vector<Prop> props;
std::vector<Explosion> explosions;
Tank player;
Base base;
int currentLevel = 0;
int playerLife = MAX_LIFE;
int score = 0;

// ===================== 菜单与关卡选择 =====================
void showMenu() {
    setbkcolor(RGB(30,30,30));
    cleardevice();
    settextstyle(60, 0, _T("黑体"));
    settextcolor(WHITE);
    outtextxy(200, 100, _T("FC坦克大战"));
    settextstyle(30, 0, _T("黑体"));
    outtextxy(250, 250, _T("按 1/2/3 选择关卡，ESC 退出"));
}

int selectLevel() {
    showMenu();
    while (1) {
        if (GetAsyncKeyState('1') & 0x8000) return 0;
        if (GetAsyncKeyState('2') & 0x8000) return 1;
        if (GetAsyncKeyState('3') & 0x8000) return 2;
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
        case 3: setfillcolor(RGB(0, 100, 200)); break; // 水
        case 4: setfillcolor(RGB(0, 200, 0)); break; // 草
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
    player.x = 9 * BLOCK_SIZE;
    player.y = 13 * BLOCK_SIZE;
    player.dir = UP;
    player.color = RGB(0, 255, 0);
    player.isPlayer = true;
    player.cooldown = 0;
    player.type = 0;
    player.hp = 1;
    player.speed = 4;
    player.prop_double = 0;
    player.prop_invincible = 0;
}
void initEnemies() {
    enemies.clear();
    for (int i = 0; i < ENEMY_NUM; i++) {
        Tank e;
        e.x = (i % 3) * 8 * BLOCK_SIZE + BLOCK_SIZE;
        e.y = BLOCK_SIZE;
        e.dir = DOWN;
        e.isPlayer = false;
        e.cooldown = rand() % 30;
        e.type = rand() % 4;
        switch (e.type) {
        case 0: e.color = RGB(255, 0, 0); e.hp = 1; e.speed = 2; break; // 普通
        case 1: e.color = RGB(255, 128, 0); e.hp = 1; e.speed = 4; break; // 快速
        case 2: e.color = RGB(128, 0, 255); e.hp = 3; e.speed = 2; break; // 高血量
        case 3: e.color = RGB(0, 200, 255); e.hp = 2; e.speed = 2; break; // 智能
        }
        e.prop_double = 0;
        e.prop_invincible = 0;
        enemies.push_back(e);
    }
}

// ===================== 精美化坦克与子弹 =====================
void drawTank(const Tank& t) {
    setfillcolor(t.color);
    fillroundrect(t.x, t.y, t.x + BLOCK_SIZE, t.y + BLOCK_SIZE, 10, 10);
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
    solidcircle(x, y, frame * 6);
}

// ===================== 子弹、碰撞、AI、操作 =====================
void moveBullets() {
    for (auto& b : bullets) {
        if (!b.alive) continue;
        switch (b.dir) {
        case UP: b.y -= 8; break;
        case DOWN: b.y += 8; break;
        case LEFT: b.x -= 8; break;
        case RIGHT: b.x += 8; break;
        }
        if (b.x < 0 || b.x > WIDTH || b.y < 0 || b.y > HEIGHT)
            b.alive = false;
    }
}
void bulletCollision() {
    for (auto& b : bullets) {
        if (!b.alive) continue;
        // 碰砖块/铁块/基地
        for (auto& bl : blocks) {
            if (bl.alive &&
                b.x > bl.x && b.x < bl.x + BLOCK_SIZE &&
                b.y > bl.y && b.y < bl.y + BLOCK_SIZE) {
                if (bl.type == 1) bl.alive = false; // 砖块可破坏
                if (bl.type == 5) base.alive = false; // 基地被打
                b.alive = false;
            }
        }
        // 敌人被打
        if (b.fromPlayer) {
            for (auto& e : enemies) {
                if (e.x < b.x && b.x < e.x + BLOCK_SIZE &&
                    e.y < b.y && b.y < e.y + BLOCK_SIZE) {
                    e.x = -1000;
                    b.alive = false;
                }
            }
        }
        // 玩家被打
        if (!b.fromPlayer) {
            if (player.x < b.x && b.x < player.x + BLOCK_SIZE &&
                player.y < b.y && b.y < player.y + BLOCK_SIZE) {
                player.x = -1000;
                b.alive = false;
            }
        }
    }
}
bool canMove(int x, int y) {
    if (x < 0 || x + BLOCK_SIZE > WIDTH || y < 0 || y + BLOCK_SIZE > HEIGHT)
        return false;
    for (auto& bl : blocks) {
        if (bl.alive && bl.type != 4 && bl.type != 3 &&
            x + BLOCK_SIZE > bl.x && x < bl.x + BLOCK_SIZE &&
            y + BLOCK_SIZE > bl.y && y < bl.y + BLOCK_SIZE)
            return false;
    }
    return true;
}
void enemyAI() {
    for (auto& e : enemies) {
        if (e.x < 0) continue;
        if (rand() % 20 == 0) e.dir = (Direction)(rand() % 4);
        int nx = e.x, ny = e.y;
        switch (e.dir) {
        case UP: ny -= 2; break;
        case DOWN: ny += 2; break;
        case LEFT: nx -= 2; break;
        case RIGHT: nx += 2; break;
        }
        if (canMove(nx, ny)) {
            e.x = nx; e.y = ny;
        }
        if (e.cooldown > 0) e.cooldown--;
        else {
            bullets.push_back({ e.x + BLOCK_SIZE / 2, e.y + BLOCK_SIZE / 2, e.dir, false, true });
            e.cooldown = 50 + rand() % 50;
        }
    }
}
void playerControl() {
    int nx = player.x, ny = player.y;
    if (GetAsyncKeyState('W') & 0x8000) { player.dir = UP; ny -= 4; }
    else if (GetAsyncKeyState('S') & 0x8000) { player.dir = DOWN; ny += 4; }
    else if (GetAsyncKeyState('A') & 0x8000) { player.dir = LEFT; nx -= 4; }
    else if (GetAsyncKeyState('D') & 0x8000) { player.dir = RIGHT; nx += 4; }
    if (canMove(nx, ny)) {
        player.x = nx; player.y = ny;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (player.cooldown == 0) {
            bullets.push_back({ player.x + BLOCK_SIZE / 2, player.y + BLOCK_SIZE / 2, player.dir, true, true });
            player.cooldown = 20;
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
    if (player.x < 0) return -1; // 玩家死
    if (!base.alive || checkBaseHit()) return -2; // 基地被毁
    if (!enemyAlive) return 1;   // 敌人全死
    return 0; // 游戏继续
}

void showResult(int result) {
    settextstyle(60, 0, _T("黑体"));
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    if (result == 1)
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 30, _T("胜利！进入下一关"));
    else if (result == -1)
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 30, _T("你输了！"));
    else if (result == -2)
        outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 30, _T("基地被毁！"));
    Sleep(2000);
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
        case 0: setfillcolor(RGB(255,0,0)); break; // 加血
        case 1: setfillcolor(RGB(255,255,0)); break; // 无敌
        case 2: setfillcolor(RGB(0,255,0)); break; // 加速
        case 3: setfillcolor(RGB(0,128,255)); break; // 双发
        }
        fillcircle(p.x + BLOCK_SIZE/2, p.y + BLOCK_SIZE/2, 16);
    }
}
void propCollision() {
    for (auto& p : props) {
        if (!p.alive) continue;
        if (player.x + BLOCK_SIZE > p.x && player.x < p.x + BLOCK_SIZE &&
            player.y + BLOCK_SIZE > p.y && player.y < p.y + BLOCK_SIZE) {
            switch (p.type) {
            case 0: playerLife = min(playerLife + 1, MAX_LIFE); break;
            case 1: player.prop_invincible = PROP_DURATION; break;
            case 2: player.speed = 8; break;
            case 3: player.prop_double = PROP_DURATION; break;
            }
            p.alive = false;
            // mciSendString("play resource/pick.wav", 0, 0, 0); // 如有音效
        }
    }
}
void updateProps() {
    for (auto& p : props) {
        if (p.alive) p.duration--;
        if (p.duration <= 0) p.alive = false;
    }
}

// ===================== 爆炸动画 =====================
void addExplosion(int x, int y) {
    explosions.push_back({x, y, 0, true});
    // mciSendString("play resource/explode.wav", 0, 0, 0); // 如有音效
}
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
    swprintf_s(buf, 128, _T("分数:%d  关卡:%d  敌人:%d  生命:%d"), score, currentLevel+1, (int)enemies.size(), playerLife);
    outtextxy(10, 10, buf);
    if (player.prop_invincible > 0) outtextxy(10, 40, _T("无敌中"));
    if (player.prop_double > 0) outtextxy(10, 60, _T("双发中"));
    if (player.speed > 4) outtextxy(10, 80, _T("加速中"));
}

// ===================== 主程序入口 =====================
int main() {
    srand((unsigned)time(0));
    initgraph(WIDTH, HEIGHT);
    while (1) {
        currentLevel = selectLevel();
        for (int level = currentLevel; level < LEVELS; ++level) {
            initMap(level);
            initPlayer();
            initEnemies();
            int result = 0;
            while (1) {
                cleardevice();
                drawBlocks();
                drawBase();
                drawTank(player);
                for (auto& e : enemies) if (e.x >= 0) drawTank(e);
                drawBullets();
                playerControl();
                enemyAI();
                moveBullets();
                bulletCollision();
                spawnProp();
                drawProps();
                propCollision();
                updateProps();
                drawExplosions();
                updateExplosions();
                drawInfo();
                result = checkGameOver();
                if (result != 0) break;
                FlushBatchDraw();
                Sleep(16);
            }
            cleardevice();
            showResult(result);
            if (result != 1) break; // 失败或基地被毁则退出
        }
    }
    closegraph();
    return 0;
}