#ifndef BLOCK_H
#define BLOCK_H

#define HASH_SIZE 256
#define NONCE_SIZE 10
#define DEFAULT_DIFFICULTY 15
#define BLOCKS_TO_MINE 10
#define VALIDATION_MINUTES 1
#define VALIDATION_BLOCKS 5

#include <string>
#include <mpi.h>

using namespace std;

//Bloque de la cadena
struct Block {
  unsigned int index;
  unsigned int node_owner_number;
  unsigned int difficulty;
  unsigned long int created_at;
  char nonce[NONCE_SIZE];
  char previous_block_hash[HASH_SIZE];
  char block_hash[HASH_SIZE];
};

bool solves_problem(const string& hash);
void gen_random_nonce(char *s);
string block_to_str(const Block *block);
void block_to_hash(const Block *block, string& result);
const char* hex_char_to_bin(char c);
string hex_str_to_bin_str(const string& hex);
void define_block_data_type_for_MPI(MPI_Datatype *new_type);
bool valid_new_block(const Block *block);

#endif  // BLOCK_H
