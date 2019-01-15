#ifndef PARAMETER_CONFIG_H
#define PARAMETER_CONFIG_H

typedef struct{
    int Minute;
    int Second;
    int dayGoal;
    int monthGoal;
    int yearGoal;
}TomatoTiming;


#define Files_Path				"..//PomodroidoLite/configSetting.ini"
#define CONFIG_TOMATO_TIME   "/Setting/TomatoTime"
#define CONFIG_TOMATO_DAY_NUM   "/Setting/TomatoDayNum"
#define CONFIG_TOMATO_MONTH_NUM   "/Setting/TomatoMonthNum"
#define CONFIG_TOMATO_YEAR_NUM   "/Setting/TomatoYearNum"

#define TimerStatusBar    0
#define TimerBeginTomato  1

#define TimerOpen   0
#define TimerClose  1

#endif // PARAMETER_CONFIG_H
