/*
#include <iostream> 	// Input output stream
#include <string> 	// Strings
#include <limits> 	// MIN and MAX values for datatypes
#include <vector> 	// Vectors
#include <sstream> 	// String streams
#include <numeric> 	// Sequences of values
#include <ctime> 	// Time
#include <cmath> 	// Math functions
#include <cstdlib>
*/
// C Standard library (converting functions, rand num gen, memory management, searching, sorting)
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "fps.cpp"
using namespace sf;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
// Left or Right 
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];


int main() {	// argc - number of arguments | argv - array of pointers to strings

	// Create a video mode object
	VideoMode vm(1440, 900);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	window.setKeyRepeatEnabled(true);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(620, 0);

	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 650);

	// Is the bee currently moving?
	bool beeActive = false;

	// How fast can the bee fly
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	Texture textureCloud;

	// Load 1 new texture
	textureCloud.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\cloud.png");

	// 3 New sprites with the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// Position the clouds on the left of the screen
	// at different heights
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 200);
	spriteCloud3.setPosition(0, 400);

	// Are the clouds currently on the screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Variables to control time itself
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1440 / 2) - timeBarStartWidth / 2, 800);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track if the game is running
	bool paused = true;

	// Draw some text
	int score = 0;

	Text messageText;
	Text scoreText;
	Text fpsText;

	// We need to choose a font
	Font font;
	font.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\fonts\\myfont.ttf");

	// Set the font to out message
	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score = 0");
	fpsText.setFont(font);

	// Make it big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	fpsText.setCharacterSize(100);

	// Choose color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	fpsText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(
		textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1440 / 2.0f, 900 / 2.0f);
	scoreText.setPosition(20, 20);
	fpsText.setPosition(20, 120);


	// Prepare 6 branches
	Texture textureBranch; textureBranch.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(430, 620);

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(2000, 620);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(530, 720);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 530;
	const float AXE_POSITION_RIGHT = 930;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\graphics\\log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(620, 700);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	

	// Prepare the sounds
	// The player chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\sound\\chop.ogg");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// The player has met his end under a branch
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\sound\\death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("C:\\c++\\adudek\\visual1\\Timber\\sound\\out_of_time.flac");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	FPS fps;

	while (window.isOpen()) {
		/*
		****************************************
		Handle the players input
		****************************************
		*/

		//Creating a window
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyReleased && !paused) {
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}




			//Closing window when ESC is pressed
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
			// Start the game
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
				paused = false;

				// Reset the time and the score
				score = 0;
				timeRemaining = 6;


				// Make all the branches disappear -
				// starting in the second position
				for (int i = 1; i < NUM_BRANCHES; i++) {
					branchPositions[i] = side::NONE;
				}

				// Make sure the gravestone is hidden
				spriteRIP.setPosition(675, 2000);

				// Move the player into position
				spritePlayer.setPosition(430, 620);
				acceptInput = true;
			}
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput) {
			// First handle pressing the right cursor key
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
				// Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(840, 620);
				// Update the branches
				updateBranches(score);
				// Set the log flying to the left
				spriteLog.setPosition(620, 700);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
			// Handle the left cursor key
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
				// Make sure the player is on the left
				playerSide = side::LEFT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(430, 620);

				// update the branches
				updateBranches(score);

				// set the log flying
				spriteLog.setPosition(620, 700);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
		}


		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused) {

			// Measure time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1440 / 2.0f, 900 / 2.0f);

				// Play the out of time sound
				outOfTime.play();
			}

			// Setup the bee
			if (!beeActive) {
				// How fast is the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				//How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 400) + 400;
				spriteBee.setPosition(1500, height);
				beeActive = true;
			}
			else {
				// Move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// Has the bee reached the left hand edfe of the screen?
				if (spriteBee.getPosition().x < -100) {
					// Set it up ready to be a whole new bee next frame
					beeActive = false;
				}
			}

			// Clouds
			if (!cloud1Active) {

				// Cloud speed
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// Cloud height
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-400, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed* dt.asSeconds()),
					spriteCloud1.getPosition().y);

				// Has the cloud reached the right edge of the screen?
				if (spriteCloud1.getPosition().x > 1440) {
					// Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}

			if (!cloud2Active) {
				// Cloud speed
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// Cloud height
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-400, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed* dt.asSeconds()),
					spriteCloud2.getPosition().y);

				// Has the cloud reached the right edge of the screen?
				if (spriteCloud2.getPosition().x > 1440) {
					// Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			if (!cloud3Active) {
				// Cloud speed
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// Cloud height
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-400, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed* dt.asSeconds()),
					spriteCloud3.getPosition().y);

				// Has the cloud reached the right edge of the screen?
				if (spriteCloud3.getPosition().x > 1440) {
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			// Update the score text
			std::stringstream ss;
			ss << "Score1 = " << score;
			scoreText.setString(ss.str());

			// Update fps text
			fps.update();
			std::ostringstream oss;
			oss << "FPS = " << fps.getFPS();
			fpsText.setString(oss.str());

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					// Move the sprite to the left side
					branches[i].setPosition(410, height);

					// Flip the sprite round the other way
					branches[i].setRotation(180);

				}
				else if (branchPositions[i] == side::RIGHT) {
					// Move the sprite to the right side
					branches[i].setPosition(1040, height);

					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else {
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 1450) {

					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(620, 700);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide) {
				// death
				paused = true;
				acceptInput = false;

				if (playerSide == side::LEFT) {
					// Draw the gravestone
					spriteRIP.setPosition(420, 620);
				}
				else {
					spriteRIP.setPosition(830, 620);
				}



				// hide the player
				spritePlayer.setPosition(2000, 660);

				// hide the axe
				spriteAxe.setPosition(2000, 720);

				// hide the log
				spriteLog.setPosition(2000, 720);

				// Change the text of the message
				messageText.setString("Game Over");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1440 / 2.0f, 900 / 2.0f);

				// Play the death sound
				death.play();
			}

		} // end of paused IF statement
		/*
		****************************************
		Draw the scene
		****************************************
		*/

		window.clear();	// Clear everything from the last frame

		// Draw our game scene here
		window.draw(spriteBackground);

		// Draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}


		// Draw the tree
		window.draw(spriteTree);

		// Draw the player
		window.draw(spritePlayer);

		// Draw the axe
		window.draw(spriteAxe);
		// Draw the flying log
		window.draw(spriteLog);

		// Draw the gravestone
		window.draw(spriteRIP);

		// Draw the insect
		window.draw(spriteBee);

		// Draw pause message
		if (paused) {
			window.draw(messageText);
		};

		

		// Draw score
		//if(!paused){
		window.draw(scoreText);
		window.draw(fpsText);
		//};


		// Draw the timebar
		window.draw(timeBar);


		window.display();	// Show everything we just drew

		
	}
	return 0;
}


// Function definition
void updateBranches(int seed) {
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 3);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT; break;
	case 1:
		branchPositions[0] = side::RIGHT; break;
	default:
		branchPositions[0] = side::NONE; break;
	}
}



/* #include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Title");

	sf::Event event;

	while (window.isOpen()) 
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}

	return 0;
}
*/
