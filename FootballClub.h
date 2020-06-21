#ifndef FOOTBALL_CLUB_H_D
#define FOOTBALL_CLUB_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_NAME 20
#define LEN_POS 10
#define MAX_SCORE 100
#define MIN_SCORE -100

// List node which contains information about one football player.
typedef struct Player {
	char *name;					// player's name
	char *position;				// player's game position
	int score;					// player's score
	int injured;				// indicator for injury (1 true, 0 false)
	struct Player *next;		// next list node
	struct Player *prev;		// previous list node
} Player;

// Structure which contains information about one football club.
typedef struct FootballClub {
	char *name;					// club's name
	Player *players;			// list with players in good shape
	Player *injured_players;	// list with injured players
	struct FootballClub *next;	// next list node
} FootballClub;

FootballClub *initialize_clubs(int clubs_no, char **names);
FootballClub *add_club(FootballClub *clubs, char *name);
FootballClub* search_club(FootballClub *clubs, char *club_name);
void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score);
int search(FootballClub *clubs, char *player_name,char *club_name);
Player* search_player(FootballClub *clubs, char *club_name,char *player_name);
void remove_player(FootballClub *clubs, char *club_name, char *player_name);
void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no);
void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club);
void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score);
void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score);
void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name);
void destroy_player_list(Player *player);
void destroy_club_list(FootballClub *clubs);





FootballClub *initialize_clubs(int clubs_no, char **names) 
{
	//caz in care primeste input nul
	if(clubs_no == 0 || names == NULL)
		return NULL;
	FootballClub* clubs = (FootballClub*)malloc(sizeof(FootballClub));
	if(clubs == NULL) exit(-1);
	for(int i = 0; i < clubs_no; i++) 	
	{	
		//caz in care lista e goala
		if(i == 0)
		{	
			clubs->name = (char*)malloc(LEN_NAME * sizeof(char));
			if(clubs->name == NULL) exit(-1);
			strcpy(clubs->name,names[i]);
			clubs->players = NULL;
			clubs->injured_players = NULL;
			clubs->next = NULL;
		}
		else
			if(i != 0)
			{
				//aloca un club nou
				FootballClub* new_club = 
					(FootballClub*)malloc(sizeof(FootballClub));
				if(new_club == NULL) exit(-1);
				new_club->name = (char*)malloc(LEN_NAME * sizeof(char));
				if(new_club->name == NULL) exit(-1);
				strcpy(new_club->name,names[i]);
				new_club->players = NULL;
				new_club->injured_players = NULL;
				new_club->next = NULL;

				//realizeaza adugarea la sfarsitul listei
				FootballClub* temp = clubs;
				while(temp->next != NULL)
					temp = temp->next;
				temp->next = new_club;
				new_club->next = NULL;
			}
		
	}
				
	return clubs;


}



FootballClub *add_club(FootballClub *clubs, char *name) 
{
	//aloca club nou
	FootballClub* new_club = (FootballClub*)malloc(sizeof(FootballClub));
	if(new_club == NULL) exit(-1);
	new_club->name = (char*)malloc(LEN_NAME * sizeof(char));
	if(new_club->name == NULL ) exit(-1);
	strcpy(new_club->name , name); 
	new_club->players = NULL;
	new_club->injured_players = NULL;
	new_club->next = NULL;
	FootballClub* temp = NULL;
	//caz in care lista e goala
	if(clubs == NULL)
	{
		clubs = new_club;
		new_club->next = NULL; 
		return clubs;
	}
	//itereaza pana la sfarsitul listei si adauga jucatorul
	else
	{
	temp = clubs;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = new_club;
	return clubs;
	}
	return NULL;
		

} 

