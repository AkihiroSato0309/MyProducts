var SceneFadeOut = function(removeScene, NextScene, color, duration)
{
	var fadeSprite = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
	fadeSprite.backgroundColor = color;
	fadeSprite.opacity = 0;
	removeScene.addChild(fadeSprite);
	fadeSprite.tl.fadeIn(duration).then(function(){g_game.replaceScene(new NextScene())});
}
