var PHZAction =
{
    uiNode:null,
    clickCB:null,
    clickCBTarget:null,
    ackBtns:[],
    niaoBtns:[],
    piaoBtns:[],
    acksVal:0,
    init:function()
    {
        if(this.uiNode == null)
        {
            return;
        }
        this.ackBtns.push(this.getUI("chiBtn"));
        this.ackBtns.push(this.getUI("link2"));
        this.ackBtns.push(this.getUI("pengBtn"));
        this.ackBtns.push(this.getUI("link1"));
        this.ackBtns.push(this.getUI("huBtn"));
        this.ackBtns.push(this.getUI("link0"));
        this.ackBtns.push(this.getUI("guoBtn"));
        this.addUIClickEvt("chiBtn");
        this.addUIClickEvt("pengBtn");
        this.addUIClickEvt("huBtn");
        this.addUIClickEvt("guoBtn");

        this.niaoBtns.push(this.getUI("noniaoBtn"));
        this.piaoBtns.push(this.getUI("nopiaoBtn"));
        for(var i =0;i<4;i++)
        {
            this.niaoBtns.push(this.getUI("niaoBtn"+i));
            this.addUIClickEvt("niaoBtn"+i);
            utils.findNode(this.getUI("niaoBtn"+i),"num").ignoreContentAdaptWithSize(true);

            this.piaoBtns.push(this.getUI("piaoBtn"+i));
            this.addUIClickEvt("piaoBtn"+i);
            utils.findNode(this.getUI("piaoBtn"+i),"num").ignoreContentAdaptWithSize(true);
        }
        this.addUIClickEvt("noniaoBtn");
        this.addUIClickEvt("nopiaoBtn");

        this.addUIClickEvt("readyBtn");
        this.addUIClickEvt("uptableBtn");
        this.addUIClickEvt("outBtn");
        this.addUIClickEvt("updateBtn");

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
        this.showUI("readyBtn",false);
        this.showUI("uptableBtn",false);
        this.showUI("outBtn",false);
        this.showUI("updateBtn",false);
        this.showUIs(this.ackBtns,false);
        this.showUIs(this.niaoBtns,false);
        this.showUIs(this.piaoBtns,false);
    },
    addUIClickEvt:function(key)
    {
        var self = this;
        var ui= this.getUI(key);
        if(ui) {
            ui.addClickEventListener(function (sender, evt) {

                if(self.clickCB &&sender &&sender.isVisible())
                {
                    self.clickCB(self.clickCBTarget,key);
                }

            });
        }
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
    //排序按钮
    sortBtns:function(btns)
    {
        var size = cc.director.getWinSize();
       // utils.sortNodes(btns,size.width*0.5,320,10);
        var num =0;
        for(var i in btns)
        {
            if(btns[i].isVisible())
            {
                num++;
            }
        }
        var sy = 320;
        var sx = 1000;
        for(var i =btns.length-1;i>=0;--i) {
            if(btns[i].isVisible())
            {
                sx-=btns[i].getContentSize().width*(1-btns[i].getAnchorPoint().x)*btns[i].getScaleX();
                btns[i].setPosition(sx,sy);
                sx-=btns[i].getContentSize().width*btns[i].getAnchorPoint().x*btns[i].getScaleX()-16;
            }
        }

    },
    sortBtns1:function(btns)
    {

        var size = cc.director.getWinSize();
        utils.sortNodes(btns,size.width*0.5,320,10);
    },

    showUptableBtn:function(is)
    {
        this.showUI("uptableBtn",is);
    },
    showReadyBtn:function(is)
    {
        this.showUI("readyBtn",is);
    },
    showOutBtn:function(is)
    {
        this.showUI("outBtn",is);
    },
    showUpdateBtn:function(is)
    {
        this.showUI("updateBtn",is);
    },
    showAckBtns:function(is,acks)
    {
        this.showUIs(this.ackBtns,false);

        this.acksVal =acks;
        if(is)
        {
            if((acks&PHZAck.ACK_CHI) == PHZAck.ACK_CHI)
            {
                this.showUI("chiBtn",true);
                this.showUI("link2",true);
            }
            if((acks&PHZAck.ACK_PENG) == PHZAck.ACK_PENG)
            {
                this.showUI("pengBtn",true);
                this.showUI("link1",true);
            }
            if((acks&PHZAck.ACK_HU) == PHZAck.ACK_HU)
            {
                this.showUI("huBtn",true);
                this.showUI("link0",true);
            }
            this.showUI("guoBtn",true);
            this.sortBtns(this.ackBtns);
        }
    },
    showNiaoBtns:function(is)
    {
        this.showUIs(this.niaoBtns,false);
        if(!is)
        {
            return;
        }
        var val =PHZModel.neiBetNums;
        for(var i in val)
        {
            if(i >=4)
            {
                break;
            }
            this.showUI("niaoBtn"+i,true);
            utils.findNode(this.getUI("niaoBtn"+i),"num").setString(val[i]);
        }
        this.showUI("noniaoBtn",true);
        this.sortBtns1(this.niaoBtns);
    },
    showPiaoBtns:function(is)
    {
        this.showUIs(this.piaoBtns,false);
        if(!is)
        {
            return;
        }
        var val =PHZModel.piaoBetNums;
        for(var i in val)
        {
            if(i >=4)
            {
                break;
            }
            this.showUI("piaoBtn"+i,true);
            utils.findNode(this.getUI("piaoBtn"+i),"num").setString(val[i]);
        }
        this.showUI("nopiaoBtn",true);
        this.sortBtns1(this.piaoBtns);
    }
};
