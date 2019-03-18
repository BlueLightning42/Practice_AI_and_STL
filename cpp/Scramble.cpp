/*
different version of https://github.com/BlueLightning42/Practice_AI_and_STL/blob/master/cpp/Scramble.c

made for cpp 
same concept generate a 6-10 character "seed" along with a binary seed based on it.
I just ported the example I made into cpp...fixed errors with the conversion and changed a few langauge specific things...rand() etc

tried porting everything to bitsets but then casting between things became a nightmare.

*/

#include <iostream>
#include <chrono>
#include <random>
#include <stdint.h>
#include <bitset>
#include <string.h>
#include <cmath>

using byte = unsigned char;

// 12345678 12345678 12345678 etc -> 11111111 22222222 33333333 etc
std::bitset<96> mutate12(byte* stored_seed){
	std::bitset<96> buff;

	for(int i=0; i<8;i++){
		for(int k=0; k<8;k++){
			buff[i+k*8] = (stored_seed[i] & (1 << k)) >> k;	
		}		
	}
	for (int i = 64; i< 96; i++){
		buff[i] = (stored_seed[(i-64)/ 8] & (1 << (i%8))) >> (i%8);
	}
	return buff;
}

void print_b(byte * buff){
	for(int i=0; i < 10; i++){
		std::cout << std::bitset<8>(buff[i]);
	}
}


char char_map[] = "nmQWaVBNvbDFGOlZXtyuipASEPsCYUIdfghjkRTerHJKLzxcqwM\0\0\0\0\0\0\0\0\0\0\0\0";
void string_to_seed(byte* stored_seed, byte* seed_string, int length){
  //basically base 51 (A-Za-z minus o)
  for (int i=0; i<length; i++){
    if(seed_string[i] == 'o') seed_string[i] = 'O'; // replace o's with capitial o's
    for (int m=0; m<51; m++){
      if (char_map[m] == seed_string[i]){
        stored_seed[i] = m * (i%5+1);
      }
    }
  }
  // continue filling 10 (slightly different) bytes even if the string is less than 10 characters
  for (int i=length; i<10; i++){
    if(seed_string[i] == 'o') seed_string[i] = 'O'; // replace o's with capitial o's
    for (int m=0; m<51; m++){
      if (char_map[m] == seed_string[i-length])
        stored_seed[i] = m * (i%5+1);
    }
  }
  stored_seed[10] = length-6;
}

void l_seed_to_string(byte* stored_seed, byte* seed_string, int length){
  //basically base 51 (A-Za-z minus o)
  for (int i=0; i<length; i++){//only the first 6 characters matter
    seed_string[i] = char_map[ (stored_seed[i])/(i%5+1) ];
  }
  seed_string[length] = 0;//null terminate it
}


void c_seed_to_string(unsigned char * seed, uint32_t character_seed){
  for(int i=0; i<6; i++){
    seed[i] = char_map[character_seed % 51];

    character_seed /= 51;
  }
  seed[6] = '\0';
}


void generate_seed(/**param**/){
  uint32_t character_seed;
  char seed[]   = "__________";
  char storage[]= "___________";
  
  std::mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count() ^
	 			std::random_device{}());
	std::uniform_int_distribution<> dis1(0, 50);
	std::uniform_int_distribution<> dis2(0, 64);

	char seed_string[10];
	
	for(int i=0;i<6;i++){// first 6 characters are important minimal number of digits
		seed_string[i] = char_map[dis1(eng)];
	}	
	for(int i=6; i<10;i++){//chance of being a \0 and thus cutting last characters from seed
		seed_string[i] = char_map[dis2(eng)];
	}

  

  // map is 64 characters 13 of which are null and a chance of cutting the string short.
  std::cout << "Generated seed: "<< seed_string << '\n';
  //std::cout << "Stored Seed's Binary Form:\t";
  string_to_seed((byte *)(&storage), (byte *)(&seed_string),strlen(seed_string));
  print_b((byte *)(&storage));

  l_seed_to_string((byte *)(&storage), (byte *)(&seed_string),((int)storage[10] &0b111)+6);
  std::cout << "  mutated ↓\n";
  std::cout << mutate12((byte *)(&storage)) << '\n';
  std::cout << "Refound Seed:   " <<seed_string << "\n" << std::endl;


}

int main() {

	for(int i=0;i<10;i++)
		generate_seed();

	

	std::cout << "random number with the seed it gets: ";
	std::mt19937 eng(std::chrono::high_resolution_clock::now().time_since_epoch().count() ^
	 			std::random_device{}());
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);
	uint32_t seed = dis(eng);
	std::cout << std::endl;
	char out[]   = "__________";
	c_seed_to_string((byte*)(&out), seed);
	std::cout <<"# " << seed << "   seed: " << out;


}
