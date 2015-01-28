Item = Class.create(Sprite,{
	// コンストラクタ
	initialize:function(addGroup, graphNumber, itemMgr)
	{
		Sprite.call(this, ITEM_SIZE, ITEM_SIZE);			// スプライトの初期化
		this.image = g_game.assets[TEX_ITEM_GRAPHS];		// アイテム画像指定
		this.frame = graphNumber;							// 表示アイテム指定
		this.speed = Math.random() * 3.0 + 1.0;				// 落下スピードを初期化
		this.m_itemMgr = itemMgr;							// アイテム管理
		this.m_removeFlag = false;							// 削除フラグ
		this.m_price = 0;
		this.m_weight = 0;
		
		//--------------------------------------------------------------
		// グラフィックナンバーによって商品を変更する
		//--------------------------------------------------------------
		switch(graphNumber)
		{
		case ITEM_DVD:
			this.m_price = ITEM_PRICE_DVD;
			break;
			
		case ITEM_DRIER:
			this.m_price = ITEM_PRICE_DRIER;
			break;
			
		case ITEM_HEADPHONE:
			this.m_price = ITEM_PRICE_HEADPHONE;
			break;
			
		case ITEM_TABLET:
			this.m_price = ITEM_PRICE_TABLET;
			break;
			
		case ITEM_DISICAMERA:
			this.m_price = ITEM_PRICE_DISICAMERA;
			break;
			
		case ITEM_3DPRINTER:
			this.m_price = ITEM_PRICE_3DPRINTER;
			break;
		}
		
		// 指定シーンに追加
		addGroup.addChild(this);							
		
		// 出現位置を指定
		this.x = Math.random() * (GAME_SIZE_WIDTH - this.width - BOX_WIDTH);
		this.y = -this.height;
		
		// 画像の中心を表す
		this.m_centerX = this.x + this.width * 0.5;
		this.m_centerY = this.y + this.height * 0.5;
		
		//--------------------------------------------------------------
		// 床に落ちるアクションをつける  
		//--------------------------------------------------------------
		this.tl.moveY(ITEM_BREAK_Y, ITEM_BREAK_Y / this.speed).
				then(function(){this.m_removeFlag = true;}).
				cue({
					0 : function(){this.opacity = 0;},
					5 : function(){this.opacity = 1;},
					10: function(){this.opacity = 0;},
					15: function(){this.opacity = 1;},
					20: function(){DeleteThis(this);}});
	}	
})
