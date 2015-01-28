#include "CustomPointEffect.h"
USING_NS_CC;


namespace PointEffect{

	PlayerFollower* PlayerFollower::createWithTexture(Texture2D* texture)
	{
		PlayerFollower *sprite = new PlayerFollower();
		if (sprite && sprite->initWithTexture(texture))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
}