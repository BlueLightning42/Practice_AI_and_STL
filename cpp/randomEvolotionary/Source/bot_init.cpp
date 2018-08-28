#include "pool.h"



float Bot::get_weigths(int i) const{
	if (i < 8) {return this->weigths.at(i);}
	/* else */ return 0;
}
void Bot::set_max(){
	this->_temp_max_health = this->max_health;
	this->health = this->max_health;
}

void Bot::print_stats() const{
	std::cout << "\nGeneric bot #" << this->bot_number << 
	"!\nMax Health:" << this->max_health << 
	"\nStrength:" << this->strength << 
	"\nWeigths/how important it is to heal:\n\tOwn Health~\t" << weigths[0] <<
	"x\n\tOther Health~\t" << weigths[2] <<
	"x\n\tOwn Strength~\t" << weigths[1] <<
	"x\n\tOther Strength~\t" << weigths[3] << 'x' << std::endl;
	
}

int Bot::object_count = 0;
Bot::Bot(std::vector<double> w, int p): weigths(w), max_health(std::abs(p)), health(std::abs(p)), strength(110-std::abs(p)) {
	this->bot_number = object_count;
	object_count++;
}

void Bot::remake(const Bot &b, std::vector<bool> mask, double mutation_factor, int newpersonality){
	for(int i=0;i<8;i++){
		if (mask[i]){
			this->weigths[i] = b.get_weigths(i) + mutation_factor;
		}
	}
	if( newpersonality == 0 ) newpersonality=1;
	this->max_health = std::abs(((newpersonality) + b.get_max_health())/2);
	if(this->max_health <= 1) this->max_health = 1;
	this->health = this->max_health;
	this->strength = 110 - this->max_health;
}

void Bot::death_match(Bot& other){
	//continually attack each other unstil self or other bot's life reaches 0
	this->set_max();
	other.set_max();
	while(true){
		this->attack(other);
		other.attack(*this);
		if(!this->alive()) return;
		if(!other.alive()) return;
	}
}

void Bot::attack(Bot& other){
	bool _info = weigths[0] * this->get_health() + weigths[1] * this->get_strength()   > weigths[2] * other.get_health() + other.get_strength() * weigths[3];
	if(_info){
		other.get_damaged(this->get_strength());
	}else{
		this->heal_self();
	}
}

void Bot::get_damaged(int dmg){
	this->health -= dmg;
}
void Bot::heal_self(){
	this->_temp_max_health -= 2;
	this->health += this->_temp_max_health/2;
	if (this->_temp_max_health <= 0) {
		this->health = -1;
		return;
	}
	this->health %= this->_temp_max_health;
}
