#include <iostream> 
#include <cstdlib>
#include <vector> 
#include <unistd.h>
#include <termios.h>
using namespace std ;

const int width = 25 ; 
const int height = 25 ; 
int x, y , fruitX, fruitY , gameScore; 
enum Direction { STOP = 0 , LEFT , RIGHT , UP , DOWN } ; 
Direction dir ;
vector<pair<int,int>> snake; 
bool gameOver = false;
void setNonBlockingInput(bool enable){
	static struct termios oldt,newt; 
	if (enable){
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt ;
		newt.c_lflag &= ~(ICANON |ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	}else {
		tcsetattr(STDIN_FILENO,TCSANOW, &oldt);
	}
}
int checkInput(){
	struct timeval tv = {0,0}; 
	fd_set fds; 
	FD_ZERO(&fds); 
	FD_SET(STDIN_FILENO, &fds); 
	return select(STDIN_FILENO + 1 , &fds , NULL, NULL, &tv);
}
void gameInput(){
	if (checkInput()){
		char key; 
		read(STDIN_FILENO, &key, 1); 
		switch(key){
			case 'a': 
				dir = LEFT; 
				break; 
			case 'd':
				dir = RIGHT; 
				break; 
			case 'w':
				dir = UP;
				break; 
			case 's':
				dir = DOWN; 
				break; 
			case 'x' : 
				gameOver = true; 
				break; 
		}
	}
}
void gameSetup(){
	gameOver = false ;
	x = width / 2; 
	y = height /2 ; 
	fruitX = rand() % width; 
	fruitY = rand() % height ; 
	dir = STOP ; 
	snake.push_back({x,y}); 
}
void drawGame() {
	system("clear"); 
	for (int i = 0 ; i < width + 2 ; i ++ ){
		cout << "#"; 
	}
	cout << endl; 
	for (int i = 0 ; i < height; i ++ ) {
		for (int j = 0 ; j < width; j ++) {
			if (j==0) cout << "#" ; 
			if (i == y && j == x) {
				cout << "O"; 
			}else if (i == fruitY && j == fruitX){
				cout << "F" ; 
			}else {
				bool isBody = false ;
				for (auto s : snake){
					if (s.first == j && s.second == i){
						cout << "o";
						isBody = true; 
						break; 
					}
				}
				if (!isBody) cout << " " ; 
			}

			if( j == width -1 ) cout << "#" ; 
		}
		cout << endl ;
	}
	for (int i=0 ; i < width +2 ; i ++ ){
		cout << "#"; 
	}
	cout << endl; 
	cout << "Score: " << gameScore << endl ;
}
void movement(){
	for (int i = snake.size() - 1; i > 0 ; i ++){
		snake[i] = snake[i-1] ; 
	}	
	if (dir == LEFT) x--; 
	if (dir == RIGHT) x++; 
	if (dir == UP) y--; 
	if (dir == DOWN) y++; 
	snake[0] = {x,y} ; 
}
void checkState(){
	if (x>= width || x < 0 || y >= height || y < 0 ){
		gameOver = true ;

	}
	for (int i =1  ; i < snake.size() ; i ++ ) {
		if ( x == snake[i].first && snake[i].second == y ) {
			gameOver = true ;
		}	
	}
	if (x == fruitX && y == fruitY){
		gameScore += 10; 
		fruitX = rand() % width; 
		fruitY = rand() % height ; 
		snake.push_back({-1,-1}); 
	}
}
int main () {
	setNonBlockingInput(true); 
	gameSetup();

	while(!gameOver){

		drawGame();
		gameInput(); 
		movement();
		checkState();
		usleep(150000); 
	}
	setNonBlockingInput(false);
	cout << "Game Over " << endl; 
	cout << "Your score: " << gameScore << endl ; 
	return 0 ; 
}
