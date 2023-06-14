#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define TILE 10
#define IX(i,j)(i+j*100)
#define RED '0'
#define GREEN '1'
#define BLUE '2'

int screen[100 * 100];
int u;

int Count(int x, int y, int who)
{
	int count = 0;
	int i, j, col, row;
	int haslife, self;
	for (i = -1; i < 2; i++)
		for (j = -1; j < 2; j++)
		{
			col = (x + i + 100) % 100;
			row = (y + i + 100) % 100;
			self = (col == x && row == y);
			if (screen[IX(col, row)] == who)
				haslife = 1;
			else
				haslife = 0;

			if (haslife && !self)
				count++;
		}
	return count;
}

void clrscr(void)
{
	int i, j, o;
	u++;
	srand(u);
	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)
		{
			o = rand() % 3;
			switch (o)
			{
				case 0: screen[IX(i, j)] = RED; break;
				case 1: screen[IX(i, j)] = GREEN; break;
				case 2: screen[IX(i, j)] = BLUE; break;

			}
		}
}


void newscreen(void)
{
	char screen2[100 * 100];
	int i, j;
	int haslife, neighboursCount, r = 0;
	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++) 
		{
			r = 0;
			neighboursCount = Count(i, j, GREEN);
			if (screen2[IX(i, j)] == RED && neighboursCount >= 3) 
			{
				screen2[IX(i, j)] = GREEN;
				r = 1;
			}
			neighboursCount = Count(i, j, BLUE);
			if (screen2[IX(i, j)] == GREEN && neighboursCount >= 3)
			{
				screen2[IX(i, j)] = BLUE;
				r = 1;
			}
			neighboursCount = Count(i, j, RED);
			if (screen2[IX(i, j)] == BLUE && neighboursCount >= 3)
			{
				screen2[IX(i, j)] = RED;
				r = 1;
			}
			if(!r) 
				screen2[IX(i, j)] = screen[IX(i, j)];
		}
	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)			
			screen[IX(i, j)] = screen2[IX(i, j)];
}


void render(HDC dc) 
{
	int red[3] = { 255, 0, 0 };
	int green[3] = { 0, 255, 0 };
	int blue[3] = { 0, 0, 255 };

	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP memBM = CreateCompatibleBitmap(dc, 100*TILE, 100*TILE);
	SelectObject(memDC, memBM);
	int i, j;
	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++) 
		{

			SelectObject(memDC, GetStockObject(DC_BRUSH));
			switch (screen[IX(i,j)])
			{
				case RED: {
					SetDCBrushColor(memDC, RGB(red[0], red[1], red[2]));
					SelectObject(memDC, GetStockObject(DC_PEN));
					SetDCPenColor(memDC, RGB(red[0], red[1], red[2]));

				}break;
				case BLUE: {
					SetDCBrushColor(memDC, RGB(green[0], green[1], green[2]));
					SelectObject(memDC, GetStockObject(DC_PEN));
					SetDCPenColor(memDC, RGB(green[0], green[1], green[2]));

				}break;
				case GREEN: {
					SetDCBrushColor(memDC, RGB(blue[0], blue[1], blue[2]));
					SelectObject(memDC, GetStockObject(DC_PEN));
					SetDCPenColor(memDC, RGB(blue[0], blue[1], blue[2]));

				}break;
			}


			
			Rectangle(memDC, i * TILE, j * TILE, i * TILE + TILE, j * TILE + TILE);
		}
	BitBlt(dc, 0, 0, 100 * TILE, 100 * TILE, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
	DeleteObject(memBM);
}

LRESULT Mydef(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProcA(hwnd, message, wparam, lparam);
}

int main(void) {
	WNDCLASSA wcl;
	memset(&wcl, 0, sizeof(WNDCLASSA));

	wcl.lpszClassName = "win1";
	wcl.lpfnWndProc = Mydef;

	RegisterClassA(&wcl);

	
	HWND hwnd;
	hwnd = CreateWindowA("win1", "Name", WS_OVERLAPPEDWINDOW, 10, 10, 100*TILE, 100*TILE, NULL, NULL, NULL, NULL);
	ShowWindow(hwnd, SW_SHOWNORMAL);

	HDC dc = GetDC(hwnd);

	MSG msg;
	clrscr();
	while (1)
	{
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}		
		newscreen();
		render(dc);
	}
}