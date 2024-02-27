#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include "functions_head.h"

HANDLE hConsole;
COORD c;

void start_game(int level, int *position){
    level_info game_level;
    set_level(level, &game_level);
    spacecraft_info spacecraft;
    spacecraft.health = 100;
    spacecraft.x = 40;
    spacecraft.y = 20;
    spacecraft.score = 0;
    spacecraft.bullet_worth = 15;
    spacecraft.fire = 0;
    bullet *head_bullet = NULL;
    bomber *head_bomber = NULL;
    min *head_min = NULL;
    fighter head_fighter;
    head_fighter.health = 0;
    fighter_bullet *head_fighter_bullet = NULL;
    drone head_drone;
    head_drone.health = 0;
    cargo_ship head_cargo_ship; 
    head_cargo_ship.health = 0;
    head_cargo_ship.health_load = 0;
    time_t t1 = time(NULL);
    time_t t_bomber = t1;
    time_t t_min = t1;
    time_t t_fighter = t1;
    time_t t_fighter_bullet = t1;
    time_t t_drone = t1;
    time_t t_cargo_ship = t1;
    long t1_millis = currentMillis();
    long t_bombs = t1_millis;
    long t_bomberx = t1_millis;
    long t_bombery = t1_millis;
    long t_bullet = t1_millis;
    long tm_min = t1_millis;
    long tm_fighter = t1_millis;
    long tm_fighter_bullet = t1_millis;
    long tm_drone = t1_millis;
    long tm_cargo_ship = t1_millis;
    long tm_spacecraft_fire = t1_millis;
    srand(time(NULL));
    int i  = 0;
    printf("\e[1;1H\e[2J");
    while(1){
        if(kbhit()){
            char c;
            c = getch();
            if(c == 72 && spacecraft.y>1){
                spacecraft.y--;
                spacecraft.fire = 1;
            }
            else if(c == 80 && spacecraft.y<22)
                spacecraft.y++;
            if(c == 75 && spacecraft.x>1)
                spacecraft.x--;
            else if(c == 77 && spacecraft.x<84)
                spacecraft.x++;
            else if(c == ' '){
                if(head_bullet == NULL){
                    head_bullet = malloc(sizeof(bullet));
                    head_bullet->x = spacecraft.x+16;
                    head_bullet->y = spacecraft.y;
                    head_bullet->next = NULL;
                    head_bullet->prev = NULL;
                }
                else{
                    bullet *current;
                    for(current = head_bullet; current->next != NULL; current = current->next);
                    current->next = malloc(sizeof(bullet));
                    current->next->x = spacecraft.x+16;
                    current->next->y = spacecraft.y;
                    current->next->next = NULL;
                    current->next->prev = current;
                }
            }
            else if(c == 'p'){
                while(getch() != 'p');
            }
        }
        printf("\e[1;1H\e[2J");
        time_t t2 = time(NULL);
        if(t2 - t1 >= 1){
            spacecraft.score += t2-t1;
            t1 = t2;
        }
        print_border_score(&spacecraft);
        print_spacecraft(&spacecraft);
        print_bullets(&head_bullet);
        long t2_millis = currentMillis();
        long tm_spacecraft_fire2 = t2_millis;
        if(tm_spacecraft_fire2 - tm_spacecraft_fire > 400){
            spacecraft.fire = 0;
            tm_spacecraft_fire = tm_spacecraft_fire2;
        }
        long t_bullet2 = t2_millis;
        if(t_bullet2 - t_bullet >= 50){
            for(bullet *current = head_bullet; current != NULL; current = current->next)
                current->y-=2;
            t_bullet = t_bullet2;
        }
        if(t2 - t_bomber > game_level.bomber_spawn){
            if(head_bomber == NULL){
                head_bomber = malloc(sizeof(bomber));
                if(rand()%2 == 0){
                    head_bomber->next = NULL;
                    head_bomber->prev = NULL;
                    head_bomber->x = 1;
                    head_bomber->y = 1;
                    head_bomber->side = 0;
                    head_bomber->bomb = 1;
                    head_bomber->x_bomb = -1;
                    head_bomber->y_bomb = -1;
                    head_bomber->health = 10;
                    head_bomber->damage = game_level.bomb_damage;
                }
                else{
                    head_bomber->next = NULL;
                    head_bomber->prev = NULL;
                    head_bomber->x = 102;
                    head_bomber->y = 1;
                    head_bomber->side = 1;
                    head_bomber->bomb = 1;
                    head_bomber->x_bomb = -1;
                    head_bomber->y_bomb = -1;
                    head_bomber->health = 10;
                    head_bomber->damage = game_level.bomb_damage;
                }
            }
            else{
                bomber *current;
                for(current = head_bomber; current->next != NULL; current = current->next);
                current->next = malloc(sizeof(bomber));
                if(rand()%2 == 0){
                    current->next->x = 1;
                    current->next->y = 1;
                    current->next->side = 0;
                    current->next->next = NULL;
                    current->next->prev = current; 
                    current->next->bomb = 1;
                    current->next->x_bomb = -1;
                    current->next->y_bomb = -1;
                    current->next->health = 10;
                    current->next->damage = game_level.bomb_damage;
                }
                else{
                    current->next->x = 102;
                    current->next->y = 1;
                    current->next->side = 1;
                    current->next->next = NULL; 
                    current->next->prev = current; 
                    current->next->bomb = 1;
                    current->next->x_bomb = -1;
                    current->next->y_bomb = -1;
                    current->next->health = 10;
                    current->next->damage = game_level.bomb_damage;
                }
            }
            if(game_level.bomber_spawn > 6)
                game_level.bomber_spawn--;
            t_bomber = t2;
        }
        print_bomber(&head_bomber, &spacecraft);
        print_bomber_bomb(&head_bomber, &spacecraft);
        int check1 = 0, check2 = 0, check3 = 0;
        long t_bombs2 = t2_millis;
        long t_bomberx2 = t2_millis;
        long t_bombery2 = t2_millis;
        for(bomber *current = head_bomber; current != NULL; current = current->next){
            if(t_bomberx2 - t_bomberx >= 40){
                if(current->side == 0)
                    current->x++;
                else
                    current->x--;
                check3 = 1;
            }
            if(t_bombery2 - t_bombery >= 400){
                current->y++;
                check2 = 1;
            }
            if(t_bombs2 - t_bombs >= game_level.bomb_speed){
                if(current->bomb == 0)
                    current->y_bomb++;
                check1 = 1;
            }
        }
        if(check1) t_bombs = t_bombs2;
        if(check2) t_bombery = t_bombery2;
        if(check3) t_bomberx = t_bomberx2;
        if(t2 - t_min > game_level.mine_spawn){
            if(head_min == NULL){
                head_min = malloc(sizeof(min));
                head_min->next = NULL;
                head_min->prev = NULL;
                head_min->x = rand()%103 + 1;
                head_min->y = 1;
                head_min->health = 10;
                head_min->damage = game_level.mine_damage;
            }
            else{
                min *current;
                for(current = head_min; current->next != NULL; current->next = current->next->next);
                current->next = malloc(sizeof(min));
                current->next->next = NULL;
                current->next->prev = current;
                current->next->x = rand()%103 + 1;
                current->next->y = 1;
                current->next->health = 10;
                current->next->damage = game_level.mine_damage;
            }
            if(game_level.mine_spawn > 8)
                game_level.mine_spawn--;
            t_min = t2;
        }
        print_min(&head_min, &spacecraft);
        long tm_min2 = t_bullet2;
        if(tm_min2 - tm_min >= game_level.mine_speed){
            for(min *current = head_min; current != NULL; current = current->next){
                current->y++;
            }
            tm_min = tm_min2;
        }
        if(t2 - t_fighter > 13){
            head_fighter.x = rand()%90 + 1;
            head_fighter.y = 1;
            head_fighter.health = 15;
            t_fighter = t2;
        }
        print_fighter(&head_fighter, &spacecraft);
        long tm_fighter2 = t2_millis;
        if(tm_fighter2 - tm_fighter >= game_level.fighter_speed){
            head_fighter.y++;
            tm_fighter = tm_fighter2;
        }
        if(t2 - t_fighter_bullet > 1 && head_fighter.health > 0){
            if(head_fighter_bullet == NULL){
                head_fighter_bullet = malloc(sizeof(fighter_bullet));
                head_fighter_bullet->next = NULL;
                head_fighter_bullet->prev = NULL;
                head_fighter_bullet->x = head_fighter.x+14;
                head_fighter_bullet->y = head_fighter.y+5;
                head_fighter_bullet->damage = game_level.fighter_bullet_damage;
            }
            else{
                fighter_bullet *current;
                for(current = head_fighter_bullet; current->next != NULL; current->next = current->next->next);
                current->next = malloc(sizeof(fighter_bullet));
                current->next->next = NULL;
                current->next->prev = NULL;
                current->next->x = head_fighter.x+14;
                current->next->y = head_fighter.y+5;
                current->next->damage = game_level.fighter_bullet_damage;
            }
            t_fighter_bullet = t2;
        }
        print_fighter_bullet(&head_fighter_bullet, &spacecraft);
        long tm_fighter_bullet2 = t_bullet2;
        if(tm_fighter_bullet2 - tm_fighter_bullet >= 500){
            for(fighter_bullet *current = head_fighter_bullet; current != NULL; current = current->next){
                current->y++;
            }
            tm_fighter_bullet2 = tm_fighter_bullet;
        }
        if(t2 - t_drone > 18){
            head_drone.x = rand()%90 + 1;
            head_drone.y = 1;
            head_drone.health = 10;
            head_drone.color = 1;
            head_drone.damage = game_level.drone_damage;
            t_drone = t2;
        }
        print_drobe(&head_drone, &spacecraft);
        long tm_drone2 = t2_millis;
        if(tm_drone2 - tm_drone >= game_level.drone_speed){
            if(head_drone.x > spacecraft.x+18)
                head_drone.x--;
            else if(head_drone.x < spacecraft.x+18)
                head_drone.x++;
            if(head_drone.y+3 > spacecraft.y)
                head_drone.y--;
            else if(head_drone.y+3 < spacecraft.y)
                head_drone.y++;
            head_drone.color++;
            tm_drone = tm_drone2;
        }
        if(t2 - t_cargo_ship > game_level.cargo_ship_spawn){
            head_cargo_ship.x = rand()%80 + 1;
            head_cargo_ship.y = 1;
            head_cargo_ship.health = 15;
            head_cargo_ship.health_load = 1;
            head_cargo_ship.load = rand()%2;
            if(game_level.cargo_ship_spawn < 18)
                game_level.cargo_ship_spawn++;
            t_cargo_ship = t2;
        }
        print_cargo_ship(&head_cargo_ship, &spacecraft);
        long tm_cargo_ship2 = t2_millis;
        if(tm_cargo_ship2 - tm_cargo_ship >= 200){
            head_cargo_ship.y++;
            tm_cargo_ship = tm_cargo_ship2;
        }
        usleep(100);
        hit_bullet(&spacecraft, &head_bomber, &head_bullet, &head_min, &head_fighter, &head_drone, &head_cargo_ship);
        if(spacecraft.health <= 0 || spacecraft.bullet_worth <= 0) {
            if(i == 0){
                spacecraft.score += spacecraft.health;
                spacecraft.score += spacecraft.bullet_worth;
                spacecraft.health = 0;
                i++;
                continue;
            }
            sleep(2);
            game_over(position, spacecraft.score, level);
            break;
        }
    }
    bomber *current1 = head_bomber;
    while(current1 != NULL){
        bomber *temp = current1;
        current1 = current1->next;
        free(temp);
    }
    min *current2 = head_min;
    while(current2 != NULL){
        min *temp = current2;
        current2 = current2->next;
        free(temp);
    }
    fighter_bullet *current3 = head_fighter_bullet;
    while(current3 != NULL){
        fighter_bullet *temp = current3;
        current3 = current3->next;
        free(temp);
    }
    bullet *current4 = head_bullet;
    while(current4 != NULL){
        bullet *temp = current4;
        current4 = current4->next;
        free(temp);
    }
}

