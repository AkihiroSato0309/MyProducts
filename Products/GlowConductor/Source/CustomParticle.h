#pragma once
#include "CCParticleSystemQuad.h"

NS_CC_BEGIN

//***************************************************************************************
// 自作パーティクルのクラス群
//***************************************************************************************
namespace MyParticles{

	// 闇オブジェクトを中心に発生するパーティクル
	class DarkParticle : public cocos2d::ParticleSystemQuad
	{
	public:
		CREATE_FUNC(DarkParticle);

	protected:
		DarkParticle(){}

		virtual ~DarkParticle(){}

		bool init(){ return initWithTotalParticles(20); }
		virtual bool initWithTotalParticles(int numberOfParticles);

	private:
		CC_DISALLOW_COPY_AND_ASSIGN(DarkParticle);
	};

	// タッチ時に発生するパーティクル
	class TouchParticle : public cocos2d::ParticleSystemQuad
	{
	public:
		CREATE_FUNC(TouchParticle);

	protected:
		TouchParticle(){}
		virtual ~TouchParticle(){}

		bool init(){ return initWithTotalParticles(18); }
		virtual bool initWithTotalParticles(int numberOfParticles);

	private:
		CC_DISALLOW_COPY_AND_ASSIGN(TouchParticle);
	};

	// 自機から出るパーティクル
	class GlowFollowParticle : public cocos2d::ParticleSystemQuad
	{
	public:
		CREATE_FUNC(GlowFollowParticle);

	protected:
		GlowFollowParticle(){}
		virtual ~GlowFollowParticle(){}

		bool init(){ return initWithTotalParticles(60); }
		virtual bool initWithTotalParticles(int numberOfParticles);

	private:
		CC_DISALLOW_COPY_AND_ASSIGN(GlowFollowParticle);
	};

	// バックレイヤーエフェクトで使用するパーティクル
	class FlashParticle : public cocos2d::ParticleSystemQuad
	{
	public:
		CREATE_FUNC(FlashParticle);

	protected:
		FlashParticle(){}

		virtual ~FlashParticle(){}

		bool init(){ return initWithTotalParticles(40); }
		virtual bool initWithTotalParticles(int numberOfParticles);

	private:
		CC_DISALLOW_COPY_AND_ASSIGN(FlashParticle);
	};
}

NS_CC_END