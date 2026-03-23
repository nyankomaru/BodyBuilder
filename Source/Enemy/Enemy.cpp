#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include"Enemy.h"

// EnemyStatus をファイルから読み込む関数
// 入力テキスト例:
// Enemy0: mChest=345, mBack=291, mArm=424, mLeg=378
// Enemy1: mChest=412, mBack=208, mArm=350, mLeg=439
bool Enemy::LoadEnemyStatuses(const char* filename, std::vector<EnemyStatus>& enemies)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open enemy status file: " << filename << std::endl;
        return false;
    }

    enemies.clear();

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        // Enemy番号と後半を分割
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string enemyName = line.substr(0, colonPos);
        std::string stats = line.substr(colonPos + 1);

        EnemyStatus es;

        // ステータス部分のパース
        // 例: " mChest=345, mBack=291, mArm=424, mLeg=378"
        std::istringstream statStream(stats);
        std::string token;

        while (std::getline(statStream, token, ','))
        {
            // 空白除去
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

            size_t eqPos = token.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key = token.substr(0, eqPos);
            std::string valueStr = token.substr(eqPos + 1);
            int value = std::stoi(valueStr);

            if (key == "mChest") es.mChest = value;
            else if (key == "mBack") es.mBack = value;
            else if (key == "mArm") es.mArm = value;
            else if (key == "mLeg") es.mLeg = value;
        }

        enemies.push_back(es);
    }

    return true;
}