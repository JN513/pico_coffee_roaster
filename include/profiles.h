#ifndef __PROFILES_H__
#define __PROFILES_H__

typedef struct {
    int time;
    int bt;
} ProfilePoint;

typedef enum {
    ROAST_LIGHT,
    ROAST_MEDIUM,
    ROAST_MEDIUM_DARK,
    ROAST_DARK
} RoastType;

typedef struct {
    const char *name;
    const ProfilePoint *points;
    int size;
    int total_time;
    int use_variable_fan;
    RoastType type;
} RoastProfile;



// =========================
// LIGHT
// =========================

static const ProfilePoint profile_light_fast[] = {
    {0,30},{90,150},{210,185},{300,200},{360,206}
};

static const ProfilePoint profile_light_slow[] = {
    {0,30},{100,150},{240,188},{360,202},{420,207}
};

static const ProfilePoint profile_light_extended_maillard[] = {
    {0,   30},
    {270, 150}, // Secagem padrão (4.5 min) [cite: 190]
    {720, 195}, // Maillard muito longo (+7.5 min) [cite: 192, 198]
    {840, 205} // 1º Crack e finalização rápida para não escurecer
};

static const ProfilePoint profile_light_long_slow_start[] = {
    {0,   30},
    {510, 150}, // Fase de Secagem estendida (8.5 min) [cite: 126]
    {750, 190}, // Maillard gradual (+4 min) [cite: 128]
    {870, 205} // Finalização logo após o 1º Crack para manter clara [cite: 585]
};



// =========================
// MEDIUM
// =========================

static const ProfilePoint profile_medium_short[] = { // 4
    {0,30},{100,150},{240,185},{360,200},{450,210}
};

static const ProfilePoint profile_medium_short2[] = {
    {0,30},{120,150},{300,190},{420,205},{540,210}
};

static const ProfilePoint profile_medium_long1[] = {
    {0,30},{180,150},{420,190},{600,205},{720,210}
};

static const ProfilePoint profile_medium_long2[] = {
    {0,30},{180,150},{420,195},{600,215},{780,222}
};

static const ProfilePoint profile_medium_balanced[] = { // 8
    {0,30},{150,150},{360,190},{540,205},{660,212}
};



// =========================
// MEDIUM DARK
// =========================

static const ProfilePoint profile_medium_dark1[] = {
    {0,30},{110,150},{260,188},{390,202},{510,212}
};

static const ProfilePoint profile_medium_dark2[] = { // 10
    {0,30},{150,150},{330,188},{510,205},{690,215}
};

static const ProfilePoint profile_medium_dark_long[] = { // 11
    {0,30},{180,150},{420,200},{600,225},{780,232}
};



// =========================
// DARK
// =========================

static const ProfilePoint profile_dark1[] = {
    {0,30},{150,150},{330,190},{510,205},{720,218}
};



// =========================
// SWEET PROFILE
// =========================

static const ProfilePoint profile_sweet[] = {
    {0,30},{90,150},{220,182},{340,198},{430,209}
};

// =========================
// Custom profiles
// =========================

static const ProfilePoint profile_medium_espresso_slow_start[] = {
    {0,   30},    // Início à temperatura ambiente
    {510, 150},  // Fim da Fase de Secagem (8.5 min): Baixo RoR inicial típico do Slow Start [cite: 126, 127]
    {750, 196},  // Milestone de Alteração de Cor para 1º Crack (4 min): Maillard prolongada para complexidade [cite: 128, 844]
    {870, 215},  // Torra Média (Development): 2 min após o 1º Crack para corpo e doçura [cite: 745, 850]
    {930, 225}   // Finalização: Antes do 2º Crack (~16 min) para evitar amargor excessivo [cite: 78, 132]
};