void menu(int *position){
    int key = 0;
    while(key != 13){
        if(*position < 0){
            *position = 4 - (*position * -1)%4;
        }
        printf("\e[1;1H\e[2J");
        print_border();
        c.X = 20; c.Y = 5;
        SetConsoleCursorPosition(hConsole, c);
        printf(YEL"   .---. ,---.    .--.    ,--,  ,---.       ,---.    .--.  ,-. ,'|\"\\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("  ( .-._)| .-.\\  / /\\ \\ .' .')  | .-'       | .-.\\  / /\\ \\ |(| | |\\ \\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf(" (_) \\   | |-' )/ /__\\ \\|  |(_) | `-.       | `-'/ / /__\\ \\(_) | | \\ \\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf(" _  \\ \\  | |--' |  __  |\\  \\    | .-'       |   (  |  __  || | | |  \\ \\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("( `-'  ) | |    | |  |)| \\  `-. |  `--.     | |\\ \\ | |  |)|| | /(|`-' /");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf(" `----'  /(     |_|  (_)  \\____\\/( __.'     |_| \\)\\|_|  (_)`-'(__)`--'");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("        (__)                   (__)             (__)"Reset);
        c.X = 48; c.Y = 14;
        SetConsoleCursorPosition(hConsole, c);
        switch ((*position) %4){
        case 0:
            printf("===> " BRED "(  Play  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Logs )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Level )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( QUIT )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        case 1:
            printf("     ( Play )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  Logs  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Level )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( QUIT )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c); 
            break;
        case 2:
            printf("     ( Play )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Logs )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  Level  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( QUIT )"); 
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        case 3:
            printf("     ( Play )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Logs )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Level )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  QUIT  )" Reset); 
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        }
        key = getch();
        if (key == 72)
            (*position)--;
        else if(key == 80)
            (*position)++;
    }
}

