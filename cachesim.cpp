#include "cachesim.hpp"
#include <iostream>
#include <fstream>
using namespace std;

/**
 * Subroutine for initializing the cache. You many add and initialize any global or heap
 * variables as needed.
 * XXX: You're responsible for completing this routine
 *
 * @c1 The total number of bytes for data storage in L1 is 2^c1
 * @b1 The size of L1's blocks in bytes: 2^b1-byte blocks.
 * @s1 The number of blocks in each set of L1: 2^s1 blocks per set.
 * @v Victim Cache's total number of blocks (blocks are of size 2^b1).
 *    v is in [0, 4].
 * @c2 The total number of bytes for data storage in L2 is 2^c2
 * @b2 The size of L2's blocks in bytes: 2^b2-byte blocks.
 * @s2 The number of blocks in each set of L2: 2^s2 blocks per set.
 * Note: c2 >= c1, b2 >= b1 and s2 >= s1.
 */
 

 //Defining global variables//
 uint64_t address_counter = 0;
 uint64_t accesses; // global counter, keeps track of the number of access. Earier IC.
 char access_sequence [7] = "******";
 uint64_t reads = 0;
 uint64_t writes = 0;
 
 uint64_t L1_access = 0;
 uint64_t L1_read_miss = 0;
 uint64_t L1_write_miss = 0;
 uint64_t L1_write_back = 0;
 
 uint64_t L2_access = 0;
 uint64_t L2_read_miss = 0;
 uint64_t L2_write_miss = 0;
 uint64_t L2_write_back = 0;
 
 uint64_t victim_access = 0;
 uint64_t victim_hit = 0;
  
 uint64_t L1_size[3] = {0,0,0}; //c1,b1,s1
 uint64_t L2_size[3] = {0,0,0};
 uint64_t VC_size[3] = {0,0,0};
 uint64_t V = 0;
 
/* uint64_t L1_size0;
uint64_t L1_size1;
uint64_t L1_size2;

uint64_t L2_size0 ;
uint64_t L2_size1 ;
uint64_t L2_size2 ;

uint64_t VC_size0 ;
uint64_t VC_size1 ;
uint64_t VC_size2 ; */
 
tag_store L1_tag_store;
tag_store L2_tag_store;
tag_store vc_tag_store;
 // End of defining global variables//
 
 
 void setup_cache(uint64_t c1, uint64_t b1, uint64_t s1, uint64_t v, uint64_t c2, uint64_t b2, uint64_t s2) {

uint64_t sv = v;
uint64_t bv = b1;
uint64_t cv = b1*v;
uint64_t i;
uint64_t temp;
uint64_t temp_index;
uint64_t len;
uint64_t n;

L1_size = {c1,b1,s1}; 			 
L2_size = {c2,b2,s2};
if (v != 0)
	VC_size = {cv,bv,sv};

 temp = pow(2,(c1-b1-s1)) * pow(2,(s1));
 L1_tag_store.tag = new uint64_t[temp];
 L1_tag_store.index = new uint64_t[temp];
 L1_tag_store.lru_counter = new uint64_t[temp];
 L1_tag_store.dirty_bit = new bool[temp];
 L1_tag_store.valid_bit = new bool[temp];
 
 temp = pow(2,(c2-b2-s2)) * pow(2,(s2));
 L2_tag_store.tag = new uint64_t[temp];
 L2_tag_store.index = new uint64_t[temp];
 L2_tag_store.lru_counter = new uint64_t[temp];
 L2_tag_store.dirty_bit = new bool[temp];
 L2_tag_store.valid_bit = new bool[temp];
 
 if (v != 0){
	 vc_tag_store.tag = new uint64_t[v];
	 vc_tag_store.index = new uint64_t[v];
	 vc_tag_store.lru_counter = new uint64_t[v];
	 vc_tag_store.dirty_bit = new bool[v];
	 vc_tag_store.valid_bit = new bool[v];	 
	 V = v;
 } 
 //Set all valid bit, lru_counters, and tags to zero.
len = pow(2,(c1-b1-s1));
for (i=0; i<num_of_tag_generate(1);i++){
	n = i/len;
	temp_index = i - (n*len);	
	L1_tag_store.tag[i]  = 0;
	L1_tag_store.index[i]  = temp_index;
	L1_tag_store.lru_counter[i]  = 0;
	L1_tag_store.valid_bit[i]  = false;
	L1_tag_store.dirty_bit[i]  = false;
}
 
//for (i=0; i<2^(b2);i++){
//	 for (j=0; j<2^(c2-s2-b2); j++){
//		 uint64_t temp_ind = j+(i*(2^(c2-b2-s2)));
len = pow(2,(c2-b2-s2));	
for (i=0;i<num_of_tag_generate(2);i++){
	n = i/len;
	temp_index = i - (n*len);
	L2_tag_store.tag[i]  = 0;
	L2_tag_store.index[i]  = temp_index;
	L2_tag_store.lru_counter[i]  = 0;
	L2_tag_store.valid_bit[i]  = false;
	L2_tag_store.dirty_bit[i]  = false;	
}		 
		 
		 
if (v != 0){
	for (i=0; i<num_of_tag_generate(3);i++){
		vc_tag_store.tag[i]  = 0;
		vc_tag_store.index[i]  = 0;
		vc_tag_store.lru_counter[i]  = 0;
		vc_tag_store.valid_bit[i]  = false;
		vc_tag_store.dirty_bit[i]  = false;	
	}
}		 
//place all dimesnsions/parameters of cache in memory
}
/***************************************************************
 * Subroutine that simulates the cache one trace event at a time.
 * XXX: You're responsible for completing this routine
 *
 * @type The type of event, can be READ or WRITE.
 * @arg  The target memory address
 * @p_stats Pointer to the statistics structure
 ***************************************************************/
