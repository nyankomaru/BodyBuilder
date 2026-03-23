#include "SceneSelect.h"
#include "Fwk/Framework.h"

//============================================================
// ■ 初期化
//   シーン開始時に呼び出される
//   - 背景やスプライト、UIの初期化
//   - 次のシーンの初期値を設定
//============================================================
void SceneSelect::Init() {
    // 次のシーンは初期状態では無し
    mNextScene = SceneType::None;

    // 背景スプライトの読み込みと初期化
    mTexture.Load("Images\\Jim.png");    // 背景画像読み込み
    mSprite.Init();                       // スプライト初期化
    mSprite.SetTexture(mTexture);         // テクスチャ設定
    mSprite.SetSize(1280, 720);           // スプライトサイズ設定
    mSprite.SetPosition(0, 0);            // 位置設定（左上）
    mSprite.SetVisible(true);             // 表示状態を有効化

    // 背景2オブジェクトを初期化
    GetBG2()->Init();
}

//============================================================
// ■ 終了
//   シーン終了時に呼び出される
//   - 使用リソースの解放
//============================================================
void SceneSelect::Term() {
    mTexture.Unload();    // テクスチャ解放
    mSprite.Term();       // スプライト終了処理
    GetBG2()->Term();     // 背景2終了処理
}

//============================================================
// ■ 更新
//   毎フレーム呼び出される
//   - 入力に応じて選択肢を変更
//   - 決定キーで次のシーンを設定
//============================================================
void SceneSelect::Update() {
    // ▼ 下キー入力：選択インデックスを進める
    if (Input_I->IsKeyDown(VK_DOWN)) {
        ++mSelectedIndex;                   // インデックスを1つ増やす
        if (mSelectedIndex >= kSceneCount)  // 上限を超えた場合は先頭に戻す
            mSelectedIndex = 0;
    }

    // ▼ 上キー入力：選択インデックスを戻す
    if (Input_I->IsKeyDown(VK_UP)) {
        --mSelectedIndex;                   // インデックスを1つ減らす
        if (mSelectedIndex < 0)             // 下限を下回った場合は末尾に設定
            mSelectedIndex = kSceneCount - 1;
    }

    // ▼ Zキー入力：選択したシーンを次シーンに設定
    if (Input_I->IsKeyDown('Z')) {
        mNextScene = mSceneList[mSelectedIndex];
    }

    // ゲームオブジェクトの更新（PlayerやUIなど）
    GameObjectUpdate();
}

//============================================================
// ■ 描画
//   毎フレーム呼び出される
//   - 背景、スプライト、UI、選択肢文字を描画
//============================================================
void SceneSelect::Render() {
    // 背景描画
    mSprite.Draw();
    GetBG2()->Render();

    // ゲームオブジェクト描画（PlayerやUIなど）
    GameObjectRender();

    // 文字色設定
    float color_selected[3] = { 0.0f, 1.0f, 1.0f }; // 選択中の文字は黄色
    float color_normal[3] = { 0.85f, 0.0f, 0.85f }; // 通常の文字色（紫）

    // 選択肢の描画ループ
    for (int i = 0; i < kSceneCount; ++i) {
        const std::string& label = menuLabels[i];                // 選択肢テキスト取得
        float* color = (mSelectedIndex == i) ? color_selected : color_normal;
        SetTextColor(color);                                     // 色設定
        PrintText(label.c_str(), 380.0f, 250.0f + i * 40.0f);    // テキスト描画
    }
}
