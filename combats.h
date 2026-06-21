#define BURROWER        0x100
#define CHOMPER         0x200
#define NIBBIT          0x400
#define SHRINKER        0x800
#define SLIMES          0x1000
#define THIEVES         0x2000
#define WORKERS         0x4000
#define CRAWLER         0x8000
#define MUSHROOM        0x10000
#define KNIGHTS         0x20000
#define SCROLLS         0x40000
#define SEAPUNK         0x80000
#define SLUGS           0x100000
#define EXOSKELETONS    0x200000

static int OVERGROWTH_EASY[] = {
    0 | CRAWLER,            // fuzzy wurm crawler
    1 | NIBBIT,             // nibbit
    2 | SHRINKER,           // shrinker beetle
    3 | SLIMES              // 3 slimes
};

static int OVERGROWTH_HARD[] = {
    4,                      // cubex construct
    5 | MUSHROOM | SLIMES,  // slime + flyconid
    6,                      // fogmog
    7,                      // inklets
    8,                      // mawler
    9,                      // 2 nibbits
    10 | SHRINKER | CRAWLER,// shrinker + crawler
    11,                     // ruby raiders
    12 | SLIMES,            // 4 slimes
    13,                     // slithering strangler
    14 | MUSHROOM,          // snapping jaxfruit + flyconid
    15                      // vine shambler
};

static char* OVERGROWTH[] = {
    "fuzzy wurm crawler",
    "nibbit",
    "shrinker beetle",
    "3 slimes",
    "cubex construct",
    "slime + flyconid",
    "fogmog",
    "inklets",
    "mawler",
    "2 nibbits",
    "shrinker + crawler",
    "ruby raiders",
    "4 slimes",
    "slithering strangler",
    "snapping jaxfruit + flyconid",
    "vine shambler"
};

static int UNDERDOCKS_EASY[] = {
    0 | SLUGS,      // 2 corpse slugs
    1 | SEAPUNK,    // seapunk
    2,              // sludge spinner
    3               // toadpoles
};

static int UNDERDOCKS_HARD[] = {
    4 | SLUGS,      // 3 corpse slugs
    5,              // cultists
    6,              // fossil stalker
    7,              // gremlin merc
    8,              // haunted ship
    9,              // living fog
    10,             // punch construct
    11 | SEAPUNK,   // cultist + seapunk
    12,             // sewer clam
    13              // two tailed rats
};

static char* UNDERDOCKS[] = {
    "2 corpse slugs",
    "seapunk",
    "sludge spinner",
    "toadpoles",
    "3 corpse slugs",
    "cultists",
    "fossil stalker",
    "gremlin merc",
    "haunted ship",
    "living fog",
    "punch construct",
    "cultist + seapunk",
    "sewer clam",
    "two tailed rats"
};

static int HIVE_EASY[] = {
    0 | WORKERS,        // 2 bowlbugs
    1 | EXOSKELETONS,   // 3 exoskeletons
    2 | THIEVES,        // thieving hopper
    3 | BURROWER        // tunneler
};

static int HIVE_HARD[] = {
    4 | WORKERS,        // 3 bowlbugs
    5 | CHOMPER,        // 2 chompers
    6 | EXOSKELETONS,   // 4 exoskeletons
    7,                  // hunter killer
    8,                  // louse progenitor
    9,                  // mytes
    10,                 // ovicopter
    11 | WORKERS,       // slumbering beetle
    12,                 // spiny toad
    13                  // the obscura
};

static char* HIVE[] = {
    "2 bowlbugs",
    "3 exoskeletons",
    "thieving hopper",
    "tunneler",
    "3 bowlbugs",
    "2 chompers",
    "4 exoskeletons",
    "hunter killer",
    "louse progenitor",
    "mytes",
    "ovicopter",
    "slumbering beetle",
    "spiny toad",
    "the obscura"
};

static int GLORY_EASY[] = {
    0,              // devoted sculptor
    1 | SCROLLS,    // 3 scrolls
    2               // turret operator
};

static int GLORY_HARD[] = {
    3,              // axebots
    4,              // punch + cubex constructs
    5,              // fabricator
    6,              // frog knight
    7,              // globe head
    8,              // owl magistrate
    9 | SCROLLS,    // 4 scrolls
    10,             // slimed berserker
    11              // lost and forgotten
};

static char* GLORY[] = {
    "devoted sculptor",
    "3 scrolls",
    "turret operator",
    "axebots",
    "punch + cubex constructs",
    "fabricator",
    "frog knight",
    "globe head",
    "owl magistrate",
    "4 scrolls",
    "slimed berserker",
    "lost and forgotten"
};
