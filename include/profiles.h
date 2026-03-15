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



// =========================
// MEDIUM
// =========================

static const ProfilePoint profile_medium_short[] = {
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

static const ProfilePoint profile_medium_balanced[] = {
    {0,30},{150,150},{360,190},{540,205},{660,212}
};



// =========================
// MEDIUM DARK
// =========================

static const ProfilePoint profile_medium_dark1[] = {
    {0,30},{110,150},{260,188},{390,202},{510,212}
};

static const ProfilePoint profile_medium_dark2[] = {
    {0,30},{150,150},{330,188},{510,205},{690,215}
};

static const ProfilePoint profile_medium_dark_long[] = {
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
// PROFILE LIST (ordered)
// =========================

static const RoastProfile profiles[] = {

    // LIGHT

    {
        "Light Fast",
        profile_light_fast,
        sizeof(profile_light_fast)/sizeof(ProfilePoint),
        360,
        ROAST_LIGHT
    },

    {
        "Light Slow",
        profile_light_slow,
        sizeof(profile_light_slow)/sizeof(ProfilePoint),
        420,
        ROAST_LIGHT
    },


    // MEDIUM

    {
        "Medium Short",
        profile_medium_short,
        sizeof(profile_medium_short)/sizeof(ProfilePoint),
        450,
        ROAST_MEDIUM
    },

    {
        "Medium Short 2",
        profile_medium_short2,
        sizeof(profile_medium_short2)/sizeof(ProfilePoint),
        540,
        ROAST_MEDIUM
    },

    {
        "Medium Long 1",
        profile_medium_long1,
        sizeof(profile_medium_long1)/sizeof(ProfilePoint),
        720,
        ROAST_MEDIUM
    },

    {
        "Medium Long 2",
        profile_medium_long2,
        sizeof(profile_medium_long2)/sizeof(ProfilePoint),
        780,
        ROAST_MEDIUM
    },

    {
        "Medium Balanced",
        profile_medium_balanced,
        sizeof(profile_medium_balanced)/sizeof(ProfilePoint),
        660,
        ROAST_MEDIUM
    },


    // MEDIUM DARK

    {
        "Medium Dark",
        profile_medium_dark1,
        sizeof(profile_medium_dark1)/sizeof(ProfilePoint),
        510,
        ROAST_MEDIUM_DARK
    },

    {
        "Medium Dark 2",
        profile_medium_dark2,
        sizeof(profile_medium_dark2)/sizeof(ProfilePoint),
        690,
        ROAST_MEDIUM_DARK
    },

    {
        "Medium Dark Long",
        profile_medium_dark_long,
        sizeof(profile_medium_dark_long)/sizeof(ProfilePoint),
        780,
        ROAST_MEDIUM_DARK
    },


    // DARK

    {
        "Dark",
        profile_dark1,
        sizeof(profile_dark1)/sizeof(ProfilePoint),
        720,
        ROAST_DARK
    },


    // SWEET

    {
        "Sweet",
        profile_sweet,
        sizeof(profile_sweet)/sizeof(ProfilePoint),
        430,
        ROAST_MEDIUM
    }

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