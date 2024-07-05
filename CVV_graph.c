#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <unistd.h>

#define LARGEUR 30
#define HAUTEUR 7

typedef struct Chips{
    char type;
    int life;
    int line;
    int position;
    int price;
    struct Chips* next;
}Chips;

typedef struct Virus{
    int type;
    int life;
    int line;
    int position;
    int speed;
    int turn;
    struct Virus* next;
    struct Virus* next_line;
    struct Virus* prev_line;
}Virus;

typedef struct{
    Virus* virus;
    Chips* chips;
    int turn;
    int money;
}Game;

typedef struct{
    char plateau[HAUTEUR][LARGEUR];
}Plateau;

typedef struct{
    int x;
    int y;
} Case;

typedef enum
{
    false,
    true
}Bool;

void Plateau_ascii(Plateau *plateau)
{
    for(int i = 0;i < HAUTEUR ;i++){
        for(int j = 0;j< LARGEUR ;j++){
            plateau->plateau[i][j] = '.' ;
        }
    }
}
void Cree_fenetre(){
    int largeur = (30 * 50) + 85;
    int longeur = (7 * 100) + 200;
    MLV_create_window("Chips vs Virus","Chips vs Virus",largeur,longeur);
}

void information_plateau(Game *game)
{
    MLV_draw_filled_rectangle(455,25,50,25,MLV_COLOR_BLACK);
    MLV_draw_text(405,25,"Money :",MLV_COLOR_RED1);
    char money;
    sprintf(&money,"%d",game->money);
    MLV_draw_text(465,25,&money,MLV_COLOR_RED1);
}

