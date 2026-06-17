/*
 * sts2 seed search     tckmn <andy@tck.mn>
 * extremley messy version (see cleaned.c for something more usable)
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int NEOW, REWARDS, UP_FRONT, TRANSFORMATIONS, DOLL_ROOM, CRYSTAL_SPHERE, UNKNOWN_MAP_POINT, TRASH_HEAP, AROMA_OF_CHAOS, SHOPS, TABLET_OF_TRUTH, SYMBIOTE, MORPHIC_GROVE, COMBAT_TARGETS, COMBAT_ENERGY_COSTS, TEZCATARA, PAEL, OROBAS, BG_MUSIC, NICHE, CORPSE_SLUGS_WEAK;

int init(int *inext, int *inextp, int *SeedArray, int Seed) {
    // int inext;
    // int inextp;
    // int SeedArray[56] = {0};

    int ii;
    int mj, mk;

    int subtraction = Seed < 0 ? -Seed : Seed; //(Seed == Int32.MinValue) ? Int32.MaxValue : Math.Abs(Seed);
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

int firstShuf(int *inext, int *inextp, int *SeedArray, int count) {
    int *arr = malloc(count * sizeof *arr);
    for (int i = 0; i < count; ++i) arr[i] = i;

    int num = count;
    while (num > 1) {
        --num;
        int num2 = nextint(inext, inextp, SeedArray, num+1);
        int tmp = arr[num];
        arr[num] = arr[num2];
        arr[num2] = tmp;
    }

    int ret = arr[0];
    free(arr);
    return ret;
}

void check(char *seed) {
    int a, b;
    int c[56];

    // // check for overgrowth
    // init(&a, &b, c, hashcode(seed));
    // printf("%d\n", nextint(&a, &b, c, 2));
    // // if (nextint(&a, &b, c, 2) != 0) return;

    // check for poultice
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    if (nextint(&a, &b, c, 8) != 3) return;

    // check for cotv + scythe
    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed) + 1) + (unsigned int)hashcode("transformations")));
    int fst = nextint(&a, &b, c, 80), snd = nextint(&a, &b, c, 80);
    if (!(fst == 7 && snd == 74 || fst == 74 && snd == 7)) return;

    // // check first event
    // init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed) + 1) + (unsigned int)hashcode("transformations")));
    // advance(&a, &b, c,
    //         26-1 + 32-1 + 38-1 + 26-1 + // relic pools
    //         2 // darv nonsense
    //         );
    // if (nextint(&a, &b, c, 31) != 5) return;

    // printf("%s\n", seed);
    // check for overgrowth
    init(&a, &b, c, hashcode(seed));
    printf("%f\n", sample(&a, &b, c));
    // printf("%d\n", nextint(&a, &b, c, 2));
    // if (nextint(&a, &b, c, 2) != 0) return;
}

void scatter(char *seed) {
    int a, b;
    int c[56];

    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 1) return;

    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed) + 1) + (unsigned int)hashcode("NEOW")));
    double fst = sample(&a, &b, c);

    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed) + 1) + (unsigned int)hashcode("transformations")));
    double snd = sample(&a, &b, c);

    printf("%f\t%f\n", fst, snd);
}

void bones(char *seed) {
    int a, b;
    int c[56];

    // check for bones
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    if (nextint(&a, &b, c, 8) != 4) return;

    // check for docks (0) overgrowth (1)
    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 1) return;

    // printf("%s\n", seed);

    // init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("niche")));
    // printf("%f\n", sample(&a, &b, c));

    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("niche")));
    printf("%d\n", nextint(&a, &b, c, 10));
}

void target(char *seed) {
    int a, b;
    int c[56];

    // check for growth
    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 1) return;

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    double fst = sample(&a, &b, c);

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)hashcode("combat_targets")));
    double snd = sample(&a, &b, c);

    // if (fst < 0.5) printf("%s\n", seed);

    printf("%f\t%f\n", fst, snd);
}

void shop(char *seed) {
    int a, b;
    int c[56];

    // check for growth
    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 1) return;

    // check for cursed pearl
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    if (nextint(&a, &b, c, 8) != 0) return;

    // gold
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("rewards")));
    int fst = nextint(&a, &b, c, 9);

    // shop common relic
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)hashcode("up_front")));
    int snd = nextint(&a, &b, c, 26);

    printf("%d\t%d\n", fst, snd);
}

void stuff(char *seed) {
    int a, b;
    int c[56];

    int base = hashcode(seed);

    init(&a, &b, c, base);
    int act1 = nextint(&a, &b, c, 2);
    int randchar = nextint(&a, &b, c, 5);

    init(&a, &b, c, base + NEOW);
    int neowcurse = nextint(&a, &b, c, 8);
    int neow2 = nextint(&a, &b, c, 2);

    init(&a, &b, c, base + REWARDS);
    double rewardssample = sample(&a, &b, c);
    int rare = (int)(rewardssample*25);
    int relicrar = rewardssample < 0.5 ? 0 : rewardssample < 0.83 ? 1 : 2;
    int pot = rewardssample < 0.4 ? 1 : 0;
    int gold = nextint(&a, &b, c, 9);
    int potrar = 0, potval = 0;
    if (pot) {
        double x = sample(&a, &b, c);
        potrar = x < 0.1 ? 3 : x < 0.35 ? 2 : 1;
        potval = nextint(&a, &b, c, 16);
    }
    int uncommon = sample(&a, &b, c) < 0.3349 ? 11 : 10;

    init(&a, &b, c, base + UP_FRONT);
    int shopcom1 = nextint(&a, &b, c, 26);
    int shopcom2 = nextint(&a, &b, c, 26);

    init(&a, &b, c, base + CRYSTAL_SPHERE);
    next(&a, &b, c);
    int divination = nextint(&a, &b, c, 40);

    init(&a, &b, c, base + TRANSFORMATIONS);
    int transform = nextint(&a, &b, c, 80);

    init(&a, &b, c, base + DOLL_ROOM);
    int doll = nextint(&a, &b, c, 3);

    init(&a, &b, c, base + UNKNOWN_MAP_POINT);
    int qmark = sample(&a, &b, c) < 0.1 ? 1 : 0;

    init(&a, &b, c, base + TRASH_HEAP);
    int trash = nextint(&a, &b, c, 10);

    init(&a, &b, c, base + AROMA_OF_CHAOS);
    int aroma = nextint(&a, &b, c, 80);

    init(&a, &b, c, base + SHOPS);
    int salecard = nextint(&a, &b, c, 5);
    double card1 = sample(&a, &b, c);
    int price1 = (int)lround(50*(0.95+sample(&a, &b, c)*0.1));

    init(&a, &b, c, base + TABLET_OF_TRUTH);
    int tablet = nextint(&a, &b, c, 12);

    init(&a, &b, c, base + SYMBIOTE);
    int symbiote = nextint(&a, &b, c, 80);

    init(&a, &b, c, base + MORPHIC_GROVE);
    int grove = nextint(&a, &b, c, 80);

    init(&a, &b, c, base + COMBAT_TARGETS);
    int target = nextint(&a, &b, c, 2);

    init(&a, &b, c, base + COMBAT_ENERGY_COSTS);
    int slither = nextint(&a, &b, c, 4);

    init(&a, &b, c, base + TEZCATARA);
    int tez1 = nextint(&a, &b, c, 3);
    int tez2 = nextint(&a, &b, c, 3);
    int tez3 = nextint(&a, &b, c, 4);

    init(&a, &b, c, base + PAEL);
    int pael1 = nextint(&a, &b, c, 3);
    int pael2 = nextint(&a, &b, c, 7);

    init(&a, &b, c, base + OROBAS);
    int orocolor = nextint(&a, &b, c, 4);
    int orogem = sample(&a, &b, c) < 0.3333333f;
    int oro1 = nextint(&a, &b, c, 4);

    init(&a, &b, c, base + BG_MUSIC);
    int music = nextint(&a, &b, c, 2);

    init(&a, &b, c, base + NICHE);
    double nichesample = sample(&a, &b, c);
    int bones = (int)(nichesample*10);
    int leaf = (int)(nichesample*80);

    init(&a, &b, c, base + 2 + CORPSE_SLUGS_WEAK);
    int slug = nextint(&a, &b, c, 3);

    // puts(seed);

    // printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\n", seed, act1, neowcurse, gold, shopcommonrelic, transform, doll);
    // printf("%d\t%d\t%d\t%d\t%d\t%d\n", act1, neowcurse, gold, shopcommonrelic, transform, doll);
    // printf("%d\t%d\t%d\n", shopcom2, gold, divination);
    // printf("%d\t%d\n", salecard, symbiote);
    // if (neowcurse == 3) printf("%d\t%d\n", act1, transform);
    // if (act1 == 1) printf("%d\t%d\t%d\t%d\n", neowcurse, shopcom1);
    // printf("%d\t%d\n", potrar == 0 ? uncommon : potrar, oro1);
    // if (neowcurse != 2) printf("%d\t%d\t%d\n", act1, neowcurse, relicrar);
    // printf("%d\t%d\t%d\n", act1, neowcurse, doll);
    // printf("%d\t%d\n", price1, oro1);
    // printf("%d\t%d\t%d\n", act1, neowcurse, qmark);
    // if (act1 == 0) printf("%d\t%d\t%d\n", neowcurse, slug, target);
    // printf("%d\t%d\t%d\n", act1, neowcurse, tez1);
    printf("%d\t%d\n", randchar, gold);
}

void qmark(char *seed) {
    int a,b; int c[56];

    // init(&a, &b, c, hashcode(seed));
    // double fst = sample(&a, &b, c);

    // init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)hashcode("unknown_map_point")));
    // double snd = sample(&a, &b, c);

    // printf("%f\t%f\n", fst, snd);

    // init(&a, &b, c, hashcode(seed));
    // int fst = nextint(&a, &b, c, 2);

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    int fst = nextint(&a, &b, c, 8);

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)hashcode("unknown_map_point")));
    double x = sample(&a, &b, c);
    if (x > 0.1 && x < 0.12) printf("%s\n", seed);
    int snd = x < 0.1 ? 1 : 0;

    printf("%d\t%d\n", fst, snd);
}

void shopcard(char *seed) {
    int a,b; int c[56];

    // potion
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + 1 + (unsigned long)hashcode("rewards")));
    next(&a, &b, c);
    if (sample(&a, &b, c) > 0.4) return;

    // qmark
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)hashcode("unknown_map_point")));
    next(&a, &b, c);
    if (sample(&a, &b, c) > 0.2) return;

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + 1 + (unsigned long)hashcode("shops")));
    next(&a, &b, c);
    double fst = sample(&a, &b, c);

    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + 1 + (unsigned long)hashcode("CRYSTAL_SPHERE")));
    next(&a, &b, c);
    double snd = sample(&a, &b, c);

    printf("%s\t%f\t%f\n", seed, fst, snd);
}

void poultice(char *seed) {
    int a, b;
    int c[56];

    // check for poultice
    init(&a, &b, c, (unsigned int)((unsigned int)hashcode(seed) + (unsigned long)1 + (unsigned long)hashcode("NEOW")));
    if (nextint(&a, &b, c, 8) != 3) return;

    // check for docks (0) overgrowth (1)
    init(&a, &b, c, hashcode(seed));
    if (nextint(&a, &b, c, 2) != 1) return;

    // printf("%s\n", seed);

    // init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("niche")));
    // printf("%f\n", sample(&a, &b, c));

    init(&a, &b, c, (unsigned int)((unsigned int)((unsigned long)hashcode(seed)) + (unsigned int)hashcode("transformations")));
    printf("%d\n", nextint(&a, &b, c, 80));
}

void rarekaleid(char *seed) {
    int a,b; int c[56];
    int base = hashcode(seed);

    // init(&a, &b, c, base + NEOW);
    // if (nextint(&a, &b, c, 8) != 3) return;

    init(&a, &b, c, base + REWARDS);
    for (int i = 0; i < 6; ++i) {
        if (sample(&a, &b, c) > 0.0149) return;
        next(&a, &b, c);
        next(&a, &b, c);
    }

    puts(seed);
}

/*
		RelicOption<CursedPearl>("INITIAL", "NEOW.pages.DONE.CURSED.description"),
		RelicOption<HeftyTablet>("INITIAL", "NEOW.pages.DONE.CURSED.description"),
		RelicOption<LargeCapsule>("INITIAL", "NEOW.pages.DONE.CURSED.description"),
		RelicOption<LeafyPoultice>("INITIAL", "NEOW.pages.DONE.CURSED.description"),
		RelicOption<NeowsBones>("INITIAL", "NEOW.pages.DONE.POSITIVE.description"),
		RelicOption<PrecariousShears>("INITIAL", "NEOW.pages.DONE.CURSED.description"),
        tress
		RelicOption<SilverCrucible>("INITIAL", "NEOW.pages.DONE.CURSED.description")
*/

