#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h> 
#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

double window_x = 1000, window_y = 720;
double dx = 20;
double dy = 40;
double nukeSpeed = 40;
double click_x, click_y, score_x, score_y;
double img_x = 640, laser_x = img_x + 45, size_x = 110, laserSize_x = 20, enemySize_x = 70;
double img_y = 72, laser_y = 72, size_y = 120, laserSize_y = 43;
double nukeSize_x = 28, nukeSize_y = 100;
char laserImg[] = {"images/laser2.bmp"};
char bgImg[11][20] = {"images/bg/bg1.bmp","images/bg/bg2.bmp","images/bg/bg3.bmp","images/bg/bg4.bmp","images/bg/bg5.bmp",
			"images/bg/bg6.bmp","images/bg/bg7.bmp","images/bg/bg8.bmp","images/bg/bg9.bmp","images/bg/bg10.bmp",
			"images/bg/bg11.bmp"};
char player[4][24] = {"images/spaceship1.bmp", "images/spaceship2.bmp", "images/spaceship3.bmp", "images/spaceship4.bmp"};
char enemy[3][30] = {"images/enemy/enemyBlack1.bmp", "images/enemy/enemyGreen2.bmp", "images/enemy/enemyBlue3.bmp"};
char explode[3][35] = {"images/explosion/explode1.bmp", "images/explosion/explode2.bmp", "images/explosion/explode3.bmp"};
char scoreImg[10][30] = {"images/score/0.bmp", "images/score/1.bmp", "images/score/2.bmp", "images/score/3.bmp", "images/score/4.bmp", 
						"images/score/5.bmp", "images/score/6.bmp", "images/score/7.bmp", "images/score/8.bmp", "images/score/9.bmp"};
char menuImg[5][30] = {"images/menu/menu1.bmp", "images/menu/menu2.bmp", "images/menu/menu3.bmp", "images/menu/menu4.bmp", "images/menu/menu5.bmp"};
char aboutImg[10][30] = {"images/about/about1.bmp", "images/about/about2.bmp", "images/about/about3.bmp", "images/about/about4.bmp", "images/about/about5.bmp", 
						"images/about/about6.bmp", "images/about/about7.bmp", "images/about/about8.bmp", "images/about/about9.bmp", "images/about/about10.bmp"};
char numbers[4][30] = {"images/numbers/0.bmp", "images/numbers/1.bmp", "images/numbers/2.bmp", "images/numbers/3.bmp"};
char numbers2[10][30] = {"images/numbers2/0.bmp", "images/numbers2/1.bmp", "images/numbers2/2.bmp", "images/numbers2/3.bmp", "images/numbers2/4.bmp", "images/numbers2/5.bmp"
						, "images/numbers2/6.bmp", "images/numbers2/7.bmp", "images/numbers2/8.bmp", "images/numbers2/9.bmp"};
char nuke[] = {"images/nuke.bmp"};

double bg_speed = 12;
int playerIndex = 0;
int enemyIndex1 = 0;
double randomBaseSpeed;
double enemySpeed = 5;
int explodeIndex = 0;
int menuIndex = 0;
int countForMenu = 0;
int aboutIndex = 0;
int lives = 3;
int tempScore, digits;
int highScore;
int seconds = 0;
int nukeScore = 0;
int normalScore = 0;

//for tracking the score
int score = 0;
int _time = 0; // in seconds


struct axes {
	double x;
	double y;
};


axes bg[11];
axes enm[3];
axes enmBaseSpeed[3];
axes s[10];
axes nukeAxis[3];

bool goToLeft = false;
bool goToRight = false;
bool shootLaser = false;
bool playerShooted = false;
bool gameMenu = true;
bool insideGame = false;
bool clickEffect = false;
bool showAbout = false;
bool showHighScore = false;
bool countForSeconds = false;
bool nukeActivated = false;
bool canActivateNuke = false;
bool musicOn = true;

