var MJPlayer = cc.Layer.extend(
    {
        tag: "MJPlayer",
        m_pMoney: null,
        m_size: null,
        m_pAvatar: null,
        pid: -1,
        uid: 0,
        seatid: -1,
        sec: 0,
        callback: null,
        clickCallback: null,
        banker_flag: null,
        ready_flag: null,
        lack_flag: null,
        piao_flag: null,
        ga_flag: null,
        maima_icon: null,
        maima_nums: null,
        offline_flag: null,
        timeount_flag: null,
        player_black: null,
        fengwei_flag: null,
        huapai_flag: null,
        huapai_nums: null,
        curHuaPaiNums: 0,
        ctor: function () {
            this._super();
            this.ignoreAnchorPointForPosition(false);
            this.setAnchorPoint(cc.p(0.5, 0.5));
        },
        login: function (seatid) {
            this.seatid = seatid;
            var player = MJModel.players[this.seatid];
            if (!player)return;
            this.uid = player.uid;
            this.m_pMoney.setString(player.money - MJModel.take_in);

            if (this.m_pAvatar) this.m_pAvatar.removeFromParent(true);
            this.m_pAvatar = Utils.createCircleAvatar(player.avatar, "Avatars/user4_unlogin.png", getResPath("RoomMJ2/head_bg.png"), cc.size(68, 68));
            this.m_pAvatar.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height - 45));
            this.addChild(this.m_pAvatar);

            this.setVisible(true);
            this.setPosition(MJConfig.getPlayerPos(this.pid));

            this.setBankerFlag(false);
            this.setReadyFlag(false);
            this.setTimeOut(false);
            this.setOffline(false);
            this.setMaiMa(0);
        },

        logout: function () {
            this.uid = 0;
            this.setVisible(false);
        },

        setLackType: function (lack) {
            this.lack_flag.setVisible(true);
            if (lack == MJ_TYPE.MJ_TYPE_TIAO) {
                this.lack_flag.setTexture(getResPath("RoomMJ2/tiao_icon.png"));
            }
            else if (lack == MJ_TYPE.MJ_TYPE_TONG) {
                this.lack_flag.setTexture(getResPath("RoomMJ2/tong_icon.png"));
            }
            else if (lack == MJ_TYPE.MJ_TYPE_WAN) {
                this.lack_flag.setTexture(getResPath("RoomMJ2/wan_icon.png"));
            }
            else {
                this.lack_flag.setVisible(false);

            }
        },

        setPiaoType: function (piao) {
            this.piao_flag.setVisible(true);
            var pos = MJModel.getPosBySeatid(this.seatid);
            if (pos == 2) {
                this.piao_flag.setPosition(cc.p(this.m_size.width + this.piao_flag.getContentSize().width / 2 - 5, this.lack_flag.getContentSize().height + this.piao_flag.getContentSize().height / 2 - 8));
            } else {
                this.piao_flag.setPosition(cc.p(this.m_size.width / 2, this.m_size.height + this.piao_flag.getContentSize().height / 2));
            }
            if (piao == 1) {
                this.piao_flag.setTexture(getResPath("RoomMJ/scmj/piao1_tip.png"));
            } else if (piao == 2) {
                this.piao_flag.setTexture(getResPath("RoomMJ/scmj/piao2_tip.png"));
            } else if (piao == 4) {
                this.piao_flag.setTexture(getResPath("RoomMJ/hunan/piao4_tip.png"));
            } else if (piao == 5) {
                this.piao_flag.setTexture(getResPath("RoomMJ/scmj/piao5_tip.png"));
            } else if (piao == 6) {
                this.piao_flag.setTexture(getResPath("RoomMJ/hunan/piao6_tip.png"));
            } else {
                this.piao_flag.setVisible(false);
            }
        },

        setGaType: function (piao) {
            this.ga_flag.setVisible(true);
            var pos = MJModel.getPosBySeatid(this.seatid);
            if (pos == 2) {
                this.ga_flag.setPosition(cc.p(this.m_size.width + this.ga_flag.getContentSize().width / 2 - 5, this.ga_flag.getContentSize().height / 2));
            } else {
                this.ga_flag.setPosition(cc.p(this.m_size.width / 2, this.m_size.height + this.ga_flag.getContentSize().height / 2));
            }
            if (piao == 1) {
                this.ga_flag.setTexture(getResPath("RoomMJ/hnmj/ga1.png"));
            } else if (piao == 2) {
                this.ga_flag.setTexture(getResPath("RoomMJ/hnmj/ga2.png"));
            } else if(piao == 3){
                this.ga_flag.setTexture(getResPath("RoomMJ/hnmj/ga3.png"));
            }else if (piao == 5) {
                this.ga_flag.setTexture(getResPath("RoomMJ/hnmj/ga5.png"));
            } else {
                this.ga_flag.setVisible(false);
            }
        },

        reset: function () {
            this.setBankerFlag(false);
            this.setReadyFlag(false);
            this.setMaiMa(0);
            this.setTimeOut(false);
            this.setOffline(false);
            this.setLackType(MJ_TYPE.MJ_TYPE_EMPTY);
            this.setFengWei(-1);
            this.setPiaoType(-1);
            this.setHuaPaiNum(0);
        },

        setId: function (id) {
            this.pid = id;
            var btn = new ccui.Button(getResPath("RoomMJ2/player_bg.png"));
            btn.setZoomScale(0);
            btn.setPosition(cc.p(btn.getContentSize().width / 2, btn.getContentSize().height / 2));
            btn.setTag(5);
            var self = this;
            btn.addClickEventListener(function (sender, evt) {
                self.playerClickCallback(self);
            });
            this.addChild(btn, -1);

            var headBg = new cc.Sprite(getResPath("RoomMJ2/head_bg.png"));
            headBg.setPosition(cc.p(btn.getContentSize().width / 2, btn.getContentSize().height - 8 - headBg.getContentSize().height / 2));
            btn.addChild(headBg);

            this.m_size = btn.getContentSize();
            this.setContentSize(this.m_size);

            this.m_pMoney = new cc.LabelTTF("0", "Thonburi", 20);
            this.m_pMoney.setPosition(cc.p(this.m_size.width / 2, 20));
            this.m_pMoney.setColor(cc.color(255, 255, 255));
            this.addChild(this.m_pMoney);

            this.banker_flag = new cc.Sprite(getResPath("RoomMJ/banker.png"));
            this.banker_flag.setVisible(false);
            this.banker_flag.setPosition(cc.p(this.m_size.width - this.banker_flag.getContentSize().width / 2 + 10, this.m_size.height - this.banker_flag.getContentSize().height / 2 + 5));
            this.addChild(this.banker_flag, 100);

            this.lack_flag = new cc.Sprite(getResPath("RoomMJ2/tiao_icon.png"));
            this.lack_flag.setAnchorPoint(cc.p(0.5, 0.5));
            if(id == 1){
                this.lack_flag.setPosition(cc.p(-this.lack_flag.getContentSize().width / 2, this.lack_flag.getContentSize().height / 2));
            }else{
                this.lack_flag.setPosition(cc.p(this.m_size.width + this.lack_flag.getContentSize().width / 2, this.lack_flag.getContentSize().height / 2));
            }
            this.lack_flag.setVisible(false);
            this.addChild(this.lack_flag, 100);

            this.piao_flag = new cc.Sprite(getResPath("RoomMJ/scmj/piao1_tip.png"));
            this.piao_flag.setVisible(false);
            this.addChild(this.piao_flag, 100);

            this.ga_flag = new cc.Sprite(getResPath("RoomMJ/hnmj/ga1.png"));
            this.ga_flag.setVisible(false);
            this.addChild(this.ga_flag, 100);

            this.ready_flag = new cc.Sprite(getResPath("RoomMJ/ready.png"));
            this.ready_flag.setVisible(false);
            if (id == 1) {
                this.ready_flag.setPosition(cc.p(0, this.m_size.height - 10));
            }
            else {
                this.ready_flag.setPosition(cc.p(this.m_size.width - 10, this.m_size.height - 10));
            }

            this.addChild(this.ready_flag, 100);


            this.player_black = new cc.Sprite(getResPath("RoomMJ/player_black.png"));
            this.player_black.setVisible(false);
            this.player_black.setPosition(cc.p(this.m_size.width / 2, this.m_size.height / 2));
            this.addChild(this.player_black, 99);

            this.offline_flag = new cc.Sprite(getResPath("RoomMJ/offline.png"));
            this.offline_flag.setVisible(false);
            this.offline_flag.setPosition(cc.p(this.m_size.width / 2, 64));
            this.addChild(this.offline_flag, 100);

            this.timeount_flag = new cc.Sprite(getResPath("RoomMJ/timeover.png"));
            this.timeount_flag.setVisible(false);
            this.timeount_flag.setPosition(cc.p(this.m_size.width / 2, 64));
            this.addChild(this.timeount_flag, 100);
        },

        playerClickCallback: function (ref) {
            if (this.clickCallback) {
                this.clickCallback(this.pid);
            }
        },

        setClickCallback: function (callback) {
            this.clickCallback = callback;
        },

        showBalanceNumber: function (value) {
            if (value == 0) {
                return;
            }

            var label = null;
            if (value > 0) {
                label = new cc.LabelAtlas(":" + value, "Room/duizhan/dz_balance_num_win.png", 28, 40, '0');
            } else {
                label = new cc.LabelAtlas(":" + Math.abs(value), "Room/duizhan/dz_balance_num_lose.png", 28, 40, '0');
            }
            label.setAnchorPoint(cc.p(0.5, 0.5));
            if (this.pid == 2) {
                label.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height / 2));
            }
            else {
                label.setPosition(cc.p(this.getContentSize().width / 2, this.getContentSize().height - 10));
            }

            this.addChild(label, 200);

            label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)), cc.delayTime(3),
                cc.removeSelf()));
        },

        updateInfo: function () {
            var player = MJModel.players[this.seatid];
            if (!player)return;

            this.m_pMoney.setString(player.money - MJModel.take_in);
        },

        setBankerFlag: function (v) {
            if (this.banker_flag) this.banker_flag.setVisible(v);
        },

        setReadyFlag: function (v) {
            if (this.ready_flag) this.ready_flag.setVisible(v);
        },

        setMaiMa: function (nums) {
        },

        setOffline: function (v) {
            if (this.offline_flag) this.offline_flag.setVisible(v);
            if (v) this.setTimeOut(false);
            if (this.player_black) this.player_black.setVisible(v);
        },

        setTimeOut: function (v) {
            if (v && this.offline_flag && this.offline_flag.isVisible())return;

            if (this.timeount_flag) this.timeount_flag.setVisible(v);
            if (this.player_black) this.player_black.setVisible(v);
        },

        removeZhuanClock: function () {
            var clock = this.getChildByName("zhuan_clock");
            if (clock) {
                clock.removeFromParent();
                clock = null;
            }
        },

        setZhuanClock: function () {
            if (!this.isVisible() || MJModel.players[this.seatid].ready != 0) {
                return;
            }
            var clock = this.getChildByName("zhuan_clock");
            var text = null;
            if (!clock) {
                clock = new cc.Sprite("RoomPublic/room_public_clock.png");
                clock.setName("zhuan_clock");
                var pos = MJModel.getPosBySeatid(this.seatid);
                var v = cc.p(110, 30);
                if (pos == 1) {
                    v = cc.p(-20, 30);
                }
                clock.setPosition(v);
                this.addChild(clock);

                text = new ccui.Text("", "AmericanTypewriter", 22);
                text.setName("clock_text");
                text.setPosition(cc.p(clock.getContentSize().width / 2,clock.getContentSize().height / 2));
                text.setTextColor(cc.color(255, 221, 87));
                text.setTextHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER);
                text.setAnchorPoint(cc.p(0.5, 0.5));

                clock.addChild(text);
            }
            if (clock) {
                clock.setVisible(true);
                text = clock.getChildByName("clock_text");
            }
            var time = MJModel.players[this.seatid].zhuan_surplustime;
            if (text) {
                text.setString(time);
            }
        },

        setFengWei: function (seatid) {
        },

        setHuaPaiNum: function (num) {
            if (num == 0) {
                if (this.huapai_flag) {
                    this.huapai_flag.setVisible(false);
                }
                this.curHuaPaiNums = 0;
                return;
            }

            if (this.huapai_flag == null) {
                this.huapai_flag = new cc.Sprite(getResPath("RoomMJ/gdmj/hua.png"));
                this.huapai_flag.setAnchorPoint(cc.p(1, 0.5));
                this.addChild(this.huapai_flag);

                this.huapai_nums = new cc.LabelAtlas("90", getResPath("RoomMJ/gdmj/hua_nums.png"), 18, 24, '0');
                this.huapai_nums.setAnchorPoint(cc.p(0, 0.5));
                this.huapai_nums.setPosition(cc.p(this.huapai_flag.getContentSize().width, this.huapai_flag.getContentSize().height / 2));
                this.huapai_nums.setColor(cc.color(253, 251, 152));
                this.huapai_flag.addChild(this.huapai_nums);
            }

            this.curHuaPaiNums += num;
            var pos;
            var csize = this.getContentSize();
            if (this.pid == 2) {
                pos = cc.p(csize.width / 2, -15);
            }
            else {
                pos = cc.p(csize.width / 2, csize.height + 15);
            }
            this.huapai_flag.setVisible(true);
            this.huapai_flag.setPosition(pos);

            this.huapai_nums.setString("9" + this.curHuaPaiNums);
        }
    });

