var DDZAction =
{
    uiNode:null,
   
    clickCB:null,
    piaoBtns:[],
    jqBtns:[],
    tlBtns:[],
    mpBtns:[],
    outBtns:[],

    pdkmpBtns:[],
    pdkqgBtns:[],
    pdkjbBtns:[],
    pdkompBtns:[],
    pdkoutBtns:[],
    pdksplitBtns:[],
    pdksplitsueBtn:[],

    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }

        this.piaoBtns.push(this.getUI("piaoBtn0"));
        this.piaoBtns.push(this.getUI("piaoBtn1"));
        this.piaoBtns.push(this.getUI("piaoBtn2"));
        this.piaoBtns.push(this.getUI("piaoBtn3"));
        this.piaoBtns.push(this.getUI("bupiaoBtn"));

        this.jqBtns.push(this.getUI("jiaodzBtn"));
        this.jqBtns.push(this.getUI("qiangdzBtn"));
        this.jqBtns.push(this.getUI("jiaofenBtn0"));
        this.jqBtns.push(this.getUI("jiaofenBtn1"));
        this.jqBtns.push(this.getUI("jiaofenBtn2"));
        this.jqBtns.push(this.getUI("bujiaoBtn"));
        this.jqBtns.push(this.getUI("buqiangBtn"));

        this.tlBtns.push(this.getUI("tiBtn"));
        this.tlBtns.push(this.getUI("laBtn"));
        this.tlBtns.push(this.getUI("butiBtn"));
        this.tlBtns.push(this.getUI("bulaBtn"));

        this.mpBtns.push(this.getUI("mingpaiBtn"));
        this.mpBtns.push(this.getUI("bumingpaiBtn"));

        this.outBtns.push(this.getUI("buchuBtn"));
        this.outBtns.push(this.getUI("chongxuanBtn"));
        this.outBtns.push(this.getUI("tishiBtn"));
        this.outBtns.push(this.getUI("chupaiBtn"));
        this.outBtns.push(this.getUI("passBtn"));


        //跑的快需要添加按钮
        this.pdkmpBtns.push(this.getUI("pdk_mingpaiBtn"));
        this.getUI("pdk_mingpaiBtn").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.pdkmpBtns.push(this.getUI("pdk_bumingpaiBtn"));

        this.pdkqgBtns.push(this.getUI("qiangguanBtn"));
        this.getUI("qiangguanBtn").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.pdkqgBtns.push(this.getUI("buqiangguanBtn"));

        this.pdkjbBtns.push(this.getUI("pdk_jiabeiBtn"));
        this.getUI("pdk_jiabeiBtn").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.pdkjbBtns.push(this.getUI("pdk_bujiabeiBtn"));

        this.pdkompBtns.push(this.getUI("pdk_omingpaiBtn"));
        this.getUI("pdk_omingpaiBtn").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.pdkompBtns.push(this.getUI("pdk_obujiabeiBtn"));

        this.pdkoutBtns.push(this.getUI("chongxuanBtn"));
        this.pdkoutBtns.push(this.getUI("pdk_tishiBtn"));
        this.pdkoutBtns.push(this.getUI("pdk_chupaiBtn"));

        this.pdksplitBtns.push(this.getUI("pdk_splitcardBtn"));
        this.pdksplitBtns.push(this.getUI("pdk_passsplitBtn"));
        this.pdksplitsueBtn.push(this.getUI("pdk_sureBtn"));



        this.getUI("piaoBtn0").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.getUI("piaoBtn1").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.getUI("piaoBtn2").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.getUI("piaoBtn3").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.addUIClickEvt("piaoBtn0");
        this.addUIClickEvt("piaoBtn1");
        this.addUIClickEvt("piaoBtn2");
        this.addUIClickEvt("piaoBtn3");
        this.addUIClickEvt("bupiaoBtn");
        this.addUIClickEvt("buqiangBtn");

        this.addUIClickEvt("bujiaoBtn");
        this.addUIClickEvt("jiaodzBtn");
        this.addUIClickEvt("qiangdzBtn");
        this.addUIClickEvt("piaoBtn3");
        this.addUIClickEvt("jiaofenBtn0");
        this.addUIClickEvt("jiaofenBtn1");
        this.addUIClickEvt("jiaofenBtn2");

        this.addUIClickEvt("tiBtn");
        this.addUIClickEvt("laBtn");
        this.addUIClickEvt("butiBtn");
        this.addUIClickEvt("bulaBtn");

        this.addUIClickEvt("readyBtn");
        this.addUIClickEvt("noreadyBtn");
        this.addUIClickEvt("tuoguanBtn");
        this.addUIClickEvt("uptableBtn");

        this.addUIClickEvt("mingpaiBtn");
        this.addUIClickEvt("bumingpaiBtn");

        this.addUIClickEvt("buchuBtn");
        this.addUIClickEvt("chongxuanBtn");
        this.addUIClickEvt("tishiBtn");
        this.addUIClickEvt("chupaiBtn");
        this.addUIClickEvt("passBtn");


         //跑的快需要添加按钮
        this.addUIClickEvt("pdk_mingpaiBtn");
        this.addUIClickEvt("pdk_bumingpaiBtn");
        this.addUIClickEvt("qiangguanBtn");
        this.addUIClickEvt("buqiangguanBtn");
        this.addUIClickEvt("pdk_jiabeiBtn");
        this.addUIClickEvt("pdk_bujiabeiBtn");
        this.addUIClickEvt("pdk_omingpaiBtn");
        this.addUIClickEvt("pdk_obujiabeiBtn");
        this.addUIClickEvt("pdk_tishiBtn");
        this.addUIClickEvt("pdk_chupaiBtn");

        this.addUIClickEvt("pdk_splitcardBtn");
        this.addUIClickEvt("pdk_passsplitBtn");
        this.addUIClickEvt("pdk_sureBtn");


        this.getUI("mingpaiBtn").getChildByName("num").ignoreContentAdaptWithSize(true);
        this.getUI("talkBtn").addTouchEventListener(function (sender, evt) {

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
        this.showUI("noreadyBtn",false);
        this.showUI("readyBtn",false);
        this.showUI("tuoguanBtn",false);
        this.showUI("uptableBtn",false);
        this.showUIs(this.piaoBtns,false);
        this.showUIs(this.jqBtns,false);
        this.showUIs(this.mpBtns,false);
        this.showUIs(this.outBtns,false);

        this.showUIs(this.pdkmpBtns,false);
        this.showUIs(this.pdkqgBtns,false);
        this.showUIs(this.pdkjbBtns,false);
        this.showUIs(this.pdkompBtns,false);
        this.showUIs(this.pdkoutBtns,false);
        this.showUIs(this.pdksplitBtns,false);
        this.showUIs(this.pdksplitsueBtn,false);

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

        utils.sortNodes(btns,size.width*0.5,310,5);
    },

    sortotherBtns:function(btns)
    {
        var size = cc.director.getWinSize();

        utils.sortNodes(btns,size.width*0.5,200,5);
    },

    //显示飘按钮
    showPiaoBtns:function(is)
    {
        if(is)
        {
            for(var i in DDZModel.piaoCoins)
            {
                var btn = this.getUI("piaoBtn"+i);
                if(btn)
                {
                    btn.setVisible(true);
                    btn.getChildByName("num").setString(""+DDZModel.piaoCoins[i]);
                }
            }
            this.showUI("bupiaoBtn",true);
            this.sortBtns(this.piaoBtns);
        }
        else
        {
            this.showUIs(this.piaoBtns,false);
        }
    },
    showJiaoDZBtns:function(is)
    {
        this.showUIs(this.jqBtns,false);
        if(is)
        {
            if(DDZModel.tableType == 1)
            {
                this.showUI("jiaodzBtn",true);
            }
            else
            {
                this.showUI("jiaofenBtn0",true);
                this.showUI("jiaofenBtn1",true);
                this.showUI("jiaofenBtn2",true);
            }
            this.showUI("bujiaoBtn",true);
            this.sortBtns(this.jqBtns);
        }
    },

    showJiaoFenAble:function(action,isMust)
    {
        this.showUI("jiaofenBtn0",action<=1 &&!isMust);
        this.showUI("jiaofenBtn1",action<=2&&!isMust);
        this.showUI("jiaofenBtn2",action<=3);

        this.showUI("bujiaoBtn",!isMust);
        this.sortBtns(this.jqBtns);
    },
    showQiangDZBtns:function(is)
    {
        this.showUIs(this.jqBtns,false);
        if(is)
        {
            if(DDZModel.tableType == 1)
            {
                this.showUI("qiangdzBtn",true);
            }
            else
            {
                this.showUI("jiaofenBtn0",true);
                this.showUI("jiaofenBtn1",true);
                this.showUI("jiaofenBtn2",true);
            }
            this.showUI("buqiangBtn",true);
            this.sortBtns(this.jqBtns);
        }
    },

    showTiLaBtns:function(tlType,is)
    {
        this.showUIs(this.tlBtns,false);
        if(is)
        {
            if(tlType == 1)
            {
                this.showUI("tiBtn",true);
                this.showUI("butiBtn",true);
            }
            else if(tlType ==2)
            {
                this.showUI("laBtn",true);
                this.showUI("bulaBtn",true);
            }
            this.sortBtns(this.tlBtns);
        }
    },
    showMingPaiBtns:function(is)
    {
        this.showUIs(this.mpBtns,false);
        if(is)
        {
            this.getUI("mingpaiBtn").getChildByName("num").setString(":"+DDZModel.addRatios[DDZRatio.MING_PAI]);
            this.showUIs(this.mpBtns,true);
            this.sortBtns(this.mpBtns);
        }
    },
    checkOutCard:function()
    {
        if(DDZModel.isMySeatID(DDZModel.curSeatID))//最后操作是好我方时
        {
            this.showBtnEnable("chupaiBtn",!DDZCards.checkUpCard());
        }
    },

    showOutCardBtns:function(is)
    {
        this.showUIs(this.outBtns,false);
        this.showBtnEnable("chupaiBtn",false);
        DDZTips.showPass(false);
        if(is)
        {
            if(DDZCards.tipCardNum() > 0)//有牌可出
            {
                if(DDZModel.outCardMode == DDZCardState.NEW_CARD)//必须出牌时
                {
                    this.showUI("buchuBtn",false);
                }
                else
                {
                    this.showUI("buchuBtn",true);
                }
                this.showUI("chongxuanBtn",true);
                this.showUI("tishiBtn",true);
                this.showUI("chupaiBtn",true);
                this.checkOutCard();
            }
            else
            {
                this.showUI("passBtn",true);
                DDZTips.showPass(true);
            }

            this.sortBtns(this.outBtns);
        }
    },


    //跑的快按钮的显示

    pdk_showmpBtns:function(is)
    {
        if(is)
        {
            for(var i in this.pdkmpBtns)
            {
                var btn = this.getUI("pdk_mingpaiBtn");
                if(btn)
                {
                    this.getUI("pdk_mingpaiBtn").getChildByName("num").setString(":"+DDZModel.pdk_mpCoints);
                }
            }
            this.showUIs(this.pdkmpBtns,true);
            this.sortBtns(this.pdkmpBtns);
        }
        else
        {
            this.showUIs(this.pdkmpBtns,false);
        }
    },

    pdk_showompBtns:function(is)
    {
        if(is)
        {
            for(var i in this.pdkompBtns)
            {
                var btn = this.getUI("pdk_omingpaiBtn");
                if(btn)
                {
                    this.getUI("pdk_omingpaiBtn").getChildByName("num").setString(":"+DDZModel.pdk_ompCoints);
                }
            }
            this.showUIs(this.pdkompBtns,true);
            this.sortBtns(this.pdkompBtns);
        }
        else
        {
            this.showUIs(this.pdkompBtns,false);
        }
    },


    pdk_showqgBtns:function(is)
    {
        if(is)
        {
            for(var i in this.pdkqgBtns)
            {
                var btn = this.getUI("qiangguanBtn");
                if(btn)
                {
                    this.getUI("qiangguanBtn").getChildByName("num").setString(":"+DDZModel.pdk_qiangguan);
                }
            }
            this.showUIs(this.pdkqgBtns,true);
            this.sortBtns(this.pdkqgBtns);
        }
        else
        {
            this.showUIs(this.pdkqgBtns,false);
        }
    },


    pdk_showjbBtns:function(is)
    {
        if(is)
        {
            for(var i in this.pdkjbBtns)
            {
                var btn = this.getUI("pdk_jiabeiBtn");
                if(btn)
                {
                    this.getUI("pdk_jiabeiBtn").getChildByName("num").setString(":"+DDZModel.pdk_jiabei);
                }
            }
            this.showUIs(this.pdkjbBtns,true);
            this.sortBtns(this.pdkjbBtns);
        }
        else
        {
            this.showUIs(this.pdkjbBtns,false);
        }
    },

    pdkcheckOutCard:function()
    {
        if(DDZModel.isMySeatID(DDZModel.curSeatID))//最后操作是好我方时
        {
            this.getUI("pdk_chupaiBtn").getChildByName("enable").setVisible(!DDZCards.checkUpCard());
        }
    },

    pdk_showoutCardBtns:function(is)
    {
        this.showUIs(this.pdkoutBtns,false);
        if(is)
        {
            this.showUI("chongxuanBtn",true);
            this.showUI("pdk_tishiBtn",true);
            this.showUI("pdk_chupaiBtn",true);
            this.sortBtns(this.pdkoutBtns);
            this.pdkcheckOutCard();
        }
    },


    pdk_showsplitCardBtn:function(is)
    {
        this.showUIs(this.pdksplitBtns,false);
        if(is)
        {
            this.showUIs(this.pdksplitBtns,true);
            this.sortBtns(this.pdksplitBtns);
        }
    },

    pdk_suresplitBtn:function(is)
    {
        this.showUIs(this.pdksplitsueBtn,false);
        if(is)
        {
            this.showUIs(this.pdksplitsueBtn,true);
            this.sortBtns(this.pdksplitsueBtn);
        }
    },

    
    showTuoGuanBtn:function(is)
    {
       // this.showUI("tuoguanBtn",is);
        this.showUI("uptableBtn",!is);
    },
    showUptableBtn:function(is)
    {
        this.showUI("uptableBtn",is);
       // this.showUI("tuoguanBtn",!is);
    },
    showReadyBtn:function(is)
    {
        this.showUI("readyBtn",is);
    },
    showNoReadyBtn:function(is)
    {
        this.showUI("noreadyBtn",is);
    },

    showpdkSplitSureBtn:function(is)
    {
        this.showUIs(this.pdksplitsueBtn,false);
        if(is)
        {
            this.showUIs(this.pdksplitsueBtn,true);
            this.sortotherBtns(this.pdksplitsueBtn);
        }
    }
};
