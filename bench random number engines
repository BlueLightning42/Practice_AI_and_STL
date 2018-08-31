#include <iostream>
#include <chrono>
#include <random>


template<
	class Generator = std::mt19937
> class Random_looper {
public:
  typedef Generator generator_type;
	typedef std::uniform_real_distribution<double> 	distribution_type1;

	// default constructor
    explicit Random_looper(Generator&& _eng 
        = Generator{
			// In case random device is deterministic chrono is used as if it was time(0)
			static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) ^
			// Also use random device seed just in case its been implemented to get better random results
			std::random_device{}()
			}) : engine(std::move(_eng)) {}
    // construct from existing pre-defined engine
    explicit Random_looper(const Generator& _eng)
        : engine(_eng) {}
	volatile void operator()(int times_to_loop){
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		//bench of generating a bunch of random stuff
		for(int i=0;i<times_to_loop;i++){
			dist_real(this->engine);
		}

		std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> fp_ms = end - begin;
		std::cout << "   " << fp_ms.count() << "   \tmilli seconds" << std::endl;
    }	


private:
    generator_type engine;
    distribution_type1 	dist_real{-50.0, 50.0};
};


int main() {
	//Change if intrested in testing different times
	const int Times_to_loop = 100000;
	
	std::cout << "Bench of all the standard random engines on REPL...results may vary based on implementation/what operating system and archetecture and optomisation level is used so dont use these as law but just a general idea\n" << std::endl;
	
	//multiple scopes to force RAII to delete each engine after they leave...effects performance? for some reason?
	std::cout << "\n(what java uses/inaccurate but fast)\nlinear congruential engines" << std::endl;
	std::cout << "-  minstd_rand0" << std::endl;
{	Random_looper<std::minstd_rand0> loop_1;
	loop_1(Times_to_loop);
	loop_1(Times_to_loop);
	loop_1(Times_to_loop);
}
	std::cout << "-  minstd_rand" << std::endl;
{	Random_looper<std::minstd_rand> loop_2;
	loop_2(Times_to_loop);
	loop_2(Times_to_loop);
	loop_2(Times_to_loop);
}

	std::cout << "\n(most popular engine/higher period/most \"random\")\nmersenne twister engines " << std::endl;

	std::cout << "-  mt19937" << std::endl;
{	Random_looper<std::mt19937> loop_3;
	loop_3(Times_to_loop);
	loop_3(Times_to_loop);
	loop_3(Times_to_loop);
}
	std::cout << "-  mt19937_64" << std::endl;
{	Random_looper<std::mt19937_64> loop_4;
	loop_4(Times_to_loop);
	loop_4(Times_to_loop);
	loop_4(Times_to_loop);
}

	std::cout << "\n(can lead to really good results on certain processors but harder to tune)\nsubtract with carry engines" << std::endl;

	std::cout << "-  ranlux24_base" << std::endl;
{	Random_looper<std::ranlux24_base> loop_5;
	loop_5(Times_to_loop);
	loop_5(Times_to_loop);
	loop_5(Times_to_loop);
}
	std::cout << "-  ranlux48_base" << std::endl;
{	Random_looper<std::ranlux48_base> loop_6;
	loop_6(Times_to_loop);
	loop_6(Times_to_loop);
	loop_6(Times_to_loop);
}

	std::cout << "\n(honestly just use this if you want to be hipster theres no benefit)\ndiscard block engines" << std::endl;
	std::cout << "-  ranlux24" << std::endl;
{	Random_looper<std::ranlux24> loop_7;
	loop_7(Times_to_loop);
	loop_7(Times_to_loop);
	loop_7(Times_to_loop);
}
	std::cout << "-  ranlux48" << std::endl;
{	Random_looper<std::ranlux48> loop_8;
	loop_8(Times_to_loop);
	loop_8(Times_to_loop);
	loop_8(Times_to_loop);
}

	std::cout << "\n(takes another random generator and randomizes its results...)\nshuffle order engine" << std::endl;
	std::cout << "-  knuth_b" << std::endl;
{	Random_looper<std::knuth_b> loop_9;
	loop_9(Times_to_loop);
	loop_9(Times_to_loop);
	loop_9(Times_to_loop);
}

}