void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score) 
{
	if(clubs == NULL)
		return;
	//aloca un jucator nou
	Player* new_player = (Player*)malloc(sizeof(Player));
	if(new_player == NULL) exit(-1);
	new_player->name = (char*)malloc(LEN_NAME * sizeof(char));
	if(new_player->name == NULL) exit(-1);
	strcpy(new_player->name,player_name);
	new_player->position = (char*)malloc(LEN_POS * sizeof(char));
	if(new_player->position == NULL) exit(-1);
	strcpy(new_player->position,position);
	new_player->score = score;
	new_player->injured = 0;
	new_player->next = new_player->prev = NULL;
	
	FootballClub *current = clubs;
	//gaseste clubul primit ca parametru
	while(current != NULL && strcmp(club_name, current->name) != 0) 
	{
		current = current->next;
	}
	//daca clubul nu exista se elibereaza memoria alocata
	if(current == NULL)
	{	free(new_player->name);
		free(new_player->position);
		free(new_player);
		return;
	}
	//caz in care clubul este gol
	if(current->players == NULL)
	{
		new_player->prev = NULL;
		current->players = new_player;
		return;
	} 
	Player* temp = current->players;
	//cauta locul in care jucatorul trebuie adaugat in functie de pozitie
	while(temp->next != NULL)
	{
		if(strcmp(temp->next->position,position) >= 0)
			break;
		temp = temp->next;
	}
	
	if(temp->next != NULL && strcmp(temp->next->position,position) == 0)
	{	
		//cauta locul in care jucatorul trebuie adaugat in functie de scor
		while(temp->next != NULL && strcmp(temp->next->position,position) == 0
			&& temp->next->score > score )
		{ 
			temp = temp->next;
			
		}
	}

	if(temp->next != NULL && temp->next->score == score )
	{
		//cauta locul in care jucatorul trebuie adaugat in functie de nume
		while(temp->next != NULL && strcmp(temp->next->position,position) == 0 
			&& temp->next->score == score && 
			strcmp(temp->next->name,player_name) < 0)
		{
			temp = temp->next;
			
		}
	}
	//cazul in care jucatorul trebuie adaugat pe prima pozitie din lista
	if(temp->prev == NULL && (strcmp(temp->position,position) > 0
		|| (strcmp(temp->position,position) == 0 && temp->score < score)
		||(strcmp(temp->position,position) == 0 && temp->score == score 
			&& strcmp(temp->name,player_name) > 0)) )
	{
		new_player->prev = NULL;
		new_player->next = temp;
		temp->prev = new_player;
		current->players = new_player;
		return;
	} 

	//inserare jucator in pozitie intermediara
	
	new_player->next = temp->next;
	temp->next = new_player;
	new_player->prev = temp;
	if(new_player->next != NULL)
		new_player->next->prev = new_player;
	return; 


}
void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club) 
{
	//cauta jucatorul ce trebuie transferat
	Player* player = search_player(clubs,old_club,player_name);
	if(player == NULL)
		return;
	//verifica starea de sanatate a jucatorului
	int check_injured = search(clubs,player->name,old_club);
	//creeaza copii ale datelor jucatorului pentru ca nu fi pierdute la remove
	Player* aux = player;
	char* name_temp = (char*)malloc(LEN_NAME * sizeof(char));
	if(name_temp == NULL) exit(-1);
	char* position_temp = (char*)malloc(LEN_POS * sizeof(char));
	if(position_temp == NULL) exit(-1);
	int score_temp = aux->score;
	strcpy(name_temp,aux->name);
	strcpy(position_temp,aux->position);
	//sterge jucatorul din clubul vechi
	remove_player(clubs,old_club,player->name);	
	//insreaza jucatorul in functie de starea de sanatate
	if(check_injured == 1)
		add_player(clubs,new_club,name_temp,position_temp,score_temp);
	if(check_injured == 0)
	{
		add_player(clubs,new_club,name_temp,position_temp,score_temp);
		add_injury(clubs, new_club, name_temp, 0);
	}
	

	free(name_temp);
	free(position_temp);

	
}

