#include <iostream>
#include <conio.h>
#include <vector>

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
	*/
	Key(char keyChar, bool verticalDoors, vector<vector<char>> openableDoors) {
		this->keyChar = keyChar;
		this->verticalDoors = verticalDoors;
		this->openableDoors = openableDoors;
	}
};

int main() {
	// FullScreen is 247 chars Wide

	// The character the player plays as
	char playerChar{ '0' };

	// Available keys (Can have multiple instances of same key, but one verticle and one horizontal
	vector<Key> keys{
		Key('F', false, {{'[', ']'}, {'<', '>'}}),
		Key('F', false, {{'(', ')'}}),
		Key('E', false, {{'{', '}'}})
	};

	// World map
	vector<string> map{
		"####################################################################################################",
		"#                        #                                                                         #",
		"#                        #                                                                         #",
		"#    0         F         #                                                                         #",
		"#                        #                                                                         #",
		"#                        #                                                                         #",
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
		"#                                                                                                  #",
		"#                                                                                                  #",
		"#                                                                                                  #",
		"#                                                                                                  #",
		"#                                                                                                  #",
		"#                                                                                                  #",
		"####################################################################################################"
	};

	// Player position in coordinates 
	// { x, y }
	// { 1, 0 }
	vector<int> playerPosition{};

	// Gets player's start location
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].length(); x++) {
			if (map[y][x] == playerChar) {
				playerPosition = { x, y };
			}
		}
	}

	// Default character to place on the floor when the character moves
	char defaultFloorChar{ ' ' };

	// What character to place on the floor
	// Usually just the default floor char, but changes if you run over a key if you already have one
	char floorChar{ defaultFloorChar };

	do {
		// Which way the player moves
		char movementChar{};

		// Map as a single string
		string compiledMap{ "" };

		// Previous coords of character
		vector<int> oldCoords{ playerPosition };

		// If player is going left / right
		bool l{ false };
		bool r{ false };

		// Turns map into a single string to make output faster
		for (int i = 0; i < map.size(); i++) {
			compiledMap += map[i] + "\n";
		}

		// Clears and re-displays the screen after every move
		system("cls");
		cout << compiledMap;

		// Gets users input
		movementChar = _getch();

		// Checks and moves player's position
		if (movementChar == 'w' || movementChar == 'H') {
			playerPosition[1]--;
		}
		else if (movementChar == 's' || movementChar == 'P') {
			playerPosition[1]++;
		}
		else if (movementChar == 'a' || movementChar == 'K') {
			playerPosition[0] -= 2;
			l = true;
		}
		else if (movementChar == 'd' || movementChar == 'M') {
			playerPosition[0] += 2;
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

		//map[oldCoords[1]][oldCoords[0]] = floorChar;
		map[playerPosition[1]][playerPosition[0]] = '0';
	} while (true);

	return 0;
}