/*
			ModelDb.Card<Clumsy>(),
			ModelDb.Card<Debt>(),
			ModelDb.Card<Decay>(),
			ModelDb.Card<Doubt>(),
			ModelDb.Card<Guilty>(),
			ModelDb.Card<Injury>(),
			ModelDb.Card<Normality>(),
			ModelDb.Card<Regret>(),
			ModelDb.Card<Shame>(),
			ModelDb.Card<Writhe>()
*/

/*
 0   ModelDb.Card<Afterlife>(),
 1   ModelDb.Card<BansheesCry>(),
 2   ModelDb.Card<BlightStrike>(),
 3   ModelDb.Card<BoneShards>(),
 4   ModelDb.Card<BorrowedTime>(),
 5   ModelDb.Card<Bury>(),
 6   ModelDb.Card<Calcify>(),
 7   ModelDb.Card<CallOfTheVoid>(),
 8   ModelDb.Card<CaptureSpirit>(),
 9   ModelDb.Card<Cleanse>(),
10   ModelDb.Card<Countdown>(),
11   ModelDb.Card<DanseMacabre>(),
12   ModelDb.Card<DeathMarch>(),
13   ModelDb.Card<Deathbringer>(),
14   ModelDb.Card<DeathsDoor>(),
15   ModelDb.Card<Debilitate>(),
16   ModelDb.Card<Defile>(),
17   ModelDb.Card<Defy>(),
18   ModelDb.Card<Delay>(),
19   ModelDb.Card<Demesne>(),
20   ModelDb.Card<DevourLife>(),
21   ModelDb.Card<Dirge>(),
22   ModelDb.Card<DrainPower>(),
23   ModelDb.Card<Dredge>(),
24   ModelDb.Card<Eidolon>(),
25   ModelDb.Card<EndOfDays>(),
26   ModelDb.Card<EnfeeblingTouch>(),
27   ModelDb.Card<Eradicate>(),
28   ModelDb.Card<Fear>(),
29   ModelDb.Card<Fetch>(),
30   ModelDb.Card<Flatten>(),
31   ModelDb.Card<Friendship>(),
32   ModelDb.Card<GraveWarden>(),
33   ModelDb.Card<Graveblast>(),
34   ModelDb.Card<Hang>(),
35   ModelDb.Card<Haunt>(),
36   ModelDb.Card<HighFive>(),
37   ModelDb.Card<Invoke>(),
38   ModelDb.Card<Lethality>(),
39   ModelDb.Card<Melancholy>(),
40   ModelDb.Card<Misery>(),
41   ModelDb.Card<NecroMastery>(),
42   ModelDb.Card<NegativePulse>(),
43   ModelDb.Card<Neurosurge>(),
44   ModelDb.Card<NoEscape>(),
45   ModelDb.Card<Oblivion>(),
46   ModelDb.Card<Pagestorm>(),
47   ModelDb.Card<Parse>(),
48   ModelDb.Card<Poke>(),
49   ModelDb.Card<PullAggro>(),
50   ModelDb.Card<PullFromBelow>(),
51   ModelDb.Card<Putrefy>(),
52   ModelDb.Card<Rattle>(),
53   ModelDb.Card<Reanimate>(),
54   ModelDb.Card<Reap>(),
55   ModelDb.Card<ReaperForm>(),
56   ModelDb.Card<Reave>(),
57   ModelDb.Card<RightHandHand>(),
58   ModelDb.Card<Sacrifice>(),
59   ModelDb.Card<Scourge>(),
60   ModelDb.Card<SculptingStrike>(),
61   ModelDb.Card<Seance>(),
62   ModelDb.Card<SentryMode>(),
63   ModelDb.Card<Severance>(),
64   ModelDb.Card<SharedFate>(),
65   ModelDb.Card<Shroud>(),
66   ModelDb.Card<SicEm>(),
67   ModelDb.Card<SleightOfFlesh>(),
68   ModelDb.Card<Snap>(),
69   ModelDb.Card<SoulStorm>(),
70   ModelDb.Card<Sow>(),
71   ModelDb.Card<SpiritOfAsh>(),
72   ModelDb.Card<Spur>(),
73   ModelDb.Card<Squeeze>(),
74   ModelDb.Card<TheScythe>(),
75   ModelDb.Card<TimesUp>(),
76   ModelDb.Card<Transfigure>(),
77   ModelDb.Card<Undeath>(),
78   ModelDb.Card<Veilpiercer>(),
79   ModelDb.Card<Wisp>()
*/

