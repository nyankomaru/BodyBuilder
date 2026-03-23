#include "Player/Player.h"
#include "Fwk/Framework.h"
#include "GameObjectMng/GameObjectMng.h"

//============================================================
// ■ Player 実装
//   プレイヤーキャラクターの初期化・更新・描画・終了処理
//   各ステータス別のアニメーション更新
//   スコアに応じたステータス上昇を提供
//============================================================

//------------------------------------------------------------
// ■ 初期化
//   テクスチャ読み込み、スプライト設定、アニメーション初期化
//------------------------------------------------------------
void Player::Init()
{
    // テクスチャ読み込み
    mTexture.Load("Images/Player.png");

    // スプライト初期化
    mSprite.Init();
    mSprite.SetTexture(mTexture);
    mSprite.SetSize(608.0f, 711.0f);   // 表示サイズ
    mSprite.SetPosition(0, 100);       // 初期位置
    mSprite.SetVisible(true);

    // アニメーション初期化
    _initAnimation();
}

//------------------------------------------------------------
// ■ 更新
//   ステータスに応じて更新処理を切り替える
//   スプライトも毎フレーム更新
//------------------------------------------------------------
void Player::Update()
{
    switch (mStatus)
    {
    case Status::Barbell:
        _updateBarbell();
        break;
    case Status::Dumbbel:
        _updateDumbbel();
        break;
    case Status::LatPulldown:
        _updateLatPulldown();
        break;
    case Status::RunningMachine:
        _updateRunningMachine();
        break;
    case Status::Squat:
        _updateSquat();
        break;
    }

    // スプライトアニメーション更新
    mSprite.Update();
}

//------------------------------------------------------------
// ■ 描画
//   スプライトを画面に描画
//------------------------------------------------------------
void Player::Render()
{
    mSprite.Draw();
}

//------------------------------------------------------------
// ■ 終了
//   スプライト終了処理、テクスチャ解放
//------------------------------------------------------------
void Player::Term()
{
    mSprite.Term();
    mTexture.Unload();
}

//------------------------------------------------------------
// ■ アニメーション初期化
//   各ステータス別のアニメーションデータを作成し
//   スプライトに登録
//------------------------------------------------------------
void Player::_initAnimation()
{
    // UV幅と高さ（スプライトシート分割）
    float uvW = 1.f / 4.f;
    float uvH = 1.f / 5.f;

    // スプライトセル構造体リスト
    UVRect uvs[] = {
        {uvW * 0.0f,uvH * 0.0f,uvW,uvH}, // スクワット1
        {uvW * 1.0f,uvH * 0.0f,uvW,uvH}, // スクワット2
        {uvW * 2.0f,uvH * 0.0f,uvW,uvH}, // スクワット3
        {uvW * 0.0f,uvH * 1.0f,uvW,uvH}, // ランニングマシン1
        {uvW * 1.0f,uvH * 1.0f,uvW,uvH}, // ランニングマシン2
        {uvW * 2.0f,uvH * 1.0f,uvW,uvH}, // ランニングマシン3
        {uvW * 3.0f,uvH * 1.0f,uvW,uvH}, // ランニングマシン4
        {uvW * 0.0f,uvH * 2.0f,uvW,uvH}, // バーベル1
        {uvW * 1.0f,uvH * 2.0f,uvW,uvH}, // バーベル2
        {uvW * 0.0f,uvH * 3.0f,uvW,uvH}, // ダンベル1
        {uvW * 1.0f,uvH * 3.0f,uvW,uvH}, // ダンベル2
        {uvW * 0.0f,uvH * 4.0f,uvW,uvH}, // ラットプルダウン1
        {uvW * 1.0f,uvH * 4.0f,uvW,uvH}, // ラットプルダウン2
    };

    // ステータス別アニメーション定義
    UVRect squat[] = { uvs[0], uvs[1], uvs[2], uvs[1], {} };
    UVRect runningmachine[] = { uvs[6], uvs[3], uvs[4], uvs[5], {} };
    UVRect barbell[] = { uvs[7], uvs[8], {} };
    UVRect dumbbel[] = { uvs[9], uvs[10], {} };
    UVRect latpulldown[] = { uvs[11], uvs[12], {} };

    // アニメーションデータ作成
    Animation anim[5];
    CreateAnimationUV(anim[0], "squat", 5, 5.f, true, squat);
    CreateAnimationUV(anim[1], "runningmachine", 5, 3.f, true, runningmachine);
    CreateAnimationUV(anim[2], "barbell", 3, 3.f, true, barbell);
    CreateAnimationUV(anim[3], "dumbbel", 3, 3.f, true, dumbbel);
    CreateAnimationUV(anim[4], "latpulldown", 3, 3.f, true, latpulldown);

    // スプライトに登録
    for (int i = 0; i < 5; ++i) {
        mSprite.AddAnimation(anim[i]);
    }

    // 初期アニメはランニングマシン
    mSprite.PlayAnimation("runningmachine");
}

//------------------------------------------------------------
// ■ ステータス別更新処理
//   各ステータスに応じて適切なアニメーションを再生
//------------------------------------------------------------
void Player::_updateBarbell() {
    const string animName_barbell[] = { "barbell","","","" };
    mSprite.PlayAnimation(animName_barbell[0], AnimationPlayStyle::Continue);
}

void Player::_updateDumbbel() {
    const string animName_dumbbel[] = { "dumbbel","","","" };
    mSprite.PlayAnimation(animName_dumbbel[0], AnimationPlayStyle::Continue);
}

void Player::_updateLatPulldown() {
    const string animName_latpulldown[] = { "latpulldown","","","" };
    mSprite.PlayAnimation(animName_latpulldown[0], AnimationPlayStyle::Continue);
}

void Player::_updateRunningMachine() {
    const string animName_runningmachine[] = { "runningmachine","","","" };
    mSprite.PlayAnimation(animName_runningmachine[0], AnimationPlayStyle::Continue);
}

void Player::_updateSquat() {
    const string animName_squat[] = { "squat","","","" };
    mSprite.PlayAnimation(animName_squat[0], AnimationPlayStyle::Continue);
}

//------------------------------------------------------------
// ■ スコアに応じたステータス上昇
//   Good / Normal / Bad に応じて iStatus を加算
//   StatusData にも反映
//------------------------------------------------------------
void Player::IncreaseStatus(Score score)
{
    if (!mStatusData) {
        DebugLog("Player::IncreaseStatus: mStatusData is nullptr!");
        return;
    }

    int increment = 0;
    switch (score) {
    case Score::Good:   increment = 3; break;   // Goodなら3上昇
    case Score::Normal: increment = 1; break;   // Normalなら1上昇
    case Score::Bad:    increment = 0; break;   // Badは変化なし
    default: break;
    }

    // 内部カウンタ更新
    iStatus += increment;

    DebugLog("Player::IncreaseStatus: status=%d increment=%d iStatus=%d",
        (int)mStatus, increment, iStatus);

    // 現在ステータスに対応する部位のステータスに反映
    mStatusData->IncreaseStatus(mStatus, increment);
}