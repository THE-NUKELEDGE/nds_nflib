#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

int main(int argc, char **argv)
{
    consoleDemoInit(); // Initialize console

    while (1)
    {
        swiWaitForVBlank();
    }

    return 0;
}