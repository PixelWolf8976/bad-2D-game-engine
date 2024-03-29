#pragma once

#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <exception>
#include <queue>
#include <limits>
#include <set>
#include <climits>
#include <algorithm>
#include <list>

using namespace std;

class EnemyDamageOutOfRange : public exception {
private:
	string errorMessage{};

public:
	EnemyDamageOutOfRange(const char* message) : errorMessage(message) {}

	const char* what() const noexcept override {
		return errorMessage.c_str();
	}
};

class Engine {
private:
	class Key {
	public:
		char keyChar{}; // Character that represents this key
		bool hasKey{ false }; // If the player has this key
		bool verticalDoors{}; // Set to true if door is up / down, not left / right

		// Doors this key can open 
		// Only a vector vector char so you don't run out of characters to use (can use same key on multiple sets of doors)
		// In each vector of chars, the char on the left is the left most side of the door, char on right is right most side of door
		// Door can only be two chars wide or tall
		vector<vector<char>> openableDoors{};

		/*
			keyChar: Character that represents this key
			openableDoors: Doors this key can open

			verticle doors: postition 0 = top, position 1 = bottom of door
		*/
		Key(char keyChar, bool verticalDoors, vector<vector<char>> openableDoors) {
			this->keyChar = keyChar;
			this->verticalDoors = verticalDoors;
			this->openableDoors = openableDoors;
		}
	};

	// Gateways to different levels
	class LevelGate {
	public:
		char gateChar{}; // Character that represents this gate
		int levelTo{}; // Level this gate transports the player to

		LevelGate(char gateChar, int levelTo) {
			this->gateChar = gateChar;
			this->levelTo = levelTo;
		}
	};

	// Idk, used in all enemies so may as well put here
	class Enemy {
	public:
		vector<int> enemyCoords{}; // Position of enemy
		int level{}; // What level the enemy is on
		char enemyChar{}; // The char that represents this enemy
		char floorChar{}; // character that the enemy places
		int damage{}; // -1 for insta kill otherwise in hp

		Enemy(vector<int> enemyCoords, int level, char enemyChar, int damage) {
			this->enemyCoords = enemyCoords;
			this->level = level;
			this->enemyChar = enemyChar;
			this->damage = damage;

			if (damage < -1) {
				throw EnemyDamageOutOfRange("Damage must be greater than -1!");
			}
		}
	};

	class TargetingEnemy : public Enemy {
	public:
		int speed{}; // Speed of enemy (player speed is 2)
		int upCount{ 0 }; // Checks how many attempts have been made to move up

		TargetingEnemy(vector<int> enemyCoords, int level, char enemyChar, int damage, int speed) : Enemy(enemyCoords, level, enemyChar, damage) {
			this->speed = speed;
		}

		// Returns path for ai to follow
		vector<vector<int>> pathfind(vector<string> map, vector<char> wallChars, char playerChar) {

			// Find player position
			vector<int> playerPosition;

			for (int y = 0; y < map.size(); y++) {
				for (int x = 0; x < map[y].length(); x++) {
					if (map[y][x] == playerChar) {
						playerPosition = { x, y };
					}
				}
			}

			// Initialize visited array to keep track of visited cells
			vector<vector<bool>> visited(map.size(), vector<bool>(map[0].length(), false));

			// Initialize queue for BFS
			queue<vector<int>> q;

			// Starting position of the enemy
			vector<int> start = { enemyCoords[0], enemyCoords[1] };

			// Enqueue the starting position
			q.push(start);
			visited[start[1]][start[0]] = true;

			// Vector to store the path
			vector<vector<int>> path;

			// 2D vector to store parent information for path reconstruction
			vector<vector<vector<int>>> parent(map.size(), vector<vector<int>>(map[0].length()));

			// Directions for movement (up, down, left, right)
			vector<vector<int>> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

			while (!q.empty()) {

				vector<int> current = q.front();
				q.pop();

				// Check if the current position is the player's position
				if (current == playerPosition) {
					// Reconstruct the path from the player's position to the enemy's position
					vector<int> currentPos = playerPosition;
					while (currentPos != start) {
						path.push_back(currentPos);
						currentPos = parent[currentPos[1]][currentPos[0]];
					}

					reverse(path.begin(), path.end());
					return path;
				}

				// Explore neighbors
				for (auto dir : directions) {

					int nx = current[0] + dir[0];
					int ny = current[1] + dir[1];

					// Check if the neighbor is within the map boundaries and is not a wall
					if (nx >= 0 && ny >= 0 && ny < map.size() && nx < map[ny].size() && !visited[ny][nx] &&
						find(wallChars.begin(), wallChars.end(), map[ny][nx]) == wallChars.end()) {
						q.push({ nx, ny });
						visited[ny][nx] = true;
						parent[ny][nx] = current;
					}
				}
			}

			// No path found
			return {};
		}

