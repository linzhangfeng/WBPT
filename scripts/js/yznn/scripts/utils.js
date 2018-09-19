
var packet =
{
    data:{},
    parse: function (d) {
        var s = JPacket.decodeStr(d);
        try{
            this.data = JSON.parse(s);
            return true;
        }
        catch(e)
        {
            return false;
        }
    },
    toJson:function()
    {
        return this.data;
    },
    toStr:function()
    {
        return JSON.stringify(this.data);
    },
    cmd: function () {
        if(this.data.hasOwnProperty("cmd") == false)
        {
            return -2;
        }
        if(isNaN(this.data["cmd"]))
        {
            return -2;
        }
        return this.data["cmd"];
    }
};

var utils ={

// 获取整形数值
    getValInt:function(val,key)
    {
        if(val.hasOwnProperty(key))
        {
            return val[key];
        }
        return 0;
    },

     //左位补0
    pad:function(num,n)
    {
        var len = num.toString().length;
        while(len < n) {
            num = "0" + num;
            len++;
        }
        return num+"";
    },

// 获取字符串数值
    getValStr:function(val,key)
    {
        if(val.hasOwnProperty(key))
        {
            return val[key];
        }
        return "";
    },

    getValBool:function(val,key)
    {
        if(val.hasOwnProperty(key))
        {
            if(val[key] == 0)
            {
                return false;
            }else{
                return true;
            }
        }
        return false;
    },

    //深拷贝对象
    clone:function(obj){
        function Fn(){}
        Fn.prototype = obj;
        var o = new Fn();
        for(var a in o){
        if(typeof o[a] == "object") {
            o[a] = utils.clone(o[a]);
        }
    }

    return o;
    },
    //获取精灵帧
  spriteFrame:function(key)
{
   return cc.spriteFrameCache.getSpriteFrame(key);
},

    //转换16进制
    toHex2:function(num)
    {
        if(num < 16)
        {
            return "0"+num.toString(16);
        }
        return num.toString(16);
    },

    scaleToSize:function(node,w,h)
    {
        node.setScaleX( w/node.getContentSize().width);
        node.setScaleY(h/node.getContentSize().height);
    }
    ,
    removeListData:function(list,data)
    {
        for(var i in list)
        {
            if(list[i] == data)
            {
                list.splice(i,1);
                break;
            }

        }
    },
    newAni:function(key,start,end,dt,loop)
    {
        var ani = new cc.Animation();
        for(var i = start;i<end;++i)
        {
            ani.addSpriteFrame(this.spriteFrame(key+i+".png"));
        }
        ani.setDelayPerUnit(dt);
      //  ani.setRestoreOriginalFrame(true);
        if(loop && loop >0)
        {
            ani.setLoops(loop);
        }
        return cc.animate(ani);
    },
    newAni1:function(key,list,dt,loop)
    {
        var ani = new cc.Animation();
        for(var i in list)
        {
            ani.addSpriteFrame(this.spriteFrame(key+list[i]+".png"));
        }
        ani.setDelayPerUnit(dt);
     //   ani.setRestoreOriginalFrame(true);
        if(loop && loop >0)
        {
            ani.setLoops(loop);
        }
        return cc.animate(ani);
    },
    parseName:function(max,name)
    {
        var l = name.length;
        var blen = 0;
        var len = [];
        var n=0;
        for(var i=0; i<l; i++) {
            n =0;
            if ((name.charCodeAt(i) & 0xff00) != 0) {
                blen ++;
                n++;
            }
            blen ++;
            n++;
            len[i] =n;
        }
        if(blen > max)
        {
            n = 0;
            for(var i=0; i<l; i++)
            {
                n+=len[i];
                if(n>=max)
                {
                    n = i+1;
                    break;
                }
            }
            return name.substr(0,n)+"..";
        }
        return name;
    },
    getSavePath:function()
    {
        return jsb.fileUtils.getWritablePath()+"Assets/";
    },
    getImageSavePath:function(fileUrl)
    {
        if(!fileUrl)
        {
            return;
        }
        var findFlag = "/chess_img/";
        var cutPos = fileUrl.indexOf(findFlag);
        if (cutPos <=0) {
            return fileUrl;
        } else {
            var localPath = fileUrl.slice(cutPos + findFlag.length);
            return this.getSavePath() + "images/" + localPath;
        }

    },
    hasImageFile:function(f)
    {
        return jsb.fileUtils.isFileExist(this.getImageSavePath(f));
    },
    format :function(string)
    {
       var args=arguments;
        var pattern=new RegExp("%([0-9]+)", "g");
        return String(string).replace(pattern
            , function(match, index) {
                if (index==0 || index >=args.length)
                    throw "Invalid index in format string";
                return args[index];
            });
    },

    random:function(start,end)
    {
        return (Math.floor(utils.randomf(start,end+1)));
    },
    randomf:function(start,end)
    {
        return Math.random()*(end-start)+start;
    },
    findNode:function(node,name)
    {
        if(!node || !node.getName)
        {
            return null;
        }
        if(node.getName() ==name)
        {
            return node;
        }
        var cs = node.getChildren();
        for(var i in cs)
        {
            var ret = utils.findNode(cs[i],name);
            if(ret)
            {
                return ret;
            }
        }
        return null;
    },

    //排序按钮
    sortNodes:function(nodes,midX,midY,dx)
    {
        var num =0;
        var len = 0
        for(var i in nodes)
        {
            if(nodes[i].isVisible())
            {
                num++;
                len+=nodes[i].getContentSize().width*nodes[i].getScaleX();
            }
        }

        var by = midY;
        var bx = midX - (len+(num-1)*dx)*0.5;
        for(var i in nodes) {
            if(nodes[i].isVisible())
            {
                bx+=nodes[i].getContentSize().width*nodes[i].getScaleX()*nodes[i].getAnchorPoint().x;
                nodes[i].setPosition(bx,by);
                bx+=nodes[i].getContentSize().width*nodes[i].getScaleX()*(1-nodes[i].getAnchorPoint().x);
                bx+=dx;
            }
        }

    },

    getCardPos:function(players,pos,cardIndex)
    {
         var Play_position = players.getChildByName("player"+pos).getPosition();
         var cins = 35;
         var mycins = 73;
         var leftdistance = 90;
         var rightdistance = -225;
         var myintable = -135;

        if(pos == 0)
        {
            cc.pAddIn(Play_position,cc.p(myintable+cardIndex*mycins,5));
        }
        else if(pos == 1)
        {
            cc.pAddIn(Play_position,cc.p(rightdistance+cardIndex*cins,5));
        }
        else if(pos == 2)
        {
            cc.pAddIn(Play_position,cc.p(rightdistance+cardIndex*cins,5));
        }
        else if(pos == 3 )
        {
            cc.pAddIn(Play_position,cc.p(leftdistance+cardIndex*cins,5));  
        }
        else if(pos == 4)
        {
            cc.pAddIn(Play_position,cc.p(leftdistance+cardIndex*cins,5));
        }

        
         return Play_position;
    },      

    getFixMyCardPos:function(players,pos,cardIndex)
    {
        var Play_position = players.getChildByName("player"+pos).getPosition();
        var mycins = 35;
        var myintable = -60;
        if(pos == 0)
        {
            cc.pAddIn(Play_position,cc.p(myintable+cardIndex*mycins,5));
        }
        return Play_position;
    },

    getSplitCardPos:function(players,pos,cardIndex)
    {
        var Play_position = players.getChildByName("player"+pos).getPosition();

         var cins = 35;
         var leftdistance = 90;
         var leftdistance_two = 150;
         var rightdistance = -90;
         var rightdistance_two = -150;
         var myintable = -25;

         var index = cardIndex%2;
         if(pos == 0)
        {
            cc.pAddIn(Play_position,cc.p(myintable+index*cins,120));
        }
        else if(pos == 1)
        {
            cc.pAddIn(Play_position,cc.p(rightdistance+index*cins,120));
        }
        else if(pos == 2)
        {
            cc.pAddIn(Play_position,cc.p(rightdistance_two+index*cins,100));
        }
        else if(pos == 3)
        {
            cc.pAddIn(Play_position,cc.p(leftdistance_two+index*cins,100)); 
        }
        else if(pos == 4 )
        {
            cc.pAddIn(Play_position,cc.p(leftdistance+index*cins,120));  
        }

        if(cardIndex<2)
        {
            cc.pAddIn(Play_position,cc.p(0,100));
        }

         return Play_position;
    },


    getChipStartPos:function(pos,playpostion)
    {
        var posv = cc.p(playpostion.x,playpostion.y);

        if(pos == 0)
        {
            cc.pAddIn(posv,cc.p(-50,130));  
        }
        else if(pos == 1)
        {
            cc.pAddIn(posv,cc.p(-380,0));  
        }
        else if(pos == 2)
        {
            cc.pAddIn(posv,cc.p(-380,0));  
        }
        else if(pos == 3)
        {
            cc.pAddIn(posv,cc.p(280,0));
        }
        else if(pos == 4)
        {
            cc.pAddIn(posv,cc.p(280,0));   
        }
        
        return posv;
    },
    getChipEndPos:function(pos,playpostion)
    {
        var posv = cc.p(playpostion.x,playpostion.y);

        if(pos == 0)
        {
            cc.pAddIn(posv,cc.p(50,180));  
        }
        else if(pos == 1)
        {
            cc.pAddIn(posv,cc.p(-280,50));  
        }
        else if(pos == 2)
        {
            cc.pAddIn(posv,cc.p(-280,50));
        }
        else if(pos == 3)
        {
            cc.pAddIn(posv,cc.p(380,50));
        }
        else if(pos == 4)
        {
            cc.pAddIn(posv,cc.p(380,50));   
        }
        
        return posv;
    },

    getBetMoneyBgPos:function(players,pos)
    {
        var betBgPos = players.getChildByName("player"+pos).getPosition();

        if(pos == 0)
        {
            cc.pAddIn(betBgPos,cc.p(0,100));  
        }
        else if(pos == 1 )
        {
            cc.pAddIn(betBgPos,cc.p(-330,-30));  
        }
        else if(pos == 2)
        {
            cc.pAddIn(betBgPos,cc.p(-330,-30));   
        }
        else if(pos == 3)
        {
            cc.pAddIn(betBgPos,cc.p(330,-30)); 
        }
        else if(pos == 4)
        {
            cc.pAddIn(betBgPos,cc.p(330,-30)); 
        }
        
        return betBgPos;
    },

    getPlayerPos:function(pos)
    {

        var bPos = null;
        if(pos == 0)
        {
            bPos = cc.p(640,165);
        }
        else if(pos == 1)
        {
            bPos = cc.p(1190,330);
        }
        else if(pos == 2)
        {
            bPos = cc.p(1190,540);
        }
        else if(pos == 3)
        {
            bPos = cc.p(90,540);
        }
        else if(pos == 4)
        {
            bPos = cc.p(90,330);
        }
        return bPos;
    },

    getoutBtnPos:function(index)
    {
        var bpos = cc.p(0,0);
        if(index == 0)
        {
            cc.pAddIn(bpos,cc.p(88,215));  
        }
        else if(index == 1)
        {
            cc.pAddIn(bpos,cc.p(195,112));
        }
        else if(index == 2)
        {
            cc.pAddIn(bpos,cc.p(360,70));
        }
        else if(index == 3)
        {
            cc.pAddIn(bpos,cc.p(525,112));
        }
        else if(index == 4)
        {
            cc.pAddIn(bpos,cc.p(632,215));
        }
    },




    createCircleAvatar:function(url, defualtFile, clipfile, size)
    {
        try {
            var savePath = this.getImageSavePath(url);

            if (this.hasImageFile(url)) {
                var avatar = new cc.Sprite(savePath);
                if (avatar == null) {
                    avatar = new cc.Sprite(defualtFile);
                }
                this.makeCircleSprite(avatar, savePath, clipfile, size);
                return avatar;
            }
            else {
                var avatar = new cc.Sprite(defualtFile);
                var self = this;
                this.makeCircleSprite(avatar, defualtFile, clipfile, size);
                CCHttpAgent.getInstance().sendHttpImage(url, savePath, savePath, function (tag) {
                    if (self.hasImageFile(url)) {
                        self.makeCircleSprite(avatar, savePath, clipfile, size);
                    }
                    CCHttpAgent.getInstance().popPackets(tag);
                }, 15);

                return avatar;
            }
        } catch (e) {
            var avatar = new cc.Sprite(defualtFile);
            var s_izex = size.width / avatar.getContentSize().width;
            var s_izey = size.height / avatar.getContentSize().height;
            avatar.setScale(s_izex, s_izey);
            return avatar;
        }
    },

makeCircleSprite:function(avatar_s, avatafile, clipfile, size)
{
    try {
            var Stencil = new cc.Sprite(clipfile);
            var avatar = new cc.Sprite(avatafile);

            if (avatar == null) {
                var s_izex = size.width / avatar_s.getContentSize().width;
                var s_izey = size.height / avatar_s.getContentSize().height;
                avatar_s.setScale(s_izex, s_izey);
                return;
            }
            var a_rect = avatar.getContentSize();
            var cl_rect = Stencil.getContentSize();

            var scaleX = (a_rect.width) / cl_rect.width;
            var scaleY = (a_rect.height) / cl_rect.height;

            var minscale = Math.min(scaleX, scaleY);
            Stencil.setScale(minscale);

            var renderTexture = new cc.RenderTexture(a_rect.width, a_rect.height);

            Stencil.setPosition(cc.p(a_rect.width / 2, a_rect.height / 2));
            avatar.setPosition(cc.p(a_rect.width / 2, a_rect.height / 2));

            Stencil.setBlendFunc(cc.ONE, cc.ZERO);//Stencil.setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
            avatar.setBlendFunc(cc.DST_ALPHA, cc.ZERO);//avatar.setBlendFunc((BlendFunc){GL_DST_ALPHA, GL_ZERO});

            renderTexture.begin();
            Stencil.visit();
            avatar.visit();
            renderTexture.end();

            avatar_s.setTexture(renderTexture.getSprite().getTexture());
            avatar_s.setFlippedY(true);
            var texture = avatar_s.getTexture();
            if (typeof texture.setAntiAliasTexParameters === "function") texture.setAntiAliasTexParameters();

            var s_izex = size.width / avatar_s.getContentSize().width;
            var s_izey = size.height / avatar_s.getContentSize().height;
            avatar_s.setScale(s_izex, s_izey);
        } catch (e) {
            var s_izex = size.width / avatar_s.getContentSize().width;
            var s_izey = size.height / avatar_s.getContentSize().height;
            avatar_s.setScale(s_izex, s_izey);
        }
},

cmin:function(one,two)
{
    if(one < two)
    {
        return one;
    }else{
        return two;
    }
},


     getFastChatSound:function(sex,chat)
    {
        var fastChat_map=[];
        fastChat_map["别墨迹了"] = "moji";
        fastChat_map["底裤都输光了"] = "shuguang";
        fastChat_map["风水轮流转"] = "fengshui";
        fastChat_map["今儿个真高兴"] = "gaoxing";
        fastChat_map["看我通杀全场"] = "tongsha";
        fastChat_map["快点下注吧"] = "xiazhu";
        fastChat_map["你太牛了"] = "tainiu";
        fastChat_map["下手别这样重啊"] = "xiashou";
        fastChat_map["咋比乌龟还慢呢"] = "wugui";
        fastChat_map["这些钱都是我的"] = "wode";
        cc.log(chat);
        if (fastChat_map.hasOwnProperty(chat))
        {
            return sex+"_"+fastChat_map[chat]+".mp3";
        }
        return "";
    },

    getTimeStamp:function()
    {
        return (new Date).valueOf();
    },

    getTime:function(t)
    {
        var date = new Date(t);
        return date.getFullYear()+"-"+this.pad(date.getMonth()+1,2)+"-"+this.pad(date.getDate(),2)+"  " +this.pad(date.getHours(),2)+":"+this.pad(date.getMinutes(),2)+":"+this.pad(date.getSeconds(),2);
    },

};


