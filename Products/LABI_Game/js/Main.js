enchant();

window.onload = function()
{
	// ゲーム作成
	g_game = new Game(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
	g_game.fps = GAME_FPS;
	
	// 素材のプリロード
	TexturePreLoad();
	
	g_game.onload = function()
	{
		// シーンを作成
		var scene = new TitleScene();
		g_game.pushScene(scene);
	}
	
	g_game.start();
}