static const ProfilePoint profile_chocolate_notes_extended_maillard[] = {
    {0,   30},    
    {270, 150},  // Fim da Secagem (4.5 min): Fase inicial mais rápida que o SS [cite: 188]
    {720, 196},  // Fase Maillard Estendida (+7.5 min): Crucial para notas de chocolate 
    {810, 212},  // Desenvolvimento Pós-1º Crack: 1.5 min para doçura de chocolate ao leite [cite: 192]
    {840, 222}   // Finalização: Próximo ao 2º Crack para chocolate amargo e corpo denso 
};

static const ProfilePoint profile_fast_start_acidity[] = {
    {0,   30},
    {300, 150}, // Fase de Secagem rápida (5 min) [cite: 115]
    {510, 196}, // Maillard rápida (3.5 min) [cite: 117]
    {720, 210}, // Desenvolvimento (3.5 min até o 1º Crack) [cite: 119]
    {960, 237}  // Finalização: Torra clara/média em 16 min [cite: 78, 121]
};

static const ProfilePoint profile_production_balanced[] = {
    {0,   30},
    {300, 150}, // Fase de Secagem estável (5 min) [cite: 151]
    {570, 196}, // Maillard controlada (4.5 min) [cite: 153]
    {840, 212}, // Desenvolvimento equilibrado (4.5 min) [cite: 155]
    {960, 237}  // Finalização padrão [cite: 78, 157]
};


static const ProfilePoint profile_medium_sweetness[] = {
    {0,   30},
    {330, 150}, // Secagem intermediária (5.5 min) [cite: 138]
    {570, 196}, // Maillard (4 min) [cite: 140]
    {750, 212}, // Desenvolvimento focado em açúcares (3 min) [cite: 142]
    {960, 237}  // Finalização equilibrada [cite: 144]
};

static const ProfilePoint profile_medium_espresso_2[] = {
    {0,   30},
    {300, 150}, // 300s (5 min): Secagem mais rápida [cite: 115]
    {540, 196}, // Primeiro Crack ocorre aos 9 min (300s + 240s) [cite: 118]
    {720, 215}, // Dá mais 3 min de desenvolvimento para o Espresso
    {840, 225}  // Finalização total em 14 min
};

static const ProfilePoint profile_medium_short_long[] = {
    {0,30},{180,150},{360,195},{550,207},{710,217}
};


// =========================
// PROFILE LIST (ordered)
// =========================

