#include "Player.h"

void Player::Initialize(Model* model, Model* bulletModel, const Vector3& position) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	bulletModel_ = bulletModel;

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

	Move();

	TransformUI(viewProjection);

	Attack();

	if (playerLife_ <= 0) {
		isPlayerLive_ = false;
	}

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

}
void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::Move() {

	XINPUT_STATE joyState{};

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	const float kLimitX = 33.0f;
	const float kLimitY = 17.0f;

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kLimitY);

	worldTransform_.UpdateMatrix();

}

void Player::Attack() {

	XINPUT_STATE joyState{};

	if (bulletCoolTime_ <= 0) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {

				SetShot();

			}
		}
	}

	if (bulletCoolTime_ > 0) {
		bulletCoolTime_--;
	}

}

void Player::SetShot() {

	if (bulletCoolTime_ <= 0) {

		const float bulletSpeed = 4.0f;
		Vector3 velosity(0.0f, 0.0f, 1.0f);

		velosity = worldTransform3DReticle_.translation_ - GetWorldPosition();
		velosity = Normalize(velosity) * bulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet;
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velosity);

		playerBullets_.push_back(newBullet);

		bulletCoolTime_ = 3;

	}

}

void Player::Restart(const Vector3& position) {

	playerLife_ = 4;
	isPlayerLive_ = true;
	bulletCoolTime_ = 0;

	worldTransform_.translation_ = position;

	sprite2DReticle_->SetPosition({640.0f, 360.0f});

}

void Player::OnCollision() { playerLife_--; }

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::TransformUI(ViewProjection& viewProjection) {

	/*const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;

	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = {
	    worldTransform3DReticle_.matWorld_.m[3][0], 
		worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]};

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewPort =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	positionReticle = Transform(positionReticle, matViewProjectionViewPort);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x,positionReticle.y));*/


	//POINT mousePosition{};
	//GetCursorPos(&mousePosition);

	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	XINPUT_STATE joyState{};

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 10.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 10.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	} /* else {
		sprite2DReticle_->SetPosition({float(mousePosition.x), float(mousePosition.y)});
	}*/

	if (sprite2DReticle_->GetPosition().x < 0.0f) {
		sprite2DReticle_->SetPosition({0.0f, sprite2DReticle_->GetPosition().y});
	}
	if (sprite2DReticle_->GetPosition().x > 1280.0f) {
		sprite2DReticle_->SetPosition({1280.0f, sprite2DReticle_->GetPosition().y});
	}
	if (sprite2DReticle_->GetPosition().y < 0.0f) {
		sprite2DReticle_->SetPosition({sprite2DReticle_->GetPosition().x, 00.0f});
	}
	if (sprite2DReticle_->GetPosition().y > 720.0f) {
		sprite2DReticle_->SetPosition({sprite2DReticle_->GetPosition().x, 720.0f});
	}

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewPort =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matViewProjectionViewPort);

	Vector3 posNear{};
	Vector3 posFar{};

	posNear = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 0.0f);
	posFar = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 1.0f);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 reticleDirection = posNear - posFar;
	reticleDirection = Normalize(reticleDirection);

	const float kDistanceObject = 100.0f;

	worldTransform3DReticle_.translation_ = posNear - (reticleDirection * kDistanceObject);
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