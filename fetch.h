#ifndef _HFETCH
#define _HFETCH

#define TL_CHAR L'┏'
#define TR_CHAR L'┓'
#define BL_CHAR L'┗'
#define BR_CHAR L'┛'
#define V_CHAR L'┃'
#define H_CHAR L'━'
#define SEPARATOR '>'

#define STR_LEN 39

// box size
#define T_MARGIN 1
#define B_MARGIN 1
#define L_MARGIN 2
#define R_MARGIN 2
#define AFTER_SEP_MARGIN 2

#define L_MARGIN_IN 1
#define R_MARGIN_IN 1

#define SEP_ALIGN 10

#define clear_buffer while (fgetc(pipe) != EOF) // clear buffer

struct __system_info
{
    char host_name[STR_LEN], host[STR_LEN];
    char kernel_name[STR_LEN], kernel[STR_LEN];
    char distro_name[STR_LEN], distro[STR_LEN];
    char de_name[STR_LEN], de[STR_LEN];
    char wm_name[STR_LEN], wm[STR_LEN];
    char packages_name[STR_LEN], packages[STR_LEN];
    char terminal_name[STR_LEN], terminal[STR_LEN];
    char shell_name[STR_LEN], shell[STR_LEN];
    char uptime_name[STR_LEN], uptime[STR_LEN];
};

typedef struct __system_info SystemInfo;

#define EXCLUDE(OPTS, OPT) OPTS &= ~OPT

enum ToFetch
{
    NONE     = 1 << 0,
    HOST     = 1 << 1,
    KERNEL   = 1 << 2,
    DISTRO   = 1 << 3,
    DE       = 1 << 4,
    WM       = 1 << 5,
    PACKAGES = 1 << 6,
    TERMINAL = 1 << 7,
    SHELL    = 1 << 8,
    UPTIME   = 1 << 9,
    ALL      = 0xFFFF
};

enum Colors
{
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
};

#define set_bold() wprintf(L"\e[1m")
#define set_color(color) wprintf(L"\e[%dm", color + 30)
#define reset_color() wprintf(L"\e[0m")

#define UPTIME_S                                                  \
    "awk '{print int($1/3600) \" hours, \" int(($1%3600)/60) \" " \
    "minutes\"}' /proc/uptime"
#define PACKAGES_S "echo \"$(pacman -Q | wc -l) (pacman)\""
#define DISTRO_S \
    "echo \"$(grep '^NAME=' /etc/os-release | cut -d'=' -f2 | tr -d '\"')\""
#define KERNEL_S "cat /proc/version | cut -d' ' -f1,3"

#endif // !_HFETCH
