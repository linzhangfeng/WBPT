
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
// 获取字符串数值
    getValStr:function(val,key)
    {
        if(val.hasOwnProperty(key))
        {
            return val[key];
        }
        return "";
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
       // ani.setRestoreOriginalFrame(true);
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
       // ani.setRestoreOriginalFrame(true);
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
        if(!node)
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
    scaleAll:function(node)
    {
        var size = cc.director.getWinSize();
        var bgSize = node.getContentSize();
        node.setScaleX(size.width/bgSize.width);
        node.setScaleY(size.height/bgSize.height);
        node.setAnchorPoint(cc.p(0,0));
        node.setPosition(0,0);
    },
    getTime:function(t)
    {
        var date = new Date(t);
        return date.getFullYear()+"-"+(date.getMonth()+1)+"-"+date.getDate()+"  "+date.getHours()+":"+date.getMinutes()+":"+date.getSeconds();
    },

    getTimeStamp:function()
    {
        return (new Date).valueOf();
    },
    createCircleAvatar: function (url, defualtFile, clipfile, size) {
        try {
            var savePath = Utils.getImageSavePath(url);

            if (Utils.hasImageFile(url)) {
                var avatar = new cc.Sprite(savePath);
                if (avatar == null) {
                    avatar = new cc.Sprite(defualtFile);
                }
                Utils.makeCircleSprite(avatar, savePath, clipfile, size);
                return avatar;
            }
            else {
                var avatar = new cc.Sprite(defualtFile);
                Utils.makeCircleSprite(avatar, defualtFile, clipfile, size);
                CCHttpAgent.getInstance().sendHttpImage(url, savePath, savePath, function (tag) {
                    if (Utils.hasImageFile(url)) {
                        Utils.makeCircleSprite(avatar, savePath, clipfile, size);
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
    getCCAnimate: function (buf, frameNums, frame, perunit, loop, to2d) {
        var SpriteAni = new cc.Animation();
        for (var i = 0; i < frame.length; i++) {
            var f = __String.createWithFormat(buf, to2d ? Utils.to2d(frame[i]) : frame[i]);
            SpriteAni.addSpriteFrameWithFile(f);
        }
        SpriteAni.setDelayPerUnit(perunit);

        return cc.animate(SpriteAni);
    },
};

var __String =
    {
        createWithFormat: function (string) {
            var args = arguments;
            var pattern = new RegExp("%([0-9]+)", "g");
            return String(string).replace(pattern
                , function (match, index) {
                    if (index == 0 || index >= args.length)
                        throw "Invalid index in format string";
                    return args[index];
                });
        }
    };

var time = function (v) {
    var t = (new Date).valueOf();
    return t;
};


