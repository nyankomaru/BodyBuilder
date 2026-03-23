#pragma once
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
#include "Time/TimeMng.h"
#include <cmath>

// forward 宣言（必要に応じて #include "TimeMng.h" に置き換え可能）
class TimeMng;

//-----------------------------------------------------------------------------
// Timer クラス
// ミニゲーム用の残り時間表示（秒単位）を管理するクラス
//----------------------------------------------------------------------------- 
class Timer
{
public:
    // 初期化
    // テクスチャの読み込み、スプライト初期化などを行う
    void Init();

    // 残り時間を更新
    // MiniGame から呼び出され、タイマーを表示する
    void UpdateRemaining(float remainingSeconds);

    // 描画
    // 現在の残り時間を画面に描画する
    void Render();

    // 終了処理
    // テクスチャ解放やスプライト終了処理を行う
    void Term();

    // タイマー位置設定
    // 描画位置を変更できる
    void SetPosition(const Vector2f& pos);

private:
    // 秒数を 10 の位と 1 の位に分解する
    struct Number2 {
        int tens;   // 10 の位
        int ones;   // 1 の位
    };

    // 秒数を Number2 構造体に分解
    Number2 SplitSeconds(int seconds) const;

    // 指定したスプライトに数字を設定する
    void SetDigit(Sprite& sprite, int digit);

private:
    Texture mTimeTex;          // 「:」などのタイマー区切り用テクスチャ
    Sprite  mTimeSprite;       // タイマー描画用スプライト

    Texture mNumberTex;        // 数字用テクスチャ
    Sprite  mNumberSprite[2];  // 2桁分の数字スプライト

    Vector2f mPosition = { 580.0f, 300.0f }; // デフォルト描画位置
};
