#include "CustomParticle.h"
#include "CCDirector.h"
#include "CCTextureCache.h"
#include "firePngData.h"
#include "SCameraTargetMgr.h"

NS_CC_BEGIN

static Texture2D* getDefaultTexture()
{
	Texture2D* texture = nullptr;
	Image* image = nullptr;
	do
	{
		bool ret = false;
		const std::string key = "/__firePngData";
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(key);
		CC_BREAK_IF(texture != nullptr);

		image = new Image();
		CC_BREAK_IF(nullptr == image);
		ret = image->initWithImageData(__firePngData, sizeof(__firePngData));
		CC_BREAK_IF(!ret);

		texture = Director::getInstance()->getTextureCache()->addImage(image, key);
	} while (0);

	CC_SAFE_RELEASE(image);

	return texture;
}

namespace MyParticles
{
	// 闇オブジェクトに関するパーティクル
	bool DarkParticle::initWithTotalParticles(int numberOfParticles)
	{
		if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
		{
			_duration = DURATION_INFINITY;

			setEmitterMode(Mode::RADIUS);

			setStartRadius(50);
			setStartRadiusVar(10);
			setEndRadius(4);
			setEndRadiusVar(4);
			setRotatePerSecond(-1000);
			setRotatePerSecondVar(100);

			// angle
			_angle = 90;
			_angleVar = 360;

			// デフォルト発生位置
			cocos2d::Size winSize = Director::getInstance()->getWinSize();
			this->setPosition(Point::ZERO);
			setPosVar(Point::ZERO);

			// パーティクルの生存時間
			_life = 2.0f;
			_lifeVar = 1.0f;

			// ピクセル単位でのサイズ
			_startSize = 200.0f;
			_startSizeVar = 20.0f;
			_endSize = 100.0f;
			_endSizeVar = 5.0f;

			// 一秒あたりのパーティクル発生量
			_emissionRate = _totalParticles / _duration;

			// パーティクルの色
			_startColor.r = 0.1f;
			_startColor.g = 0.1f;
			_startColor.b = 0.1f;
			_startColor.a = 0.01f;
			_startColorVar.r = 0.0f;
			_startColorVar.g = 0.0f;
			_startColorVar.b = 0.0f;
			_startColorVar.a = 0.0f;
			_endColor.r = 0.0f;
			_endColor.g = 0.0f;
			_endColor.b = 0.0f;
			_endColor.a = 1.0f;
			_endColorVar.r = 0.0f;
			_endColorVar.g = 0.0f;
			_endColorVar.b = 0.0f;
			_endColorVar.a = 0.0f;

			Texture2D* texture = getDefaultTexture();
			if (texture != nullptr)
			{
				setTexture(texture);
			}

			// additive
			this->setBlendAdditive(false);
			return true;
		}
		return false;
	}

	// タッチ時に出現するパーティクル
	bool TouchParticle::initWithTotalParticles(int numberOfParticles)
	{
		if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
		{
			_duration = 0.01f;

			setEmitterMode(Mode::RADIUS);

			// 半径法での設定
			setStartRadius(0);
			setStartRadiusVar(0);
			setEndRadius(100);
			setEndRadiusVar(20);
			setRotatePerSecond(0);
			setRotatePerSecondVar(0);

			// angle
			_angle = 90;
			_angleVar = 360;

			// デフォルト発生位置
			cocos2d::Size winSize = Director::getInstance()->getWinSize();
			this->setPosition(Point(winSize.width / 2, winSize.height / 2));
			setPosVar(Point::ZERO);

			// パーティクルの回転
			_startSpin = 100.0f;
			_startSpinVar = 100.0f;
			_endSpin = 720.0f;
			_endSpinVar = 100.0f;

			// パーティクルの生存時間
			_life = 0.5f;
			_lifeVar = 0.0f;

			// ピクセル単位でのサイズ
			_startSize = 30.0f;
			_startSizeVar = 10.0f;
			_endSize = 10.0f;
			_endSizeVar = 1.0f;

			// 一秒あたりのパーティクル発生量
			_emissionRate = _totalParticles / _duration;

			// パーティクルの色
			_startColor.r = 0.7f;
			_startColor.g = 0.7f;
			_startColor.b = 0.1f;
			_startColor.a = 1.0f;
			_startColorVar.r = 0.2f;
			_startColorVar.g = 0.2f;
			_startColorVar.b = 0.0f;
			_startColorVar.a = 0.0f;
			_endColor.r = 1.0f;
			_endColor.g = 1.0f;
			_endColor.b = 0.0f;
			_endColor.a = 0.0f;
			_endColorVar.r = 0.5f;
			_endColorVar.g = 0.5f;
			_endColorVar.b = 0.0f;
			_endColorVar.a = 0.0f;

			Texture2D* texture = getDefaultTexture();
			if (texture != nullptr)
			{
				setTexture(texture);
			}

			// additive
			this->setBlendAdditive(true);
			this->setPositionType(ParticleSystem::PositionType::RELATIVE);
			return true;
		}
		return false;
	}

