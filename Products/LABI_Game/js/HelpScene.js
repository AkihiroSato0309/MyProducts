HelpScene = Class.create(Scene,{
	// コンストラクタ
	initialize:function()
	{
		Scene.call(this);			// スーパークラスの初期化
		
		this.helpNumber = 0;		// ヘルプの番号
		this.helpSpriteArray = [];
		
		//==========================================================================
		// ヘルプスプライトを作成
		//==========================================================================
		var helpSprite_1 = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		helpSprite_1.image = g_game.assets[TEX_HELP_1];
		this.helpSpriteArray.push(helpSprite_1);
		// はじめだけあらかじめ出しておく
		this.addChild(helpSprite_1);
		
		var helpSprite_2 = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		helpSprite_2.image = g_game.assets[TEX_HELP_2];
		this.helpSpriteArray.push(helpSprite_2);
		
		var helpSprite_3 = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		helpSprite_3.image = g_game.assets[TEX_HELP_3];
		this.helpSpriteArray.push(helpSprite_3);


		//==========================================================================
		// タッチするたびにスプライトを更新し、最後にタイトルに戻る
		//==========================================================================
		this.ontouchstart = function()
		{
			this.helpSpriteArray[this.helpNumber].parentNode.removeChild(this.helpSpriteArray[this.helpNumber]);
			this.helpNumber++;
			if(this.helpNumber >= this.helpSpriteArray.length)
			{
				g_game.popScene(this);
				this.helpNumber = 0;
				return;
			}
			this.addChild(this.helpSpriteArray[this.helpNumber]);
			
		}
	}
})
