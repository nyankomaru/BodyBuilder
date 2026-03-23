#pragma once
#include "Data.h"

class TournamentData : public Data {
public:
    void Init() override {
        mRank = 0;
        mScore = 0;
    }

    void Save(class KeyValueFile& kv) override {}
    void Load(class KeyValueFile& kv) override {}

    int GetRank() const { return mRank; }
    int GetScore() const { return mScore; }

    void SetRank(int v) { mRank = v; }
    void SetScore(int v) { mScore = v; }

private:
    int mRank = 0;
    int mScore = 0;
};