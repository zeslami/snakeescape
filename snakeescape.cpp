#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;


#define MAX 100
#define WIDTH 77
#define HEIGHT 22
#define INIT_player_LENGTH 2
#define INIT_snake_LENGTH 4

#define WALL -2
#define PLAYER -3
#define SNAKE -1
#define NOTHING 0

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define EXIT -1
static int dx[5] = { 1, 0, -1, 0 };
static int dy[5] = { 0, -1, 0, 1 };
int ground[MAX][MAX];
int mekan1;
int mekan2;
int input = RIGHT;
int inputt = DOWN;	
int item = NOTHING;
int Score = 0;

ofstream soatfile; 
ofstream scorefile;

int menu();

void gotoxy(int column, int row)
{
	HANDLE hStdOut;
	COORD coord;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(hStdOut, coord);
}

void clearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells and cell attributes in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,		//handle to console screen buffer
		(TCHAR) ' ',	//character to write to the buffer
		cellCount,		//number of cells to write to
		homeCoords,		//coordinates of first cell
		&count			//receives the number of characters written
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,			//handle to console screen buffer
		csbi.wAttributes,	//Character attributes to use
		cellCount,			//Number of cells to set attribute
		homeCoords,			//Coordinate of first cell
		&count				//receives the number of characters written
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int oppositeDirection(int input1, int input2)
{
	if (input1 == LEFT && input2 == RIGHT)
		return 1;
	if (input1 == RIGHT && input2 == LEFT)
		return 1;
	if (input1 == UP && input2 == DOWN)
		return 1;
	if (input1 == DOWN && input2 == UP)
		return 1;

	return 0;
}

struct Coordinate
{
	int x, y;
};

class Ground
{
private:
	
public:
    void initGround();
    void firstDraw();
	Coordinate body[WIDTH*HEIGHT];
	
};

void Ground::initGround()// meghdar dehi avalie zamin --->> mnm ino mizaram 
{
	int i, j;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			ground[i][j] = 0;

	for (i = 0; i <= WIDTH + 1; i++)
	{
		//top bottom wall
		ground[0][i] = WALL;
		ground[HEIGHT + 1][i] = WALL;
	}
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		//right left wall
		ground[i][0] = WALL;
		ground[i][WIDTH + 1] = WALL;
	}
};

void Ground::firstDraw()
{
	clearScreen();
	int i, j;
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		for (j = 0; j <= WIDTH + 1; j++)
		{
			switch (ground[i][j])
			{
			case NOTHING:
				cout << " "; break;
			case WALL:
				if ((i == 0 && j == 0)
					|| (i == 0 && j == WIDTH + 1)
					|| (i == HEIGHT + 1 && j == 0)
					|| (i == HEIGHT + 1 && j == WIDTH + 1))
					cout << "+";    //the 4 corners
				else
					if (j == 0 || j == WIDTH + 1)
						cout << "|";    //left/right wall
					else
						cout << "-";    //top/bottom wall
				break;
			case SNAKE:
				if (i == body[0].y && j == body[0].x)
					cout << "O";
				else
					cout << "+";
				break;
            case PLAYER:
                if (i == body[0].y && j == body[0].x)
                    cout << "#";
			default:				//food
			  cout << " ";	//prints a middle dot
			}
		}
		cout << endl;
	}
};

class snake : public Ground
{
private:
	int mekan;
	int tol;
	Coordinate bodyy[WIDTH*HEIGHT];
	int direction;
public:
	void initSnake(int p,int t);
	void updateSnake1(int delay);
	void updateSnake2(int delay);

};

void snake::initSnake(int p,int t)// meghdar dehi avalie mar
{
	tol = INIT_snake_LENGTH;		//set head of snake to be at the centre
	bodyy[0].x = WIDTH - p ;
	bodyy[0].y = HEIGHT - t;
	direction = inputt;//jehat
	

	int i;
	for (i = 1; i < tol; i++)
	{
		bodyy[i].x = bodyy[i - 1].x - dx[direction];		//if moving right,
		bodyy[i].y = bodyy[i - 1].y - dy[direction];		//body is on the left
	}
	//let the ground know the snake's position
	for (i = 0; i < tol; i++)
		ground[bodyy[i].y][bodyy[i].x] = SNAKE;// mogheyat mar be zamin dade mishavad
}

