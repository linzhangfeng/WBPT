
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
    getValInt:function(val,key,d)
    {
        if(val.hasOwnProperty(key))
        {
            return val[key];
        }
        if(d != undefined)
        {
            return d;
        }
        return 0;
    },
// 获取bool数值
    getValBool:function(val,key)
    {
        if(val.hasOwnProperty(key))
        {
            return val[key];
        }
        return false;
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

    clone:function(o){
        if(typeof o !=="object" || o == null)
        {
            return o;
        }

        var n = {};
        if(o.constructor == Array)
        {
            n = [];
        }
        for (var i in o) {
            n[i] = this.clone(o[i]);
        }
        return n;
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
        for(var i = start;i<=end;++i)
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
        if(!node||!node.getName)
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
    getTimeStamp:function()
    {
        return (new Date).valueOf();
    },
    logObj:function(obj)
    {
        cc.log(JSON.stringify(obj));
    },

    showAvatarIcon:function(icon,avatar,sex)
    {
        var path = utils.getImageSavePath(avatar);
        if(jsb.fileUtils.isFileExist(path))
        {
            icon.loadTexture(path);
        }
        else
        {
            icon.loadTexture("public/res/pic/avatars/"+(sex == 1 ? "gg":"mm")+".png");
            CCHttpAgent.getInstance().sendHttpImage(avatar,path
                ,path,function(tag)
                {
                    CCHttpAgent.getInstance().popPackets(tag);
                    try
                    {
                        if(jsb.fileUtils.isFileExist(path)&&icon.getParent())
                        {
                            icon.loadTexture(path);
                        }
                    }
                    catch (e)
                    {

                    };
                },15);
        }
    },
    //排序按钮
    sortNodes:function(nodes,midX,midY,dx)
    {
        var num =0;
        var len = 0;
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
    addPP:function(p0,p1)
    {
        return cc.p(p0.x+p1.x,p0.y+p1.y);
    },
    subPP:function(p0,p1)
    {
        return cc.p(p0.x-p1.x,p0.y-p1.y);
    },
    addPS:function(p,s)
    {
        return cc.p(p.x+s.width,p.y+ s.height);
    },
    atlasNum:function(num)
    {
        if(num <0)
        {
            return ":"+Math.abs(num);
        }
        return ":"+num;
    },
    jjNum:function(num)
    {
        if(num >0)
        {
            return "+"+num;
        }
        return ""+num;
    },
    ssNum:function(num)
    {
        if(num <0)
        {
            return ":"+(-num);
        }
        return ""+num;
    },
    sjNum:function(num)
    {
        if(num <0)
        {
            return ":"+(-num);
        }
        return ";"+num;
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
        return date.getFullYear()+"-"+this.pad(date.getMonth()+1,2)+"-"+this.pad(date.getDate(),2)+"  "
            +this.pad(date.getHours(),2)+":"+this.pad(date.getMinutes(),2)+":"+this.pad(date.getSeconds(),2);
    }

};