//for pausing
bool moveBG = false;
bool moveEnemy = false;
bool paused = false;
bool showResume = false;
bool showQuit = false;


void readHighScore()
{
	FILE *file;
	file = fopen("score.txt", "r");
	if(file != NULL)
	{
		fscanf(file, "%d", &highScore);
	}
}

void writeHighScore()
{
	FILE *file;
	file = fopen("score.txt", "w");
	if(file != NULL)
	{
		fprintf(file, "%d", score);
	}
}

void renderBackground()
{
	for(int i = 0; i < 11; i++)
	{
		iShowBMP(bg[i].x, bg[i].y, bgImg[i]);
		if(!paused)
		{
			bg[i].y -= bg_speed;
			if(bg[i].y <= -72)
			{
				bg[i].y = window_y;
			}
		}
	}
}

void randomBaseSpeedInitializer()
{
	int sum = 5;
	for(int i = 0; i < 3; i++)
	{
		enmBaseSpeed[i].y = rand() % sum;
		if(i == 0) enmBaseSpeed[i].y += 2;
		sum += 5;
	}
}

void displayEnemies()
{
	//int sum2 = 5;
	for(int i = 0; i < 3; i++)
	{
		iShowBMP2(enm[i].x, enm[i].y, enemy[i], 0);

		//Moving enemies
		if(!paused)
		{
			enm[i].y -= enmBaseSpeed[i].y + enemySpeed;
			if(enm[i].y <= 0)
			{
				enm[i].y = window_y;
			}
			//sum2 += 5;

			//Enemy position initializing
			if(enm[i].y >= window_y)
			{
				int sum;
				if(i == 0) sum = 0;
				else if(i == 1) sum = 350;
				else if(i == 2) sum = 700;
				enm[i].x = rand() % 200 + sum;
				//enm[i].y = window_y;
			}
		}
	}
}

void initialPosOfLaser()
{
	shootLaser = false;
	laser_x = img_x + 45;
	laser_y = 72;
}

void enemyDestruction()
{
	for(int i = 0; i < 3; i++)
	{
		//Checking if laser hits enemies and showing explosion and destroying enemy
		if(laser_x + laserSize_x >= enm[i].x && laser_x <= enm[i].x + enemySize_x && laser_y + laserSize_y >= enm[i].y && laser_y + laserSize_y >= size_y)
		{

			iShowBMP2(enm[i].x, enm[i].y, explode[explodeIndex], 0);
			explodeIndex++;
			
			if(explodeIndex >= 3)
			{
				//Increasing score
				normalScore++;
				score++;

				//Initial position of laser
				initialPosOfLaser();

				//enemy returning to window_y after getting destroyed
				enm[i].y = window_y;
				explodeIndex = 0;

				nukeAxis[i].y = 0;
			}
		}
	}

	for(int i = 0; i < 3; i++)
	{
		//Checking if laser hits enemies and showing explosion and destroying enemy
		if(nukeAxis[i].x + nukeSize_x >= enm[i].x && nukeAxis[i].x <= enm[i].x + enemySize_x && nukeAxis[i].y + nukeSize_y >= enm[i].y && nukeActivated && nukeScore % 3 < 3)
		{

			iShowBMP2(enm[i].x, enm[i].y, explode[explodeIndex], 0);
			explodeIndex++;
			
			if(explodeIndex >= 3)
			{
				//Increasing score
				nukeScore++;
				score++;

				//enemy returning to window_y after getting destroyed
				enm[i].y = window_y;
				explodeIndex = 0;

				//nukeAxis[i].y = 0;
				if(nukeScore > 0 && nukeScore % 3 == 0)
				{
					nukeActivated = false;
					canActivateNuke = false;
					for(int j = 0; j < 3; j++) nukeAxis[j].y = 0;
					normalScore = 0;
				}
			}
		}
	}
}

