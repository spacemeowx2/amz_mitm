#include <stdio.h>
#include <stdlib.h>
#include <switch.h>
#include <string.h>
#include "ldn.h"

#define MODULEID 0x233
static Service g_ldnSrv;
static LdnMitmConfigService g_ldnConfig;

Result saveLogToFile() {
    Result rc = 0;
    rc = ldnMitmSaveLogToFile(&g_ldnConfig);
    if (R_FAILED(rc)) {
        printf("Save log to file failed %x\n", rc);
        return rc;
    }

    return rc;
}

void cleanup() {
    serviceClose(&g_ldnSrv);
    serviceClose(&g_ldnConfig.s);
}

void die(const char *reason) {
    printf("fatal: %s\npress any key to exit.", reason);
    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown) {
            break;
        }
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
    cleanup();
    exit(1);
}

void printHeader() {
    char version[32];
    Result rc = ldnMitmGetVersion(&g_ldnConfig, version);
    if (R_FAILED(rc)) {
        strcpy(version, "Error");
    }

    printf("    ldnmitm_config " VERSION_STRING "\n          amz_mitm %s\n\n", version);
}

const char * getOnOff(u32 enabled) {
    if (enabled) {
        return CONSOLE_GREEN "ON" CONSOLE_RESET;
    } else {
        return CONSOLE_RED "OFF" CONSOLE_RESET;
    }
}

void printStatus() {
    u32 enabled;
    Result rc = ldnMitmGetLogging(&g_ldnConfig, &enabled);
    if (R_FAILED(rc)) {
        die("failed to get logging status");
    }
    printf("Logging(X): %s\n", getOnOff(enabled));

    rc = ldnMitmGetEnabled(&g_ldnConfig, &enabled);
    if (R_FAILED(rc)) {
        die("failed to get enabled status");
    }
    printf("amz_mitm(Y): %s\n", getOnOff(enabled));

    putchar('\n');
    puts("Press X: toggle logging (sd:/amz_mitm.log)");
    puts("Press Y: toggle amz_mitm");
    puts("Press B: exit");
}

void reprint() {
    consoleClear();

    printHeader();
    printStatus();
}

void toggleLogging() {
    u32 enabled;
    Result rc = ldnMitmGetLogging(&g_ldnConfig, &enabled);
    if (R_FAILED(rc)) {
        die("failed to get logging status");
    }
    rc = ldnMitmSetLogging(&g_ldnConfig, !enabled);
    if (R_FAILED(rc)) {
        die("failed to set logging status");
    }
}

void toggleEnabled() {
    u32 enabled;
    Result rc = ldnMitmGetEnabled(&g_ldnConfig, &enabled);
    if (R_FAILED(rc)) {
        die("failed to get enabled status");
    }
    rc = ldnMitmSetEnabled(&g_ldnConfig, !enabled);
    if (R_FAILED(rc)) {
        die("failed to set enabled status");
    }
}

void getLdnMitmConfig() {
    Result rc = ldnMitmGetConfig(&g_ldnConfig);
    if (R_SUCCEEDED(rc)) {
        return;
    }

    Result namedRc = rc;
    rc = smGetService(&g_ldnSrv, "ldn:u");
    if (R_FAILED(rc)) {
        die("failed to get service ldn:u");
    }
    rc = ldnMitmGetConfigFromService(&g_ldnSrv, &g_ldnConfig);
    if (R_SUCCEEDED(rc)) {
        return;
    }

    printf("error code: 0x%x, 0x%x\n", rc, namedRc);
    die("amz_mitm is not loaded or too old(requires amz_mitm >= v1.1.2)");
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    getLdnMitmConfig();

    reprint();
    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_B) {
            break;
        }

        if (kDown & KEY_LSTICK) {
            Result rc = saveLogToFile();
            if (R_SUCCEEDED(rc)) {
                puts("Export complete");
            }
        }

        if (kDown & KEY_X) {
            toggleLogging();
            reprint();
        }

        if (kDown & KEY_Y) {
            toggleEnabled();
            reprint();
        }

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    cleanup();
    return 0;
}
