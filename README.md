# TerminalGraphicsLibrary

A wrapper around the windows api which allows you to draw to the command line. It's basically a 1:1 port of my [TerminalGraphicsLibrary](https://github.com/gaetano-foster/TerminalGraphicsLibrary), save for some small changes.

## How to use

1. Compile it into a static library however you want, using any compiler that works.
2. Go to the [Releases](https://github.com/gaetano-foster/CMDGraphicsLibrary/releases) page and download a precompiled library.

## Warning

This library is in its beta stage and is still being tested and fixed. There are some minor bugs, but it is mostly functional.

## Sample Code
Here is how the code would be used. This program renders a triangle to the command line in an infinite loop.

```
#include "CMD_GL.h"

int main(int argc, char **argv) {
	CMD_Renderer *renderer = CMD_RendererMakeNew(120, 40);
	CMD_SetRenderPixel(renderer, L"@", FG_MAGENTA, BG_CYAN | BG_INTENSE);
	renderer->mode = FILL;
	system("cls");
	while (1) {
		CMD_clrscr(renderer);
		CMD_RenderDrawTri(renderer, (CMD_Tri) { (CMD_Vec2i) { 10, 5 }, (CMD_Vec2i) { 10, 10 }, (CMD_Vec2i) { 5, 5 } });
		CMD_RendererPresent(renderer);
	}
	CMD_RendererDestroy(renderer);
}
```
