#ifndef CACHESIM_HPP
#define CACHESIM_HPP

#ifdef CCOMPILER
#include <stdint.h>
#else
#include <cstdint>
#include <math.h>
#endif

struct cache_stats_t {
    uint64_t accesses;
    uint64_t accesses_l2;
    uint64_t accesses_vc;
    uint64_t reads;
    uint64_t read_misses_l1;
    uint64_t read_misses_l2;
    uint64_t writes;
    uint64_t write_misses_l1;
    uint64_t write_misses_l2;
    uint64_t write_back_l1;
    uint64_t write_back_l2;
    uint64_t victim_hits;
    double   avg_access_time_l1;
};

struct tag_store {	
  uint64_t *tag;
  uint64_t *index;
  uint64_t *lru_counter;
  bool *dirty_bit;
  bool *valid_bit;
};

extern tag_store L1_tag_store;
extern tag_store L2_tag_store;
extern tag_store vc_tag_store;

/*** Global Variables ***/

 extern uint64_t IC; // global counter, keeps track of the number of instructions
 
 extern uint64_t reads;
 extern uint64_t writes;
 
 extern uint64_t L1_access;
 extern uint64_t L1_read_miss ;
 extern uint64_t L1_write_miss ;
 extern uint64_t L1_write_back ;
 
 extern uint64_t L2_access ;
 extern uint64_t L2_read_miss ;
 extern uint64_t L2_write_miss ;
 extern uint64_t L2_write_back ;
 
 extern uint64_t victim_hit ;
 extern uint64_t victim_access ;
 
  extern uint64_t L1_size[]; //c1,b1,s1
  extern uint64_t L2_size[];
  extern uint64_t VC_size[];
 extern uint64_t V ;
 
 /*** End of Global Variales ***/

void setup_cache(uint64_t c1, uint64_t b1, uint64_t s1, uint64_t v,
                 uint64_t c2, uint64_t b2, uint64_t s2);
void cache_access(char type, uint64_t arg, cache_stats_t* p_stats);
void complete_cache(cache_stats_t *p_stats);

int L1_read_hit_fn(uint64_t i);
int L1_write_hit_fn(uint64_t i);
int L1_read_miss_fn(uint64_t arg );
int L1_write_miss_fn(uint64_t arg);
int vc_read_miss_fn(uint64_t arg, int first_access);
int vc_read_hit_fn(uint64_t arg);
int vc_write_fn(uint64_t arg, bool dirty_bit);
int L2_read_hit_fn(uint64_t arg);
int L2_read_miss_fn(uint64_t arg);
int L2_write_hit_fn(uint64_t arg, bool dirty_bit);
int L2_write_miss_fn(uint64_t arg, bool dirty_bit);


void remove_FIFO_block();
uint64_t index_generator(uint64_t arg, int mem);
uint64_t tag_generator(uint64_t arg, int mem);
uint64_t lru_block_finder(uint64_t arg, int mem);
uint64_t num_of_tag_generate(int mem);
uint64_t address_generate(uint64_t tag, uint64_t index, int mem);

static const uint64_t DEFAULT_C1 = 12;   /* 4KB Cache 3;//*/
static const uint64_t DEFAULT_B1 = 5;    /* 32-byte blocks 1;//*/
static const uint64_t DEFAULT_S1 = 3;   /* 8 blocks per set 1;//*/
static const uint64_t DEFAULT_C2 = 15;  /* 32KB Cache 3;// */
static const uint64_t DEFAULT_B2 = 5;    /* 32-byte blocks 1;//*/
static const uint64_t DEFAULT_S2 = 4;   /* 16 blocks per set 2;//*/
static const uint64_t DEFAULT_V =  0;   /* 3 blocks in VC 2;//*/

/** Argument to cache_access rw. Indicates a load */
static const char     READ = 'r';
/** Argument to cache_access rw. Indicates a store */
static const char     WRITE = 'w';

#endif /* CACHESIM_HPP */
