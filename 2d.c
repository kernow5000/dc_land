/*
 *  simple 2d stuff
 *  kernow 7/2002
*/


#include <kos.h>
#include <dc_land/2d.h>


void v_init()
  // maybe an int for res?
{
   int x,y;
   // x = res_x;
   // y = res_y;
   // switch(res_x) case 640 switch to 640
   vid_set_mode(DM_640x480, PM_RGB555);
   x = 640;
   y = 480;
}

void v_clear(int r, int g, int b)
{
   vid_clear(r,g,b);
}


void v_pixel(int x, int y, int r, int g, int b)
{
   // plot directly to vram
   vram_s[x + (640 * y)] = RGB565(r,g,b);
}



void v_circle(int x, int y, int radius, int r, int g, int b)
{
   int balance, xoff, yoff;
   
   xoff = 0;
   yoff = radius;
   balance=-radius;
   
   do {
      v_pixel(x+xoff, y+yoff, r,g,b);
      v_pixel(x-xoff, y+yoff, r,g,b);
      v_pixel(x-xoff, y-yoff, r,g,b);
      v_pixel(x+xoff, y-yoff, r,g,b);
      
      v_pixel(x+yoff, y+xoff, r,g,b);
      v_pixel(x-yoff, y+xoff, r,g,b);
      v_pixel(x-yoff, y-xoff, r,g,b);
      v_pixel(x+yoff, y-xoff, r,g,b);
      
      if ((balance+= xoff++ + xoff) >=0) {
	 --yoff;
	 balance-= yoff + yoff;
      }
   }
   while (xoff <= yoff);
   
}


void v_drawstring(int x, int y, char *s)
{
   // take opaque out for simplicity?..done
   bfont_draw_str( (vram_s+y * 640)+x, 640, 0, s);
}


void v_drawstring_ctr(int x, int y, char *text)
{
  int length;
  
  length = strlen(text); // find out length
  length = length * 13; // include padding
  x-= length / 2;
  v_drawstring(x,y,text); // call normal
}



void v_fade() {
  
  int i;
  
  // funKy faDe
  
  for (i = 200; i>=0; i-=10) {
    vid_clear(i,i,i);
    vid_waitvbl();
  }

}



/* bresenham line drawing function
 * ported from delphi src
 * kernow
 */
void v_line(int Ax, int Ay, int Bx, int By, int r, int g, int b)
{
  
  int dX = abs(Bx-Ax);    // store the change in X and Y of the line endpoints
  int dY = abs(By-Ay);
  
  int Xincr, Yincr;
  if (Ax > Bx) { Xincr=-1; } else { Xincr=1; }    // which direction in X?
  if (Ay > By) { Yincr=-1; } else { Yincr=1; }    // which direction in Y?
  
  if (dX >= dY)   // if X is the independent variable
    {           
      int dPr         = dY<<1;
      int dPru        = dPr - (dX<<1);  
      int P           = dPr - dX;  
      
      for (; dX>=0; dX--) {
	v_pixel(Ax, Ay, r,g,b); // plot the pixel
	if (P > 0) { 
	  Ax+=Xincr;     
	  Ay+=Yincr;     
	  P+=dPru;         
	} else {
	  Ax+=Xincr;
	  P+=dPr;
	}
      }               
    } else {
      int dPr         = dX<<1;           // amount to increment decision if right is chosen (always)
      int dPru        = dPr - (dY<<1);   // amount to increment decision if up is chosen
      int P           = dPr - dY;  // decision variable start value
      
      for (; dY>=0; dY--) {
	v_pixel(Ax, Ay, r,g,b); // plot the pixel
	if (P > 0) { 
	  Ax+=Xincr;         // increment dependent variable
	  Ay+=Yincr;         // increment independent variable
	  P+=dPru;           // increment decision (for up)
	} else {
	  Ay+=Yincr;         // increment independent variable
	  P+=dPr;            // increment decision (for right)
	}
      }               
    }               
}

void v_box(int x, int y, int sx, int sy, int r, int g, int b)
{
  v_line (x, y, x + sx, y, r,g,b);
  v_line (x + sx, y, x + sx, y + sy, r,g,b);
  v_line (x + sx, y + sy, x, y + sy,r,g,b);
  v_line (x, y + sy, x, y,r,g,b);
}


uint32 getPixel (int x, int y) {
  uint32 c;
  
  c = vram_s [x + (640 * y)];

  return c;
}

void fillToBorder(int x, int y, int border, int colour)
{
	int lastBorder;

	/* Seek left */
	int leftLimit, rightLimit;
	int i;
	
	leftLimit = (-1);
	if (border < 0) {
	  /* Refuse to fill to a non-solid border */
	  return;
	}
	
	for (i = x; (i >= 0); i--) {
	  if (getPixel(i, y) == border) {
	    break;
	  }
	  v_pixel(i, y, 0, 255, 0);
	  leftLimit = i;
	}
	if (leftLimit == (-1)) {
	  return;
	}
	
	
	/* Seek right */
	rightLimit = x;
	for (i = (x+1); (i < 640); i++) {	
	  if (getPixel(i, y) == border) {
			break;
	  }
	  v_pixel(i, y, 0, 255, 0);
	  rightLimit = i;
	}
	
	
	
	/* Look at lines above and below and start paints */
	
	/* Above */
	if (y > 0) {
	  lastBorder = 1;
	  for (i = leftLimit; (i <= rightLimit); i++) {
	    int c;
			c = getPixel(i, y-1);
			if (lastBorder) {
			  if ((c != border) && (c != colour)) {	
			    fillToBorder(i, y-1, border, colour);		
			    lastBorder = 0;
			  }
			} else if ((c == border) || (c == colour)) {
			  lastBorder = 1;
			}
	  }
	}

	
	/* Below */
	if (y < 480) {
	  lastBorder = 1;
		for (i = leftLimit; (i <= rightLimit); i++) {
		  int c;
		  c = getPixel(i, y+1);
		  if (lastBorder) {
		    if ((c != border) && (c != colour)) {	
		      fillToBorder(i, y+1, border, colour);		
		      lastBorder = 0;
		    }
			} else if ((c == border) || (c == colour)) {
			  lastBorder = 1;
			}
		}
	}
}




void v_clr(int xpos, int ypos, int nx, int ny, int r, int g, int b)
{
  // really bad clearing function
  int j;
  
  for (j = ypos; j < ny; j++)
    v_line(xpos,j,nx,j, r,g,b);
}
