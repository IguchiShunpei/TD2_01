#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "math.h"
#include "affin/affin.h"

namespace MyFunc
{
	//ラジアン変換
	float Deg2Rad(float x);

	//度変換
	float Rad2Deg(float x);

	void UpdateWorldTransform(WorldTransform& worldTransform);
};

