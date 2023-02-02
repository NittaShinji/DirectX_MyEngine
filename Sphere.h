#include "Object3d.h"

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Initialize();

	void Update();

	void Draw();

	//void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

private:

	Object3d* objce3d_;



};

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}