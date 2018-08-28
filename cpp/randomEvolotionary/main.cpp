#include "source/pool.h"

int main() {
/*
	Trying to learn std::random and templated classes 
	dumb way to do so is make a broken game where you can either attack or heal and your attack damage is inversly proportional to your max health just to see what it looked like.

	"game" is just a test and in terms of playing is broken

	however its amusing finding out the botts that had the lowest max health survived
	(hit first and fast with low healing weigths)

*/
	Pool<std::knuth_b> 	pool_1;
	std::cout << "\nGenerating Fighters" << std::endl;
	pool_1.fill_to(400);
	std::cout << "Cycling death matches" << std::endl;
	pool_1.cycle(500);
	std::cout << "Finding a single winner\nUsing the knuth_b random engine" << std::endl;
	pool_1.till_one_remains();


	Pool<std::mt19937> 	pool_2;
	std::cout << "\nGenerating Fighters" << std::endl;
	pool_2.fill_to(1000);
	std::cout << "Cycling death matches" << std::endl;
	pool_2.cycle(100);
	std::cout << "Finding a single winner\nUsing the m19937 random engine" << std::endl;
	pool_2.till_one_remains();
	

	Pool<std::minstd_rand> pool_3;
	std::cout << "\nGenerating Fighters one last time" << std::endl;
	pool_3.fill_to(5000);
	std::cout << "Cycling death matches" << std::endl;
	pool_3.cycle(1000);
	std::cout << "Finding a single winner\nUsing the m19937 random engine" << std::endl;
	pool_3.till_one_remains();

}