void cache_access(char type, uint64_t arg, cache_stats_t* p_stats) {
	access_sequence[0] = '*';
	access_sequence[1] = '*';
	access_sequence[2] = '*';
	access_sequence[3] = '*';
	access_sequence[4] = '*';
	access_sequence[5] = '*';
	access_sequence[6] = '\0';
	address_counter++;
	int block_retrived_flag= 0;
	uint64_t arg_tag = tag_generator(arg,1);
	uint64_t arg_index = index_generator(arg,1);
	uint64_t L1_num_of_tag = num_of_tag_generate(1);	
		
	//----------Search Algorithm----------//
	//check if required address(arg) is present in tag_store of L1 cache
	//in: num_of_tag, which cache(L1,L2 or VC), arg_tag, arg_index
	// out: hit_flag
	
	//Update L1 access counter
	accesses++;
	L1_access++;
	
	uint64_t i;
	int hit_flag = 0;
	//loop through all the blocks in one set	
	

	for (i = 0;i < L1_num_of_tag; i++) {	
		if (L1_tag_store.tag[i] == arg_tag) {
			if (L1_tag_store.index[i] == arg_index){
				hit_flag = 1;
				break;	
			}			
		}
	}		
	//--------------------//

	if (hit_flag == 1){ // L1 Hit case			
		access_sequence[0] = 'H';
		access_sequence[1] = '1';
		if (type == 'r'){
			reads++;
			block_retrived_flag = L1_read_hit_fn(i);		
		}
		else if (type == 'w'){
			writes++;
			block_retrived_flag = L1_write_hit_fn(i);		
		}
	}
	if (V == 0){
		access_sequence[4] = ' ';
		access_sequence[5] = ' ';
	}
	

	if (hit_flag == 0) {
		//L1 Miss case		
		access_sequence[0] = 'M';
		access_sequence[1] = '1';
		if (type == 'r'){
			reads++;			
			block_retrived_flag = L1_read_miss_fn(arg);		
		}			
		else {
			writes++;			
			block_retrived_flag = L1_write_miss_fn(arg);
		}
	}	
	
	p_stats->accesses = address_counter;
	p_stats->accesses_l2 = L2_access;
	p_stats->accesses_vc = victim_access ;
	p_stats->reads = reads;
	p_stats->read_misses_l1 = L1_read_miss;
	p_stats->read_misses_l2 = L2_read_miss;
	p_stats->writes = writes;
	p_stats->write_misses_l1 = L1_write_miss;
	p_stats->write_misses_l2 = L2_write_miss;
	p_stats->write_back_l1 = L1_write_back;
	p_stats->write_back_l2 = L2_write_back;
	p_stats->victim_hits = victim_hit;
	p_stats->avg_access_time_l1 = 0;
	
	
	ofstream out("deb.txt", ios::out | ios::app );
	//if ((address_counter == 16979)){			
		//if ((index_generator(arg,2) == 63) && (access_sequence[0] == 'M') && (access_sequence[1] == '1'))
	//cout << address_counter <<'\t' << index_generator(arg,2) << '\t' << access_sequence[0] << access_sequence[1] << '\n'; }
	 //cout << address_counter <<'\t' << tag_generator(arg,1) << '\t' << index_generator(arg,1) << '\t' << tag_generator(arg,2) << '\t' << index_generator(arg,2) << '\n';
	//cout << tag_generator(arg,1) << '\t' << index_generator(arg,1) << '\t' << tag_generator(arg,2) << '\t' << index_generator(arg,2) << '\n';
	
	//myfile.open("astar.trace");
	// Print all cache properties
	cout << address_counter << '\n';
	/* cout << access_sequence << '\n';
	uint64_t j;
	for (j=0;j<num_of_tag_generate(1);j++){
		cout << L1_tag_store.tag[j] << '\t' << L1_tag_store.lru_counter[j] << '\t' << L1_tag_store.dirty_bit[j] << '\n';
	}
	cout << '\n';
	for (j=0;j<num_of_tag_generate(2);j++){
		cout << L2_tag_store.tag[j] << '\t' << L2_tag_store.lru_counter[j] << '\t' << L2_tag_store.dirty_bit[j] <<'\n';
	}
	cout << '\n';
	for (j=0;j<num_of_tag_generate(3);j++){
		cout << vc_tag_store.tag[j] << '\t' << vc_tag_store.lru_counter[j] << '\n';
	}
	cout << '\n';
	cout << L1_write_back << '\n';
	cout << L2_write_back << '\n';
	cout << "---------------" << '\n';
	cout << '\n'; */
	
	     //if (address_counter == 2572) {			
		 //cout << tag_generator(arg,3) << '\n';
		 //or (j=0; j<num_of_tag_generate(2); j++){
			 // if (index_generator(arg,2) == 14) //tag 3621
				 //out << address_counter << '\t' << tag_generator(arg,2) << '\t' << index_generator(arg,2) << '\n' ;
				 //cout << tag_generator(arg,2) << '\t' << index_generator(arg,2) << '\n';
				/* cout << accesses<<'\n';
				
				   for (j=0;j<num_of_tag_generate(2);j++){
					if (L2_tag_store.index[j] == 63) {
						cout << address_counter << '\t' << j << '\t' << L2_tag_store.tag[j] << '\t' << L2_tag_store.index[j] << '\t' << L2_tag_store.lru_counter[j] << '\t' << L2_tag_store.valid_bit[j] << '\t' << L2_tag_store.dirty_bit[j] <<'\n';
					cout << '\n';
 */

				/* for (j=0;j<num_of_tag_generate(1);j++){
					if (L1_tag_store.index[j] == 15) {
						cout << address_counter << '\t' << j << '\t' << L1_tag_store.tag[j] << '\t' << L1_tag_store.index[j] << '\t' << L1_tag_store.lru_counter[j] << '\t' << L1_tag_store.valid_bit[j] << '\n';
					cout << '\n' */;					
					// }					
				// }		 	
			// } 	  
				 //cout << address_counter << '\t' << vc_tag_store.tag[j] << '\t' << vc_tag_store.lru_counter[j] << '\n';				 
		//}
		//cout << '\n' << '\n';	 			 		 
	 //}  
				//cout << address_counter << '\t' << arg_tag << '\t' << arg_index << '\n'; 
		//address_counter = address_counter;
		//myfile << access_sequence << '\n' ;
		//if (address_counter == 2728)
		out << access_sequence << '\n' ;
		//cout << address_counter << '\n';
			//out << access_sequence << '\n' ;
					
			//cout << address_counter << '\t' << tag_generator(arg,1) << '\t' << index_generator(arg,1) << '\n';
		 /* for (j=0; j<L1_num_of_tag ;j++){		
			if ((L1_tag_store.index[j] == arg_index)){
				cout << address_counter << '\t' <<  L1_tag_store.tag[j] << '\t' << L1_tag_store.index[j] << '\t' << L1_tag_store.valid_bit[j] << '\n';}} */
		 
		out.close();		
		
		/* for (j=0;j<num_of_tag_generate(1);j++)
			cout<<L1_tag_store.tag[j]<< '\t';
		cout<<'\n';
		for (j=0;j<num_of_tag_generate(2);j++)
			cout<<L2_tag_store.tag[j]<< '\t';
		cout<<'\n';
		for (j=0;j<num_of_tag_generate(3);j++)
			cout<<vc_tag_store.tag[j]<< '\t';
		cout<<'\n'<< "------" <<'\n'; */
}
/**** End of Cache_access function ****/


