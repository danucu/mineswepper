#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define DIMBOARD 15 //+extra 2 row si 2 cols
#define TRUE 1
#define FALSE 0
 
char minesboard[DIMBOARD][DIMBOARD];
char cleanboard[DIMBOARD][DIMBOARD];
char FLAG = 'F';
 
void create_cleanboard();
void create_board_without_mines();
void show_cleanboard();
void show_board_without_mines();
int findneighborsmines(int, int);
void flushoutneighborsmines(int, int);
void setFlag();
void lostgame();
void wingame();
int checkforwingame();
void playagain();
void startgame();
 
int main()
{
	startgame();
	return 0;
}
 
 //++++++ afisare tabla cu mine ++++++
 void show_cleanboard()
{
	int i, j;
 
	// afisarea liniei cu numere
	printf("  ");
	for(i = 1; i < DIMBOARD - 1; i++)
		printf("%d ", i);
	printf("\n");
 
	// afisarea coloanelor cu numere
	for(i = 0; i < DIMBOARD; i++)
	{
		for(j = 0; j < DIMBOARD; j++)
			printf("%c ", minesboard[i][j]);
		if(i > 0 &&  i < DIMBOARD - 1)
			printf("%d ", i);
		printf("\n");
	} 
}
 
 // +++++ tabla implicita fara mine ++++++
void show_board_without_mines()
{
	int i, j;
 
	system("cls");
	// afisarea liniei cu numere
	printf("  ");
	for(i = 1; i < DIMBOARD - 1; i++)
		printf("%d ", i);
	printf("\n");
 
	// ascumderea primei si ultimei linii si a primei si ultimei coloane
	for(i = 0; i < DIMBOARD; i++)
		for(j = 0; j < DIMBOARD; j++)
			if(i == 0 || i == DIMBOARD - 1)
				cleanboard[i][j] = ' ';
 
	for(j = 0; j < DIMBOARD; j++)
		for(i = 0; i < DIMBOARD; i++)
			if(j == 0 || j == DIMBOARD - 1)
				cleanboard[i][j] = ' ';
 
	// afisarea coloanei cu numere
	for(i = 0; i < DIMBOARD; i++)
	{
		for(j = 0; j < DIMBOARD; j++)
		{
			printf("%c ", cleanboard[i][j]);
		}
		if(i > 0 &&  i < DIMBOARD - 1)
			printf("%d", i);
		printf("\n");
	}
}
 
 // ++++++ creearea tablei cu mine utilizand doua linii si doua coloane fara mine pentru verificare +++++
void create_cleanboard()
{
	int i, j;
 
	// pune la toate elementele de pe tabla semnul -
	for(i = 0; i < DIMBOARD; i++)
		for(j = 0; j < DIMBOARD; j++)
			minesboard[i][j] = '-';
 
	// creeare functie aleatoare
	srand(time(NULL));
 
	// adaugare mine aleator (marchez cu *)
	for(j = 0; j < DIMBOARD; j++)
	{
		int random = rand() % (DIMBOARD - 1) + 1;
		minesboard[random][j] = '*';
	}
 
	// ma asigur ca prima si ultima linie impreuna cu prima si ultima coloana nu contin mine
	for(i = 0; i < DIMBOARD; i++)
		for(j = 0; j < DIMBOARD; j++)
			if(i == 0 || i == DIMBOARD - 1)
				minesboard[i][j] = ' ';
 
	for(j = 0; j < DIMBOARD; j++)
		for(i = 0; i < DIMBOARD; i++)
			if(j == 0 || j == DIMBOARD - 1)
				minesboard[i][j] = ' ';
}
 
// ++++++ creearea tablei pentru interactiunea cu utilizatorul fara mine, fiecare element va fi cu - ++++++

void create_board_without_mines()
{
	int i, j;
 
	// pune la toate elementele de pe tabla semnul -
	for(i = 0; i < DIMBOARD; i++)
		for(j = 0; j < DIMBOARD; j++)
			cleanboard[i][j] = '-';
 
 
	// ma asigur ca prima si ultima linie impreuna cu prima si ultima coloana nu contin mine
	for(i = 0; i < DIMBOARD; i++)
		for(j = 0; j < DIMBOARD; j++)
			if(i == 0 || i == DIMBOARD - 1)
				cleanboard[i][j] = ' ';
 
	for(j = 0; j < DIMBOARD; j++)
		for(i = 0; i < DIMBOARD; i++)
			if(j == 0 || j == DIMBOARD - 1)
				cleanboard[i][j] = ' ';
}
 
// ++++++ caut minele in toate directiile si intorc numarul de mine gasite in vecinatati ++++++
int findneighborsmines(int row, int col)
{
	int mines = 0;
 
	// verific sus, jos, stanga, dreapta
	if(minesboard[row - 1][col] == '*')
		mines++;
	if(minesboard[row + 1][col] == '*')
		mines++;
	if(minesboard[row][col - 1] == '*')
		mines++;
	if(minesboard[row][col + 1] == '*')
		mines++;
 
	// verific in diagonale
	if(minesboard[row - 1][col + 1] == '*')
		mines++;
	if(minesboard[row - 1][col - 1] == '*')
		mines++;
	if(minesboard[row + 1][col + 1] == '*')
		mines++;
	if(minesboard[row + 1][col - 1] == '*')
		mines++;
 
	return mines;
}
 
