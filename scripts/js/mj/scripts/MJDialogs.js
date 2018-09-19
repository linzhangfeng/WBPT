var MJUserInfo = cc.Layer.extend(
    {
        tag: "MJUserInfo",
        ctor: function () {
            this._super();
        },
        init: function () {

        },
    });

var BigFaceMenu = cc.Layer.extend({

    tag: "BigFaceMenu",
    m_pCallback: null,
    ctor: function () {
        this._super();
        var size = cc.director.getWinSize();
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.0, 0.0));
        this.setContentSize(cc.size(size.width, size.height));
    },
    onEnter: function () {
        this._super();
        // 部分事件监听
        var self = this;
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
    },
    setCallback: function (callback) {
        this.m_pCallback = callback;
    },
    onTouchBegan: function (touch, event) {
        this.hide();
        event.stopPropagation();
        return true;
    },
    show: function (rotation) {

        var self = this;
        var radius = 150;
        var M_PI = 3.14159;
        var offsets =
                [
                    cc.p(radius * Math.cos((-90.0 + rotation) / 180.0 * M_PI), radius * Math.sin((-90.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((-60.0 + rotation) / 180.0 * M_PI), radius * Math.sin((-60.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((-30.0 + rotation) / 180.0 * M_PI), radius * Math.sin((-30.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((0.0 + rotation) / 180.0 * M_PI), radius * Math.sin((0.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((30.0 + rotation) / 180.0 * M_PI), radius * Math.sin((30.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((60.0 + rotation) / 180.0 * M_PI), radius * Math.sin((60.0 + rotation) / 180.0 * M_PI)),
                    cc.p(radius * Math.cos((90.0 + rotation) / 180.0 * M_PI), radius * Math.sin((90.0 + rotation) / 180.0 * M_PI)),
                ]
            ;

        var icons =
                [
                    "Room/bigface/flower/flower1.png",
                    "Room/bigface/cheers/cheers1.png",
                    "Room/bigface/kiss/kiss1.png",
                    "Room/bigface/eggs/egg1.png",
                    "Room/bigface/shoe/shoe1.png",
                    "Room/bigface/bomb/bomb1.png",
                    "RoomPublic/facechat_text.png",
                ]
            ;

        for (var i = 0; i < 7; i++) {
            var btn = new ccui.Button();
            btn.loadTextures("RoomPublic/facechat1.png", "RoomPublic/facechat2.png", "");
            btn.setPosition(offsets[i]);
            btn.setTag(i);
            btn.addClickEventListener(function (r) {
                if (self.m_pCallback) self.m_pCallback(r.getTag());
                self.removeFromParent(true);
            });
            this.addChild(btn);

//        if (i != 6)
            {
                var img = new cc.Sprite(icons[i]);
                img.setPosition(cc.p(btn.getContentSize().width / 2, btn.getContentSize().height / 2));
                if (i != 6) img.setScale(0.7);
                btn.addChild(img);
            }
        }

        this.setScale(0);
        this.runAction(cc.scaleTo(0.1, 1));
    },

    hide: function () {
        this.runAction(cc.sequence(cc.scaleTo(0.1, 0), cc.removeSelf()));
    }
});


var MJPlayerQueue = cc.Layer.extend({
    tag: "MJPlayerQueue",
    seatid: null,
    roomid: null,
    listView: null,
    ctor: function () {
        this._super();
        cc.log("ooooooooooooooops");
        var self = this;
        var winSize = cc.director.getWinSize();
        var contentSize = cc.size(300, 450);

        var contentBg = new cc.Scale9Sprite("RoomPublic/common/popup_window.png", cc.rect(20, 20, 160, 160));
        contentBg.setContentSize(contentSize);
        contentBg.setName("content_bg");
        this.addChild(contentBg);
        contentBg.setPosition(cc.p(winSize.width / 2, winSize.height / 2));

        var closeBtn = new ccui.Button("Public/cancel_icon_yellow.png", "Public/cancel_icon_yellow_check.png");
        closeBtn.setAnchorPoint(cc.p(0.5, 0.5));
        closeBtn.setPosition(cc.p(contentSize.width, contentSize.height));
        closeBtn.setName("closeBtn");
        contentBg.addChild(closeBtn);

//        closeBtn.addClickEventListener(CC_CALLBACK_0(RoomPlayerQueue::closeCallBack, this));
        closeBtn.addClickEventListener(function (sender, evt) {
            self.btnCallback(sender);
        });

        var title = new cc.LabelTTF("排队列表", "Thonburi", 30);
        title.setAnchorPoint(cc.p(0.5, 0.5));
        title.setPosition(cc.p(contentSize.width / 2, contentSize.height - 24));
        title.setColor(cc.color(240, 203, 91, 255));
        contentBg.addChild(title);

        var yellowBtn = new ccui.Button("Public/btn_yellow.png", "Public/btn_yellow.png", "Public/btn_yellow.png");
        yellowBtn.setScale9Enabled(true);
        yellowBtn.setContentSize(cc.size(290, 52));
        yellowBtn.setPosition(cc.p(contentSize.width / 2, 30));
        yellowBtn.setName("joinup");
        contentBg.addChild(yellowBtn, 1000);
        yellowBtn.setTitleText("加入队列");
        yellowBtn.setTitleFontSize(40);
        yellowBtn.setTitleColor(cc.color(95, 62, 19, 255));
        yellowBtn.setVisible(false);
        yellowBtn.setTitleFontName("AmericanTypewriter");
        yellowBtn.addClickEventListener(function (sender) {
            sendUpTable();
        });

        var redBtn = new ccui.Button("Public/btn_red.png", "Public/btn_red.png", "Public/btn_red.png");
        redBtn.setScale9Enabled(true);
        redBtn.setContentSize(cc.size(290, 52));
        redBtn.setPosition(cc.p(contentSize.width / 2, 30));
        redBtn.setName("outqueue");
        contentBg.addChild(redBtn, 1000);
        redBtn.setVisible(false);
        redBtn.setTitleText("退出队列");
        redBtn.setTitleFontSize(40);
        redBtn.setTitleColor(cc.color(229, 229, 229, 255));
        redBtn.setTitleFontName("AmericanTypewriter");

        redBtn.addClickEventListener(function (sender) {
            sendDownTable();
        });

        listView = new ccui.ListView();
        listView.setDirection(ccui.ScrollView.DIR_VERTICAL);
        listView.setContentSize(cc.size(270, 345));
        listView.setItemsMargin(15);
        listView.setPosition(cc.p(10, 30));
        listView.setBounceEnabled(false);
        listView.setAnchorPoint(cc.p(0, 0));
        listView.setScrollBarEnabled(false);
        contentBg.addChild(listView, 100);
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                return self.onTouchBegan(touch, event);
            }
        });
        if (listener)cc.eventManager.addListener(listener, this);
        return true;
    },

    updateButton: function (status) {
        var contentSize = cc.size(300, 450);

        var contentBg = this.getChildByName("content_bg");
        var yellowBtn = contentBg.getChildByName("joinup");
        var redBtn = contentBg.getChildByName("outqueue");

        yellowBtn.setVisible(status == 3);
        redBtn.setVisible(status == 0 || status == 1);

        if (status == 2) {
            listView.setContentSize(cc.size(270, 400));
            listView.setPosition(cc.p(10, 4));
        } else {
            listView.setContentSize(cc.size(270, 345));
            listView.setPosition(cc.p(10, 30));
        }
    },
    setSeatId: function (id) {
        seatid = id;
    },
    setRoomId: function (id) {
        roomid = id;
    },

    updateView: function (data) {

        //Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
        var tableInfo = parsePacket("proto.login.AckUpTableInfo_UC", data);

        listView.removeAllItems();
        var itemSize = cc.size(270, 70);

        Log(this,"tableInfo.players.length-----------"+tableInfo.players.length);
        for (var i = 0; i < tableInfo.players.length; i++) {

            var player = tableInfo.players[i];

            var textColor = cc.color(225, 205, 173, 255);
            if (player.seatid == seatid) {
                textColor = cc.color(240, 203, 91, 255);
            }

            var layout = new ccui.Layout();
            layout.setContentSize(itemSize);

            var item = new ccui.Widget();
            item.setContentSize(itemSize);
            item.setColor(cc.color(51, 51, 51));
            item.setAnchorPoint(cc.p(0, 0.5));
            item.setPosition(cc.p(0, itemSize.height / 2));
            layout.addChild(item);

            var bg = new cc.LayerColor(cc.color(51, 51, 51, 255));
            bg.setContentSize(itemSize);
            item.addChild(bg);


            var indexTex = new cc.LabelTTF("" + i + 1, "AmericanTypewriter", 18);
            indexTex.setAnchorPoint(cc.p(1.0, 0.5));
            indexTex.setPosition(cc.p(40, itemSize.height / 2));
            indexTex.setColor(textColor);
            item.addChild(indexTex);

            var m_pAvatar = Utils.createCircleAvatar(player.avatar, "Avatars/user4_unlogin.png", getResPath("RoomMJ2/head_bg.png"), cc.size(68, 68));
            m_pAvatar.setPosition(cc.p(90, itemSize.height / 2));
            m_pAvatar.setScale(0.6);
            item.addChild(m_pAvatar);

            var nameText = new cc.LabelTTF(player.name, "AmericanTypewriter", 25);
            nameText.setAnchorPoint(cc.p(0.0, 0.5));
            nameText.setPosition(cc.p(130, itemSize.height / 2));
            nameText.setColor(textColor);
            item.addChild(nameText);

            listView.addChild(layout);
        }
        listView.refreshView();
    },

    btnCallback: function (ref) {
        var n = ref;
        var name = n.getName();
        var self = this;
        //Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
        cc.log("lllllllll001");
        if (name == "closeBtn") {
            cc.eventManager.pauseTarget(this, false);
            this.setVisible(false);
            cc.log("lllllllll002");
        }
    },

    onTouchBegan: function (touch, event) {
        var contentBg = this.getChildByName("content_bg");
        if (contentBg.getBoundingBox().containsPoint(this.convertToNodeSpace(touch.getLocation()))) {
        }
        else {
            //Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
            cc.eventManager.pauseTarget(this, false);
            this.setVisible(false);
        }
        return true;
    },
});