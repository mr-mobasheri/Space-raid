#include<stdio.h>
#include <windows.h>
#include <unistd.h>
#include "functions_head.h"

HANDLE hConsole;
COORD c;

int main(){
    int position = 0;
    int level = 0;
    printf(HIDE_cursor);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while(1){
        menu(&position);
        if(position < 0)
            position = 4 - (position*-1)%4;
        if(position%4 == 3){
            print_bye();
            sleep(2);
            break;
        }
        if(position%4 == 1)
            print_logs();
        while(position%4 == 0)
            start_game(level, &position);
        if(position%4 == 2){
            choose_level(&level);
            if(level < 0)
                level = 4 - (level*-1)%4;
        }
        position = 0;
    }
    return 0;
}