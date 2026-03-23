#include "TapGauge.h"
#include "Fwk/Framework.h"

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// ゲージ、カーソル、判定ライン（Good/Normal）のスプライト・テクスチャを読み込み
void TapGauge::Init()
{
    // ゲージ本体（背景）テクスチャとスプライト
    mBarTex.Load("Images/Bad.png");        // バーの背景画像
    mBarSprite.Init();
    mBarSprite.SetTexture(mBarTex);
    mBarSprite.SetSize(mWidth, mHeight);
    mBarSprite.SetPosition(mX, mY);
    mBarSprite.SetVisible(true);

    // カーソルスプライト
    mCursorTex.Load("Images/Cursor.png");  // カーソル画像
    mCursorSprite.Init();
    mCursorSprite.SetTexture(mCursorTex);
    mCursorSprite.SetSize(128, 64);
    mCursorSprite.SetVisible(true);
    mCursorSprite.SetPriority(100);

    // Good 判定ライン
    mGoodTex.Load("Images/Good.png");
    mGoodSprite.Init();
    mGoodSprite.SetTexture(mGoodTex);
    mGoodSprite.SetSize(mWidth, mHeight);
    mGoodSprite.SetPosition(mX, mY);
    mGoodSprite.SetVisible(true);

    // Normal 判定ライン
    mNormalTex.Load("Images/Normal.png");
    mNormalSprite.Init();
    mNormalSprite.SetTexture(mNormalTex);
    mNormalSprite.SetSize(128, mHeight);
    mNormalSprite.SetPosition(mWidth, mY);
    mNormalSprite.SetVisible(true);
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
// TimeMng のタイマー情報をもとにカーソル位置、Good/Normal範囲を更新
void TapGauge::Update(const TimeMng& timeMng)
{
    const TimeData& data = timeMng.GetCurrent();  // 現在パターン取得
    float maxTime = data.maxTime;                 // 最大時間
    float timer = timeMng.GetTimer();            // 現在タイマー
    if (maxTime <= 0.0f) return;                 // 無効パターンなら終了

    // 時間をゲージ上のX座標に変換するラムダ
    auto toX = [&](float time) {
        float ratio = time / maxTime;
        // 方向によって変換
        if (mDirection == 1)  // 左→右
            return mX - mWidth / 2 + ratio * mWidth;
        else                  // 右→左
            return mX + mWidth / 2 - ratio * mWidth;
        };

    // Normal 判定ラインの計算・設定
    float normalStartX = toX(data.normalStart);
    float normalEndX = toX(data.normalEnd);
    float normalWidth = fabs(normalEndX - normalStartX);
    mNormalSprite.SetSize(normalWidth, mHeight);
    mNormalSprite.SetPosition((normalStartX + normalEndX) / 2, mY);

    // Good 判定ラインの計算・設定
    float goodStartX = toX(data.goodStart);
    float goodEndX = toX(data.goodEnd);
    float goodWidth = fabs(goodEndX - goodStartX);
    mGoodSprite.SetSize(goodWidth, mHeight);
    mGoodSprite.SetPosition((goodStartX + goodEndX) / 2, mY);

    // カーソル位置を設定（右桁オフセット）
    float cursorX = toX(timer);
    mCursorSprite.SetPosition(cursorX + 64, mY + 64);
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void TapGauge::Render()
{
    mBarSprite.Draw();     // 背景バー
    mNormalSprite.Draw();  // Normal 判定ライン
    mGoodSprite.Draw();    // Good 判定ライン
    mCursorSprite.Draw();  // カーソル
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void TapGauge::Term()
{
    // テクスチャ解放とスプライト終了
    mBarTex.Unload();    mBarSprite.Term();
    mCursorTex.Unload(); mCursorSprite.Term();
    mGoodTex.Unload();   mGoodSprite.Term();
    mNormalTex.Unload(); mNormalSprite.Term();
}

//-----------------------------------------------------------------------------
// ゲージの移動方向を外部から設定
// dir = 1 → 左→右
// dir = -1 → 右→左
//-----------------------------------------------------------------------------
void TapGauge::SetDirection(int dir)
{
    mDirection = (dir >= 0) ? 1 : -1;
}

//-----------------------------------------------------------------------------
// 方向を変更した後、カーソルを初期位置にリセット
//-----------------------------------------------------------------------------
void TapGauge::ResetPosition()
{
    if (mDirection == 1)
        mCursorSprite.SetPosition(mX - mWidth / 2, mY); // 左端スタート
    else
        mCursorSprite.SetPosition(mX + mWidth / 2, mY); // 右端スタート
}
