/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 18;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
 /*   Display modes of the tab bar: never shown, always shown, shown only in  */
 /*   monocle mode in presence of several windows.                            */
 /*   A mode can be disabled by moving it after the showtab_nmodes end marker */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab            = showtab_auto; /* Default tab bar show mode  */
static const Bool toptab            = True;         /* False means bottom tab bar */

static const char *fonts[]          = { "CaskaydiaCove Nerd Font:size=11" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#1d2021";
static const char col_gray2[]       = "#32302f";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_fg[]					= "#fbf1c7";
static const char col_cyan[]        = "#005577";
static const char col_green[]       = "#b8bb26";
static const char col_urgborder[]   = "#ff0000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray1, col_green,  col_green},
	[SchemeUrg]  = { col_urgborder, col_green,  col_urgborder  },
	[SchemeStatus]  = { col_fg, col_gray1,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_gray1, col_green,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { col_fg, col_gray1,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_fg, col_gray2,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { col_fg, col_gray1,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const char *const autostart[] = {
	//"/home/pryamcem/.local/bin/pixel-lock", NULL,
	//Set wallpaper
	"feh", "--bg-fill", "/home/pryamcem/.wallpapers/pawel-czerwinski-IskA3TGmyGk-unsplash.jpg", NULL,
	//Set touchpad and trackpad settings
	"xinput", "set-prop", "Elan Touchpad", "libinput Accel Speed", "0.4", NULL,
	"xinput", "set-prop", "12", "325", "-0.6", NULL,
	"xsetroot", "-cursor_name" ,"left-ptr", NULL,
	//Set keyboard layout
	"setxkbmap", "-layout", "us,ua", "-option", "grp:caps_toggle,grp_led:caps", NULL,
	//Autostart some apps
	"dwmblocks", NULL,
	"xfce4-power-manager", NULL,
	"xfce4-clipman", NULL,
	"nm-applet", NULL,
	"pa-notify", NULL,
	"/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1", NULL, 
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
//static const char *tags[] = { "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι", "κ" }; //replace boring numbers with greek alphabet

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance					 title       tags mask     isfloating   monitor */
	{ "Gcolor3",  NULL,							 NULL,       0,            1,           -1 },
	//{ "my-notes", NULL,							 NULL,       0,            1,           -1 },
	{ "mpv",      NULL,							 NULL,       0,            1,           -1 },
	//{ "Brave",    NULL,							 NULL,       1 << 1,       0,           -1 },
	{ "TelegramDesktop",    NULL,		 NULL,       1 << 8,       0,           -1 },
	{ "Lollypop", NULL,							 NULL,       1 << 9,       0,           -1 },
	{ NULL,  "music.youtube.com",    NULL,			 1 << 9,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[T]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
//static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-drun", "-show", "drun", NULL };
static const char *termcmd[] = { "kitty", NULL };
static const char *filecmd[] = { "thunar", NULL };
static const char *webcmd[] = { "brave", NULL };
static const char *clipman[] = { "xfce4-popup-clipman", NULL };
static const char *statusUpdate[] = { "/home/pryamcem/.config/dwmblocks/scripts/sb-update-all", NULL };

static const char *raiseVolume[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *lowerVolume[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *muteVolume[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *muteMicVolume[] = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };


static const char *updateVolume[] = { "pkill", "-RTMIN+10", "dwmblocks", NULL };

static const char *scrSelectBuffer[] = {"/home/pryamcem/.config/dwm/scripts/screenshot", "select", NULL};
static const char *scrAllBuffer[] = {"/home/pryamcem/.config/dwm/scripts/screenshot", NULL};
static const char *scrWindowBuffer[] = {"/home/pryamcem/.config/dwm/scripts/screenshot", "window", NULL};

static const char *rofiExit[] = {"/home/pryamcem/.local/bin/rofiexit", NULL};

static const char *ytMusicExec[] = { "brave", "--app=https://music.youtube.com", NULL };
static const char *deezerExec[] = { "brave", "--app=https://deezer.com", NULL };
static const char *myNotesExec[] = {"kitty", "--class", "my-notes", "vim", "-c", "VimwikiIndex", NULL};

#define PrScrDWM	    0x0000ff61
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ ControlMask,					        XK_Tab,    spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,			 spawn,          {.v = clipman } },
	{ MODKEY,                       XK_F2,		 spawn,          {.v = webcmd } },
	{ MODKEY,                       XK_F3,		 spawn,          {.v = filecmd } },
	{ MODKEY,                       XK_F4,		 spawn,          {.v = ytMusicExec } },
	//{ MODKEY,                       XK_F4,		 spawn,          {.v = deezerExec } },
	{ MODKEY,                       XK_n,		   spawn,          {.v = myNotesExec } },

	{ MODKEY,                       XK_u,		   spawn,          {.v = statusUpdate } },


	{ 0,														XF86XK_AudioRaiseVolume,	spawn,				 {.v = updateVolume } },
	{ 0,														XF86XK_AudioRaiseVolume,	spawn,				 {.v = raiseVolume } },

	{ 0,														XF86XK_AudioLowerVolume,	spawn,         {.v = updateVolume } },
	{ 0,														XF86XK_AudioLowerVolume,	spawn,         {.v = lowerVolume } },

	{ 0,														XF86XK_AudioMute,					spawn,         {.v = updateVolume } },
	{ 0,														XF86XK_AudioMute,					spawn,         {.v = muteVolume } },

	{ 0,														XF86XK_AudioMicMute,			spawn,         {.v = muteMicVolume } },

	{ ControlMask|ShiftMask,    XK_Print,		   spawn,          {.v = scrSelectBuffer } },
	{ ShiftMask,								XK_Print,		   spawn,          {.v = scrAllBuffer } },
	{ Mod1Mask|ShiftMask,				XK_Print,		   spawn,          {.v = scrWindowBuffer } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0] } },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1] } },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2] } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_minus,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_minus,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	{ MODKEY|ShiftMask,					    XK_e,      spawn,           {.v = rofiExit} },
	{ MODKEY|Mod1Mask,					    XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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

