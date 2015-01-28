GameScene = Class.create(Scene,{
	// コンストラクタ
	initialize:function()
	{
		Scene.call(this);					// スーパークラスの初期化
		
		//===========================================================================
		// 変数宣言
		//===========================================================================
		
		var Point = { x : 0, y : 0 };			
		
		var touchPoint = Point;				// タッチポイント
		var tukumoMovePoint = Point;		// つくもたんの移動先
		var getPointLabel = new Label();	// 得点の表示ラベル
		var timer;							// タイマー
		var timeGraphArray = [];			// タイムを表示する画像配列（2ケタ）
		var scoreGraphArray = [];			// スコアを表示する画像配列(8桁)
		var tukumo = null;					// つくもたん
		var labi = null;					// らびたん
		var tukumo_icon = null;				// らびたんのお助けアイコン
		var junk_icon = null;				// じゃんくたんのお助けアイコン
		var thisScene = this;				// このシーンを保存
		var itemBox = null;					// 回収ボックス
		
		var timeLimitString;				// 残り時間を文字列に変換するための変数
		var stringTime;						// 変換した各桁の数値を入れる変数
		
		var itemGroup = new Group();		// アイテムを追加するグループ
		var uiGroup = new Group();			// ゲームのグループ
		this.addChild(itemGroup);			
		this.addChild(uiGroup);				
				
				
		//===========================================================================
		// 初期化処理
		//===========================================================================
		
		// ゲームカウントを初期化
		g_game.frame = 0;
		
		// タッチした位置を取得する
		function GetTouchPoint(e)
		{
			// タッチした位置を取得する
			touchPoint.x = e.x;
			touchPoint.y = e.y;
			
			// ゲーム範囲内をタッチした場合、らびたんに移動ポイントを渡す
			if(touchPoint.x < GAME_SIZE_WIDTH)
			{
				labi.m_moveFlag = true;
				labi.movePointX = touchPoint.x;
			}
		}
		
		// イベントを登録
		this.ontouchstart = GetTouchPoint;
		
		//------------------------------------------------------------------
		// ゲームの背景を作成
		//------------------------------------------------------------------
		var gameBackSprite = new Sprite(GAME_SIZE_WIDTH, WIN_SIZE_HEIGHT);
		gameBackSprite.image = g_game.assets[TEX_GAME_BACK];
		this.insertBefore(gameBackSprite, itemGroup);
		
		//------------------------------------------------------------------
		// ゲームのUI部分を作成する
		//------------------------------------------------------------------
		
		var gameRightSpace = new Sprite(GAME_RIGHT_SPACE, WIN_SIZE_HEIGHT);
		gameRightSpace.image = g_game.assets[TEX_GAME_RIGHT_SPACE];
		gameRightSpace.x = GAME_SIZE_WIDTH;
		gameRightSpace.y = 0;
		uiGroup.addChild(gameRightSpace);
		
		// タイム表示画像の初期化
		for(var i = 0; i < TIME_MAX; i++)
		{
			var numberSprite = new Sprite(NUMBER_WIDTH, NUMBER_HEIGHT);
			numberSprite.image = g_game.assets[TEX_NUMBER];
			numberSprite.frame = 0;
			numberSprite.originX = 0;
			numberSprite.originY = 0;
			numberSprite.scaleX = 0.7;
			numberSprite.scaleY = 0.7;
			numberSprite.x = GAME_SIZE_WIDTH + (GAME_RIGHT_SPACE - NUMBER_WIDTH * numberSprite.scaleX * 2) * 0.5 + i * numberSprite.scaleX * NUMBER_WIDTH;
			numberSprite.y = 25;
			uiGroup.addChild(numberSprite);
			timeGraphArray.push(numberSprite);
		}
		
		// スコア表示画像の初期化
		for(var i = 0; i < SCORE_MAX; i++)
		{
			var numberSprite = new Sprite(NUMBER_WIDTH, NUMBER_HEIGHT);
			numberSprite.image = g_game.assets[TEX_NUMBER];
			numberSprite.frame = 0;
			numberSprite.originX = 0;
			numberSprite.originY = 0;
			numberSprite.scaleX = 0.7;
			numberSprite.scaleY = 0.7;
			numberSprite.x = (GAME_RIGHT_SPACE - NUMBER_WIDTH * numberSprite.scaleX * 2) * 0.5 + i * numberSprite.scaleX * NUMBER_WIDTH;
			numberSprite.y = 10;
			uiGroup.addChild(numberSprite);
			scoreGraphArray.push(numberSprite);
		}
		
		
		//------------------------------------------------------------------
		// アイテム管理を作成
		//------------------------------------------------------------------
		
		var itemMgr = new ItemMgr(itemGroup);
		this.addChild(itemMgr);
		
		
		//------------------------------------------------------------------
		// らびたんを生成
		//------------------------------------------------------------------
		
		labi = new LabiTan(itemMgr, this, uiGroup);		
		
		
		//=====================================================================
		// お助けキャラを生成する
		//=====================================================================
		
		//------------------------------------------------------------------
		// つくもたんアイコン
		//------------------------------------------------------------------
		tukumo_icon = new Sprite(SUPPORT_CION_SIZE, SUPPORT_CION_SIZE);
		tukumo_icon.image = g_game.assets[TEX_TUKUMO_ICON];
		tukumo_icon.x = GAME_SIZE_WIDTH + 3;
		tukumo_icon.y = WIN_SIZE_HEIGHT - tukumo_icon.height - 2;
		
		// タッチした時の動作を指定
		tukumo_icon.ontouchstart = function()
		{
			// すでに使用済みの場合はスルーする
			if(tukumo_icon.frame == 1) return;
			
			// カットインを入れる
			var cutIn = new Sprite(WIN_SIZE_WIDTH, 120);
			cutIn.image = g_game.assets[TEX_TUKUMO_CUTIN];
			cutIn.x = -WIN_SIZE_WIDTH;
			cutIn.y = 100;
			thisScene.addChild(cutIn);
			cutIn.tl.moveX(0, 10).delay(10).
				moveX(WIN_SIZE_WIDTH, 10).
				removeFromScene();
			
			// アイコンを黒に切り替える
			tukumo_icon.frame = 1;
			
			// つくもたん
			tukumo = new TukumoTan(itemMgr, thisScene, uiGroup);
			thisScene.insertBefore(tukumo, uiGroup);
		}
		uiGroup.addChild(tukumo_icon);
		
		//------------------------------------------------------------------
		// じゃんくたんアイコン
		//------------------------------------------------------------------
		junk_icon = new Sprite(SUPPORT_CION_SIZE, SUPPORT_CION_SIZE);
		junk_icon.image = g_game.assets[TEX_JUNK_ICON];
		junk_icon.x = GAME_SIZE_WIDTH + junk_icon.width + 5;
		junk_icon.y = WIN_SIZE_HEIGHT - junk_icon.height - 2;
		
		// タッチした時の動作を指定
		junk_icon.ontouchstart = function()
		{			
			// すでに使用済みの場合はスルーする
			if(junk_icon.frame == 1) return;
			
			// カットインを入れる
			var cutIn = new Sprite(WIN_SIZE_WIDTH, 120);
			cutIn.image = g_game.assets[TEX_JUNK_CUTIN];
			cutIn.x = -WIN_SIZE_WIDTH;
			cutIn.y = 100;
			thisScene.addChild(cutIn);
			cutIn.tl.moveX(0, 10).delay(10).moveX(WIN_SIZE_WIDTH, 10).removeFromScene();
			
			
			// アイコンを使用済み画像に切り替える
			junk_icon.frame = 1;
			
			// らびたんの所持アイテムを回収する
			labi.m_carryWeight = 0;
			
		}
		
		uiGroup.addChild(junk_icon);
		
		
		//------------------------------------------------------------------
		// 得点を表示
		//------------------------------------------------------------------
		
		for(var i = 0; i < SCORE_MAX; i++)
		{
			var tmpScore = g_totalScore / Math.pow(10, SCORE_MAX - i - 1) % 10;
			scoreGraphArray[i].frame = tmpScore;
		}
		
		
		//------------------------------------------------------------------
		// 回収ボックスを作成
		//------------------------------------------------------------------
		
		itemBox = new Sprite(BOX_WIDTH, BOX_HEIGHT);
		itemBox.image = g_game.assets[TEX_TIME_BACK];
		itemBox.x = GAME_SIZE_WIDTH - itemBox.width;
		itemBox.y = WIN_SIZE_HEIGHT - itemBox.height;
		this.addChild(itemBox);
		
		
		//=====================================================================
		// 更新処理
		//=====================================================================
		
		this.onenterframe =function()
		{
			
			// 残り時間を計算する
			timer = GAME_LIMIT_TIME - Math.floor(g_game.frame/g_game.fps);
			
			// タイマーが0になったらシーンを変更する
			if(timer < 0)
			{
				timer = 0;
				SceneFadeOut(this, ResultScene, "black", SCENE_FADE_TIME);
				this.onenterframe = null;
				labi.tl.clear();
				labi.onenterframe = null;
				if(tukumo != null)
				{
					tukumo.tl.clear();
				 	tukumo.onenterframe = null;
				}
			} 
			
			// 残り時間の表示を更新する
			timeLimitString = timer.toString(10);
			for(var i = 0; i < TIME_MAX; i++)
			{
				stringTime = timer / Math.pow(10, TIME_MAX - i - 1) % 10;
				timeGraphArray[i].frame = stringTime;
			}
			
			
			//----------------------------------------------------------------
			// 回収ボックスとらびたんの当たり判定をとる
			//----------------------------------------------------------------
			
			if(labi.intersect(itemBox) && labi.m_carryWeight > 0)
			{
				// 商品を回収する
				labi.m_carryWeight = 0;
				
				// インスプライトを表示させる
				var inSprite = new Sprite(64, 32);
				inSprite.image = g_game.assets[TEX_IN];
				inSprite.x = itemBox.x + itemBox.width * 0.5 - inSprite.width * 0.5;
				inSprite.y = 220;
				this.addChild(inSprite);
				
				// 移動して消える処理
				inSprite.tl.moveBy(0, -20, 40).then(function(){DeleteThis(this);});
				
			}
			
			
			//----------------------------------------------------------------
			// 商品を作成する
			//----------------------------------------------------------------
			
			if(this.age > 60)
			{
				this.age = 0;
				var itemNum = Math.floor(Math.random() * ITEM_VAL);
				var tmpItem = new Item(itemGroup, itemNum);
				itemMgr.AddItem(tmpItem);
			}
			
			
			//------------------------------------------------------------------
			// 得点を更新
			//------------------------------------------------------------------
			
			for(var i = 0; i < SCORE_MAX; i++)
			{
				var tmpScore = g_totalScore / Math.pow(10, SCORE_MAX - i - 1) % 10;
				scoreGraphArray[i].frame = tmpScore;
			}
			
		}	
		
		// 背景色を設定
		this.backgroundColor = '#7ecef4';
	}	
});