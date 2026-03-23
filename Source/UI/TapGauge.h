#pragma once
#include "Fwk/Framework.h"
#include "Time/TimeMng.h"
#include "Fwk/Graphics.h"

//-----------------------------------------------------------------------------
// TapGauge
// ミニゲーム用のゲージ表示クラス
// タイマーに応じてカーソルが左右に移動し、判定ライン（Good/Normal）を表示
//-----------------------------------------------------------------------------
class TapGauge
{
public:
    // 初期化
    // テクスチャ読み込み、スプライト初期化、位置設定などを行う
    void Init();

    // 終了処理
    // スプライトとテクスチャを解放する
    void Term();

    // 更新
    // TimeMng から現在のタイマー情報を受け取り、カーソル位置を更新
    void Update(const TimeMng& timeMng);

    // 描画
    // バー・カーソル・判定ラインを描画
    void Render();

    // ゲージの移動方向を設定
    // dir = 1 : 左→右
    // dir = -1 : 右→左
    void SetDirection(int dir);

    // ゲージを初期位置に戻す
    // 方向が反転した場合などに呼び出す
    void ResetPosition();

private:
    float mX = 0.f;          // ゲージの中心X座標
    float mY = -300.f;       // ゲージのY座標（画面基準）
    float mWidth = 512.f;    // ゲージの幅
    float mHeight = 64.f;    // ゲージの高さ

    int mDirection = 1;      // 移動方向（1=左→右, -1=右→左）

    // ゲージ描画用テクスチャ
    Texture mBarTex;         // バー全体
    Texture mCursorTex;      // 移動カーソル
    Texture mGoodTex;        // Good 判定ライン
    Texture mNormalTex;      // Normal 判定ライン

    // スプライト
    Sprite mBarSprite;
    Sprite mCursorSprite;
    Sprite mGoodSprite;
    Sprite mNormalSprite;
};