/************** Function block for L1 read Hit *******************/
int L1_read_hit_fn(uint64_t i){
	
	L1_tag_store.lru_counter[i] = accesses;	
	return 1;
}
/****** End of block ******/


/************** Function block for L1 write Hit  **************/
int L1_write_hit_fn(uint64_t i){	
	
	L1_tag_store.dirty_bit[i] = true;
	L1_tag_store.lru_counter[i] = accesses;	
	return 1;
}
/*** End of block ***/



/************ L1 Read Miss Function Block *************/

int L1_read_miss_fn(uint64_t arg){
	uint64_t j;
	uint64_t j_low;
	int empty_flag = 0;
	uint64_t empty_index = 0;	
	uint64_t temp_LRU_storage;
	bool temp_LRU_dirty; //[3] = {0,0,0}; // Tag, index, dirty bit
	int block_retrived_flag = 0;	
	int block_present_flag = 0;
	uint64_t L1_num_of_tag = num_of_tag_generate(1);
	uint64_t arg_tag = tag_generator(arg,1);
	uint64_t arg_index = index_generator(arg,1);
//	uint64_t temp_dirty;
	int hit_flag = 0;
	
	L1_read_miss++;		
	//check if there is a empty location in L1. If yes, get its sequence number.
	temp_LRU_dirty = false;
	temp_LRU_storage = 0;
	empty_flag = 0;
	empty_index = 0;
	block_retrived_flag = 0;
	for (j=0; j<L1_num_of_tag ;j++){
		//initially declared empty_flag = 0 => no empty space in L1
		if ((L1_tag_store.index[j] == arg_index)&&(L1_tag_store.valid_bit[j] == false)){
			empty_flag = 1; //Available empty space in L1 at the set with same index as incoming argument
			empty_index = j;			
			break;
		}
	}	
	
	if (empty_flag == 0){	// No empty flag, so move LRU to a variable		
		//search for LRU from L1 block, so that we can remove it to make space in the set with same index as incoming address for the incoming address.
		j_low = lru_block_finder(arg, 1);		
		// store lru block in temp_LRU_storage and temp_LRU_dirty
		temp_LRU_storage = address_generate(L1_tag_store.tag[j_low], L1_tag_store.index[j_low], 1);
		temp_LRU_dirty = L1_tag_store.dirty_bit[j_low];			
			
		L1_tag_store.valid_bit[j_low] = false;
		L1_tag_store.tag[j_low]= 0;
		L1_tag_store.dirty_bit[j_low]= false;
		L1_tag_store.lru_counter[j_low]= 0;				
		//Lru block removed to variable, thus now we have space in L1 to bring requires block up.
	}		 		
	
	
	//Miss Repair  bring the block up to L1
	//Search if the required address arg is present in victim cache.
	if (V != 0){
		victim_access++;
		accesses++;
		block_present_flag = 0;
		for (j=0; j<V; j++){
			if (tag_generator(arg,3) == vc_tag_store.tag[j]){				
				block_present_flag = 1;					
				break;
			}			
		}	
		
		
		/* if (address_counter == 1636){
			for (j=0; j<V; j++){
			if (tag_generator(arg,3) == vc_tag_store.tag[j]){
				block_present_flag = 1;								
				cout << tag_generator(arg,3) << '\t' << vc_tag_store.tag[j] << '\t' << vc_tag_store.valid_bit[j] << '\t' << vc_tag_store.lru_counter[j] << '\n';
				}			
			}
		} */		
		if (block_present_flag == 1){
			access_sequence[2] = 'H';
			access_sequence[3] = 'V';
			block_retrived_flag = vc_read_hit_fn(arg);
		}					
		else if (block_present_flag == 0){
			access_sequence[2] = 'M';
			access_sequence[3] = 'V';
			block_retrived_flag = vc_read_miss_fn(arg,1);				
		}				
	}
	
	// Special Case if Victim Cache is not present, go to L2 cache and search for it directly. 
	else if (V == 0){
		access_sequence[4] = ' ';
		access_sequence[5] = ' ';
		L2_access++;
		accesses++;
		hit_flag = 0;
		for (j=0;j<(num_of_tag_generate(2));j++) {
			if (L2_tag_store.tag[j] == tag_generator(arg,2)) { 
				if ((L2_tag_store.index[j] == index_generator(arg,2)) && (L2_tag_store.valid_bit[j] == true)){
					hit_flag = 1;
					break;
				}
			}
		}
		
		if (hit_flag == 1) { // L2 read Hit case	
			access_sequence[2] = 'H';
			access_sequence[3] = '2';
			block_retrived_flag = L2_read_hit_fn(arg);			
		}
		else if (hit_flag == 0) {
			access_sequence[2] = 'M';
			access_sequence[3] = '2';
			block_retrived_flag = L2_read_miss_fn(arg);	
			L2_read_miss++;
		}						
	}					
	if (block_retrived_flag == 1){
				
		//if necessary block is retrived, search for an empty space in L1 cache and place it there. This takes care of both available and empty spaces.
		for (j=0; j<L1_num_of_tag; j++){						
			if ((L1_tag_store.valid_bit[j] == false) && (L1_tag_store.index[j] == arg_index)){				
				L1_tag_store.tag[j] = arg_tag;
				L1_tag_store.index[j] = arg_index;
				L1_tag_store.valid_bit[j] = true;
				L1_tag_store.dirty_bit[j] = false;
				L1_tag_store.lru_counter[j] = accesses;	
				break;				
			}
		}
	}	
	
	////move variable from temp_LRU_storage[tag,index,dirty bit] to VC. If no Victim cache, move to L2
	int storage_cleared_flag = 0; //0 = LRU storage has a value
	if ((empty_flag == 0)&&(V != 0)){
		storage_cleared_flag = vc_write_fn(temp_LRU_storage, temp_LRU_dirty); 		
	}		
	//If Victim cache SIZE is zero(does not exist), check if the Block evicted from L1 is present in L2, and make it a L2 write hit/miss. 
	else if ((empty_flag == 0)&&(V == 0)){ 
		if (temp_LRU_dirty == true){
			L2_access++;
			accesses++;
		
			hit_flag = 0; 		
			for (j=0;j<(num_of_tag_generate(2));j++) {
				if (L2_tag_store.tag[j] == tag_generator(temp_LRU_storage,2)) { 
					if ((L2_tag_store.index[j] == index_generator(temp_LRU_storage,2)) && (L2_tag_store.valid_bit[j] == true)){
						hit_flag = 1;
						break;
					}
				}
			}					
			if (hit_flag == 1) { // L2 read Hit case			
				 storage_cleared_flag = L2_write_hit_fn(temp_LRU_storage, temp_LRU_dirty);			 
			}
			else if (hit_flag == 0) {			
				storage_cleared_flag = L2_write_miss_fn(temp_LRU_storage, temp_LRU_dirty);			
				L2_write_miss++;
			}		
			if(temp_LRU_dirty == true){
				L1_write_back++;
				//cout<< tag_generator(temp_LRU_storage,1) << '\n';
			}	
		}
	}		
		
	temp_LRU_dirty = false;
	temp_LRU_storage = 0;
	
	return block_retrived_flag;	
}
/************* END of L1_read_miss_fn ******************/



