#include <math.h>
#include <CMD_GL.h>

static void 
__cmdhline(CMD_Renderer *renderer, 
			int x1, 
			int x2, 
			int y) 
{
    if (x1 >= x2) 
        SWAP(x1, x2);

	for (; x1 <= x2 ; x1++) 
        CMD_RenderDrawPoint(renderer, x1, y);
}

void 
CMD_RenderDrawPoint(CMD_Renderer *renderer, 
					INT32 x, 
					INT32 y) 
{
    renderer->back[y*renderer->SCR_W+x] = (CMD_Pixel) { renderer->sym, renderer->col };
}

void 
CMD_RenderDrawLine(CMD_Renderer *renderer, 
					INT32 x1, 
					INT32 y1, 
					INT32 x2, 
					INT32 y2) 
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; 
    dy = y2 - y1;
	dx1 = dx >= 0 ? dx : -dx; 
    dy1 = dy >= 0 ? dy : -dy;
	px = 2 * dy1 - dx1;	
    py = 2 * dx1 - dy1;

	if (dy1 <= dx1){
		if (dx >= 0) { 
            x = x1; 
            y = y1; 
            xe = x2; 
        }
		else { 
            x = x2; 
            y = y2; 
            xe = x1; 
        }
		CMD_RenderDrawPoint(renderer, x, y);
		
		for (i = 0; x<xe; i++) {
			x = x + 1;
			if (px<0)
				px = px + 2 * dy1;
			else {
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) 
                    y = y + 1; 
                else 
                    y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			CMD_RenderDrawPoint(renderer, x, y);
		}
	}
	else {
		if (dy >= 0) { 
            x = x1; 
            y = y1; 
            ye = y2; 
        }
		else { 
            x = x2; 
            y = y2; 
            ye = y1; 
        }

		CMD_RenderDrawPoint(renderer, x, y);

		for (i = 0; y<ye; i++) {
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else {
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			CMD_RenderDrawPoint(renderer, x, y);
		}
	}
}

