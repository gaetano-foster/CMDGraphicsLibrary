#ifndef __CMD_GL_H__
#define __CMD_GL_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <Windows.h>

#define SWAP(x, y)       \
	do                   \
	{                    \
		(x) = (x) ^ (y); \
		(y) = (x) ^ (y); \
		(x) = (x) ^ (y); \
	} while (0)
#define VEC2_GENERIC(type) \
	struct {               \
		type x, y;         \
	}

#define FG_INTENSE		FOREGROUND_INTENSITY
#define BG_INTENSE		BACKGROUND_INTENSITY

typedef enum {
	FG_BLACK = 0,
	FG_RED = FOREGROUND_RED,
	FG_GREEN = FOREGROUND_GREEN,
	FG_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	FG_BLUE = FOREGROUND_BLUE,
	FG_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	FG_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	FG_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
} FG_COLOR;

typedef enum {
	BG_BLACK = 0,
	BG_RED = BACKGROUND_RED,
	BG_GREEN = BACKGROUND_GREEN,
	BG_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
	BG_BLUE = BACKGROUND_BLUE,
	BG_MAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
	BG_CYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
	BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
} BG_COLOR;

typedef struct _color {
	FG_COLOR fg;
	BG_COLOR bg;
} CMD_Color;

typedef struct _pixel {
	const wchar_t *sym;
	CMD_Color col;
} CMD_Pixel;

typedef struct _renderer {
	const wchar_t *sym;
	CMD_Color col;
	enum {
		WIREFRAME,
		FILL
	} mode;
	CMD_Pixel *back, *front;  // front and back buffers 
	int SCR_W, SCR_H, SCR_SZ; // Screen width, screen height, screen size 
    HANDLE console;
    INT32 bytes_written;
    
} CMD_Renderer;

typedef VEC2_GENERIC(INT32) CMD_Vec2i;
typedef VEC2_GENERIC(float) CMD_Vec2f;

typedef struct CMD_Rect {
	INT32 x, y;
	INT32 w, h;
} CMD_Rect;

typedef struct CMD_Tri {
	CMD_Vec2i points[3];
} CMD_Tri;

typedef struct CMD_Circle {
	INT32 x, y;
	INT32 rad;
} CMD_Circle;

void CMD_ClearScreen(CMD_Renderer *renderer);
CMD_Renderer* CMD_RendererMakeNew(INT32 width, INT32 height);
void CMD_RendererDestroy(CMD_Renderer *renderer);
void CMD_RendererPresent(CMD_Renderer *renderer);
void CMD_SetRenderPixel(CMD_Renderer *renderer, const wchar_t *sym, FG_COLOR fg, BG_COLOR bg);
void CMD_RenderDrawPoint(CMD_Renderer *renderer, INT32 x, INT32 y);
void CMD_RenderDrawLine(CMD_Renderer *renderer, INT32 x1, INT32 y1, INT32 x2, INT32 y2);
void CMD_RenderDrawRect(CMD_Renderer *renderer, CMD_Rect rect);
void CMD_RenderDrawTri(CMD_Renderer *renderer, CMD_Tri tri);
void CMD_RenderDrawCircle(CMD_Renderer *renderer, CMD_Circle cir);

#ifdef __cplusplus
}
#endif
#endif // __CMD_GL_H__