void print_border(){
    c.X = 0; c.Y = 0;
    SetConsoleCursorPosition(hConsole, c);
    for(int i = 0; i < 120; i++) printf("-");
    for(int i = 0; i < 28; i++) printf("\n|");
    printf("\n");
    for(int i = 0; i < 120; i++) printf("-");
    c.X = 119; c.Y = 1;
    for(int i = 0; i < 28; i++){
        SetConsoleCursorPosition(hConsole, c);
        printf("|");
        (c.Y)++;
    }
}

void print_border_score(spacecraft_info *spacecraft){
    c.X = 0; c.Y = 0;
    SetConsoleCursorPosition(hConsole, c);
    for(int i = 0; i < 54; i++) printf("-");
    printf(REDB"Score: %.3d"Reset, spacecraft->score);
    for(int i = 0; i < 55; i++) printf("-");
    for(int i = 0; i < 28; i++) printf("\n|");
    printf("\n");
    for(int i = 0; i < 120; i++) printf("-");
    c.X = 119; c.Y = 1;
    for(int i = 0; i < 28; i++){
        SetConsoleCursorPosition(hConsole, c);
        printf("|");
        (c.Y)++;
    }
}

void print_spacecraft(spacecraft_info *spacecraft){
    c.X =spacecraft->x; c.Y = spacecraft->y;
    SetConsoleCursorPosition(hConsole, c);
    printf(CYN"                /_\\");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("             __/(_)\\__");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("        ____/_ ===== _\\____");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf(" ______/ _ \\(_)_____(_)/ _ \\_____");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("<___ (+___+)| / ");
    printf(RED "%3d" CYN, spacecraft->health);
    printf(" \\ |(+___+) ___>");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("  O O  \\___/ |  (_)  | \\___/  O O");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("            \\_\\_____/_/"Reset);
    c.Y++;
    if(spacecraft->fire == 1 && c.Y<=28){
        SetConsoleCursorPosition(hConsole, c);
        printf(BRED"              \\\\//\\\\//"Reset);
        c.Y++;
        if(c.Y<=28){
            SetConsoleCursorPosition(hConsole, c);
            printf(BRED"               \\/  \\/ "Reset);
        }
    }
}