/************ L1 Write Miss Function Block *************/
int L1_write_miss_fn(uint64_t arg){	
	uint64_t j;
	uint64_t j_low;
	int empty_flag = 0;
	uint64_t empty_index = 0;	
	uint64_t temp_LRU_storage; //[3] = {0,0,0}; // Tag, index, dirty bit
	bool temp_LRU_dirty;
	int block_retrived_flag = 0;	
	int block_present_flag = 0;
	uint64_t L1_num_of_tag = num_of_tag_generate(1);
	uint64_t arg_tag = tag_generator(arg,1);
	uint64_t arg_index = index_generator(arg,1);	
	int hit_flag;	
	
	//check if there is a empty location in L1. If yes, get its sequence number.
	L1_write_miss++;
	temp_LRU_dirty = false;
	temp_LRU_storage = 0;
	
	empty_flag = 0;
	empty_index = 0;
	block_retrived_flag = 0;
	for (j=0; j<L1_num_of_tag ;j++){
		//initially declared empty_flag = 0 => no empty space in L1
		if ((L1_tag_store.index[j] == arg_index)&&(L1_tag_store.valid_bit[j] == false)){
			empty_flag = 1; //Available empty space in L1 at the set with same index as incoming argument
			empty_index = j;			
			break;
		}
	}	
	
	if (empty_flag == 0){	// No empty flag, so move LRU to a variable		
		//search for LRU from L1 block, so that we can remove it to make space in the set with same index as incoming address for the incoming address.
		j_low = lru_block_finder(arg, 1);
		temp_LRU_storage = address_generate(L1_tag_store.tag[j_low], L1_tag_store.index[j_low], 1);
		temp_LRU_dirty = L1_tag_store.dirty_bit[j_low];
		//Lru block removed to variable, thus now we have space in L1 to bring requires block up.
		L1_tag_store.tag[j_low] = 0;
		L1_tag_store.valid_bit[j_low] = false;
		L1_tag_store.lru_counter[j_low] = 0;
		L1_tag_store.dirty_bit[j_low] = false;		
	}			
	//Search if the required address arg is present in victim cache.
	if (V != 0){
		victim_access++;
		accesses++;
		block_present_flag = 0;
		for (j=0; j<V; j++){
			if (tag_generator(arg,3) == vc_tag_store.tag[j]){
				block_present_flag = 1;				
				break;
			}			
		}			
		if (block_present_flag == 1){
			access_sequence[2] = 'H';
			access_sequence[3] = 'V';
			block_retrived_flag = vc_read_hit_fn(arg);
		}
		else if (block_present_flag == 0){
			access_sequence[2] = 'M';
			access_sequence[3] = 'V';
			block_retrived_flag = vc_read_miss_fn(arg, 1);				
		}
	}	
	// Special Case if Victim Cache is not present, go to L2 cache and search for it directly. 
	// search for block in L2
	// if present call L2 read hit
	// if not present call L2 read miss function
	//return block retrived flag at the end
	else if (V == 0){ 
		L2_access++;
		accesses++;
		hit_flag = 0;
		for (j=0;j<(num_of_tag_generate(2));j++) {
			if (L2_tag_store.tag[j] == tag_generator(arg,2)) { 
				if ((L2_tag_store.index[j] == index_generator(arg,2)) && (L2_tag_store.valid_bit[j] == true)){
					hit_flag = 1;
					break;
				}
			}
		}				
		access_sequence[4] = ' ';
		access_sequence[5] = ' ';
		if (hit_flag == 1) { // L2 read Hit case	
			access_sequence[2] = 'H';
			access_sequence[3] = '2';
			block_retrived_flag = L2_read_hit_fn(arg);
		}
		else if (hit_flag == 0) {	
			access_sequence[2] = 'M';
			access_sequence[3] = '2';
			block_retrived_flag = L2_read_miss_fn(arg);	
			L2_read_miss++;
		}						
	}	
	
	if (block_retrived_flag == 1){
		
		//if necessary block is retrived, search for an empty space in L1 cache and place it there. This takes care of both available and empty spaces.
		for (j=0; j<L1_num_of_tag; j++){
			if ((L1_tag_store.valid_bit[j] == false) && (L1_tag_store.index[j] == arg_index)){
				L1_tag_store.tag[j] = arg_tag;
				L1_tag_store.index[j] = arg_index;
				L1_tag_store.valid_bit[j] = true;
				L1_tag_store.dirty_bit[j] = true;
				L1_tag_store.lru_counter[j] = accesses;		
				break;
			}
		}
	}
	// Now we have the required block in L1, as requested. Now we must check if any block from L1 was evicted in the process. If yes, then place that in Victim cache (or L2 Cache, if V = 0. For L2 cache, if it is present, it will be L2 write hit, else L2 write miss. )		
	//move variable from temp_LRU_storage[tag,index,dirty bit] to VC. If no Victim cache, move to L2
	int storage_cleared_flag = 0;	
	if ((empty_flag == 0)&&(V != 0)){	
		storage_cleared_flag = vc_write_fn(temp_LRU_storage,temp_LRU_dirty); 
	}	
	//If Victim cache is zero(does not exist), check if the Block evicted from L1 is present in L2, and make it a L2 write hit/miss. If hit, update Dirty Bit. If miss, get from Main memory, and set Dirty bit to 1.
	else if ((empty_flag == 0)&&(V == 0)){ 
		if (temp_LRU_dirty == true){
			L2_access++;
			accesses++;
		
			hit_flag = 0;
			for (j=0;j<(num_of_tag_generate(2));j++) {
				if (L2_tag_store.tag[j] == tag_generator(temp_LRU_storage,2)) { 
					if ((L2_tag_store.index[j] == index_generator(temp_LRU_storage,2)) && (L2_tag_store.valid_bit[j] == true)){
						hit_flag = 1;
						break;
					}
				}
			}		
						
			if (hit_flag == 1) { // L2 read Hit case
				 storage_cleared_flag = L2_write_hit_fn(arg,temp_LRU_dirty);
			}
			else if (hit_flag == 0) {
				 storage_cleared_flag = L2_write_miss_fn(arg,temp_LRU_dirty);	
				 L2_write_miss++;
			}	
						

			if(temp_LRU_dirty == 1){
				L1_write_back++;
				//cout<< tag_generator(temp_LRU_storage,1) << '\n';
			}
		}
	}
		
	temp_LRU_dirty = false;
	temp_LRU_storage = 0;
	return block_retrived_flag;
	
}
/************* End of L1_write_miss_fn ******************/