	// 自機から出るパーティクル
	bool GlowFollowParticle::initWithTotalParticles(int numberOfParticles)
	{
		if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
		{
			// duration
			_duration = DURATION_INFINITY;

			// Gravity Mode
			this->_emitterMode = Mode::GRAVITY;

			// Gravity Mode: gravity
			this->modeA.gravity = Point(0, 0);

			// Gravity Mode: radial acceleration
			this->modeA.radialAccel = 0;
			this->modeA.radialAccelVar = 0;

			// Gravity Mode: speed of particles
			this->modeA.speed = 0;
			this->modeA.speedVar = 0;

			// starting angle
			_angle = 90;
			_angleVar = 0;

			// emitter position
			Size winSize = Director::getInstance()->getWinSize();
			this->setPosition(Point(winSize.width / 2, 60));
			this->_posVar = Point(20, 20);

			// life of particles
			_life = 5;
			_lifeVar = 0.25f;


			// size, in pixels
			_startSize = 54.0f;
			_startSizeVar = 10.0f;
			_endSize = START_SIZE_EQUAL_TO_END_SIZE;

			// emits per frame
			_emissionRate = _totalParticles / _life;

			// color of particles
			_startColor.r = 0.76f;
			_startColor.g = 0.25f;
			_startColor.b = 0.12f;
			_startColor.a = 1.0f;
			_startColorVar.r = 0.0f;
			_startColorVar.g = 0.0f;
			_startColorVar.b = 0.0f;
			_startColorVar.a = 0.0f;
			_endColor.r = 0.0f;
			_endColor.g = 0.0f;
			_endColor.b = 0.0f;
			_endColor.a = 1.0f;
			_endColorVar.r = 0.0f;
			_endColorVar.g = 0.0f;
			_endColorVar.b = 0.0f;
			_endColorVar.a = 0.0f;

			Texture2D* texture = getDefaultTexture();
			if (texture != nullptr)
			{
				setTexture(texture);
			}

			// additive
			this->setBlendAdditive(true);
			// プレイヤーではなく、ゲームレイヤーに追加することを前提
			this->setPositionType(ParticleSystem::PositionType::FREE);
			return true;
		}
		return false;
	}

	// 画面上にランダムに出て消えるパーティクル
	bool FlashParticle::initWithTotalParticles(int numberOfParticles)
	{
		if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
		{
			// duration
			_duration = DURATION_INFINITY;

			// Gravity Mode
			this->_emitterMode = Mode::GRAVITY;

			// Gravity Mode: gravity
			this->modeA.gravity = Point(0, 0);

			// Gravity Mode: radial acceleration
			this->modeA.radialAccel = 0;
			this->modeA.radialAccelVar = 0;

			// Gravity Mode: speed of particles
			this->modeA.speed = 0;
			this->modeA.speedVar = 0;

			// starting angle
			_angle = 90;
			_angleVar = 0;

			// 発生地点設定
			Size winSize = Director::getInstance()->getWinSize();
			this->setPosition(Point(winSize.width / 2.0f, winSize.height / 2.0f));
			this->_posVar = Point(winSize.width, winSize.height / 2.0f);

			// パーティクルの命
			_life = 2.0f;
			_lifeVar = 1.5f;


			// size, in pixels
			_startSize = 10.0f;
			_startSizeVar = 10.0f;
			_endSize = 100.0f;
			_endSizeVar = 70.0f;

			// emits per frame
			_emissionRate = _totalParticles / _life;

			// color of particles
			_startColor.r = 1.0f;
			_startColor.g = 1.0f;
			_startColor.b = 1.0f;
			_startColor.a = 1.0f;
			_startColorVar.r = 0.4f;
			_startColorVar.g = 0.4f;
			_startColorVar.b = 0.4f;
			_startColorVar.a = 0.4f;
			_endColor.r = 1.0f;
			_endColor.g = 1.0f;
			_endColor.b = 1.0f;
			_endColor.a = 0.0f;
			_endColorVar.r = 0.4f;
			_endColorVar.g = 0.4f;
			_endColorVar.b = 0.4f;
			_endColorVar.a = 0.2f;

			Texture2D* texture = getDefaultTexture();
			if (texture != nullptr)
			{
				setTexture(texture);
			}

			// additive
			this->setBlendAdditive(true);
			this->setPositionType(ParticleSystem::PositionType::RELATIVE);
			return true;
		}
		return false;
	}
}

NS_CC_END