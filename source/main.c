#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <switch.h>

u32 getBatteryPercent(void)
{
    u32 percent = 0;
    psmInitialize();
    psmGetBatteryChargePercentage(&percent);
    psmExit();
    return percent;
}

const char *colors[] = {
    "\x1b[41m", // red
    "\x1b[42m", // green
    "\x1b[43m", // yellow
    "\x1b[44m", // blue
    "\x1b[45m", // magenta
    "\x1b[46m", // cyan
    "\x1b[47m", // white
    "\x1b[40m"  // black
};

void printBackgroundColor(const char *color);

int main(int argc, char **argv)
{
    consoleInit(NULL);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);

    printf("DeathPixelChecker\n");
    printf("Battery: %u%%\n", getBatteryPercent());
    printf("[-] to start the check\n");

    consoleUpdate(NULL);

    int colorIndex = 0;

    while(appletMainLoop())
    {
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        if (kDown & HidNpadButton_Minus) {
            consoleClear();

            printf("controls :\n");
            printf("(a) to change background color\n");
            printf("(+) to exit the app\n");
            printf("press (a) to continue\n");

            consoleUpdate(NULL);

            while (appletMainLoop()) {
                padUpdate(&pad);
                u64 menuButtons = padGetButtonsDown(&pad);

                if (menuButtons & HidNpadButton_Plus) break;

                if (menuButtons & HidNpadButton_Minus) {
                    break;
                }

                if (menuButtons & HidNpadButton_A) {
                    printBackgroundColor(colors[colorIndex]);
                    colorIndex++;
                    if (colorIndex >= 8) colorIndex = 0;
                }

                consoleUpdate(NULL);
            }
        }

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}

void printBackgroundColor(const char *color)
{
    consoleClear();
    
    printf(color);
    for (int y = 0; y < 45; y++) {
        for (int x = 0; x < 80; x++) {
            printf(" ");
        }
        printf("\n");
    }
    printf("\x1b[H");

    consoleUpdate(NULL);
}
