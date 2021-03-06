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
  int winheight = 300;
  int winwidth = 500;
  int borderwidth = 10;
  int stdcposx = 20;
  int stdcposy = 20;
  int cposx = stdcposx;
  int cposy = stdcposy;
  int lastdeltime = 0;
  int lastrowlength = 0;
  int lastkeysym = 0;
  char *msg = "Clear";
  int bwidth=strlen(msg)*6;
  int bheight=15;
  int btop=20;
  int bmargin = 2;
  int bleft=winwidth - bwidth - 20;
  GC white;
  XColor white_color;
  Colormap colormap;
  char whiteidk[] = "#FFFFFF";
  GC red;
  XColor red_color;
  Colormap rcolormap;
  char redidk[] = "#FF0000";

  disp = XOpenDisplay(NULL);
  if (disp == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen = DefaultScreen(disp);
  win = XCreateSimpleWindow(disp, RootWindow(disp, screen), winx, winy, winwidth, winheight, borderwidth,
  BlackPixel(disp, screen), WhitePixel(disp, screen));
  XSelectInput(disp, win, ExposureMask | KeyPressMask | ButtonPressMask);
  // Window XCreateWindow(display, parent, x, y, width, height, border_width, depth,
  //                    class, visual, valuemask, attributes)
  XMapWindow(disp, win);
  colormap = DefaultColormap(disp, 0);
  white = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, colormap, whiteidk, &white_color);
  XAllocColor(disp, colormap, &white_color);
  XSetForeground(disp, white, white_color.pixel);
  rcolormap = DefaultColormap(disp, 0);
  red = XCreateGC(disp, win, 0, 0);
  XParseColor(disp, colormap, redidk, &red_color);
  XAllocColor(disp, colormap, &red_color);
  XSetForeground(disp, red, red_color.pixel);
  while (1) {
    XNextEvent(disp, &e);
    if (e.type == Expose)
      {
        XFillRectangle(disp, win, red, bleft, btop, bwidth, bheight);
        XDrawString(disp, win, white, bleft+bmargin, btop+bmargin+9, msg, strlen(msg));
        //http://tronche.com/gui/x/xlib/graphics/drawing-text/XDrawString.html
      }
      if(e.type == 4) // button press
        {
          int x=e.xbutton.x;
          int y=e.xbutton.y;
          int button = e.xbutton.button;
          if(!q) printf("Button pressed. X: %d, Y: %d Button: %d\n",x,y,button);
          if(button == 1)
            {
              if(y < bheight+btop && x < bwidth+bleft)
                {
                  if(y > btop && x > bleft)
                    {
                      if(!q) printf("You've hitten the button!!\nClearing...\n");
                      XFillRectangle(disp, win, white, 0, 0, winwidth, winheight);
                      cposx = stdcposx;
                      cposy = stdcposy;
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
                  if(keysym == 65307 && lastkeysym == 65406) // alt+ESC
                    {
                      printf("Exiting...\n");
                      return 0;
                    }
                    else if(keysym == 65293) // line break
                    {
                      cposy+=10;
                      lastrowlength = cposx;
                      cposx = stdcposx;
                    }
                    else if(keysym == 32) // " "
                    {
                      cposx+=6;
                    }
                    else if(keysym == 65288) // delete
                    {
                      time_t now = time(0);
                      if(!q) printf("Time: %ld\n",now);
                      int diff = now-lastdeltime;
                      if(!q) printf("Time since last deletion: %d\n",diff);
                      XFillRectangle(disp, win, white, cposx-6, cposy-9, 6, 11);
                      if(diff == 0)
                        {
                          XFillRectangle(disp, win, white, cposx-12, cposy-9, 6, 11);
                        }
                        lastdeltime = now;
                        if(cposx <= stdcposx)
                          {
                            if(cposy != stdcposy)
                              {
                                cposy-=10;
                                cposx = lastrowlength;
                              }
                            }
                            else
                              {
                                cposx-=6;
                                if(diff == 0 && cposx-6 > stdcposx)
                                  {
                                    cposx-=6;
                                  }
                                }
                              }
                              else if(keysym == 43) // plus
                              {
                                XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, "+", 1);
                                cposx+=6;
                              }
                              else if(keysym == 44) // comma
                              {
                                XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, ",", 1);
                                cposx+=6;
                              }
                              else if(keysym == 45) // minus
                              {
                                XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, "-", 1);
                                cposx+=6;
                              }
                              else if(keysym == 46) // period
                              {
                                XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, ".", 1);
                                cposx+=6;
                              }
                              else if(keysym == 65289) // tab
                              {
                                XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, "   ", 3);
                                cposx+=18;
                              }
                              else
                                {
                                  if(lastkeysym != 65506 || lastkeysym != 65505)
                                    {
                                      if(keysym == 49)
                                        {
                                          ascii = "!";
                                        }
                                        else if(keysym == 43)
                                        {
                                          ascii = "?";
                                        }
                                      }
                                      if(keysym != 65506 && keysym != 65505)
                                        {
                                          XDrawString(disp, win, DefaultGC(disp, screen), cposx, cposy, ascii, strlen(ascii));
                                          cposx+=strlen(ascii)*6;
                                        }
                                      }
                                      lastkeysym = keysym;
                                    }
                                  }
                                  XCloseDisplay(disp);
                                  return 0;
                                }
