#include <X11/Xlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
// #includ ur #life

bool q = false;
int winwidth = 1290;
int winheight = 910;
int sliderpos = 1;

GC GC_color(Display *disp, Window win, char color[])
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

void slider(Display *disp, Window win, int screen, int height, int width, int wscale, int left, int top)
{
  if(sliderpos < 0) sliderpos = 0;
  if(sliderpos > 100) sliderpos = 100;
  int relx = winwidth / wscale;
  int rely = winwidth / wscale;
  int absheight = height*rel;
  int abswidth = width*rel;
  int absleft = left*rel;
  int abstop = top*rel;
  if(!q) printf("Rel: %d height: %d, width: %d, left: %d, top: %d, sliderpos: %d\n",rel, absheight, abswidth, absleft, abstop, sliderpos);
  XFillRectangle(disp, win, GC_color(disp,win,"#FF0000"), absleft, abstop-2, abswidth, absheight+9);
  float htop = abstop+(absheight/100.0)*sliderpos;
  int hleft = absleft+2;
  if(!q) printf("Htop: %f hleft: %d\n",htop,hleft);
  XFillRectangle(disp, win, GC_color(disp,win,"#00FF00"), hleft, htop, abswidth-4, 5);
}

int main(int argc, char *argv[]) {
  if(argv[1] != NULL && strcmp(argv[1], "-q") == 0) q=true;
  Display *disp;
  Window win;
  XEvent e;
  int screen;
  int winx = 0;
  int winy = 0;
  int borderwidth = 1;
  int wscale = 100; // 1m
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
              if(button == 4) sliderpos++; slider(disp,win,screen, 30,4,wscale,80,12);
              if(button == 5) sliderpos--; slider(disp,win,screen, 30,4,wscale,80,12);
            }
            if(e.type == 25) // resize
              {
                int width=e.xresizerequest.width;
                int height=e.xresizerequest.height;
                if(!q) printf("Window resize. Width: %d, Height: %d\n",width,height);
                winheight=height;
                winwidth=width;
                slider(disp,win,screen, 30,4,wscale,80,12);
              }
            if(e.type == 6) // mouse move
              {
                int x=e.xmotion.x;
                int y=e.xmotion.y;
                if(!q) printf("Mouse moved. X: %d, Y: %d\n",x,y);
              }
      if (e.type == Expose)
        {
          XFillRectangle(disp, win, GC_color(disp,win,"#FFFFFF"), 0, 0, winwidth, winheight);
          slider(disp,win,screen, 30,4,wscale,80,12);
        }
    }
  }
