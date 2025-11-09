#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BUTTON_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- Modes --------
enum GameMode { MENU, FLAPPY, PONG };
GameMode currentGame = MENU;

// -------- Button --------
unsigned long lastPress = 0;
bool buttonHeld = false;

// -------- Flappy Bird --------
int birdY, velocity, pipeX, gapY;
bool doublePipe, singleTop;
const int birdSize = 8;
const int gravity = 1;
const int jumpStrength = -4;
const int gapHeight = 27;
const int pipeWidth = 10;
const int singlePillarHeight = 20;
int gameSpeed;
int score;

// -------- Pong --------
int playerY = 26, compY = 26;
int paddleH = 16;
int ballX = SCREEN_WIDTH / 2, ballY = SCREEN_HEIGHT / 2;
int ballDX = -2, ballDY = 1;
int playerSpeed = 2;
float compYf = 26;
float compSpeed = 0.9;   // Easier AI
int scorePlayer = 0, scoreComp = 0;

// ======================================================
// ===============  Button Utils =========================
// ======================================================
bool buttonPressed() {
  static bool lastState = HIGH;
  bool curr = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && curr == LOW) {
    lastState = LOW;
    return true;
  }
  if (curr == HIGH) lastState = HIGH;
  return false;
}

bool buttonLongPress() {
  if (digitalRead(BUTTON_PIN) == LOW && !buttonHeld) {
    lastPress = millis();
    buttonHeld = true;
  }
  if (buttonHeld && digitalRead(BUTTON_PIN) == HIGH) {
    buttonHeld = false;
    return (millis() - lastPress) > 600;
  }
  return false;
}

// ======================================================
// ====================  MENU  ===========================
// ======================================================
int menuIndex = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  drawHomeScreen();
}

void loop() {
  if (currentGame == MENU) menuLoop();
  else if (currentGame == FLAPPY) flappyLoop();
  else if (currentGame == PONG) pongLoop();
}

void drawHomeScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(38, 8);
  display.println("GAMES");
  display.display();
  delay(800);
}

void menuLoop() {
  if (buttonPressed()) menuIndex = (menuIndex + 1) % 2;
  if (buttonLongPress()) {
    if (menuIndex == 0) startFlappy();
    else startPong();
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(38, 8);
  display.println("GAMES");

  display.setTextSize(1);
  display.setCursor(28, 35);  // shifted left
  display.print(menuIndex == 0 ? "> " : "  ");
  display.println("Flappy Bird");

  display.setCursor(28, 48);  // shifted left
  display.print(menuIndex == 1 ? "> " : "  ");
  display.println("Pong");

  display.display();
}

// ======================================================
// ===============  FLAPPY BIRD  =========================
// ======================================================
void startFlappy() {
  birdY = SCREEN_HEIGHT / 2;
  velocity = 0;
  pipeX = SCREEN_WIDTH;
  gapY = 30;
  doublePipe = true;
  singleTop = false;
  score = 0;
  gameSpeed = 50;
  currentGame = FLAPPY;
}

void flappyLoop() {
  if (digitalRead(BUTTON_PIN) == LOW) velocity = jumpStrength;

  velocity += gravity;
  if (velocity > 3) velocity = 3;
  birdY += velocity;
  if (birdY < 0) birdY = 0;
  if (birdY > SCREEN_HEIGHT - birdSize) birdY = SCREEN_HEIGHT - birdSize;

  pipeX -= 2;
  if (pipeX + pipeWidth < 0) {
    pipeX = SCREEN_WIDTH;
    gapY = random(5, SCREEN_HEIGHT - gapHeight - 5);
    doublePipe = random(0, 2);
    if (!doublePipe) singleTop = random(0, 2);
    score++;
    if (score >= 12) gameSpeed = 35;
  }

  if (checkCollision()) {
    gameOverFlappy();
    return;
  }

  display.clearDisplay();
  display.fillRect(10, birdY, birdSize, birdSize, SSD1306_WHITE);
  if (doublePipe) {
    display.fillRect(pipeX, 0, pipeWidth, gapY, SSD1306_WHITE);
    display.fillRect(pipeX, gapY + gapHeight, pipeWidth,
                     SCREEN_HEIGHT - (gapY + gapHeight), SSD1306_WHITE);
  } else {
    if (singleTop)
      display.fillRect(pipeX, 0, pipeWidth, singlePillarHeight, SSD1306_WHITE);
    else
      display.fillRect(pipeX, SCREEN_HEIGHT - singlePillarHeight,
                       pipeWidth, singlePillarHeight, SSD1306_WHITE);
  }

  display.setTextSize(1);
  display.setCursor(110, 0);
  display.print(score);
  display.display();
  delay(gameSpeed);
}

bool checkCollision() {
  if (birdY <= 0 || birdY + birdSize >= SCREEN_HEIGHT) return true;
  int birdX1 = 10, birdX2 = 10 + birdSize;
  if (birdX2 >= pipeX && birdX1 <= pipeX + pipeWidth) {
    if (doublePipe) {
      if (birdY < gapY || birdY + birdSize > gapY + gapHeight) return true;
    } else {
      if (singleTop && birdY < singlePillarHeight) return true;
      if (!singleTop && birdY + birdSize > SCREEN_HEIGHT - singlePillarHeight)
        return true;
    }
  }
  return false;
}

void gameOverFlappy() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println("Game Over");
  display.setTextSize(1);
  display.setCursor(40, 45);
  display.print("Score: ");
  display.print(score);
  display.display();
  delay(2000);
  currentGame = MENU;
  drawHomeScreen();
}

