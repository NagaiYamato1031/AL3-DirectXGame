#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw() override;

	private:
	enum EnemyPartsIndex {
		kEnemyBody,
		kEnemyWeapon,

		kEnemyCount,
	};

};
