#pragma once
#include <vector>

class EnemyHealthManager {
public:
    void setHealths(const std::vector<int>& pea, const std::vector<int>& peb, const std::vector<int>& pec) {
        peaHP = pea;
        pebHP = peb;
        pecHP = pec;
    }
    std::vector<int>& getPeaHP() { return peaHP; }
    std::vector<int>& getPebHP() { return pebHP; }
    std::vector<int>& getPecHP() { return pecHP; }
    void damagePea(int idx, int dmg) { if (idx >= 0 && idx < peaHP.size()) peaHP[idx] -= dmg; }
    void damagePeb(int idx, int dmg) { if (idx >= 0 && idx < pebHP.size()) pebHP[idx] -= dmg; }
    void damagePec(int idx, int dmg) { if (idx >= 0 && idx < pecHP.size()) pecHP[idx] -= dmg; }
private:
    std::vector<int> peaHP;
    std::vector<int> pebHP;
    std::vector<int> pecHP;
};