/************* Function for VC Read Miss ******************/
int vc_read_miss_fn(uint64_t arg, int first_access){
	uint64_t j;
	int hit_flag;
	int block_retrived_flag;

	// Since block is not in VC, search for it in L2 Cache.
	L2_access++;
	accesses++;
	hit_flag = 0;		
	for (j=0;j<(num_of_tag_generate(2));j++) {
		if (L2_tag_store.tag[j] == tag_generator(arg,2)) { 
			if ((L2_tag_store.index[j] == index_generator(arg,2)) && (L2_tag_store.valid_bit[j] == true)){
				hit_flag = 1;
				break;
			}
		}
	}	
	if (hit_flag == 1) { // L2 read Hit case			
		block_retrived_flag = L2_read_hit_fn(arg);
		if (first_access == 1){
			access_sequence[4] = 'H';
			access_sequence[5] = '2';
			first_access = 0;			
		}
	}
	else if (hit_flag == 0) {		
		block_retrived_flag = L2_read_miss_fn(arg);			
		if (first_access == 1){
			access_sequence[4] = 'M';
			access_sequence[5] = '2';
			first_access = 0;
			L2_read_miss++;			
		}
	}	
	return block_retrived_flag;
}	
/************* End of vc_read_miss_fn ******************/



/************* Function for VC Read Hit ******************/
int vc_read_hit_fn(uint64_t arg){
	victim_hit++;	
	int block_removed_flag;
	uint64_t j;
	uint64_t vc_num_of_tag = num_of_tag_generate(3);
	uint64_t arg_tag = tag_generator(arg,3);
	
	for (j=0;j<vc_num_of_tag;j++){
		if ((vc_tag_store.tag[j] == arg_tag) && (vc_tag_store.valid_bit[j] == true)){
			vc_tag_store.tag[j] = 0;
			vc_tag_store.valid_bit[j] = false;
			vc_tag_store.lru_counter[j] = 0;	
			block_removed_flag = 1;	
			break;
		}
	}		
		
	return block_removed_flag;
}
/************* End of vc_read_hit_fn ******************/



