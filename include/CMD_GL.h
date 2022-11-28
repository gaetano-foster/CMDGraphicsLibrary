#ifndef __CMD_GL_H__
#define __CMD_GL_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "CMD_GL_Types.h"
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
    // WinAPI stuff 
    HANDLE console;
    DWORD bytes_written;
    
} CMD_Renderer;

typedef VEC2_GENERIC(i32) CMD_Vec2i;
typedef VEC2_GENERIC(float) CMD_Vec2f;

typedef struct _rect {
	i32 x, y;
	i32 w, h;
} CMD_Rect;

typedef struct _tri {
	CMD_Vec2i points[3];
} CMD_Tri;

typedef struct _cir {
	i32 x, y;
	i32 rad;
} CMD_Circle;

void CMD_clrscr(CMD_Renderer *renderer);
CMD_Renderer* CMD_RendererMakeNew(i32 width, i32 height);
void CMD_RendererDestroy(CMD_Renderer *renderer);
void CMD_RendererPresent(CMD_Renderer *renderer);
void CMD_SetRenderPixel(CMD_Renderer *renderer, const wchar_t *sym, FG_COLOR fg, BG_COLOR bg);
void CMD_RenderDrawPoint(CMD_Renderer *renderer, i32 x, i32 y);
void CMD_RenderDrawLine(CMD_Renderer *renderer, i32 x1, i32 y1, i32 x2, i32 y2);
void CMD_RenderDrawRect(CMD_Renderer *renderer, CMD_Rect rect);
void CMD_RenderDrawTri(CMD_Renderer *renderer, CMD_Tri tri);
void CMD_RenderDrawCircle(CMD_Renderer *renderer, CMD_Circle cir);

#ifdef __cplusplus
}
#endif
#endif // __CMD_GL_H__