var WZDNAction =
{
    uiNode:null,
    clickCB:null,
    btnCenter:null,

    outBtns:[],
    outBtns_q:[],

    outButtonPosition:[],

    readyBtn:null,

    szBtn:null,

    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

        this.btnCenter = cc.p(640,165);

        this.outButtonPosition.push(cc.p(240,35));
        this.outButtonPosition.push(cc.p(440,35));
        this.outButtonPosition.push(cc.p(640,35));
        this.outButtonPosition.push(cc.p(840,35));
        this.outButtonPosition.push(cc.p(1040,35));

        this.outBtns.push(this.getUI("btn_one"));
        this.outBtns.push(this.getUI("btn_two"));
        this.outBtns.push(this.getUI("btn_three"));
        this.outBtns.push(this.getUI("btn_four"));
        this.outBtns.push(this.getUI("btn_five"));


        this.outBtns_q.push(this.getUI("btnq_bq"));
         this.outBtns_q.push(this.getUI("btnq_one"));
        this.outBtns_q.push(this.getUI("btnq_two"));
        this.outBtns_q.push(this.getUI("btnq_three"));
        this.outBtns_q.push(this.getUI("btnq_four"));
       


        this.readyBtn = this.getUI("btn_ready");
        this.szBtn = this.getUI("btn_sz");

        this.addUIClickEvt("btn_one");
        this.addUIClickEvt("btn_two");
        this.addUIClickEvt("btn_three");
        this.addUIClickEvt("btn_four");
        this.addUIClickEvt("btn_five");
        this.addUIClickEvt("btn_ready");

        this.addUIClickEvt("btnq_bq");
        this.addUIClickEvt("btnq_one");
        this.addUIClickEvt("btnq_two");
        this.addUIClickEvt("btnq_three");
        this.addUIClickEvt("btnq_four");
        this.addUIClickEvt("btn_sz");


        this.getUI("btn_voice").addTouchEventListener(function (sender, evt) {

            if(evt == cc.EventTouch.EventCode.BEGAN)
            {
                PlatformHelper.startRecord();
            }
            else if(evt == cc.EventTouch.EventCode.ENDED || evt == cc.EventTouch.EventCode.CANCELLED)
            {
                PlatformHelper.stopRecord();
            }

        });


        this.reset();
    },


    reset:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

    this.showUIs(this.outBtns,false);
    this.showUIs(this.outBtns_q,false);
    this.readyBtn.setVisible(false);

    },
    addUIClickEvt:function(key)
    {
        var self = this;
        var ui= this.getUI(key);
        if(ui) {
            ui.addClickEventListener(function (sender, evt) {

                if(self.clickCB)
                {
                    self.clickCB(key);
                }

            });
        }
    },
    showBtnEnable:function(key,is)
    {
        this.getUI(key).getChildByName("enable").setVisible(is);
        this.getUI(key).setEnabled(!is);
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
    //排序按钮
    sortBtns:function(btns)
    {
        var size = cc.director.getWinSize();

        utils.sortNodes(btns,size.width*0.5,300,5);
    },

    sortotherBtns:function(btns)
    {
        var size = cc.director.getWinSize();

        utils.sortNodes(btns,size.width*0.5,200,5);
    },

    showBtnEnable:function(key,vi)
    {
        this.getUI(key).setEnabled(vi);
    },

    setGenValue:function(bet)
    {
         this.getUI("btn_gen").getChildByName("num").setString(bet);
    },

    showOutCardBtns:function(isQ)
    {
        var useBtns = null;
        if(isQ)
        {
            useBtns = this.outBtns_q;
        }else
        {
            useBtns = this.outBtns;
        }

        var centerposition = this.btnCenter;
        var outBtnPosition = this.outButtonPosition;

        var rebackButton = cc.callFunc(function(){
            var leng = useBtns.length;
                    for(var i in useBtns)
                    {
                        if(useBtns[i].getPosition().x != centerposition.x && useBtns[i].getPosition().y != centerposition.y)
                        {
                            (function(btn)
                            {
                                    var  func = cc.callFunc(function () {
                                        btn.setVisible(false);
                                    });
                                 btn.runAction(cc.sequence(cc.delayTime(0.05),func,
                                    cc.moveTo(0.15,centerposition),cc.delayTime(0.05),cc.callFunc(function(){
                                        btn.setVisible(false);
                                    })
                                    ))
                            })(useBtns[i])
                        }
                    }
                });
        var delayt = cc.delayTime(0.5);

        var outAllButton = cc.callFunc(function(){
                    for(var i in useBtns)
                    {
                        useBtns[i].setVisible(true);
                        useBtns[i].runAction(cc.sequence(cc.delayTime(0.1),
                            cc.moveTo(0.15, outBtnPosition[i])
                            ))
                       
                    }
                });
        this.uiNode.runAction(cc.sequence(rebackButton,delayt,outAllButton));
    },

    showOutCardBtnsSigle:function()
    {
        var useBtns = this.outBtns;
        var centerposition = this.btnCenter;
        var outBtnPosition = this.outButtonPosition;
        
         var outAllButton = cc.callFunc(function(){
                    for(var i in useBtns)
                    {
                        useBtns[i].setVisible(true);
                        useBtns[i].runAction(cc.sequence(cc.delayTime(0.05),
                            cc.moveTo(0.15, outBtnPosition[i])
                            ))
                       
                    }
                });
        this.uiNode.runAction(cc.sequence(outAllButton));
    },


     setBtnZhu:function(ratio)
    {
        var len = this.outBtns.length;
        for(var i = 0; i < len ;i++)
        {
            var str = this.outBtns[i].getChildByName("num");
            str.setString(ratio[i]);
        }

    },

    hideOutCardBtns:function(isq)
    {
        var useBtns = null;
        if(isq)
        {
            useBtns = this.outBtns_q;
        }else
        {
            useBtns = this.outBtns;
        }

        var centerposition = this.btnCenter;

       var rebackButton = cc.callFunc(function(){
                    for(var i in useBtns)
                    {
                        if(useBtns[i].getPosition().x != centerposition.x || useBtns[i].getPosition().y != centerposition.y)
                        {
                            (function(btn)
                            {
                                var  func = cc.callFunc(function () {
                                        
                                    });
                                 btn.runAction(cc.sequence(cc.delayTime(0.05),func,
                                    cc.moveTo(0.15,centerposition),cc.delayTime(0.05),cc.callFunc(function(){
                                        btn.setVisible(false);
                                    })
                                    ))
                            })(useBtns[i])
                        }
                    }
                });
        this.uiNode.runAction(cc.sequence(rebackButton));
    },

    hideOutCardBtnsSingle:function()
    {
        var useBtns = this.outBtns;
        var centerposition = this.btnCenter;

       var rebackButton = cc.callFunc(function(){
                    for(var i in useBtns)
                    {
                        if(useBtns[i].getPosition().x != centerposition.x || useBtns[i].getPosition().y != centerposition.y)
                        {
                            (function(btn)
                            {
                                 btn.runAction(cc.sequence(cc.moveTo(0.05,centerposition),cc.delayTime(0.05),cc.callFunc(function(){
                                        btn.setVisible(false);
                                    })
                                    ))
                            })(useBtns[i])
                        }
                    }
                });
        this.uiNode.runAction(cc.sequence(rebackButton));
    },

    
    showTuoGuanBtn:function(is)
    {
        this.showUI("tuoguanBtn",is);
        this.showUI("uptableBtn",!is);
    },

    showReadyBtn:function(is)
    {
        this.showUI("btn_ready",is);
    },

    showSzBtn:function(is)
    {
        this.showUI("btn_sz",is);
    },

};
