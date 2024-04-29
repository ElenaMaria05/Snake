#include<raylib.h>
#include<deque>
#include<raymath.h>
#include<iostream>
using namespace std;

Color green = { 173,204,96,255 };//RGB si alpha
Color darkGreen = { 43,51,24,255 };

int sizeCelule = 30;
int numarCelule = 25;
bool continua_joc = true;
int score = 0;

bool ElementInCoada(Vector2 element, deque<Vector2> coada) {
	for (unsigned int i = 0; i < coada.size(); i++) {
		if (Vector2Equals(coada[i], element)) {
			return true;
		}
	}
	return false;
}
double lastUpdateTime = 0;
bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Food {
public:
	Vector2 pozitie;
	Texture2D texture;

	Food() {
		Image imagine = LoadImage("Imagini/mar.png");
		texture = LoadTextureFromImage(imagine);
		UnloadImage(imagine);
		pozitie = pozitieRandom();
	}
	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, pozitie.x * sizeCelule, pozitie.y * sizeCelule, WHITE);
	}
	Vector2 pozitieRandom() {
		float x_mar = GetRandomValue(0, numarCelule - 1);
		float y_mar = GetRandomValue(0, numarCelule - 1);
		return Vector2{ x_mar,y_mar };
	}

};

class Snake {
public:
	deque<Vector2> body = { Vector2{12.0f, 12.0f}, Vector2{11.0f, 12.0f}, Vector2{10.0f, 12.0f} };
	Vector2 directie{ 1,0 };
	bool addSegment = false;
	void Draw() {
		for (unsigned int i = 0; i < body.size(); i++) {
			float x_snake = body[i].x;
			float y_snake = body[i].y;
			Rectangle segment = Rectangle{ x_snake * sizeCelule ,y_snake * sizeCelule ,(float)sizeCelule,(float)sizeCelule };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);

		}
	}
	void Update() {
		if (addSegment) {
			body.push_back(Vector2Add(body[0], directie));
			score++;
			addSegment = false;
		}
		else {
			body.pop_back();
			body.push_front(Vector2Add(body[0], directie));
		}
	}
	void Resetare(float x, float y) {
		body[0].x = x;
		body[0].y = y;
		Draw();
	}
};

class Game {
public:
	Snake snake = Snake();
	Food mar = Food();

	void Draw() {
		snake.Draw();
		mar.Draw();
	}
	void Update() {
		snake.Update();
		ColiziuneMargini();
		ColiziuneCoada();
	}

	void ColiziuneMar() {
		if (Vector2Equals(snake.body[0], mar.pozitie)) {
			mar.pozitie = mar.pozitieRandom();
			snake.addSegment = true;
		}
	}
	void ColiziuneMargini() {
		float x = snake.body[0].x;
		float y = snake.body[0].y;

		if (x < 0) {
			x = numarCelule - 1;
		}
		else if (x >= numarCelule) {
			x = 0;
		}

		if (y < 0) {
			y = numarCelule - 1;
		}
		else if (y >= numarCelule) {
			y = 0;
		}
		snake.Resetare(x, y);
	}
	void ColiziuneCoada() {
		deque<Vector2> corpFaraCap = snake.body;
		corpFaraCap.pop_front();
		if (ElementInCoada(snake.body[0], corpFaraCap)) {
			continua_joc = false;
		}
	}

};
int main() {

	InitWindow(sizeCelule * numarCelule, sizeCelule * numarCelule, "Snake");
	SetTargetFPS(60);

	Game game = Game();
	while (WindowShouldClose() == false) {
		BeginDrawing();
		ClearBackground(green);
		if (!continua_joc) {
			break;
		}
		if (eventTriggered(0.2)) {
			game.Update();
		}

		if (IsKeyDown(KEY_W) && game.snake.directie.y != 1) {
			game.snake.directie = { 0,-1 };
		}
		if (IsKeyDown(KEY_S) && game.snake.directie.y != -1) {
			game.snake.directie = { 0,1 };
		}
		if (IsKeyDown(KEY_A) && game.snake.directie.x != 1) {
			game.snake.directie = { -1,0 };
		}
		if (IsKeyDown(KEY_D) && game.snake.directie.x != -1) {
			game.snake.directie = { 1,0 };
		}

		game.ColiziuneMar();
		game.Draw();

		
		EndDrawing();
	}
	if (!continua_joc) {
		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(green);
			DrawText("JOCUL S-A TERMINAT", numarCelule * sizeCelule / 2 - 280, numarCelule * sizeCelule / 2 - 90, 50, darkGreen);
			DrawText(TextFormat("Ai obtinut scorul %i", score), numarCelule * sizeCelule / 2 - 130, numarCelule * sizeCelule / 2 - 30, 30, darkGreen);
			DrawText("apasa x pentru a iesi", numarCelule * sizeCelule / 2 - 100, numarCelule * sizeCelule / 2 +10 , 20, darkGreen);
			EndDrawing();
			
		}
	}
	
	CloseWindow();
	
	return 0;
}