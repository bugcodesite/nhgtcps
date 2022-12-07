#include <stdlib.h>
#include <stdio.h>
#include "../inc/config.h"

int main(int argc,char**argv){
    lmm_start();
    printf("%s\n",ver);
    test();
    lmm_end();
}