void Creation_plateau(){

    MLV_Image *background;
    MLV_Image *pikachu,*carapuce,*salameche,*bulbizare;
    MLV_Image *wallpaper,*wallpaper_pikachu,*wallpaper_starter,*pokemon;
    int largeur = (30 * 50) + 100;
    int longeur = (7 * 100) + 200;
    MLV_draw_filled_rectangle(0,0,largeur,longeur,MLV_COLOR_BLACK);
    wallpaper = MLV_load_image("assets/wallpaper.jpg");
    wallpaper_pikachu = MLV_load_image("assets/pikachu_wallpaper.jpg");
    wallpaper_starter = MLV_load_image("assets/starter_wallpaper.jpg");
    pokemon = MLV_load_image("assets/pokemon_go.jpg");
    pikachu = MLV_load_image("assets/pikachu.png");
    carapuce = MLV_load_image("assets/carapuce.png");
    salameche = MLV_load_image("assets/salameche.png");
    bulbizare = MLV_load_image("assets/bulbizare.png");
    MLV_resize_image_with_proportions(pikachu,150,100);
    MLV_resize_image_with_proportions(salameche,150,100);
    MLV_resize_image_with_proportions(bulbizare,150,100);
    MLV_resize_image_with_proportions(carapuce,150,100);
    MLV_resize_image_with_proportions(pokemon,350,350);
    MLV_resize_image_with_proportions(wallpaper_pikachu,300,300);
    MLV_resize_image_with_proportions(wallpaper_starter,600,600);
    MLV_draw_image(wallpaper,500,0);
    MLV_draw_image(wallpaper_pikachu,5,700);
    MLV_draw_image(pokemon,650,0);
    MLV_draw_image(wallpaper_starter,1000,5);
    MLV_draw_image(pikachu,10,0);
    MLV_draw_image(carapuce,30,100);
    MLV_draw_image(salameche,210,0);
    MLV_draw_image(bulbizare,210,100);
    MLV_draw_rectangle(0,0,200,100,MLV_COLOR_WHITE);
    MLV_draw_rectangle(1,1,199,99,MLV_COLOR_WHITE);
    MLV_draw_text(150,25,"50",MLV_COLOR_RED1);
    MLV_draw_rectangle(0,100,200,100,MLV_COLOR_WHITE);
    MLV_draw_rectangle(1,101,199,99,MLV_COLOR_WHITE);
    MLV_draw_text(350,25,"60",MLV_COLOR_RED1);
    MLV_draw_rectangle(200,0,200,100,MLV_COLOR_WHITE);
    MLV_draw_rectangle(201,1,199,99,MLV_COLOR_WHITE);
    MLV_draw_text(150,125,"70",MLV_COLOR_RED1);
    MLV_draw_rectangle(200,100,200,100,MLV_COLOR_WHITE);
    MLV_draw_rectangle(201,101,199,99,MLV_COLOR_WHITE);
    MLV_draw_text(350,125,"80",MLV_COLOR_RED1);
    MLV_actualise_window();
    for(int i = 0;i<HAUTEUR+1;i++)
    {
        if(i==0)
        {
            MLV_draw_line(1,197,LARGEUR*53-6-1,197 ,MLV_COLOR_WHITE);   
            MLV_draw_line(1,198,LARGEUR*53-6-1,198 ,MLV_COLOR_WHITE);   
            MLV_draw_line(1,200,LARGEUR*53-6-1,200 ,MLV_COLOR_WHITE);    
        }
        if(i==HAUTEUR)
        {
            MLV_draw_line(1,196+100*i,LARGEUR*53-6-1,196+100*i ,MLV_COLOR_WHITE);   
            MLV_draw_line(1,197+100*i,LARGEUR*53-6-1,197+100*i ,MLV_COLOR_WHITE);   
            MLV_draw_line(1,198+100*i,LARGEUR*53-6-1,198+100*i,MLV_COLOR_WHITE);    
        }
        MLV_draw_line(0,199 + i*100,LARGEUR*53-6,199 + 100*i,MLV_COLOR_WHITE);
    }
    for(int j=0;j<LARGEUR+1;j++)
    {
        if(j==0)
        {
            MLV_draw_line(1,199,1,199 + LARGEUR*99,MLV_COLOR_WHITE);   
            MLV_draw_line(2,199,2,199 + LARGEUR*99,MLV_COLOR_WHITE);   
            MLV_draw_line(3,199,3,199 + LARGEUR*99,MLV_COLOR_WHITE);    
        }
        MLV_draw_line(j*99,199,j*99,199 + LARGEUR*99,MLV_COLOR_WHITE);
    }
    MLV_actualise_window();
}
void affiche_chips(Game *game)
{
    Chips *actuel = game->chips;
    while(actuel != NULL)
    {
        MLV_Image* image;
        if(actuel->type == 'W')
        {
            image = MLV_load_image("assets/pikachu.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,99*actuel->position-95,105 + 100*actuel->line);
        }
        if(actuel->type == 'Y')
        {
            image = MLV_load_image("assets/carapuce.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,99*actuel->position-95,105 + 100*actuel->line);
        }
        if(actuel->type == 'X')
        {
            image = MLV_load_image("assets/salameche.png");
            MLV_resize_image_with_proportions(image,89,89);
            MLV_draw_image(image,99*actuel->position-95,105 + 100*actuel->line);
        }
        if(actuel->type == 'Z')
        {
            image = MLV_load_image("assets/bulbizare.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,100*actuel->position-100 ,105 + 100*actuel->line);
        }
        actuel = actuel->next;
        MLV_actualise_window();
    }
}
void affiche_virus(Game *game)
{
    Virus *actuel = game->virus;
    while(actuel != NULL)
    {
        MLV_Image* image;
        char type = actuel->type;
        int largeur = 30*50-1400;
        int longeur = 7*100+200;
        if(type == 'E')
        {
            image = MLV_load_image("assets/pokeball.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,(actuel->position+1)*99-largeur-95,105 + 100*actuel->line);
        }
        if(type == 'A')
        {
            image = MLV_load_image("assets/masterball.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,(actuel->position+1)*99-largeur-95,105 + 100*actuel->line);
        }
        if(type == 'B')
        {
            image = MLV_load_image("assets/miaouss.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,(actuel->position+1)*99-largeur-95,105 + 100*actuel->line);
        }
        if(type == 'C')
        {
            image = MLV_load_image("assets/team_rocket.png");
            MLV_resize_image_with_proportions(image,90,90);
            MLV_draw_image(image,(actuel->position+1)*99-largeur-95,105 + 100*actuel->line);
        }
        if(type == 'D')
        {
            image = MLV_load_image("assets/red.png");
            MLV_resize_image_with_proportions(image,100,100);
            MLV_draw_image(image,(actuel->position+1)*99-largeur-95,105 + 100*actuel->line);
        }
        actuel = actuel->next;
    }
}

void clique_case(Case* zone)
{
    int x,y;
    MLV_wait_mouse(&x,&y);
    zone->x = x;
    zone->y = y;
    (*zone).x = x/100 + 1;
    (*zone).y = (y-200)/100 + 1;
}
void clique_type(Case* zone)
{
    int x,y;
    MLV_wait_mouse(&x,&y);
    zone->x = x;
    zone->y = y;
    (*zone).x = x;
    (*zone).y = y;    
}
int choix_type(Case *zone)
{
    int x = zone->x;
    int y = zone->y;
    if(x>0 && x<200 && y>0 && y<100){return 'W';}
    if(x>200 && x<400 && y>0 && y<100){return 'X';}
    if(x>0 && x<200 && y>100 && y<200){return 'Y';}
    if(x>200 && x<400 && y>100 && y<200){return 'Z';}
    return 0;
}

/////////////////VIRUS////////////////////
void afficher_virus(Game *game)
{
    if(game == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Virus *actuel = game->virus;
    while(actuel != NULL)
    {
        printf("Virus(%d,%d)\n",actuel->line,actuel->position);
        actuel = actuel->next;
    }
    printf("NULL\n");
}

int nb_virus_txt(){

    FILE *fichier = fopen("level/level1.txt","r");
    int nb_virus = 0;
    int car;

    if(fichier == NULL)
        exit(1);
    while((car = fgetc(fichier)) != EOF)
    {
        if(car == '\n')
        {
            nb_virus += 1; 
        }
    }
    fclose(fichier);
    return nb_virus - 2;
}

int virus_life(int type)
{
    if (type == 'E')
        return 50;
    if (type == 'A')
        return 100;
    if (type == 'B')
        return 70;
    if (type == 'C')
        return 80;
    if (type == 'D')
        return 100;
    return 0;
}
int virus_speed(int type)
{
    if (type == 'E')
        return 1;
    if (type == 'A')
        return 1;
    if (type == 'B')
        return 1;
    if (type == 'C')
        return 2;
    if (type == 'D')
        return 2;
    return 0;
}
int virus_attaque(Virus *virus)
{
    if(virus->type == 'E'){return 15;}
    if(virus->type == 'A'){return 10;}
    if(virus->type == 'B'){return 30;}
    if(virus->type == 'C'){return 40;}
    if(virus->type == 'D'){return 40;}
    return 0;
}

Virus* Cree_Virus(char type,int line,int turn)
{
    Virus* virus = (Virus*)malloc(sizeof(Virus));
    virus->type = type;
    virus->line = line;
    virus->turn = turn;
    virus->position = LARGEUR + turn;
    virus->life = virus_life(type);
    virus->speed = virus_speed(type);
    virus->next = NULL;
    virus->next_line = NULL;
    virus->prev_line = NULL;
    return virus;
}

void Ajoute_virus(Game* game, int type, int life, int line, int position, int speed, int turn)
{
    
    Virus* new_virus = (Virus*)malloc(sizeof(Virus));
    new_virus->type = type;
    new_virus->life = life;
    new_virus->line = line;
    new_virus->position = position;
    new_virus->speed = speed;
    new_virus->turn = turn;
    new_virus->next = NULL;
    new_virus->next_line = NULL;
    new_virus->prev_line = NULL;
    
    
    
    if (game->virus != NULL)
    { 
        Virus* virus = game->virus;
        Virus* last_virus_line = NULL;
    
        while (1)
        {
            
            if (virus->line == new_virus->line)
            {
                last_virus_line = virus;
            }
            if (virus->next == NULL)
            {
                break;
            }
            else
            {
                virus = virus->next;
            }
        }

        virus->next = new_virus;


        if (last_virus_line != NULL)
        {
            last_virus_line->next_line = new_virus;
            new_virus->prev_line = last_virus_line;
        }
        
    }
    else
    {
        game->virus = new_virus;
    }
}

void Suppresion_virus(Game* game, Virus* virus)
{
    if (game->virus == virus)
    {
        game->virus = virus->next;
        if(virus->next_line != NULL)
        {
            virus->next_line->prev_line = NULL;
        }
        if(virus->next == NULL)
        {
            game->virus = NULL;
        }
        free(virus);
    }
    else
    {
        Virus* actuel_virus = game->virus;
        while (actuel_virus != NULL && actuel_virus->next != virus) {
            actuel_virus = actuel_virus->next;
        }
        if (actuel_virus != NULL)
        {
            actuel_virus->next = virus->next;
            if (virus->prev_line != NULL) {
                virus->prev_line->next_line = virus->next_line;
            }
            if (virus->next_line != NULL) {
                virus->next_line->prev_line = virus->prev_line;
            }
            free(virus);
        }
    }
}
void chaine_virus(Game *game,Virus *tab_virus)
{
    for(int i = 0;i<nb_virus_txt()-1;i++)
    {
        int type = tab_virus[i].type;
        int line = tab_virus[i].line;
        int turn = tab_virus[i].turn;
        int position = 16 + turn;
        int life = virus_life(type);
        int speed = virus_speed(type);
        Ajoute_virus(game,type,life,line,position,speed,turn);
    }
}

Bool Depassement_virus(Game *game,Virus *virus)
{
    if(virus->prev_line != NULL)
    {
        int prev_line = virus->prev_line->position;
        int prev_speed = virus->prev_line->speed;
        if (virus->position - virus->speed <= prev_line)            
        {   
            return true;
        }
    }
    return false;
}
Bool Depassement_chips(Game *game,Virus *virus)
{
    Chips *actuel_chips= game->chips;
    while(actuel_chips != NULL)
    {
        if(actuel_chips->position >= virus->position - virus->speed && actuel_chips->line == virus->line)
        {
            virus->position = actuel_chips->position+1;
            return true;
        }
        actuel_chips = actuel_chips->next;
    }
    return false;
}

void virus_part(Game *game)
{
    Virus *actuel = game->virus;
    Bool depassement_virus;
    Bool depassement_chips;
    while(actuel != NULL)
    {   
        if (actuel->position-1>=0)
        {
            depassement_virus = Depassement_virus(game,actuel);
            depassement_chips = Depassement_chips(game,actuel);
            if(depassement_virus == true)
            {
                actuel->position = actuel->prev_line->position+1;
            }
            if(depassement_virus == false)
            {
                if(depassement_chips == false)
                {
                    actuel->position -= actuel->speed;
                    if (actuel->position <= 1){actuel->position = 1;}
                }      
            }
        }   
        actuel = actuel->next;
    }
}
void virus_action(Game *game)
{
    Virus *actuel_virus = game->virus;
    while(actuel_virus != NULL)
    {
        Chips *actuel_chips = game->chips;
        while(actuel_chips != NULL)
        {
            if(actuel_virus->position-1 == actuel_chips->position && actuel_virus->line == actuel_chips->line)
            {
                actuel_chips->life -= virus_attaque(actuel_virus);    
            }
            actuel_chips = actuel_chips->next;
        }
        actuel_virus = actuel_virus->next;
    }

}
void Plateau_virus(Plateau *plateau,Game *game)
{
    Virus *actuel = game->virus;
    while(actuel != NULL)
    {   
        if(actuel->position < LARGEUR)
        {
            plateau->plateau[actuel->line-1][actuel->position] = actuel->type;
        }
        actuel = actuel->next;
    }
}
void delete_virus(Game *game)
{
    if(game->virus != NULL)
    {
        Virus *actuel = game->virus;
        while(actuel != NULL)
        {

            if(actuel->life <= 0)
            {
                Suppresion_virus(game,actuel);
            }
            actuel = actuel->next;
        }
    }
}
/////////////////CHIPS////////////////////

int chips_life(char type)
{
    if (type == 'W')
        return 50;
    if (type == 'X')
        return 60;
    if (type == 'Y')
        return 70;
    if (type == 'Z')
        return 80;
    return 0;
}
int chips_money(char type)
{
    if (type == 'W')
        return 50;
    if (type == 'X')
        return 60;
    if (type == 'Y')
        return 70;
    if (type == 'Z')
        return 80;
    return 0;
}
int chips_attaque(Chips *chips)
{
    if(chips->type == 'W'){return 10;}
    if(chips->type == 'X'){return 20;}
    if(chips->type == 'Y'){return 30;}
    if(chips->type == 'Z'){return 40;}
    return 0;
}

Chips* Derniere_chips(Game* game)
{
    Chips* chips = game->chips;
    while (chips->next != NULL) {
        chips = chips->next;
    }
    return chips;
}

Bool Again_chips(Chips *chips)
{   
    if(chips->type != 'W' && chips->type != 'X' && chips->type != 'Y' && chips->type != 'Z'){return false;}
    else{return true;}
}

Chips* cree_chips()
{
    Chips* new_chips = (Chips*)malloc(sizeof(Chips));
    Case* new_type = (Case*)malloc(sizeof(Case));
    Case* new_coo = (Case*)malloc(sizeof(Case));
    clique_type(new_type);
    new_chips -> type = choix_type(new_type);
    if (Again_chips(new_chips) == false){return new_chips;}
    new_chips->life = chips_life(new_chips->type);
    clique_case(new_coo);
    if(new_coo->y >= 1)
    {
        new_chips -> line = new_coo -> y;
    }
    else
    {
        return new_chips;
    }
    new_chips -> position = new_coo -> x;
    new_chips->price = chips_money(new_chips->type);
    new_chips->next = NULL;
    return new_chips;
}


void Ajoute_Chips(Game* game,Chips *chips)
{
    if (game->chips != NULL)
    {
        Chips* last_chips = Derniere_chips(game);
        last_chips->next = chips;
    }
    else
    {
        game->chips = chips;
    }
}
void Suppresion_chips(Game *game,Chips *chips)
{
    if (game->chips == chips)
        {
            game->chips = chips->next;
        }
    else
    {
        Chips* actuel_chips = game->chips;
        while (actuel_chips != NULL && actuel_chips->next != chips) {
            actuel_chips = actuel_chips->next;
        }
        if (actuel_chips != NULL)
        {
            actuel_chips->next = chips->next;
            free(chips);
        }
    }
}

Bool Verification_money(Game* game,Chips* chips)
{
    if(game->money >= chips->price){return true;}
    if(game->money < chips->price){return false;}
    return false;
}
void Plateau_chips(Plateau *plateau,Game *game)
{
    Chips *actuel = game->chips;
    while(actuel != NULL)
    {
        if(actuel->position < LARGEUR && actuel->position >= 0)
        {
            plateau->plateau[actuel->line-1][actuel->position] = actuel->type;
        }
        actuel = actuel->next;
    }
}

void chips_part(Game *game,Plateau *plateau)
{
    Chips* new_chips = (Chips*)malloc(sizeof(Chips));
    while(game->money > 50)
    {
        new_chips = cree_chips();
        if (Again_chips(new_chips) == false)
        {
            return;
        }
        while(Verification_money(game,new_chips) == false)
        {
            new_chips = cree_chips();
        }
        game->money -= new_chips->price;
        Ajoute_Chips(game,new_chips);
        affiche_chips(game);
        information_plateau(game);
        MLV_actualise_window();
    }
}
void chips_action(Game *game)
{
    Chips *actuel_chips = game->chips;
    while(actuel_chips != NULL)
    {
        Virus *actuel_virus = game->virus;
        while(actuel_virus != NULL)
        {
            if(actuel_virus->position <= LARGEUR)
            {
                if(actuel_virus->position > actuel_chips->position && actuel_virus->line == actuel_chips->line)
                {
                    if(actuel_virus->prev_line == NULL)
                    {
                        actuel_virus->life -= chips_attaque(actuel_chips);
                    }
                }
            }
            actuel_virus = actuel_virus->next;
        }
        actuel_chips = actuel_chips->next;
    }
}
void delete_chips(Game *game)
{
    Chips *actuel = game->chips;
    while(actuel != NULL)
    {
        if(actuel->life <= 0)
        {
            Suppresion_chips(game,actuel);
        }
        actuel = actuel->next;
    }    
}
///
/////CONDITION DE VICTOIRE/////

Bool Chips_Victoire(Game *game)
{
    if(game == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Virus *actuel = game->virus;
    if(actuel != NULL)
    {
        return false;
    }
    return true;
}

Bool Virus_Victoire(Game *game)
{
    if(game == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Virus *actuel = game->virus;
    while(actuel != NULL)
    {
        if(actuel->position < 2)
        {
            return true;
        }
        actuel = actuel->next;
    }
    return false;
    printf("NULL\n");
}

Bool Fin_de_jeu(Game *game)
{
    if(Chips_Victoire(game) == true)
    {
        MLV_stop_music();
        MLV_Music* music = MLV_load_music("music/victoire.mp3");
        MLV_play_music(music,0.1,1);
        return true;
    }
    if(Virus_Victoire(game) == true)
    {
        MLV_stop_music();
        MLV_Music* music = MLV_load_music("muisc/defaite.mp3");
        MLV_play_music(music,0.1,1);
        return true;
    }
    return false;
}

///////////////////////
///// PARTIE ASCII ////
///////////////////////


void* money_start(const char* path,Game *game)
{
    FILE *fichier = fopen(path,"r");
    int piece;
    if(fichier == NULL)
        return NULL;
    fscanf(fichier,"%d",&piece);
    game->money = piece; 
    fclose(fichier);
    return NULL;
}
void tour_start(Game *game)
{
    game->turn = 0;
}
void* Creation_tableau(const char* path,Plateau *plateau,Virus *tab_virus)
{   
    FILE *fichier = fopen(path,"r");
    Virus* virus;
    Virus* prev_virus;
    int piece,turn,line,cmpt_virus = 0,c;
    char type;
    if(fichier == NULL){return NULL;}
    fscanf(fichier,"%d",&piece);
    for(int cmpt_virus=0;cmpt_virus<nb_virus_txt();cmpt_virus++)
    {
        fscanf(fichier,"%d %d %c", &turn,&line,&type);
        virus = Cree_Virus(type,line,turn);
        if(cmpt_virus != 0)
        {
            if(prev_virus->line == virus->line && prev_virus->position == virus->position)
            {
                break;
            }
        }
        prev_virus = virus;
        tab_virus[cmpt_virus] = *virus;
    }
    fclose(fichier);
    return NULL;
}


void changement_tour(Game *game){
    (*game).turn += 1;
}

int main()
{
    Case *zone;
    Plateau* plateau = (Plateau*)malloc(sizeof(Plateau));
    Game* game = (Game*)malloc(sizeof(Game));
    game->chips = NULL;
    game->virus = NULL;
    char path[11] = "level/level1.txt";
    Virus tab_virus[nb_virus_txt()];
    Cree_fenetre();
    MLV_init_audio();
    MLV_Music *music = MLV_load_music("music/pokemon_music.mp3");
    MLV_play_music(music,0.05,1);
    Creation_plateau();
    Creation_tableau(path,plateau,tab_virus);
    money_start(path,game);
    tour_start(game);
    delete_virus(game);
    chaine_virus(game,tab_virus);
    Plateau_ascii(plateau);
    chips_part(game,plateau);
    while(true)
    {
        delete_virus(game);
        delete_chips(game);
        Creation_plateau();
        affiche_virus(game);
        affiche_chips(game);
        Plateau_ascii(plateau);
        Plateau_chips(plateau,game);
        Plateau_virus(plateau,game);
        chips_action(game);
        delete_virus(game);
        virus_part(game);
        virus_action(game);
        changement_tour(game);
        MLV_actualise_window();
        sleep(1);
        if(Fin_de_jeu(game) == true)
        {
            Creation_plateau();
            affiche_virus(game);
            affiche_chips(game);
            MLV_actualise_window();
            sleep(1000000);
            MLV_stop_music();
            MLV_free_audio();
            break;
        }
    }
    return 0;
}