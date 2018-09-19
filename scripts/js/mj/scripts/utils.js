var time = function (v) {
    var t = (new Date).valueOf();
    return t;
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
var Utils = {

// 获取整形数值
    getValInt: function (val, key) {
        if (val.hasOwnProperty(key)) {
            return val[key];
        }
        return 0;
    },
// 获取字符串数值
    getValStr: function (val, key) {
        if (val.hasOwnProperty(key)) {
            return val[key];
        }
        return "";
    },

    //深拷贝对象
    clone: function (obj) {
        function Fn() {
        }

        Fn.prototype = obj;
        var o = new Fn();
        for (var a in o) {
            if (typeof o[a] == "object") {
                o[a] = Utils.clone(o[a]);
            }
        }

        return o;
    },
    //获取精灵帧
    spriteFrame: function (key) {
        return cc.spriteFrameCache.getSpriteFrame(key);
    },

    //转换16进制
    toHex2: function (num) {
        if (num < 16) {
            return "0" + num.toString(16);
        }
        return num.toString(16);
    },

    to2d: function (num) {
        if (num < 10) {
            return "0" + num.toString(10);
        }
        return num.toString(10);
    },

    scaleToSize: function (node, w, h) {
        node.setScaleX(w / node.getContentSize().width);
        node.setScaleY(h / node.getContentSize().height);
    }
    ,
    removeListData: function (list, data) {
        for (var i in list) {
            if (list[i] == data) {
                list.splice(i, 1);
                break;
            }

        }
    },
    newAni: function (key, start, end, dt, loop) {
        var ani = new cc.Animation();
        for (var i = start; i < end; ++i) {
            ani.addSpriteFrame(this.spriteFrame(key + i + ".png"));
        }
        ani.setDelayPerUnit(dt);
        ani.setRestoreOriginalFrame(true);
        if (loop && loop > 0) {
            ani.setLoops(loop);
        }
        return cc.animate(ani);
    },
    newAni1: function (key, list, dt, loop) {
        var ani = new cc.Animation();
        for (var i in list) {
            ani.addSpriteFrame(this.spriteFrame(key + list[i] + ".png"));
        }
        ani.setDelayPerUnit(dt);
        ani.setRestoreOriginalFrame(true);
        if (loop && loop > 0) {
            ani.setLoops(loop);
        }
        return cc.animate(ani);
    },
    parseName: function (max, name) {
        var l = name.length;
        var blen = 0;
        var len = [];
        var n = 0;
        for (var i = 0; i < l; i++) {
            n = 0;
            if ((name.charCodeAt(i) & 0xff00) != 0) {
                blen++;
                n++;
            }
            blen++;
            n++;
            len[i] = n;
        }
        if (blen > max) {
            n = 0;
            for (var i = 0; i < l; i++) {
                n += len[i];
                if (n >= max) {
                    n = i + 1;
                    break;
                }
            }
            return name.substr(0, n) + "..";
        }
        return name;
    },
    getSavePath: function () {
        return jsb.fileUtils.getWritablePath() + "Assets/";
    },
    getImageSavePath: function (fileUrl) {
        if (!fileUrl) {
            return;
        }
        var findFlag = "/chess_img/";
        var cutPos = fileUrl.indexOf(findFlag);
        if (cutPos <= 0) {
            return fileUrl;
        } else {
            var localPath = fileUrl.slice(cutPos + findFlag.length);
            return this.getSavePath() + "images/" + localPath;
        }

    },
    hasImageFile: function (f) {
        return jsb.fileUtils.isFileExist(this.getImageSavePath(f));
    },
    format: function (string) {
        var args = arguments;
        var pattern = new RegExp("%([0-9]+)", "g");
        return String(string).replace(pattern
            , function (match, index) {
                if (index == 0 || index >= args.length)
                    throw "Invalid index in format string";
                return args[index];
            });
    },
    Random: function (start, end) {
        return Math.floor(Math.random() * (end - start + 1)) + start;
    },
    randomf: function (start, end) {
        return Math.random() * (end - start) + start;
    },
    findNode: function (node, name) {
        if (!node) {
            return null;
        }
        if (node.getName() == name) {
            return node;
        }
        var cs = node.getChildren();
        for (var i in cs) {
            var ret = Utils.findNode(cs[i], name);
            if (ret) {
                return ret;
            }
        }
        return null;
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

    makeCircleSprite: function (avatar_s, avatafile, clipfile, size) {
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
    getFastChatSound: function (chat) {
        var fastChat_map = [];
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
        cc.log("" + chat);
        if (fastChat_map.hasOwnProperty(chat)) {
            return fastChat_map[chat];
        }
        return "";
    },
    getCCAnimate: function (buf, frameNums, frame, perunit, loop, to2d) {
        var SpriteAni = new cc.Animation();
        for (var i = 0; i < frame.length; i++) {
            var f = __String.createWithFormat(buf, to2d ? Utils.to2d(frame[i]) : frame[i]);
            SpriteAni.addSpriteFrameWithFile(f);
        }
        SpriteAni.setDelayPerUnit(perunit);

        return cc.animate(SpriteAni);
    }
};


