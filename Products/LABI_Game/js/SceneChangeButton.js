//=========================================================================
//　
// フェードアウトしながらシーンチェンジするボタン
//
//=========================================================================
SceneChangeButton = Class.create(Sprite,{
	// コンストラクタ
	initialize:function(buttonWidth, 	// ボタンの横幅
						buttonHeight, 	// ボタンの縦幅
						textureName, 	// 画像の名前
						NextScene,		// 次のシーンのクラス
						removeScene,	// 離れるシーン
						color,			// 色（指定ないとデフォルト）
						duration)		// フェード時間(指定ないとデフォルト)
	{
		// デフォルト引数を設定
		if(typeof color === 'undefined') color = "black";
		if(typeof duration === 'undefined') duration = 0;
		
		// 大きさを設定
		Sprite.call(this, buttonWidth, buttonHeight);
		
		// 画像を追加
		this.image = g_game.assets[textureName];
		
		this.x = 0;
		this.y = 0;
		
		// 次のシーンに切り替える
		this.ontouchstart = function()
		{
			SceneFadeOut(removeScene, NextScene, color, duration);
		};
		
	}
})
