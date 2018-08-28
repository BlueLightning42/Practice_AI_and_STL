#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <chrono>

class Bot{
private:
	std::vector<double> weigths;
	static int object_count;
	int bot_number;
	int strength;
	int health;
	int max_health;
	int _temp_max_health; //punish always healing.
public:
	void remake(const Bot&, std::vector<bool>, double, int);
	Bot(std::vector<double>, int);
	~Bot() = default;

	inline int   get_health() const{return this->health;}
	inline int   get_max_health() const{return this->max_health;}
	inline int   get_strength() const{return this->strength;}
	inline bool  alive() const{return (this->health > 0);}
	inline float get_weigths(int) const;
	inline void  set_max();
	inline void  get_damaged(int);
	void print_stats() const;
	void attack(Bot&);
	void death_match(Bot&);
	void heal_self();
};



template<
	class Generator = std::mt19937
> class Pool {
public:
    typedef Generator generator_type;
	typedef std::uniform_real_distribution<double> 	distribution_type1;
	typedef std::uniform_int_distribution<int> 		distribution_type2;
	typedef std::bernoulli_distribution				distribution_type3;

	// default constructor
    explicit Pool(Generator&& _eng 
        = Generator{
			// In case random device is deterministic chrono is used as if it was time(0)
			static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) ^
			// Also use random device seed just in case its been implemented to get better random results
			std::random_device{}()
			}) : engine(std::move(_eng)) {}
    // construct from existing pre-defined engine
    explicit Pool(const Generator& _eng)
        : engine(_eng) {}
	
	double get_random_float() { return this->dist_real(this->engine); }
	int get_random_int() 	  { return this->dist_int(this->engine); }

	void fill_to(){
		for(int _ = fighters.size(); _ < this->max_pool_size; _++){
			std::vector<double> rand_vec(10);
			for(int i = 0; i<10; i++){
				rand_vec.push_back(get_random_float());
			}
			this->fighters.emplace_back(rand_vec, this->get_random_int());
		}
	}
	void fill_to(int pool_size){
		this->max_pool_size = pool_size;
		this->fill_to();
	}
	void cycle(int amount){
		this->cycle_amount = amount;
		std::vector<double> rand_vec(10);
		bool winner=1;
		int num=0;
		std::vector<bool> mask;
		while(amount){
			std::shuffle ( fighters.begin(), fighters.end(), engine);
			for(auto itter = this->fighters.begin(); itter <fighters.end()-1; itter += 2){
				itter->death_match(*(itter+1));
				if ( !itter->alive() && !(itter+1)->alive()) {
					//replace both bots with completly new ones in a tie.
					rand_vec.clear();
					for(int i = 0; i<10; i++){
						rand_vec.push_back(get_random_float());
					}
					itter = fighters.erase(itter);  
					itter = fighters.insert(itter,Bot(rand_vec, this->get_random_int()));
					
					rand_vec.clear();
					for(int i = 0; i<10; i++){
						rand_vec.push_back(get_random_float());
					}
					(itter+1) = fighters.erase((itter+1));  
					(itter+1) = fighters.insert((itter+1),Bot(rand_vec, this->get_random_int()));
				}
				winner = itter->alive();
				//std::generate(mask.begin(), mask.end(), [&engine, &dist_bool] { return dist_bool(engine); });
				mask.clear();
				for(int i = 0; i<5; i++){
					mask.push_back(dist_bool(engine));
				}

				(*(itter+!winner)).remake(*(itter + winner), mask, get_random_float(), get_random_int());
			}
		amount--;
		}
	}
	void till_one_remains(){
		std::vector<Bot> winners;
		bool winner;
		while(this->fighters.size() > 1){
			//std::shuffle ( this->fighters.begin(), this->fighters.end(), this->engine);
			for(auto itter = this->fighters.begin(); itter <fighters.end()-1; itter += 2){
				itter->death_match(*(itter+1));
				if ( !itter->alive() && !(itter+1)->alive()) continue;
				winner = itter->alive();
				winners.push_back(*(itter+winner));
			}
			if(winners.size() == 0) break;
			this->fighters = winners;
			winners.clear();
		}
		std::cout << "\n...and cycled " << cycle_amount << " times\n with a pool " << max_pool_size << " large the winner is" << std::endl;
		fighters.at(0).print_stats();
	}
private:
    generator_type engine;
    distribution_type1 	dist_real{-50.0, 50.0};
	distribution_type2	dist_int{-10, 10};
	distribution_type3 	dist_bool{0.5}; //50% chance
	std::vector<Bot> fighters;
	int max_pool_size = 1000;
	int cycle_amount = 0;
};








