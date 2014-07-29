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
  bool q = false;
  if(argv[1] != NULL && strcmp(argv[1], "-q") == 0) q=true;
  Display *disp;
  Window win;
  XEvent e;
  int screen;
  int winx = 0;
  int winy = 0;
  int winwidth = 500;
  int winheight = 500;
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

    }
  }
