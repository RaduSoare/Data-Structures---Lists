
#ifndef TEAM_EXTRACTOR_H_D
#define TEAM_EXTRACTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FootballClub.h"
#define MAX_PLAYERS 1000
#define NUMBER_OF_POSITIONS 4

int compare_aux(const void* a, const void* b);
Player* push(Player* players, char* name, char* position, 
	int score, int injured);
Player* get_best_player(FootballClub *clubs, char *position);
Player* get_top_players(FootballClub *clubs, int N);
Player *get_players_by_score(FootballClub *clubs, int score);
Player* get_players_by_position(FootballClub *clubs, char *position);
Player* get_best_team(FootballClub *clubs);
int compare_psn(const void* a, const void* b);
Player* union_teams(FootballClub *clubs, char *club_A, char *club_B) ;


//structura auxiliara pentru a retine anumite date despre jucatori
typedef struct 
{
	int score_copy;
	char name_copy[LEN_NAME];
	char club_copy[LEN_NAME];
	char position_copy[LEN_NAME];
	int special_copy;
}TTeam;


Player* union_teams(FootballClub *clubs, char *club_A, char *club_B) 
{
	Player* union_list = NULL;
	//cauta cele 2 cluburi
	FootballClub* clubA = search_club(clubs,club_A);
	if(clubA == NULL)
		return NULL; 
	FootballClub* clubB = search_club(clubs,club_B);
	if(clubB == NULL)
		return NULL;
	//aloca dinamic un array pentru a retine temporar jucatorii
	TTeam* union_array = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(union_array == NULL) exit(-1);
	int number_of_players = 0;

	for(int i = 1; i <= 2; i++)
	{
		//itereaza prin ambele cluburi
		Player* current = NULL;
		if(i == 1 )
			current = clubA->players;
		if(i == 2)
			current = clubB->players;
		while(current)
		{	
			//retine datele jucatorilor intr-un array
			union_array[number_of_players].score_copy = current->score;
			strcpy(union_array[number_of_players].name_copy,current->name);
			strcpy(union_array[number_of_players].position_copy,
				current->position);
			if(i == 1)
			strcpy(union_array[number_of_players].club_copy,club_A);
			if(i == 2)
			strcpy(union_array[number_of_players].club_copy,club_B);
			union_array[number_of_players].special_copy = current->injured;
			++number_of_players; 
			
			current = current->next;
		}
	}
	//sorteaza array-ul dupa criteriile cerute
	qsort(union_array,number_of_players,sizeof(TTeam),compare_psn);
	for(int i = 0; i < number_of_players; i++)
	{
		//gaseste jucatorul respectiv
		Player* aux = search_player(clubs,union_array[i].club_copy,
			union_array[i].name_copy);
		//adauga jucatorul la sfarsitul listei
		union_list = push(union_list,aux->name, aux->position,
			aux->score,aux->injured); 
		
	}
	free(union_array);

	return union_list;

} 
Player* get_best_player(FootballClub *clubs, char *position) 
{
	if(clubs == NULL)
		return NULL;

	Player* best_player  = NULL;
	//aloca dinamic un array pentru a retine temporar jucatorii
	TTeam* best_player_array = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(best_player_array == NULL) exit(-1);
	int number_of_players = 0;

	FootballClub* current_club = clubs;
	while(current_club)
	{
		Player* current_player = current_club->players;
		while(current_player)
		{
			//retine datele jucatorilor de pe pozitia primita ca parametru
			if(strcmp(current_player->position,position) == 0)
			{
				best_player_array[number_of_players].score_copy 
					= current_player->score;
				strcpy(best_player_array[number_of_players].name_copy,
					current_player->name);
				strcpy(best_player_array[number_of_players].club_copy,
					current_club->name);
				best_player_array[number_of_players].special_copy 
					= current_player->injured;
				++number_of_players; 
			}
			current_player = current_player->next;
		}
		current_club = current_club->next;
	}
	if(number_of_players == 0)
	{
		free(best_player_array);
		return NULL;
	}
	//sorteaza array-ul
	qsort(best_player_array,number_of_players,sizeof(TTeam),compare_aux);
	//introduce in lista doar jucatorul cu scorul cel mai mare
	//adica primul din vector
	Player* aux = search_player(clubs,best_player_array[0].club_copy,
		best_player_array[0].name_copy);
	best_player = push(best_player,aux->name, aux->position,
		aux->score,aux->injured); 
	free(best_player_array);
	return best_player;

}
Player* get_top_players(FootballClub *clubs, int N) 
{
	if(clubs == NULL || N == 0)
		return NULL;

	Player* top_players = NULL;
	int number_of_players = 0;
	//aloca dinamic un array pentru a retine cei mai buni jucatori
	TTeam* array_of_top_players = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(array_of_top_players == NULL) exit(-1);

	FootballClub* current_club = clubs;
	while(current_club)
	{	
		Player* current_player = current_club->players;
		//aloca dinamic un array pentru a retine jucatorii din ficare club
		TTeam* array_of_players = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
		if(array_of_players == NULL) exit(-1);
		int temp_number_of_players = 0;
		while(current_player)
		{
			//retine datele jucatorilor din fiecare club
			array_of_players[temp_number_of_players].score_copy = 
				current_player->score;
			strcpy(array_of_players[temp_number_of_players].name_copy,
				current_player->name);
			strcpy(array_of_players[temp_number_of_players].club_copy,
				current_club->name);
			++temp_number_of_players; 
			
			current_player = current_player->next; 
		}
		
		//sorteaza array-ul de jucatori din fiecare club
		qsort(array_of_players,temp_number_of_players,
			sizeof(TTeam),compare_aux);
		//verifica daca se gasesc mai putin jucatori de N in array
		int copy_N = N;
		if(temp_number_of_players < N )
			copy_N = temp_number_of_players; 
		//ia in vectorul final doar primii N jucatori de la fiecare club
		for(int i = 0; i < copy_N; i++)
		{
			array_of_top_players[number_of_players] = array_of_players[i];
			number_of_players++;
		}

		free(array_of_players); 
		current_club = current_club->next;

	}
	//sorteaza array-ul de cei mai buni jucatori
	qsort(array_of_top_players,number_of_players,sizeof(TTeam),compare_aux);
	for(int i = 0; i < number_of_players; i++)
	{
		//cauta jucatorul si il insereaza in lista de cei mai buni jucatori
		Player* aux = search_player(clubs,array_of_top_players[i].club_copy,
			array_of_top_players[i].name_copy);
		top_players = push(top_players,aux->name, aux->position,
			aux->score,aux->injured);
	}
	
	free(array_of_top_players); 
	
	return top_players;


}