/************* Function for VC Write ******************/
int vc_write_fn(uint64_t arg, bool dirty_bit ){
	//LRU_storage = {tag,index,dirty_bit}
	uint64_t j;	
	uint64_t vc_num_of_tag = num_of_tag_generate(3);
	int empty_flag;
	uint64_t empty_index;	
	int write_done = 0;
	int hit_flag = 0;
	//uint64_t arg_tag = tag_generator(arg,3);
	int ready_for_vc_write;
	
	//we much check if it's Dirty bit is 1 or 0. If zero, can add to VC. If 1, we have to perform writeback to L2, and then update the clean block in VC.

	ready_for_vc_write = 0;	
	
			
	if (dirty_bit == false){
		// We must check if there is space is VC for this. If there is, put there. Else remove FIFO.
		for (j=0;j<vc_num_of_tag;j++){
			if(vc_tag_store.valid_bit[j] == false){
				empty_flag = 1; //empty block is available in VC. thus no need to remove FIFO block.
				empty_index = j;			
				break;			
			}		
		}		
		
		if (empty_flag == 0){
			remove_FIFO_block();
		}	// As there was no empty space in VC, we have removed the FIFO block to make space for incoming block.
		
		ready_for_vc_write = 1;	
	}
	
	else if (dirty_bit == true){// dirty bit of incoming address is 1. So we must write it back in L2. So let us search if the block is present in L2 or not. Accordingly we shall call it L2 write miss or write Hit.
		//L1_write_back++; //Updated in L2 write hits and misses blocks.
		L2_access++;
		accesses++;
		L1_write_back++;
		hit_flag = 0;
		for (j=0;j<num_of_tag_generate(2);j++){
			if ((L2_tag_store.tag[j] == tag_generator(arg,2)) && (L2_tag_store.index[j] == index_generator(arg,2)) && (L2_tag_store.valid_bit[j] == true)){
				hit_flag = 1;
				break;
			}
		}			
		if (hit_flag == 1)
			ready_for_vc_write = L2_write_hit_fn(arg, true);				
		else if (hit_flag == 0){
			ready_for_vc_write = L2_write_miss_fn(arg,true);
			L2_write_miss++;
		}
		
		
		//Now that we have updated the dirty block in L2, we must place it in VC. Thus, check if tehre is space in VC. If not remove FIFO block from VC and replace with this block, as it is now clean.
		
		empty_flag = 0;
		empty_index = 0;
		for (j=0;j<vc_num_of_tag;j++){
			if(vc_tag_store.valid_bit[j] == false){
				empty_flag = 1; //empty block is available in VC. thus no need to remove FIFO block.
				empty_index = j;			
				break;			
			}		
		}		
		
		if (empty_flag == 0){
			remove_FIFO_block();
		}	// As there was no empty space in VC, we have removed the FIFO block to make space for incoming block.
		
		ready_for_vc_write = 1;
	}	
	if (ready_for_vc_write == 1){
		
		for (j=0; j<vc_num_of_tag;j++){
			if(vc_tag_store.valid_bit[j] == false){
				vc_tag_store.tag[j] = tag_generator(arg,3);
				vc_tag_store.valid_bit[j] = true;
				vc_tag_store.lru_counter[j]= accesses;
				vc_tag_store.dirty_bit[j] = 0;	
				write_done = 1;
				break;
			}
		}			
	}	
	return write_done;	
}
/************* End of vc_write ******************/

/************* Function for L2 Read Hit ******************/
int L2_read_hit_fn(uint64_t arg){
	
	uint64_t j;
	uint64_t arg_tag = tag_generator(arg,2);
	uint64_t arg_index = index_generator(arg,2);
	
	//L2_tag_store.lru_counter[j] = accesses;
	for(j= 0; j<num_of_tag_generate(2);j++){
		if ((L2_tag_store.tag[j] == arg_tag) && (L2_tag_store.index[j] == arg_index) && (L2_tag_store.valid_bit[j] == true)) {
			L2_tag_store.lru_counter[j] = accesses;
			break;
		}		
	}
	return 1;
}
/************* End of L2_read_hit_fn ******************/


/************* Function for L2 Write Hit ******************/
int L2_write_hit_fn(uint64_t arg, bool dirty_bit){
	
	uint64_t j;
	uint64_t arg_tag = tag_generator(arg,2);
	uint64_t arg_index = index_generator(arg,2);
	
	if (V != 0){ //In this case the write is coming from L1, because VC cannot hold dirty bits. Thus make DB = 1
		for(j= 0; j<num_of_tag_generate(2);j++){
			if ((L2_tag_store.tag[j] == arg_tag) && (L2_tag_store.index[j] == arg_index) && (L2_tag_store.valid_bit[j] == true)){
				L2_tag_store.lru_counter[j] = accesses;
				L2_tag_store.dirty_bit[j] = true;
				break;
			}		
		}
	}		
	
	if (V == 0){//Inthis case as there is no VC, we cannot tell if the block in L1 is dirty or not. Update L2 DB as per L1 DB.		
		if (dirty_bit == true){
			for(j= 0; j<num_of_tag_generate(2);j++){
				if ((L2_tag_store.tag[j] == arg_tag) && (L2_tag_store.index[j] == arg_index) && (L2_tag_store.valid_bit[j] == true)){
					L2_tag_store.lru_counter[j] = accesses;
					L2_tag_store.dirty_bit[j] = true;
					break;
				}
			}
		}
		else if (dirty_bit == false){
			for(j= 0; j<num_of_tag_generate(2);j++){
				if ((L2_tag_store.tag[j] == arg_tag) && (L2_tag_store.index[j] == arg_index) && (L2_tag_store.valid_bit[j] == true)){
					L2_tag_store.lru_counter[j] = accesses;
					L2_tag_store.dirty_bit[j] = false;
					break;
				}
			}
		}
	} 	
	return 1;
}
/************* End of L2_write_hit_fn ******************/


