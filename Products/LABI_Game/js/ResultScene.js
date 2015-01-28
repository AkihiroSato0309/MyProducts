ResultScene = Class.create(Scene,{
	// コンストラクタ
	initialize:function()
	{
		Scene.call(this);				// スーパークラスの初期化
		
		var backGraph = null;			// 背景スプライト
		var scoreShowBack = null;		// スコア表示の板スプライト
		var scoreGraphArray = [];		// スコア表示の数字画像
		var thisScene = this;			// 今のシーンを保存
		
		// スコア描画配列の初期化
		for(var i = 0; i < SCORE_MAX; i++)
		{
			var numberSprite = new Sprite(NUMBER_WIDTH, NUMBER_HEIGHT);
			numberSprite.image = g_game.assets[TEX_NUMBER];
			numberSprite.frame = 0;
			numberSprite.originX = 0;
			numberSprite.originY = 0;
			numberSprite.scaleX = 1;
			numberSprite.scaleY = 1;
			// X座標を一桁ずつずらす処理
			numberSprite.x = (WIN_SIZE_WIDTH - NUMBER_WIDTH * numberSprite.scaleX * SCORE_MAX) * 0.5 + i * NUMBER_WIDTH * numberSprite.scaleX;
			numberSprite.y = 100;
			scoreGraphArray.push(numberSprite);
		}
				
		// スコアをシーンに追加する関数
		var AddScoreGraph = function()
		{
			for(var i = 0; i < SCORE_MAX; i++)
			{
				var tmpScore = g_totalScore / Math.pow(10, SCORE_MAX - i - 1) % 10;
				scoreGraphArray[i].frame = tmpScore;
				thisScene.addChild(scoreGraphArray[i]);
			}
		}
		
		// 背景を表示
		backGraph = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		backGraph.image = g_game.assets[TEX_RESULT_BACK];
		this.addChild(backGraph);
		
		// スコア背景を表示
		scoreShowBack = new Sprite(WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		scoreShowBack.image = g_game.assets[TEX_SCORE_SHOW_BACK];
		scoreShowBack.y = -WIN_SIZE_HEIGHT;
		this.addChild(scoreShowBack);
		
		//==========================================================================
		// リザルトシーンの流れ
		//==========================================================================
		scoreShowBack.tl.moveTo(0, 0, 60, enchant.Easing.SIN_EASEIN).
							moveBy(0, -50, 20, enchant.Easing.SIN_EASEOUT).
							moveBy(0,  50, 20, enchant.Easing.SIN_EASEIN).
							then(AddScoreGraph);
		
		this.backgroundColor = "rgb(255, 150, 150)";
	}
})