void clear_spacecraft(spacecraft_info *spacecraft){
    c.X = spacecraft->x; c.Y = spacecraft->y;
    SetConsoleCursorPosition(hConsole, c);
    printf("                    ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                       ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                           ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                                 ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                                   ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                                 ");
    (c.Y)++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                        ");   

}

void print_bullets(bullet ** head){
    bullet *current = *head;
    while(current != NULL){
        if(current->y < 1){
            bullet *temp = current;
            if(current->next == NULL){
                if(current == *head){
                    free(*head);
                    *head = NULL;
                    current = NULL;
                }
                else{
                    current = current->prev;
                    current->next = NULL;
                    current = NULL;
                    free(temp);      
                }
            }
            else if(current == *head){
                *head = current->next;
                current = current->next;
                current->prev = NULL;
                free(temp);
            }
            else{
                current->next->prev = current->prev;
                current->prev->next = current->next;
                current = current->next;
                free(temp);
            }
        }
        else{
            c.X = current->x;
            c.Y = current->y;
            SetConsoleCursorPosition(hConsole, c);
            printf(RED"/^\\"Reset);
            current = current->next;
        }
    }
}

void print_bomber(bomber ** head, spacecraft_info *spacecraft){
    bomber *current = *head;
    while(current != NULL){
        if(current->x < 1 || current->x > 102 || current->health <= 0 || (current->x+16 >= spacecraft->x && current->x <= spacecraft->x+30
        && current->y <= spacecraft->y+7 && current->y+3 >= spacecraft->y)){
            if(current->x+16 >= spacecraft->x && current->x <= spacecraft->x+30
            && current->y <= spacecraft->y+7 && current->y+3 >= spacecraft->y){
                spacecraft->health -= 5;
                spacecraft->bullet_worth -= 1;
            }
            if(current->x_bomb > 0){
                current = current->next;
                continue;
            }
            bomber *temp = current;
            if(current->next == NULL){
                if(current == *head){
                    free(*head);
                    *head = NULL;
                    current = NULL;
                }
                else{
                    current = current->prev;
                    current->next = NULL;
                    current = NULL;
                    free(temp);      
                }
            }
            else if(current == *head){
                *head = current->next;
                current = current->next;
                current->prev = NULL;
                free(temp);
            }
            else{
                current->next->prev = current->prev;
                current->prev->next = current->next;
                current = current->next;
                free(temp);
            }
        }
        else if(current->side == 1){
            c.X = current->x;   
            c.Y = current->y;
            SetConsoleCursorPosition(hConsole, c);
            printf(GRN"   ____       _");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf(" |__\\_\\_o,___/ \\");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("([___\\_\\_____-\\'");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf(" | o'" Reset);
            current = current->next;
        }
        else{
            c.X = current->x;
            c.Y = current->y;
            SetConsoleCursorPosition(hConsole, c);
            printf(GRN" _     ____");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("/ \\___,o_\\_\\__|");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("'\\-_______\\_\\_])");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("            o'|" Reset);
            current = current->next;
        }
    }   
}