Player *get_players_by_score(FootballClub *clubs, int score) 
{
	if(clubs == NULL || score < -100 || score > 100)
		return NULL;
	Player* score_list = NULL;
	//aloca dinamic un array pentru a retine temporar jucatorii
	//cu scorul mai mare de "score"
	TTeam* score_array = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(score_array == NULL) exit(-1);
	int number_of_players = 0;
	FootballClub* current_club = clubs;
	while(current_club)
	{

		for(int i = 1; i <= 2; i++)
		{	
			//itereaza atat prin lista de jucatori 
			//cat si prin lista de accidentati
			Player* current_player = NULL;
			if(i == 1)
				current_player = current_club->players;
			else if(i == 2)
				current_player = current_club->injured_players;
			while(current_player)
			{
				if(current_player->score >= score)
				{	
					//retine datele jucatorilor cu scor mai mare de "score"
					score_array[number_of_players].score_copy =
						current_player->score;
					strcpy(score_array[number_of_players].name_copy,
						current_player->name);
					strcpy(score_array[number_of_players].club_copy,
						current_club->name);
					++number_of_players;
				}
				current_player = current_player->next;
			}
		}
		current_club = current_club->next;
	}
	//sorteaza array-ul
	qsort(score_array,number_of_players,sizeof(TTeam),compare_aux);
	for(int i = 0; i < number_of_players; i++)
	{
		//adauga in lista toti jucatorii cu scorul cerut
		Player* aux = search_player(clubs,score_array[i].club_copy,
			score_array[i].name_copy);
		score_list = push(score_list,aux->name,aux->position,
			aux->score,aux->injured);
		
	}
	free(score_array);

	return score_list;
} 

