#pragma once
#include "pch.h"
class MiscConfig
{
    std::string ConfigName;

public:
    MiscConfig(const std::string& name)
    {
        ConfigName = name;
    }

    bool AdminESP = true;
    bool ChangeTime = false;
    int Time = 12;
    bool RemoveWaterEffect = false;
    bool AdminFlag = false;
    void ToJsonColour(json* j, const std::string& name, D2D1::ColorF* colour)
    {
        (*j)[ConfigName][name][LIT("r")] = colour->r;
        (*j)[ConfigName][name][LIT("g")] = colour->g;
        (*j)[ConfigName][name][LIT("b")] = colour->b;
        (*j)[ConfigName][name][LIT("a")] = colour->a;

    }
    void FromJsonColour(json j, const std::string& name, D2D1::ColorF* colour)
    {
        if (j[ConfigName].contains(name))
        {
            colour->r = j[ConfigName][name][LIT("r")];
            colour->g = j[ConfigName][name][LIT("g")];
            colour->b = j[ConfigName][name][LIT("b")];
            colour->a = j[ConfigName][name][LIT("a")];
        }
    }

    json ToJson()
    {
        json j;
        j[ConfigName][LIT("AdminESP")] = AdminESP;
        j[ConfigName][LIT("ChangeTime")] = ChangeTime;
        j[ConfigName][LIT("Time")] = Time;
        j[ConfigName][LIT("RemoveWaterEffect")] = RemoveWaterEffect;
        j[ConfigName][LIT("AdminFlag")] = AdminFlag;
     

        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("AdminESP")))
            AdminESP = j[ConfigName][LIT("AdminESP")];
        if (j[ConfigName].contains(LIT("ChangeTime")))
            ChangeTime = j[ConfigName][LIT("ChangeTime")];
        if (j[ConfigName].contains(LIT("Time")))
            Time = j[ConfigName][LIT("Time")];
        if (j[ConfigName].contains(LIT("RemoveWaterEffect")))
            RemoveWaterEffect = j[ConfigName][LIT("RemoveWaterEffect")];
        if (j[ConfigName].contains(LIT("AdminFlag")))
            AdminFlag = j[ConfigName][LIT("AdminFlag")];
    }
};