void shootNuke()
{
	for(int i = 0; i < 3; i++)
	{
		nukeAxis[i].x = enm[i].x + (enemySize_x / 2);
		nukeAxis[i].y += nukeSpeed;
		iShowBMP2(nukeAxis[i].x, nukeAxis[i].y, nuke, 0);
	}
}

void playerLifeDecreament()
{
	for(int i = 0; i < 3; i++)
	{
		if(img_x + size_x >= enm[i].x && img_x <= enm[i].x + enemySize_x && img_y + size_y >= enm[i].y)
		{
			lives--;
			countForSeconds = true;
			enm[i].y = window_y;
		}
	}
}

void slowDownPlayer()
{
	if(countForSeconds && seconds <= 2 && lives >= 1)
	{
		dx = 5;
	}
	else
	{
		dx = 20;
		countForSeconds = false;
		seconds = 0;
	}
}

void animatePlayer()
{
	if(playerShooted)
	{
		iShowBMP2(img_x, img_y, player[3], 0);
		playerShooted = false;
	}
	else
	{
		iShowBMP2(img_x, img_y, player[playerIndex], 0);
		playerIndex++;
		if(playerIndex == 3)
		{
			playerIndex = 0;
		}
	}
}

void playerMovement()
{
	if(goToRight)
	{
		img_x += dx;
		if(laser_y < size_y)
		{
			laser_x += dx;
		}
	}
	if(goToLeft)
	{
		img_x -= dx;
		if(laser_y < size_y)
		{
			laser_x -= dx;
		}
	}

	//Moving the laser image
	if(shootLaser)
	{
		laser_y += dy;
	}
	if(laser_y >= window_y)
	{
		//Initial position of laser
		initialPosOfLaser();
	}
}

void playerAutoMovement()
{
	if(img_x + size_x >= window_x)
	{
		goToRight = false;
		goToLeft = true;
	}
	if(img_x <= 0)
	{
		goToRight = true;
		goToLeft = false;
	}
}

void initialGameState()
{
	gameMenu = true;
	insideGame = false;
	paused = false;
	score = 0;
	_time = 0;
	lives = 3;
}

void displayScore()
{
	tempScore = score;
	char charScore[] = {"          "};
	digits = 0;

	while(tempScore)
	{
		digits++;
		tempScore /= 10;
	}
	tempScore = score;
	
	while(digits--)
	{
		if(tempScore % 10 == 0) charScore[digits] = '0';
		else if(tempScore % 10 == 1) charScore[digits] = '1';
		else if(tempScore % 10 == 2) charScore[digits] = '2';
		else if(tempScore % 10 == 3) charScore[digits] = '3';
		else if(tempScore % 10 == 4) charScore[digits] = '4';
		else if(tempScore % 10 == 5) charScore[digits] = '5';
		else if(tempScore % 10 == 6) charScore[digits] = '6';
		else if(tempScore % 10 == 7) charScore[digits] = '7';
		else if(tempScore % 10 == 8) charScore[digits] = '8';
		else if(tempScore % 10 == 9) charScore[digits] = '9';
		tempScore /= 10;
	}
	iText(133, 27.5, charScore, GLUT_BITMAP_TIMES_ROMAN_24);
}

void displayLives()
{
	iShowBMP2(381, 26, numbers[lives], 0);
	if(lives == 0)
	{
		initialGameState();
		enemySpeed = 5;
		for(int i = 0; i < 3; i++)
		{
			enm[i].y = window_y;
		}
	}
}

void animateMenu()
{
	iShowBMP(0, 0, menuImg[menuIndex]);
	countForMenu++;
	if(countForMenu % 1 == 0 && countForMenu > 0)
	{
		menuIndex++;
	}
	if(menuIndex >= 5)
	{
		menuIndex = 0;
	}
}

void displayHighScore()
{
	int temp = highScore;
	int digits = 0;
	while(temp)
	{
		digits++;
		temp /= 10;
	}
	temp = highScore;
	int sum = 220 + (digits * 120);
	for(int i = digits; i > 0; i--)
	{
		for(int j = 0; j < 10; j++)
		{
			if(temp % 10 == j) iShowBMP2(sum, 292, numbers2[j], 0);
		}
		sum -= 120;
		temp /= 10;
	}
}



