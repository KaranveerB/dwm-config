/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;              /* border pixel of windows */
static const unsigned int gappx     = 2;              /* gaps between windows */
static const unsigned int snap      = 24;             /* snap pixel */
static const int showbar            = 1;              /* 0 means no bar */
static const int topbar             = 1;              /* 0 means bottom bar */
static const int usealtbar          = 1;              /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar";      /* alt bar class name */
static const char *altbarcmd        = "$HOME/tmp.sh"; /* alt bar launch command */
static const char *fonts[]          = { "DejaVu Sans Mono:size=11:antialias=true:autohint=true",
					"Symbols Nerd Font:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "DejaVu Sans Mono:size=11:antialias=true:autohint=true";

static const char norm_fg[]         = "#a2dfef";
static const char norm_bg[]         = "#010311";
static const char norm_border[]     = "#719ca7";
static const char sel_fg[]          = "#18aaec";
static const char sel_bg[]          = "#010311";
static const char sel_border[]      = "#a2dfef";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
};

/* tagging */
static const char *tags[] = { "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            False,       -1  }, 
};
/** swaptag 6.2 **/
void swaptags(const Arg *arg);

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */ static const int nmaster     = 1;    /* number of clients in master area */ static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */ static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask /* Super and alt are swapped using autostart (~/.dwm/autostart_blocking.sh)  */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} }, 

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", norm_bg, "-nf", norm_fg, "-sb", sel_border, "-sf", sel_fg, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *firefox_yt[] = { "firefox", "--new-window", "youtube.com" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	
	/* spawns */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_z,      spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = firefox_yt } },
	
	/* screenshot */
	{ MODKEY,                       XK_s,      spawn,          SHCMD("scrot --select") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("scrot") },

	/* notifications */
	{ MODKEY,                       XK_n,      spawn,     
		SHCMD("kill -s USR1 $(pidof deadd-notification-center)") }, 
	
	/* volume */
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,
		SHCMD("amixer -q set Master 5%+ && kill -35 $(pidof dwmblocks)") },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,  spawn,
		SHCMD("amixer -q set Master 1%+ && kill -35 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,
		SHCMD("amixer -q set Master 5%- && kill -35 $(pidof dwmblocks)") },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,  spawn,
		SHCMD("amixer -q set Master 1%- && kill -35 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_AudioMute,         spawn,
		SHCMD("amixer -D pulse set Master toggle && kill -35 $(pidof dwmblocks)") },
	
	/* brightness */
	{ 0,                           XF86XK_MonBrightnessUp,     spawn,
		SHCMD("brightnessctl s +10%") },
	{ ShiftMask,                   XF86XK_MonBrightnessUp,     spawn,
		SHCMD("brightnessctl s +1%")  },
	{ 0,                           XF86XK_MonBrightnessDown,   spawn,
		SHCMD("brightnessctl s 10%-") },
	{ ShiftMask,                   XF86XK_MonBrightnessDown,   spawn,
		SHCMD("brightnessctl s 1%-")  },
	
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
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
	/* power */
	{ MODKEY,                       XK_e,      spawn,          SHCMD("systemctl suspend") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("systemctl hibernate") },
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

void
swaptags(const Arg *arg)
{
	unsigned int newtag = arg->ui & TAGMASK;
	unsigned int curtag = selmon->tagset[selmon->seltags];

	if (newtag == curtag || !curtag || (curtag & (curtag-1)))
		return;

	for (Client *c = selmon->clients; c != NULL; c = c->next) {
		if((c->tags & newtag) || (c->tags & curtag))
			c->tags ^= curtag ^ newtag;

		if(!c->tags) c->tags = newtag;
	}

	selmon->tagset[selmon->seltags] = newtag;

	focus(NULL);
	arrange(selmon);
}
