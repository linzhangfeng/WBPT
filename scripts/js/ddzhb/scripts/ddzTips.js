
var DDZJiaBei=
{
    SPRING_RE:0,
    SPRING:1,
    ROCKET:2,
    BOMB:3,
    TI:4,
    LA:5,
    MING_PAI:6,
    QIANG_DZ:7,
};
var DDZTips =
{
    uiNode:null,
    passTips:[],
    inTips:[],
    jbTips:[],
    thinkTips:[],
    myTip:null,
    allTip:null,
    liujuTip:null,
    dzTip:null,
    clock:null,
    clockTo:null,
    clockTxt:null,
    clockNum :-1,
    clockPos:-1,
    playType:[],
    tipsBg:[],
    tilaTxt:[],
    mp:[],
    bigfaceTime:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        var bg = this.uiNode.getParent();
        this.playType.push(utils.findNode(bg,"typezq"));
        this.playType.push(utils.findNode(bg,"typegq"));
        this.playType.push(utils.findNode(bg,"typejq"));
        this.playType.push(utils.findNode(bg,"typesw"));
        this.playType.push(utils.findNode(bg,"typezh"));
        this.playType.push(utils.findNode(bg,"typehp"));
        this.myTip =this.getUI("myTipBg");
        this.myTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.allTip =this.getUI("allTipBg");
        this.allTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.liujuTip=this.getUI("liuju");
        this.dzTip=this.getUI("dizhu");
        this.clock=this.getUI("clock");
        this.clockTxt=this.clock.getChildByName("txt");
        this.clockTo=this.clock.getChildByName("to");
        this.getUI("coin").ignoreContentAdaptWithSize(true);
        this.getUI("ratio").ignoreContentAdaptWithSize(true);
        this.getUI("zha").ignoreContentAdaptWithSize(true);
        var passTxts =["txt_buchu","txt_bujiao","txt_buqiang"
            ,"txt_guo","txt_jiaodizhu","txt_mingpai","txt_qiangdizhu","txt_woqiang","txt_yaobuqi"];
        var poses =[cc.p(640,225),cc.p(1080,420),cc.p(200,420)];
        for(var i =0;i < 3;++i)
        {
            this.passTips[i] ={};
            for(var j in passTxts)
            {
                var tip = new cc.Sprite(utils.spriteFrame("ddz/"+passTxts[j]+".png"));
                this.uiNode.addChild(tip,0);
                tip.setName(passTxts[j]+i);
                tip.setPosition(poses[i]);
                this.passTips[i][passTxts[j]] =tip;
            }
            this.jbTips.push(this.getUI("jb"+i));


            this.thinkTips.push(utils.findNode(this.uiNode,"think"+i));
            this.thinkTips[i].setLocalZOrder(10);

            this.jbTips[i].runAction(cc.repeatForever(utils.newAni("ddz/jb/effects_jinbaodeng_000",0,7,0.1)));
        }
        for(var i =0;i < 3;++i)
        {
            this.tilaTxt[i] ="";
            this.tipsBg[i] = this.getUI("tips_bg"+i);
            this.mp[i] = this.getUI("mp"+i);

            this.tipsBg[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
        }
        this.showUIs(this.playType,false);
        this.reset();
        var self = this;
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
                if(self.clockPos == 0)
                {
                    if(self.clockNum ==3)
                    {
                        PlatformHelper.vibrate();
                    }
                    if(self.clockNum <=3)
                    {
                        Sound.getInstance().playEffect("public/res/sound/time_click.mp3");
                    }
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
    reset:function()
    {
        this.getUI("nopai").setVisible(false);
        this.showUIs1(this.passTips,false);
        this.showUIs1(this.inTips,false);
        this.showUIs(this.jbTips,false);
        this.showUIs(this.thinkTips,false);
        this.showUIs(this.tipsBg,false);
        this.showUIs(this.mp,false);
        this.myTip.setVisible(false);
        this.allTip.setVisible(false);
        this.liujuTip.setVisible(false);
        this.dzTip.setVisible(false);
        this.clock.setVisible(false);

        for(var i=0;i<3;++i)
        {
            this.tilaTxt[i] ="";
            this.stopWaitTime(i);
        }
        this.hideTime();
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

    showUIs:function(list,is)
    {
        for(var i in list)
        {
            list[i].setVisible(is);
        }
    },
    showUIs1:function(list,is)
    {
        for(var i in list)
        {
            for(var j in list[i]) {
                list[i][j].setVisible(is);
            }
        }
    },
    showPassTip:function(key,pos,v)
    {
        this.passTips[pos][key].setVisible(v);
    },
    hidePassTips:function()
    {
        this.showUIs1(this.passTips,false);
    },
    hideInTips:function()
    {
        this.showUIs1(this.inTips,false);
    },
    hideJBTips:function()
    {
        this.showUIs(this.jbTips,false);
    },
    hideThinkTips:function()
    {
        this.showUIs(this.thinkTips,false);
    },
    //提示我方状态信息
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
    showMyTip1:function(msg)
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
    //流局
    showNoDiZhu:function(v)
    {
        this.liujuTip.setVisible(v);
    },
    //地主
    showDiZhu:function(pos)
    {
        if(pos == -1)
        {
            this.dzTip.setVisible(false);
        }
        else
        {
            var p = cc.p(60,360);
            if(pos ==1)
            {
                p =cc.p(1220,610);
            }
            else  if(pos ==2)
            {
                p =cc.p(60,610);
            }
            this.dzTip.setVisible(true);
            if(DDZModel.fastShow)
            {
                this.dzTip.setScale(1);
                this.dzTip.setPosition(p);
            }
            else
            {
                this.dzTip.setPosition(640,360);
                this.dzTip.setScale(0);
                this.dzTip.runAction(cc.sequence(cc.scaleTo(0.3,1.3),cc.spawn(cc.moveTo(0.2,p)
                    ,cc.delayTime(0.2),cc.scaleTo(0.2,1))));
            }
        }
    },
    //设置底注
    setCoin:function(coin)
    {
        this.getUI("coin").setString(""+coin);
    },
    //设置倍率
    setRatio:function(v)
    {
        this.getUI("ratio").setString(v+"倍");
    },
    //设置倍率
    setZha:function(v)
    {
        if(v>100)
        {
            this.getUI("zha").setString("无限");
        }
        else
        {
            this.getUI("zha").setString(v+"倍");
        }
    },
    hideJiaoQiangTip:function()
    {
        for(var i =0 ;i<3;++i)
        {
            this.showPassTip("txt_bujiao",i,false);
            this.showPassTip("txt_buqiang",i,false);
            this.showPassTip("txt_qiangdizhu",i,false);
            this.showPassTip("txt_jiaodizhu",i,false);
            this.showPassTip("txt_woqiang",i,false);
        }
    },
    hideMingPaiTip:function()
    {
        for(var i =0 ;i<3;++i)
        {
            this.showPassTip("txt_mingpai",i,false);
        }
    },
    hideOutCardTip:function()
    {
        for(var i =0 ;i<3;++i)
        {
            this.showPassTip("txt_buchu",i,false);
            this.showPassTip("txt_yaobuqi",i,false);
            this.showPassTip("txt_guo",i,false);
        }
    },
    //显示叫分
    showJiaoFen:function(pos,num)
    {
        if(num >=1 && num <=3)
        {
            var tip = this.passTips[pos]["txt_fen"+num];
            tip.setVisible(true);
        }
        else if(num ==5)
        {
            var tip = this.passTips[pos]["txt_bujiao"];
            tip.setVisible(true);
        }
        else if(num ==0) {

            for(var i= 1;i <=3;++i)
            {
                this.passTips[pos]["txt_fen"+i].setVisible(false);
            }
            this.passTips[pos]["txt_bujiao"].setVisible(false);
        }
    },
    //显示叫抢
    showJiaoQiang:function(pos,num)
    {
        this.passTips[pos]["txt_jiaodizhu"].setVisible(false);
        this.passTips[pos]["txt_woqiang"].setVisible(false);
        this.passTips[pos]["txt_bujiao"].setVisible(false);
        this.passTips[pos]["txt_buqiang"].setVisible(false);
        if(num == 3)
        {
            var tip = this.passTips[pos]["txt_jiaodizhu"];
            tip.setVisible(true);
        }
        else if(num ==4)
        {
            var tip = this.passTips[pos]["txt_woqiang"];
            tip.setVisible(true);
        }
        else if(num ==5)
        {
            var tip = this.passTips[pos]["txt_bujiao"];
            if(DDZModel.state == DDZState.PREPLAY_TWO)
            {
                tip = this.passTips[pos]["txt_buqiang"];
            }
            tip.setVisible(true);
        }
    },
    //显示盖抢
    showGaiQiang:function(pos,type)
    {
        if(type ==1)
        {
            this.tipsBg[pos].setVisible( true);
            this.tipsBg[pos].getChildByName("txt").setString("不盖抢");
        }
        else  if(type ==2)
        {
            this.tipsBg[pos].setVisible( true);
            this.tipsBg[pos].getChildByName("txt").setString("盖抢");
        }
        else
        {
            this.tipsBg[pos].setVisible( false);
        }
    },
    //显示踢拉
    showTiLa:function(pos,tlType,tlAction)
    {
        //ti
        if(tlType == 1)
        {
            if(tlAction ==1)
            {
                this.tilaTxt[pos]="不追";
            }
            else  if(tlAction ==2)
            {
                this.tilaTxt[pos]="追";
            }
            else
            {
                this.tilaTxt[pos]="";
            }
        }//la
        else if(tlType ==2)
        {
            if(tlAction ==1)
            {
                this.tilaTxt[pos]="不反";
            }
            else  if(tlAction ==2)
            {
                this.tilaTxt[pos]="反追";
            }
            else
            {
                this.tilaTxt[pos]="";
            }
        }
        else
        {
            this.tilaTxt[pos]="";
        }

        if(this.tilaTxt[pos]!="")
        {
            this.tipsBg[pos].setVisible( true);
            this.tipsBg[pos].getChildByName("txt").setString(this.tilaTxt[pos]);
        }
    },
    needBuLa:function()
    {
        var pos = DDZModel.getDzPos();
        if(this.tilaTxt[pos]!="")
        {
            return false;
        }
        for(var i in this.tilaTxt)
        {
            if(pos!=i &&(this.tilaTxt[i]=="追"||this.tilaTxt[i]==""))
            {
                return false;
            }
        }
        return true;
    },
    getNoTiPos:function()
    {
        var dzPos = DDZModel.getPosBySeatID(DDZModel.dzSeatID);
        for(var i =0 ;i<3;++i)
        {
            if(dzPos !=i&&this.tilaTxt[i] == "")
            {
                return i;
            }
        }
        return -1;
    },
    //手牌警报
    showHandCardJB:function(pos,v)
    {
        this.jbTips[pos].setVisible(v);
    },
    showOutCard:function(pos,v)
    {
        this.showPassTip("txt_buchu",pos,v);
    },
    //显示时间
    showTime:function(pos,num)
    {
        var p=cc.p(48,10);
        var r = 90;
        if(pos ==1)
        {
            p =cc.p(96,54);
            r = 0;
        }
        else  if(pos ==2)
        {
            p =cc.p(0,54);
            r = 180;
        }
        this.clockPos = pos;
        this.clockNum = num;
        this.clock.setVisible(true);
        //this.clock.setPosition(p);
        this.clockTxt.setString(num);
        this.clockTo.setPosition(p);
        this.clockTo.setRotation(r);
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
            var player = DDZModel.getPlayerByPos(pos);
            this.showTimeOut(pos,true);

           /* if(player && player.isOffline == 0)
            {
                var self = this;
                var func = cc.callFunc(function(){
                    self.showTimeOut(pos,self.thinkTips[pos].think);
                });
                this.thinkTips[pos].runAction(cc.sequence(cc.fadeIn(0),cc.delayTime(3),cc.fadeOut(0.5)
                ,cc.delayTime(5),cc.fadeIn(0),cc.delayTime(3),cc.fadeOut(0.5),cc.hide()));
                this.thinkTips[pos].runAction(cc.sequence(cc.delayTime(8),func));
            }
            else
            {
                this.showTimeOut(pos,true);
            }*/
        }
        else
        {
            this.thinkTips[pos].think =false;
        }
    },
    //显示超时提示
    showTimeOut:function(pos,v)
    {
        this.thinkTips[pos].setVisible(true);
        this.thinkTips[pos].stopAllActions();
        this.thinkTips[pos].think =v;

        var p = -1;
        //优先判断需要断线提示
        for(var i =0;i <3;++i)
        {
            var player = DDZModel.getPlayerByPos(i);
            if( this.thinkTips[i].think && player&&player.isOffline == 1 )
            {
                p =i;
                break;
            }
        }
        if(p == -1)//优先判断自己需要超时
        {
            for(var i =0;i <3;++i)
            {
                var player = DDZModel.getPlayerByPos(i);
                if( this.thinkTips[i].think )
                {
                    p =i;
                    break;
                }
            }
        }
        if(p!= -1)
        {
            var player = DDZModel.getPlayerByPos(p);
            if( player&&player.isOffline==0)
            {
               // this.showAllTip("玩家 "+player.name+" 操作超时，请耐心等待！");
                this.showAllTip("");
            }
            else if( player)
            {
                this.showAllTip("玩家 "+player.name+" 已掉线，游戏被迫暂停！");
                this.allTip.setColor(cc.color.RED);
            }
            else
            {
                cc.log("erro player:"+pos)
            }
        }
        else{
            this.showAllTip("");
            this.thinkTips[pos].setVisible(false);
        }
    },
    checkTimeOut:function(pos)
    {
        if(this.thinkTips[pos].think)
        {
            this.updateTimeOut(pos);
        }
    },
    updateTimeOut:function(pos) {
        this.showTimeOut(pos, this.thinkTips[pos].think);
    },
    showJiaBeiEf:function(type,num)
    {
        return;
        if(this.uiNode.getChildByName("jiaBeiEf"))
        {
            var self = this;
            this.uiNode.runAction(cc.sequence(cc.delayTime(0.5),cc.callFunc(function(){
                self.showJiaBeiEf(type,num);
            })));
            return;
        }
        var tip ="";
        switch (type)
        {
            case  DDZJiaBei.BOMB:
                tip = "炸弹X"+num+"倍";
                break;
            case  DDZJiaBei.TI:
                tip = "追X"+num+"倍";
                break;
            case  DDZJiaBei.LA:
                tip = "反追X"+num+"倍";
                break;
            case  DDZJiaBei.MING_PAI:
                tip = "明牌X"+num+"倍";
                break;
            case  DDZJiaBei.QIANG_DZ:
                tip = "抢地主X"+num+"倍";
                break;
            case  DDZJiaBei.ROCKET:
                tip = "王炸X"+num+"倍";
                break;
            case  DDZJiaBei.SPRING:
                tip = "春天X"+num+"倍";
                break;
            case  DDZJiaBei.SPRING_RE:
                tip = "反春天X"+num+"倍";
                break;
        }
        var t =new ccui.Text(tip,"",26);
        t.setAnchorPoint(cc.p(0,0.5));
        t.setPosition(870,22);
        t.setName("jiaBeiEf");
        this.uiNode.addChild(t,1000);
        t.runAction(cc.sequence(cc.delayTime(1),cc.fadeOut(0.5),cc.removeSelf()));
    },

    showWinCoin:function(pos,num)
    {
        if (num == 0) {
            return;
        }

        var label=null;
        if (num > 0) {
            label = new cc.LabelAtlas(":"+num,PackName+ "/res/font/dz_balance_num_win.png", 28, 40, '0');
        } else {
            label = new cc.LabelAtlas(":"+(-num),PackName+ "/res/font/dz_balance_num_lose.png", 28, 40, '0');
        }
        label.setAnchorPoint(cc.p(0.5, 0.5));
        this.uiNode.addChild(label, 20000);
        var pn = DDZPlayers.players[pos].uiNode;
        label.setPosition(cc.p(pn.getPosition().x , pn.getPosition().y+pn.getContentSize().height*0.5- 10));

        label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)),cc.delayTime(3),cc.removeSelf()));

    },
    //展示牌型动画
    showCardEf:function(type)
    {
        if(DDZModel.fastShow)
        {
            return;
        }
        var size = cc.director.getWinSize();
        var sp =null;
        var ef =null;
        var begin = cc.p(0,0);
        var end = cc.p(0,0);
        var moveTime =0;
        var showTime =0;
        var sound ="";
        var ey =380;
        switch (type)
        {
            case libddz.CardsType.CARD_TYPE_ONELINE:
                sp =new cc.Sprite(utils.spriteFrame("ddz/sz/action_sz1.png"));
                if(sp)
                {
                    ef = utils.newAni("ddz/sz/action_sz",1,7,0.1);
                    begin = cc.p(size.width / 2,ey);
                    end = cc.p(size.width / 2,ey);
                    moveTime = 0;
                    showTime = 1.0;
                    sound = PackName+"/res/sound/effect/lianshun_sound.mp3";
                }
                break;
            case libddz.CardsType.CARD_TYPE_TWOLINE:
                sp =new cc.Sprite(utils.spriteFrame("ddz/ld/action_ld1.png"));
                if(sp)
                {
                    ef = utils.newAni("ddz/ld/action_ld",1,8,0.1);
                    begin = cc.p(size.width / 2,ey);
                    end = cc.p(size.width / 2,ey);
                    moveTime = 0;
                    showTime = 1.0;
                    sound = PackName+"/res/sound/effect/lianshun_sound.mp3";
                }
                break;
            case libddz.CardsType.CARD_TYPE_PLANEWITHONE:
                sp =new cc.Sprite(utils.spriteFrame("ddz/fj/action_plane1.png"));
                if(sp)
                {
                    ef = utils.newAni("ddz/fj/action_plane",1,2,0.1,100);
                    begin = cc.p(0,ey);
                    end = cc.p(size.width,ey);
                    moveTime = 1.0;
                    showTime = 1.0;
                    sound = PackName+"/res/sound/effect/plane_sound.mp3";
                }
                break;
            case libddz.CardsType.CARD_TYPE_SOFT_BOMB:
            case libddz.CardsType.CARD_TYPE_HARD_BOMB:
            case libddz.CardsType.CARD_TYPE_ROCKET:
            case libddz.CardsType.CARD_TYPE_FIVE_BOMB:
            case libddz.CardsType.CARD_TYPE_PLANE_BOMB:
            case libddz.CardsType.CARD_TYPE_PLANE_LAIZI_BOMB:
                sp =new cc.Sprite(utils.spriteFrame("ddz/zd/bm1.png"));
                if(sp)
                {
                    ef = utils.newAni("ddz/zd/bm",1,11,0.1);
                    begin = cc.p(size.width / 2,ey);
                    end = cc.p(size.width / 2,ey);
                    moveTime = 0;
                    showTime = 1.1;
                    sound = PackName+"/res/sound/effect/zd_rocket_sound.mp3";
                }
                break;
        }

        if(sp)
        {
            sp.setPosition(begin);

            sp.runAction(cc.spawn(ef,cc.moveTo(moveTime, end),cc.sequence(cc.delayTime(showTime),cc.removeSelf())));

            this.uiNode.addChild(sp,10000);

            if (sound!= "")
            {
                Sound.getInstance().playEffect(sound);
            }
        }

    },

    showBigface:function(b,end,fid,sex)
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

                    if(end.x <200)
                    {
                        end.x =25;
                    }
                    else
                    {
                        end.x =1040;
                    }
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

                try
                {
                    if(endEf)
                    {
                        self.uiNode.addChild(endEf);
                        endEf.setPosition(end);
                        endEf.getAnimation().playWithIndex(0);
                        endEf.runAction(cc.sequence(cc.delayTime(endDT),cc.removeSelf()));
                    }
                }
                catch(e){

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

        var p = DDZModel.getPlayerByUID(uid);
        var isSound =false;
        if (dat.hasOwnProperty("filepath"))
        {
            isSound =true;
            if(SoundModel.getInstance().addSound)
            {
                SoundModel.getInstance().addSound(dat["filepath"]);
            }
        }
        if(p&&p.seatid !=-1)
        {
            this.showChat(DDZModel.getPosBySeatID(p.seatid),content,isSound,utils.getValInt(dat,"time"));
            var file = utils.getFastChatSound(p.sex ==1 ?"m":"w",content);
            if(file!="")
            {
                Sound.getInstance().playEffect("public/res/sound/fastchat/"+file);
            }
        }

    },

    showChat:function(pos,content,isSound,time)
    {
        var bg =new DDZPlayerChat();
        if(isSound)
        {
            bg.initSound(pos,content,false,pos==1,time);
        }
        else
        {
            bg.initChat(pos,content,false,pos==1);
        }
        this.uiNode.removeChildByTag(96666);
        this.uiNode.addChild(bg,10000,96666);

        var p = DDZPlayers.getPosition(pos);
        if (pos == 0)
        {
            bg.setAnchorPoint(cc.p(0, 0.5));
            bg.setPosition(p.x + 50, p.y+50);
        }
        else if (pos == 1)
        {
            bg.setAnchorPoint(cc.p(0, 0.5));
            bg.setPosition(p.x - 50, p.y+50);
        }
        else if (pos == 2)
        {
            bg.setAnchorPoint(cc.p(0.0, 0.5));
            bg.setPosition(p.x + 50, p.y+50);
        }

        bg.runAction(cc.sequence(cc.delayTime(3),cc.removeSelf()));
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
    showPlayType:function()
    {
        this.playType[0].setVisible(DDZModel.tableType ==1);
        this.playType[1].setVisible(DDZModel.tableType ==2);
        this.playType[2].setVisible(DDZModel.tableType ==3);
        this.playType[3].setVisible(!libddz.cardsFind.splitRocket);
        this.playType[4].setVisible(DDZModel.zh ==true);
        this.playType[5].setVisible(true);
        utils.sortNodes(this.playType,640,565,20);
    },
    showPass:function(is)
    {
        this.getUI("nopai").setVisible(is);
    },
    showMp: function (pos) {

        var tip = this.mp[pos];
        tip.setVisible(true);
        tip.getAnimation().playWithIndex(0,-1,0);
        tip.runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    }
};
