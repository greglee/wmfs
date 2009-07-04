/*
*      wmfs.h
*      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
*      All rights reserved.
*
*      Redistribution and use in source and binary forms, with or without
*      modification, are permitted provided that the following conditions are
*      met:
*
*      * Redistributions of source code must retain the above copyright
*        notice, this list of conditions and the following disclaimer.
*      * Redistributions in binary form must reproduce the above
*        copyright notice, this list of conditions and the following disclaimer
*        in the documentation and/or other materials provided with the
*        distribution.
*      * Neither the name of the  nor the names of its
*        contributors may be used to endorse or promote products derived from
*        this software without specific prior written permission.
*
*      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef WMFS_H
#define WMFS_H

/* Lib headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <confuse.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xrandr.h>

/* Local headers */
#include "config.h"
#include "structs.h"

/* MACRO */
#define ButtonMask   (ButtonPressMask | ButtonReleaseMask | ButtonMotionMask)
#define MouseMask    (ButtonMask | PointerMotionMask)
#define KeyMask      (KeyPressMask | KeyReleaseMask)

#define CWIN(win, parent, x, y, w, h, b, mask, col, at)                             \
    do {                                                                            \
        win = XCreateWindow(dpy, (parent), (x), (y), (w), (h), (b), CopyFromParent, \
        InputOutput, CopyFromParent, (mask), (at));                                 \
        XSetWindowBackground(dpy, win, (col));                                      \
    } while (/*CONSTCOND*/0)

#define SCREEN       DefaultScreen(dpy)
#define ROOT         RootWindow(dpy, SCREEN)
#define MAXH         DisplayHeight(dpy, DefaultScreen(dpy))
#define MAXW         DisplayWidth(dpy, DefaultScreen(dpy))
#define ATOM(a)      XInternAtom(dpy, (a), False)
#define INFOBARH     ((conf.bars.height > 0) ? conf.bars.height : (font->height * 1.5))
#define FHINFOBAR    ((font->height - font->descent) + (INFOBARH - font->height) / 2)
#define SHADH        (1)
#define SHADC        (0x000000) /* 'Cause i don't know how darken a color yet */
#define BORDH        conf.client.borderheight
#define TBARH        ((conf.titlebar.height < BORDH) ? BORDH : conf.titlebar.height)
#define FRAMEW(w)    ((w) + BORDH * 2)
#define FRAMEH(h)    ((h) + (BORDH  + TBARH))
#define ROUND(x)     (float)((x > 0) ? x + (float)0.5 : x - (float)0.5)
#define RESHW        (6 * BORDH)
#define BUTTONWH     (TBARH / 2)
#define CHECK(x)     if(!(x)) return
#define IFREE(x)     if(x) free(x)
#define LEN(x)       (sizeof(x) / sizeof((x)[0]))
#define deb(p)       fprintf(stderr, "debug: %d\n", (p))
#define PAD          conf.pad

/* barwin.c */
BarWindow *barwin_create(Window parent,
                         int x, int y,
                         uint w, uint h,
                         uint bg, char*fg,
                         Bool entermask,
                         Bool stipple,
                         Bool border);
void barwin_draw_text(BarWindow *bw, int x, int y, char *text);
void barwin_delete(BarWindow *bw);
void barwin_delete_subwin(BarWindow *bw);
void barwin_map(BarWindow *bw);
void barwin_map_subwin(BarWindow *bw);
void barwin_unmap(BarWindow *bw);
void barwin_unmap_subwin(BarWindow *bw);
void barwin_move(BarWindow *bw, int x, int y);
void barwin_resize(BarWindow *bw, uint w, uint h);
void barwin_refresh_color(BarWindow *bw);
void barwin_refresh(BarWindow *bw);

/* draw.c */
void draw_text(Drawable d, int x, int y, char* fg, int pad, char *str);
void draw_rectangle(Drawable dr, int x, int y, uint w, uint h, uint color);
ushort textw(const char *text);

/* infobar.c */
void infobar_init(void);
void infobar_draw(int sc);
void infobar_draw_layout(int sc);
void infobar_draw_taglist(int sc);
void infobar_draw_statustext(int sc, char *str);
void infobar_destroy(void);
void infobar_set_position(int pos);
void uicb_infobar_togglepos(uicb_t);