// ++++++ cazul in care am pierdut ++++++
void lostgame()
{
	char ans;
 
	printf("\nHuh, Ai pierdut");
	printf("\nVrei sa joci din nou? (d/n)--> ");
	scanf(" %c", &ans);
 
	if(toupper(ans) == 'D')
	{
		system("cls");
		startgame();
	}
 
	else
		exit(0);
}

// ++++++ cazul in care am castigat ++++++
void wingame()
{
	char ans;
 
	printf("\nAi CASTIGAT!");
	printf("\nVrei sa joi din nou? (d/n)--> ");
	scanf(" %c", &ans);
 
	if(toupper(ans) == 'D')
	{
		system("cls");
		startgame();
	}

	else
		exit(0);
}
 
/*
curata campul de mine vecine, verific pozitia curenta a minelor vecine apoi verific toate cele 8 directii
din alte locatii fara minele vecine pana gasesc o pozitie care se suprapune peste o mina
actualizez tabla de joc cu numarul de mine din acea pozitie	
 */
void flushoutneighborsmines(int row, int col)
{
	int neighbormines = 0;
	int i = 0;
 
		neighbormines = findneighborsmines(row, col);
		cleanboard[row][col] = (char)(((int)'0') + neighbormines);
 
		neighbormines = 0;

		// verificare mine vecine mergand in sus
		while(neighbormines < 1 && i < row)
		{
			neighbormines = findneighborsmines(row - i, col);
			cleanboard[row - i][col] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand in jos
		while(neighbormines < 1 && row + i < DIMBOARD - 2)
		{
			neighbormines = findneighborsmines(row + i, col);
			cleanboard[row + i][col] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand la stanga
		while(neighbormines < 1 && i < col)
		{
			neighbormines = findneighborsmines(row, col - i);
			cleanboard[row][col - i] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand la dreapta
		while(neighbormines < 1 && col + i < DIMBOARD - 2)
		{
			neighbormines = findneighborsmines(row, col + i);
			cleanboard[row][col + i] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand in stanga-sus
		while(neighbormines < 1 && col + i < DIMBOARD - 2 && i < row)
		{
			neighbormines = findneighborsmines(row - i, col + i);
			cleanboard[row + i][col + i] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand dreapta-sus
		while(neighbormines < 1 && i < row && i < row && i < col)
		{
			neighbormines = findneighborsmines(row - i, col - i);
			cleanboard[row - i][col - i] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand in stanga-jos		
		while(neighbormines < 1 && row + i < DIMBOARD - 2 && col + i < DIMBOARD - 2)
		{
			neighbormines = findneighborsmines(row + i, col + i);
			cleanboard[row + i][col + i] = (char)(((int)'0') + neighbormines);
			i++;
		}
		i = 0;
		neighbormines = 0;

		// verificare mine vecine mergand dreapta-jos
		while(neighbormines < 1 && row + i < DIMBOARD - 2 && i < col)
		{
			neighbormines = findneighborsmines(row + i, col - i);
			cleanboard[row + i][col - i] = (char)(((int)'0') + neighbormines);
			i++;
		}
}
 
/*
 verific daca jocul a fost castigat, caut pe toata tabla de joc o pozitie fara mina care inca este marcata cu -
 ceea ce semnifica ca jucatorul poate face o incercare, daca nu exista nici un loc cu semnul -
 care nu contine nici o mina jucatorul a castigat
 */
int checkforwingame()
{
	int i, j;
 
	for(i = 1; i < DIMBOARD - 1; i++)
		for(j = 1; j < DIMBOARD - 1; j++) {
			if(cleanboard[i][j] == '-' && minesboard[i][j] != '*')
				return FALSE;
		}
 
	return TRUE;
}
 
// pornire joc si continuare in functie de optiunea jucatorului
void startgame()
{
	int row, col;
 
	// creeare table joc
	create_cleanboard();
	create_board_without_mines();
	show_board_without_mines();
 
	for(;;)
	{
		do {
		printf("Alege:\n");
		printf("Linie -> ");
		scanf("%d", &row);
		printf("Coloana -> ");
		scanf("%d", &col);
		} while(row < 1 || row > DIMBOARD - 2 || col < 1 || col > DIMBOARD - 2);
 
 
		if(minesboard[row][col] == '*')
		{
			printf("Ups, ai calcat o MINA!\n");
			show_cleanboard();
			lostgame();
		}
		else
			flushoutneighborsmines(row, col);
							 
		show_board_without_mines();
 
		if(checkforwingame() == TRUE)
			wingame();
 
		// printf("\n [Testing] \n");
		// show_cleanboard();
	}
}
