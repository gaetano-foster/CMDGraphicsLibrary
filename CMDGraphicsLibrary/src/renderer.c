#include <CMD_GL.h>

static void 
__cmdglswp(CMD_Pixel **buf1, 
           CMD_Pixel **buf2) 
{
	CMD_Pixel *temp = *buf1;
	*buf1 = *buf2;
	*buf2 = temp;
}

CMD_Renderer* 
CMD_RendererMakeNew(INT32 width, 
                    INT32 height) 
{
    CMD_Renderer *renderer = malloc(sizeof(CMD_Renderer));
    renderer->mode = FILL;
    renderer->sym = L"#";
    renderer->col = (CMD_Color) { 0, 0 };
    renderer->SCR_W = width;
    renderer->SCR_H = height;
    renderer->SCR_SZ = width * height;
    renderer->back = calloc(renderer->SCR_SZ, sizeof(CMD_Pixel));
    renderer->front = calloc(renderer->SCR_SZ, sizeof(CMD_Pixel));
    renderer->console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(renderer->console);
    renderer->bytes_written = 0;
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(renderer->console, &info);
    return renderer;
}

void 
CMD_RendererDestroy(CMD_Renderer *renderer) 
{
    free(renderer->back);
    free(renderer->front);
    free(renderer);
}

void 
CMD_RendererPresent(CMD_Renderer *renderer) 
{
    for (int y = 0; y < renderer->SCR_H; y++) {
        for (int x = 0; x < renderer->SCR_W; x++) {
            WORD color = renderer->front[y*renderer->SCR_W+x].col.bg | renderer->front[y*renderer->SCR_W+x].col.fg;
            WriteConsoleOutputAttribute(renderer->console, &color, 1, (COORD) { x, y }, &(renderer->bytes_written));
            WriteConsoleOutputCharacterW(renderer->console, renderer->front[y*renderer->SCR_W+x].sym, 1, (COORD) { x, y }, &(renderer->bytes_written));
        }
    }
    
    __cmdglswp(&(renderer->back), &(renderer->front));
}

void 
CMD_SetRenderPixel(CMD_Renderer *renderer, 
                   const wchar_t *sym, 
                   FG_COLOR fg, 
                   BG_COLOR bg) 
{
    if (sym[0] != '\0' && sym[1] == '\0')
        renderer->sym = sym;

    renderer->col.fg = fg;
    renderer->col.bg = bg;
}

void 
CMD_ClearScreen(CMD_Renderer *renderer) 
{
    for (int i = 0; i < renderer->SCR_SZ; i++) {
		renderer->back[i] = (CMD_Pixel) { 0, renderer->col };
    }
}