/* client.c */
void client_attach(Client *c);
void client_configure(Client *c);
void client_detach(Client *c);
void client_focus(Client *c);
/* client_gb_*() {{{ */
Client* client_gb_win(Window w);
Client* client_gb_frame(Window w);
Client* client_gb_titlebar(Window w);
Client* client_gb_resize(Window w);
Client* client_gb_button(Window w, int *n);
/* }}} */
void client_get_name(Client *c);
void client_hide(Client *c);
void client_kill(Client *c);
Bool ishide(Client *c, int screen);
void client_map(Client *c);
Client* client_manage(Window w, XWindowAttributes *wa, Bool ar);
void client_geo_hints(XRectangle *geo, Client *c);
void client_moveresize(Client *c, XRectangle geo, Bool r);
void client_maximize(Client *c);
void client_size_hints(Client *c);
void client_swap(Client *c1, Client *c2);
void client_raise(Client *c);
void client_unhide(Client *c);
void client_unmanage(Client *c);
void client_unmap(Client *c);
void client_set_wanted_tag(Client *c);
void client_update_attributes(Client *c);
void uicb_client_raise(uicb_t);
void uicb_client_prev(uicb_t);
void uicb_client_next(uicb_t);
void uicb_client_kill(uicb_t);

/* ewmh.c */
void ewmh_init_hints(void);
void ewmh_get_number_of_desktop(void);
void ewmh_update_current_tag_prop(void);
void ewmh_get_client_list(void);
void ewmh_get_desktop_names(void);
void ewmh_set_desktop_geometry(void);
void ewmh_set_workarea(void);
void ewmh_manage_net_wm_state(long data_l[], Client *c);
void ewmh_manage_window_type(Client *c);

/* frame.c */
void frame_create(Client *c);
void frame_delete(Client *c);
void frame_moveresize(Client *c, XRectangle geo);
void frame_update(Client *c);

/* config.c */
void conf_alias_section(cfg_t *cfg_a);
void conf_misc_section(cfg_t *cfg_m);
void conf_bar_section(cfg_t *cfg_b);
void conf_root_section(cfg_t *cfg_r);
void conf_client_section(cfg_t *cfg_c);
void conf_layout_section(cfg_t *cfg_l);
void conf_tag_section(cfg_t *cfg_t);
void conf_menu_section(cfg_t *cfg_m);
void conf_keybind_section(cfg_t *cfg_k);
void init_conf(void);

/* event.c */
void buttonpress(XButtonEvent *ev);
void configureevent(XConfigureRequestEvent *ev);
void clientmessageevent(XClientMessageEvent *ev);
void destroynotify(XDestroyWindowEvent *ev);
void enternotify(XCrossingEvent *ev);
void expose(XExposeEvent *ev);
void focusin(XFocusChangeEvent *ev);
void grabkeys(void);
void keypress(XKeyPressedEvent *ev);
void mappingnotify(XMappingEvent *ev);
void maprequest(XMapRequestEvent *ev);
void propertynotify(XPropertyEvent *ev);
void unmapnotify(XUnmapEvent *ev);
void xrandrnotify(XEvent *ev);
void send_client_event(long data[5], char *atom_name);
void getevent(XEvent ev);

/* menu.c */
void menu_init(Menu *menu, char *name, int nitem, uint bg_f, char *fg_f, uint bg_n, char *fg_n);
void menu_new_item(MenuItem *mi, char *name, void *func, char *cmd);
void menu_draw(Menu menu, int x, int y);
Bool menu_manage_event(XEvent *ev, Menu *menu, BarWindow *winitem[]);
void menu_focus_item(Menu *menu, int item, BarWindow *winitem[]);
void menu_draw_item_name(Menu *menu, int item, BarWindow *winitem[]);
int menu_get_longer_string(MenuItem *mt, int nitem);
void uicb_menu(uicb_t cmd);

/* launcher.c */
void launcher_execute(Launcher launcher);
void uicb_launcher(uicb_t);

