#include "fetch.h"
#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

SystemInfo
get_system_info(enum ToFetch to_fetch)
{
    FILE      *pipe;
    char       buffer[STR_LEN];
    SystemInfo sysinfo = { 0 };

    if (HOST & to_fetch)
    {
        pipe = popen("echo ${USER}@$(hostname)", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.host, buffer);
        strcpy(sysinfo.host_name, "host");
    }
    if (KERNEL & to_fetch)
    {
        pipe = popen(KERNEL_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.kernel, buffer);
        strcpy(sysinfo.kernel_name, "kernel");
    }
    if (DISTRO & to_fetch)
    {
        pipe = popen(DISTRO_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.distro, buffer);
        strcpy(sysinfo.distro_name, "distro");
    }
    if (DE & to_fetch)
    {
        pipe = popen("echo $XDG_CURRENT_DESKTOP", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.de, buffer);
        strcpy(sysinfo.de_name, "de");
    }
    if (WM & to_fetch)
    {
        pipe = popen("echo $XDG_CURRENT_DESKTOP", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.wm, buffer);
        strcpy(sysinfo.wm_name, "wm");
    }
    if (PACKAGES & to_fetch)
    {
        pipe = popen(PACKAGES_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.packages, buffer);
        strcpy(sysinfo.packages_name, "packages");
    }
    if (TERMINAL & to_fetch)
    {
        pipe = popen("echo $TERM", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.terminal, buffer);
        strcpy(sysinfo.terminal_name, "terminal");
    }
    if (SHELL & to_fetch)
    {
        pipe = popen("echo $SHELL | cut -d '/' -f3", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.shell, buffer);
        strcpy(sysinfo.shell_name, "shell");
    }
    if (UPTIME & to_fetch)
    {
        pipe = popen(UPTIME_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.uptime, buffer);
        strcpy(sysinfo.uptime_name, "uptime");
    }

    if (CPU & to_fetch)
    {
        pipe = popen(CPU_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.cpu, buffer);
        strcpy(sysinfo.cpu_name, "cpu");
    }

    if (GPU & to_fetch)
    {
        pipe = popen(GPU_S, "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.gpu, buffer);
        strcpy(sysinfo.gpu_name, "gpu");
    }

    if (MODEL & to_fetch)
    {
        pipe = popen("cat /sys/class/dmi/id/product_name", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.model, buffer);
        strcpy(sysinfo.model_name, "model");
    }

    if (VENDOR & to_fetch)
    {
        pipe = popen("cat /sys/class/dmi/id/sys_vendor", "r");
        fgets(buffer, STR_LEN, pipe);
        clear_buffer;
        buffer[strlen(buffer) - 1] = 0;
        strcpy(sysinfo.vendor, buffer);
        strcpy(sysinfo.vendor_name, "vendor");
    }

    pclose(pipe);

    return sysinfo;
    buffer[strlen(buffer)] = 0;
}

void
print_info(SystemInfo sysinfo, int len)
{
    for (int i = 0; i < sizeof(SystemInfo); i += STR_LEN * 2)
    {
        // real info
        if (strlen((char *) &sysinfo + i) > 0)
        {
            wprintf(L"%-*s", L_MARGIN, "");    // left margin
            putwchar(V_CHAR);                  // border
            wprintf(L"%-*s", L_MARGIN_IN, ""); // left margin (in)

            reset_color();
            set_bold();
            set_color(WHITE);
            wprintf(L"%-*s", SEP_ALIGN, (char *) &sysinfo + i); // field name

            reset_color();
            set_bold();
            set_color(GREEN);
            wprintf(L"%c%-*s", SEPARATOR, AFTER_SEP_MARGIN, ""); // after sep margin

            reset_color();
            set_color(WHITE);
            wprintf(L"%-*s", len - SEP_ALIGN - AFTER_SEP_MARGIN - 3,
                    (char *) &sysinfo + i + STR_LEN); // field info

            wprintf(L"%-*s", R_MARGIN_IN, ""); // right margin (in)
            putwchar(V_CHAR);                  // border
            wprintf(L"%-*s", R_MARGIN, "");    // right margin
            putwchar('\n');                    // new line
        }
    }
}

int
get_len(SystemInfo sysinfo)
{
    int len = 0, l;
    int PADD = R_MARGIN_IN + SEP_ALIGN + AFTER_SEP_MARGIN + L_MARGIN_IN + 1;
    for (int i = 0; i < sizeof(SystemInfo); i += STR_LEN * 2)
        if (strlen((char *) &sysinfo + i) > 0)
            if ((l = strlen((char *) &sysinfo + i + STR_LEN)) + PADD > len)
                len = l + PADD;
    return len;
}

void
render_info(SystemInfo sysinfo)
{
    int len = get_len(sysinfo);
    setlocale(LC_ALL, "");
    // top margin
    for (int i = 0; i < T_MARGIN; i++)
        putwchar('\n');
    wprintf(L"%-*s", L_MARGIN, ""); // left margin
    // top bar
    putwchar(TL_CHAR);
    for (int i = 0; i < len; i++)
        putwchar(H_CHAR);
    putwchar(TR_CHAR);
    putwchar('\n');
    // meddium bar
    print_info(sysinfo, len);
    wprintf(L"%-*s", L_MARGIN, ""); // left margin
    // bottom bar
    putwchar(BL_CHAR);
    for (int i = 0; i < len; i++)
        putwchar(H_CHAR);
    putwchar(BR_CHAR);
    putwchar('\n');
    // bottom margin
    for (int i = 0; i < B_MARGIN; i++)
        putwchar('\n');
}

int
main()
{
    int OPT = ALL;
    EXCLUDE(OPT, DE);
    EXCLUDE(OPT, CPU);
    EXCLUDE(OPT, GPU);

    SystemInfo sysinfo = get_system_info(OPT);
    render_info(sysinfo);
    return 0;
}
