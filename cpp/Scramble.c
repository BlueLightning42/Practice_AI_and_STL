/*
Really simple mutation of a random seed in char form to a "longer" random seed in binary form...
Could be used to create a sharable character sequence for a game that turns into a seed used for various game states...
None cryptographically secure "mutation" function used simply to make different seeds create drastically different binary seeds...
basically just to make sure similar character sequences create different seeds so changing one character appears to create an entirely different seed.
trying to make sure its non destructive so the original seed can be extracted easily.
*/



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

unsigned int int_to_bin(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}
void print_c_b(unsigned char* c){
  unsigned int temp;
  for(int i=0; i<11;i++){
    temp = (unsigned int)(c[i]);
    temp = int_to_bin(temp);
    printf("%08u ",temp);
  } puts("");
}
void print_u_b(uint32_t n){
  printf("%08u %08u %08u %08u \n",
  			int_to_bin((n>>24) & 0xff),
  			int_to_bin((n>>16) & 0xff),
  			int_to_bin((n>>8 ) & 0xff),
  			int_to_bin((n>>0 ) & 0xff));
}


uint32_t mutate(uint32_t A){
  A = ( (A & 0xFFFF0000) >> 16) ^ ((A & 0x0000FFFF) << 16);
  A = ( (A & 0x0F0F0F0F) << 4) ^ ( (A & 0xF0F0F0F0) >> 4);
  A = ( (A & 0x55555555) << 1) ^ ( (A & 0xAAAAAAAA) >> 1);
  return A;
}
uint32_t unmutate(uint32_t A){
  A = ( (A & 0x55555555) << 1) ^ ( (A & 0xAAAAAAAA) >> 1);
  A = ( (A & 0x0F0F0F0F) << 4) ^ ( (A & 0xF0F0F0F0) >> 4);
  A = ( (A & 0xFFFF0000) >> 16) ^ ((A & 0x0000FFFF) << 16);
  return A;
}

char map[] = "nmQWaVBNvbDFGOlZXtyuipASEPsCYUIdfghjkRTerHJKLzxcqwM\0\0\0\0\0\0\0\0\0\0\0\0";
void string_to_seed(unsigned char* stored_seed, unsigned char* seed_string, int length){
  //basically base 51 (A-Za-z minus o)
  for (int i=0; i<length; i++){
    if(seed_string[i] == 'o') seed_string[i] = 'O'; // replace o's with capitial o's
    for (int m=0; m<51; m++){
      if (map[m] == seed_string[i]){
        stored_seed[i] = m * (i%5+1);
      }
    }
  }
  // continue filling 10 (slightly different) bytes even if the string is less than 10 characters
  for (int i=length; i<10; i++){
    if(seed_string[i] == 'o') seed_string[i] = 'O'; // replace o's with capitial o's
    for (int m=0; m<51; m++){
      if (map[m] == seed_string[i-length])
        stored_seed[i] = m * (i%5+1);
    }
  }
  stored_seed[10] = length-6;
}

void l_seed_to_string(unsigned char* stored_seed, unsigned char* seed_string, int length){
  //basically base 51 (A-Za-z minus o)
  for (int i=0; i<length; i++){//only the first 6 characters matter
    seed_string[i] = map[ (stored_seed[i])/(i%5+1) ];
  }
  seed_string[length] = 0;//null terminate it
}

void c_seed_to_string(unsigned char * seed, uint32_t character_seed){
  character_seed = mutate(character_seed);

  for(int i=0; i<6; i++){
    seed[i] = map[character_seed % 51];
    //printf("%d\n",seed[i]);//debug
    character_seed /= 51;
  }
}
uint32_t string_to_c_seed(unsigned char * seed){
  uint32_t character_seed = 0;

  for(int i=5; i>=0; i--){
    character_seed += map[seed[i]] * (51*(6-i));
    //printf("%d\n",map[seed[i]]);//debug
    for (int m=0; m<51; m++){
      if (map[m] == seed[i])
       character_seed = character_seed*51 + m;
    }
  }
  if(character_seed==0) puts("ERROR: Character seed cannot be generated from string");
  return unmutate(character_seed);
}



unsigned long long values[] = 
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void count_values(uint32_t seed, unsigned long long* values){
  for(int i = 0; i<51;i++){
    values[i] += 0x1 & seed;
    seed = seed >> 1;
  }
}

void generate_seed(/**param**/){
  uint32_t character_seed;
  char seed[]   = "__________";
  char storage[]= "___________";
  while(1|1 /**valid**/){
    //why can't I have a guarentee RAND_MAX will be this large uggg
    character_seed = (( (uint32_t) rand() << 16) & 0xFFFF0000) |
                     (( (uint32_t) rand() << 0 ) & 0x0000FFFF);
    break;
    //check random againsed param make sure luck isn't super high or low if not playing on hard
  }
  c_seed_to_string((unsigned char *)(&seed), character_seed);
  // map is 64 characters 13 of which are null and a chance of cutting the string short.
  seed[6] = map[rand()&0b111111];
  seed[7] = map[rand()%0b111111];
  seed[8] = map[rand()%0b111111];
  seed[9] = map[rand()%0b111111];
  printf("Generated seed: \t%s\nSeed's binary form: \t", seed);
  print_u_b(character_seed);
  puts("Stored Seed's Binary Form:\t");
  string_to_seed((unsigned char *)(&storage), (unsigned char *)(&seed),strlen(seed));
  print_c_b((unsigned char *)(&storage));
  l_seed_to_string((unsigned char *)(&storage), (unsigned char *)(&seed),((int)storage[10] &0b111)+6);
  printf("Refound Seed: \t\t%s\nBinary Form:\t\t",seed);
  print_u_b(string_to_c_seed((unsigned char *)(&seed)));
  puts("");

}

int main(void) {
  srand(time(NULL));
  for(int i=0; i<10; i++){
    generate_seed();
  }
  return 0;
}