/* mouse.c */
void mouse_dragborder(XRectangle geo, GC g);
void mouse_move(Client *c);
void mouse_resize(Client *c);
void mouse_grabbuttons(Client *c, Bool focused);
void uicb_mouse_move(uicb_t);
void uicb_mouse_resize(uicb_t);

/* util.c */
ulong color_enlight(ulong col);
void *emalloc(uint element, uint size);
long getcolor(char *color);
void setwinstate(Window win, long state);
char* _strdup(char const *str);
/* Conf usage {{{ */
void* name_to_func(char *name, func_name_list_t *l);
ulong char_to_modkey(char *name, key_name_list_t key_l[]);
uint char_to_button(char *name, name_to_uint_t blist[]);
Layout layout_name_to_struct(Layout lt[], char *name, int n, func_name_list_t llist[]);
char* alias_to_str(char *conf_choice);
/* }}} */
XRectangle get_mouse_pos(void);
char *char_to_str(const char c);
void spawn(const char *str, ...);
void swap_ptr(void **x, void **y);
void uicb_spawn(uicb_t);

/* tag.c */
void tag_set(int tag);
void tag_transfert(Client *c, int tag);
void uicb_tag(uicb_t);
void uicb_tag_next(uicb_t);
void uicb_tag_prev(uicb_t);
void uicb_tagtransfert(uicb_t);

/* screen.c */
int screen_count(void);
XRectangle screen_get_geo(int s);
int screen_get_with_geo(int x, int y);
int screen_get_sel(void);
void screen_set_sel(int screen);
void screen_init_geo(void);
void uicb_screen_select(uicb_t);
void uicb_screen_next(uicb_t);
void uicb_screen_prev(uicb_t);

/* layout.c */
void arrange(int screen);
void freelayout(int screen);
void layoutswitch(Bool b);
void maxlayout(int screen);
Client *tiled_client(int screen, Client *c);
/* tile {{{ */
 void grid(int screen);
 void tile(int screen);
 void tile_left(int screen);
 void tile_top(int screen);
 void tile_bottom(int screen);
 void mirror_vertical(int screen);
 void mirror_horizontal(int screen);
/* }}} */
void uicb_tile_switch(uicb_t);
void uicb_togglemax(uicb_t);
void uicb_togglefree(uicb_t);
void uicb_layout_prev(uicb_t);
void uicb_layout_next(uicb_t);
void uicb_set_mwfact(uicb_t);
void uicb_set_nmaster(uicb_t);
void uicb_set_layout(uicb_t);
void uicb_toggle_resizehint(uicb_t);

/* init.c */
void init(void);
void init_root(void);
void init_layout(void);
void init_font(void);
void init_gc(void);
void init_cursor(void);
void init_key(void);
void init_geometry(void);

/* getinfo.c */
void getinfo_tag(void);
void getinfo_screen(void);
void getinfo_layout(void);
void getinfo_mwfact(void);
void getinfo_nmaster(void);
void getinfo(char *info);

/* wmfs.c */
int errorhandler(Display *d, XErrorEvent *event);
int errorhandlerdummy(Display *d, XErrorEvent *event);
void quit(void);
void mainloop(void);
void scan(void);
Bool check_wmfs_running(void);
void exec_uicb_function(char *func, char *cmd);
void set_statustext(char *str);
void handle_signal(int signum);
void uicb_quit(uicb_t);
void uicb_reload(uicb_t);

/* Variables */

/* Principal */
Display *dpy;
GC gc, gc_stipple;
int selscreen;
Conf conf;
Key *keys;
Bool exiting;
char *statustext;
XRectangle *sgeo;
XRectangle *spgeo;
Cursor cursor[CurLast];
char *argv_global;
int xrandr_event;

/* Fonts */
XftFont *font;

/* Atoms list */
Atom net_atom[net_last];

/* InfoBar/Tags */
InfoBar *infobar;
Tag **tags;
int *seltag;
Menu menulayout;

/* Important Client */
Client *clients;
Client *sel;

/* Other */
func_name_list_t *func_list;
func_name_list_t *layout_list;
uint numlockmask;

#endif /* WMFS_H */


