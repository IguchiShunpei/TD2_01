#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
#include "EnemyBullet.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//自機クラスの前方宣言
class Player;

class Enemy
{

	enum class Phase
	{
		None,
		Approach,//接近
		Leave,   //離脱
	};

public:
	/// 生成
	void Initialize(Model* model, uint32_t textureHandle);

	/// 更新
	void Update();

	/// 描画
	void Draw(ViewProjection viewProjection);

	//接近
	void Approach();

	//離脱
	void Leave();

	//弾発射
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//半径を返す関数
	float GetRadius();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_ = { 0.0f,0.0f,0.1f };

	//接近速度
	Vector3 approach_ = {0.0f,0.0f,-0.1f};

	//離脱速度
	Vector3 leave_ = {-0.15f,0.1f,0.1f};

	// Affin関数の構造体
	affin::AffinMat affinMat;

	//打ち出すまでの時間
	float delayTimer = 0.0f;

	//敵の行動パターン
	Phase phase_ = Phase::None;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//自キャラ
	Player* player_ = nullptr;

	//半径
	float radius = 1.0f;
};

