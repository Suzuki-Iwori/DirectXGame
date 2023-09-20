#include <Model.h>
#include <WorldTransform.h>
#include <cassert>

class Particle final {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; };

private:
	const int32_t kLifeCount = 20;
	int32_t deathTimer_ = kLifeCount;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velosity_{};

	bool isDead_ = false;
};