Player* get_players_by_position(FootballClub *clubs, char *position)
{
	if(clubs == NULL)
		return NULL;
	Player* position_list = NULL;
	//aloca dinamic un array pentru a retine temporar jucatorii
	//de pe pozitia "position"
	TTeam* position_array = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(position_array == NULL) exit(-1);
	int number_of_players = 0;
	FootballClub* current_club = clubs;
	
	while(current_club)
	{
		for(int i = 1; i <= 2; i++)
		{	
			Player* current_player = NULL;
			if(i == 1)
				current_player = current_club->players;
			else if(i == 2)
				current_player = current_club->injured_players;
			while(current_player)
			{
				if(strcmp(current_player->position, position) == 0)
				{	
					//retine datele jucatorilor de pe pozitia "position"
					position_array[number_of_players].score_copy =
						current_player->score;
					strcpy(position_array[number_of_players].name_copy,
						current_player->name);
					strcpy(position_array[number_of_players].club_copy,
						current_club->name);
					++number_of_players;
				}
				current_player = current_player->next;
			}
		}

		current_club = current_club->next;
	}
	
	qsort(position_array,number_of_players,sizeof(TTeam),compare_aux);
	for(int i = 0; i < number_of_players; i++)
	{
		//adauga in lista toti jucatorii de pe pozitia "position"
		Player* aux = search_player(clubs,position_array[i].club_copy,
			position_array[i].name_copy);
		position_list = push(position_list,aux->name,
			aux->position,aux->score,aux->injured);
	}
	free(position_array);

	return position_list;
}

