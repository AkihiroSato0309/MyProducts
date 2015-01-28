TukumoTan = Class.create(Sprite,{
	//####################################################################
	// コンストラクタ
	//####################################################################
	initialize:function(itemMgr, addNode, addNodeBefore)
	{
		// スーパークラスのコンストラクタ
		Sprite.call(this, 64, 64);
		
		this.image = g_game.assets[TEX_TUKUMO_TAN];				// 使用画像を指定
		this.frame = 0;
		
		this.originX = 32;
		this.originY = 64;
		
		this.scaleX = 0.0;										// スケールX
		this.scaleY = 2.0;										// スケールY
			
		this.x = GAME_SIZE_WIDTH * 0.5 - this.width * 0.5;		// 座標Xの初期位置を設定
		this.y = GAME_FLOOR_Y - this.height;					// 座標Yの初期位置を設定
		
		this.m_itemMgr = itemMgr;								// アイテムマネージャーを保存
		
		this.m_executeAiFlag = true;							// 商品走査AIを行うかどうか
		
		this.m_movingFlag = false;								// 移動中フラグ
		
		this.m_animationCounter = 0;							// アニメーションに使用するカウンター
		
		this.m_movePointX = 0;									// 移動先のX座標
		
		this.m_halfWidth = this.width * 0.5;					// 横幅の半分のサイズ
		this.m_centerX = this.x + this.m_halfWidth;				// 画像の中心位置のX座標
		
		this.m_existFrame = TUKUMO_SUPPORT_TIME * GAME_FPS;		// 画面上に存在可能なフレーム数を計算
		
		
		//================================================================
		// 生存時間バーのスプライト作成
		//================================================================
		this.m_bar = new Sprite(WEIGHT_BAR_WIDTH, 8);
		this.m_bar.image = g_game.assets[TEX_WEIGHT_BAR];
		this.m_bar.x = this.x;
		this.m_bar.y = this.y + this.height;
		addNode.insertBefore(this.m_bar, addNodeBefore);
		
		//================================================================
		// 登場時のアクションを登録する
		//================================================================
		
		this.tl.scaleTo(1, 1, TUKUMO_APPEAR_TIME);
		
		
		//================================================================
		// アイテムを自動追尾するAI
		//================================================================
		
		this.ItemGetAI = function()
		{
			// 配列が0の時はスルーする
			if(this.m_itemMgr.m_items.length == 0) return;
			
			var nearItemX;										// プレイヤーと最も近い商品のX座標
			var nearDefferenceX = 2000;							// プレイヤーと最も近い商品とのX座標の差	
			var defferenceX;									// プレイヤーと比較する商品とのX座標の差
			
			// 自身とX軸が最も近い商品を取得する
			for(var i = 0; i < this.m_itemMgr.m_items.length; i++)
			{
				defferenceX = Math.abs(this.m_itemMgr.m_items[i].m_centerX - this.m_centerX);
				if(defferenceX < nearDefferenceX)
				{
					nearDefferenceX = defferenceX;
					nearItemX = this.m_itemMgr.m_items[i].m_centerX;
				}
			}
			
			// ほぼ動かない場合、動かさない
			if(nearDefferenceX < 2) return;
			
			// 反転処理
			if(this.m_movePointX - this.x < 0) 	this.scaleX = 1;
			else								this.scaleX = -1;
			
			// 移動アクションを追加する
			this.m_movingFlag = true;
			this.m_movePointX = nearItemX - this.m_halfWidth;	// 実際に移動するポイント
			this.tl.clear().moveX(this.m_movePointX, Math.abs(this.m_movePointX - this.x) / TUKUMO_MOVE_SPEED).
				then(function(){
					this.m_movingFlag = false;
					this.frame = 0;
				});
		}	
		
		//================================================================
		// 更新処理
		//================================================================
		
		this.onenterframe = function()
		{
			// 一定カウントは動かない
			if(this.age <= TUKUMO_APPEAR_TIME) return 0;
			
			// 移動AI
			this.ItemGetAI();
			
			// 画像の中心のX座標を更新
			this.m_centerX = this.x + this.m_halfWidth;		
			
			// 移動AI
			this.ItemGetAI();
			
			//---------------------------------------------------------------
			// あたり判定をとる
			//---------------------------------------------------------------
			if(this.m_itemMgr.Hit(this))
			{
				// 画像をゲットした画像に切り替える
				this.frame = GET_ANIMATION;
				this.tl.delay(GET_STOP_FRAME).then(function(){this.frame = 0;});
			};
			
			//---------------------------------------------------------------
			// 存在時間を超えた場合、更新処理をやめ、帰る
			//---------------------------------------------------------------
			if(this.age > this.m_existFrame)
			{
				this.onenterframe = null;
				this.m_bar.parentNode.removeChild(this.m_bar);
				this.tl.clear().
						scaleTo(0, 2, TUKUMO_APPEAR_TIME).
						removeFromScene();				
			}
			
			//---------------------------------------------------------------
			// 生存時間バーの更新
			//---------------------------------------------------------------
			this.m_bar.x = this.x;
			this.m_bar.width = WEIGHT_BAR_WIDTH - WEIGHT_BAR_WIDTH * (this.age / this.m_existFrame); 
			
			//---------------------------------------------------------------
			// アニメーションさせる
			//---------------------------------------------------------------
			if(this.m_movingFlag === true)
			{
				// アニメーションカウンタを増加させる
				this.m_animationCounter++;
				
				if(this.m_animationCounter >= 4)
				{
					this.m_animationCounter = 0;
					this.frame++;
					if(this.frame > 2) this.frame = 1;
				}
			}
		}
	}
})

