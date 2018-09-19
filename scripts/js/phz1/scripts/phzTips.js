
var PHZTips =
{
    uiNode:null,
    action:{},
    hz:null,
    zhuan:null,
    myTip:null,
    allTip:null,
    huxi:[],
    lossHu:[],
    winHu:[],
    niaos:[],
    piaos:[],
    coins:[],
    jb:[],
    playTypes:[],
    bigfaceTime:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.waitTip =this.getUI("waitTip");
        this.waitTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.clock = utils.findNode(this.uiNode.getParent(),"to");
        this.clock.getChildByName("num").ignoreContentAdaptWithSize(true);
        for(var i=0;i<4;++i)
        {
            this.jb[i] = this.getUI("jb"+i);
            this.jb[i].runAction(cc.repeatForever(utils.newAni("phz/effects_jinbaodeng_000",0,7,0.1)));
            this.huxi[i] = this.getUI("hx"+i);
            utils.findNode(this.huxi[i],"num").ignoreContentAdaptWithSize(true);
            this.lossHu[i] = this.getUI("loss"+i);
            this.lossHu[i].pos =  this.lossHu[i].getPosition();
            this.winHu[i] = this.getUI("win"+i);
            this.winHu[i].pos =  this.winHu[i].getPosition();
            utils.findNode(this.lossHu[i],"num").ignoreContentAdaptWithSize(true);
            utils.findNode(this.winHu[i],"num").ignoreContentAdaptWithSize(true);
        }

        utils.findNode(this.uiNode.getParent(),"difen").ignoreContentAdaptWithSize(true);

        this.playType =utils.findNode(this.uiNode.getParent(),"type");
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type1"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type2"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type3"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type0"));
        this.playTypes[1].setAnchorPoint(cc.p(0.5,0.5));
        this.playTypes[1].setContentSize(cc.size(130,40));
        this.playTypes[1].getChildByName("num").setString(":2");
        this.playTypes[1].getChildByName("num").ignoreContentAdaptWithSize(true);
        this.showUIs(this.playTypes,false);

        this.myTip =this.getUI("myTip");
        this.myTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.allTip =this.getUI("allTip");
        this.allTip.getChildByName("txt").ignoreContentAdaptWithSize(true);


        this.action[PHZAck.ACK_CHI] = this.getUI("chi");
        this.action[PHZAck.ACK_CHI_EX] = this.getUI("chi");
        this.action[PHZAck.ACK_HU] = this.getUI("hu");
        this.action[PHZAck.ACK_PAO] = this.getUI("pao");
        this.action[PHZAck.ACK_PENG] = this.getUI("peng");
        this.action[PHZAck.ACK_TI] = this.getUI("ti");
        this.action[PHZAck.ACK_WEI] = this.getUI("wei");
        this.action[PHZAck.ACK_CW] = this.getUI("cw");
        this.action[PHZAck.ACK_DIAN_PAO] = this.getUI("dianpao");
        this.hz = this.getUI("hz");
        this.start = this.getUI("start");
        this.yh = this.getUI("yh");

        this.xg = this.getUI("xg");
        this.xgTip = this.getUI("xgTip");
        for(var i =0;i<4;++i)
        {
            this.niaos.push(this.getUI("niao"+i));
            this.piaos.push(this.getUI("piao"+i));
            this.niaos[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
            this.piaos[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
        }

        this.zhuan = this.getUI("zhuan");
        this.getUI("lian").getChildByName("num").ignoreContentAdaptWithSize(true);

        this.reset();
        var self = this;
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
        this.myTip.setVisible(false);
        this.allTip.setVisible(false);
        this.showUIs(this.jb,false);
        this.showUIs(this.niaos,false);
        this.showUIs(this.piaos,false);
        this.showUIs(this.huxi,false);
        this.showUIs(this.action,false);
        this.showUIs(this.lossHu,false);
        this.showUIs(this.winHu,false);
        this.yh.setVisible(false);
        this.hz.setVisible(false);
        this.start.setVisible(false);
        this.xg.setVisible(false);
        this.xgTip.setVisible(false);
        this.zhuan.setVisible(false);
        this.waitTip.setVisible(false);
        this.hideClock();
        //this.showDiFen(1);
    },
    getUI:function(key)
    {
        return  utils.findNode(this.uiNode,key);
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
                    else if(end.x <640)
                    {
                        end.x =450;
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

        var p = PHZModel.getPlayerByUID(uid);
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
            this.showChat(PHZModel.getPosBySeatID(p.seatid),content,isSound,utils.getValInt(dat,"time"));
            var file = utils.getFastChatSound(p.sex ==1 ?"m":"w",content);
            if(file!="")
            {
                Sound.getInstance().playEffect("public/res/sound/fastchat/"+file);
            }
        }

    },

    showChat:function(pos,content,isSound,time)
    {
        var bg =new PHZPlayerChat();
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


        var p =  PHZPlayers.getPosition(pos);
        if (pos == 0)
        {
            bg.setAnchorPoint(cc.p(0, 0.5));
            bg.setPosition(p.x + 50, p.y+50);
        }
        else if (pos == 1)
        {
            bg.setAnchorPoint(cc.p(0, 0.5));
            bg.setPosition(p.x - 50, p.y+30);
        }
        else if (pos == 2)
        {
            bg.setAnchorPoint(cc.p(0.0, 0.5));
            bg.setPosition(p.x + 50, p.y+30);
        }
        else if (pos == 3)
        {
            bg.setAnchorPoint(cc.p(0.0, 0.5));
            bg.setPosition(p.x + 50, p.y+20);
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
    //显示操作提示
    showActionTip:function(pos,ack,ackEx)
    {
        var tip = this.action[ack];
        if(tip)
        {
            tip.setVisible(true);
            var p =cc.p(0,0);
            switch (pos)
            {
                case 0:
                    p.x = 640;
                    p.y = 330;
                    break;
                case 1:
                    p.x = 1280-300;
                    p.y = 480;
                    break;
                case 2:
                    p.x = 640;
                    p.y = 540;
                    break;
                case 3:
                    p.x = 300;
                    p.y = 480;
                    break;
            }
            tip.setPosition(p);
            if(ack==PHZAck.ACK_DIAN_PAO)
            {
                tip.getAnimation().play("effectspaohzph1",-1,0);
                tip.runAction(cc.sequence(cc.delayTime(1),cc.callFunc(function(){
                    tip.getAnimation().play("effectspaohzph2",-1,1);
                })));
            }
            else
            {
                tip.getAnimation().playWithIndex(0, -1, 0);
                if(ack==PHZAck.ACK_HU)
                {
                    tip.runAction(cc.sequence(cc.delayTime(3),cc.hide()));
                }
                else
                {
                    tip.runAction(cc.sequence(cc.delayTime(1.5),cc.hide()));
                }
            }

        }

        var sound ="";
        switch (ack)
        {
            case PHZAck.ACK_CHI:
            case PHZAck.ACK_CHI_EX:
                if(ackEx ==1)
                {
                    sound = utils.format("%1/res/sound/%2/%3_bi.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else
                {
                    sound = utils.format("%1/res/sound/%2/%3_chi.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }

                break;
            case PHZAck.ACK_PAO:
                sound = utils.format("%1/res/sound/%2/%3_pao.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_PENG:
                if(ackEx ==PHZPwType.TYPE_SAN_DA)
                {
                    sound = utils.format("%1/res/sound/%2/%3_peng3.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else if(ackEx ==PHZPwType.TYPE_SI_QING)
                {
                    sound = utils.format("%1/res/sound/%2/%3_peng4.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else
                {
                    sound = utils.format("%1/res/sound/%2/%3_peng.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                break;
            case PHZAck.ACK_WEI:
                if(ackEx ==PHZPwType.TYPE_SAN_DA)
                {
                    sound = utils.format("%1/res/sound/%2/%3_wei3.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else if(ackEx ==PHZPwType.TYPE_SI_QING)
                {
                    sound = utils.format("%1/res/sound/%2/%3_wei4.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else
                {
                    sound = utils.format("%1/res/sound/%2/%3_wei.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                break;
            case PHZAck.ACK_TI:
                sound = utils.format("%1/res/sound/%2/%3_ti.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_CW:
                if(ackEx ==PHZPwType.TYPE_SAN_DA)
                {
                    sound = utils.format("%1/res/sound/%2/%3_wei3.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else if(ackEx ==PHZPwType.TYPE_SI_QING)
                {
                    sound = utils.format("%1/res/sound/%2/%3_wei4.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                }
                else {
                    sound = utils.format("%1/res/sound/%2/%3_wei.mp3", PackName, PHZModel.soundIndex, PHZPlayers.getSex(pos));
                }

                break;
            case PHZAck.ACK_HU:
                if(ackEx>0)
                {
                    sound = utils.format("%1/res/sound/%2/%3_hu_%4.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos),ackEx);
                }
                break;
        }
        if(sound!="")
        {
            Sound.getInstance().playEffect(sound);
        }
    },
    //显示荒庄提示
    showHz:function()
    {
        this.hz.setVisible(true);
        this.hz.getAnimation().playWithIndex(0,-1,0);
        this.hz.runAction(cc.sequence(cc.delayTime(2),cc.hide()));
    },
    //显示庄提示
    showZhuan:function(pos,num)
    {
        this.zhuangNum = num;
        if(num<=0)
        {
            return;
        }
        this.zhuan.setVisible(true);
        var p = cc.p(90,100);
        switch (pos)
        {
            case 1:
                p = cc.p(1280-90,595);
                break;
            case 2:
                p = cc.p(575,690);
                break;
            case 3:
                p = cc.p(90,595);
                break;
        }

        if( PHZModel.fastShow)
        {
            this.zhuan.setPosition(p);
            this.zhuan.setScale(1);
        }
        else
        {
            this.zhuan.setPosition(640,360);
            this.zhuan.setScale(0);
            this.zhuan.runAction(cc.sequence(cc.scaleTo(0.3,1.3),cc.spawn(cc.moveTo(0.2,p)
                ,cc.delayTime(0.2),cc.scaleTo(0.2,1))));
        }

        utils.findNode(this.zhuan,"num").setString(num+1);

    },
    //显示胡息数量
    showHuxi:function(pos,num)
    {
        this.huxi[pos].setVisible(num);
        this.huxi[pos].num = num;
        utils.findNode(this.huxi[pos],"num").setString(num+"");
    },
getHuxi:function(pos)
{
  return   this.huxi[pos].num;
},
    hideHuxi:function(pos)
    {
        this.huxi[pos].setVisible(false);
    },
    showPlayType:function(type)
    {
        var sp = utils.spriteFrame("phz/"+type+".png");
        if(sp)
        {
            this.playType.setSpriteFrame(sp);
        }
    },
    showPlayTypes:function(qz,zhong,lian,kan)
    {
        this.zhongZhuang= zhong;
        this.lianZhuang= lian;
        utils.findNode(this.zhuan,"lian1").setVisible(zhong);
        utils.findNode(this.zhuan,"lian").setVisible(lian);
        this.showUIs(this.playTypes,false);
            this.playTypes[0].setVisible(qz);
            this.playTypes[1].setVisible(zhong);
            this.playTypes[2].setVisible(lian);
        this.playTypes[3].setVisible(!kan);
        utils.sortNodes(this.playTypes,640,500,20);
    },
    showDiFen:function(df,hz)
    {
        this.difen = df;
        if(hz>=0)
        {
            utils.findNode(this.uiNode.getParent(),"difen").setString("底分 "+df+"    黄庄"+hz+"次");
        }
        else
        {
            utils.findNode(this.uiNode.getParent(),"difen").setString("底分 "+df);
        }
    },
    showHzNum:function(hz)
    {
        if(hz>=0)
        {
            utils.findNode(this.uiNode.getParent(),"difen").setString("底分 "+this.difen+"    黄庄"+hz+"次");
        }
        else
        {
            utils.findNode(this.uiNode.getParent(),"difen").setString("底分 "+this.difen);
        }
    },
    showClock:function(pos,time) {
        this.hideClock();
        this.clock.setVisible(true);
        this.clock.time = time;
        utils.findNode(this.clock,"num").setString(time+"");
        var self =this;
        var action = cc.repeat(cc.sequence(cc.delayTime(1),cc.callFunc(function () {
            self.clock.time --;
            utils.findNode(self.clock,"num").setString(self.clock.time+"");

            if(pos == 0)
            {
                if(self.clock.time ==3)
                {
                    PlatformHelper.vibrate();
                }
                if(self.clock.time<=3)
                {
                    Sound.getInstance().playEffect("public/res/sound/time_click.mp3");
                }
            }
            if(self.clock.time==0)
            {
                PHZPlayers.setThink(pos,true);
                self.clock.stopAllActions();
            }
        })),time);
        action.setTag(88);
        this.clock.runAction(action);
        switch(pos)
        {
            case 0:
                this.clock.getChildByName("num").setRotation(-90);
                this.clock.setRotation(90);
                this.clock.setPosition(640,390);
                break;
            case 1:
                this.clock.getChildByName("num").setRotation(0);
                this.clock.setRotation(0);
                this.clock.setPosition(720,430);
                break;
            case 2:
                this.clock.getChildByName("num").setRotation(90);
                this.clock.setRotation(-90);
                this.clock.setPosition(640,480);
                break;
            case 3:
                this.clock.getChildByName("num").setRotation(180);
                this.clock.setRotation(180);
                this.clock.setPosition(560,430);
                break;
        }
    },
    hideClock:function(pos)
    {
        this.clock.setVisible(false);
        this.clock.time =0;
        this.clock.stopAllActions();
        if(pos)
        {
            PHZPlayers.setThink(pos,false);
        }
        else
        {
            for(var i =0;i<4;++i)
            {
                PHZPlayers.setThink(i,false);
            }
        }
    },
    showNiao:function(pos,val)
    {
        if(val>=0)
        {
            this.niaos[pos].data = val;
        }
        this.niaos[pos].setVisible(val>=0);
        this.niaos[pos].getChildByName("txt").setString("活鸟"+val+"只");
    },
    getNoNiaoPos:function()
    {
      for(var i=0;i<3;++i)
      {
          if(! this.niaos[i].isVisible())
          {
              return i;
          }
      }
        return -1;
    },
    showPiao:function(pos,val)
    {
        if(val>=0)
        {
            this.piaos[pos].data = val;
        }
        this.piaos[pos].setVisible(val>=0);
        this.piaos[pos].getChildByName("txt").setString("拖码"+val+"分");
    },
    getNoPiaoPos:function()
    {
        for(var i=0;i<3;++i)
        {
            if(! this.piaos[i].isVisible())
            {
                return i;
            }
        }
        return -1;
    },
    showNiaos:function(is)
    {
        this.showUIs(this.niaos,is);
    },
    showPiaos:function(is)
    {
        this.showUIs(this.piaos,is);
    },
    hasNiao:function()
    {
        return this.niaos[0].hasOwnProperty("data");
    },
    hasPiao:function()
    {
        return this.piaos[0].hasOwnProperty("data");
    },
    showXiangGong:function(pos)
    {
        var p =cc.p(0,0);
        switch (pos)
        {
            case 0:
                p.x = 640;
                p.y = 350;
                break;
            case 1:
                p.x = 1280-300;
                p.y = 480;
                break;
            case 2:
                p.x = 300;
                p.y = 480;
                break;
        }
        this.xg.setPosition(p);
        this.xg.setVisible(true);
        this.xg.getAnimation().playWithIndex(0,-1,0);
        this.xg.runAction(cc.sequence(cc.delayTime(2),cc.hide()));
        if(pos == 0)
        {
            this.xgTip.setVisible(true);
        }
    },
    showStart:function()
    {
        this.start.setVisible(true);
        this.start.getAnimation().playWithIndex(0,-1,0);
        this.start.runAction(cc.sequence(cc.delayTime(3),cc.hide()));
    },
    showHuNum:function(pos, num)
    {
        this.lossHu[pos].setVisible(num<0);
        this.winHu[pos].setVisible(num>0);
        this.lossHu[pos].stopAllActions();
        this.winHu[pos].stopAllActions();
        if(num<0)
        {
            utils.findNode(this.lossHu[pos],"num").setString(":"+(-num));

            var dis = this.lossHu[pos].pos;
            var start = cc.p(640+(dis.x-640)*0.3,430+(dis.y-430)*0.3);
            this.lossHu[pos].setOpacity(0);
            this.lossHu[pos].setPosition(start);
            this.lossHu[pos].runAction(cc.sequence(cc.spawn(cc.sequence(cc.fadeIn(0.5),cc.delayTime(2),cc.fadeOut(0.5)),cc.moveTo(3, this.lossHu[pos].pos)),cc.hide()));
        }
        else  if(num>0)
        {
            utils.findNode(this.winHu[pos],"num").setString(num);
            var dis = this.winHu[pos].pos;
            var start = cc.p(640+(dis.x-640)*0.3,430+(dis.y-430)*0.3);
            this.winHu[pos].setOpacity(0);
            this.winHu[pos].setPosition(start);
            this.winHu[pos].runAction(cc.sequence(cc.spawn(cc.sequence(cc.fadeIn(0.5),cc.delayTime(2),cc.fadeOut(0.5)),cc.moveTo(3, this.winHu[pos].pos)),cc.hide()));
        }
    },
    showJb:function(pos,is)
    {
        this.jb[pos].setVisible(is);

        if(is)
        {
            utils.format("%1/res/sound/%2/%3_jb.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
        }
    },
    showYH:function()
    {
        this.yh.setVisible(true);
        Sound.getInstance().playEffect(utils.format("%1/res/sound/yanhua.mp3", PackName));
        this.yh.getAnimation().playWithIndex(0,-1,1);
        this.yh.runAction(cc.sequence(cc.delayTime(2),cc.callFunc(function(){
            Sound.getInstance().playEffect(utils.format("%1/res/sound/yanhua.mp3", PackName));
        }),cc.delayTime(2),cc.hide()));

    },
    getPlayersPos:function(pos)
    { var p2 = cc.p(63,66);
        switch (pos)
        {
            case 2:
                p2.x =550;
                p2.y =670;
                break;
            case 3:
                p2.x =63;
                p2.y =560;
                break;
            case 1:
                p2.x =1280-63;
                p2.y =560;
                break;
        }
        return p2;
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
        var ret = new cc.Sprite(utils.spriteFrame("phz/icon_coin.png"));
        this.uiNode.addChild(ret,100);
        this.coins.push(ret);
        return ret;
    },
    showCoinTo:function(f,t, num )
    {
        var fPos = this.getPlayersPos(f);
        var tPos = this.getPlayersPos(t);

        var n = Math.abs(num) ;
        var dt =0.4;
        for(var j=0;j<n;++j)
        {
            dt += utils.randomf(0.02,0.08);
            var start =utils.clone(fPos) ;
            var c = this.newCoin();
            c.setVisible(true);
            c.setOpacity(0);
            c.setPosition(start);
            var end =utils.clone(tPos) ;
            c.runAction(cc.sequence(cc.delayTime(dt),cc.fadeIn(0),cc.moveTo(0.5,end),cc.delayTime(0.1),cc.hide()));

        }
        return dt;
    },
    showCoinSound:function(dt) {
        var  n = Math.floor((dt-0.6)/0.2);
        n =  Math.max(n,0);
        for(var j=0;j<=n;++j)
        {
            this.uiNode.runAction(cc.sequence(cc.delayTime(0.6+j*0.2), cc.callFunc(function () {
                Sound.getInstance().playEffect(utils.format("%1/res/sound/coins.mp3", PackName));
            })));
        }
    },
    showWait:function(state)
    {
        if(PHZModel.fastShow)
        {
            return;
        }
        this.waitTip.setVisible(state !=0);
        if(state ==1)
        {
            var txt =this.waitTip.getChildByName("txt");
            txt.setString("等待其他玩家操作");
            txt.setTextColor(cc.color.YELLOW);
        }
        else if(state ==2)
        {
            var txt =this.waitTip.getChildByName("txt");
            txt.setString("其他玩家已过牌，您可以操作了");
            txt.setTextColor(cc.color.WHITE);
        }
    }

};