/************* Function for L2 Read miss ******************/
int L2_read_miss_fn(uint64_t arg){
	uint64_t j;
	uint64_t j_low;
	uint64_t L2_num_of_tag = num_of_tag_generate(2);
	uint64_t arg_tag = tag_generator(arg,2);
	uint64_t arg_index = index_generator(arg,2);
	int empty_flag = 0;
	uint64_t temp_LRU_storage;//[3] = {0,0,0}; //tag,index,dirty_bit
	bool temp_LRU_dirty;
		

	temp_LRU_dirty = false;
	//L2_read_miss++;
	empty_flag = 0;
	for (j=0;j<L2_num_of_tag;j++){		
		if ((L2_tag_store.index[j]==arg_index)&&(L2_tag_store.valid_bit[j] == false)){
			empty_flag = 1;			
			break;
		}		
	}
	
	if (empty_flag == 0){
		j_low = lru_block_finder(arg,2);
		// store the LRU block to variables temp_LRU_storage and temp_lru_dirty
		temp_LRU_storage = address_generate(L2_tag_store.tag[j_low], L2_tag_store.index[j_low], 2);
		temp_LRU_dirty = L2_tag_store.dirty_bit[j_low];
		//clear the block which we just copied to variable
		L2_tag_store.tag[j_low] = 0;		
		L2_tag_store.valid_bit[j_low] = false;
		L2_tag_store.dirty_bit[j_low] = false;
		L2_tag_store.lru_counter[j_low] = 0;		
	}
	
	//Since we have space in L2 now, we can bring the address up from memory. It's dirty bit will be 0.
	for (j=0;j<L2_num_of_tag;j++){		
		if((L2_tag_store.index[j]==arg_index)&&(L2_tag_store.valid_bit[j] == false)){
			L2_tag_store.tag[j] = arg_tag;
			L2_tag_store.index[j] = arg_index;
			L2_tag_store.valid_bit[j] = true;
			L2_tag_store.dirty_bit[j] = false;
			L2_tag_store.lru_counter[j] = accesses;					
			break;
		}		
	}
	
	//Now we have our required block in L2. Next we must check the block held in variable. If its dirty bit is one, Write it back to memory. And clear the storage
	if (temp_LRU_dirty == true){
		L2_write_back++;
	}	
	temp_LRU_storage = 0;
	temp_LRU_dirty = false;	
	
	return 1;	
	
}
/************* End of L2_read_miss_fn ******************/


/************* Function for L2 write miss ******************/
int L2_write_miss_fn(uint64_t arg, bool dirty_bit){
	uint64_t j;
	uint64_t L2_num_of_tag = num_of_tag_generate(2);
	int empty_flag;
	uint64_t empty_index;
	uint64_t j_low;
	uint64_t L2_temp_LRU_storage; //[3]; //tag , index, dirty bit
	bool L2_temp_dirty;
	
	
	uint64_t arg_tag = tag_generator(arg,2) ;
	uint64_t arg_index = index_generator(arg,2);
	
	L2_temp_dirty = false;
	empty_flag = 0;
	//L2_write_miss++;
	
	for(j=0;j<L2_num_of_tag;j++){
		if ((L2_tag_store.index[j]==arg_index)&&(L2_tag_store.valid_bit[j] == false)){
			empty_flag = 1;
			empty_index = j;
			break;
		}
	}	
	
	if(empty_flag == 0){
		//Find LRU block and copy its contents onto a variable
		j_low = lru_block_finder(arg,2);
		L2_temp_LRU_storage = address_generate(L2_tag_store.tag[j_low],L2_tag_store.index[j_low],2);
		L2_temp_dirty = L2_tag_store.dirty_bit[j_low];
		//Clear contents of LRU Location in L2 after copying it to variable
		L2_tag_store.tag[j_low] = 0;	
		L2_tag_store.dirty_bit[j_low] = false;
		L2_tag_store.valid_bit[j_low] = false;
		L2_tag_store.lru_counter[j_low] = 0;	
	}
	
	// write the block to L2 from memory.
	for(j=0;j<L2_num_of_tag;j++){		
		if ((L2_tag_store.index[j]==arg_index)&&(L2_tag_store.valid_bit[j] == false)){			
			L2_tag_store.tag[j] = arg_tag;
			L2_tag_store.index[j] = arg_index;
			L2_tag_store.valid_bit[j] = true;
			L2_tag_store.dirty_bit[j] = true;
			L2_tag_store.lru_counter[j] = accesses;			
			
			if (V == 0)	{
				// no victim cache hence block coming from L1 cache		
				L2_tag_store.valid_bit[j] = dirty_bit;	
			}
			else // VC present. block has come for write back as it could not be placed in vc. this dirty bit = 1
				L2_tag_store.valid_bit[j] = true;
			
			break;
		}
	}	
	
	//empty_index is the sequence number of tag which is empty. write the required data in LRU_storage to this address 

	//if L2_LRU_storage has dirty bit of 1, it must be updated in memory. this write back from l2.
	if (L2_temp_dirty == true)
		L2_write_back++;
	
	//clear L2 lru storage  variable 
	L2_temp_LRU_storage = 0;
	L2_temp_dirty = false;
	
	return 1;
}
/************* End of L2_write_miss_fn ******************/


/******* Function for FIFO ******/
// This function implements a FIFO where it removes from VC the block with lowest value in LRU counter field of the block. 
void remove_FIFO_block(){
	uint64_t j;
	uint64_t j_low = 0;
	uint64_t vc_num_of_tag = num_of_tag_generate(3);
	uint64_t temp = accesses;	
	for (j=0; j<vc_num_of_tag; j++){
		if((vc_tag_store.lru_counter[j] < temp) && (vc_tag_store.valid_bit[j]==true)){
			temp = vc_tag_store.lru_counter[j];
			j_low = j;
		}
	}
	//Removing the fifo of vc by clearing tag, valid bit and lru counter bits
	vc_tag_store.tag[j_low] = 0;
	vc_tag_store.valid_bit[j_low] = false;
	vc_tag_store.lru_counter[j_low] = 0;	
}


/*********** End of FIFO Function block ********/


