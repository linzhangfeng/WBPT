
var CXTips =
{
    
    clock:null,
    clockTxt:null,
    uiNode:null,
    clockNum :-1,

    myTip:null,   //问题的提示
    allTip:null,  //所有的提示
    pibg:null,  //要出的皮,背景提示

    myAssets:null, //我的资产

    compare_first_tip:null,
    compare_last_tip:null,

    game_lj_xiu:null,

    specialCardName:null,

    bigfaceTime:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        var bg = utils.findNode(this.uiNode.getParent(),"bg");

        this.clock=this.getUI("time_clock");
        this.clockTxt=this.clock.getChildByName("num");
        this.clockTxt.ignoreContentAdaptWithSize(true);

        this.compare_first_tip = this.getUI("cx_com_first");
        this.compare_last_tip = this.getUI("cx_com_last");
        this.game_lj_xiu = this.getUI("cx_com_xiu");

        this.myTip =this.getUI("myTipBg");
        this.myTip.getChildByName("txt").ignoreContentAdaptWithSize(true);

        this.pibg = this.getUI("pibg");
        this.pibg.getChildByName("num").ignoreContentAdaptWithSize(true);

        this.myAssets = this.getUI("lastmon_bg");
        this.myAssets.getChildByName("num").ignoreContentAdaptWithSize(true);

        this.allTip =this.getUI("allTipBg");
        this.allTip.getChildByName("txt").ignoreContentAdaptWithSize(true);

        this.specialCardName = this.getUI("tip_bg");
        this.specialCardName.getChildByName("cardName").ignoreContentAdaptWithSize(true);

        this.reset();

        var self = this;
        //开启每秒的定时器
        this.uiNode.schedule(function(dt)
        {
             if(self.clockNum < 0)
            {
                return;
            }
            self.clockNum--;
            if(self.clockNum>=0)
            {
                self.clockTxt.setString(""+self.clockNum);
                if(self.clockNum ==5)
                {
                    PlatformHelper.vibrate();
                }
                if(self.clockNum <=5)
                {
                   // Sound.getInstance().playEffect("public/res/sound/time_click.mp3");
                   Sound.getInstance().playEffect(PackName+"/res/sound/com/time_over.mp3");
                }
            }
        },1);

        this._listenerChat = cc.EventListener.create({
            event:cc.EventListener.CUSTOM,
            eventName:"notify_room_chat1",
            callback:function(evt){
                self.notifyChat(JsUtils.decodeBase64(evt.getUserData()));
                cc.log("notify_room_chat1");
            }
        });


    cc.eventManager.addListener(this._listenerChat,this.uiNode);
    },

    stopTipTimer:function()
    {
        this.uiNode.unscheduleAllCallbacks();
    },

    reset:function()
    {
        this.clock.setVisible(false);
        this.compare_first_tip.setVisible(false);
        this.compare_last_tip.setVisible(false);
        this.game_lj_xiu.setVisible(false);


        this.myTip.setVisible(false);
        this.pibg.setVisible(false);
        this.myAssets.setVisible(true);
        this.showMyAssets(true);
        this.allTip.setVisible(false);

    },

    showSpecialCard:function(vi,cN)
    {
        this.specialCardName.setVisible(vi);
        this.specialCardName.getChildByName("cardName").setString(cN);
    },

    showTipTou:function(vi)
    {
        this.hideTouAndWei();
        this.compare_first_tip.setVisible(vi);
    },

    showTipWei:function(vi)
    {
        this.hideTouAndWei();
        this.compare_last_tip.setVisible(vi);
    },

    hideTouAndWei:function()
    {
        this.compare_first_tip.setVisible(false);
        this.compare_last_tip.setVisible(false);
    },

    showLJXiu:function(vi)
    {
        this.game_lj_xiu.setVisible(vi);
    },

    showMyAssets:function(vi)
    {
        var myMoney = this.myAssets.getChildByName("num");
        if(CXModel.seatID != -1)
        {
            if(CXModel.seatID <0 || CXModel.seatID > 5)
            {
                return;
            }
            var player = CXModel.players[CXModel.seatID];
            if(player!= null && player != undefined)
            {
                myMoney.setString("$"+player.money);
                this.myAssets.setVisible(vi);
            }
            
        }
        
    },

    getUI:function(key)
    {
        return this.uiNode.getChildByName(key);
    },

    showUI:function(key ,is)
    {
        var ui= this.getUI(key);
        if(ui)
        {
            ui.setVisible(is);
        }
        else{
            cc.log("ui erro :"+key);
        }
    },

    showMPAnimation:function(pos,vi)
    {

    },

    showQunaguan:function(pos,vi)
    {
        
    },

    showLastCardNum:function(pos,vi,number)
    {

    },


    showwin:function(pos,vi)
    {

    },

    showUIs:function(list,is)
    {
        for(var i in list)
        {
            list[i].setVisible(is);
        }
    },

    //出牌提示
    showMyOutCardTip:function(type,vi)
    {

    },


    //发牌之后第一个出牌的提示
    showFirstOutCardTip:function(type,pos)
    {
        
    },

    hideFirstOutCardTip:function()
    {

    },

    showPassTip:function(key,pos,v)
    {
        this.passTips[pos][key].setVisible(v);
        this.passTips[pos][key].runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    hideJBTips:function()
    {

    },
    hideThinkTips:function()
    {

    },

    //提示我方状态信息   -- 弹出提示的信息
    showMyTip:function(msg)
    {
        if(msg == "")
        {
            this.myTip.setVisible(false);
            return;
        }
        this.myTip.setVisible(true);
        this.myTip.stopAllActions();
        var txt = this.myTip.getChildByName("txt");
        txt.setAnchorPoint(cc.p(0.5,0.5));
        txt.setString(msg);
        txt.setPositionX(this.myTip.getContentSize().width*0.5);
        this.myTip.runAction(cc.sequence(cc.delayTime(3),cc.hide()));
    },


    //提示等待提示
    showWaitTip:function(msg)
    {
        if(msg == "")
        {
            this.myTip.setVisible(false);
            return;
        }
        this.myTip.setVisible(true);
        this.myTip.stopAllActions();
        var txt = this.myTip.getChildByName("txt");
        txt.setString(msg);
        txt.setAnchorPoint(cc.p(0,0.5));
        txt.setPositionX(this.myTip.getContentSize().width*0.5 - txt.getContentSize().width*0.5);

        this.myTip.runAction(cc.repeatForever(cc.sequence(cc.delayTime(0.2),cc.callFunc(function(){
            txt.setString(msg+".");
        }),cc.delayTime(0.2),cc.callFunc(function(){
            txt.setString(msg+"..");
        }),cc.delayTime(0.2),cc.callFunc(function(){
            txt.setString(msg+"...");
        }))));
    },

    //提示信息
    showAllTip:function(msg)
    {
        if(msg == "")
        {
            this.allTip.setVisible(false);
            return;
        }
        this.allTip.setVisible(true);
        this.allTip.setColor(cc.color.WHITE);
        this.allTip.getChildByName("txt").setString(msg);
    },


    showEnunciationCoin:function(pos,num)
    {
        if (num == 0) {
            return;
        }
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var pn = players.getChildByName("player"+pos);
        var label=null;
        if (num > 0) {
            label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/dz_balance_num_win.png", 28, 40, '0');
        } else {
            label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/dz_balance_num_lose.png", 28, 40, '0');
        }
        label.setAnchorPoint(cc.p(0.5, 0.5));
        this.uiNode.addChild(label, 20000);
        label.setPosition(cc.p(pn.getPosition().x , pn.getPosition().y+pn.getContentSize().height*0.5- 10));
        label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)),cc.delayTime(3),cc.removeSelf()));

    },


    //设置底注
    setCoin:function(coin)
    {
 
    },
    //设置倍率
    setRatio:function(v)
    {

    },

    setRadioAndCoin:function()
    {
       
    },

    checkGameOptionlist:function(oplist,op)
    {
        var flg = false;
        for(var i = 0 ;i<oplist.length;i++)
        {
            if(oplist[i]==op)
            {
                flg = true;
                break;
            }
        }
        return flg;
    },

    //设置倍率
    setZha:function(v)
    {

    },

    hideJiaoQiangTip:function()
    {

    },
    hideMingPaiTip:function()
    {

    },
    hideOutCardTip:function()
    {

    },


    showOutCard:function(pos,v)
    {

    },
    //显示时间
    showTime:function(num)
    {
        this.clockNum = num;
        this.clock.setVisible(true);
        this.clockTxt.setString(num);
    },

    hideTime:function()
    {
        this.clockPos = -1;
        this.clockNum = -1;
        this.clock.setVisible(false);
    },

    //超时处理
    startWaitTime:function(pos,num)
    {
        this.stopWaitTime(pos);
        var self = this;
        var func =cc.callFunc(function () {
            self.showThink(pos,true);
        });
        var ac = cc.sequence(cc.delayTime(num),func);
        ac.setTag(10000+pos);
        this.uiNode.runAction(ac);
    },
    stopWaitTime:function(pos)
    {
        this.uiNode.stopActionByTag(10000+pos);
        this.showThink(pos,false);
    },

    //显示想一想超
    showThink:function(pos,v)
    {
        this.thinkTips[pos].setVisible(v);
        this.thinkTips[pos].stopAllActions();
        if(v)
        {
            this.thinkTips[pos].think =true;
            var player = CXModel.getPlayerByPos(pos);
            this.showTimeOut(pos,true);
        }
        else
        {
            this.thinkTips[pos].think =false;
        }
    },

    //显示超时提示
    showTimeOut:function(pos,v)
    {

    },

    checkTimeOut:function(pos)
    {

    },

    updateTimeOut:function(pos) {

    },
    showJiaBeiEf:function(type,num)
    {

    },

    showbi:function(pos)
    {
        
    },

    showWinCoin:function(pos,num)
    {
        if (num == 0) {
            return;
        }

        var label=null;
        if (num > 0) {
            label = cc.LabelAtlas(":"+num,PackName+ "/res/font/dz_balance_num_win.png", 28, 40, '0');
        } else {
            label = cc.LabelAtlas(":"+(-num),PackName+ "/res/font/dz_balance_num_lose.png", 28, 40, '0');
        }
        label.setAnchorPoint(cc.p(0.5, 0.5));
        this.uiNode.addChild(label, 20000);
        var pn = CXPlayers.players[pos].uiNode;
        label.setPosition(cc.p(pn.getPosition().x , pn.getPosition().y+pn.getContentSize().height*0.5- 10));

        label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)),cc.delayTime(3),cc.removeSelf()));

    },
    //展示牌型动画
    showCardEf:function(type)
    {
       
    },

    showBigface:function(b,end,fid,sex,tpos)
    {
        var appear;
        var move;

        var sound = "";
        var face = null;
        switch (fid)
        {
            case 1:
                face =new cc.Sprite(utils.spriteFrame("public/flower.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.7, end);
                    sound = "public/res/sound/bigface/"+sex.toUpperCase()+"_xianhua.mp3";
                }
                break;
            case 2:
                face =new cc.Sprite(utils.spriteFrame("public/cheers.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.7, end);
                    sound = "public/res/sound/bigface/"+sex.toUpperCase()+"_pijiu.mp3";

                    end.x = end.x - 20;
                }
                break;
            case 3:
                face =new cc.Sprite(utils.spriteFrame("public/kiss.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.moveTo(0.7, end);
                    sound = "public/res/sound/bigface/"+sex.toUpperCase()+"_kiss.mp3";
                }
                break;
            case 4:
                face =new cc.Sprite(utils.spriteFrame("public/redbag.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720),cc.moveTo(0.5, end));
                }
                break;
            case 5:
                face =new cc.Sprite(utils.spriteFrame("public/firecrackers.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720),cc.moveTo(0.5, end));
                }
                break;
            case 6:
                face =new cc.Sprite(utils.spriteFrame("public/fireworks.png"));
                if(face)
                {
                    appear = cc.fadeIn(0.5);
                    move = cc.spawn(cc.rotateBy(0.5, 720),cc.moveTo(0.5, end));
                }
                break;
            default:
            {
                cc.log( "RoomDDZTip showBigFace fid is %d " ,+fid );
                break;
            }
        }

        if(face)
        {
            face.setPosition(b);
            face.setOpacity(0);
            this.uiNode.addChild(face);

            var self = this;
            var playsoundcallback = cc.callFunc(function()
            {
                var endEf =null;
                var endDT =2;
                if(fid == 1)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/xianhua.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effects_bqhua.ExportJson");
                    endEf = new ccs.Armature("effects_bqhua");
                }
                else if(fid == 2)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/pijiu.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effects_bqganbei.ExportJson");
                    endEf = new ccs.Armature("effects_bqganbei");
                    endDT = 0.7;
                }
                else if(fid == 3)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/"+sex+"_wen.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effects_bqweng.ExportJson");
                    endEf = new ccs.Armature("effects_bqweng");
                    endDT = 1;
                }
                else if(fid == 4)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/redbag.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effects_bqhongbao.ExportJson");
                    endEf = new ccs.Armature("effects_bqhongbao");
                    endDT =3;
                }
                else if(fid == 5)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/firecrackers.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effectsbqbianpao.ExportJson");
                    endEf = new ccs.Armature("effectsbqbianpao");
                    endDT =5;
                }
                else if(fid == 6)
                {
                    Sound.getInstance().playEffect("public/res/sound/bigface/fireworks.mp3");
                    ccs.armatureDataManager.addArmatureFileInfo("public/res/effects/effects_bqyanhua.ExportJson");
                    endEf = new ccs.Armature("effects_bqyanhua");
                }
                if(endEf)
                {
                    var pp = cc.p(end.x,end.y);
                    if(fid == 2)
                    {
                        if(tpos == 1 || tpos == 2)
                        {
                            pp.x = pp.x -  160;
                        }
                    }

                    self.uiNode.addChild(endEf);
                    endEf.setPosition(pp);
                    endEf.getAnimation().playWithIndex(0);
                    endEf.runAction(cc.sequence(cc.delayTime(endDT),cc.removeSelf()));
                }
            });

            face.runAction(cc.sequence(appear,move,playsoundcallback,cc.removeSelf()));

            if(sound!="")
            {
                Sound.getInstance().playEffect(sound);
            }
        }
    },

    notifyChat:function(str)
    {
        if(str == "")
        {
            return;
        }

        var dat = JSON.parse(str);
        if(dat == null )
        {
            return;
        }

        var uid = dat["uid"];
        var name = dat["name"];
        var content = dat["content"];

        var p = CXModel.getPlayerByUID(uid);
        if (dat.hasOwnProperty("filepath"))
        {
            SoundModel.getInstance().addSound(dat["filepath"]);
        }
        if(p&&p.seatid !=-1)
        {
            this.showChat(CXModel.getPosBySeatID(p.seatid),content);
            var file = utils.getFastChatSound(p.sex ==1 ?"m":"w",content);
            if(file!="")
            {
                cc.log("ddddd");
                Sound.getInstance().playEffect("public/res/sound/fastchat/"+file);
            }
        }
        // this.showMoveChat(uid,name,content);
    },
    showChat:function(pos,content)
    {
        try
        {
             var text = ccui.Text(content, "AmericanTypewriter", 30);

            var chat_bg =  ccui.Scale9Sprite.createWithSpriteFrameName("CX/bar_txt.png");
            chat_bg.setPreferredSize(cc.size(text.getContentSize().width+50, text.getContentSize().height+20));

            this.uiNode.addChild(chat_bg,-100);
            chat_bg.setColor(cc.color(125,125,125));

            text.setAnchorPoint(cc.p(0.5, 0.5));
            text.setPosition(chat_bg.getContentSize().width / 2,chat_bg.getContentSize().height / 2);
            chat_bg.addChild(text);

            var p = CXPlayers.getPosition(pos);
            if (pos == 0)
            {
                chat_bg.setAnchorPoint(cc.p(0, 0.5));
                chat_bg.setPosition(p.x + 50, p.y+50);
            }
            else if (pos == 1)
            {
                chat_bg.setAnchorPoint(cc.p(1.0, 0.5));
                chat_bg.setPosition(p.x - 50, p.y+50);
            }
            else if (pos == 2)
            {
                chat_bg.setAnchorPoint(cc.p(1.0, 0.5));
                chat_bg.setPosition(p.x - 50, p.y+50);
            }else if (pos == 3)
            {
                chat_bg.setAnchorPoint(cc.p(0, 0.5));
                chat_bg.setPosition(p.x + 50, p.y+50);
            }
            else if (pos == 4)
            {
                chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
                chat_bg.setPosition(p.x + 50, p.y+50);
            }else if (pos == 5)
            {
                chat_bg.setAnchorPoint(cc.p(0.0, 0.5));
                chat_bg.setPosition(p.x + 50, p.y+50);
            }

            chat_bg.runAction(cc.sequence(cc.delayTime(3),cc.removeSelf()));

        }catch(e)
        {
            cc.spriteFrameCache.addSpriteFrames("cx/res/img/common.plist");
        }

    },
    showMoveChat:function(uid,name,content)
    {
        var winSize = cc.director.getWinSize();
        var container = new cc.Node();
        this.uiNode.addChild(container, 100);
        var totalWidth = 0;

        var contentLabel = cc.Label.createWithSystemFont(content, "Arial", 24);
        contentLabel.setAnchorPoint(cc.p(0, 0));
        container.addChild(contentLabel);
        totalWidth += contentLabel.getContentSize().width;

        if (ZJHModel.getInstance().uid == uid)
        {
            var nameLabel = cc.Label.createWithSystemFont(name + ":", "Arial", 24);
            nameLabel.setColor(cc.color(0, 255, 0));
            nameLabel.setAnchorPoint(cc.p(0, 0));
            nameLabel.setPosition(cc.p(0, 0));
            container.addChild(nameLabel);
            totalWidth += nameLabel.getContentSize().width;

            contentLabel.setPosition(nameLabel.getContentSize().width, 0);
        }
        else
        {
            var nameLabel = cc.Label.createWithSystemFont(name + ":", "Arial", 24);
            nameLabel.setColor(cc.color(181, 243, 255));
            nameLabel.setAnchorPoint(cc.p(0, 0));
            nameLabel.setPosition(cc.p(0, 0));
            container.addChild(nameLabel);
            totalWidth += nameLabel.getContentSize().width;

            contentLabel.setPosition(nameLabel.getContentSize().width, 0);
        }

        var begin = cc.p(winSize.width, utils.randomf(winSize.height * 0.7, winSize.height * 0.8));
        container.setPosition(begin);
        container.runAction(cc.sequence(cc.moveBy(6, cc.p(-winSize.width - totalWidth, 0)),cc.removeSelf()));

    },

    showMust:function()
    {

    },

    showMustTip:function()
    {

    },

    showPass:function(is)
    {

    },
    showMp: function (pos) {

    },


    showSplitCardTip:function(type,vi)
    {
        
    },

    showSilder:function(vi,pre)
    {
        this.slider.setPercent(pre);
    },

    showSilderVis:function(vi)
    {
        this.slider.setVisible(vi);
    },

    showsplitReadyTip:function(pos,vi)
    {
        this.allReadyTips[pos].setVisible(vi);
    },


};
