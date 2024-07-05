#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <unistd.h>

#define LARGEUR 16
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
    Virus* virus;
}Prev_vague;

typedef struct{
    char plateau[HAUTEUR][LARGEUR];
}Plateau;

typedef enum
{
    false,
    true
}Bool;

void Plateau_ascii(Plateau *plateau)
{
    for(int i = 0;i < HAUTEUR ;i++){
        for(int j = 1;j< LARGEUR ;j++){
            plateau->plateau[i][j] = ' ' ;
        }
    }
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
void chaine_virus(Game *game,Game *prev_vague,Virus *tab_virus)
{
    for(int i = 0;i<nb_virus_txt()-1;i++)
    {
        int type = tab_virus[i].type;
        int line = tab_virus[i].line;
        int turn = tab_virus[i].turn;
        int position = LARGEUR + turn;
        int life = virus_life(type);
        int speed = virus_speed(type);
        Ajoute_virus(game,type,life,line,position,speed,turn);
        Ajoute_virus(prev_vague,type,life,line,position-LARGEUR,speed,turn);
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
            printf("YES 1\n");
            return true;
        }
        actuel_chips = actuel_chips->next;
    }
    printf("YES 2\n");
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
                    if (actuel->position < 0){actuel->position = 0;}
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
    if (type == 'V')
        return 50;
    if (type == 'W')
        return 120;
    if (type == 'X')
        return 70;
    if (type == 'Y')
        return 80;
    if (type == 'Z')
        return 90;
    return 0;
}
int chips_money(char type)
{
    if (type == 'W')
        return 10;
    if (type == 'X')
        return 20;
    if (type == 'Y')
        return 30;
    if (type == 'Z')
        return 50;
    return 0;
}
int chips_attaque(Chips *chips)
{
    if(chips->type == 'V'){return 10;}
    if(chips->type == 'W'){return 0;}
    if(chips->type == 'X'){return 20;}
    if(chips->type == 'Y'){return 40;}
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
    if(chips->type!='W'&&chips->type!='X'&&chips->type!='Y'&&chips->type!='Z'&&chips->type!='V')
    {return false;}
    else
    {return true;}
}

Chips* cree_chips()
{
    Chips* new_chips = (Chips*)malloc(sizeof(Chips));
    printf("Type : ");
    scanf(" %c",&(new_chips->type));
    if (Again_chips(new_chips) == false){return new_chips;}
    new_chips->life = chips_life(new_chips->type);
    printf("Ligne : ");
    scanf(" %d",&(new_chips->line));
    printf("Position : ");
    scanf(" %d",&(new_chips->position));
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

void affichage_chips()
{
    printf("Quelles Pokemon voulez vous construire ?\n");
    printf("Pikachu : V\n");
    printf("carapuce : W\n");
    printf("Salameche: X\n");
    printf("bulbizare : Y\n");
    printf("Sacha : Z\n");
    printf("Pas de tourelle : S\n\n");
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
    while(game->money > 10)
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
        if(actuel->position == 0)
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
        printf("Les Chips ont Gagné !\n");
        return true;
    }
    if(Virus_Victoire(game) == true)
    {
        printf("Les virus ont Gagné dommage...\n");
        return true;
    }
    return false;
}

///////////////////////
///// PARTIE ASCII ////
///////////////////////


void affichage_plateau(Plateau *plateau,Game *game)
{
    printf("Tour : %d Argent : %d\n",game->turn,game->money);
    for(int k = 0;k< LARGEUR ;k++)
    {
            printf("%2d|",k);
    }
    printf("\n");
    for(int i = 0;i < HAUTEUR ;i++){
        printf("%d|",i+1);
        for(int j = 1;j< LARGEUR ;j++){
            printf("%3c",plateau->plateau[i][j]);
        }
        printf("|");
        printf("\n");
    }
}


void Plateau_prev_vague(Plateau *plateau,Prev_vague *prev_vague)
{
    Virus *actuel = prev_vague->virus;
    while(actuel != NULL)
    {   
        plateau->plateau[actuel->line-1][actuel->position] = actuel->type;
        actuel = actuel->next;
    }
}  

//////////////////////////

void* money_start(const char* path,Game *game)
{
    FILE *fichier = fopen(path,"r");
    int piece;
    if(fichier == NULL){return NULL;}
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


/////////////////////MAIN////////////////////

int main()
{
    Plateau* plateau = (Plateau*)malloc(sizeof(Plateau));
    Game* game = (Game*)malloc(sizeof(Game));
    Game* prev_vague = (Game*)malloc(sizeof(Game));
    game->chips = NULL;
    game->virus = NULL;
    char path[11] = "level/level1.txt";
    
    Virus tab_virus[nb_virus_txt()];
    Creation_tableau(path,plateau,tab_virus);
    money_start(path,game);
    tour_start(game);
    chaine_virus(game,prev_vague,tab_virus);
    Plateau_ascii(plateau);
    delete_virus(prev_vague);
    Plateau_virus(plateau,prev_vague);
    affichage_plateau(plateau,game);
    affichage_chips();
    chips_part(game,plateau);
    while(true)
    {
        delete_virus(game);
        delete_chips(game);
        Plateau_ascii(plateau);
        Plateau_chips(plateau,game);
        Plateau_virus(plateau,game);
        chips_action(game);
        virus_part(game);
        virus_action(game);
        changement_tour(game);
        /*
        printf("\e[1;1H\e[2J");
        */
        sleep(1);
        affichage_plateau(plateau,game);
        if(Fin_de_jeu(game) == true)
        {
            break;
        }
    }
    return 0;
}