/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "theme.h"

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            False,       -1  }, 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask /* Super and alt are swapped in ~/.xsessions */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb",
	norm_bg, "-nf", norm_fg, "-sb", sel_border, "-sf", sel_fg, NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", "-theme",
	"~/.config/rofi/launchers/type-4/style-3.rasi", NULL};
static const char *termcmd[]  = { "kitty", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *firefox_yt[] = { "firefox", "--new-window", "youtube.com", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	
	/* spawns */
	// { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_d,      spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_z,      spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = firefox_yt } },
	
	/* screenshot */
	{ MODKEY,                       XK_s,      spawn,
		SHCMD("maim -s -k -B -u > ~/down/screenshot-$(date +'%Y-%m-%d_%H-%M-%S').png") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,
		SHCMD("maim -s -k -B -u | xclip -selection clipboard -t image/png") },

	/* notifications */
	{ MODKEY,                       XK_n,      spawn,     
		SHCMD("kill -s USR1 $(pidof deadd-notification-center)") }, 
	
	/* volume */
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,
		SHCMD("pamixer -i 5") },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,  spawn,
		SHCMD("pamixer -i 1") },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,
		SHCMD("pamixer -d 5") },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,  spawn,
		SHCMD("pamixer -d 1") },
	{ 0,                            XF86XK_AudioMute,         spawn,
		SHCMD("pamixer -t") },

    /* audio/video player */
    { 0,                            XF86XK_AudioPlay,         spawn,
        SHCMD("playerctl play-pause") },
    { 0,                            XF86XK_AudioStop,         spawn,
        SHCMD("playerctl stop") },
    { 0,                            XF86XK_AudioPrev,         spawn,
        SHCMD("playerctl previous") },
    { 0,                            XF86XK_AudioNext,         spawn,
        SHCMD("playerctl next") },
    { 0,                            XF86XK_AudioStop,         spawn,
        SHCMD("playerctl next") },
	
	/* brightness */
	{ 0,                           XF86XK_MonBrightnessUp,    spawn,
		SHCMD("brightnessctl s +10%") },
	{ ShiftMask,                   XF86XK_MonBrightnessUp,    spawn,
		SHCMD("brightnessctl s +1%")  },
	{ 0,                           XF86XK_MonBrightnessDown,  spawn,
		SHCMD("brightnessctl s 10%-") },
	{ ShiftMask,                   XF86XK_MonBrightnessDown,  spawn,
		SHCMD("brightnessctl s 1%-")  },

	{ 0,                           XF86XK_Calculator,         spawn,
		SHCMD("pidof speedcrunch || speedcrunch && speedcrunch") },
	
	/* toggle bar */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	
	/* tiling */
	/** stack **/
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },

	/** mfact**/
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.005} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.005} },

	/** cfact **/
	{ MODKEY,                       XK_o,      setcfact,       {.f = 0.00} },
	{ MODKEY,                       XK_u,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_u,      setcfact,       {.f = -0.1} },
	{ MODKEY,                       XK_i,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_i,      setcfact,       {.f = +0.1} },
	
	/* kill client */
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	
	/* layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	/* all tag */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	
	/* monitor */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	
	/* tag */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	
	/* quit dwm */
	{ MODKEY|ShiftMask|ControlMask, XK_e,      quit,           {0} },
	/* power */
	{ MODKEY,                       XK_p,      spawn,          SHCMD("systemctl suspend") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("systemctl hibernate") },
	{ MODKEY|ShiftMask|ControlMask, XK_p,      spawn,          SHCMD("systemctl poweroff") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};