void remove_player(FootballClub *clubs, char *club_name, char *player_name) 
{
	FootballClub *current = clubs;
	//cauta clubul din care trebuie sters jucatorul
	while(current != NULL && strcmp(club_name, current->name) != 0 )
		current = current->next;
	if(current == NULL)
			return;
	//verifica daca jucatorul este in lista de jucatori sau de accidentati
	Player *player = NULL; 
	if(search(clubs,player_name,club_name) == 1)
		player = current->players;
	else
		if(search(clubs,player_name,club_name) == 0)
			player = current->injured_players;
	Player *toBeDeleted = NULL;
	//cauta jucatorul in lista corespunzatoare
	while(player != NULL && strcmp(player_name, player->name) != 0)
		player= player->next;
	if(player == NULL)
		return;
	//caz in care este singurul jucator din lista
	if(player->prev == NULL && player->next == NULL)
	{
		toBeDeleted = player;
		if(search(clubs,player_name,club_name) == 1)
			current->players = NULL;
		else
			if(search(clubs,player_name,club_name) == 0)
				current->injured_players = NULL;
		free(toBeDeleted->name);
		free(toBeDeleted->position);
		free(toBeDeleted);
		return;

	}
	//caz in care este primul jucator din lista
	if(player->prev == NULL)
	{
		toBeDeleted = player;
		player->next->prev = NULL;
		if(search(clubs,player_name,club_name) == 1)
			current->players = player->next;
		else
			if(search(clubs,player_name,club_name) == 0)
				current->injured_players = player->next;
		free(toBeDeleted->name);
		free(toBeDeleted->position);
		free(toBeDeleted);
		return;
	}
	//caz in care este ultimul jucator din lista
	if(player->next == NULL)
	{
		toBeDeleted = player;
		player->prev->next = NULL;
		free(toBeDeleted->name);
		free(toBeDeleted->position);
		free(toBeDeleted);
		return;
	}
	//celelalte cazuri
	toBeDeleted = player;
	player->prev->next = player->next;
	player->next->prev = player->prev;
	free(toBeDeleted->name);
	free(toBeDeleted->position);
	free(toBeDeleted);
	return;
	
}
void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score) 
{
	//cauta jucatorul
	Player* player = search_player(clubs,club_name,player_name);
	if(player == NULL)
		return;
	//verifica daca scorul se afla in intervalul cerut
	if(score > MAX_SCORE)
		score = MAX_SCORE;
	if(score < MIN_SCORE)
		score = MIN_SCORE;
	player->score = score;
	//verifica starea de sanatate a jucatorului
	int check_injured = search(clubs,player->name,club_name);
	//creeaza copii pentru datele jucatorului
	char* name_temp = (char*)malloc(LEN_NAME * sizeof(char));
	if(name_temp == NULL) exit(-1);
	strcpy(name_temp,player->name);
	char* position_temp = (char*)malloc(LEN_POS * sizeof(char));
	if(position_temp == NULL) exit(-1);
	strcpy(position_temp,player->position);
	int score_temp = player->score;
	
	
	//realizeaza update-ul scorului
	remove_player(clubs, club_name, player_name); 
	if(check_injured == 1)
		add_player(clubs, club_name, name_temp, position_temp, score_temp);
	if(check_injured == 0)
	{
		add_player(clubs, club_name, name_temp, position_temp, score_temp);
		add_injury(clubs, club_name, name_temp, 0);
	}	

	free(name_temp);
	free(position_temp);
	
}
void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score) 
{
	//cauta jucatorul
	Player* player = search_player(clubs,club_name,player_name);
	if(player == NULL)
		return;
	player->score = score;
	strcpy(player->position, position); 
	//verifica starea de sanatate a jucatorului
	int check_injured = search(clubs,player->name,club_name);
	//creeaza copii pentru datele jucatorului
	char* name_temp = (char*)malloc(LEN_NAME * sizeof(char));
	if(name_temp == NULL) exit(-1);
	strcpy(name_temp,player->name);
	char* position_temp = (char*)malloc(LEN_POS * sizeof(char));
	if(position_temp == NULL) exit(-1);
	strcpy(position_temp,player->position);
	//realizeaza update-ul pozitiei
	remove_player(clubs, club_name, player_name); 
	if(check_injured == 1)
		add_player(clubs, club_name, name_temp, position_temp, score);
	if(check_injured == 0)
	{
		add_player(clubs, club_name, name_temp, position_temp, score);
		add_injury(clubs, club_name, name_temp, 0);
	}
	free(name_temp);
	free(position_temp);


}




void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no) 
{
	if(clubs == NULL)
		return;
	//cauta jucatorul
	Player* player = search_player(clubs,club_name,player_name);
	if(player == NULL)
		return;
	//aloca un nou jucator
	Player* new_player = (Player*)malloc(sizeof(Player));
	if(new_player == NULL) exit(-1);
	//calcul scor
	new_player->score = player->score - 0.1 * days_no;
	if(new_player->score > MAX_SCORE) new_player->score = MAX_SCORE;
	if(new_player->score < MIN_SCORE) new_player->score = MIN_SCORE;
	new_player->injured = 1;
	new_player->position = (char*)malloc(LEN_POS * sizeof(char));
	if(new_player->position == NULL) exit(-1);
	strcpy(new_player->position,player->position);
	new_player->name = (char*)malloc(LEN_NAME * sizeof(char));
	if(new_player->name == NULL) exit(-1);
	strcpy(new_player->name,player_name);
	new_player->next = new_player->prev = NULL;

	FootballClub *current = clubs;
	//gaseste clubul la care trebuie adaugat
	while(strcmp(club_name, current->name) != 0 && current != NULL)
	{
		current = current->next;
	}
	if(current == NULL)
		return;
	//caz in care lista de accidentati e goala
	if(current->injured_players == NULL)
	{
		new_player->prev = NULL;
		current->injured_players = new_player;
		remove_player(clubs, club_name, player_name);
		return;
	}
	Player* temp = current->injured_players;
	//cauta locul de inserare
	while(current != NULL && temp->next != NULL)
	{
		if(strcmp(temp->next->name,player_name) >= 0)
			break;
		temp = temp->next;
	}
	
	//caz in care inserarea se face pe prima pozitie
	if(temp->prev == NULL && (strcmp(temp->name,player_name) > 0))
	{
		new_player->prev = NULL;
		new_player->next = temp;
		temp->prev = new_player;
		current->injured_players = new_player;
		remove_player(clubs, club_name, player_name);
		return;	

	} 
	//celelalte cazuri
	new_player->next = temp->next;
	temp->next = new_player;
	new_player->prev = temp;
	if(new_player->next != NULL)
		new_player->next->prev = new_player;
	remove_player(clubs, club_name, player_name);
	return;
	
}



