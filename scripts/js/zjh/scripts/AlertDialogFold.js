/**
 * Created by sherard on 2017/1/19.
 */

var AlertDialogFold = {

    ALERT_DIALOG_BUTTON_CONFIRM: 20,
    ALERT_DIALOG_BUTTON_CANCEL: 21,
    ALERT_DIALOG_BUTTON_CLOSE: 22,

    m_pCallback: null,


    ctor: function () {
        this.m_pCallback = null;

        var mask = new cc.LayerColor(cc.color(0, 0, 0, 100));
        this.addChild(mask);

        var size = cc.director.getWinSize();

        var bgSize = cc.size(598 + 34, 450);
        var dialog = new cc.Sprite();
        dialog.setContentSize(cc.size(bgSize.width, bgSize.height));

        var sp = new cc.Scale9Sprite(getResPath("Room/dialog/v_dialog_info_bg.9.png"));
        sp.setAnchorPoint(cc.p(0.5, 0.5));
        sp.setPosition(bg.getContentSize() / 2);
        sp.setContentSize(cc.size(width, height));
        dialog.addChild(sp);
        dialog.setPosition(winSize / 2);
        dialog.setName("dialog");
        this.addChild(dialog, 100);

        var titleSp = new cc.Sprite(getResPath("Room/dialog/v_dialog_title_bg.png"));
        titleSp.setPosition(cc.p(bgSize.width / 2, bgSize.height - 60));
        dialog.addChild(titleSp);

        var bottomSp = new cc.Sprite(getResPath("Room/dialog/v_alert_dialog_bottom_bg.png"));
        bottomSp.setPosition(cc.p(bgSize.width / 2, 54));
        dialog.addChild(bottomSp);

        var titleLabel = new cc.LabelTTF("弃牌提示", "Arial", 36);
        titleLabel.setPosition(titleSp.getPosition());
        dialog.addChild(titleLabel);

        var contentLabel = new cc.LabelTTF("建议先看牌再弃牌", "Arial", 32);
        contentLabel.setPosition(cc.p(bgSize.width / 2, 280));
        if (!RoomZJHInstance.IsMySee) {
            dialog.addChild(contentLabel);
        }


        var foldTip = sys.localStorage.getItem("fold_tip");
        var switchBox = new ccui.CheckBox()(getResPath("Room/dialog/dialog_checkbox_normal.png"), getResPath("Room/dialog/dialog_checkbox_selected.png"));
        switchBox.setPosition(cc.p(200, 215));
        switchBox.setSelected(!foldTip);
        switchBox.addEventListener(function (ref, type) {
            if (type == ccui.CheckBox.EVENT_SELECTED) {
                sys.localStorage.setItem("fold_tip", false);
            }
            else if (type == ccui.CheckBox.EVENT_UNSELECTED) {
                sys.localStorage.setItem("fold_tip", false);
            }
        });
        dialog.addChild(switchBox);

        var tipLabel = new cc.LabelTTF("以后不需要这个提示", "Arial", 24);
        tipLabel.setAnchorPoint(cc.p(0, 0.5));
        tipLabel.setPosition(cc.p(switchBox.getPositionX() + 35, switchBox.getPositionY()));
        dialog.addChild(tipLabel);

        var confirmBtn = new ccui.Button(getResPath("Room/dialog/v_alert_dialog_btn_green.png"), getResPath("Room/dialog/v_alert_dialog_btn_green_press.png"));
        confirmBtn.setPosition(cc.p(bgSize.width * 0.25, 130));
        confirmBtn.setName("fold_confirm");
        confirmBtn.addClickEventListener(this.buttonClicked);
        confirmBtn.setTitleText("确认弃牌");
        confirmBtn.setTitleFontSize(32);
        dialog.addChild(confirmBtn);

        var cancelBtn = new ccui.Button(getResPath("Room/dialog/v_alert_dialog_btn_red.png"), getResPath("Room/dialog/v_alert_dialog_btn_red_press.png"));
        cancelBtn.setPosition(cc.p(bgSize.width * 0.75, confirmBtn.getPositionY()));
        cancelBtn.setName("fold_cancel");
        cancelBtn.addClickEventListener(this.buttonClicked);
        cancelBtn.setTitleText("取消弃牌");
        cancelBtn.setTitleFontSize(32);
        dialog.addChild(cancelBtn);

        var closeBtn = new ccui.Button(getResPath("Room/dialog/v_dialog_close.png"), getResPath("Room/dialog/v_dialog_close_press.png"));
        closeBtn.setPosition(cc.p(bgSize.width - 55, bgSize.height - 55));
        closeBtn.setTag(this.ALERT_DIALOG_BUTTON_CLOSE);
        closeBtn.addClickEventListener(this.buttonClicked);
        dialog.addChild(closeBtn);

        var touchListener = new cc.EventListenerTouchOneByOne();
        touchListener.onTouchBegan = this.onTouchBegan;
        touchListener.setSwallowTouches(true);
        cc.director.getEventDispatcher().addEventListenerWithSceneGraphPriority(touchListener, this);
    },

    buttonClicked: function (sender) {
        var btn = sender;
        if (btn != null) {
            var name = btn.getName();

            if (this.m_pCallback != null) {
                this.m_pCallback(name);
            }
            this.removeFromParentAndCleanup(true);
        }
    },

    onTouchBegan: function (touch, event) {
        var dialog = this.getChildByName("dialog")
        if (!dialog.getBoundingBox().containsPoint(touch.getLocation())) {
            this.removeFromParentAndCleanup(true);
            event.stopPropagation();
        }
    }

};