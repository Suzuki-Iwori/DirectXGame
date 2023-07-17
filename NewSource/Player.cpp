#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;


	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f, 360.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	input_ = Input::GetInstance();

}
void Player::Update(ViewProjection& viewProjection) {

	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		} else {
			return false;
		}
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	const float kLimitX = 33.0f;
	const float kLimitY = 17.0f;

	//if (input_->PushKey(DIK_LEFT)) {
	//	move.x -= kCharacterSpeed;
	//}
	//else if (input_->PushKey(DIK_RIGHT)) {
	//	move.x += kCharacterSpeed;
	//}

	//if (input_->PushKey(DIK_UP)) {
	//	move.y += kCharacterSpeed;
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	move.y -= kCharacterSpeed;
	//}

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	}
	 else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	 if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	 } else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	 }


	Rotate();

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kLimitY);

	worldTransform_.UpdateMatrix();


	TransformUI(viewProjection);

	Attack();

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

}
void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection);
	}

}
void Player::Rotate() {

	//const float kRotateSpeed = 0.02f;	

	//if (input_->PushKey(DIK_A)) {
	//	worldTransform_.rotation_.y -= kRotateSpeed;
	//} else if (input_->PushKey(DIK_D)) {
	//	worldTransform_.rotation_.y += kRotateSpeed;
	//}

}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float bulletSpeed = 1.0f;
		Vector3 velosity(0.0f, 0.0f, 1.0f);

		velosity = worldTransform3DReticle_.translation_ - GetWorldPosition();
		velosity = Normalize(velosity) * bulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet;
		newBullet->Initialize(model_, GetWorldPosition(), velosity);

		playerBullets_.push_back(newBullet);

	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::TransformUI(ViewProjection& viewProjection) {

	//const float kDistancePlayerTo3DReticle = 50.0f;
	//Vector3 offset = {0.0f, 0.0f, 1.0f};
	//offset = TransformNormal(offset, worldTransform_.matWorld_);
	//offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	//worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;

	//worldTransform3DReticle_.UpdateMatrix();

	//Vector3 positionReticle = {
	//    worldTransform3DReticle_.matWorld_.m[3][0], 
	//	worldTransform3DReticle_.matWorld_.m[3][1],
	//    worldTransform3DReticle_.matWorld_.m[3][2]};

	//Matrix4x4 matViewport =
	//    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//Matrix4x4 matViewProjectionViewPort =
	//    viewProjection.matView * viewProjection.matProjection * matViewport;

	//positionReticle = Transform(positionReticle, matViewProjectionViewPort);

	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x,positionReticle.y));


	POINT mousePosition{};
	GetCursorPos(&mousePosition);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition({float(mousePosition.x), float(mousePosition.y)});

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewPort =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matViewProjectionViewPort);

	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0.0f);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1.0f);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceObject = 100.0f;

	worldTransform3DReticle_.translation_ = posNear - (mouseDirection * kDistanceObject);
	worldTransform3DReticle_.UpdateMatrix();

}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

Player::~Player() { 
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition{};

	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];

	return worldPosition;
}