void snake::updateSnake1(int delay)
{
	int i;
	Coordinate prev[WIDTH*HEIGHT];
	for (i = 0; i < tol; i++)
	{
		prev[i].x = bodyy[i].x;
		prev[i].y = bodyy[i].y;
	}

	if (inputt != EXIT && !oppositeDirection(direction, inputt))
	{

			direction = inputt;
	}

		

	bodyy[0].x = prev[0].x + dx[direction];		//head of snake
	bodyy[0].y = prev[0].y + dy[direction];		//follows the direction

	if (ground[bodyy[0].y][bodyy[0].x] < NOTHING)
	{
		item = -1;
		return;
	}

	else
	{
		ground[bodyy[tol - 1].y][bodyy[tol - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(bodyy[tol - 1].x, bodyy[tol - 1].y);		// if snake does not get food,
		cout << " ";						// erase last part because the snake is moving
	}

	for (i = 1; i < tol; i++)
	{
		bodyy[i].x = prev[i - 1].x;	//body follows the previous
		bodyy[i].y = prev[i - 1].y;	//location that it was from
	}

	gotoxy(bodyy[1].x, bodyy[1].y);
	cout << "+";					//change the previous head to a body
	gotoxy(bodyy[0].x, bodyy[0].y);
	cout << "O";					//add a head to the snake

	//let the ground know the snake's position
	for (i = 0; i < tol; i++)
		ground[bodyy[i].y][bodyy[i].x] = SNAKE;
	
	int a1 = bodyy[0].y;
	int a2 = bodyy[0].x;
	if (a1 > mekan1 && a2 > mekan2)
	{
		inputt = UP;
	}
	else if (a1 == mekan1 && a2 > mekan2)
	{
		inputt = LEFT;
	}
	else if (a1 == mekan2 && a2 < mekan2)
	{
		inputt = RIGHT;
	}
	else if (a1 > mekan1 && a2 == mekan2)
	{
		inputt = UP;
	}
	else if (a1 < mekan1 && a2 == mekan2)
	{
		inputt = DOWN;
	}
	else if (a1 > mekan1 && a2 < mekan2)
	{
		inputt = RIGHT;
	}
	else if (a1 < mekan1 && a2 > mekan2)
	{
		inputt = LEFT;
	}
	else if (a1 < mekan1 && a2 < mekan2)
	{
		inputt = DOWN;
	}
	
	
	Sleep(delay);

	return;
}
void snake::updateSnake2(int delay)
{
	int i;
	Coordinate prev[WIDTH*HEIGHT];
	for (i = 0; i < tol; i++)
	{
		prev[i].x = bodyy[i].x;
		prev[i].y = bodyy[i].y;
	}

	if (inputt != EXIT && !oppositeDirection(direction, inputt))
	{

			direction = inputt;
	}

		

	bodyy[0].x = prev[0].x + dx[direction];		//head of snake
	bodyy[0].y = prev[0].y + dy[direction];		//follows the direction

	if (ground[bodyy[0].y][bodyy[0].x] < NOTHING)
	{
		item = -1;
		return;
	}

	else
	{
		ground[bodyy[tol - 1].y][bodyy[tol - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(bodyy[tol - 1].x, bodyy[tol - 1].y);		// if snake does not get food,
		cout << " ";						// erase last part because the snake is moving
	}

	for (i = 1; i < tol; i++)
	{
		bodyy[i].x = prev[i - 1].x;	//body follows the previous
		bodyy[i].y = prev[i - 1].y;	//location that it was from
	}

	gotoxy(bodyy[1].x, bodyy[1].y);
	cout << "+";					//change the previous head to a body
	gotoxy(bodyy[0].x, bodyy[0].y);
	cout << "O";					//add a head to the snake

	//let the ground know the snake's position
	for (i = 0; i < tol; i++)
		ground[bodyy[i].y][bodyy[i].x] = SNAKE;
	
	int a1 = bodyy[0].y;
	int a2 = bodyy[0].x;
	if (a1 > mekan1 && a2 > mekan2)
	{
		inputt = DOWN;
	}
	else if (a1 == mekan1 && a2 > mekan2)
	{
		inputt = RIGHT;
	}
	else if (a1 == mekan2 && a2 < mekan2)
	{
		inputt = LEFT;
	}
	else if (a1 > mekan1 && a2 == mekan2)
	{
		inputt = DOWN;
	}
	else if (a1 < mekan1 && a2 == mekan2)
	{
		inputt = UP;
	}
	else if (a1 > mekan1 && a2 < mekan2)
	{
		inputt = UP;
	}
	else if (a1 < mekan1 && a2 > mekan2)
	{
		inputt = DOWN;
	}
	else if (a1 < mekan1 && a2 < mekan2)
	{
		inputt = UP;
	}
	
	
	Sleep(delay);

	return;
}

class player : public Ground
{
private:
	int length;
	Coordinate body[WIDTH*HEIGHT];
	int direction;
	string fullname;
	
public:

	void initplayer();
	void updateplayer(int delay);
	void setfullname(string name , string family);
	string getfullname();
	

};

void player::initplayer()// meghdar dehi avalie mar
{
	length = INIT_player_LENGTH;		//set head of player to be at the centre
	body[0].x = WIDTH / 2;
	body[0].y = HEIGHT / 2;
	input = RIGHT;
	direction = input;//jehat

	int i;
	for (i = 1; i < length; i++)
	{
		body[i].x = body[i - 1].x - dx[direction];		//if moving right,
		body[i].y = body[i - 1].y - dy[direction];		//body is on the left
	}
	//let the ground know the player's position
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = PLAYER;// mogheyat mar be zamin dade mishavad

};

void player::updateplayer(int delay)
{
	int i;
	Coordinate prev[WIDTH*HEIGHT];
	for (i = 0; i < length; i++)
	{
		prev[i].x = body[i].x;
		prev[i].y = body[i].y;
	}

	if (input != EXIT && !oppositeDirection(direction, input))
		direction = input;

	body[0].x = prev[0].x + dx[direction];		//head of player
	body[0].y = prev[0].y + dy[direction];		//follows the direction

	if (ground[body[0].y][body[0].x] < NOTHING)
	{
		item = -1;
		return;
	}

		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);		// if player does not get ,
		cout << " ";						// erase last part because the player is moving

	for (i = 1; i < length; i++)
	{
		body[i].x = prev[i - 1].x;	//body follows the previous
		body[i].y = prev[i - 1].y;	//location that it was from
	}

	gotoxy(body[1].x, body[1].y);
	cout << "#";					//change the previous head to a body
	gotoxy(body[0].x, body[0].y);
	cout << "#";					//add a head to the player

	//let the ground know the player's position
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = PLAYER;
	
		
	mekan1 = body[0].y;
	mekan2 = body[0].x;
	Sleep(delay);

	return;
}

void player :: setfullname(string name , string family)
{
	fullname = name + " " + family;
};

string  player :: getfullname()
{
	return fullname;
};

int sorat()
{
	int sorat;
	cout << "Choose a number from 1 to 8" << endl;
	cout << "*** 1 indicates the lowest speed, 8 indicates the highest speed ***" << endl;
	cout << "9 . exit" << endl;
	cin >> sorat;
	switch (sorat)
	{
	case 1:
		
   		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "1000"; 
    	soatfile.close();
		break;
	case 2:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "800"; 
    	soatfile.close();
		break;
	case 3:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "600"; 
    	soatfile.close();
		break;
	case 4:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "400"; 
    	soatfile.close();
		break;
	case 5:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "200"; 
    	soatfile.close();
		break;
	case 6:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "100"; 
    	soatfile.close();
		break;
	case 7:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "50"; 
    	soatfile.close();
		break;
	case 8:
		soatfile.open("sorat.txt" , ios::out); 
   		soatfile << "20"; 
    	soatfile.close();
		break;
	case 9:
		menu();
		break;
	default:
		//sorat();
		break;
	}

	menu();
}

void userInput1(void* id)
{
	do
	{
		int c = _getch();
		switch (c)
		{
		case 'W': case 'w':  input = UP   ; break;
		case 'S': case 's':  input = DOWN ; break;
		case 'D': case 'd':  input = RIGHT; break;
		case 'A': case 'a':  input = LEFT ; break;
		case '4':	    input = EXIT ; break;
		}
	} while (input != EXIT && item >= 0);

	_endthread();
	return;
}

void userInput2(void* id)//tabe harkat random
{
	do
	{
		int min = 0;
		int max = 5;
    	srand(time(0));
     	int c = (rand() % (max - min)) ;
		
		//cout << c;
		//c = _getch();
		switch (c)
		{
		case 'W': case 'w': case 1: input = UP   ; break;
		case 'S': case 's': case 2: input = DOWN ; break;
		case 'D': case 'd': case 3: input = RIGHT; break;
		case 'A': case 'a': case 4: input = LEFT ; break;
		case 27:	    input = EXIT ; break;
		}
	} while (input != EXIT && item >= 0);

	_endthread();
	return;
}


int startgameeasy()
{
	
	string line;
  ifstream soatfile ("sorat.txt");
  if (soatfile.is_open())
  {
    while ( getline (soatfile,line) )
    {
      cout << line << '\n';
    }
    soatfile.close();
  }
	int delay = atoi(line.c_str());//sorat mar harchi bishtarr sorat kamtar v baraks
	
	
	srand(time(NULL));//tolid adad tasadofi
	player nagini1;// shy mar be esm negini
    snake mar1;
	nagini1.initGround();// meghdar dehi avaliye bazi
    mar1.initGround();
	nagini1.initplayer();
    mar1.initSnake(35 , 15);
	nagini1.firstDraw();

    mar1.firstDraw();
	_beginthread(userInput1, 0, (void*)0);
	//_beginthread(userInput2, 0, (void*)0);
	
	do
	{
		nagini1.updateplayer(delay);       
        mar1.updateSnake1((delay * 2));
		Score++;

	
	} while (item >= 0 && input != EXIT);

	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";

	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	Score = Score / 8;
	scorefile.open("score.txt" , ios::app); 
   	scorefile << Score << endl; 
    scorefile.close();
	cout << "Your score is "<<Score;

	gotoxy(WIDTH / 2, HEIGHT / 2);
	getchar();
	getchar();
	clearScreen();

	_getch();
	return 0;
};

int startgammedium()
{
	
	string line;
  ifstream soatfile ("sorat.txt");
  if (soatfile.is_open())
  {
    while ( getline (soatfile,line) )
    {
      cout << line << '\n';
    }
    soatfile.close();
  }
	int delay = atoi(line.c_str());//sorat mar harchi bishtarr sorat kamtar v baraks
	
	
	srand(time(NULL));//tolid adad tasadofi
	player nagini1;// shy mar be esm negini
    snake mar1;
    snake mar2;
	nagini1.initGround();// meghdar dehi avaliye bazi
    mar1.initGround();
    mar2.initGround();
	nagini1.initplayer();
    mar1.initSnake(30,15);
	mar2.initSnake(65,15);
	nagini1.firstDraw();
    mar1.firstDraw();
	mar2.firstDraw();

	_beginthread(userInput1, 0, (void*)0);
    //_beginthread(userInput1, 0, (void*)0);
	
	do
	{
		nagini1.updateplayer(delay);
        mar2.updateSnake2(delay);
		mar1.updateSnake2(delay);
		Score++;
	} while (item >= 0 && input != EXIT);

	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";

	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	Score = Score / 8;
	scorefile.open("score.txt" , ios::app); 
   	scorefile <<Score << endl; 
    scorefile.close();
	cout << "Your score is "<<Score;

	gotoxy(WIDTH / 2, HEIGHT / 2);
	getchar();
	getchar();
	clearScreen();

	_getch();
	return 0;
};

int startgamhard()
{
	
	string line;
  ifstream soatfile ("sorat.txt");
  if (soatfile.is_open())
  {
    while ( getline (soatfile,line) )
    {
      cout << line << '\n';
    }
    soatfile.close();
  }
	int delay = atoi(line.c_str());//sorat mar harchi bishtarr sorat kamtar v baraks
	
	
	srand(time(NULL));//tolid adad tasadofi
	player nagini1;// shy mar be esm negini
	player nagini2;
    snake mar1;
    snake mar2;
	snake mar3;
	nagini1.initGround();// meghdar dehi avaliye bazi
    mar1.initGround();
    mar2.initGround();
	mar3.initGround();
	nagini1.initplayer();
    mar1.initSnake(60,15);
	mar2.initSnake(40,15);
	mar3.initSnake(20, 15);
	nagini1.firstDraw();
    mar1.firstDraw();
	mar2.firstDraw();
	mar3.firstDraw();
	_beginthread(userInput1, 0, (void*)0);
	//_beginthread(userInput2, 0, (void*)0);
	
	do
	{
		nagini1.updateplayer(delay);
        mar1.updateSnake1((delay ));
        mar2.updateSnake1((delay ));
		mar3.updateSnake1((delay ));
		Score++;
	
	} while (item >= 0 && input != EXIT && inputt != EXIT);

	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";

	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	Score = Score / 8;
	scorefile.open("score.txt" , ios::app); 
   	scorefile <<Score << endl; 
    scorefile.close();
	cout << "Your score is "<<Score;

	gotoxy(WIDTH / 2, HEIGHT / 2);
	getchar();
	getchar();
	clearScreen();

	_getch();
	return 0;
};

void newgame()
{
	player players;
    string name;
    string  family;
    cout << "enter name palyer --->>";
    cin >> name  ;
    cout << "enter famili player --->>";
    cin >> family;
	players.setfullname(name, family);
	scorefile.open("score.txt" , ios::app); 
   	scorefile << players.getfullname() <<" "; 
    scorefile.close();



   int select;
   cout << "select level" << endl;
   cout << "1 . easy" << endl;
   cout << "2 . medium" << endl;
   cout << "3 . hard" << endl;
   cin >> select;
   switch (select)
   {
   case 1:
		startgameeasy();
      /*تابعی باید فرخوانی بشه که توش یک  تا مار هستش */
      break;
   
   case 2:
	   startgammedium();
      /*تابعی باید فراخوانی بشه توش دو مار هستش */
      break;

   case 3:
		startgamhard();
      /*تابعی باید فراخوانی بشه که توش سه مار باشه */
      break;
   
   default:
      newgame();
      break;
   }
}
void score()
{

	string linee;
	ifstream scorefile ("score.txt");
	if (scorefile.is_open())
  	{
    	while ( getline (scorefile,linee) )
    		{
      			cout << linee << '\n';
    		}
    	scorefile.close();
 	}

}

int menu()
{
   int select;
   cout << "**** welcom to game ****" << endl;
   cout << "1 . new game"<< endl;
   cout << "2 . Set the game speed" << endl;
   cout << "3 . Show scores"<<endl;
   cout << "4 . exit" << endl;
   cin >> select;
   switch (select)
   {
   case 1:
      newgame();
      break;
	case 2:
		sorat();
		break;
    case 3:
		score();
      break;
	case 4:
		return 0;
      break;

   
   default:
		cout <<"Choose from the options"<<endl;
   
      break;
	}  

}

int main()
{
	menu();
	
}