void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name) 
{
	if(clubs == NULL)
		return;
	//cauta clubul
	FootballClub *club = search_club(clubs, club_name);
	if(club == NULL)
		return;	
	//cauta jucatorul
	Player* player = search_player(clubs,club_name,player_name);
	if(player == NULL)
		return;
	Player* aux = player;
	//realizez copii ale datelor jucatorului
	char* name_temp = (char*)malloc(LEN_NAME *  sizeof(char));
	if(name_temp == NULL) exit(-1);
	char* position_temp = (char*)malloc(LEN_POS *  sizeof(char));
	if(position_temp == NULL) exit(-1);
	strcpy(name_temp,aux->name); 
	strcpy(position_temp,aux->position); 
	int score_temp = aux->score;
	//modific indicatorul de stare
	aux->injured = 0; 
	//elimin din lista de aciidentati
	remove_player(clubs, club->name, player->name);
	//adaug in lista de jucatori
	add_player(clubs, club->name, name_temp, position_temp, score_temp); 
	free(name_temp);
	free(position_temp);
	
	
}

// Frees memory for a list of Player.
void destroy_player_list(Player *player) 
{
	if(player == NULL)
		return;
	Player *head = player, *aux = NULL;
	while(head)
	{
		aux = head->next;
		free(head->name);
		free(head->position);
		free(head);
		head = aux;
	} 
}

// Frees memory for a list of FootballClub.
void destroy_club_list(FootballClub *clubs) 
{
	if(clubs == NULL)
		return;
	FootballClub *head = clubs, *aux = NULL;
	while(head != NULL)
	{
		aux = head->next;
		destroy_player_list(head->players);
		destroy_player_list(head->injured_players);
		free(head->name);
		free(head);
		head = aux;

	} 
	
}
FootballClub* search_club(FootballClub *clubs, char *club_name)
{
	FootballClub *club = clubs;
	//gaseste clubul primit ca parametru
	while(club != NULL && strcmp(club_name, club->name) != 0 )
		club = club->next;
	if(club == NULL)
			return NULL;
	return club;

}

int search(FootballClub *clubs, char *player_name,char *club_name)
{
	//verifica in ce lista se afla un jucator
	FootballClub *club = search_club(clubs, club_name);
	if(club == NULL) exit(-1);
	Player *player = club->players;
	while(player != NULL && strcmp(player_name, player->name) != 0)
		player= player->next;
	if(player != NULL)
		return 1;
	else
	{
		player = club->injured_players;
		while(player != NULL && strcmp(player_name, player->name) != 0)
			player= player->next;
		if(player != NULL)
			return 0;
	}
	return -1;
	

}
Player* search_player(FootballClub *clubs, char *club_name,char *player_name)
{
	FootballClub *club = search_club(clubs, club_name);
	if(club == NULL)
		return NULL;
	Player *player = NULL;	
	//cauta in lista de jucatori
	if(search(clubs,player_name,club_name) == 1)
		player = club->players;
	//cauta in lista de accidentati
	else if(search(clubs,player_name,club_name) == 0)
		player = club->injured_players;
	//gaseste jucatorul
	while(player != NULL && strcmp(player_name, player->name) != 0)
		player= player->next;
	if(player == NULL)
		return NULL; 
	return player;

}

// Displays a list of players.
void show_list(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	while (player) {
		fprintf(f, "(%s, %s, %d, %c) ", 
			player->name,
			player->position,
			player->score,
			player->injured ? 'Y' : '_');
		player = player->next;
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}

// Displays a list of players in reverse.
void show_list_reverse(FILE *f, Player *players, int free_memory) {
	fprintf(f, "P: ");
	Player *player = players;
	if (player) {
		while (player->next) {
			player = player->next;
		}
		while (player) {
			fprintf(f, "(%s, %s, %d, %c) ", 
				player->name,
				player->position,
				player->score,
				player->injured ? 'Y' : '_');
			player = player->prev;
		}
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}


// Displays information about a football club.
void show_clubs_info(FILE *f, FootballClub *clubs) {
	fprintf(f, "FCs:\n");
	while (clubs) {
		fprintf(f, "%s\n", clubs->name);
		fprintf(f, "\t");
		show_list(f, clubs->players, 0);
		fprintf(f, "\t");
		show_list(f, clubs->injured_players, 0);
		clubs = clubs->next;
	}
}

#endif // FOOTBALL_CLUB_H_INCLUDED