void iDraw()
{
	iClear();

	if(insideGame)
	{
		//Rendering background
		renderBackground();

		//Displaying enemies
		displayEnemies();

		//for nuke
		if(normalScore == 10)
		{
			canActivateNuke = true;
		}
		if(nukeActivated)
		{
			shootNuke();
		}

		enemyDestruction();

		playerLifeDecreament();

		slowDownPlayer();

		//Score menu
		iShowBMP(0, 0, "images/bg/menu_bar.bmp");
		if(canActivateNuke)
		{
			iText(500, 27.5, "Nuke can be activated", GLUT_BITMAP_TIMES_ROMAN_24);
		}

		//Displaying laser
		iShowBMP2(laser_x, laser_y, laserImg, 0);

		//Displaying and animating player's spaceship
		animatePlayer();
	
		if(!paused)
		{
			//Moving spaceship automatically when it touches border
			playerAutoMovement();

			//Moving spaceship left and right
			playerMovement();
		}

		//click effect
		if(clickEffect)
		{
			iShowBMP2(click_x, click_y, "images/clickEffect.bmp", 0);
			clickEffect = false;
		}

		//display score
		displayScore();

		displayLives();

		//High score
		if(score > highScore)
		{
			writeHighScore();
			highScore = score;
		}

		//Puase menu
		if(paused)
		{
			iShowBMP2(250, 135, "images/pauseMenu/pauseMenu1.bmp", 0);
			if(showResume)
			{
				iShowBMP2(250, 135, "images/pauseMenu/resume.bmp", 0);
			}
			if(showQuit)
			{
				iShowBMP2(250, 135, "images/pauseMenu/quit.bmp", 0);
			}
		}
	}

	else if(gameMenu)
	{
		animateMenu();

		if(showAbout)
		{
			iShowBMP2(365, 58, aboutImg[aboutIndex], 0);
			if(aboutIndex < 9)
			{
				aboutIndex++;
			}
		}

		if(showHighScore)
		{
			iShowBMP(0, 0, "images/menu/highScore2.bmp");
			displayHighScore();
		}

		if(clickEffect)
		{
			iShowBMP2(click_x, click_y, "images/clickEffect.bmp", 0);
			clickEffect = false;
		}
	}

}


void iMouseMove(int mx, int my)
{
	
}


void iPassiveMouseMove(int mx, int my)
{

	if(insideGame)
	{
		if(paused)
		{
			if(mx >= 376 && mx <=623 && my >= 401 && my <= 468)
			{
				showResume = true;
				showQuit = false;
			}
			else if(mx >= 376 && mx <=623 && my >= 304 && my <= 368)
			{
				showQuit = true;
				showResume = false;
			}
			else
			{
				showQuit = false;
				showResume = false;
			}
		}
	}
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if(gameMenu)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//click effect
			click_x = (double)mx - 12.5;
			click_y = (double)my - 12.5;
			clickEffect = true;

			//play game
			
			if(mx >= 154 && mx <=365 && my >= 456 && my <= 505)
			{
				gameMenu = false;
				insideGame = true;
				moveBG = true;
				moveEnemy = true;
				randomBaseSpeedInitializer();
			}

			//exiting game
			if(mx >= 154 && mx <=365 && my >= 210 && my <= 257)
			{
				exit(0);
			}

			//about
			if(mx >= 154 && mx <=365 && my >= 292 && my <= 344)
			{
				if(!showAbout)
				{
					showAbout = true;
				}
				else
				{
					showAbout = false;
					aboutIndex = 0;
				}
			}

			//high score
			if(mx >= 154 && mx <=365 && my >= 374 && my <= 425)
			{
				if(!showHighScore)
				{
					showHighScore = true;
				}
			}
			if(mx >= 931 && mx <=window_x && my >= 654 && my <= window_y)
			{
				if(showHighScore)
				{
					showHighScore = false;
				}
			}

			//close about
			if(mx >= 903 && mx <=964 && my >= 509 && my <= 560)
			{
				showAbout = false;
				aboutIndex = 0;
			}

			printf("%d %d\n",mx,my);
		}
	}


	if(insideGame)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//click effect
			click_x = (double)mx - 12.5;
			click_y = (double)my - 12.5;
			clickEffect = true;
		}

		if(mx >= 928 && mx <=1000 && my >= 0 && my <= 72)
		{
			paused = true;
		}

		if(paused)
		{
			if(mx >= 376 && mx <= 623 && my >= 401 && my <= 468)
			{
				paused = false;
				showResume = false;
			}
			else if(mx >= 376 && mx <=623 && my >= 304 && my <= 368)
			{
				gameMenu = true;
				insideGame = false;
				paused = false;
				score = 0;
				normalScore = 0;
				nukeScore = 0;
				_time = 0;
				lives = 3;
				enemySpeed = 5;
				for(int i = 0; i < 3; i++)
				{
					enm[i].y = window_y;
				}
				showQuit = false;
			}
		}

		printf("%d %d\n",mx,my);
	}
	
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}


