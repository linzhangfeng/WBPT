var PHZPlayerChat = cc.Node.extend({
    ctor: function () {
        this._super();
    },
    initChat: function (pos, content, flippedX, flippedY) {
        var imageView = new ccui.ImageView("phz/icon_chat_020.png",1);
        imageView.setScale9Enabled(true);
        imageView.setCapInsets(cc.rect(40, 25, 18, 13));
        imageView.setAnchorPoint(cc.p(0.0, 0.5));
        this.addChild(imageView);
        if (flippedX)imageView.setScaleY(-1);
        if (flippedY)imageView.setScaleX(-1);

        var isSingleLine = true;
        var text = new cc.LabelTTF(content, "Thonburi", 18);
        if (text.getContentSize().width > 140){
            text = new cc.LabelTTF(content, "Thonburi", 18, cc.size(140,0));
            isSingleLine = false;
        }

        text.setColor(cc.color(50, 50, 50));
        text.setAnchorPoint(cc.p(0.0, 0.5));
        this.addChild(text);

        var textSize = text.getContentSize();
        imageView.setContentSize(cc.size(textSize.width + 50, isSingleLine ? textSize.height + 35 : textSize.height + 27));

        var tx = 33;
        var ty = 4;
        if (flippedX){
            ty = -4;
        }

        if (flippedY){
            text.setAnchorPoint(cc.p(1.0, 0.5));
            tx = -33;
        }
        text.setPosition(cc.p(tx, ty));
        this.setContentSize(imageView.getContentSize());
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.0, 0.5));
    },
    initSound: function (pos, filepath, flippedX, flippedY, time) {
        var imageView = new ccui.ImageView("phz/icon_chat_021.png",1);
        imageView.setScale9Enabled(true);
        imageView.setCapInsets(cc.rect(40, 25, 18, 13));
        imageView.setAnchorPoint(cc.p(0.0, 0.5));
        this.addChild(imageView);
        //if (flippedX)imageView.setFlippedX(true);
        //if (flippedY)imageView.setFlippedY(true);
        if (flippedX)imageView.setScaleY(-1);
        if (flippedY)imageView.setScaleX(-1);

        var text = new cc.Sprite(utils.spriteFrame("phz/icon_chat_003.png"));
        text.setAnchorPoint(cc.p(0.0, 0.5));
        text.setScale(-0.2);
        this.addChild(text);

        var animation = utils.newAni1("phz/icon_chat_00",[1,2,3],0.2,0);
        text.runAction(cc.repeatForever(animation) );

        var textSize = text.getBoundingBox();
        imageView.setContentSize(cc.size(textSize.width + 70, textSize.height + 40));

        var tx = 70;
        var ty = 4;
        if (flippedX){
            ty = -4;
        }

        if (flippedY){
            text.setAnchorPoint(cc.p(1.0, 0.5));
            tx = -40;
            text.setScale(0.2);
        }
        text.setPosition(cc.p(tx, ty));
        this.setContentSize(imageView.getContentSize());
        this.ignoreAnchorPointForPosition(false);
        this.setAnchorPoint(cc.p(0.0, 0.5));
    }
});