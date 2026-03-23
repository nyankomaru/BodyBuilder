#include "Timer.h"
#include "Fwk/Framework.h"
#include <algorithm> // std::clamp（必要なら）
#include <cmath>

//-----------------------------------------------------------------------------
// Timer::Init
// タイマー描画用のスプライトとテクスチャを初期化
//-----------------------------------------------------------------------------
void Timer::Init()
{
    // 背景（Time.png）読み込み
    mTimeTex.Load("Images/Time.png");
    mTimeSprite.Init();
    mTimeSprite.SetTexture(mTimeTex);
    mTimeSprite.SetSize(512.0f, 98.0f); // 幅・高さ
    mTimeSprite.SetPosition(mPosition.x - 120.0f, mPosition.y); // 背景描画位置
    mTimeSprite.SetVisible(true);

    // 数字アトラス読み込み（0~9 の数字が横に並んでいる前提）
    mNumberTex.Load("Images/numbers.png");

    const float digitW = 64.0f; // 数字1桁の幅
    const float digitH = 64.0f; // 数字1桁の高さ

    // 左右2桁分のスプライト初期化
    for (int i = 0; i < 2; ++i) {
        mNumberSprite[i].Init();
        mNumberSprite[i].SetTexture(mNumberTex);
        mNumberSprite[i].SetSize(digitW, digitH);
        mNumberSprite[i].SetVisible(true);
    }

    // 初期位置設定
    mNumberSprite[0].SetPosition({ mPosition.x , mPosition.y });  // 右桁
    mNumberSprite[1].SetPosition({ mPosition.x - 36.0f, mPosition.y });  // 左桁
}

//-----------------------------------------------------------------------------
// clamp_int
// 小さなユーティリティ関数：値を指定範囲に制限
// std::clamp が使えない環境向け
//-----------------------------------------------------------------------------
static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

//-----------------------------------------------------------------------------
// Timer::UpdateRemaining
// 残り秒数を受け取り、スプライトに数字を反映する
//-----------------------------------------------------------------------------
void Timer::UpdateRemaining(float remainingSeconds)
{
    // 小数点以下切り上げして整数秒に変換
    int seconds = static_cast<int>(std::ceil(remainingSeconds));
    // 0～60 秒に制限
    seconds = clamp_int(seconds, 0, 60);

    // 秒を 10 の位と 1 の位に分解
    Number2 n = SplitSeconds(seconds);

    // スプライトに数字を反映
    SetDigit(mNumberSprite[0], n.ones); // 右桁
    SetDigit(mNumberSprite[1], n.tens); // 左桁

    // スプライト更新
    for (int i = 0; i < 2; ++i) mNumberSprite[i].Update();
}

//-----------------------------------------------------------------------------
// Timer::Render
// タイマー背景と数字を描画
//-----------------------------------------------------------------------------
void Timer::Render()
{
    mTimeSprite.Draw();        // 背景描画
    mNumberSprite[1].Draw();   // 十の位（左桁）
    mNumberSprite[0].Draw();   // 一の位（右桁）
}

//-----------------------------------------------------------------------------
// Timer::Term
// スプライトとテクスチャを解放
//-----------------------------------------------------------------------------
void Timer::Term()
{
    mTimeSprite.Term();
    mTimeTex.Unload();

    for (int i = 0; i < 2; ++i) {
        mNumberSprite[i].Term();
    }
    mNumberTex.Unload();
}

//-----------------------------------------------------------------------------
// Timer::SetPosition
// タイマーの描画位置を変更
//-----------------------------------------------------------------------------
void Timer::SetPosition(const Vector2f& pos)
{
    mPosition = pos;
    mTimeSprite.SetPosition(mPosition.x - 120.0f, mPosition.y);
    mNumberSprite[0].SetPosition({ mPosition.x - 36.f, mPosition.y });  // 右桁
    mNumberSprite[1].SetPosition({ mPosition.x - 36.0f, mPosition.y });  // 左桁
}

//-----------------------------------------------------------------------------
// Timer::SplitSeconds
// 秒数を 10 の位と 1 の位に分解
//-----------------------------------------------------------------------------
Timer::Number2 Timer::SplitSeconds(int seconds) const
{
    Number2 n;
    n.tens = (seconds / 10) % 10;  // 10の位
    n.ones = seconds % 10;          // 1の位
    return n;
}

//-----------------------------------------------------------------------------
// Timer::SetDigit
// 指定したスプライトに数字を設定
// 数字テクスチャは横に 0~9 が並んでいる想定
//-----------------------------------------------------------------------------
void Timer::SetDigit(Sprite& sprite, int digit)
{
    // 安全対策：0～9 に制限
    if (digit < 0) digit = 0;
    if (digit > 9) digit = 9;

    // UV 設定：横方向 10 分割のアトラス
    const float u = 0.1f * static_cast<float>(digit); // 左端
    const float v = 0.0f;                              // 上端
    const float w = 0.1f;                              // 幅
    const float h = 1.0f;                              // 高さ

    sprite.SetTexCoord(u, v, w, h);
}