Player* get_best_team(FootballClub *clubs) 
{
	if(clubs == NULL)
		return NULL;
	
	Player* best_team_list = NULL;
	//aloca dinamic un array pentru a retine temporar jucatorii
	TTeam* best_team_array = (TTeam*)malloc(MAX_PLAYERS * sizeof(TTeam));
	if(best_team_array == NULL) exit(-1);
	int number_of_players = 0;
	//creeaza un array de cuvinte in care stocheaza denumirile pozitiilor 
	char** positions = calloc(4, sizeof(char*));
	if(positions == NULL) exit(-1);
	for(int i = 0 ; i < 4; i++)
	{
		positions[i] = calloc(20, sizeof(char));
		if(positions[i] == NULL) 
		{
			for(int j = 0; j < i ; j++)
				free(positions[j]);
			exit(-1);
		}
	}
	strcpy(positions[0],"portar"); 
	strcpy(positions[1],"fundas");
	strcpy(positions[2],"mijlocas"); 
	strcpy(positions[3],"atacant");
	//creeaza un array cu numarul de jucatori ce trebuie extras
	//din fiecare pozitie
	int positions_needed[] = {1,4,3,3};

	char **position_aux = calloc(1, sizeof(char*));
	if(position_aux == NULL) exit(-1);
	position_aux[0] = calloc(20, sizeof(char));
	if(position_aux[0] == NULL) exit(-1);
	//creeaza un array de cluburi
	FootballClub** temp_club = (FootballClub**)malloc
		(NUMBER_OF_POSITIONS * sizeof(FootballClub*));
	if(temp_club == NULL) exit(-1);

	

	for(int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		//creeaza o lista cu toti jucatorii de pe pozitia i
		Player* list_of_players = get_players_by_position(clubs, positions[i]);
		strcpy(position_aux[0],positions[i]);
		//creeaza un club in care sa fie retinuta lista de jucatori creata
		temp_club[i] = initialize_clubs(1,position_aux);
		temp_club[i]->players = list_of_players;
		//creeaza o lista cu cei mai buni jucatori din lista creata
		Player* top_players = get_top_players(temp_club[i],
			positions_needed[i]);
		Player* temp = NULL;
		while(top_players)
		{
			//retine datele jucatorilor 
			best_team_array[number_of_players].score_copy =
				top_players->score;
			strcpy(best_team_array[number_of_players].name_copy,
				top_players->name);
			strcpy(best_team_array[number_of_players].club_copy,
				temp_club[i]->name);
			best_team_array[number_of_players].special_copy = i;
			++number_of_players;
			
			temp = top_players;
			top_players = top_players->next;
			free(temp->name);
			free(temp->position);
			free(temp);

		}  		


	} 
	//sorteaza array-ul de cei mai buni jucatori
	qsort(best_team_array,number_of_players,sizeof(TTeam),compare_aux);
	for(int i = 0; i < number_of_players; i++)
	{
		//adauga in lista jucatorii
		Player* aux = search_player(temp_club[best_team_array[i].special_copy],
			best_team_array[i].club_copy,best_team_array[i].name_copy);
		best_team_list = push(best_team_list,aux->name,aux->position,
			aux->score,aux->injured); 
		
		
	}
	
	free(position_aux[0]);
	free(position_aux);
	for (int i = 0; i < NUMBER_OF_POSITIONS; i++) {
		free(positions[i]);
	}
	free(positions);
	free(best_team_array);
	for(int i = 0; i < NUMBER_OF_POSITIONS; i++)
	{
		destroy_club_list(temp_club[i]); 
	}

	free(temp_club); 

	return best_team_list; 


}
int compare_psn(const void* a, const void* b)
{
	//compara crescator dupa pozitie, descrescator dupa scor
	//crescator dupa nume  
	TTeam *x = (TTeam*)a;
	TTeam *y = (TTeam*)b;
	if(strcmp(x->position_copy,y->position_copy) != 0)
		return strcmp(x->position_copy,y->position_copy);
	else
		if(y->score_copy != x->score_copy)
			return (y->score_copy - x->score_copy);
	else
	 return strcmp(x->name_copy,y->name_copy);
	


}
int compare_aux(const void* a, const void* b)
{
	//compara descrescator dupa scor si crescator dupa nume
	TTeam *x = (TTeam*)a;
	TTeam *y = (TTeam*)b;
	if(y->score_copy != x->score_copy)
		return y->score_copy - x->score_copy;

	return strcmp(x->name_copy,y->name_copy);
}

Player* push(Player* players, char* name, char* position, 
	int score, int injured)
{
	//adauga jucatori la sfarsitul unei liste
	Player* new_player = (Player*)malloc(sizeof(Player));
	if(new_player == NULL) exit(-1);
	new_player->name = (char*)malloc(LEN_NAME * sizeof(char));
	if(new_player->name == NULL) exit(-1);
	strcpy(new_player->name,name);
	new_player->position = (char*)malloc(LEN_POS * sizeof(char));
	if(new_player->position == NULL) exit(-1);
	strcpy(new_player->position,position);
	new_player->score = score;
	new_player->injured = injured;
	new_player->next = new_player->prev = NULL; 

	
	if(players == NULL)
	{
		new_player->prev = NULL;
		players = new_player;
		return players;
	}
	else
	{	
		Player* temp = NULL;
		if(players != NULL)
		  temp = players;
		
		while(temp->next != NULL)
			temp = temp->next;
		
		temp->next = new_player;
		new_player->prev = temp;
		new_player->next = NULL;
		return players;
	} 
	return NULL;




}

#endif // TEAM_EXTRACTOR_H_INCLUDED