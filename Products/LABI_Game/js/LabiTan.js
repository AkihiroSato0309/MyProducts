LabiTan = Class.create(Sprite,{
	// コンストラクタ
	initialize:function(itemMgr, addNode, addNodeBefore)
	{
		Sprite.call(this, 64, 64);							// スプライトの初期化
		this.image = g_game.assets[TEX_LABI_TAN];			// 画像の呼び出し
		this.frame = 0;
		
		this.x = 0;											// X座標指定
		this.y = GAME_FLOOR_Y - this.height;				// Y座標指定
		
		this.flipFlag = false;								// 反転フラグ
		this.moveFlag = false;								// 移動フラグ
		
		this.speed = LABI_MOVE_SPEED;						// 移動速度の指定
		this.movePointX;									// 移動するXポイント
		this.m_moveFlag = false;							// 移動フラグ
		this.m_movingFlag = false;							// 移動中フラグ
		
		this.m_animationCounter = 0;						// アニメーションに使用するカウンター
		
		this.m_carryWeight = 0;								// らびたんが持っている商品の重さ
		
		this.originX = 32;									// 中心点を指定
		
		this.m_centerPointX;								// 画像の中心位置のX座標
		
		this.m_halfWidth = this.width * 0.5;				// 横幅の半分
		
		this.m_itemMgr = itemMgr;							// アイテムマネージャーを保持
		
		//----------------------------------------------------------------
		// シーンに追加
		//----------------------------------------------------------------
		addNode.insertBefore(this, addNodeBefore);
		
		//----------------------------------------------------------------
		// 重さバーのスプライト作成
		//----------------------------------------------------------------
		this.m_bar = new Sprite(0, 8);
		this.m_bar.image = g_game.assets[TEX_WEIGHT_BAR];
		this.m_bar.x = this.x;
		this.m_bar.y = this.y + this.height;
		addNode.insertBefore(this.m_bar, addNodeBefore);
			
		
		//===================================================================
		// 反転処理
		//===================================================================
		
		this.Flip = function()
		{
			if(this.flipFlag == true) 	this.scaleX = 1;
			else						this.scaleX = -1;
		}
		
		
		//===================================================================
		// 更新処理
		//===================================================================
		
		this.onenterframe = function()
		{
		
			// 商品の所持量によって移動速度を変える
			this.speed = LABI_MOVE_SPEED - LABI_MOVEDOWN_SPEED * (this.m_carryWeight / CARRY_WEIGHT_MAX);
			
			//-------------------------------------------------------------------
			// 移動フラグが立っていた場合、移動させる
			//-------------------------------------------------------------------
			if(this.m_moveFlag == true)
			{
				// 画像横幅の半分を引く
				this.movePointX -= this.m_halfWidth;
				
				// 等速で移動させるための処理
				this.tl.clear().
					moveX(this.movePointX, Math.abs(this.movePointX - this.x) / this.speed).
					then(function(){
						this.m_movingFlag = false;
						this.frame = 0;});
				
				
				// 反転の有無
				if(this.movePointX < this.x) this.flipFlag = true;
				if(this.movePointX > this.x) this.flipFlag = false;
				
				// 移動フラグを倒し、移動中フラグを立てる
				this.m_moveFlag = false;
				this.m_movingFlag = true;
			}
			
			//-------------------------------------------------------------------
			// 移動アニメーションをつける
			//-------------------------------------------------------------------
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
			
			//-------------------------------------------------------------------
			// あたり判定をとる
			//-------------------------------------------------------------------
			if((this.m_carryWeight < CARRY_WEIGHT_MAX) && this.m_itemMgr.Hit(this))
			{
				// 画像をゲットした画像に切り替える
				this.frame = GET_ANIMATION;
				this.tl.delay(GET_STOP_FRAME).then(function(){this.frame = 0;});
				
				// 所持重量を増やす
				this.m_carryWeight += 10;
				
				// 所持量を超えた場合、最大量に固定する
				if(this.m_carryWeight > CARRY_WEIGHT_MAX)
				{
					this.m_carryWeight = CARRY_WEIGHT_MAX;
				}				
			}
			
			
			// 画像中心位置を更新
			this.m_centerPointX = this.x + this.m_halfWidth;
			
			// 重さバーの更新
			this.m_bar.x = this.x;
			this.m_bar.width = WEIGHT_BAR_WIDTH * (this.m_carryWeight / CARRY_WEIGHT_MAX); 
			
			// 移動方向に応じて向きを変える
			this.Flip();
		}
	}
})