// ======================================================
// ===============  ONE-BUTTON PONG  ====================
// ======================================================
void startPong() {
  playerY = 26;
  compYf = 26;
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballDX = -2;
  ballDY = 1;
  scorePlayer = 0;
  scoreComp = 0;
  currentGame = PONG;
}

void pongLoop() {
  // Player movement
  if (digitalRead(BUTTON_PIN) == LOW) playerY -= playerSpeed;
  else playerY += playerSpeed;
  if (playerY < 0) playerY = 0;
  if (playerY > SCREEN_HEIGHT - paddleH) playerY = SCREEN_HEIGHT - paddleH;

  // Easier AI (slower + random delay)
  int compCenter = compYf + paddleH / 2;
  float randomness = random(-2, 3) * 0.2;
  if (abs(compCenter - ballY) > 5) {
    if (compCenter < ballY) compYf += (compSpeed + randomness);
    else compYf -= (compSpeed + randomness);
  }
  compY = (int)compYf;
  if (compY < 0) compY = 0;
  if (compY > SCREEN_HEIGHT - paddleH) compY = SCREEN_HEIGHT - paddleH;

  // Ball
  ballX += ballDX;
  ballY += ballDY;
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 2) ballDY = -ballDY;

  // Paddle collisions
  if (ballX <= 5 && ballY + 2 >= playerY && ballY <= playerY + paddleH) {
    ballDX = -ballDX;
    ballX = 6;
  }
  if (ballX >= SCREEN_WIDTH - 7 &&
      ballY + 2 >= compY && ballY <= compY + paddleH) {
    ballDX = -ballDX;
    ballX = SCREEN_WIDTH - 8;
  }

  // Scoring
  if (ballX < 0) { scoreComp++; resetBall(); }
  else if (ballX > SCREEN_WIDTH) { scorePlayer++; resetBall(); }

  // End condition
  if (scorePlayer >= 10 || scoreComp >= 10) {
    pongGameOver();
    return;
  }

  display.clearDisplay();
  display.fillRect(2, playerY, 3, paddleH, SSD1306_WHITE);
  display.fillRect(SCREEN_WIDTH - 5, compY, 3, paddleH, SSD1306_WHITE);
  display.fillRect(ballX, ballY, 2, 2, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(55, 0);
  display.print(scorePlayer);
  display.print("-");
  display.print(scoreComp);
  display.display();
  delay(20);
}

void resetBall() {
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballDX = (random(0, 2) == 0) ? -2 : 2;
  ballDY = (random(0, 2) == 0) ? -1 : 1;
}

void pongGameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 20);
  if (scorePlayer > scoreComp) display.println("You Win!");
  else display.println("You Lose");
  display.display();
  delay(2000);
  currentGame = MENU;
  drawHomeScreen();
}
