
var CXChip = cc.Node.extend(
    {
        uiNode: null,
        value:-1,

        ctor:function ()
        {
            this._super();
            var json = ccs.load(PackName+"/res/chip.json");
            this.uiNode =json.node;
            this.addChild(this.uiNode);
            this.setContentSize(this.uiNode.getContentSize());
            this.setAnchorPoint(cc.p(0.5,0.5));
            this.uiNode.getChildByName("money_bg").loadTexture("CX/money.png",1);
            this.uiNode.getChildByName("money_bg").getChildByName("num").ignoreContentAdaptWithSize(true);
            this.uiNode.getChildByName("money_bg").getChildByName("num").setVisible(false);
            this.reset();
        },

        getUI:function(key)
        {
            return this.uiNode.getChildByName(key);
        },
        showUI:function(key ,is)
        {
            this.getUI(key).setVisible(is);
        },
        //设置牌点数
        setValue:function(value)
        {
            this.value = value;
            this.uiNode.getChildByName("money_bg").getChildByName("num").setString(value);
        },

        getValue:function()
        {
            return this.value;
        },


        reset:function()
        {
            this.stopAllActions();
            this.setVisible(false);
        },

    });

var CXChips = {

    uiNode: null,
    playChip:{},

    pichip:[],

    recordchipNum:[],


    init:function()
    {
        for(var i =0;i<6;i++)
        {
            this.playChip[i] = [];
        }
    },

    newChip:function()
    {
        var temChip = new CXChip();
        return temChip;
    },

    sendBetMoney:function(pos,value,number,sendbet)
    {
        if(number <= 0)
        {
            return;
        }
        for(var i = 0; i<number;i++)
        {
            var chip = this.newChip();
            chip.setValue(value);
            this.playChip[pos].push(chip);
        }
        this.sendBetMoney_ani(pos,sendbet);
    },


    reset:function()
    {
        for(var i = 0; i < 6; i++)
        {
            var number = this.recordchipNum[i];
            for(var k = 0; k< number; k++)
            {
                var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART+i*300 + k));
                if(chip != null)
                {
                    this.uiNode.removeChild(chip,true);
                    chip = null;
                }
            }
            this.playChip[i] = [];

            var betbg = this.uiNode.getChildByTag(parseInt(CardTagUM.SENDBETMONEY_BG + i));
            if(betbg != null)
            {
                betbg.removeAllChildren();
                this.uiNode.removeChild(betbg);
                betbg = null;
            }
        }
        this.recordchipNum = [];
        this.pichip = [];

        this.addRemoveChips();

        cc.spriteFrameCache.addSpriteFrames("cx/res/img/common.plist");
    },

    addRemoveChips:function()
    {
        for(var i =0;i<6;i++)
        {
            this.recordchipNum[i] = 50;
        }


        for(var i = 0; i < 6; i++)
        {
            var number = this.recordchipNum[i];
            for(var k = 0; k< number; k++)
            {
                var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART+i*300 + k));
                if(chip != null)
                {
                    this.uiNode.removeChild(chip,true);
                    chip = null;
                }
            }

            var betbg = this.uiNode.getChildByTag(parseInt(CardTagUM.SENDBETMONEY_BG + i));
            if(betbg != null)
            {
                betbg.removeAllChildren();
                this.uiNode.removeChild(betbg);
                betbg = null;
            }

            for(var k = 0; k< 20; k++)
            {
                var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPPISTART+i*20 + k));
                if(chip != null)
                {
                    this.uiNode.removeChild(chip,true);
                    chip = null;
                }
            }
        }

        this.recordchipNum = [];
    },

    v_recordChipNum:function()
    {
        if(this.recordchipNum.length <= 0)
        {
            for(var i =0;i<6;i++)
            {
                this.recordchipNum[i] = this.playChip[i].length;
            }
        } 
    },

    resetPlayerchip:function(pos)
    {
        var number = this.playChip[pos].length;
        for(var k = 0; k< number; k++)
        {
            
            var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART+i*300 + k));
            if(chip != null)
            {
                this.uiNode.removeChild(chip,true);
                chip = null;
            }
        }
        this.playChip[pos] = [];

        var betbg = this.uiNode.getChildByTag(parseInt(CardTagUM.SENDBETMONEY_BG + pos));
        if(betbg != null)
        {
            betbg.removeAllChildren();
            this.uiNode.removeChild(betbg);
            betbg = null;
        }    
    },
    
    sendBetMoney_ani:function(pos,sendbet)
    {
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var Play_position = players.getChildByName("player"+pos).getPosition();
        var startpos = utils.getChipStartPos(pos,Play_position);
        var endpos = utils.getChipEndPos(pos,Play_position);
        for(var i = 0 ; i< this.playChip[pos].length; i++)
        {
            var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART+pos*300 + i));
            if(chip == null)
            {
                this.playChip[pos][i].setVisible(true);
                this.playChip[pos][i].setPosition(Play_position);
                this.playChip[pos][i].setTag(parseInt(CardTagUM.CHIPSTART+pos*300 + i));
                this.uiNode.addChild(this.playChip[pos][i]);
                var movetoPosition = cc.p(this.randNum(startpos.x,endpos.x),this.randNum(startpos.y,endpos.y));
                (function(c,mpostion)
                {
                    c.runAction(cc.sequence(cc.moveTo(0.2,mpostion)));
                })(this.playChip[pos][i],movetoPosition);
            }
            else
            {
                log("==================herer====================");
                chip.setVisible(true);
                var _width = endpos.x - startpos.x;
                var _height = endpos.y - startpos.y;
                var rect = cc.rect(startpos.x,startpos.y,_width,_height);
                if(!cc.rectContainsPoint(rect,chip.getPosition()))
                {
                    var movetoPosition = cc.p(this.randNum(startpos.x,endpos.x),this.randNum(startpos.y,endpos.y));
                    chip.setPosition(movetoPosition);
                }
            }
        }

        var betbg = this.uiNode.getChildByTag(parseInt(CardTagUM.SENDBETMONEY_BG + pos));
        if(betbg == null)
        {
             betbg = new cc.Sprite(utils.spriteFrame("CX/money_bar.png"));
             betbg.setTag(parseInt(CardTagUM.SENDBETMONEY_BG + pos));
             var bgpos = utils.getBetMoneyBgPos(players,pos);
             betbg.setPosition(bgpos);
             

             var bet_bg_num = cc.LabelTTF("", "Arial", 22);
             bet_bg_num.setString("100");
             bet_bg_num.setName("num");
             var bgsize = betbg.getContentSize();
             bet_bg_num.setPosition(cc.p(bgsize.width/2,bgsize.height/2));
             betbg.addChild(bet_bg_num);

             this.uiNode.addChild(betbg);
        }
        else
        {
            var bgpos = utils.getBetMoneyBgPos(players,pos);
            betbg.setVisible(true);
            betbg.setPosition(bgpos);

        }

        this.updatePlayerSendBetMoney(pos,sendbet);
    },

    chipBackToMySelf:function(pos)
    {
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var Play_position = players.getChildByName("player"+pos).getPosition();

        var delay = 0.05;

        for(var i in this.playChip[pos])
        {
            delay+=0.01;
            (function(chip,d,pp)
            {
                var card_sound = cc.callFunc(function(){
                  Sound.getInstance().playEffect(PackName+"/res/sound/bet/collecting_bet.mp3"); 
                });
                chip.runAction(cc.sequence(cc.delayTime(0.05),card_sound,cc.moveTo(d,pp),cc.delayTime(0.05),cc.RemoveSelf()));
            })(this.playChip[pos][i],delay,Play_position);
        }
        this.playChip[pos] = [];

        var betbg = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + pos);
        if(betbg != null)
        {
            betbg.removeAllChildren();
            this.uiNode.removeChild(betbg);
            betbg = null;
        }

        CXPlayers.players[pos].updatePlayerMoney();   //刷新身上数量
    },

    chipToOtherPlayer:function(pos,winpos)
    {
        var bnum = this.playChip[pos].length;

        var players = utils.findNode(this.uiNode.getParent(),"players");
        var end_position = players.getChildByName("player"+topos).getPosition();

        for(var i = 0; i < bnum; i++)
        {
            (function(chip,position)
            {
                chip.runAction(cc.sequence(cc.moveTo(0.2,position),cc.delayTime(0.1),cc.RemoveSelf()));
            })(this.playChip[pos][i],end_position);
        }

         //需要更新 玩家身上的剩余簸簸数量
         CXPlayers.updatePlayerMoney(topos);
         var oplayer = CXModel.getPlayerByPos(pos);

        var betbg = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + pos);
        if(betbg != null)
        {
            betbg.removeAllChildren();
            this.uiNode.removeChild(betbg);
        }

    },

    chipBackForNum:function(pos,betNum,allsendBobo,all)
    {
        var bnum = Math.ceil(betNum/CXModel.stand_bobo);
        var lastnum = 0;

        if(bnum > this.playChip[pos].length)
        {
            bnum = this.playChip[pos].length;
        }

        lastnum = this.playChip[pos].length - bnum;

        var position = CXTips.pibg.getPosition();
        var left = position.x - 60;
        var right = position.x + 60;
        var top = position.y + 90;
        var bottom = position.y + 30;

        var tempchip = [];
        var mySendchip = [];


        if(bnum < this.playChip[pos].length)
        {
            for(var j = bnum;j<this.playChip[pos].length;j++)
            {
                tempchip.push(this.playChip[pos][j]);
            }

            for(var j = 0;j<bnum;j++)
            {
                mySendchip.push(this.playChip[pos][j]);
            }

            this.playChip[pos] = [];
            for(var j = 0; j < tempchip.length;j++)
            {
                this.playChip[pos].push(tempchip[j]);
            }
        }
        else
        {
            if(this.playChip[pos].length > 1)
            {  
                var allnum = this.playChip[pos].length - 1;
                for(var j = allnum;j<this.playChip[pos].length;j++)
                {
                    tempchip.push(this.playChip[pos][j]);
                }

                for(var j = 0;j<allnum;j++)
                {
                    mySendchip.push(this.playChip[pos][j]);
                }

                this.playChip[pos] = [];
                for(var j = 0; j < tempchip.length;j++)
                {
                    this.playChip[pos].push(tempchip[j]);
                }
            }
        }
        


        var lastbobo = allsendBobo - betNum;    // 发送 玩家剩余的簸簸数量
        this.updatePlayerSendBetMoney(pos,lastbobo);

        CXTips.pibg.getChildByName("num").setString("当前下注额:"+all);
        CXTips.pibg.setVisible(true);


        for(var i = 0; i < mySendchip.length; i++)
        {
            var end_pos = cc.p(this.randNum( left,right),this.randNum(bottom,top));
            this.pichip.push(mySendchip[i]);
            (function(chip,position)
            {
                // try
                // {
                 chip.runAction(cc.sequence(cc.moveTo(0.1,position)));
                // }catch(e)
                // {
                //     chip.setPosition(position);
                // }
            })(mySendchip[i],end_pos);
        }

    },

    //发送皮到桌上
    sendPichipTotable:function(pos,cur_pi,all_pi,delay)
    {
        var bnum = Math.ceil(cur_pi/CXModel.stand_bobo);
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var start_position = players.getChildByName("player"+pos).getPosition();

        var position = CXTips.pibg.getPosition();
        var left = position.x - 60;
        var right = position.x + 60;
        var top = position.y + 90;
        var bottom = position.y + 30;

        var curdelay = delay;
        for(var i = 0; i < bnum; i++)
        {
            curdelay+=0.01;
            var chip = this.newChip();
            chip.setValue(CXModel.stand_bobo);
            chip.setPosition(start_position);
            chip.setVisible(true);
            chip.setTag(parseInt(CardTagUM.CHIPPISTART+20*pos+i));
            this.uiNode.addChild(chip,10);
            this.pichip.push(chip);
            var end_pos = cc.p(this.randNum( left,right),this.randNum(bottom,top));
            (function(c,d,position)
            {   
                // try
                // {
                 c.runAction(cc.sequence(cc.moveTo(d,position)));
                // }catch(e)
                // {
                //     c.setPosition(position)
                // }
            })(chip,curdelay,end_pos);

            
        }

        CXTips.pibg.getChildByName("num").setString("当前的皮:"+all_pi);
        CXTips.pibg.setVisible(true);

        //需要更新 玩家身上的剩余簸簸数量
        CXPlayers.updatePlayerMoney(pos);
    },

    //自己的钱发到桌上
    sendMyBetToTable:function(pos,all,delay)
    {
        var mybetchip = this.playChip[pos];
        var position = CXTips.pibg.getPosition();
        var left = position.x - 60;
        var right = position.x + 60;
        var top = position.y + 90;
        var bottom = position.y + 30;


        var betbg = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + pos);
        if(betbg != null)
        {
            betbg.removeAllChildren();
            this.uiNode.removeChild(betbg);
        }

    
        if(mybetchip.length > 0)
        {
            var lenth = mybetchip.length;
            var curdelay = delay;
            for(var i = 0; i < lenth; i++)
            {
                curdelay+= 0.01;
                var chip = this.playChip[pos][i];
                this.pichip.push(chip);
                var end_pos = cc.p(this.randNum( left,right),this.randNum(bottom,top));
                (function(c,d,position)
                {
                      c.runAction(cc.sequence(cc.moveTo(d,position)));
                })(chip,curdelay,end_pos);

            }

            CXTips.pibg.getChildByName("num").setString("当前下注额:"+all);
            CXTips.pibg.setVisible(true);
        }

        this.playChip[pos] = [];
    },

    //赢家收钱  收皮
    winAllmoney:function(winseatid)
    {
        var pos = CXModel.getPosBySeatID(winseatid);
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var end_pos = players.getChildByName("player"+pos).getPosition();

        var ppchip = this.pichip;
         if(ppchip.length > 0)
        {
            var lenth = ppchip.length;
            var curdelay = 0.2;
            for(var i = 0; i < lenth; i++)
            {
                curdelay+= 0.05;
                var chip = ppchip[i];
                (function(c,d,position)
                {
                      var card_sound = cc.callFunc(function(){
                          Sound.getInstance().playEffect(PackName+"/res/sound/bet/collecting_bet.mp3"); 
                        });
                    c.runAction(cc.sequence(cc.delayTime(0.05),card_sound,cc.moveTo(d,position),cc.RemoveSelf()));

                })(chip,curdelay,end_pos);


                
            }

            CXTips.pibg.setVisible(false);
        }
        //需要更新 玩家身上的剩余簸簸数量
        CXPlayers.updatePlayerMoney(pos);
    },

    winPiMoneyToPos:function(pos,start,num)
    {
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var end_pos = players.getChildByName("player"+pos).getPosition();

        var ppchip = this.pichip;
         if(ppchip.length > 0)
        {
            var lenth = ppchip.length;
            var curdelay = 0.2;
            for(var i = start; i < start + num; i++)
            {
                curdelay+= 0.01;
                var chip = ppchip[i];
                (function(c,d,position)
                {
                      c.runAction(cc.sequence(cc.moveTo(d,position),cc.RemoveSelf()));
                })(chip,curdelay,end_pos);

                
            }

            //如果需要更新当前在桌上的簸簸数量的话 在这里更新
            if(start + num >= lenth)
            {
                CXTips.pibg.setVisible(false);
            }

        }
        //需要更新 玩家身上的剩余簸簸数量
        CXPlayers.updatePlayerMoney(pos);
    },

    updatePlayerSendBetMoney:function(pos,sendbet)
    {
        var tex = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + pos).getChildByName("num");
        tex.setString(sendbet);
    },

    randNum:function(minnum,maxnum){
        return Math.floor(minnum + Math.random() * (maxnum - minnum));
    },

};