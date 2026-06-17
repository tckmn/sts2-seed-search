/*
 * sts2 seed search     tckmn <andy@tck.mn>
 * cleaned up version (see uncleaned.c for my original code)
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

const int MBIG =  0x7fffffff;
const int MSEED = 161803398;
const int MZ = 0;

int init(int *inext, int *inextp, int *SeedArray, int Seed) {
    int ii;
    int mj, mk;

    int subtraction = Seed < 0 ? -Seed : Seed;
    // technically the below is correct, but it's overwhelmingly likely to not matter so let's ignore it
    // int subtraction = Seed == 0x80000000 ? 0x7FFFFFFF : Seed < 0 ? -Seed : Seed;
    mj = MSEED - subtraction;
    SeedArray[55]=mj;
    mk=1;
    for (int i=1; i<55; i++) {
      ii = (21*i)%55;
      SeedArray[ii]=mk;
      mk = mj - mk;
      if (mk<0) mk+=MBIG;
      mj=SeedArray[ii];
    }
    for (int k=1; k<5; k++) {
      for (int i=1; i<56; i++) {
    SeedArray[i] -= SeedArray[1+(i+30)%55];
    if (SeedArray[i]<0) SeedArray[i]+=MBIG;
      }
    }
    *inext=0;
    *inextp = 21;
    Seed = 1;
}

int next(int *inext, int *inextp, int *SeedArray) {
    int retVal;

    int locINext = *inext;
    int locINextp = *inextp;

    if (++locINext >=56) locINext=1;
    if (++locINextp>= 56) locINextp = 1;

    retVal = SeedArray[locINext]-SeedArray[locINextp];

    if (retVal == MBIG) retVal--;
    if (retVal<0) retVal+=MBIG;

    SeedArray[locINext]=retVal;

    *inext = locINext;
    *inextp = locINextp;

    return retVal;
}

double sample(int *inext, int *inextp, int *SeedArray) { return next(inext, inextp, SeedArray) * (1.0/MBIG); }
int nextint(int *inext, int *inextp, int *SeedArray, int max) { return (int)(sample(inext, inextp, SeedArray) * max); }
int advance(int *inext, int *inextp, int *SeedArray, int num) { while (num--) next(inext, inextp, SeedArray); }

// expects arr to be malloc'd to length at least count
int shuf(int *inext, int *inextp, int *SeedArray, int *arr, int count) {
    for (int i = 0; i < count; ++i) arr[i] = i;
    int num = count;
    while (num > 1) {
        --num;
        int num2 = nextint(inext, inextp, SeedArray, num+1);
        int tmp = arr[num];
        arr[num] = arr[num2];
        arr[num2] = tmp;
    }
}

// can add more here
int AROMA_OF_CHAOS, BG_MUSIC, COMBAT_ENERGY_COSTS, COMBAT_TARGETS,
    CORPSE_SLUGS_WEAK, CRYSTAL_SPHERE, DOLL_ROOM, MORPHIC_GROVE, NEOW, NICHE,
    OROBAS, PAEL, REWARDS, SHOPS, SYMBIOTE, TABLET_OF_TRUTH, TEZCATARA,
    TRANSFORMATIONS, TRASH_HEAP, UNKNOWN_MAP_POINT, UP_FRONT;

// example to check distribution of neow's bones curse in underdocks
void example_neowsBones(char *seed) {
    int a, b; int c[56]; int base = hashcode(seed);

    // check for bones
    init(&a, &b, c, base + NEOW);
    if (nextint(&a, &b, c, 8) != 4) return;

    // check for underdocks
    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 0) return;

    // output which curse we got
    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("niche")));
    printf("%d\n", nextint(&a, &b, c, 10));
}

// example to create neow curse relic vs. first leafy poultice transform scatterplot
void example_neowTransformCorrelation(char *seed) {
    int a, b; int c[56]; int base = hashcode(seed);

    init(&a, &b, c, base + NEOW);
    double fst = sample(&a, &b, c);

    init(&a, &b, c, base + TRANSFORMATIONS);
    double snd = sample(&a, &b, c);

    printf("%f\t%f\n", fst, snd);
}

// example to find kaleidoscopes with the first reward all rare
void example_kaleidoscope(char *seed, int *buf) {
    int a, b; int c[56]; int base = hashcode(seed);

    init(&a, &b, c, base + REWARDS);
    // change this number to change how many consecutive rares we want
    for (int i = 0; i < 3; ++i) {
        if (sample(&a, &b, c) > 0.0149) return;
        next(&a, &b, c);
        next(&a, &b, c);
    }

    // simulate neow process
    // kaleidoscope is ordinarily 5th of 16 choices
    // 0 and 1 remove early
    // 2 and 3 and 5 remove late
    init(&a, &b, c, base + NEOW);
    int neow = nextint(&a, &b, c, 8);
    if (neow != 2) next(&a, &b, c);
    next(&a, &b, c);
    next(&a, &b, c);
    shuf(&a, &b, c, buf, neow < 6 && neow != 4 ? 15 : 16);
    int idx = neow < 2 ? 3 : 4;

    if (buf[0] == idx || buf[1] == idx) puts(seed);
}

// see the uncleaned file for more examples

int main(int argc, char* argv[]) {
    AROMA_OF_CHAOS = 1 + hashcode("AROMA_OF_CHAOS");
    BG_MUSIC = hashcode("bg_music");
    COMBAT_ENERGY_COSTS = hashcode("combat_energy_costs");
    COMBAT_TARGETS = hashcode("combat_targets");
    CORPSE_SLUGS_WEAK = hashcode("CORPSE_SLUGS_WEAK");
    CRYSTAL_SPHERE = 1 + hashcode("CRYSTAL_SPHERE");
    DOLL_ROOM = 1 + hashcode("DOLL_ROOM");
    MORPHIC_GROVE = 1 + hashcode("MORPHIC_GROVE");
    NEOW = 1 + hashcode("NEOW");
    NICHE = hashcode("niche");
    OROBAS = 1 + hashcode("OROBAS");
    PAEL = 1 + hashcode("PAEL");
    REWARDS = 1 + hashcode("rewards");
    SHOPS = 1 + hashcode("shops");
    SYMBIOTE = 1 + hashcode("SYMBIOTE");
    TABLET_OF_TRUTH = 1 + hashcode("TABLET_OF_TRUTH");
    TEZCATARA = 1 + hashcode("TEZCATARA");
    TRANSFORMATIONS = 1 + hashcode("transformations");
    TRASH_HEAP = 1 + hashcode("TRASH_HEAP");
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
        example_kaleidoscope(seed, buf);
   }

    return 0;
}
