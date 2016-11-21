#ifdef CCOMPILER
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#endif

#include <unistd.h>
#include "cachesim.hpp"

void print_help_and_exit(void) {
    printf("cachesim [OPTIONS] < traces/file.trace\n");
    printf("-h\t\tThis helpful output\n");
    printf("L1 parameters:\n");
    printf("  -c C1\t\tTotal size in bytes is 2^C1\n");
    printf("  -b B1\t\tSize of each block in bytes is 2^B1\n");
    printf("  -s S1\t\tNumber of blocks per set is 2^S1\n");
    printf("Victim cache parameters:\n");
    printf("  -v V\t\tNumber of blocks per set is V\n");
    printf("L2 parameters:\n");
    printf("  -C C2\t\tTotal size in bytes is 2^C2\n");
    printf("  -B B2\t\tSize of each block in bytes is 2^B2\n");
    printf("  -S S2\t\tNumber of blocks per set is 2^S2\n");
    exit(0);
}

void print_statistics(cache_stats_t* p_stats);

int main(int argc, char* argv[]) {
    int opt;
    uint64_t c1 = DEFAULT_C1;
    uint64_t b1 = DEFAULT_B1;
    uint64_t s1 = DEFAULT_S1;
    uint64_t c2 = DEFAULT_C2;
    uint64_t b2 = DEFAULT_B2;
    uint64_t s2 = DEFAULT_S2;
    uint64_t v = DEFAULT_V;

    /* Read arguments */
    while(-1 != (opt = getopt(argc, argv, "c:b:s:v:C:B:S:h"))) {
        switch(opt) {
        case 'c':
            c1 = atoi(optarg);
            break;
        case 'b':
            b1 = atoi(optarg);
            break;
        case 's':
            s1 = atoi(optarg);
            break;
        case 'v':
            v = atoi(optarg);
            break;
        case 'C':
            c2 = atoi(optarg);
            break;
        case 'B':
            b2 = atoi(optarg);
            break;
        case 'S':
            s2 = atoi(optarg);
            break;
        case 'h':
            /* Fall through */
        default:
            print_help_and_exit();
            break;
        }
    }

    printf("Cache Settings\n");
    printf("c: %" PRIu64 "\n", c1);
    printf("b: %" PRIu64 "\n", b1);
    printf("s: %" PRIu64 "\n", s1);
    printf("v: %" PRIu64 "\n", v);
    printf("C: %" PRIu64 "\n", c2);
    printf("B: %" PRIu64 "\n", b2);
    printf("S: %" PRIu64 "\n", s2);
    printf("\n");

    /* Setup the cache */
    setup_cache(c1, b1, s1, v, c2, b2, s2);

    /* Setup statistics */
    cache_stats_t stats;
    memset(&stats, 0, sizeof(cache_stats_t));

    /* Begin reading the file */
    char rw;
    uint64_t address;
    while (!feof(stdin)) {
        int ret = fscanf(stdin, "%c %" PRIx64 "\n", &rw, &address);
        if(ret == 2) {
            cache_access(rw, address, &stats);
        }
    }

    complete_cache(&stats);

    print_statistics(&stats);

    return 0;
}

void print_statistics(cache_stats_t* p_stats) {
    printf("Cache Statistics\n");
    printf("Accesses: %" PRIu64 "\n", p_stats->accesses);
    printf("Accesses to L2: %" PRIu64 "\n", p_stats->accesses_l2);
    printf("Accesses to VC: %" PRIu64 "\n", p_stats->accesses_vc);
    printf("Reads: %" PRIu64 "\n", p_stats->reads);
    printf("Read misses to L1: %" PRIu64 "\n", p_stats->read_misses_l1);
    printf("Read misses to L2: %" PRIu64 "\n", p_stats->read_misses_l2);
    printf("Writes: %" PRIu64 "\n", p_stats->writes);
    printf("Write misses to L1: %" PRIu64 "\n", p_stats->write_misses_l1);
    printf("Write misses to L2: %" PRIu64 "\n", p_stats->write_misses_l2);
    printf("Write backs from L1: %" PRIu64 "\n", p_stats->write_back_l1);
    printf("Write backs from L2: %" PRIu64 "\n", p_stats->write_back_l2);
    printf("L1 victims hit in victim cache: %" PRIu64 "\n", p_stats->victim_hits);
    printf("Average access time (AAT) for L1: %f\n", p_stats->avg_access_time_l1);
}

