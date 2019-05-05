#include "GameLoop.h"
#include <process.h>
#include <stdexcept>
#include <iostream>

int main(int argc, char *argv[])
{
 	try{
 		GAME_LOOP.init();
 		GAME_LOOP.run();
 		GAME_LOOP.destroy();
 	} catch(std::exception &e){
 		std::cerr << "Exception: " << e.what() << std::endl;
 		exit(1);
 	}

	return 0;
}