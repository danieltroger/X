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
  int winy = 0;
  int winx = 0;
  int winheight = 910;
  int winwidth = 1285;
  int borderwidth = 1;
  char *msg = "Clear screen";
  int bwidth=strlen(msg)*6;
  int bheight=15;
  int btop=20;
  int bmargin = 2;
  int bleft=winwidth - bwidth - 20;
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
  XSelectInput(disp, win, ExposureMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ResizeRedirectMask);
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
  while (1) {
    XNextEvent(disp, &e);
    if (e.type == Expose)
      {
        if(!q) printf("Clearing & repainting window...\n");
        XFillRectangle(disp, win, white, 0, 0, winwidth, winheight); // clear the screen
        XFillRectangle(disp, win, red, bleft, btop, bwidth, bheight); // print the button
        XDrawString(disp, win, white, bleft+bmargin, btop+bmargin+9, msg, strlen(msg)); // button text
        //http://tronche.com/gui/x/xlib/graphics/drawing-text/XDrawString.html
      }
      if(e.type == 6) // mouse move
        {
          int x=e.xmotion.x;
          int y=e.xmotion.y;
          if(!q) printf("Mouse moved. X: %d, Y: %d\n",x,y);
          XFillRectangle(disp, win, blue, x, y, 10,10);
        }
        if(e.type == 25) // resize
          {
            int width=e.xresizerequest.width;
            int height=e.xresizerequest.height;
            if(!q) printf("Window resize. Width: %d, Height: %d\n",width,height);
            winheight=height;
            winwidth=width;
            bleft=winwidth - bwidth - 20;
            // should've done a function but c&p is easier
            XFillRectangle(disp, win, red, bleft, btop, bwidth, bheight);
            XDrawString(disp, win, white, bleft+bmargin, btop+bmargin+9, msg, strlen(msg));
          }
        if(e.type == 4) // button press
          {
            int x=e.xbutton.x;
            int y=e.xbutton.y;
            int button = e.xbutton.button; // button type 1 is left click 3 right
            if(!q) printf("Button pressed. X: %d, Y: %d Button: %d\n",x,y,button);
            if(button == 1) // left click
              {
                XFillRectangle(disp, win, DefaultGC(disp, screen), x, y, 20, 20);
                if(y < bheight+btop && x < bwidth+bleft)
                  {
                    if(y > btop && x > bleft)
                      {
                        if(!q) printf("You've hitten the button!!\nClearing...\n");
                        XFillRectangle(disp, win, white, 0, 0, winwidth, winheight);
                        if(!q) printf("Repainting clearbutton...\n");
                        XFillRectangle(disp, win, red, bleft, btop, bwidth, bheight);
                        XDrawString(disp, win, white, bleft+bmargin, btop+bmargin+9, msg, strlen(msg));
                      }
                    }
                  }
                }
                if (e.type == KeyPress)
                  {
                    if(!q) printf("Keycode: %d\n", e.xkey.keycode);
                    unsigned long keysym = XLookupKeysym(&e.xkey, 0);
                    if(!q) printf("Keysym: %lu\n",keysym);
                    char *ascii = XKeysymToString(keysym);
                    if(!q) printf("ASCII: %s\n",ascii);
                    if(keysym == 65307) // ESC
                      {
                        printf("Exiting...\n");
                        XCloseDisplay(disp);
                        return 0;
                      }
                    }
                  }
                }
