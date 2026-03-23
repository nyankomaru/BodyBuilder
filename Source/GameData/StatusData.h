#pragma once
#include "Data.h"
#include "GameDef.h"

///------------------------------------------------------------
/// @class StatusData
/// @brief プレイヤーの各種ステータス（筋肉・体力など）を管理するクラス
///------------------------------------------------------------
class StatusData : public Data {
public:
    //----------------------------------------------------------
    // 初期化
    //----------------------------------------------------------
    void Init() override {
        mEndurance = 10;
        mChest = 10;
        mBack = 10;
        mArm = 10;
        mLeg = 10;
        ResetDiff(); // 差分も初期化
    }

    void Save(class KeyValueFile& kv) override {}
    void Load(class KeyValueFile& kv) override {}

    //----------------------------------------------------------
    // Getter
    //----------------------------------------------------------
    int GetEndurance() const { return mEndurance; }
    int GetChest() const { return mChest; }
    int GetBack() const { return mBack; }
    int GetArm() const { return mArm; }
    int GetLeg() const { return mLeg; }

    //----------------------------------------------------------
    // Setter
    //----------------------------------------------------------
    void SetEndurance(const int v) { mEndurance = v; }
    void SetChest(const int v) { mChest = v; }
    void SetBack(const int v) { mBack = v; }
    void SetArm(const int v) { mArm = v; }
    void SetLeg(const int v) { mLeg = v; }

    //----------------------------------------------------------
    // ステータスの増加（または減少）
    //----------------------------------------------------------
    void IncreaseStatus(Status status, int increment)
    {
        switch (status)
        {
        case Status::Barbell:         // バーベル（胸）
            mChest += increment;
            mDiffChest += increment;
            break;
        case Status::Dumbbel:         // ダンベル（腕）
            mArm += increment;
            mDiffArm += increment;
            break;
        case Status::LatPulldown:     // ラットプルダウン（背中）
            mBack += increment;
            mDiffBack += increment;
            break;
        case Status::RunningMachine:  // ランニングマシン（持久力）
            mEndurance += increment;
            mDiffEndurance += increment;
            break;
        case Status::Squat:           // スクワット（脚）
            mLeg += increment;
            mDiffLeg += increment;
            break;
        default:
            break;
        }

        // 範囲制限（0〜999）
        Clamp(mChest);
        Clamp(mArm);
        Clamp(mBack);
        Clamp(mEndurance);
        Clamp(mLeg);
    }

    //----------------------------------------------------------
    // 上昇量（差分）を取得
    //----------------------------------------------------------
    int GetDiff(Status status) const
    {
        switch (status)
        {
        case Status::Barbell:         return mDiffChest;
        case Status::Dumbbel:         return mDiffArm;
        case Status::LatPulldown:     return mDiffBack;
        case Status::RunningMachine:  return mDiffEndurance;
        case Status::Squat:           return mDiffLeg;
        default:                      return 0;
        }
    }

    //----------------------------------------------------------
    // 上昇量（差分）をリセット（リザルト表示後に呼ぶ）
    //----------------------------------------------------------
    void ResetDiff()
    {
        mDiffEndurance = 0;
        mDiffChest = 0;
        mDiffBack = 0;
        mDiffArm = 0;
        mDiffLeg = 0;
    }

private:
    //----------------------------------------------------------
    // 範囲制限（0〜999）
    //----------------------------------------------------------
    void Clamp(int& v) const {
        if (v < 0) v = 0;
        else if (v > 999) v = 999;
    }

private:
    //----------------------------------------------------------
    // 現在値
    //----------------------------------------------------------
    int mEndurance = 10;
    int mChest = 10;
    int mBack = 10;
    int mArm = 10;
    int mLeg = 10;

    //----------------------------------------------------------
    // 上昇差分
    //----------------------------------------------------------
    int mDiffEndurance = 0;
    int mDiffChest = 0;
    int mDiffBack = 0;
    int mDiffArm = 0;
    int mDiffLeg = 0;
};