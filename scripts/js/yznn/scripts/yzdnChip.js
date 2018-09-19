
var YZDNChip = cc.Node.extend(
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
            this.uiNode.getChildByName("money_bg").loadTexture("YZDN/money.png",1);
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

var YZDNChips = {

    uiNode: null,
    playChip:[],

    pichip:[],

    recordchipNum:[],

    cun:0,

    t_cun:0,


    init:function()
    {
        this.playChip = [];
        this.cun = 0;
    },

    newChip:function()
    {
        var temChip = new YZDNChip();
        return temChip;
    },

    sendBetMoney:function(pos,number,sendbet)
    {
        if(number <= 0)
        {
            return;
        }
        for(var i = 0; i<number;i++)
        {
            var chip = this.newChip();
            this.playChip.push(chip);
            this.cun++;
            log("======this.cun======ccccc=="+this.cun);
            this.sendBetMoney_ani(chip,pos,this.cun);
        }
        this.sendBetBg(pos,sendbet);
    },


    sendBetMoneyToTable:function(pos,number)
    {
        if(number <= 0)
        {
            return;
        }
        for(var i = 0; i<number;i++)
        {
            var chip = this.newChip();
            this.playChip.push(chip);
            this.cun++;
            this.sendchipTotable(chip,pos,this.cun);
        }
    },


    sendBetMoney_ani:function(chip,pos,tag)
    {
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var Play_position = players.getChildByName("player"+pos).getPosition();
        var startpos = utils.getChipStartPos(pos,Play_position);
        var endpos = utils.getChipEndPos(pos,Play_position);
        

        chip.setVisible(true);
        chip.setPosition(Play_position);
        chip.setTag(parseInt(CardTagUM.CHIPSTART+tag));
        this.uiNode.addChild(chip);
        var movetoPosition = cc.p(this.randNum(startpos.x,endpos.x),this.randNum(startpos.y,endpos.y));
        (function(c,mpostion)
        {
            if(YZDNModel.fastShow)
            {
                c.setPosition(mpostion);
            }
            else
            {
                c.runAction(cc.sequence(cc.moveTo(0.2,mpostion)));
            }
            
        })(chip,movetoPosition);

        
    },

    sendBetBg:function(pos,sendbet)
    {
        var betbg = this.uiNode.getChildByTag(parseInt(CardTagUM.SENDBETMONEY_BG + pos));
        var players = utils.findNode(this.uiNode.getParent(),"players");
        if(betbg == null)
        {
             betbg = cc.Sprite(utils.spriteFrame("YZDN/money_bar.png"));
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


    reset:function()
    {
        var number = this.cun;
        for(var k = 0; k<= number; k++)
        {
            var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART + k));
            if(chip != null)
            {
                this.uiNode.removeChild(chip,true);
                chip = null;
            }
        }
        this.playChip = [];


        for(var i = 0; i < 5; i++)
        {
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
        this.cun = 0;
    },
    
    //所有人的筹码移动到桌上
    sendAllPlayerChipTpTable:function()
    {
        var mybetchip = this.playChip;
        var position = YZDNTips.pibg.getPosition();
        var left = position.x - 60;
        var right = position.x + 60;
        var top = position.y + 90;
        var bottom = position.y + 30;

        for(var k = 0;k < 5; k++)
        {
            var betbg = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + k);
            if(betbg != null)
            {
                betbg.removeAllChildren();
                this.uiNode.removeChild(betbg);
            }
        }

        if(mybetchip.length > 0)
        {
            var lenth = mybetchip.length;
            var curdelay = 0.15;
            for(var i = 0; i < lenth; i++)
            {
                curdelay+= 0.01;
                var chip = this.playChip[i];
                var end_pos = cc.p(this.randNum( left,right),this.randNum(bottom,top));
                (function(c,d,position)
                {
                     if(YZDNModel.fastShow)
                     {
                        c.setPosition(position);
                     }
                     else
                     {
                        c.runAction(cc.sequence(cc.moveTo(d,position)));
                     }
                      
                })(chip,curdelay,end_pos);

            }
        }
    },

    //发送筹码到桌上
    sendchipTotable:function(chip,pos,tag)
    {

        var players = utils.findNode(this.uiNode.getParent(),"players");
        var start_position = players.getChildByName("player"+pos).getPosition();

        var position = YZDNTips.pibg.getPosition();
        var left = position.x - 60;
        var right = position.x + 60;
        var top = position.y + 90;
        var bottom = position.y + 30;

        var curdelay = 0.2;
        chip.setVisible(true);
        chip.setTag(parseInt(CardTagUM.CHIPSTART+tag));
        chip.setPosition(start_position);
        this.uiNode.addChild(chip,10);

        var end_pos = cc.p(this.randNum(left,right),this.randNum(bottom,top));
        (function(c,d,position)
        {   
            if(YZDNModel.fastShow)
            {
                c.setPosition(position);
            }
            else
            {
                c.runAction(cc.sequence(cc.moveTo(d,position)));
            }
            
        })(chip,curdelay,end_pos);

        //需要更新 玩家身上的剩余簸簸数量
        // YZDNPlayers.updatePlayerMoney(pos);
    },


    sendChipToPlayer:function(pos,num)
    {
        var players = utils.findNode(this.uiNode.getParent(),"players");
        var p_position = players.getChildByName("player"+pos).getPosition();
        var delay = 0.2;
        for(var k = 0;k < num;k++)
        {
            if(this.cun <= 0)
            {
                break;
            }
            delay= 0.2;
            var chip = this.uiNode.getChildByTag(parseInt(CardTagUM.CHIPSTART + this.cun));
            if(chip != null)
            {
                (function(c,d,position)
                {   
                    if(YZDNModel.fastShow)
                    {
                        c.setPosition(position);
                        c.removeFromParent(false);
                    }
                    else
                    {
                        c.runAction(cc.sequence(cc.moveTo(d,position),cc.delayTime(0.1),cc.RemoveSelf()));
                    }
                    
                })(chip,delay,p_position);
            }
            this.cun--;
        }
    },


    updatePlayerSendBetMoney:function(pos,sendbet)
    {
        var tempplaybg = this.uiNode.getChildByTag(CardTagUM.SENDBETMONEY_BG + pos);
        if(tempplaybg != null)
        {
            var tex = tempplaybg.getChildByName("num");
            tex.setString(sendbet);
        }
    },

    randNum:function(minnum,maxnum){
        return Math.floor(minnum + Math.random() * (maxnum - minnum));
    },

};