	private:
		vector<vector<int>> parent;
	};

	/*=============*
	 *  Variables  *
	 *=============*/

	// Vertical view distance in chars total (top to bottom) CANNOT be bigger than screen height (moves cursor to top left of screen, not terminal for now)
	int vvd{1000};

	// Horizontal view distance in chars total (left to right)
	int hvd{1000};

	// The character the player plays as
	char playerChar{};

	// Default character to place on the floor when the character moves
	char defaultFloorChar{ ' ' };

	// Movement characterters
	char upChar{ 'w' }, downChar{ 's' }, leftChar{ 'a' }, rightChar{ 'd' }, waitChar{ ' ' };

	// The characters that represent walls (seperate from doors)
	vector<char> wallChars{};

	// Available keys (Can have multiple instances of same key, but one verticle and one horizontal
	// verticle doors: postition 0 = top, position 1 = bottom of door
	vector<Key> keys{};

	// Gates to other levels
	vector<LevelGate> gates{};

	// Targeting enemies
	vector<TargetingEnemy> missiles{};

	// World map
	vector<vector<string>> levels{};

	/*==============================================*
	 *
	 * Methods for user (as in people using engine) *
	 *
	 *==============================================*/
public:
	// Set the default floor character
	// @param - floorChar: Default floor char; what enemies and the player place on the ground
	void setFloorChar(char floorChar) {
		defaultFloorChar = floorChar;
	}

	// Set the characters of movement
	// @param - upChar: Character for player to type to move up
	// @param - downChar: Character for player to type to move down
	// @param - leftChar: Character for player to type to move left
	// @param - rightChar: Character for player to type to move right
	// @param - waitChar: Character for player to type to wait one turn
	void setControls(char upChar, char downChar, char leftChar, char rightChar, char waitChar) {
		this->upChar = upChar;
		this->downChar = downChar;
		this->leftChar = leftChar;
		this->rightChar = rightChar;
		this->waitChar = waitChar;
	}

	// Set the view distances
	// @param - verticalViewDistance: How many characters the player can see up & down
	// @param - horizontalViewDistance: How many characters the player can see left & right (usually 2x bigger than verticalViewDistance)
	void setViewDistances(int verticalViewDistance, int horizontalViewDistance) {
		vvd = verticalViewDistance;
		hvd = horizontalViewDistance;
	}


	// Set the player's character
	// @param - playerChar: Character used to represent the player
	void setPlayerChar(char playerChar) {
		this->playerChar = playerChar;
	}

	// Add a character recognised as a wall (Doors are automatically added)
	// @param - wallChar: Character to be recognised as a wall
	void addWall(char wallChar) {
		wallChars.push_back(wallChar);
	}

	// Add a key and door to the game																																						
	// Doors: position 0 = left, position 1 = right																																					
	// Verticle doors: postition 0 = top, position 1 = bottom
	// @param - keyChar: Character that represents this key
	// @param - isVerticalDoor: If this door is vertical or horizontal; true for vertical, false for horizontal
	// @param - openableDoors: What doors this key can open
	void addKey(char keyChar, bool isVerticalDoor, vector<vector<char>> openableDoors) {
		keys.push_back(Key(keyChar, isVerticalDoor, openableDoors));
	}

	// Add a gateway to different levels
	// @param - gateChar: Character that represents this gate
	// @param - levelTo: What level this gate brings the player to
	void addGate(char gateChar, int levelTo) {
		gates.push_back(LevelGate(gateChar, levelTo));
	}

	// Add a targeting enemy to the game
	// @param - enemyCoords: Coordinates of the enemy
	// @param - level: What level the enemy is on
	// @param - enemyChar: The character that represents this enemy
	// @param - damage: The amount of damage the enemy does to the player; -1 for intant kill
	// @param - speed: Speed of the enemy; player speed is 2
	void addTargetingEnemy(vector<int> enemyCoords, int level, char enemyChar, int damage, int speed) {
		missiles.push_back(TargetingEnemy(enemyCoords, level, enemyChar, damage, speed));
	}

	// Add a single map to the end of the game
	void addMap(vector<string> map) {
		levels.push_back(map);
	}

	// Add multiple maps to the end of the game (in order)
	void addMaps(vector<vector<string>> maps) {
		for (auto map : maps) {
			levels.push_back(map);
		}
	}