static const RoastProfile profiles[] = {

    // LIGHT

    {
        "Light Fast",
        profile_light_fast,
        sizeof(profile_light_fast)/sizeof(ProfilePoint),
        360,
        0,
        ROAST_LIGHT
    },

    {
        "Light Slow",
        profile_light_slow,
        sizeof(profile_light_slow)/sizeof(ProfilePoint),
        420,
        0,
        ROAST_LIGHT
    },

    {
        "Light Extended Maillard",
        profile_light_extended_maillard,
        sizeof(profile_light_extended_maillard)/sizeof(ProfilePoint),
        840,
        0,
        ROAST_LIGHT
    },
    {
        "Light long Slow Start",
        profile_light_long_slow_start,
        sizeof(profile_light_long_slow_start)/sizeof(ProfilePoint),
        870,
        0,
        ROAST_LIGHT
    },


    // MEDIUM

    {
        "Medium Short",
        profile_medium_short,
        sizeof(profile_medium_short)/sizeof(ProfilePoint),
        450,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Short 2",
        profile_medium_short2,
        sizeof(profile_medium_short2)/sizeof(ProfilePoint),
        540,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Long 1",
        profile_medium_long1,
        sizeof(profile_medium_long1)/sizeof(ProfilePoint),
        720,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Long 2",
        profile_medium_long2,
        sizeof(profile_medium_long2)/sizeof(ProfilePoint),
        780,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Balanced",
        profile_medium_balanced,
        sizeof(profile_medium_balanced)/sizeof(ProfilePoint),
        660,
        0,
        ROAST_MEDIUM
    },


    // MEDIUM DARK

    {
        "Medium Dark",
        profile_medium_dark1,
        sizeof(profile_medium_dark1)/sizeof(ProfilePoint),
        510,
        0,
        ROAST_MEDIUM_DARK
    },

    {
        "Medium Dark 2",
        profile_medium_dark2,
        sizeof(profile_medium_dark2)/sizeof(ProfilePoint),
        690,
        0,
        ROAST_MEDIUM_DARK
    },

    {
        "Medium Dark Long", // 11
        profile_medium_dark_long,
        sizeof(profile_medium_dark_long)/sizeof(ProfilePoint),
        780,
        0,
        ROAST_MEDIUM_DARK
    },


    // DARK

    {
        "Dark", // 12
        profile_dark1,
        sizeof(profile_dark1)/sizeof(ProfilePoint),
        720,
        0,
        ROAST_DARK
    },


    // SWEET

    { // 13
        "Sweet",
        profile_sweet,
        sizeof(profile_sweet)/sizeof(ProfilePoint),
        430,
        0,
        ROAST_MEDIUM
    },

    { // 14
        "Medium expresso",
        profile_medium_espresso_slow_start,
        sizeof(profile_medium_espresso_slow_start)/sizeof(ProfilePoint),
        930,
        0,
        ROAST_MEDIUM
    },

    {
        "Chocolate Notes",
        profile_chocolate_notes_extended_maillard,
        sizeof(profile_chocolate_notes_extended_maillard)/sizeof(ProfilePoint),
        840,
        0,
        ROAST_MEDIUM
    },

    {
        "Fast Start fluit",
        profile_fast_start_acidity,
        sizeof(profile_fast_start_acidity)/sizeof(ProfilePoint),
        960,
        0,
        ROAST_MEDIUM
    },

    {
        "Balanced",
        profile_production_balanced,
        sizeof(profile_production_balanced)/sizeof(ProfilePoint),
        960,
        0,
        ROAST_MEDIUM
    },

    {
        "Sweetness",
        profile_medium_sweetness,
        sizeof(profile_medium_sweetness)/sizeof(ProfilePoint),
        960,
        0,
        ROAST_MEDIUM
    },


    {
        "Expresso 2",
        profile_medium_espresso_2,
        sizeof(profile_medium_espresso_2)/sizeof(ProfilePoint),
        840,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Short Long",
        profile_medium_short_long,
        sizeof(profile_medium_short_long)/sizeof(ProfilePoint),
        710,
        0,
        ROAST_MEDIUM
    },

    {
        "Medium Long 2 airflow",
        profile_medium_long2,
        sizeof(profile_medium_long2)/sizeof(ProfilePoint),
        780,
        1,
        ROAST_MEDIUM
    },

    {
        "Medium Balanced airflow",
        profile_medium_balanced,
        sizeof(profile_medium_balanced)/sizeof(ProfilePoint),
        660,
        1,
        ROAST_MEDIUM
    },

    {
        "Medium Short Long airflow",
        profile_medium_short_long,
        sizeof(profile_medium_short_long)/sizeof(ProfilePoint),
        710,
        1,
        ROAST_MEDIUM
    },
};

#define PROFILE_COUNT (sizeof(profiles)/sizeof(RoastProfile))

/**
 * @brief Get the profile finish time object
 * 
 * @param profile_id 
 * @return int 
 */
int get_profile_finish_time(int profile_id);

/**
 * @brief Get the profile name object
 * 
 * @param profile_id 
 * @return char* 
 */
char * get_profile_name(int profile_id);

/**
 * @brief Get the profile pointer object
 * 
 * @param profile_id 
 * @return ProfilePoint* 
 */
ProfilePoint * get_profile_pointer(int profile_id);

/**
 * @brief Get the profile size object
 * 
 * @param profile_id 
 * @return int 
 */
int get_profile_size(int profile_id);

/**
 * @brief Get the profile use variable fan object
 * 
 * @param profile_id 
 * @return int 
 */
int get_profile_use_variable_fan(int profile_id);

/**
 * @brief Get the profile type object
 * 
 * @param profile_id 
 * @return RoastType 
 */
RoastType get_profile_type(int profile_id);

/**
 * @brief 
 * 
 * @param stage 
 */
void print_stage(int stage);

#endif // !__PROFILES_H__