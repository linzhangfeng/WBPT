
var PHZTips =
{
    uiNode:null,
    clocks:[],
    action:{},
    hz:null,
    zhuan:null,
    myTip:null,
    allTip:null,
    huxi:[],
    niaos:[],
    piaos:[],
    ready:[],
    coins:[],
    huBase:null,
    playType:null,
    bigfaceTime:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        for(var i=0;i<3;++i)
        {
            this.clocks[i] = this.getUI("clockTo"+i);
            this.huxi[i] = this.getUI("hx"+i);
            this.ready[i] = this.getUI("ready"+i);
            utils.findNode(this.huxi[i],"num").ignoreContentAdaptWithSize(true);
        }

        this.getUI("difen").ignoreContentAdaptWithSize(true);

        this.waitTip =this.getUI("waitTip");
        this.waitTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.myTip =this.getUI("myTip");
        this.myTip.getChildByName("txt").ignoreContentAdaptWithSize(true);
        this.allTip =this.getUI("allTip");
        this.allTip.getChildByName("txt").ignoreContentAdaptWithSize(true);

        this.playTypes =[];
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type0"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type2"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type3"));
        this.playTypes.push(utils.findNode(this.uiNode.getParent(),"type1"));
        this.huBase =utils.findNode(this.uiNode.getParent(),"type0").getChildByName("num");
        this.huBase.ignoreContentAdaptWithSize(true);
        this.huMax =utils.findNode(this.uiNode.getParent(),"type1").getChildByName("num");
        this.huMax.ignoreContentAdaptWithSize(true);
        this.playType =utils.findNode(this.uiNode.getParent(),"type");
        this.playTypes[3].setAnchorPoint(cc.p(0.5,0.5));
        this.playTypes[3].setContentSize(cc.size(230,30));
        this.playTypes[0].setAnchorPoint(cc.p(0.5,0.5));
        this.playTypes[0].setContentSize(cc.size(200,30));
        this.showUIs(this.playTypes,false);

        this.yh = this.getUI("yh");

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

        this.xg = this.getUI("xg");
        this.xgTip = this.getUI("xgTip");
        for(var i =0;i<3;++i)
        {
            this.niaos.push(this.getUI("niao"+i));
            this.piaos.push(this.getUI("piao"+i));
            this.niaos[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
            this.piaos[i].getChildByName("txt").ignoreContentAdaptWithSize(true);
        }

        this.zhuan = this.getUI("zhuan");
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
        this.showUIs(this.niaos,false);
        this.showUIs(this.piaos,false);
        this.showUIs(this.huxi,false);
        this.showUIs(this.action,false);
        this.showUIs(this.ready,false);
        this.hz.setVisible(false);
        this.start.setVisible(false);
        this.xg.setVisible(false);
        this.xgTip.setVisible(false);
        this.zhuan.setVisible(false);
        this.yh.setVisible(false);
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
    showActionTip:function(pos,ack)
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
                    tip.runAction(cc.sequence(cc.delayTime(2),cc.hide()));
                }
            }
        }

        var sound ="";
        switch (ack)
        {
            case PHZAck.ACK_CHI:
            case PHZAck.ACK_CHI_EX:
                sound = utils.format("%1/res/sound/%2/%3_chi.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_PAO:
                sound = utils.format("%1/res/sound/%2/%3_pao.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_PENG:
                sound = utils.format("%1/res/sound/%2/%3_peng.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_WEI:
                sound = utils.format("%1/res/sound/%2/%3_wei.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_TI:
                sound = utils.format("%1/res/sound/%2/%3_ti.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_CW:
                sound = utils.format("%1/res/sound/%2/%3_cw.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
                break;
            case PHZAck.ACK_HU:
                sound = utils.format("%1/res/sound/%2/%3_hu.mp3",PackName,PHZModel.soundIndex,PHZPlayers.getSex(pos));
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
    showZhuan:function(pos)
    {
        this.zhuan.setVisible(true);
        var p = cc.p(90,120);
        switch (pos)
        {
            case 1:
                p = cc.p(1280-30,615);
                break;
            case 2:
                p = cc.p(90,615);
                break;
        }
        if(PHZModel.fastShow)
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

    },
    //显示胡息数量
    showHuxi:function(pos,num)
    {
        this.huxi[pos].setVisible(num>=0);
        if(num>=0)
        {
            utils.findNode(this.huxi[pos],"num").setString(num+"");
        }

    },

    showDiFen:function(df,hz)
    {
        this.difen = df;
        if(hz>=0)
        {
            this.getUI("difen").setString("底分 "+df+"    黄庄"+hz+"次");
        }
        else
        {
            this.getUI("difen").setString("底分 "+df);
        }
    },
    showHzNum:function(hz)
    {
        if(hz>=0)
        {
            this.getUI("difen").setString("底分 "+this.difen+"    黄庄"+hz+"次");
        }
        else
        {
            this.getUI("difen").setString("底分 "+this.difen);
        }
    },
    showClock:function(pos,time) {
        this.hideClock();
        if(pos <0&&pos>2)
        {
            return;
        }
        this.clocks[pos].setVisible(true);
        this.clocks[pos].time = time;
        var self =this;
        var action = cc.repeat(cc.sequence(cc.delayTime(1),cc.callFunc(function () {
            self.clocks[pos].time --;

            if(pos == 0)
            {
                if(self.clocks[pos].time ==3)
                {
                    PlatformHelper.vibrate();
                }
                if(self.clocks[pos].time<=3)
                {
                    Sound.getInstance().playEffect("public/res/sound/time_click.mp3");
                }
            }
            if(self.clocks[pos].time==0)
            {
                PHZPlayers.setThink(pos,true);
                self.clocks[pos].stopAllActions();
            }
        })),time);
        action.setTag(88);
        this.clocks[pos].runAction(action);
    },
    hideClock:function(pos)
    {
        if(pos)
        {
            this.clocks[pos].setVisible(false);
            this.clocks[pos].time =0;
            this.clocks[pos].stopAllActions();
            PHZPlayers.setThink(pos,false);
        }
        else
        {
            for(var i in this.clocks)
            {
                PHZPlayers.setThink(i,false);
                this.clocks[i].setVisible(false);
                this.clocks[i].time =0;
                this.clocks[i].stopAllActions();
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
    showHuBase:function(num)
    {
        var s =""+num;
        var s1 = Math.floor(num/10);
        var s2 = num%10;
        if(s1 == 1)
        {
            if(s2==0)
            {
                s = "0";
            }
            else
            {
                s = "0"+s2;
            }
        }
        else if(s1 > 1)
        {
            if(s2==0)
            {
                s = s1+"0";
            }
            else
            {
                s = s1+"0"+s2;
            }
        }
      this.huBase.setString(s);
    },
    showHuMax:function(num)
    {
        var s =""+num;
        var s1 = Math.floor(num/100);
        var s2 = num%100;
        if(s1 >= 1)
        {
            s = s1+":";
            if( Math.floor(s2/10)>0)
            {
                s+=Math.floor(s2/10)+"0";
                if((s2%10)>0)
                {
                    s+=(s2%10);
                }
            }
            else
            {
                if((s2%10)>0)
                {
                    s+=(s2%10);
                }
            }
        }
        this.huMax.setString(s);
        this.huMax.setPositionX(this.huMax.getContentSize().width);

        var type = utils.findNode(this.playTypes[3],"type");
        type.setPositionX(this.huMax.getContentSize().width+2);
        this.huMax.setPositionX(this.huMax.getContentSize().width);
        var size = this.huMax.getContentSize().width + utils.findNode(this.playTypes[3],"type").getContentSize().width+2;
        this.playTypes[3].setContentSize(cc.size(size,30));
        utils.sortNodes(this.playTypes,640,500,20);
    },
    showPlayTypes:function()
    {
        this.showUIs(this.playTypes,false);
        if(PHZModel.endScore <0)
        {
            this.playTypes[0].setVisible(true);
            this.playTypes[1].setVisible(true);
        }
        else
        {
            this.playTypes[0].setVisible(true);
            this.playTypes[2].setVisible(true);
            this.playTypes[3].setVisible(true);
        }
        utils.sortNodes(this.playTypes,640,500,20);
    },
    showPlayType:function(type)
    {
        this.type = type;
        var sp = utils.spriteFrame("phz/"+type+".png");
        if(sp)
        {
            this.playType.setSpriteFrame(sp);
        }
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

    showYH:function()
    {
        this.yh.setVisible(true);
        Sound.getInstance().playEffect(utils.format("%1/res/sound/yanhua.mp3", PackName));
        this.yh.getAnimation().playWithIndex(0,-1,1);
        this.yh.runAction(cc.sequence(cc.delayTime(2),cc.callFunc(function(){
            Sound.getInstance().playEffect(utils.format("%1/res/sound/yanhua.mp3", PackName));
        }),cc.delayTime(2),cc.hide()));

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
    getPlayersPos:function(pos)
    { var p2 = cc.p(63,66);
        switch (pos)
        {
            case 2:
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
    showCoinTo:function(f,t, num )
    {
        var fPos = this.getPlayersPos(f);
        var tPos = this.getPlayersPos(t);

        var n = Math.abs(num) ;
        var dt =0;
        var sdt = 0;
        for(var j=0;j<n;++j)
        {
            dt += utils.randomf(0,0.08);
            var start =utils.clone(fPos) ;
            var c = this.newCoin();
            c.setVisible(true);
            c.setOpacity(0);
            c.setPosition(start);
            var end =utils.clone(tPos) ;
            c.runAction(cc.sequence(cc.delayTime(dt),cc.fadeIn(0),cc.moveTo(0.5,end),cc.delayTime(0.1),cc.hide()));

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
    showCoinSound:function() {
    },
    showReady:function(pos,is)
    {
        this.ready[pos].setVisible(is);
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
