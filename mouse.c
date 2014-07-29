#include <X11/Xlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
// #includ ur #life
int main(int argc, char *argv[]) {
  bool q = true;
  if(argv[1] != NULL && strcmp(argv[1], "-v") == 0) q=false;
  Display *disp;
  Window win;
  XEvent e;
  int screen;
  int winy = 0;
  int winx = 0;
  int winheight = 300;
  int winwidth = 500;
  int borderwidth = 10;
  GC blue;
  XColor blue_color;
  Colormap colormap;
  char blueidk[] = "#0000FF";
  GC red;
  XColor red_color;
  Colormap rcolormap;
  char redidk[] = "#FF0000";
  GC white;
  XColor white_color;
  Colormap wcolormap;
  char whiteidk[] = "#FFFFFF";

  disp = XOpenDisplay(NULL);
  if (disp == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, RootWindow(disp, screen), winx, winy, winwidth, winheight, borderwidth,
  BlackPixel(disp, screen), WhitePixel(disp, screen));
  XSelectInput(disp, win, ExposureMask | KeyPressMask | PointerMotionMask | ButtonPressMask);
  // Window XCreateWindow(display, parent, x, y, width, height, border_width, depth,
  //                    class, visual, valuemask, attributes)
  XMapWindow(disp, win);
  colormap = DefaultColormap(disp, 0);
  blue = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, colormap, blueidk, &blue_color);
  XAllocColor(disp, colormap, &blue_color);
  XSetForeground(disp, blue, blue_color.pixel);
  rcolormap = DefaultColormap(disp, 0);
  red = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, colormap, redidk, &red_color);
  XAllocColor(disp, colormap, &red_color);
  XSetForeground(disp, red, red_color.pixel);
  wcolormap = DefaultColormap(disp, 0);
  white = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, wcolormap, whiteidk, &white_color);
  XAllocColor(disp, wcolormap, &white_color);
  XSetForeground(disp, white, white_color.pixel);
  char *msg = "Press me!";
  while (1) {
    XNextEvent(disp, &e);
        if (e.type == Expose)
        {
          XFillRectangle(disp, win, red, 20, 20, 60, 15);
          XDrawString(disp, win, white, 22, 32, msg, strlen(msg));
    //http://tronche.com/gui/x/xlib/graphics/drawing-text/XDrawString.html
        }
    if(e.type == 6) // mouse move
      {
        int x=e.xmotion.x;
        int y=e.xmotion.y;
        if(!q) printf("Mouse moved. X: %d, Y: %d\n",x,y);
        XFillRectangle(disp, win, blue, x, y, 3, 3);
      }
      if(e.type == 4) // mouse move
        {
          int x=e.xbutton.x;
          int y=e.xbutton.y;
          printf("Button pressed. X: %d, Y: %d\n",x,y);
          XFillRectangle(disp, win, DefaultGC(disp, screen), x, y, 10, 10);
        }
    if (e.type == KeyPress)
      {
        if(!q) printf("Keycode: %d\n", e.xkey.keycode);
        unsigned long keysym = XLookupKeysym(&e.xkey, 0);
        printf("Keysym: %lu\n",keysym);
        char *ascii = XKeysymToString(keysym);
        if(!q) printf("ASCII: %s\n",ascii);
        if(keysym == 65307) // ESC
          {
            printf("Exiting...\n");
            return 0;
          }
        }
      }
      XCloseDisplay(disp);
      return 0;
    }
