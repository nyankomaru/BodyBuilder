#include "SceneResult.h"
#include "Fwk/Framework.h"

//============================================================
// ■ 初期化
//   シーン開始時に呼び出される
//   - 背景やスプライト、UIの初期化
//   - 次のシーンの初期値を設定
//============================================================
void SceneResult::Init() {
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
void SceneResult::Term() {
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
void SceneResult::Update() {
    if (Input_I->IsKeyDown('Z'))
    {
        // 次のシーンへ
        mNextScene = SceneType::Select;

        // セーブ
        GetDataMng()->Save("Assets/Data/SaveData.kv");

        // ステータス上昇差分リセット
        GetDataMng()->GetStatusData().ResetDiff();
    }
}

//============================================================
// ■ 描画
//   毎フレーム呼び出される
//   - 背景、スプライト、UI、選択肢文字を描画
//============================================================
void SceneResult::Render()
{
    // 背景描画
    mSprite.Draw();
    GetBG2()->Render();

    StatusData& status = GetDataMng()->GetStatusData();

    int x = 300;
    int y = 200;
    int dy = 40;

    PrintText("=== TRAINING RESULT ===", x, y);
    y += dy * 2;

    struct Item {
        const char* name;
        Status type;
    } items[] = {
        {"持久力 (Endurance)",  Status::RunningMachine},
        {"胸 (Chest)",          Status::Barbell},
        {"背中 (Back)",         Status::LatPulldown},
        {"腕 (Arm)",            Status::Dumbbel},
        {"脚 (Leg)",            Status::Squat},
    };

    for (auto& item : items)
    {
        int diff = status.GetDiff(item.type);
        if (diff != 0)
        {
            std::string s = std::string(item.name) + "  +" + std::to_string(diff);
            PrintText(s.c_str(), x, y);
            y += dy;
        }
    }
}