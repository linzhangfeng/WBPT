var MJInfo = cc.Layer.extend(
    {
        tag: "MJInfo",
        _cur_time: 0,
        LeafCards_num: null,
        shengyuSp: null,
        dizhuSp: null,
        daojishi: null,
        diZhuNums: null,
        gdmj_quanfeng: null,
        directionSp: {},
        directionTip: [],
        ctor: function () {
            this._super();
            var size = cc.director.getWinSize();

            this.setIgnoreAnchorPointForPosition(false);
            this.setAnchorPoint(cc.p(0.0, 0.0));

            //底盘
            var info_bg = new cc.Sprite(getResPath("RoomMJ2/info/info_bg.png"));
            info_bg.setAnchorPoint(cc.p(0.5, 0.5));
            info_bg.setPosition(cc.p(size.width / 2, 310 + info_bg.getContentSize().height/2));
            this.addChild(info_bg);

            var leftCount = new cc.Sprite(getResPath("RoomMJ2/info/left_count.png"));
            leftCount.setAnchorPoint(cc.p(0, 0));
            leftCount.setPosition(cc.p(0, info_bg.getContentSize().height));
            info_bg.addChild(leftCount);

            //剩余牌数
            this.shengyuSp = new cc.Sprite(getResPath("RoomMJ2/info/left_count_bg.png"));
            this.shengyuSp.setAnchorPoint(cc.p(1, 0));
            this.shengyuSp.setPosition(cc.p(info_bg.getContentSize().width, info_bg.getContentSize().height - 5));
            info_bg.addChild(this.shengyuSp);

            //初始化东、北、西、南
            var d0 = new cc.Sprite(this.getDirectionSprite_hight(0));
            d0.setAnchorPoint(cc.p(0.5, 0));
            d0.setPosition(cc.p(info_bg.getContentSize().width / 2,5));
            d0.setName("dong");
            this.directionSp[d0.getName()] = d0;
            d0.setVisible(false);

            var tip0 = new cc.Sprite(this.getDirectionSprite_tip(0));
            tip0.setAnchorPoint(cc.p(0.5, 0));
            tip0.setPosition(cc.p(info_bg.getContentSize().width/2, 12));
            this.directionTip[0] = tip0;

            info_bg.addChild(d0);

            info_bg.addChild(tip0);

            var d1 = new cc.Sprite(this.getDirectionSprite_hight(1));
            d1.setAnchorPoint(cc.p(1, 0.5));
            d1.setPosition(cc.p(info_bg.getContentSize().width - 5,info_bg.getContentSize().height / 2));
            d1.setName("bei");
            this.directionSp[d1.getName()] = d1;
            d1.setVisible(false);

            var tip1 = new cc.Sprite(this.getDirectionSprite_tip(1));
            tip1.setAnchorPoint(cc.p(1, 0.5));
            tip1.setPosition(cc.p(info_bg.getContentSize().width - 12, info_bg.getContentSize().height/2));
            this.directionTip[1] = tip1;

            info_bg.addChild(d1);

            info_bg.addChild(tip1);

            var d2 = new cc.Sprite(this.getDirectionSprite_hight(2));
            d2.setAnchorPoint(cc.p(0.5, 1));
            d2.setPosition(cc.p(info_bg.getContentSize().width / 2,info_bg.getContentSize().height - 5));
            d2.setName("xi");
            this.directionSp[d2.getName()] = d2;
            d2.setVisible(false);


            var tip2 = new cc.Sprite(this.getDirectionSprite_tip(2));
            tip2.setAnchorPoint(cc.p(0.5, 1));
            tip2.setPosition(cc.p(info_bg.getContentSize().width/2, info_bg.getContentSize().height - 12));
            this.directionTip[2] = tip2;

            info_bg.addChild(d2);

            info_bg.addChild(tip2);



            var d3 = new cc.Sprite(this.getDirectionSprite_hight(3));
            d3.setAnchorPoint(cc.p(0, 0.5));
            d3.setPosition(cc.p(5,info_bg.getContentSize().height / 2));
            d3.setName("nan");
            this.directionSp[d3.getName()] = d3;
            d3.setVisible(false);

            var tip3 = new cc.Sprite(this.getDirectionSprite_tip(3));
            tip3.setAnchorPoint(cc.p(0, 0.5));
            tip3.setPosition(cc.p(12, info_bg.getContentSize().height/2));
            this.directionTip[3] = tip3;

            info_bg.addChild(d3);

            info_bg.addChild(tip3);

            this.daojishi = new cc.LabelAtlas("30", getResPath("RoomMJ2/info/time_num.png"), 22, 37, '0');
            this.daojishi.setAnchorPoint(cc.p(0.5, 0.5));
            this.daojishi.setPosition(cc.p(info_bg.getContentSize().width/2 , info_bg.getContentSize().height/2));
            info_bg.addChild(this.daojishi);

            var yellowCircle = new cc.Sprite(getResPath("RoomMJ2/info/yellow_circle.png"));
            yellowCircle.setPosition(cc.p(info_bg.getContentSize().width/2 , info_bg.getContentSize().height/2));
            info_bg.addChild(yellowCircle);

            this.showDirectionTip(-1);

        },
        showCardNums: function (nums) {
            if(!this.LeafCards_num)
            {
                this.LeafCards_num = new ccui.Text(__String.createWithFormat("%1",nums), "fonts/arialbd.ttf", 20);
                this.LeafCards_num.setTextColor(cc.color(254,234,115,255));
                this.LeafCards_num.setAnchorPoint(cc.p(0.5, 0.5));
                this.LeafCards_num.setPosition(cc.p(this.shengyuSp.getContentSize().width/2 , this.shengyuSp.getContentSize().height/2));
                this.shengyuSp.addChild(this.LeafCards_num);
            }
            else
            {
                this.LeafCards_num.setString(__String.createWithFormat("%1",nums));
            }

        },
        showTime: function (time) {
            this._cur_time = time;
            this.daojishi.setVisible(true);
            this.daojishi.setString(__String.createWithFormat("%1",this._cur_time));
            if (MJModel.isOnVideo)return;
            if(this._cur_time > 0)this.schedule(this.updateTime, 1.0);
        },
        stopTime: function () {
            cc.log("stopTime.....");
            this.unschedule(this.updateTime);
            if (this.daojishi)
            {
                this.daojishi.setString("0");
            }
        },
        setBaseMoney: function (bm) {
        },
        showDirectionTip: function (pos) {
            this.hideAllDiretionTip();
            if(pos == 0)
            {
                this.directionSp["dong"].setVisible(true);
                this.directionSp["dong"].runAction(cc.RepeatForever.create(cc.Sequence.create(new cc.FadeIn(0.3), new cc.FadeOut(0.5))));
            }else if(pos == 1)
            {
                this.directionSp["bei"].setVisible(true);
                this.directionSp["bei"].runAction(cc.RepeatForever.create(cc.Sequence.create(new cc.FadeIn(0.3), new cc.FadeOut(0.5))));

            }else if(pos == 2)
            {
                this.directionSp["xi"].setVisible(true);
                this.directionSp["xi"].runAction(cc.RepeatForever.create(cc.Sequence.create(new cc.FadeIn(0.3), new cc.FadeOut(0.5))));
            }else if(pos == 3)
            {
                this.directionSp["nan"].setVisible(true);
                this.directionSp["nan"].runAction(cc.RepeatForever.create(cc.Sequence.create(new cc.FadeIn(0.3), new cc.FadeOut(0.5))));
            }

        },

        hideAllDiretionTip: function () {
            this.directionSp["dong"].setVisible(false);
            this.directionSp["dong"].stopAllActions();
            this.directionTip[0].setTexture(this.getDirectionSprite_tip(0));

            this.directionSp["bei"].setVisible(false);
            this.directionSp["bei"].stopAllActions();
            this.directionTip[1].setTexture(this.getDirectionSprite_tip(1));

            this.directionSp["xi"].setVisible(false);
            this.directionSp["xi"].stopAllActions();
            this.directionTip[2].setTexture(this.getDirectionSprite_tip(2));

            this.directionSp["nan"].setVisible(false);
            this.directionSp["nan"].stopAllActions();
            this.directionTip[3].setTexture(this.getDirectionSprite_tip(3));
        },
        reflashDirection: function () {
            this.hideAllDiretionTip();
        },

        gdmj_showFengQuan: function (seatid) {
            if (seatid == -1)
            {
                if(this.gdmj_quanfeng)this.gdmj_quanfeng.setVisible(false);
                return;
            }

            if (MJModel.gdmj_playid == 1) return;//推倒胡 没有风

            var t = __String.createWithFormat(getResPath("RoomMJ/gdmj/zhuafeng/result-%1.png"),seatid);
            if (this.gdmj_quanfeng == null)
            {
                var size = cc.director.getWinSize();
                this.gdmj_quanfeng = new cc.Sprite(t);
                this.gdmj_quanfeng.setPosition(cc.p(61+10, size.height - 106 - 60+45));
                this.addChild(this.gdmj_quanfeng);
            }
            else
            {
                this.gdmj_quanfeng.setVisible(true);
                this.gdmj_quanfeng.setTexture(t);
            }

        },

        updateTime: function (dt) {
            if(this._cur_time > 0)
            {
                this._cur_time = this._cur_time - 1;
                this.daojishi.setString(__String.createWithFormat("%1",this._cur_time));
                if(this._cur_time == 0){
                    this.stopTime();
                }

                if (MJModel.cur_seat == MJModel.seatid)
                {
                    if (this._cur_time < 3)
                    {
                        Sound.getInstance().playEffect("Sound/time_click.mp3");
                    }

                    if (this._cur_time == 3)
                    {
                        PlatformHelper.vibrate();
                    }
                }
            }
        },

        getDirectionSprite_tip: function (pos) {
            var beginSeatid = MJModel.getPosBySeatid(0);
            return __String.createWithFormat(getResPath("RoomMJ2/info/seatid_%1/%2_0.png"),beginSeatid,pos);
        },
        getDirectionSprite_hight: function (pos) {
            return __String.createWithFormat(getResPath("RoomMJ2/info/direction_bg/%1_1.png"),pos);
        },
        getDirectionSprite_hight_light: function (pos) {
            return __String.createWithFormat(getResPath("RoomMJ2/info/direction_bg/%1_2.png"),pos);
        },//弹幕
    });
