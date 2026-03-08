#include <cstddef>
#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
//stupidnig
using namespace std;

Color black = GetColor(0x000000FF);
Color beige = GetColor(0xF5F5DCFF);
int Cell_size = 30;
int Cell_count = 25;
int offset = 75;

double LastUpdatetime = 0;

bool EventTriggered(double interval) {
  double CurrentTime = GetTime();
  if (CurrentTime - LastUpdatetime >= interval) {
    LastUpdatetime = CurrentTime;
    return true;
  }
  return false;
}

bool ElementInDequeue(Vector2 element, deque<Vector2> deque) {
  for (int i = 0; i < deque.size(); i++) {
    if (Vector2Equals(element, deque[i])) {
      return true;
    }
  }
  return false;
}

class Food {
public:
  Vector2 position = {5, 6};
  Texture2D texture;

  Food() {
    Image watermelon = LoadImage("watermelon.png");
    ImageResize(&watermelon, Cell_size, Cell_size);
    texture = LoadTextureFromImage(watermelon);
    UnloadImage(watermelon);
  }
  void Draw() {
    DrawTexture(texture, (float)offset + (int)(position.x * Cell_size),
                (float)offset + (int)(position.y * Cell_size), WHITE);
  }
  Vector2 GenerateRandomCell() {
    float y = GetRandomValue(0, Cell_count - 1);
    float x = GetRandomValue(0, Cell_count - 1);
    return Vector2{x, y};
  }
  Vector2 generate_random_pos(deque<Vector2> snakebody) {
    Vector2 position = GenerateRandomCell();
    while (ElementInDequeue(position, snakebody)) {
      position = GenerateRandomCell();
    }
    return position;
  }
};

class Snake {
public:
  deque<Vector2> body = {Vector2{5, 9}, Vector2{6, 9}, Vector2{7, 9}};
  Vector2 direction = {1, 0};
  bool addSegment = false;

  void Draw() {
    for (int i = 0; i <= body.size() - 1; i++) {
      int x = body[i].x;
      int y = body[i].y;
      DrawRectangleRounded(Rectangle{(float)offset + (float)x * Cell_size,
                                     (float)offset + (float)y * Cell_size,
                                     (float)Cell_size, (float)Cell_size},
                           0.5, 6, black);
    }
  }

  void Update() {
    body.push_front(Vector2Add(body[0], direction));
    if (addSegment == true) {
      addSegment = false;
    } else {
      body.pop_back();
    }
  }

  void Reset() {
    body = {Vector2{5, 9}, Vector2{6, 9}, Vector2{7, 9}};
    direction = {1, 0};
  }
};

class Game {
public:
  Food watermelon = Food();
  Snake niggasnake = Snake();
  bool running = true;
  Sound eatsound;
  Sound wallsound;

  void Draw() {
    niggasnake.Draw();
    watermelon.Draw();
  }

  Game() {
    InitAudioDevice();
    eatsound = LoadSound("Boom Sound Effect - Soundspace Sound Effects.mp3");
    wallsound = LoadSound("lmao.mp3");
  }

  void Update() {
    if (running) {
      niggasnake.Update();
      CheckCollisionNiggaSnakeAndWaterMelon();
      CheckCollisionWithEdge();
      CheckCollisionTail();
    }
  }

  void CheckCollisionTail() {
    deque<Vector2> headlessbody = niggasnake.body;
    headlessbody.pop_front();
    if (ElementInDequeue(niggasnake.body[0], headlessbody)) {
      GameOver();
    }
  }

  void CheckCollisionNiggaSnakeAndWaterMelon() {
    if (Vector2Equals(niggasnake.body[0], watermelon.position)) {
      watermelon.position = watermelon.generate_random_pos(niggasnake.body);
      niggasnake.addSegment = true;
      PlaySound(eatsound);
    }
  }

  void CheckCollisionWithEdge() {
    if (niggasnake.body[0].x == Cell_count || niggasnake.body[0].x == -1) {
      PlaySound(wallsound);
      GameOver();
    }
    if (niggasnake.body[0].y == Cell_count || niggasnake.body[0].y == -1) {
      PlaySound(wallsound);
      GameOver();
    }
  }

  void GameOver() {
    niggasnake.Reset();
    watermelon.position = watermelon.generate_random_pos(niggasnake.body);
    running = false;
  }
};
int main() {
  cout << "u stupid nigga" << endl;

  InitWindow(2 * offset + Cell_size * Cell_count,
             2 * offset + Cell_size * Cell_count, "niggasnake");
  SetTargetFPS(60);
  Game game = Game();

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(beige);

    if (EventTriggered(0.2)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_W) && game.niggasnake.direction.y != 1) {
      game.niggasnake.direction = {0, -1};
      game.running = true;
    }
    if (IsKeyPressed(KEY_D) && game.niggasnake.direction.x != -1) {
      game.niggasnake.direction = {1, 0};
      game.running = true;
    }
    if (IsKeyPressed(KEY_S) && game.niggasnake.direction.y != -1) {
      game.niggasnake.direction = {0, 1};
      game.running = true;
    }
    if (IsKeyPressed(KEY_A) && game.niggasnake.direction.x != 1) {
      game.niggasnake.direction = {-1, 0};
      game.running = true;
    }
    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5,
                                   (float)Cell_size * Cell_count + 10,
                                   (float)Cell_size * Cell_count + 10},
                         5, BLACK);
    game.Draw();
    DrawText("Nigga Snake", offset - 5, 20, 40, black);
    EndDrawing();
  }
  CloseWindow();
}