/*
 0   ModelDb.Event<AromaOfChaos>(),
 1   ModelDb.Event<ByrdonisNest>(),
 2   ModelDb.Event<DenseVegetation>(),
 3   ModelDb.Event<JungleMazeAdventure>(),
 4   ModelDb.Event<LuminousChoir>(),
 5   ModelDb.Event<MorphicGrove>(),
 6   ModelDb.Event<SapphireSeed>(),
 7   ModelDb.Event<SunkenStatue>(),
 8   ModelDb.Event<TabletOfTruth>(),
 9   ModelDb.Event<UnrestSite>(),
10   ModelDb.Event<Wellspring>(),
11   ModelDb.Event<WhisperingHollow>(),
12   ModelDb.Event<WoodCarvings>()
13   Event<BrainLeech>(),
14   Event<CrystalSphere>(),
15   Event<DollRoom>(),
16   Event<FakeMerchant>(),
17   Event<PotionCourier>(),
18   Event<RanwidTheElder>(),
19   Event<RelicTrader>(),
20   Event<RoomFullOfCheese>(),
21   Event<SelfHelpBook>(),
22   Event<SlipperyBridge>(),
23   Event<StoneOfAllTime>(),
24   Event<Symbiote>(),
25   Event<TeaMaster>(),
26   Event<TheFutureOfPotions>(),
27   Event<TheLegendsWereTrue>(),
28   Event<ThisOrThat>(),
29   Event<WarHistorianRepy>(),
30   Event<WelcomeToWongos>()
*/

