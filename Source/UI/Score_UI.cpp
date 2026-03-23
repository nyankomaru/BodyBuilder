#include "Score_UI.h"
#include "Fwk/Framework.h"

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// テクスチャの読み込み、スプライトの初期化、位置・サイズ・優先度設定
// スプライトは初期状態では非表示にする
//-----------------------------------------------------------------------------
void Score_UI::Init()
{
    // 判定バー（Bad）
    mBarTex.Load("Images/Bad_s.png");
    mBarSprite.Init();
    mBarSprite.SetTexture(mBarTex);
    mBarSprite.SetPosition(mX, mY);
    mBarSprite.SetSize(width, height);
    mBarSprite.SetVisible(false);
    mBarSprite.SetPriority(1000);

    // Good 判定用スプライト
    mGoodTex.Load("Images/Good_s.png");
    mGoodSprite.Init();
    mGoodSprite.SetTexture(mGoodTex);
    mGoodSprite.SetPosition(mX, mY);
    mGoodSprite.SetSize(width, height);
    mGoodSprite.SetVisible(false);
    mGoodSprite.SetPriority(1000);

    // Normal 判定用スプライト
    mNormalTex.Load("Images/Normal_s.png");
    mNormalSprite.Init();
    mNormalSprite.SetTexture(mNormalTex);
    mNormalSprite.SetPosition(mX, mY);
    mNormalSprite.SetSize(width, height);
    mNormalSprite.SetVisible(false);
    mNormalSprite.SetPriority(1000);
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
// mDisplayTimer を減算し、タイマー切れでスプライトを非表示にする
//-----------------------------------------------------------------------------
void Score_UI::Update()
{
    if (!mIsVisible) return; // 表示中でなければ何もしない

    // 経過時間を引く
    mDisplayTimer -= Time_I->GetDeltaTime();

    // タイマーが切れたら非表示にする
    if (mDisplayTimer <= 0.f) {
        mIsVisible = false;

        mBarSprite.SetVisible(false);
        mGoodSprite.SetVisible(false);
        mNormalSprite.SetVisible(false);
    }

    // スプライトの内部更新
    mBarSprite.Update();
    mGoodSprite.Update();
    mNormalSprite.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
// スプライトを描画する
//-----------------------------------------------------------------------------
void Score_UI::Render()
{
    mBarSprite.Draw();
    mNormalSprite.Draw();
    mGoodSprite.Draw();
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
// スプライトとテクスチャの解放
//-----------------------------------------------------------------------------
void Score_UI::Term()
{
    mBarTex.Unload();    mBarSprite.Term();
    mGoodTex.Unload();   mGoodSprite.Term();
    mNormalTex.Unload(); mNormalSprite.Term();
}

//-----------------------------------------------------------------------------
// スコア設定
//-----------------------------------------------------------------------------
// Score を設定して表示開始
// Bad / Good / Normal に応じたスプライトのみ表示
// 表示時間は 0.5 秒
//-----------------------------------------------------------------------------
void Score_UI::SetScore(Score score)
{
    mCurrentScore = score;
    mIsVisible = true;
    mDisplayTimer = 0.5f;  // 表示時間 0.5 秒

    // すべて非表示に
    mBarSprite.SetVisible(false);
    mGoodSprite.SetVisible(false);
    mNormalSprite.SetVisible(false);

    // 該当スコアのみ表示
    switch (score) {
    case Score::Bad:
        mBarSprite.SetVisible(true);
        break;
    case Score::Good:
        mGoodSprite.SetVisible(true);
        break;
    case Score::Normal:
        mNormalSprite.SetVisible(true);
        break;
    default:
        break;
    }
}
