# Cache-Design

In this project, we have simulated the function of a Cache. The project is implemented in C++. The simulator has  the following capabilities:

1. The simulator models a 2-level caching system with L1(Level 1 cache) and L2 (Level 2 cache). These two levels of cache have 2^(C1) and 2^(C2) bytes of data storage, having 2^(B1)-byte and 2^(B2)-byte blocks, and with sets of 2^(S1) blocks per set and 2^(S2) blocks per set, respectively (note that S=0 is a direct-mapped cache, and S = C - B is a fully associative cache). Also note that the values are restricted to C2>=C1, B2>=B1 and S2>=S1.  
2. Both L1 and L2 caches use the LRU replacement policy.
3. The simulator models a victim cache (“VC”) that holds V total blocks of size 2^(B1). V is in the range [0, 4]. The VC uses a FIFO replacement policy.
4. If there is a miss to L1 cache and a hit in VC (say for block X), then block X is placed in L1. Here, the block that X replaces (say block Y) is placed in the VC.
5. However, if there is a miss in both L1 and VC, then, the miss repair block (say block A) is fetched from L2 into L1. Block A then replaces block B in L1, which then replaces block C in the VC. Here, block C was the oldest (FIFO) block in the VC. If B is dirty, it must be written back to L2 and be marked as dirty. If B does not exist in L2 anymore, B must be allocated in L2 and marked as dirty.
6. The memory addresses are 64-bit addresses.
7. Caches are byte-addressable.
8. The L1 cache implements a write-back, write-allocate (WBWA) policy. There is an additional dirty bit for each tag in the tag store of the L1 cache.
9. If a victim cache is present (i.e., V > 0), when the L1 cache misses on a dirty block, it is written back to the L2 cache before it enters the victim cache: this means it is impossible to have dirty blocks in the victim cache. Thus the VC does not have an additional dirty bit per tag.
10. The L2 cache also implements WBWA. There is an additional dirty bit for each tag in the tag store of the L2 cache. L2 is non-inclusive.
11.  There is an additional valid bit per block of storage overhead required in both L1 and L2. The valid bits are set to 0 when the simulation begins. 
12. If a dirty block hits in L2, it is be clean when bringing it to L1.

## Breakdown on fucntions in code:

1. setup_cache: Subroutine for initializing the cache. 
2. cache_access: Subroutine that simulates the cache one trace event at a time. Type can be either READ or WRITE, which is each defined in cachesim.hpp. A READ event is a memory load operation of 1 byte to the address specified in arg. A WRITE event is a memory store operation of 1 byte to the address specified in arg.
3. complete_cache: Subroutine for cleaning up memory and calculating overall system statistics such as miss rate or average access time.

## Statistics and Output of Simulator:

For each entry in the trace file, print “Hx” or “Mx” for a hit or miss in L1 (“1”), the VC (“v”) or the L2 (“2”). Print “*” if the cache is not accessed. For example, the output would be “M1MvM2,” for a miss in all caches. 

For each entry in the trace file, print “Hx” or “Mx” for a hit or miss in L1 (“1”), the VC (“v”) or the L2 (“2”). “*” is printed if the cache is not accessed. For example, the output would be “M1MvM2,” for a miss in all caches. “H1****” means a hit in L1 etc.

The following statistics are printed: 
a. number of accesses to the cache
b. number of accesses to L2
c. number of accesses to VC
d. number of reads
e. number of read misses to L1
f. number of read misses to L2
g. number of writes
h. number of write misses to L1
i. number of write misses to L2
j. number of write backs from L1
k. number of write backs from L2
l. number of L1 victims that hit in the victim cache (for V > 0)
m. the average access time(AAT) for L1
