/*
 * sts2 seed search (v0.107.1)      tckmn <andy@tck.mn>
 * cleaned up version (see uncleaned.c for my original code)
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "combats.h"

// uncomment to occasionally output an indicator of what seed we're on
// #define PROGRESS

int hashcode(char *s) {
    unsigned int num = 352654597;
    unsigned int num2 = num;
    for (int i = 0; s[i]; i += 2) {
        num = ((num << 5) + num) ^ s[i];
        if (!s[i+1]) break;
        num2 = ((num2 << 5) + num2) ^ s[i + 1];
    }
    return num + num2 * 1566083941;
}

typedef struct {
    unsigned long s0, s1, s2, s3;
} rng;

int init(rng *r, unsigned int preseed) {
    unsigned long seed = preseed;
    unsigned long num;

    num = seed += 11400714819323198485uL;
    num = (num ^ (num >> 30)) * 13787848793156543929uL;
    num = (num ^ (num >> 27)) * 10723151780598845931uL;
    r->s0 = num ^ (num >> 31);

    num = seed += 11400714819323198485uL;
    num = (num ^ (num >> 30)) * 13787848793156543929uL;
    num = (num ^ (num >> 27)) * 10723151780598845931uL;
    r->s1 = num ^ (num >> 31);

    num = seed += 11400714819323198485uL;
    num = (num ^ (num >> 30)) * 13787848793156543929uL;
    num = (num ^ (num >> 27)) * 10723151780598845931uL;
    r->s2 = num ^ (num >> 31);

    num = seed += 11400714819323198485uL;
    num = (num ^ (num >> 30)) * 13787848793156543929uL;
    num = (num ^ (num >> 27)) * 10723151780598845931uL;
    r->s3 = num ^ (num >> 31);
}

unsigned long next(rng *r) {
    unsigned long ret = (((r->s1*5) << 7) | ((r->s1*5) >> 57)) * 9;
    unsigned long tmp = r->s1 << 17;
    r->s2 ^= r->s0;
    r->s3 ^= r->s1;
    r->s1 ^= r->s2;
    r->s0 ^= r->s3;
    r->s2 ^= tmp;
    r->s3 = (r->s3 << 45) | (r->s3 >> 19);
    return ret;
}

double sample(rng *r) { return (double)(next(r) >> 11) * 1.1102230246251565E-16; }
int nextint(rng *r, int max) { return (int)(sample(r) * max); }
int advance(rng *r, int num) { while (num--) next(r); }

// expects arr to be malloc'd to length at least count
int shuf(rng *r, int *arr, int count) {
    for (int i = 0; i < count; ++i) arr[i] = i;
    int num = count;
    while (num > 1) {
        --num;
        int num2 = nextint(r, num+1);
        int tmp = arr[num];
        arr[num] = arr[num2];
        arr[num2] = tmp;
    }
}

// e.g. calling with n=1 checks if it's the first
// calling with n=2 checks if it's among the first two
// does not call nextint enough times, make sure to advance it if continuing
int firstNAfterShuf(rng *r, int count, int target, int n) {
    int targetPos = target;
    for (int i = count; i > n;) {
        int k = nextint(r, i);
        if (k == targetPos) return 0;
        --i;
        if (i == targetPos) targetPos = k;
    }
    return 1;
}

int firstTwoAfterShuf(rng *r, int count, int target1, int target2) {
    int target1Pos = target1, target2Pos = target2;
    for (int i = count; i > 2;) {
        int k = nextint(r, i);
        if (k == target1Pos || k == target2Pos) return 0;
        --i;
        if (i == target1Pos) target1Pos = k;
        else if (i == target2Pos) target2Pos = k;
    }
    return 1;
}

// toggle debug
#if 0
#define PREV(arr, x) do { prev = x; printf("[%s] ", arr[x & 0xff]); } while (0)
#define DEBUGLINE puts("")
#else
#define PREV(arr, x) do { prev = x; } while (0)
#define DEBUGLINE 0
#endif

#define HARD(act, fullPool, num) do {                                           \
    poolsize = 0;                                                               \
    for (int i = 0; i < num; ++i) {                                             \
        if (poolsize == 0) {                                                    \
            poolsize = sizeof(fullPool)/sizeof(int);                            \
            for (int j = 0; j < poolsize; ++j) pool[j] = fullPool[j];           \
        }                                                                       \
        int candidate, doCheck = 0;                                             \
        for (int j = 0; j < poolsize; ++j) {                                    \
            if (!((pool[j] & prev & ~0xff) || pool[j] == prev)) {               \
                doCheck = 1;                                                    \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        do {                                                                    \
            candidate = nextint(&r, poolsize);                                  \
        } while (doCheck && ((pool[candidate] & prev & ~0xff) || pool[candidate] == prev));  \
        PREV(act, pool[candidate]);                                             \
        for (int j = candidate+1; j < poolsize; ++j) pool[j-1] = pool[j];       \
        --poolsize;                                                             \
    }                                                                           \
    DEBUGLINE;                                                                  \
} while (0);

void advance_elites(rng *r) {
    int prev = -1;
    for (int i = 0; i < 5; ++i) {
        int e1;
        do { e1 = nextint(r, 3); } while (e1 == prev);
        int e2 = nextint(r, 2);
        prev = 2-(e1+e2*3)/2; // lmao
        next(r);
    }
}

// can add more here
int ACT_SELECTION, AROMA_OF_CHAOS, BG_MUSIC, COMBAT_ENERGY_COSTS,
    COMBAT_TARGETS, CORPSE_SLUGS_WEAK, CRYSTAL_SPHERE, DOLL_ROOM,
    MORPHIC_GROVE, NEOW, NICHE, NONUPEIPE, OROBAS, PAEL, REWARDS, SHOPS,
    SYMBIOTE, TABLET_OF_TRUTH, TEZCATARA, TRANSFORMATIONS, TRASH_HEAP,
    UNKNOWN_MAP_POINT, UP_FRONT;

// example to check distribution of neow's bones curse in underdocks
void example_neowsBones(char *seed) {
    rng r; int base = hashcode(seed);

    // check for bones
    init(&r, base + NEOW);
    if (nextint(&r, 8) != 4) return;

    // check for underdocks
    init(&r, hashcode(seed));
    if (nextint(&r, 2) != 0) return;

    // output which curse we got
    init(&r, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("niche")));
    printf("%d\n", nextint(&r, 10));
}

// example to create neow curse relic vs. first leafy poultice transform scatterplot
void example_neowTransformCorrelation(char *seed) {
    rng r; int base = hashcode(seed);

    init(&r, base + NEOW);
    double fst = sample(&r);

    init(&r, base + TRANSFORMATIONS);
    double snd = sample(&r);

    printf("%f\t%f\n", fst, snd);
}

// example to find kaleidoscopes with the first reward all rare
void example_kaleidoscope(char *seed, int *buf) {
    rng r; int base = hashcode(seed);

    init(&r, base + REWARDS);
    // change this number to change how many consecutive rares we want
    for (int i = 0; i < 3; ++i) {
        if (sample(&r) > 0.0149) return;
        next(&r);
        next(&r);
    }

    // simulate neow process
    // kaleidoscope is ordinarily 5th of 16 choices
    // 0 and 1 remove early
    // 2 and 3 and 5 remove late
    init(&r, base + NEOW);
    int neow = nextint(&r, 8);
    if (neow != 2) next(&r);
    next(&r);
    next(&r);
    shuf(&r, buf, neow < 6 && neow != 4 ? 15 : 16);
    int idx = neow < 2 ? 3 : 4;

    if (buf[0] == idx || buf[1] == idx) puts(seed);
}

// example to find seeds with Silken Tress + Nonupeipe Glitter
void example_glam(char *seed) {
    rng r; int base = hashcode(seed);

    // check for neow tress
    init(&r, base + NEOW);
    if (nextint(&r, 8) != 6) return;

    // check for nonu glitter
    init(&r, base + NONUPEIPE);
    if (!firstNAfterShuf(&r, 10, 5, 3)) return;
    init(&r, base + NONUPEIPE);
    if (!firstNAfterShuf(&r, 9, 5, 3)) return;

    // need act for events
    init(&r, base + ACT_SELECTION);
    int act = nextint(&r, 2);

    init(&r, base + UP_FRONT);
    advance(&r,
            30-1 + 25-1 + 35-1 + 25-1 + 1 + // shared relic
            32-1 + 26-1 + 38-1 + 26-1 // player relic
            );

    // darv
    int darv2 = nextint(&r, 2);
    int darv3 = nextint(&r, 2) && !darv2;

    // prepare for acts
    int prev;
    int pool[16];
    int poolsize;

    // act 1
    advance(&r, (act?10:13)+18-1); // events
    // easy pool
    int e11 = nextint(&r, 4), e12 = nextint(&r, 3), e13 = nextint(&r, 2);
    if (e12 >= e11) {
        e12 += 1;
        if (e13 >= e11) e13 += 1;
        if (e13 >= e12) e13 += 1;
    } else {
        if (e13 >= e12) e13 += 1;
        if (e13 >= e11) e13 += 1;
    }
    // hard pool
    if (act == 0) {
        // overgrowth
        PREV(OVERGROWTH, OVERGROWTH_EASY[e11]);
        PREV(OVERGROWTH, OVERGROWTH_EASY[e12]);
        PREV(OVERGROWTH, OVERGROWTH_EASY[e13]);
        HARD(OVERGROWTH, OVERGROWTH_HARD, 12);
    } else {
        // underdocks
        PREV(UNDERDOCKS, UNDERDOCKS_EASY[e11]);
        PREV(UNDERDOCKS, UNDERDOCKS_EASY[e12]);
        PREV(UNDERDOCKS, UNDERDOCKS_EASY[e13]);
        HARD(UNDERDOCKS, UNDERDOCKS_HARD, 12);
    }
    advance_elites(&r);
    advance(&r, 1+1); // boss + ancient

    // act 2
    advance(&r, 10+18-1); // events
    // combats
    int e21 = nextint(&r, 4), e22 = nextint(&r, 3);
    if (e22 >= e21) e22 += 1;
    PREV(HIVE, HIVE_EASY[e21]);
    PREV(HIVE, HIVE_EASY[e22]);
    HARD(HIVE, HIVE_HARD, 12);
    advance_elites(&r);
    advance(&r, 1+1); // boss + ancient

    // act 3
    advance(&r, 7+18-1); // events
    // combats
    int e31 = nextint(&r, 3), e32 = nextint(&r, 2);
    if (e32 >= e31) e32 += 1;
    PREV(GLORY, GLORY_EASY[e31]);
    PREV(GLORY, GLORY_EASY[e32]);
    HARD(GLORY, GLORY_HARD, 11);
    advance_elites(&r);
    advance(&r, 1); // boss
    if (nextint(&r, 3 + (darv3 ? 1 : 0)) != 0) return; // check for nonu

    puts(seed);
}

// see the uncleaned file for more examples

int main(int argc, char* argv[]) {
    ACT_SELECTION = hashcode("act_selection");
    AROMA_OF_CHAOS = hashcode("AROMA_OF_CHAOS");
    BG_MUSIC = hashcode("bg_music");
    COMBAT_ENERGY_COSTS = hashcode("combat_energy_costs");
    COMBAT_TARGETS = hashcode("combat_targets");
    CORPSE_SLUGS_WEAK = hashcode("CORPSE_SLUGS_WEAK");
    CRYSTAL_SPHERE = hashcode("CRYSTAL_SPHERE");
    DOLL_ROOM = hashcode("DOLL_ROOM");
    MORPHIC_GROVE = hashcode("MORPHIC_GROVE");
    NEOW = hashcode("NEOW");
    NICHE = hashcode("niche");
    NONUPEIPE = hashcode("NONUPEIPE");
    OROBAS = hashcode("OROBAS");
    PAEL = hashcode("PAEL");
    REWARDS = hashcode("rewards");
    SHOPS = hashcode("shops");
    SYMBIOTE = hashcode("SYMBIOTE");
    TABLET_OF_TRUTH = hashcode("TABLET_OF_TRUTH");
    TEZCATARA = hashcode("TEZCATARA");
    TRANSFORMATIONS = hashcode("transformations");
    TRASH_HEAP = hashcode("TRASH_HEAP");
    UNKNOWN_MAP_POINT = hashcode("unknown_map_point");
    UP_FRONT = hashcode("up_front");

    char seed[11] = "AAAAAAAAAA";
    int buf[20];

    for (;;) {
        int i = 0;
        while (seed[i] == 'Z') {
#ifdef PROGRESS
            if (i==4) printf("- %s -\n", seed);
#endif
            seed[i] = 'A'; ++i;
        }
        seed[i] += 1;
        if (seed[i] == 'I' || seed[i] == 'O') ++seed[i];

        // put your test here
        example_glam(seed);
   }

    return 0;
}
