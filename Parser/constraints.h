#define SSR_SCORE 2
#define PAX_SCORE 1
#define MAXIMUM_ALLOWED_TIME_DIFF Time(144, 0)
#define ARRIVAL_DELAY_LT_6_SCORE 7
#define ARRIVAL_DELAY_LT_12_SCORE 5
#define ARRIVAL_DELAY_LT_24_SCORE 4
#define ARRIVAL_DELAY_LT_48_SCORE 3
#define EQUIPMENT_SCORE 5
#define CITYPAIRS_SCORE 0
#define DEPARTURE_DELAY_LT_6_SCORE 7
#define DEPARTURE_DELAY_LT_12_SCORE 5
#define DEPARTURE_DELAY_LT_24_SCORE 4
#define DEPARTURE_DELAY_LT_48_SCORE 3
#define CLASS_UPGRADE_ALLOWED  true
#define CLASS_DOWNGRADE_ALLOWED  true



map<ClassCDs,int> classScoresMap = {{FC,25},{BC,20},{PC,15},{EC,10}};