void iKeyboard(unsigned char key)
{
	if(insideGame)
	{
		if (key == 's')
		{
			shootLaser = true;
			playerShooted = true;
			//PlaySound("sounds/laser1.wav", NULL, SND_ASYNC);
		}
		if (key == 'w' && !nukeActivated && canActivateNuke)
		{
			nukeActivated = true;
		}
		if(key == 'p')
		{
			if(!paused)
			{
				paused = true;
			}
			else
			{
				paused = false;
			}
		}
	}
	if(key == 'k')
	{
		//printf("%d\n%d\n",score,lives);
		printf("%d\n", highScore);
	}
	if(key == 'm')
	{
		if(musicOn)
		{
			musicOn = false;
			PlaySound(0, 0, 0);
		}
		else if(!musicOn)
		{
			musicOn = true;
			PlaySound("bf.wav", NULL, SND_LOOP | SND_ASYNC);
		}
	}
	
}


void iSpecialKeyboard(unsigned char key)
{

	
	if(insideGame)
	{
		if (key == GLUT_KEY_RIGHT)
		{
			if(img_x <= window_x - size_x)
			{
				goToRight = true;
				goToLeft = false;
			}
		}
		if (key == GLUT_KEY_LEFT)
		{
			if(img_x >= 0)
			{
				goToRight = false;
				goToLeft = true;
			}
		}
		if (key == GLUT_KEY_DOWN)
		{
			goToRight = false;
			goToLeft = false;
		}
	
		if (key == GLUT_KEY_END)
		{
			gameMenu = true;
			insideGame = false;
			score = 0;
			_time = 0;
			normalScore = 0;
			nukeScore = 0;
			enemySpeed = 5;
			for(int i = 0; i < 3; i++)
			{
				enm[i].y = window_y;
			}
		}
	}
	
}


void change()
{
	if(insideGame)
	{
		if(!paused)
		{
			_time++;
			if(_time % 15 == 0 && _time >= 15)
			{
				enemySpeed += 2;
			}

			if(countForSeconds)
			{
				seconds++;
			}
		}
	}
}

void initializeAll()
{
	double sum = 0;
	for(int i = 10; i >= 0; i--)
	{
		bg[i].x = 0;
		bg[i].y = sum;
		sum += 72;

		//initializing y axis of nuke
		nukeAxis[i].y = 0;
	}
}



int main()
{
	iSetTimer(1000, change);
	if(musicOn)
	{
		PlaySound("bf.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	readHighScore();
	initializeAll();
	
	iInitialize(window_x, window_y, "Space Odyssey");
	iStart();
	return 0;
}