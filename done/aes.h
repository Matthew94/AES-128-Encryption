#ifndef OPEN_FILE
#define OPEN_FILE

void open_file(std::ifstream &infile);

#endif

#ifndef ROW_SHIFT
#define ROW_SHIFT

void row_shift(unsigned char array[4][4]);

#endif

#ifndef S_BOX
#define S_BOX

void s_box(unsigned char array[4][4]);

#endif

#ifndef S_BOX_K
#define S_BOX_K

void s_box_k(unsigned char state[4]);

#endif

#ifndef W_ARRAY
#define W_ARRAY

void write_to_array(std::ifstream &infile, unsigned char array[4][4]);

#endif

#ifndef W_FILE
#define W_FILE

void write_to_file(unsigned char array[4][4]);

#endif

#ifndef MIX_COLUMNS
#define MIX_COLUMNS

void mix_columns(unsigned char array[4][4]);

#endif

#ifndef ROT_WORD
#define ROT_WORD

void rot_word(unsigned char round_key[4][44], unsigned char rot_word[4]);

#endif

#ifndef ADD_ROUND_KEY
#define ADD_ROUND_KEY

void add_round_key(unsigned char state[4][4], unsigned char r_key[4][4]);

#endif

#ifndef KEY_SCHEDULE
#define KEY_SCHEDULE

void key_schedule(unsigned char cipher_key[4][4],
				  unsigned char round_key[4][44],
				  unsigned char a_rot_word[4]);
				 
#endif