int main(int argc, char* argv[]) {
    // printf("%d\n", hashcode("NEOW"));
    // printf("%d\n", hashcode("transformations"));

    NEOW = 1 + hashcode("NEOW");
    REWARDS = 1 + hashcode("rewards");
    UP_FRONT = hashcode("up_front");
    TRANSFORMATIONS = 1 + hashcode("transformations");
    DOLL_ROOM = 1 + hashcode("DOLL_ROOM");
    CRYSTAL_SPHERE = 1 + hashcode("CRYSTAL_SPHERE");
    UNKNOWN_MAP_POINT = hashcode("unknown_map_point");
    TRASH_HEAP = 1 + hashcode("TRASH_HEAP");
    AROMA_OF_CHAOS = 1 + hashcode("AROMA_OF_CHAOS");
    SHOPS = 1 + hashcode("shops");
    TABLET_OF_TRUTH = 1 + hashcode("TABLET_OF_TRUTH");
    SYMBIOTE = 1 + hashcode("SYMBIOTE");
    MORPHIC_GROVE = 1 + hashcode("MORPHIC_GROVE");
    COMBAT_TARGETS = hashcode("combat_targets");
    COMBAT_ENERGY_COSTS = hashcode("combat_energy_costs");
    TEZCATARA = 1 + hashcode("TEZCATARA");
    PAEL = 1 + hashcode("PAEL");
    OROBAS = 1 + hashcode("OROBAS");
    BG_MUSIC = hashcode("bg_music");
    NICHE = hashcode("niche");
    CORPSE_SLUGS_WEAK = hashcode("CORPSE_SLUGS_WEAK");

    // stuff("FTY2EK02B8");
    // stuff("E33ND54PTV");
    // stuff("85RMTMA8DE");
    // return 0;

    // rarekaleid("3YW6NGJQ6V");
    // return 0;

    char seed[9] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 0};
    for (;;) {
        int i = 0;
        while (seed[i] == 'Z') { seed[i] = 'A'; ++i; if (i==5) printf("- %s -\n", seed); }
        seed[i] += 1;
        if (seed[i] == 'I' || seed[i] == 'O') ++seed[i];
        // check(seed);
        // scatter(seed);
        // target(seed);
        // shop(seed);
        // stuff(seed);
        // qmark(seed);
        // shopcard(seed);

        // blog post
        // bones(seed);
        // poultice(seed);

        // stuff(seed);
        rarekaleid(seed);
    }

    return 0;
}
