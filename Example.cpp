#include <vector>

#include "Engine.h"

int main() {
	// Create a new engine to use
	Engine engine{};

	// Make sure to add maps
	// Im doing it like this so it's easier to see, but you don't have to
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
			"#                     F                                                                            #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                              $                                                   #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"##########  ########################################################################################",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                                                                  #",
			"#                                                               X                                  #",
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
			"#                                                                    #                             #",
			"#    0                                                               #     X                       #",
			"#                                                                    #                             #",
			"##################################      #####################################################      #",
			"#                                #      #                                                          #",
			"#                                #      #                                                          #",
			"#                                #      #       #############################################      #",
			"#      ###########################      #                                                          #",
			"#      #            #            #      #                                                          #",
			"#      #            #            #      #      #####################################################",
			"#      #      #     #     #      #      #                                                          #",
			"#      #      #     #     #      #      #                                                          #",
			"#      #      #     #     #      #      #       ###########################################        #",
			"#      #      #     #     #      #      #                                                          #",
			"#      #      #     #     #      #      #                                                          #",
			"#      #      #     #     #      #      #####################################################      #",
			"#      #      #           #             #                                                          #",
			"#      #      #           #             #                                                          #",
			"#      #      ###########################     #      ########################################      #",
			"#      #                                      #                                             #      #",
			"#      #                                      #                                             #      #",
			"#      #                                      #                                             #      #",
			"#      ###############################################################################      #      #",
			"#                                                                                    #      #      #",
			"#                                                                                    #      #      #",
			"#                                                                                    #      #      #",
			"#      #      ###############################      ###################################      #      #",
			"#      #      #                      #      #      #                                 #      #      #",
			"#      #      #                      #      #      #                                 #      #      #",
			"#      #      #      #################      #      #                                 #      #      #",
			"#      #      #                             #      #         #      ##################      #      #",
			"#      #      #                             #      #         #                       #      #      #",
			"#      #      #      #################      #      #         #                              #      #",
			"#      #      #                      #      #      #         #                              #      #",
			"#      #      #                      #      #      #         #                              #      #",
			"#      #      ########################      #      #         #         ######################      #",
			"#      #                                    #                #         #                           #",
			"#      #                                    #                #         #                           #",
			"#      #                                    #                #         #                           #",
			"#      #############################################################################################",
			"#                                                                                                  #",
			"#                                                                                             R    #",
			"#                                                                                                  #",
			"####################################################################################################"
		}
	};

	// Add maps to game
	engine.addMaps(levels);

	// By default, floor char is a space, but use this to change it if you want
	engine.setFloorChar(' ');

	// Make sure to set what character represents the player, like this
	engine.setPlayerChar('0');

	// Add any walls you have (not including doors)
	engine.addWall('#');

	// Add keys and doors you can open
	engine.addKey('F', true, {{'\\', '/'}});
	engine.addKey('F', false, { {'<', '>'}, {'(', ')'}, {'[', ']'} });
	engine.addKey('E', false, { {'{', '}'} });

	// Add targeting enemies (go torwads the player unless there is no path)
	engine.addTargetingEnemy({64, 31}, 1, 'X', -1, 2);
	engine.addTargetingEnemy({ 75, 2 }, 2, 'X', -1, 1);

	// Add gateways between levels
	engine.addGate('@', 0);
	engine.addGate('@', 1);
	engine.addGate('$', 2);
	engine.addGate('R', 0);

	// Controlls are w, a, s, d by default with space to wait, but you can change it like this
	engine.setControls('w', 's', 'a', 'd', ' ');

	// You might need this; default is 1000 x 1000
	engine.setViewDistances(51, 100);

	// Run the game once everything is set up
	engine.runGame();
}
