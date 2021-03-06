#include "Application.h"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include <iostream>

Application::Application()
{

}

Application::~Application()
{

}

void Application::Run()
{
	InitWindow(m_windowWidth, m_windowHeight, "Game Of Life");

	SetTargetFPS(60);
	Load();

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		float dt = GetFrameTime();
		Update(dt);
		Draw();
	}

	Unload();
	CloseWindow();
}

void Application::Load()
{
	// allocate memory for the grid
	m_grid = new int[m_rows * m_cols];
	m_gridBuffer = new int[m_rows * m_cols];

	// initialise cells to a random value (0 or 1)
	for (int i = 0; i < m_rows * m_cols; i++)
	{
		m_grid[i] = rand() % 100 < 10;
		m_gridBuffer[i] = m_grid[i];
	}
}

void Application::Unload()
{
	delete[] m_grid;
	m_grid = nullptr;

	delete[] m_gridBuffer;
	m_gridBuffer = nullptr;
}

void Application::Update(float dt)
{
	// copy "grid" to "gridBuffer"
	memcpy(m_gridBuffer, m_grid, sizeof(int) * m_rows * m_cols);

	// update the grid state for this frame
	for (int i = 0; i < m_rows * m_cols; i++) {
		m_grid[i] = CalculateTileState(i, m_gridBuffer);
	}
}

void Application::Draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	int tileWidth = m_windowWidth / m_cols;
	int tileHeight = m_windowHeight / m_rows;

	// draw horizontal gridlines
	for (int y = 1; y < m_rows; y++)
	{
		DrawLine(0, y * tileHeight, m_windowWidth, y * tileHeight, LIGHTGRAY);
	}

	// draw vertical gridlines
	for (int x = 1; x < m_cols; x++)
	{
		DrawLine(x*tileWidth, 0, x * tileWidth, m_windowHeight, LIGHTGRAY);
	}

	// Draw the "Alive" Tiles
	for (int i = 0; i < m_rows * m_cols; i++)
	{
		if (m_grid[i] == 0)
			continue;

		int x = i % m_cols;
		int y = i / m_cols;

		DrawRectangle(x * tileWidth, y * tileHeight, tileWidth, tileHeight, DARKGRAY);
	}


	EndDrawing();
}

int Application::CalculateTileState(int index, int* grid)
{
	// Grid is a 1D array, but it actually represents a 2D array
	// every row has "m_cols" number of columns.
	// note: you'll find it helpfull to create a lambda helper method
	// that returns the value based on row/column index: ie GetCell(row, col) 
	// - this way you'll be able to more easily get neibouring tile data

	// Calculate the row/col based on index
	// remember how to convert an index value to row/col index and visa versa?
	int col = index % m_cols;
	int row = index / m_cols;

	bool isAlive = grid[index] != 0;
	int numAliveNeibours = 0;

	bool isLeftNeibourAlive = false;
	bool isRightNeibourAlive = false;
	bool isTopNeibourAlive = false;
	bool isDownNeibourAlive = false;
	bool one = false;
	bool two = false;
	bool three = false;
	bool four = false;
	
	if( col - 1 >= 0 )
		isLeftNeibourAlive = grid[row * m_cols + (col - 1)];
	
	if( row - 1 >= 0 )
		isTopNeibourAlive = grid[(row - 1) * m_cols + col];


	if( col + 1 < m_cols)
		isRightNeibourAlive = grid[row * m_cols + (col + 1)];

	if( row + 1 < m_rows)
		isDownNeibourAlive = grid[(row + 1) * m_cols + col];

	if (row + 1 < m_rows)
		one = grid[(row + 1) * m_cols + (col+ 1)];

	if (row + 1 < m_rows)
		two = grid[(row - 1) * m_cols + (col - 1)];

	if (row + 1 < m_rows)
		three = grid[(row - 1) * m_cols + (col + 1)];

	if (row + 1 < m_rows)
		four = grid[(row + 1) * m_cols + (col - 1)];
	

	

	if (isLeftNeibourAlive == true)numAliveNeibours++;

	if (isRightNeibourAlive == true)numAliveNeibours++;

	if (isTopNeibourAlive == true)numAliveNeibours++;

	if (isDownNeibourAlive == true)numAliveNeibours++;

	if (one == true)numAliveNeibours++;

	if (two == true)numAliveNeibours++;

	if (three == true)numAliveNeibours++;

	if (four == true)numAliveNeibours++;

	if (numAliveNeibours < 2)
	{
		isAlive = false;
	}
	else if (isAlive && (numAliveNeibours == 2 || numAliveNeibours == 3))
	{
		isAlive = true;
	}
	else if (numAliveNeibours == 3)
	{
		isAlive = true;
	}
	else if (numAliveNeibours > 3)
	{
		isAlive = false;
	}

		
	

	// Task:
	// implement the rules for Conway?s game of life, the method should
	// update the isAlive value based on the rules defined here:
	// https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life 

	return isAlive;
}
