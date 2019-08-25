// Required libs
//#include "FastLED.h"
// #include "I2Cdev.h"
// #include "MPU6050.h"
//#include "Wire.h"
// #include "toneAC.h"
//#include "iSin.h"
// #include "RunningMedian.h"

// Included libs
/* #include "Enemy.h"
#include "Particle.h"
#include "Spawner.h"
#include "Lava.h"
#include "Boss.h"
#include "Conveyor.h" */

void setup();
void loop();


// ---------------------------------
// ------------ LEVELS -------------
// ---------------------------------
void loadLevel();
void spawnBoss();
void moveBoss();
void spawnEnemy(int pos, int dir, int sp, int wobble);
void spawnLava(int left, int right, int ontime, int offtime, int offset, char* state);
void spawnConveyor(int startPoint, int endPoint, int dir);
void cleanupLevel();
void levelComplete();
void nextLevel();
void gameOver();
void die();

// ----------------------------------
// -------- TICKS & RENDERS ---------
// ----------------------------------
void tickEnemies();
void tickBoss();
void drawPlayer();
void drawExit();
void tickSpawners();
void tickLava();
bool tickParticles();
void tickConveyors();
void drawAttack();
int getLED(int pos);
bool inLava(int pos);
void updateLives();


// ---------------------------------
// --------- SCREENSAVER -----------
// ---------------------------------
void screenSaverTick();

// ---------------------------------
// ----------- JOYSTICK ------------
// ---------------------------------
void getInput();