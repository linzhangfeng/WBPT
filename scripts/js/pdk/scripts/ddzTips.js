
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
    must:[],
    tipsBg:[],
    tilaTxt:[],
    piaoTxt:[],
    mp:[],

    dzimage:null,
    cardW:148,
    cardH:194,
    dx:59,
    dy:194,
    ocardW:70,
    ocardH:92,
    odx:30,
    ody:45,

    myoutcartip1:null,
    myoutcartip2:null,
    myoutcartip3:null,
    myoutcartip4:null,

    firstoutcardtip1:null,
    firstoutcardtip2:null,
    allKuangTip:[],
    mustip:[],
    szTip:[],

    coins:[],
    quguanimage:[],
    lastCardNum:[],

    winImage:[],
    slider:null,

    mympLine:[],

    typeActions:[],

    //切牌提示
    splitCardTip:null,
    splitCardTip2:null,

    allReadyTips:[],

    firsttipFlag:false,

    bigfaceTime:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        var bg = this.uiNode.getParent();
        this.must.push(utils.findNode(bg,"mustjf"));
        this.must.push(utils.findNode(bg,"must1"));
        this.must.push(utils.findNode(bg,"must2"));
        this.must.push(utils.findNode(bg,"must3"));
        this.must.push(utils.findNode(bg,"mustjq"));

        this.mustip.push(this.getUI("mustip1"));
        this.mustip.push(this.getUI("mustip2"));
        this.mustip.push(this.getUI("mustip3"));
        this.mustip.push(this.getUI("mustip4"));
        this.mustip.push(this.getUI("mustip5"));

        this.szTip[1] = this.getUI("szTip1");
        this.szTip[1].getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.szTip[2] = this.getUI("szTip2");
        this.szTip[2].getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.dzimage = this.getUI("dzimage");
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
        this.getUI("coin").setVisible(false);
        // this.getUI("ratio").ignoreContentAdaptWithSize(true);
        this.typeActions[libddz.CardsType.CARD_TYPE_THREE] = this.getUI("sanzhang");
        this.typeActions[libddz.CardsType.CARD_TYPE_ONELINE] = this.getUI("shunzi");
        this.typeActions[libddz.CardsType.CARD_TYPE_FOURWITHTWO] = this.getUI("sidaier");
        this.typeActions[libddz.CardsType.CARD_TYPE_THREEWITHTWO] = this.getUI("sandaier");
        this.typeActions[libddz.CardsType.CARD_TYPE_FOURWITHTHREE] = this.getUI("sidaisan");
        this.typeActions[libddz.CardsType.CARD_TYPE_TWOLINE] = this.getUI("liandui");
        this.typeActions[libddz.CardsType.CARD_TYPE_PLANEWITHONE] = this.getUI("feiji");
        this.typeActions[libddz.CardsType.CARD_TYPE_PLANEWITHWING] = this.getUI("feiji");
        this.typeActions[libddz.CardsType.CARD_TYPE_PLANEWHITHLACK] = this.getUI("feiji");
        this.typeActions[libddz.CardsType.CARD_TYPE_BOMB] = this.getUI("zhadan");

        //跑的快
        this.myoutcartip1 = this.getUI("nextplayer_bdtip1");
        this.myoutcartip2 = this.getUI("nextplayer_bdtip2");
        this.myoutcartip3 = this.getUI("nextplayer_bdtip3");
        this.myoutcartip4 = this.getUI("nextplayer_bdtip4");

        this.firstoutcardtip1 = this.getUI("outcardtip1");
        this.firstoutcardtip2 = this.getUI("outcardtip2");

        this.splitCardTip = this.getUI("ddz_splitCard1");
        this.splitCardTip2 = this.getUI("ddz_splitCard2");

        this.slider = this.getUI("Slidersplit");
        this.slider.addCCSEventListener(this.sliderStateChange);


        var passTxts =["txt_buchu","txt_bujiao","txt_buqiang"
            ,"txt_fen_no1","txt_fen_no2","txt_fen_no3","txt_fen1","txt_fen2","txt_fen3","txt_guo"
            ,"txt_jiaodizhu","txt_mingpai","txt_qiangdizhu","txt_woqiang","txt_yaobuqi"];
        var poses =[cc.p(640,245),cc.p(1080,420),cc.p(200,420)];
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
            this.quguanimage.push(this.getUI("qg"+i));

            var  lacardlabel = this.getUI("lastnumcard"+i);
            utils.findNode(this.getUI("lastnumcard"+i),"num").ignoreContentAdaptWithSize(true);
            // var  lacardlabel = utils.findNode(this.uiNode,"lastnumcard"+i).ignoreContentAdaptWithSize(true);
            this.lastCardNum.push(lacardlabel);
            log("=====lacardlabellacardlabellacardlabel====",this.lastCardNum.length);

            this.winImage.push(this.getUI("mywinmg"+i));

            this.allKuangTip.push(this.getUI("rot"+i));

            this.allReadyTips.push(this.getUI("ready_"+i));

            this.thinkTips.push(utils.findNode(this.uiNode,"think"+i));
            this.thinkTips[i].setLocalZOrder(10);
            this.mympLine.push(this.getUI("mingbai"+i));
            // this.jbTips[i].runAction(cc.repeatForever(utils.newAni("ddz/jb/effects_jinbaodeng_000",0,7,0.1)));
        }

        /*
        for(var i =0;i < 3;++i)
        {
            this.tilaTxt[i] ="";
            this.piaoTxt[i] ="";
            this.tipsBg[i] = this.getUI("tips_bg"+i);
            this.mp[i] = this.getUI("mp"+i);

            this.tipsBg[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
        }
        */
        this.showUIs(this.must,false);
        this.showUIs(this.mustip,false);
        
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
                if(self.clockNum ==3 &&self.clockPos == 0)
                {
                    PlatformHelper.vibrate();
                }
                if(self.clockNum <=3)
                {
                   Sound.getInstance().playEffect("public/res/sound/time_click.mp3");
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

    sliderStateChange:function(sender,type)
    {
         switch(type){
            case ccui.Slider.EVENT_PERCENT_CHANGED:
                cc.log("musicSlider percent : " + sender.getPercent());
                break;
            default:
                break;
            }
        },

    reset:function()
    {
        // this.getUI("nopai").setVisible(false);
        this.showUIs1(this.passTips,false);
        this.showUIs1(this.inTips,false);
        this.showUIs(this.jbTips,false);
        this.showUIs(this.thinkTips,false);
        this.showUIs(this.quguanimage,false);
        this.showUIs(this.winImage,false);
        this.showUIs(this.allKuangTip,false);
        this.showUIs(this.mympLine,false);
        this.showUIs(this.allReadyTips,false);
        this.showUIs(this.lastCardNum,false);
        this.showUIs(this.typeActions,false);
        
        
        // this.showUIs(this.tipsBg,false);
        this.showUIs(this.mp,false);
        this.myTip.setVisible(false);
        this.allTip.setVisible(false);
        this.liujuTip.setVisible(false);
        this.dzTip.setVisible(false);
        this.clock.setVisible(false);
        this.dzimage.setVisible(false);

        this.szTip[1].setVisible(false);
        this.szTip[2].setVisible(false);
        this.myoutcartip1.setVisible(false);
        this.myoutcartip2.setVisible(false);
        this.myoutcartip3.setVisible(false);
        this.myoutcartip4.setVisible(false);
        this.firstoutcardtip1.setVisible(false);
        this.firstoutcardtip2.setVisible(false);
        this.splitCardTip.setVisible(false);
        this.splitCardTip2.setVisible(false);
        this.slider.setVisible(false);
        

        for(var i=0;i<3;++i)
        {
            this.tilaTxt[i] ="";
            this.piaoTxt[i] ="";
            this.stopWaitTime(i);
        }

        if(DDZModel.spadeoutcart == 1)
        {
             this.firsttipFlag = false;
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

    showMPAnimation:function(pos,vi)
    {
        if(DDZCards.checkmpList(DDZCards.mpPos,pos))
        {
            this.mympLine[pos].setVisible(vi);
            this.mympLine[pos].getAnimation().playWithIndex(0,-1,0);
        }
    },

    showQunaguan:function(pos,vi)
    {
        this.quguanimage[pos].setVisible(vi);
        
    },

    showLastCardNum:function(pos,vi,number)
    {
        this.lastCardNum[pos].setVisible(vi);
        utils.findNode(this.lastCardNum[pos],"num").setString(number);
    },


    showwin:function(pos,vi)
    {
        var tip =   this.winImage[pos];
        tip.setVisible(vi);
        tip.getAnimation().playWithIndex(0, -1,0);
       tip.runAction(cc.sequence(cc.delayTime(3),cc.hide()));
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
        if(type == 1)
        {
            this.myoutcartip3.setVisible(vi);
        }
        else if(type == 0)
        {
            this.myoutcartip2.setVisible(vi);
        }
        else 
        {
            this.myoutcartip1.setVisible(vi);
        }
    },

    showfisrtCardThree:function(vi)
    {
        if(DDZModel.gamePlayType != 1 && !this.firsttipFlag && DDZModel.spadeoutcart == 1&& DDZModel.maxPlayer ==3 )
            {
                this.myoutcartip4.setVisible(vi);
            }
    },

    hideMyOutCardTip:function()
    {
        this.myoutcartip1.setVisible(false);
        this.myoutcartip2.setVisible(false);
        this.myoutcartip3.setVisible(false);
        this.myoutcartip4.setVisible(false);
    },


    //发牌之后第一个出牌的提示
    showFirstOutCardTip:function(type,pos)
    {
        var posi = cc.p(0,0);
        if(pos == 0)
        {
            if(type == 1)
            {
                posi = cc.p(270,270);
            }else{
                posi = cc.p(230,270);
            }
            
        }else if(pos == 1)
        {
            if(type == 1)
            {   
                posi = cc.p(1010,500);
            }
            else{
                posi = cc.p(1050,500);
            }
        }else if(pos == 2)
        {
            if(type == 1)
            {
                 posi = cc.p(270,500);
            }else{
                posi = cc.p(230,500);
            }
        }
        if(type == 1)
        {
            this.firstoutcardtip2.setVisible(true);
            this.firstoutcardtip2.setPosition(posi);

            //this.firstoutcardtip2.getAnimation().setSpeedScale(0.7);
            if(pos ==1)
            {
                this.firstoutcardtip2.getAnimation().playWithIndex(1,-1,0);
            }
            else
            {
                this.firstoutcardtip2.getAnimation().playWithIndex(0,-1,0);
            }
        }
        else
        {
            this.firstoutcardtip1.setVisible(true);
            this.firstoutcardtip1.setPosition(posi);
           // this.firstoutcardtip1.getAnimation().setSpeedScale(0.7);
            if(pos ==1)
            {
                this.firstoutcardtip1.getAnimation().playWithIndex(1,-1,0);
            }
            else
            {
                this.firstoutcardtip1.getAnimation().playWithIndex(0,-1,0);
            }
        }

       // this.allKuangTip[pos].setVisible(true);
        
    },

    hideFirstOutCardTip:function()
    {
        this.firstoutcardtip1.setVisible(false);
        this.firstoutcardtip2.setVisible(false);
        this.showUIs(this.allKuangTip,false);
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
        if(!DDZModel.fastShow) {
            this.passTips[pos][key].runAction(cc.sequence(cc.delayTime(2), cc.hide()));
        }
        else
        {
            this.passTips[pos][key].setVisible(false);
        }
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
        this.showThink(0,false);
        this.showThink(1,false);
        this.showThink(2,false);
        DDZPlayers.setThink(0,false);
        DDZPlayers.setThink(1,false);
        DDZPlayers.setThink(2,false);
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
            var p = cc.p(90,305);
            if(pos ==1)
            {
                p =cc.p(1250,545);
            }
            else  if(pos ==2)
            {
                p =cc.p(90,545);
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


    //跑的快
    foPlayerOutCard:function(allcards,pos)
    {
        if(pos == -1)
        {
            log("------foPlayerOutCard-----seait- error-");
        }
        else
        {
            var endpos;
            var poses ={};
            poses[0] = DDZCards.handCardPos(0,allcards,this.cardW,this.dx,this.dy);
            poses[1] = DDZCards.handCardPos(1,allcards,this.ocardW,this.odx,this.ody);
            poses[2] = DDZCards.handCardPos(2,allcards,this.ocardW,this.odx,this.ody);
            utils.scaleToSize(this.dzimage,this.cardW,this.cardH);
            if(pos == 0){
                endpos = poses[pos][allcards];
                log("123:"+allcards);
            }
            else if(pos ==1)
            {
                cc.pAddIn(poses[pos][9],cc.p(-0.3,0.3));
                endpos = poses[pos][9];
                log("456:"+allcards);
            }
            else if(pos == 2)
            {
                cc.pAddIn(poses[pos][0],cc.p(-0.3,0.3));
                endpos = poses[pos][0];
                log("789:"+allcards);
            }   
            this.dzimage.setVisible(true);
            this.dzimage.setPosition(640,360);
            var sX=  this.dzimage.getScaleX();
            var sY=  this.dzimage.getScaleX();
            this.dzimage.runAction(cc.sequence(cc.scaleTo(0.2,sX*1.3,sY*1.3),cc.scaleTo(0.2,sX,sY),cc.scaleTo(0.2,sX*1.3,sY*1.3)
                ,cc.scaleTo(0.2,sX,sY),cc.moveTo(0.5, endpos),cc.hide()));
                // this.dzimage.runAction(cc.sequence(cc.moveBy(0.5,endpos),cc.delayTime(0.2),cc.hide()));
        }
    },

    //设置底注
    setCoin:function(coin)
    {
        // this.getUI("coin").setString(""+coin);
    },
    //设置倍率
    setRatio:function(v)
    {
        // this.getUI("coin").setString(""+coin);
    },

    setRadioAndCoin:function()
    {
        if(this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.QIANGGUAN) ||
            this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.MINGPAIFAPAI) ||
            this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.MINGPAIDAPAI) ||
            this.checkGameOptionlist(DDZModel.player_option,CARDGAMETYPE.BASEDOUBLE))
        {
            // this.getUI("coin").setString("底分 "+DDZModel.base_money + "倍数 "+DDZModel.curRadio);
            this.getUI("coin").setVisible(true);
            this.getUI("coin").setString("倍数 "+DDZModel.curRadio);
        }else{
             // this.getUI("coin").setString("底分 "+DDZModel.base_money);
             this.getUI("coin").setVisible(false);
        }
       
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
        // this.getUI("zha").setString(v+"炸");
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
            this.showPassTip("txt_fen_no1",i,false);
            this.showPassTip("txt_fen_no2",i,false);
            this.showPassTip("txt_fen_no3",i,false);
            this.showPassTip("txt_fen1",i,false);
            this.showPassTip("txt_fen2",i,false);
            this.showPassTip("txt_fen3",i,false);
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
    //显示飘
    showPiao:function(pos,num)
    {
        // this.tipsBg[pos].setVisible(true);
        if(num ==0)
        {
            this.piaoTxt[pos] ="不飘";
        }
        else
        {
            this.piaoTxt[pos] ="飘"+num+"分";
        }
        // this.tipsBg[pos].getChildByName("txt").setString(this.piaoTxt[pos]);
    },
    getNoPiaoPos:function()
    {
        for(var i =0 ;i<3;++i)
        {
            if(this.piaoTxt[i] =="")
            {
                return i;
            }
        }
        return -1;
    },
    //显示踢拉
    showTiLa:function(pos,tlType,tlAction)
    {
        //ti
        if(tlType == 1)
        {
            if(tlAction ==1)
            {
                this.tilaTxt[pos]="不踢";
            }
            else  if(tlAction ==2)
            {
                this.tilaTxt[pos]="踢X"+DDZModel.addRatios[DDZRatio.TI_LA];
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
                this.tilaTxt[pos]="不拉";
            }
            else  if(tlAction ==2)
            {
                this.tilaTxt[pos]="拉X"+DDZModel.addRatios[DDZRatio.TI_LA];
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
        if(this.piaoTxt[pos] !="" && this.tilaTxt[pos]!="")
        {
            // this.tipsBg[pos].setVisible( true);
            // this.tipsBg[pos].getChildByName("txt").setString(this.piaoTxt[pos] + "  "+this.tilaTxt[pos]);
        }
        else if(this.piaoTxt[pos] == "")
        {
            // this.tipsBg[pos].setVisible( this.tilaTxt[pos]!="");
            // this.tipsBg[pos].getChildByName("txt").setString(this.tilaTxt[pos]);
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
            if(pos!=i &&(this.tilaTxt[i]=="踢"||this.tilaTxt[i]==""))
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
        this.jbTips[pos].getAnimation().playWithIndex(0,-1,1);
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
        this.clockTo.setVisible(true);
        this.clockTo.setPosition(p);
        this.clockTo.setRotation(r);
    },

    hideClockTO:function(vi)
    {
        this.clockTo.setVisible(vi);
    },

    showSingleTime:function(num)
    {
        this.clockNum = num;
        this.clock.setVisible(true);
        this.clockTxt.setString(num);
    },

    changeSignTime:function(pos,num)
    {
        this.clockPos = pos;
        this.clockNum = num;
    },

    showSingleClockTo:function(pos)
    {
        this.clockPos = pos;
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
        DDZPlayers.setThink(pos,v);
        this.thinkTips[pos].think =v;
    },
    //显示超时提示
    showTimeOut:function(pos,v)
    {
        this.thinkTips[pos].setVisible(v);
        this.thinkTips[pos].stopAllActions();
        DDZPlayers.setThink(pos,v);
        this.thinkTips[pos].think =v;
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
                tip = "踢X"+num+"倍";
                break;
            case  DDZJiaBei.LA:
                tip = "拉X"+num+"倍";
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
        var t = new ccui.Text(tip,"",26);
        t.setAnchorPoint(cc.p(0,0.5));
        t.setPosition(870,22);
        t.setName("jiaBeiEf");
        this.uiNode.addChild(t,1000);
        t.runAction(cc.sequence(cc.delayTime(1),cc.fadeOut(0.5),cc.removeSelf()));
    },


    showWinCoin:function(pos,num)
    {
        if(DDZModel.fastShow)
        {
            return;
        }
        if (num == 0) {
            return;
        }

        var label=null;
        if (num > 0) {
            label = new  cc.LabelAtlas(":"+num,PackName+ "/res/font/dz_balance_num_win.png", 28, 40, '0');
        } else {
            label = new  cc.LabelAtlas(":"+(-num),PackName+ "/res/font/dz_balance_num_lose.png", 28, 40, '0');
        }
        label.setAnchorPoint(cc.p(0.5, 0.5));
        this.uiNode.addChild(label, 20000);
        var pn = DDZPlayers.players[pos].uiNode;
        label.setPosition(cc.p(pn.getPosition().x , pn.getPosition().y+pn.getContentSize().height*0.5- 10));

        label.runAction(cc.sequence(cc.moveBy(0.5, cc.p(0, 30)),cc.delayTime(3),cc.removeSelf()));

    },
    //展示牌型动画
    showCardEf:function(pos,type)
    {
        if(DDZModel.fastShow)
        {
            return;
        }
        var p =cc.p(640,390);
        switch (pos)
        {
            case 1:
                p = cc.p(1030,520);
                break;
            case 2:
                p = cc.p(250,520);
                break;
        }

        var tip = this.typeActions[type];
        if(tip)
        {
            tip.setPosition(p);
            tip.setVisible(true);
            tip.getAnimation().playWithIndex(0,-1,0);
            tip.runAction(cc.sequence(cc.delayTime(3),cc.hide()));
        }
        var sound ="";
        switch (type)
        {
            case libddz.CardsType.CARD_TYPE_ONELINE:
                sound = PackName+"/res/sound/effect/lianshun_sound.mp3";
                break;
            case libddz.CardsType.CARD_TYPE_TWOLINE:
                sound = PackName+"/res/sound/effect/lianshun_sound.mp3";
                break;
            case libddz.CardsType.CARD_TYPE_PLANEWITHONE:
            case libddz.CardsType.CARD_TYPE_PLANEWITHWING:
             case libddz.CardsType.CARD_TYPE_PLANEWHITHLACK:
                sound = PackName+"/res/sound/effect/plane_sound.mp3";
                break;
            case libddz.CardsType.CARD_TYPE_BOMB:
                sound = PackName+"/res/sound/effect/zd_rocket_sound.mp3";
                break;
        }


        if (sound!= "")
        {
            Sound.getInstance().playEffect(sound);
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
        cc.log("chat~~~~~~~~~~~~~~~~~~~")
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
        if (dat["filepath"])
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
    showMust:function()
    {
        this.must[0].setVisible(DDZModel.tableType ==0);
        this.must[4].setVisible(DDZModel.tableType ==1);
        for(var i in DDZModel.mustDz)
        {
            this.must[DDZModel.mustDz[i]].setVisible(true);
        }
        utils.sortNodes(this.must,640,565,20);
    },

    showMustTip:function()
    {
        this.mustip[0].setVisible(DDZModel.sign_rule == 1);
        this.mustip[1].setVisible(DDZModel.bomb_can_split == 0);
        this.mustip[2].setVisible(DDZModel.sign_rule == 0);

        this.mustip[3].setVisible(DDZModel.table_type == 15);
        this.mustip[4].setVisible(DDZModel.table_type == 16);

        utils.sortNodes(this.mustip,640,640,20);
    },

    showPass:function(is)
    {
        // this.getUI("nopai").setVisible(is);
    },
    showMp: function (pos) {

        var tip = this.mp[pos];
        tip.setVisible(true);
        tip.getAnimation().playWithIndex(0,-1,0);
        tip.runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },

    showAllPlayReadytip:function(is)
    {
        for(var i = 0 ;i < 3;i++)
        {
         DDZPlayers.setReady(i, false);
        }
    },

    showSplitCardTip:function(type,vi)
    {
        this.splitCardTip.setVisible(false);
        this.splitCardTip2.setVisible(false);
        if(type == 0)
        {
            this.splitCardTip.setVisible(vi);

        }else if(type == 1){
            this.splitCardTip2.setVisible(vi);
        }
        
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

    hideAllspltReadyTip:function()
    {
        for(var i=0;i<3;i++)
        {
            this.allReadyTips[i].setVisible(false);
        }
    },
    newCoin:function()
    {
        for(var i in this.coins)
        {
            if(!this.coins[i].isVisible())
            {
                return this.coins[i];
            }
        }
        var ret = new cc.Sprite(utils.spriteFrame("ddz/icon_coin.png"));
        this.uiNode.addChild(ret,100);
        this.coins.push(ret);
        return ret;
    },

    showbi:function(pos)
    {
        for(var i = 0;i<DDZModel.maxPlayer;i++)
        {
            if(i != pos)
            {
                this.showCoinTo(i,pos,10);
            }

        }
    },
    showCoinTo:function(f,t, num )
    {
        if(DDZModel.fastShow)
        {
            return;
        }
        var fPos = DDZPlayers.getPosition(f);
        var tPos =DDZPlayers.getPosition(t);

        var n = Math.abs(num) ;
        var dt =0;
        var sdt = 0;
        for(var j=0;j<n;++j)
        {
            dt += utils.randomf(0.05,0.1);
            var start =utils.clone(fPos) ;
            var c = this.newCoin();
            c.setVisible(true);
            c.setOpacity(0);
            c.setPosition(start);
            var end =utils.clone(tPos) ;
            c.runAction(cc.sequence(cc.delayTime(dt),cc.fadeIn(0),cc.moveTo(0.3,end),cc.delayTime(0.1),cc.hide()));

            if(sdt == 0 || sdt >= 0.2)
            {
                sdt = 0;
                this.uiNode.runAction(cc.sequence(cc.delayTime(dt), cc.callFunc(function () {
                    Sound.getInstance().playEffect(utils.format("%1/res/sound/coins.mp3", PackName));
                })));
            }
            sdt +=dt;
        }
    },
    showSZTip:function(pos,is,num)
    {
        this.szTip[pos].setVisible(is);
        if(is)
        {
            this.szTip[pos].getChildByName("txt").setString(num+"连顺");
        }
    }
};