	int runGame() {
		// Sets enemies floor char to default
		for (auto& enemy : missiles) {
			enemy.floorChar = defaultFloorChar;
		}

		// Player position in coordinates 
		// { x, y }
		// { 0, 1 }
		vector<int> playerPosition{};

		// What level the player is on
		int currLevel{ 0 };

		while (true) {
			vector<string>& map = levels[currLevel];

			// Gets player's start location
			for (int y = 0; y < map.size(); y++) {
				for (int x = 0; x < map[y].length(); x++) {
					if (map[y][x] == playerChar) {
						playerPosition = { x, y };
					}
				}
			}

			if (hvd > map[playerPosition[1]].length()) {
				hvd = map[playerPosition[1]].length();
			}

			if (vvd > map.size()) {
				vvd = map.size();
			}

			// What character to place on the floor
			// Usually just the default floor char, but changes if you run over a key if you already have one
			char floorChar{ defaultFloorChar };

			do {
				// Part of map to display
				string display{ "" };

				// Which way the player moves
				char movementChar{};

				// Previous coords of character
				vector<int> oldCoords{ playerPosition };

				// If player is going left / right
				bool l{ false };
				bool r{ false };

				// Half of the vertical view distance
				int hlfVVD{ (int)((vvd / 2) + 0.5) };

				// Half of the horizontal view distance
				int hlfHVD{ (int)((hvd / 2) + 0.5) };

				// Turns map into a single string to make dynamic view distance work (with border)
				if (playerPosition[1] - hlfVVD < 0) {
					for (int i = 0; i < vvd; i++) {
						if (playerPosition[0] - hlfHVD < 0) {
							display += map[i].substr(0, hvd) + "\n";
						}
						else if (playerPosition[0] + hlfHVD >= map[i].length()) {
							display += map[i].substr(map[i].length() - hvd, hvd + 1) + "\n";
						}
						else {
							display += map[i].substr(playerPosition[0] - hlfHVD, hvd) + "\n";
						}
					}
				}
				else if (playerPosition[1] + hlfVVD >= map.size()) {
					for (int i = map.size() + 0.5 - vvd; i < map.size(); i++) {
						if (playerPosition[0] - hlfHVD < 0) {
							display += map[i].substr(0, hvd) + "\n";
						}
						else if (playerPosition[0] + hlfHVD >= map[i].length()) {
							display += map[i].substr(map[i].length() - hvd, hvd + 1) + "\n";
						}
						else {
							display += map[i].substr(playerPosition[0] - hlfHVD, hvd) + "\n";
						}
					}
				}
				else {
					for (int i = (playerPosition[1] - hlfVVD); i <= (playerPosition[1] + hlfVVD) - 1; i++) {
						if (playerPosition[0] - hlfHVD < 0) {
							display += map[i].substr(0, hvd) + "\n";
						}
						else if (playerPosition[0] + hlfHVD >= map[i].length()) {
							display += map[i].substr(map[i].length() - hvd, hvd + 1) + "\n";
						}
						else {
							display += map[i].substr(playerPosition[0] - hlfHVD, hvd) + "\n";
						}
					}
				}

				// Output
				cout << ("\033[" + to_string(vvd * 2) + "A\r") << display;

				// Gets users input
				movementChar = _getch();

				// Checks and moves player's position
				if (movementChar == upChar) {
					playerPosition[1]--;
				}
				else if (movementChar == downChar) {
					playerPosition[1]++;
				}
				else if (movementChar == leftChar) {
					playerPosition[0] -= 2;
					l = true;
					if (playerPosition[0] < 0) {
						playerPosition[0] += 2;
					}
				}
				else if (movementChar == rightChar) {
					playerPosition[0] += 2;
					if (playerPosition[0] >= map[playerPosition[1]].length()) {
						playerPosition[0] -= 2;
					}
					r = true;
				}
				else if (movementChar == waitChar) {
					// Wait
				}
				else {
					continue;
				}

				// Checks to see if player runs into door, if so, check for key. If player has key needed, remove door
				for (auto& key : keys) {
					if (key.hasKey && !key.verticalDoors) {
						for (auto& doorVector : key.openableDoors) {
							if (map[playerPosition[1]][playerPosition[0]] == doorVector[0]) {
								map[playerPosition[1]][playerPosition[0]] = defaultFloorChar;
								map[playerPosition[1]][playerPosition[0] + 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (map[playerPosition[1]][playerPosition[0]] == doorVector[1]) {
								map[playerPosition[1]][playerPosition[0]] = defaultFloorChar;
								map[playerPosition[1]][playerPosition[0] - 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
						}
					}
					else if (key.hasKey && key.verticalDoors) {
						// Door is verticle and can open (need to stop teleporting (use other code))
						// 0 is top, 1 is bottom
						for (auto& doorVector : key.openableDoors) {
							if (map[playerPosition[1]][playerPosition[0]] == doorVector[0]) {
								map[playerPosition[1]][playerPosition[0]] = defaultFloorChar;
								map[playerPosition[1] + 1][playerPosition[0]] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (map[playerPosition[1]][playerPosition[0]] == doorVector[1]) {
								map[playerPosition[1]][playerPosition[0]] = defaultFloorChar;
								map[playerPosition[1] - 1][playerPosition[0]] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (l && map[playerPosition[1]][playerPosition[0] + 1] == doorVector[0]) {
								map[playerPosition[1]][playerPosition[0] + 1] = defaultFloorChar;
								map[playerPosition[1] + 1][playerPosition[0] + 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (l && map[playerPosition[1]][playerPosition[0] + 1] == doorVector[1]) {
								map[playerPosition[1]][playerPosition[0] + 1] = defaultFloorChar;
								map[playerPosition[1] - 1][playerPosition[0] + 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (r && map[playerPosition[1]][playerPosition[0] - 1] == doorVector[0]) {
								map[playerPosition[1]][playerPosition[0] - 1] = defaultFloorChar;
								map[playerPosition[1] + 1][playerPosition[0] - 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
							else if (r && map[playerPosition[1]][playerPosition[0] - 1] == doorVector[1]) {
								map[playerPosition[1]][playerPosition[0] - 1] = defaultFloorChar;
								map[playerPosition[1] - 1][playerPosition[0] - 1] = defaultFloorChar;
								for (auto& key1 : keys) {
									if (key1.keyChar == key.keyChar) {
										key1.hasKey = false;
									}
								}
							}
						}
					}
				}

				// Checks to see if player runs into wall or closed door
				for (auto wall : wallChars) {
					if (map[playerPosition[1]][playerPosition[0]] == wall) {
						playerPosition = oldCoords;
						if (l) {
							playerPosition[0]--;
							if (map[playerPosition[1]][playerPosition[0]] == wall) {
								playerPosition = oldCoords;
							}
						}
						else if (r) {
							playerPosition[0]++;
							if (map[playerPosition[1]][playerPosition[0]] == wall) {
								playerPosition = oldCoords;
							}
						}
					}
					else if (map[playerPosition[1]][playerPosition[0] + 1] == wall && l) {
						playerPosition = oldCoords;
					}
					else if (map[playerPosition[1]][playerPosition[0] - 1] == wall && r) {
						playerPosition = oldCoords;
					}
				}

				for (auto& key : keys) {
					for (auto doors : key.openableDoors) {
						for (auto door : doors) {
							if (map[playerPosition[1]][playerPosition[0]] == door) {
								playerPosition = oldCoords;
								if (l) {
									playerPosition[0]--;
									if (map[playerPosition[1]][playerPosition[0]] == door) {
										playerPosition = oldCoords;
									}
								}
								else if (r) {
									playerPosition[0]++;
									if (map[playerPosition[1]][playerPosition[0]] == door) {
										playerPosition = oldCoords;
									}
								}
							}
							else if (map[playerPosition[1]][playerPosition[0] + 1] == door && l) {
								playerPosition = oldCoords;
							}
							else if (map[playerPosition[1]][playerPosition[0] - 1] == door && r) {
								playerPosition = oldCoords;
							}
						}
					}
				}

				// Places floor after player moves (here so picking up stuff dosent interfere)
				map[oldCoords[1]][oldCoords[0]] = floorChar;
				if (floorChar != defaultFloorChar) {
					floorChar = defaultFloorChar;
				}

				// Checks to see if player picks up item off ground
				for (auto& key : keys) {
					if (map[playerPosition[1]][playerPosition[0]] == key.keyChar && !key.hasKey) {
						map[playerPosition[1]][playerPosition[0]] = floorChar;
						for (auto& key1 : keys) {
							if (key1.keyChar == key.keyChar) {
								key1.hasKey = true;
							}
						}
					}
					else if (l && map[playerPosition[1]][playerPosition[0] + 1] == key.keyChar && !key.hasKey) {
						map[playerPosition[1]][playerPosition[0] + 1] = floorChar;
						for (auto& key1 : keys) {
							if (key1.keyChar == key.keyChar) {
								key1.hasKey = true;
							}
						}
					}
					else if (r && map[playerPosition[1]][playerPosition[0] - 1] == key.keyChar && !key.hasKey) {
						map[playerPosition[1]][playerPosition[0] - 1] = floorChar;
						for (auto& key1 : keys) {
							if (key1.keyChar == key.keyChar) {
								key1.hasKey = true;
							}
						}
					}
					else if (map[playerPosition[1]][playerPosition[0]] == key.keyChar && key.hasKey) {
						floorChar = key.keyChar;
					}
				}

				// Bool to see player is moving to next level
				bool wentThroughGate{ false };

				// Checks to see if player passes through a gate to another level
				for (auto& gate : gates) {
					if (map[playerPosition[1]][playerPosition[0]] == gate.gateChar && gate.levelTo != currLevel) {
						map[oldCoords[1]][oldCoords[0]] = playerChar;
						currLevel = gate.levelTo;
						wentThroughGate = true;
						floorChar = defaultFloorChar;
						break;
					}
					else if (l && map[playerPosition[1]][playerPosition[0] + 1] == gate.gateChar && gate.levelTo != currLevel) {
						map[oldCoords[1]][oldCoords[0]] = playerChar;
						currLevel = gate.levelTo;
						wentThroughGate = true;
						floorChar = defaultFloorChar;
						break;
					}
					else if (r && map[playerPosition[1]][playerPosition[0] - 1] == gate.gateChar && gate.levelTo != currLevel) {
						map[oldCoords[1]][oldCoords[0]] = playerChar;
						currLevel = gate.levelTo;
						wentThroughGate = true;
						floorChar = defaultFloorChar;
						break;
					}
					else if (map[playerPosition[1]][playerPosition[0]] == gate.gateChar && gate.levelTo == currLevel) {
						floorChar = gate.gateChar;
					}
				}

				// Moves player to next level if went through gate
				if (wentThroughGate) {
					break;
				}

				map[playerPosition[1]][playerPosition[0]] = playerChar;

				// Generates vector for things enemy cannot move through
				vector<char> enemyWalls{};

				for (auto wall : wallChars) {
					enemyWalls.push_back(wall);
				}

				for (auto key : keys) {
					for (auto doors : key.openableDoors) {
						for (auto door : doors) {
							enemyWalls.push_back(door);
						}
					}
				}

				for (auto enemy : missiles) {
					enemyWalls.push_back(enemy.enemyChar);
				}

				for (auto gate : gates) {
					enemyWalls.push_back(gate.gateChar);
				}

				// Runs pathfinding on enemies and moves them accordingly
				for (auto& enemy : missiles) {
					if (currLevel == enemy.level) {
						vector<vector<int>> finalPath = enemy.pathfind(map, enemyWalls, playerChar);
						if (!finalPath.empty()) {
							vector<int> goal{};
							int steps{};
							if (finalPath.size() < enemy.speed) {
								steps = finalPath.size() - 1;
							}
							else {
								steps = enemy.speed;
							}

							// How many steps delayed path is because going up speed
							int stepsDown{ 0 };

							for (int i = 0; i < steps; i++) {
								goal = finalPath[i - stepsDown];
								if (enemy.upCount == 0 && ((goal[1] > enemy.enemyCoords[1]) || (goal[1] < enemy.enemyCoords[1]))) {
									enemy.upCount = 1;
									stepsDown++;
								}
								else if (enemy.upCount == 1 && ((goal[1] > enemy.enemyCoords[1]) || (goal[1] < enemy.enemyCoords[1]))) {
									enemy.upCount = 0;
									goal = finalPath[i - stepsDown];
									map[enemy.enemyCoords[1]][enemy.enemyCoords[0]] = enemy.floorChar;
									enemy.floorChar = map[goal[1]][goal[0]];
									map[goal[1]][goal[0]] = enemy.enemyChar;
									enemy.enemyCoords = goal;
								}
								else if (!((goal[1] > enemy.enemyCoords[1]) || (goal[1] < enemy.enemyCoords[1]))) {
									goal = finalPath[i - stepsDown];
									map[enemy.enemyCoords[1]][enemy.enemyCoords[0]] = enemy.floorChar;
									enemy.floorChar = map[goal[1]][goal[0]];
									map[goal[1]][goal[0]] = enemy.enemyChar;
									enemy.enemyCoords = goal;
								}
								else {
									goal = enemy.enemyCoords;
									map[enemy.enemyCoords[1]][enemy.enemyCoords[0]] = enemy.floorChar;
									enemy.floorChar = map[goal[1]][goal[0]];
									map[goal[1]][goal[0]] = enemy.enemyChar;
								}
							}
						}
					}

					// If enemy has reached player, end game
					if (enemy.enemyCoords == playerPosition) {
						return 0;
					}
				}
			} while (true);
		}

		return 0;
	}
};
