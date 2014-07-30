#include <X11/Xlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
// #includ ur #life

bool q = false;
int winwidth = 1290;
int winheight = 910;
int sliderpos = 0;
int wscale = 100; // 1m
Display *disp;
Window win;
int screen;

GC GC_color(char color[])
{
  GC a;
  XColor b;
  Colormap c;
  c = DefaultColormap(disp, 0);
  a = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, c, color, &b);
  XAllocColor(disp, c, &b);
  XSetForeground(disp, a, b.pixel);
  return a;
}


void slider(int left, int top, int width, int height)
{
  if(sliderpos < 0) sliderpos = 0;
  if(sliderpos > 100) sliderpos = 100;
  int rel = winwidth / wscale;
  int rely = winheight / wscale;
  int absheight = height*rely;
  int abswidth = width*rel;
  int absleft = left*rel;
  int abstop = top*rely;
  if(!q) printf("Rel: %d, relY: %d, height: %d, width: %d, left: %d, top: %d, sliderpos: %d\n",rel,rely , absheight, abswidth, absleft, abstop, sliderpos);
  XFillRectangle(disp, win, GC_color("#FF0000"), absleft, abstop-2, abswidth, absheight+9);
  float htop = abstop+(absheight/100.0)*sliderpos;
  int hleft = absleft+2;
  if(!q) printf("Htop: %f hleft: %d\n",htop,hleft);
  XFillRectangle(disp, win, GC_color("#00FF00"), hleft, htop, abswidth-4, 5);
}

void sbutl(int x, int y, int but, int left, int top, int width, int height)
{
  if(sliderpos < 0) sliderpos = 0;
  if(sliderpos > 100) sliderpos = 100;
  int rel = winwidth / wscale;
  int rely = winheight / wscale;
  int absheight = (height*rely)+9;
  int abswidth = width*rel;
  int absleft = left*rel;
  int abstop = top*rely;
  //printf("%d<%d && %d < %d && %d > %d && %d > %d\n",x,absleft+abswidth,y,abstop+absheight,x,absleft,y,abstop);
  //printf("height: %d, width: %d, left: %d, top: %d\n",absheight, abswidth, absleft, abstop);
  if(x < absleft+abswidth && y < abstop+absheight && x > absleft && y > abstop)
    {
      if(!q) printf("You've scrolled on the slider!\n");
      if(but == 4) sliderpos--;
      if(but == 5) sliderpos++;
      slider(left,top,width,height);
    }
}

int main(int argc, char *argv[]) {
  if(argv[1] != NULL && strcmp(argv[1], "-q") == 0) q=true;
  XEvent e;
  int winx = 0;
  int winy = 0;
  int borderwidth = 1;
  disp = XOpenDisplay(NULL);
  if (disp == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, RootWindow(disp, screen), winx, winy, winwidth, winheight, borderwidth,
  BlackPixel(disp, screen), WhitePixel(disp, screen));
  XSelectInput(disp, win, ExposureMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ResizeRedirectMask);
  // Window XCreateWindow(display, parent, x, y, width, height, border_width, depth,
  //                    class, visual, valuemask, attributes)
  XMapWindow(disp, win);
  while (1)
    {
      XNextEvent(disp, &e);
      if (e.type == KeyPress)
        {
          if(!q) printf("Keycode: %d\n", e.xkey.keycode);
          unsigned long keysym = XLookupKeysym(&e.xkey, 0);
          if(!q) printf("Keysym: %lu\n",keysym);
          char *ascii = XKeysymToString(keysym);
          if(!q) printf("ASCII: %s\n",ascii);
          if(keysym == 65307)
            {
              printf("Exiting...\n");
              XCloseDisplay(disp);
              return 0;
            }
          }
          if(e.type == 4) // button press
            {
              int x=e.xbutton.x;
              int y=e.xbutton.y;
              int button = e.xbutton.button;
              if(!q) printf("Button pressed. X: %d, Y: %d Button: %d\n",x,y,button);
              if(button == 5) sbutl(x,y,5,95,1,5,40);
              if(button == 4) sbutl(x,y,4,95,1,5,40);
            }
            if(e.type == 25) // resize
              {
                int width=e.xresizerequest.width;
                int height=e.xresizerequest.height;
                if(!q) printf("Window resize. Width: %d, Height: %d\n",width,height);
                winheight=height;
                winwidth=width;
                slider(95,1,5,40);
              }
            if(e.type == 6) // mouse move
              {
                int x=e.xmotion.x;
                int y=e.xmotion.y;
                if(!q) printf("Mouse moved. X: %d, Y: %d\n",x,y);
              }
      if (e.type == Expose)
        {
          XFillRectangle(disp, win, GC_color("#FFFFFF"), 0, 0, winwidth, winheight);
          slider(95,1,5,40);
        }
    }
  }