/** Function to generate index bits for a given address for a given memory L1 or L2**/
uint64_t index_generator(uint64_t arg, int mem){
	uint64_t arg_tag ;
	uint64_t temp ;
	uint64_t arg_index ;
	if (mem == 1){
		arg_tag = arg >> (L1_size[0] - L1_size[2]); //tag = c-s
		temp  = arg_tag << (L1_size[0] - L1_size[2]); // tag bits followed by zeros in place of index and offset
		temp = arg - temp; // index and offset in its place
		arg_index = temp >> (L1_size[1]); // Right shift by number of offset bits to obtain tag
	}
	else if (mem == 2){
		arg_tag = arg >> (L2_size[0] - L2_size[2]); //tag = c-s
		temp  = arg_tag << (L2_size[0] - L2_size[2]); // tag bits followed by zeros in place of index and offset
		temp = arg - temp; // index and offset in its place
		arg_index = temp >> (L2_size[1]); // Right shift by number of offset bits to obtain tag
	}
	/* else if ((mem == 3) && (V != 0)){
		arg_tag = arg >> vc_size[0] - vc_size[2]; //tag = c-s
		temp  = arg_tag << vc_size[0] - vc_size[2]; // tag bits followed by zeros in place of index and offset
		temp = arg - temp; // index and offset in its place
		arg_index = temp >> vc_size[1]; // Right shift by number of offset bits to obtain tag
	} */
	return arg_index;

}

/** Function that returns the tags bits for a given address for a given memory type**/
uint64_t tag_generator(uint64_t arg, int mem){
	uint64_t arg_tag; 
	if (mem == 1){
		arg_tag = arg >> (L1_size[0] - L1_size[2]); //tag = c-s
	}
	else if (mem == 2){
		arg_tag = arg >> (L2_size[0] - L2_size[2]); //tag = c-s
	}
	else if ((mem == 3) && (V != 0)){
		arg_tag = arg >> (VC_size[1]);// - VC_size[2]); //tag = c-s
	}
	return arg_tag;

}
/** End of Tag generator function **/

/** Function that returns the sequence of LRU block in a given memory(L1,L2) for a given adderss(we will find LRU per set. THus only index is used from the address) **/
uint64_t lru_block_finder(uint64_t arg, int mem){		
	uint64_t j;
	uint64_t temp = accesses;
	uint64_t j_low = 0;
	uint64_t arg_index = index_generator(arg,mem);
	temp = accesses;
	if (mem == 1){
		for (j=0; j<num_of_tag_generate(mem); j++){
			if((arg_index == L1_tag_store.index[j]) && (L1_tag_store.lru_counter[j] < temp) && (L1_tag_store.valid_bit[j] == true)){
				temp = L1_tag_store.lru_counter[j];
				j_low = j;				
			}
		}
	}
	else if (mem == 2){
		for (j=0; j<num_of_tag_generate(mem); j++){
			if((arg_index == L2_tag_store.index[j]) && (L2_tag_store.lru_counter[j] < temp) && (L2_tag_store.valid_bit[j] == true)){
				temp = L2_tag_store.lru_counter[j];
				j_low = j;				
			}
		}
	}	
	return j_low;
}
// End of LRU_block_finder function


/** Function that return the number of tags in a cache. mem = 0 => L1, mem =1 for L2, mem = 2 for VC **/
uint64_t num_of_tag_generate(int mem){	
	uint64_t num_of_tag; 
	if (mem == 1){
		num_of_tag = pow(2,(L1_size[0] - L1_size[1])); //2^(c-b)
	}
	else if (mem == 2){
		num_of_tag = pow(2,(L2_size[0] - L2_size[1]));
	}
	else if ((mem == 3) && (V != 0)){
		num_of_tag = V;
	}
	
	return num_of_tag;
}
// End of number of tag generator function


/** Function generate address from tag,index values **/
uint64_t address_generate(uint64_t tag, uint64_t index, int mem){
	
	//Mem = memory from which the tag,index are taken.
	uint64_t concat_addr;
	uint64_t concat_tag_index;
	uint64_t num_index_bits;
	uint64_t num_offset_bits;
	if (mem == 1){
		num_offset_bits = L1_size[1];//b1;
		num_index_bits = L1_size[0]-L1_size[2]- L1_size[1];//(c1-s1-b1);
	}
	else if (mem == 2){
		num_index_bits = L2_size[0]-L2_size[2]- L2_size[1];//(c2-s2-b2);
		num_offset_bits = L2_size[1];//b2;
	}		
	else if (mem == 3){
		num_index_bits = 0;	
		num_offset_bits = VC_size[1];
	}		
	concat_tag_index = tag;//load tag onto concat_tag_index	
	concat_tag_index = concat_tag_index << num_index_bits; //Left shift by number of index bits
	concat_tag_index = concat_tag_index | index; //Add index bits to generate.	
	concat_addr = concat_tag_index << num_offset_bits; // This is one of the addresses present in the block evicted from L1. We are supposed to search for the block with this address(or rather tag) in L2.
	return concat_addr;
}
 
/** End of function address_generate **/

/**
 * Subroutine for cleaning up any outstanding memory operations and calculating overall statistics
 * such as miss rate or average access time.
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void complete_cache(cache_stats_t *p_stats) {

double AAT;
double AAT2;
double HT1;
double MR1;
double MP1;
double HT2;
double MR2;
double MP2 = 0.0000005;//*(pow(10,-6)); Devide by 1e-6 later
double MRVC;
double L1_misses = (p_stats->read_misses_l1)+(p_stats->write_misses_l1);
double L2_misses = (p_stats->read_misses_l2)+(p_stats->write_misses_l2);
double victim_misses = (p_stats->accesses_vc) - (p_stats->victim_hits);

HT2 = 4 + ((0.4)*(L2_size[2]));
cout << HT2 << '\n';
MR2 = (L2_misses)/(p_stats->accesses_l2);
cout << MP2 << '\n';
cout << MR2 << '\n';
cout << p_stats->accesses_l2 << '\n';
AAT2 = HT2+(MR2*(MP2));
cout << AAT2 << '\n';

MRVC = victim_misses/L1_misses;
cout << MRVC << '\n';
MR1 = L1_misses/ (p_stats->accesses);
cout << MR1 << '\n';
cout << p_stats->accesses << '\n';
if (V == 0)
	MP1 = (AAT2);
else
	MP1 = (MRVC)*(AAT2);
HT1 = 2 + ((0.2)*L1_size[2]);
cout << MP1 << '\n';
AAT = HT1+(MR1*MP1);
p_stats->avg_access_time_l1 = AAT;
cout << p_stats->avg_access_time_l1 << '\n';

}

