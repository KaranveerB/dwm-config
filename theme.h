/* appearance */
static const unsigned int borderpx  = 2;              /* border pixel of windows */
static const unsigned int gappx     = 2;              /* gaps between windows */
static const unsigned int snap      = 24;             /* snap pixel */
static const int showbar            = 1;              /* 0 means no bar */
static const int topbar             = 1;              /* 0 means bottom bar */
static const int usealtbar          = 1;              /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar";      /* alt bar class name */
static const char *altbarcmd        = "$HOME/.config/polybar/launch.sh"; /* alt bar launch command */
static const char *fonts[]          = { "DejaVu Sans Mono:size=11:antialias=true:autohint=true",
					"Symbols Nerd Font:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "DejaVu Sans Mono:size=11:antialias=true:autohint=true";

static const char norm_fg[]         = "#6285ef";
static const char norm_bg[]         = "#06080a";
static const char norm_border[]     = "#151515";
static const char sel_fg[]          = "#db96ea";
static const char sel_bg[]          = "#030405";
static const char sel_border[]      = "#252e77";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
};

/* tagging */
static const char *tags[] = { "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘", "⭘" };

