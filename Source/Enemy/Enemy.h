#pragma once
#include <string>
#include <vector>

// 敵のステータス管理用構造体
struct EnemyStatus {
    int mChest;     // 胸のステータス
    int mBack;      // 背中のステータス
    int mArm;       // 腕のステータス
    int mLeg;       // 脚のステータス

    EnemyStatus() : mChest(0), mBack(0), mArm(0), mLeg(0) {}
};

// ミニゲーム用タイムパターン管理クラス
class Enemy
{
public:
    bool LoadEnemyStatuses(const char* filename, std::vector<EnemyStatus>& enemies);
};