void 
CMD_RenderDrawRect(CMD_Renderer *renderer, 
				   CMD_Rect rect) 
{
    if (renderer->mode == WIREFRAME) {
        CMD_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
        CMD_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
        CMD_RenderDrawLine(renderer, rect.x + rect.w, rect.y + rect.h, rect.x, rect.y + rect.h);
        CMD_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y);
    }
    else if (renderer->mode == FILL) {
        for (int y = rect.y; y < rect.y + rect.h; y++) {
            for (int x = rect.x; x < rect.x + rect.w; x++) {
                CMD_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void 
CMD_RenderDrawTri(CMD_Renderer *renderer, 
				  CMD_Tri tri) 
{
    if (renderer->mode == WIREFRAME) {
        CMD_RenderDrawLine(renderer, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y);
        CMD_RenderDrawLine(renderer, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y);
        CMD_RenderDrawLine(renderer, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y);
    }
    else if (renderer->mode == FILL) { 
        int x1 = tri.points[0].x, y1 = tri.points[0].y,
            x2 = tri.points[1].x, y2 = tri.points[1].y,
            x3 = tri.points[2].x, y3 = tri.points[2].y;

        int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	    int changed1 = 0;
	    int changed2 = 0;
	    int signx1, signx2, dx1, dy1, dx2, dy2;
	    int e1, e2;

        // Sort vertices
	    if (y1 > y2) { SWAP(y1,y2); SWAP(x1,x2); }
	    if (y1 > y3) { SWAP(y1,y3); SWAP(x1,x3); }
	    if (y2 > y3) { SWAP(y2,y3); SWAP(x2,x3); }

	    t1x = t2x = x1; 
        y = y1;   // Starting points

	    dx1 = (x2 - x1); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	    dy1 = (y2 - y1);
    
	    dx2 = (x3 - x1); if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	    dy2 = (y3 - y1);
    
	    if (dy1 > dx1) {   // swap values
            SWAP(dx1,dy1);
	    	changed1 = 1;
	    }
	    if (dy2 > dx2) {   // swap values
            SWAP(dy2,dx2);
	    	changed2 = 1;
	    }
    
	    e2 = (dx2>>1);
        // Flat top, just process the second half
        if(y1==y2) goto next;
        e1 = (dx1>>1);
    
	    for (int i = 0; i < dx1;) {
	    	t1xp=0; t2xp=0;
	    	if(t1x<t2x) { minx=t1x; maxx=t2x; }
	    	else		{ minx=t2x; maxx=t1x; }
            // process first line until y value is about to change
	    	while(i<dx1) {
	    		i++;			
	    		e1 += dy1;
	       	   	while (e1 >= dx1) {
	    			e1 -= dx1;
   	       	   	   if (changed1) t1xp=signx1;//t1x += signx1;
	    			else          goto next1;
	    		}
	    		if (changed1) break;
	    		else t1x += signx1;
	    	}
	    // Move line
	    next1:
            // process second line until y value is about to change
	    	while (1) {
	    		e2 += dy2;		
	    		while (e2 >= dx2) {
	    			e2 -= dx2;
	    			if (changed2) t2xp=signx2;//t2x += signx2;
	    			else          goto next2;
	    		}
	    		if (changed2)     break;
	    		else              t2x += signx2;
	    	}
	    next2:
	    	if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
	    	if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	       	__cmdhline(renderer, minx, maxx, y);    // Draw line from min to max points found on the y
	    	// Now increase y
	    	if(!changed1) t1x += signx1;
	    	t1x+=t1xp;
	    	if(!changed2) t2x += signx2;
	    	t2x+=t2xp;
        	y += 1;
	    	if(y==y2) break;
    
   }    
	    next:
	    // Second half
	    dx1 = (x3 - x2); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	    dy1 = (y3 - y2);
	    t1x=x2;
    
	    if (dy1 > dx1) {   // swap values
            SWAP(dy1,dx1);
	    	changed1 = 1;
	    } else changed1 = 0;
    
	    e1 = (dx1>>1);
    
	    for (int i = 0; i<=dx1; i++) {
	    	t1xp=0; t2xp=0;
	    	if(t1x<t2x) { minx=t1x; maxx=t2x; }
	    	else		{ minx=t2x; maxx=t1x; }
	        // process first line until y value is about to change
	    	while(i<dx1) {
        		e1 += dy1;
	       	   	while (e1 >= dx1) {
	    			e1 -= dx1;
   	       	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
	    			else          goto next3;
	    		}
	    		if (changed1) break;
	    		else   	   	  t1x += signx1;
	    		if(i<dx1) i++;
	    	}
	    next3:
            // process second line until y value is about to change
	    	while (t2x!=x3) {
	    		e2 += dy2;
	       	   	while (e2 >= dx2) {
	    			e2 -= dx2;
	    			if(changed2) t2xp=signx2;
	    			else          goto next4;
	    		}
	    		if (changed2)     break;
	    		else              t2x += signx2;
	    	}	   	   
	    next4:

	    	if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
	    	if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	       	__cmdhline(renderer, minx, maxx, y);    // Draw line from min to max points found on the y
	    	// Now increase y
	    	if(!changed1) t1x += signx1;
	    	t1x+=t1xp;
	    	if(!changed2) t2x += signx2;
	    	t2x+=t2xp;
        	y += 1;
	    	if(y>y3) return;
	    }
    }
}

void 
CMD_RenderDrawCircle(CMD_Renderer *renderer, 
					 CMD_Circle cir) 
{
    int xc = cir.x, yc = cir.y, r = cir.rad;
    if (renderer->mode == WIREFRAME) {
        int x = 0;
	    int y = r;
	    int p = 3 - 2 * r;
	    if (!r) return;

	    while (y >= x) { // only formulate 1/8 of circle
	    	CMD_RenderDrawPoint(renderer, xc - x, yc - y);//upper left left
	    	CMD_RenderDrawPoint(renderer, xc - y, yc - x);//upper upper left
	    	CMD_RenderDrawPoint(renderer, xc + y, yc - x);//upper upper right
	    	CMD_RenderDrawPoint(renderer, xc + x, yc - y);//upper right right
	    	CMD_RenderDrawPoint(renderer, xc - x, yc + y);//lower left left
	    	CMD_RenderDrawPoint(renderer, xc - y, yc + x);//lower lower left
	    	CMD_RenderDrawPoint(renderer, xc + y, yc + x);//lower lower right
	    	CMD_RenderDrawPoint(renderer, xc + x, yc + y);//lower right right
	    	if (p < 0) p += 4 * x++ + 6;
	    	else p += 4 * (x++ - y--) + 10;
	    }
    }
    else if (renderer->mode == FILL) {
        int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) {
			__cmdhline(renderer, xc - x, xc + x, yc - y);
			__cmdhline(renderer, xc - y, xc + y, yc - x);
			__cmdhline(renderer, xc - x, xc + x, yc + y);
			__cmdhline(renderer, xc - y, xc + y, yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
    }
}