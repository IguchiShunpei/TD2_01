#include "RailCamera.h"

using namespace MathUtility;

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	input_ = Input::GetInstance();
	//シングルインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期設定(座標と回転角)
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	//viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	Vector3 move = { 0, 0, 0 };

	const float kCharaSpeed = 0.2f;

	Matrix4 matTranslate;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

#pragma region 移動処理
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharaSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharaSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	
	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	matTranslate = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region 回転処理
	Vector3 Rotation = { 0, 0, 0 };
	const float kCharaRotX = MyFunc::Deg2Rad(0.5f);

	Matrix4 matRotX;

	//回転処理
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.x += kCharaRotX;
	}
	if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.x -= kCharaRotX;
	}
	//(-)2πを超えたら回転角を0に戻す処理
	if (worldTransform_.rotation_.x >= (M_PI * 2) || worldTransform_.rotation_.x <= -(M_PI * 2)) {
		worldTransform_.rotation_.x = 0;
	}

	matRotX = affin::generateRotateXMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotX);

	worldTransform_.matWorld_.operator*=(matTranslate);

	//行列の転送
	worldTransform_.TransferMatrix();



	viewProjection_.eye = worldTransform_.translation_;
	//前方ベクトル
	Vector3 forward(0, 0, 1);

	//レールカメラの回転を反映
	forward = affin::matVector(forward, worldTransform_.matWorld_);

	//視点から前方に適当に進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;

	//上方ベクトル
	Vector3 up(0, 1, 0);
	//
	viewProjection_.up = affin::matVector(up, worldTransform_.matWorld_);

	//ビュープロジェクションの更新と行列バッファへの転送
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//eyeの表示
	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y,
		viewProjection_.eye.z);

}

WorldTransform* RailCamera::GetWorldPosition()
{
	return &worldTransform_;
}