long currentMillis() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void print_bomber_bomb(bomber ** head, spacecraft_info *spacecraft){
    bomber *current = *head;
    while(current != NULL){
        if((fabs(current->x - spacecraft->x) <= 2) && (current->bomb == 1)){
            current->x_bomb = current->x+9;
            current->y_bomb = current->y+4;
            current->bomb = 0;
        }
        if(current->x_bomb+4 >= spacecraft->x && current->x_bomb <= spacecraft->x+30
            && current->y_bomb+2 <= spacecraft->y+7 && current->y_bomb+4 >= spacecraft->y){
            current->x_bomb = -1;
            spacecraft->health -= current->damage;
            spacecraft->bullet_worth -= 1;
        }
        if((current->y_bomb > 25)){
            current->x_bomb = -1;
        }
        if(current->x_bomb >= 0){
            c.X = current->x_bomb;
            c.Y = current->y_bomb;
            SetConsoleCursorPosition(hConsole, c);
            printf(MAG"  )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf(" (");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);            
            printf(".-`-.");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf(":TNT:");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf(":___:"Reset);
        }
        current = current->next;
    }
}

void hit_bullet(spacecraft_info *spacecraft, bomber **head_bomber, bullet **head_bullet, min **head_min, fighter *head_fighter,
    drone *head_drone, cargo_ship *head_cargo_ship){
    bullet *current = *head_bullet;
    int check;
    while(current != NULL){
        check = 1;
        for(bomber *current2 = *head_bomber; current2 != NULL; current2 = current2->next){
            if(current->x+2 >= current2->x && current->x <= current2->x+17 && current->y <= current2->y+3 && current->y+1 >= current2->y){
                spacecraft->score += 10;
                current2->health -= spacecraft->bullet_worth;
                bullet *temp = current;
                if(current->next == NULL){
                    if(current == *head_bullet){
                        free(*head_bullet);
                        *head_bullet = NULL;
                        current = NULL;
                    }
                    else{
                        current->prev->next = NULL;
                        current = NULL;
                        free(temp);
                    }
                }
                else if(current == *head_bullet){
                    *head_bullet = current->next;
                    current = current->next;
                    current->prev = NULL;
                    free(temp);
                }
                else{
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    current = current->next;
                    free(temp);
                }
                check = 0;
                break;
            }
            if(current->x+2 >= current2->x_bomb && current->x <= current2->x_bomb+4 && current->y <= current2->y_bomb+4 && current->y+1 >= current2->y_bomb){
                current2->x_bomb = -1;
                spacecraft->score += 5;
                bullet *temp = current;
                if(current->next == NULL){
                    if(current == *head_bullet){
                        free(*head_bullet);
                        *head_bullet = NULL;
                        current = NULL;
                    }
                    else{
                        current->prev->next = NULL;
                        current = NULL;
                        free(temp);
                    }
                }
                else if(current == *head_bullet){
                    *head_bullet = current->next;
                    current = current->next;
                    current->prev = NULL;
                    free(temp);
                }
                else{
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    current = current->next;
                    free(temp);
                }
                check = 0;
                break;
            }
        }
        if(check){
            for(min* current2 = *head_min; current2 != NULL; current2 = current2->next){
                if(current->x+2 >= current2->x && current->x <= current2->x+4 && current->y <= current2->y+4 && current->y+1 >= current2->y){
                    spacecraft->score += 8;
                    current2->health -= spacecraft->bullet_worth;
                    bullet *temp = current;
                    if(current->next == NULL){
                        if(current == *head_bullet){
                            free(*head_bullet);
                            *head_bullet = NULL;
                            current = NULL;
                        }
                        else{
                            current->prev->next = NULL;
                            current = NULL;
                            free(temp);
                        }
                    }
                    else if(current == *head_bullet){
                        *head_bullet = current->next;
                        current = current->next;
                        current->prev = NULL;
                        free(temp);
                    }
                    else{
                        current->next->prev = current->prev;
                        current->prev->next = current->next;
                        current = current->next;
                        free(temp);
                    }
                    check = 0;
                    break;
                }
            }
        }
        if(check){
            if(head_fighter->health > 0 && current->x+2 >= head_fighter->x && current->x <= head_fighter->x+28 && current->y <= head_fighter->y+5 
            && current->y+1 >= head_fighter->y){
                spacecraft->score += 15;
                head_fighter->health -= spacecraft->bullet_worth;
                bullet *temp = current;
                if(current->next == NULL){
                    if(current == *head_bullet){
                        free(*head_bullet);
                        *head_bullet = NULL;
                        current = NULL;
                    }
                    else{
                        current->prev->next = NULL;
                        current = NULL;
                        free(temp);
                    }
                }
                else if(current == *head_bullet){
                    *head_bullet = current->next;
                    current = current->next;
                    current->prev = NULL;
                    free(temp);
                }
                else{
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    current = current->next;
                    free(temp);
                }
                check = 0;
            }
        }
        if(check){
            if(head_drone->health > 0 && current->x+2 >= head_drone->x && current->x <= head_drone->x+20 && current->y <= head_drone->y+3 
            && current->y+1 >= head_drone->y){
                spacecraft->score += 12;
                head_drone->health -= spacecraft->bullet_worth;
                bullet *temp = current;
                if(current->next == NULL){
                    if(current == *head_bullet){
                        free(*head_bullet);
                        *head_bullet = NULL;
                        current = NULL;
                    }
                    else{
                        current->prev->next = NULL;
                        current = NULL;
                        free(temp);
                    }
                }
                else if(current == *head_bullet){
                    *head_bullet = current->next;
                    current = current->next;
                    current->prev = NULL;
                    free(temp);
                }
                else{
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    current = current->next;
                    free(temp);
                }
                check = 0;
            }
        }
        if(check){
            if(head_cargo_ship->health > 0 && current->x+2 >= head_cargo_ship->x && current->x <= head_cargo_ship->x+31 && current->y <= head_cargo_ship->y+4 
            && current->y+1 >= head_cargo_ship->y){
                spacecraft->score += 5;
                head_cargo_ship->health -= spacecraft->bullet_worth;
                bullet *temp = current;
                if(current->next == NULL){
                    if(current == *head_bullet){
                        free(*head_bullet);
                        *head_bullet = NULL;
                        current = NULL;
                    }
                    else{
                        current->prev->next = NULL;
                        current = NULL;
                        free(temp);
                    }
                }
                else if(current == *head_bullet){
                    *head_bullet = current->next;
                    current = current->next;
                    current->prev = NULL;
                    free(temp);
                }
                else{
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                    current = current->next;
                    free(temp);
                }
                check = 0;
            }
        }
        if(check) current = current->next;
    }
}

void print_min(min **head, spacecraft_info *spacecraft){
    min *current = *head;
    while(current != NULL){
        if(current->x+5 >= spacecraft->x && current->x <= spacecraft->x+30
            && current->y <= spacecraft->y+7 && current->y+3 >= spacecraft->y){
            spacecraft->health -= current->damage;
            spacecraft->bullet_worth -= 2;
            current->health = 0;
        }
        if(current->y > 25 || current->health <= 0){
            min *temp = current;
            if(current->next == NULL){
                if(current == *head){
                    free(*head);
                    *head = NULL;
                    current = NULL;
                }
                else{
                    current->prev->next = NULL;
                    current = NULL;
                    free(temp);
                }
            }
            else if(current == *head){
                *head = current->next;
                current = current->next;
                current->prev = NULL;
                free(temp);
            }
            else{
                current->next->prev = current->prev;
                current->prev->next = current->next;
                current = current->next;
                free(temp);
            }
        }
        else{
            c.X = current->x;
            c.Y = current->y;
            SetConsoleCursorPosition(hConsole, c);
            printf(YEL".---.");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("(\\|/)");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("--0--");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("(/|\\)"Reset);
            current = current->next;
        }
    }
}

void print_fighter(fighter *head, spacecraft_info *spacecraft){
    if(head->health <= 0) return;
    if((head->y <= spacecraft->y+7 && head->y+4 >= spacecraft->y && head->x <= spacecraft->x+30 && head->x+28 >= spacecraft->x)){
        spacecraft->health -= 15;
        spacecraft->bullet_worth -= 2;
        head->health = 0;
    }
    if(head->y > 25){
        head->health = 0;
    }
    else{
        c.X = head->x;
        c.Y = head->y;
        SetConsoleCursorPosition(hConsole, c);
        printf(" .-.    _,  .-.  ,_    .-.");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("'-._'--'  \\_| |_/  '--'_.-'");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("    '-._  \\ | | /  _.-'");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("        `-.^| |^.-'");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("           `\\=/`");
    }
}

void print_fighter_bullet(fighter_bullet **head, spacecraft_info *spacecraft){
    fighter_bullet *current = *head;
    while(current != NULL){
        if((current->y >= spacecraft->y+3 && current->x >= spacecraft->x+2 && current->x <= spacecraft->x+30) || (current->y > 28)){
            if(current->y >= spacecraft->y+3 && current->x >= spacecraft->x+2 && current->x <= spacecraft->x+30){
                spacecraft->health -= current->damage;
            }
            fighter_bullet *temp = current;
            if(current->next == NULL){
                if(current == *head){
                    free(*head);
                    *head = NULL;
                    current = NULL;
                }
                else{
                    current = current->prev;
                    current->next = NULL;
                    current = NULL;
                    free(temp);      
                }
            }
            else if(current == *head){
                *head = current->next;
                current = current->next;
                current->prev = NULL;
                free(temp);
            }
            else{
                current->next->prev = current->prev;
                current->prev->next = current->next;
                current = current->next;
                free(temp);
            }
        }
        else{
            c.X = current->x;
            c.Y = current->y;
            SetConsoleCursorPosition(hConsole, c);
            printf("|");
            current = current->next;
        }
    }
}

void print_drobe(drone *head, spacecraft_info *spacecraft){
    if(head->health <= 0) return;
    if((head->y+3 >= spacecraft->y && head->y <= spacecraft->y+7 && head->x+18 >= spacecraft->x && head->x <= spacecraft->x+30)){
        spacecraft->health -= head->damage;
        spacecraft->bullet_worth -= 2;
        head->health = 0;
    }
    if(head->y > 25){
        head->health = 0;
    }
    else{
        c.X = head->x;
        c.Y = head->y;
        switch(head->color%2){
            case 0:
                SetConsoleCursorPosition(hConsole, c);
                printf(RED"\\  \\ /  /");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf(" \\||.||/");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("  \\\\V//");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("   \\./"Reset);
                break;
            case 1:
                SetConsoleCursorPosition(hConsole, c);
                printf(BLU"\\  \\ /  /");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf(" \\||.||/");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("  \\\\V//");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("   \\./"Reset);
                break;
        }
    }
}

void print_cargo_ship(cargo_ship *head, spacecraft_info *spacecraft){
    if(head->health <= 0 && head->health_load <= 0) return;
    if((head->health > 0 && head->y+4 >= spacecraft->y && head->y <= spacecraft->y+30 && head->x <= spacecraft->x+32 &&
    head->x+32 >= spacecraft->x)){
        spacecraft->health -= 5;
        spacecraft->bullet_worth -= 1;
        head->health = 0;
    }
    else if(head->health_load > 0 && head->y+4 >= spacecraft->y && head->y <= spacecraft->y+30 && head->x <= spacecraft->x+32
    && head->x+10 >= spacecraft->x){
            if(head->load%2 == 1){
                spacecraft->health += 10;
                head->health_load = 0;
                spacecraft->score += 10;
            }
            else{
                spacecraft->bullet_worth += 5;
                head->health_load = 0;
                spacecraft->score += 10;
            }
    }
    if(head->y > 27){
        head->health = 0;
        head->health_load = 0;
    }
    else{
        c.X = head->x;
        c.Y = head->y;
        if(head->health > 0){
            SetConsoleCursorPosition(hConsole, c);
            printf(GRN"         \\            /");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("          \\    __    /");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("___________\\.-|__|-./___________");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("   + + ---\\__| \\/ |__/--- + +");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("              \\__/"Reset);
        }
        else if(head->health_load > 0){
            if(head->load%2 == 1){
                SetConsoleCursorPosition(hConsole, c);
                printf(RED",d88b.d88b,");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("88888888888");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("`Y8888888Y'");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("  `Y888Y'");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf("    `Y'"Reset);
            }
            else{
                SetConsoleCursorPosition(hConsole, c);
                printf(RED" +-+-+-+-+");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf(" |A|M|M|O|");
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
                printf(" +-+-+-+-+"Reset);
            }
        }
    }
}

void game_over(int *position, int score, int level){
    system("cls");
    char level_str[10] = {0};
    switch (level%4){
    case 0:
        strcpy(level_str, "Easy");
        break;
    case 1:
        strcpy(level_str, "Medium");
        break;
    case 2:
        strcpy(level_str, "Hard");
        break;
    case 3:
        strcpy(level_str, "Expert");
        break;
    }
    print_border();
    c.X = 30; c.Y = 8;
    SetConsoleCursorPosition(hConsole, c);
    printf("Please enter your name: ");
    char input[50];
    gets(input);
    int key = 0;
    int choice = 0;
    while(key != 13){
        printf("\e[1;1H\e[2J");
        print_border();
        c.X = 30; c.Y = 10;
        SetConsoleCursorPosition(hConsole, c);
        printf(RED"   _________    _____   ____     _______  __ ___________");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("  / ___\\__  \\  /     \\_/ __ \\   /  _ \\  \\/ // __ \\_  __ \\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf(" / /_/  > __ \\|  Y Y  \\  ___/  (  <_> )   /\\  ___/|  | \\/");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf(" \\___  (____  /__|_|  /\\___  >  \\____/ \\_/  \\___  >__|");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("/_____/     \\/      \\/     \\/                   \\/"Reset);
        c.Y+=3;
        c.X = 38;
        SetConsoleCursorPosition(hConsole, c);
        printf("%s, You've got " YEL "%d" Reset " scores at %s level!", input, score, level_str);
        c.X = 48; c.Y+=2;
        SetConsoleCursorPosition(hConsole, c);
        if(choice<0)
            choice = 2 - (choice*-1)%2;
        switch (choice%2){
        case 0:
            printf("===> " RED "( Play again )"Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Menu )");
            break;
        case 1:
            printf("     ( Play again )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> " RED "( Menu )" Reset);
            break;
        }
        key = getch();
        if (key == 72)
            choice--;
        else if(key == 80)
            choice++;
    }
    time_t t = time(NULL);
    FILE *fp = fopen("logs.txt", "a");
    fprintf(fp, ">> %s scored %d points on %s level, on %s", input, score, level_str, ctime(&t));
    fclose(fp);
    if(choice < 0){
        choice = 2 - (choice*-1)%2;
    }
    *position = choice%4;
}

void choose_level(int *level){
    int key = 0;
    while(key != 13){
        printf("\e[1;1H\e[2J");
        print_border();
        c.X = 48; c.Y = 12;
        SetConsoleCursorPosition(hConsole, c);
        if(*level < 0){
            *level = 4 - (*level * -1)%4;
        }
        switch ((*level) %4){
        case 0:
            printf("===> " BRED "(  Easy  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Medium )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Hard )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Expert )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        case 1:
            printf("     ( Easy )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  Medium  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Hard )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Expert )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c); 
            break;
        case 2:
            printf("     ( Easy )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Medium )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  Hard  )" Reset);
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Expert )"); 
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        case 3:
            printf("     ( Easy )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Medium )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("     ( Hard )");
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            printf("===> "BRED"(  Expert  )" Reset); 
            c.Y++;
            SetConsoleCursorPosition(hConsole, c);
            break;
        }
        key = getch();
        if (key == 72)
            (*level)--;
        else if(key == 80)
            (*level)++;
    }
}

void set_level(int level, level_info *game_level){
    switch (level%4){
    case 0:
        game_level->bomb_speed = 700;
        game_level->mine_speed = 650;
        game_level->drone_speed = 700;
        game_level->fighter_speed = 600;
        game_level->bomber_spawn = 8;
        game_level->mine_spawn = 12;
        game_level->cargo_ship_spawn = 16;
        game_level->bomb_damage = 7;
        game_level->mine_damage = 10;
        game_level->drone_damage = 10;
        game_level->fighter_bullet_damage = 2;
        break;
    case 1:
        game_level->bomb_speed = 500;
        game_level->mine_speed = 550;
        game_level->drone_speed = 450;
        game_level->fighter_speed = 400;
        game_level->bomber_spawn = 7;
        game_level->mine_spawn = 10;
        game_level->cargo_ship_spawn = 17;
        game_level->bomb_damage = 8;
        game_level->mine_damage = 10;
        game_level->drone_damage = 12;
        game_level->fighter_bullet_damage = 3;
        break;
    case 2:
        game_level->bomb_speed = 350;
        game_level->mine_speed = 450;
        game_level->drone_speed = 300;
        game_level->fighter_speed = 250;
        game_level->bomber_spawn = 5;
        game_level->mine_spawn = 7;
        game_level->cargo_ship_spawn = 17;
        game_level->bomb_damage = 10;
        game_level->mine_damage = 13;
        game_level->drone_damage = 15;
        game_level->fighter_bullet_damage = 4;
        break;
    case 3:
        game_level->bomb_speed = 200;
        game_level->mine_speed = 300;
        game_level->drone_speed = 350;
        game_level->fighter_speed = 100;
        game_level->bomber_spawn = 4;
        game_level->mine_spawn = 5;
        game_level->cargo_ship_spawn = 18;
        game_level->bomb_damage = 12;
        game_level->mine_damage = 15;
        game_level->drone_damage = 18;
        game_level->fighter_bullet_damage = 5;
        break;
    }
}

void print_logs(){
    int key = 0;
    char log[13][100] = {0};
    int n = 0, check = 0;
    FILE *fp = fopen("logs.txt", "r");
    char temp[101] = {0};
    while(fgets(temp, 100, fp) != NULL){
        strcpy(log[n], temp);
        n++;
        if(n>=13){
            n = 0;
            check = 1;
        }
    }
    fclose(fp);
    while(key != 13){
        system("cls");
        print_border();
        c.X = 36; c.Y = 5;
        SetConsoleCursorPosition(hConsole, c);
        printf(MAG" _____         ____        _____     _____");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("(_   _)       / __ \\      / ___ \\   / ____\\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("  | |        / /  \\ \\    / /   \\_) ( (___");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("  | |       ( ()  () )  ( (  ____   \\___ \\");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("  | |   __  ( ()  () )  ( ( (__  )      ) )");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("__| |___) )  \\ \\__/ /    \\ \\__/ /   ___/ /");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("\\________/    \\____/      \\____/   /____/"Reset);
        c.X = 30; c.Y = 13;
        SetConsoleCursorPosition(hConsole, c);
        if(check){
            for(int i = n-1; i >= 0; i--){
                printf("%s", log[i]);
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
            }
            for(int i = 12; i >= n; i--){
                printf("%s", log[i]);
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
            }
        }
        else
            for(int i = n-1; i >= 0; i--){
                printf("%s", log[i]);
                c.Y++;
                SetConsoleCursorPosition(hConsole, c);
            }
        c.X = 47; c.Y = 26;
        SetConsoleCursorPosition(hConsole, c);
        printf(RED"___  ____ ____ _  _");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("|__] |__| |    |_/");
        c.Y++;
        SetConsoleCursorPosition(hConsole, c);
        printf("|__] |  | |___ | \\_"Reset);
        key = getch();
    }
}

void print_bye(){
    system("cls");
    print_border();
    c.X = 10; c.Y = 10;
    SetConsoleCursorPosition(hConsole, c);
    printf(YEL"  ,ad8888ba,                                      88     88888888ba");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf(" d8\"'    `\"8b                                     88     88      \"8b");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("d8'                                               88     88      ,8P");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("88              ,adPPYba,    ,adPPYba,    ,adPPYb,88     88aaaaaa8P'  8b       d8   ,adPPYba,");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("88      88888  a8\"     \"8a  a8\"     \"8a  a8\"    `Y88     88\"\"\"\"\"\"8b,  `8b     d8'  a8P_____88  ");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("Y8,        88  8b       d8  8b       d8  8b       88     88      `8b   `8b   d8'   8PP\"\"\"\"\"\"\"  ");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf(" Y8a.    .a88  \"8a,   ,a8\"  \"8a,   ,a8\"  \"8a,   ,d88     88      a8P    `8b,d8'    \"8b,   ,aa  ");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("  `\"Y88888P\"    `\"YbbdP\"'    `\"YbbdP\"'    `\"8bbdP\"Y8     88888888P\"       Y88'      `\"Ybbd8\"'  ");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                                                                          d8'");
    c.Y++;
    SetConsoleCursorPosition(hConsole, c);
    printf("                                                                         d8'"Reset);
}