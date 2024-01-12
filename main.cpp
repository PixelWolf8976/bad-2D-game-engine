#include <iostream>
#include <conio.h>
#include <vector>
#include <string>

using namespace std;

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

int main() {
	// FullScreen is 247 chars Wide on my monitor, different for almost every other monitor

	// Vertical view distance in chars total (top to bottom) CANNOT be bigger than screen height (moves cursor to top left of screen, not terminal)
	int vvd{ 51 };

	// Horizontal view distance in chars total (left to right)
	int hvd{ 100 };

	// The character the player plays as
	char playerChar{ '0' };

	// Available keys (Can have multiple instances of same key, but one verticle and one horizontal
	// verticle doors: postition 0 = top, position 1 = bottom of door
	vector<Key> keys{
		Key('F', false, {{'[', ']'}, {'<', '>'}}),
		Key('F', false, {{'(', ')'}}),
		Key('F', true, {{'\\', '/'}}),
		Key('E', false, {{'{', '}'}})
	};

	// Gates to other levels
	vector<LevelGate> gates{
		LevelGate('@', 1),
		LevelGate('@', 0),
		LevelGate('R', 0)
	};

	// World map
	vector<vector<string>> levels{
		{
			"####################################################################################################",
			"#                        #                                                                         #",
			"#                        #                                                                         #",
			"#    0         F         \\                 @                                                       #",
			"#                        /                                                                         #",
			"#                        #                 R                                                       #",
			"#       F                #                                                                         #",
			"#                        #                                                                         #",
			"#            F           #                                                                         #",
			"#                        #                                                                         #",
			"#                        #                                                                         #",
			"#      E                 #                                                                         #",
			"#                        #                                                                         #",
			"#                        #                                                                         #",
			"###[]###{}####()####<>####                                                                         #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"####################################################################################################"
		},
		{
			"####################################################################################################",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#    0                          @                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"####################################################################################################"
		}
	};

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

		// Default character to place on the floor when the character moves
		char defaultFloorChar{ ' ' };

		// What character to place on the floor
		// Usually just the default floor char, but changes if you run over a key if you already have one
		char floorChar{ defaultFloorChar };

		do {
			// Part of map to display
			string display{ "" };

			// Code to overwrite display
			string displayClearer{ "\033[" + to_string(vvd * 2) + "A\r" };

			// Which way the player moves
			char movementChar{};

			// Map as a single string
			string compiledMap{ "" };

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
			cout << displayClearer << display;

			// Gets users input
			movementChar = _getch();

			// Checks and moves player's position
			if (movementChar == 'w') {
				playerPosition[1]--;
			}
			else if (movementChar == 's') {
				playerPosition[1]++;
			}
			else if (movementChar == 'a') {
				playerPosition[0] -= 2;
				l = true;
				if (playerPosition[0] < 0) {
					playerPosition[0] += 2;
				}
			}
			else if (movementChar == 'd') {
				playerPosition[0] += 2;
				if (playerPosition[0] >= map[playerPosition[1]].length()) {
					playerPosition[0] -= 2;
				}
				r = true;
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
			if (map[playerPosition[1]][playerPosition[0]] == '#') {
				playerPosition = oldCoords;
				if (l) {
					playerPosition[0]--;
					if (map[playerPosition[1]][playerPosition[0]] == '#') {
						playerPosition = oldCoords;
					}
				}
				else if (r) {
					playerPosition[0]++;
					if (map[playerPosition[1]][playerPosition[0]] == '#') {
						playerPosition = oldCoords;
					}
				}
			}
			else if (map[playerPosition[1]][playerPosition[0] + 1] == '#' && l) {
				playerPosition = oldCoords;
			}
			else if (map[playerPosition[1]][playerPosition[0] - 1] == '#' && r) {
				playerPosition = oldCoords;
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
					map[oldCoords[1]][oldCoords[0]] = '0';
					currLevel = gate.levelTo;
					wentThroughGate = true;
					floorChar = ' ';
					break;
				}
				else if (l && map[playerPosition[1]][playerPosition[0] + 1] == gate.gateChar && gate.levelTo != currLevel) {
					map[oldCoords[1]][oldCoords[0]] = '0';
					currLevel = gate.levelTo;
					wentThroughGate = true;
					floorChar = ' ';
					break;
				}
				else if (r && map[playerPosition[1]][playerPosition[0] - 1] == gate.gateChar && gate.levelTo != currLevel) {
					map[oldCoords[1]][oldCoords[0]] = '0';
					currLevel = gate.levelTo;
					wentThroughGate = true;
					floorChar = ' ';
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

			map[playerPosition[1]][playerPosition[0]] = '0';
		} while (true);
	}

	return 0;
}
