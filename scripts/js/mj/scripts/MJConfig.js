var ROOM_TYPE =
    {
        ROOM_TYPE_ZJH_DZ: 1001,
        ROOM_TYPE_ZJH_DZ_SET: 2001,
        ROOM_TYPE_DDZ_DZ: 1003,
        ROOM_TYPE_DZ_DZ: 1011,
        ROOM_TYPE_MJ_DZ: 1009,
        ROOM_TYPE_MJ_PH_DZ: 1010,

        ROOM_TYPE_MJ_PH_DZ_DZ: 2201,//麻将定制
        ROOM_TYPE_GZMJ_DZ_DZ: 2202,//贵州麻将定制
        ROOM_TYPE_DDZ_DZ_DZ: 2101,//斗地主定制
        ROOM_TYPE_DDZ_DN_DZ: 2301, //斗牛定制
        ROOM_TYPE_DDZ_ZDN_DZ: 2302, //斗牛定制 --有庄

        ROOM_TYPE_SCMJ_DZ: 2203,//四川麻将定制
        ROOM_TYPE_GDMJ_DZ: 2204,//广东麻将定制
        ROOM_TYPE_SCMJ2_DZ: 2205,//四川麻将新
        ROOM_TYPE_YNMJ_DZ : 2206,
        ROOM_TYPE_HNMJ_DZ: 2208,//海南麻将
        ROOM_TYPE_HUNAN_DZ: 2210,//湖南麻将
        ROOM_TYPE_JXMJ_DZ: 2211,//江西麻将

    };

var ProtoType =
    {
        JSON: 0,
        PROTOBUF_WANBEI: 1,
        PROTOBUF_CJJ: 2,
        PROTOBUF_CJJ2 : 3,
        PROTOBUF_CJJ3: 4,
    };

var SEX_ID =
    {
        SEX_MAN: 1,
        SEX_FEMALE: 2,
    };

var MJ_TYPE =
    {
        MJ_TYPE_EMPTY: -1, //
        MJ_TYPE_WAN: 0, //万,0-8,各4张，共36张
        MJ_TYPE_TIAO: 1, //条,0-8,各4张，共36张
        MJ_TYPE_TONG: 2, //筒,0-8,各4张，共36张
    };

var PengType =
    {
        Peng_Peng: 0,
        Peng_Kang: 1,
        Peng_AnKang: 2,
        Chi_L: 3,
        Chi_M: 4,
        Chi_R: 5,
    };

var CardType =
    {
        Card_Hand: 0,
        Card_Out: 1,
        Card_Peng: 2,
        Card_Hu: 3,
        Card_Outing: 4,
        Card_Mo: 5,
        Card_End: 6,
    };

var MJ_STATUS =
    {
        MJ_GAME_FREE: 0,				//空闲场景
        MJ_GAME_CHANGECARD: 1,		//换牌场景
        MJ_GAME_CHOOSE_LACKTYPE: 2,	//选择定缺色场景
        MJ_GAME_PLAYING: 3,			//游戏中场景
        END_GAME: 4,
    };

var tagHuInfo =
    {
        Card: 0,//听的牌
        LeftNum: 0,//剩余张数
        HuFanNum: 0,//胡牌番数
    };

var tagListenInfo =
    {
        OutCard: 0,	//要打出的牌
        HuInfoLen: 0,
        HuInfo: [],//打出牌后的胡牌信息
    };

var tagGangInfo =
    {
        curCard: 0,	//杠的牌
        state: 0,      //杠的类型(弯杠 暗杠)
    };

var ActionType =
    {
        TYPE_NULL: 0x00,
        TYPE_PENG: 0x01,
        TYPE_ZHIGANG: 0x02,
        TYPE_WANGANG: 0x04,
        TYPE_ANGANG: 0x08,
        TYPE_HU: 0x10,
        TYPE_LISTEN: 0x20,
        TYPE_PASS: 0xFF,
        TYPE_BAOTING: 0x40,
        TYPE_LEFT_CHI: 0x40,
        TYPE_CENTER_CHI: 0x80,
        TYPE_RIGHT_CHI: 0x100,
        TYPE_HUA_HU: 0x200,
        TYPE_BUZHANG: 0x400,//补张
        TYPE_HAID_YAO: 0x800,//要海底牌
        TYPE_JINHUAN_PAI: 0x1000,//禁换手牌


        JITYPE_CHONGFENGJI: 1,
        JITYPE_ZERENJI: 2,

        INVALID_CHAIR         :      0xFF
    };

//听牌类型
var LISTEN_TYPE =
    {
        LISTEN_CARD_NOMAL: 0,					//常规听牌处理
        LISTEN_CARD_BAOTING: 1,				//报听
        LISTEN_CARD_BAIPAI: 2,				//摆牌
        LISTEN_CARD_FIRST_BAOPAI: 3,			//首张报牌
    };

var TCPCMD =
    {
        CONNECT_OK_RES: 0,
        CONNECT_ERROR_RES: 1,
        SEND_DATA_OK_RES: 2,
        SEND_DATA_ERROR_RES: 3,
        RECV_DATA_OK_RES: 4,
        RECV_DATA_ERROR_RES: 5,
        DISCONNECT_RES: 6,
    }


var MJConfig =
    {
        getTablePos: function () {
            var size = cc.director.getWinSize();
            return cc.p(size.width / 2, size.height / 2);
        },

        getPlayerPos: function (index) {
            var size = cc.director.getWinSize();

            var headSize = cc.size(95, 112);

            var begin;
            if (index == 0) {
                if (MJModel.seatid != -1) {
                    begin = cc.p(8 + headSize.width / 2, 135 + headSize.height / 2);
                }
                else {
                    begin = cc.p(10 + headSize.width / 2, 10 + headSize.height / 2);
                }
            }
            else if (index == 1) {
                begin = cc.p(size.width - 8 - headSize.width / 2, 345 + headSize.height / 2);
            }
            else if (index == 2) {
                begin = cc.p(265 + headSize.width / 2, size.height - 12 - headSize.height / 2);
            }
            else if (index == 3) {
                begin = cc.p(8 + headSize.width / 2, 345 + headSize.height / 2);
            }
            return begin;
        },

        getOutCardPos: function (pos, index) {
            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                var carsize = cc.size(40, 61);
                if (index < 6) {
                    var startX = size.width / 2 - 3 * (carsize.width) + carsize.width / 2;
                    _vec.x = startX + index * (carsize.width);
                    _vec.y = 292 - carsize.height / 2;
                } else if (index < 14) {
                    var startpos = index - 6;
                    var startX = size.width / 2 - 4 * (carsize.width) + carsize.width / 2;
                    _vec.x = startX + startpos * (carsize.width);
                    _vec.y = 247 - carsize.height / 2;
                } else {
                    var startpos = index - 14;
                    var startX = size.width / 2 - 5 * (carsize.width) + carsize.width / 2;
                    _vec.x = startX + startpos * (carsize.width);
                    _vec.y = 202 - carsize.height / 2;

                }

            }
            else if (pos == 1) {
                var carsize = cc.size(51, 47);
                ;
                if (index < 6) {

                    _vec.x = size.width - 520 + carsize.width / 2;
                    var startY = 295;
                    _vec.y = startY + carsize.height + index * 30 - carsize.height / 2;

                } else if (index < 14) {
                    var startpos = index - 6;
                    _vec.x = size.width - 470 + carsize.width / 2;
                    var startY = 265;
                    _vec.y = startY + carsize.height + startpos * 30 - carsize.height / 2;
                } else {
                    var startpos = index - 14;
                    _vec.x = size.width - 420 + carsize.width / 2;
                    var startY = 235;
                    _vec.y = startY + carsize.height + startpos * 30 - carsize.height / 2;
                }
            }
            else if (pos == 2) {
                var carsize = cc.size(40, 61);
                if (index < 6) {
                    var startX = size.width / 2 + 3 * (carsize.width) - carsize.width / 2;
                    _vec.x = startX - index * (carsize.width);
                    _vec.y = 555 - carsize.height / 2;
                } else if (index < 14) {
                    var startpos = index - 6;
                    var startX = size.width / 2 + 4 * (carsize.width) - carsize.width / 2;
                    _vec.x = startX - startpos * (carsize.width);
                    _vec.y = 600 - carsize.height / 2;
                } else {
                    var startpos = index - 14;
                    var startX = size.width / 2 + 5 * (carsize.width) - carsize.width / 2;
                    _vec.x = startX - startpos * (carsize.width);
                    _vec.y = 645 - carsize.height / 2;

                }
            }
            else if (pos == 3) {
                var carsize = cc.size(51, 47);
                ;
                if (index < 6) {
                    _vec.x = 468 + carsize.width / 2;
                    var startY = 491;
                    _vec.y = startY - index * 30 - carsize.height / 2;

                } else if (index < 14) {
                    var startpos = index - 6;
                    _vec.x = 418 + carsize.width / 2;
                    var startY = 521;
                    _vec.y = startY - startpos * 30 - carsize.height / 2;
                } else {
                    var startpos = index - 14;
                    _vec.x = 368 + carsize.width / 2;
                    var startY = 551;
                    _vec.y = startY - startpos * 30 - carsize.height / 2;
                }
            }
            return _vec;
        },


        getOutCardOrder: function (pos, index) {
            if (pos == 1) {
                if (index < 6) {
                    return 50 - index;
                }
                else if (index < 14) {
                    return 40 - index;
                }
                return 30 - index;
            }
            else if (pos == 2) {
                return 100 - index;
            }
            else if (pos == 3) {
                if (index < 6) {
                    return 50 + index;
                }
                else if (index < 14) {
                    return 40 + index;
                }
                return 0 + index;
            }
            return index;
        },


        getOutingCardPos: function (pos, index) {
            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                _vec.x = size.width / 2;
                _vec.y = 260;
            }
            else if (pos == 1) {
                _vec.x = size.width - 260;
                _vec.y = size.height / 2;
            }
            else if (pos == 2) {
                _vec.x = size.width / 2;
                _vec.y = 580;
            }
            else if (pos == 3) {
                _vec.x = 260;
                _vec.y = size.height / 2;
            }

            return _vec;
        },

        getOutingCardOrder: function (pos, index) {

            return 1000;
        },

        getHandCardOrder: function (pos, index) {

            if (pos == 1) {
                return 220 - index;
            } else if (pos == 0) {
                return 300 + index;
            }
            return 220 + index;
        },

        getHandCardPos: function (pos, index, all) {
            if (MJModel.isOnVideo) {
                return this.getHandCardShowPos(pos, index, all);
            }

            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                if (MJModel.seatid != -1) {

                    var cardSize = cc.size(84, 123);
                    _vec.x = size.width - 175 - (all - index) * cardSize.width - cardSize.width / 2;
                    _vec.y = 5 + cardSize.height / 2;
                } else {

                    var cardSize = cc.size(70, 103);
                    _vec.x = size.width - 220 - (all - index) * cardSize.width - cardSize.width / 2;
                    _vec.y = 10 + cardSize.height / 2;
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(26, 71);
                _vec.x = size.width - 135 - cardSize.width / 2;
                _vec.y = size.height - 160 - cardSize.height - (30) * (all - index - 1) + cardSize.height / 2;
            }
            else if (pos == 2) {
                var cardSize = cc.size(40, 61);
                _vec.x = 418 + cardSize.width / 2 + (all - index) * cardSize.width;
                _vec.y = size.height - 5 - cardSize.height / 2;
            }
            else if (pos == 3) {
                var cardSize = cc.size(26, 71);
                _vec.x = 135 + cardSize.width / 2;
                _vec.y = 235 + cardSize.height + (30) * (all - index - 1) - cardSize.height / 2;
            }

            return _vec;
        },

        getHandCardShowPos: function (pos, index, all) {
            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                if (MJModel.seatid != -1) {
                    var cardSize = cc.size(55, 80);
                    _vec.x = size.width - 193 - (all - index) * cardSize.width - cardSize.width / 2;
                    _vec.y = 5 + cardSize.height / 2;
                } else {
                    var cardSize = cc.size(56, 86);
                    _vec.x = size.width - 220 - (all - index) * cardSize.width - cardSize.width / 2;
                    _vec.y = 10 + cardSize.height / 2;
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(51, 47);
                _vec.x = size.width - 135 - cardSize.width / 2;
                _vec.y = size.height - 160 - cardSize.height - (30) * (all - index - 1) + cardSize.height / 2;
            }
            else if (pos == 2) {
                var cardSize = cc.size(40, 61);
                _vec.x = 418 + cardSize.width / 2 + (all - index) * cardSize.width;
                _vec.y = size.height - 15 - cardSize.height / 2;
            }
            else if (pos == 3) {
                var cardSize = cc.size(51, 47);
                _vec.x = 135 + cardSize.width / 2;
                _vec.y = 235 + cardSize.height + (30) * (all - index - 1) - cardSize.height / 2;
            }

            return _vec;
        },

        getMoCardPos: function (pos) {
            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                if (MJModel.seatid != -1) {
                    var cardSize = cc.size(84, 123);
                    _vec.x = size.width - 78 - cardSize.width / 2;
                    _vec.y = 5 + cardSize.height / 2;

                } else {
                    var cardSize = cc.size(70, 103);
                    _vec.x = size.width - 135 - cardSize.width / 2;
                    _vec.y = 10 + cardSize.height / 2;
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(26, 71);
                _vec.x = size.width - 135 - cardSize.width / 2;
                _vec.y = size.height - 85 - cardSize.height / 2;
            }
            else if (pos == 2) {
                var cardSize = cc.size(40, 61);
                _vec.x = 368 + cardSize.width / 2;
                _vec.y = size.height - 5 - cardSize.height / 2;
            }
            else if (pos == 3) {
                var cardSize = cc.size(26, 71);
                _vec.x = 135 + cardSize.width / 2;
                _vec.y = 160 + cardSize.height / 2;

            }

            return _vec;
        },

        getHuaCardPos: function (pos , index) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if(pos == 0){
                var cardSize = cc.size(33, 46);
                begin = cc.p(190 + cardSize.width / 2 + (index) * 30, 180 - cardSize.height / 2);
            }else if(pos == 1){
                var cardSize = cc.size(41, 36);
                begin = cc.p(size.width - 190 - cardSize.width / 2, 322 - cardSize.height - 21 * index + cardSize.height / 2);
            }else if(pos == 2){
                var cardSize = cc.size(33, 46);
                begin = cc.p(size.width - 430 + cardSize.width / 2 + (index) * 30, size.height - 78 - cardSize.height / 2);
            }else if(pos == 3){
                var cardSize = cc.size(41, 36);
                begin = cc.p(190 + cardSize.width / 2, size.height - 116 - cardSize.height / 2 - 21 * index);
            }
            return begin;
        },

        getHuCardPos: function (pos, index) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                var cardSize = cc.size(33, 46);

                if (index < 6) {
                    begin = cc.p(225 + cardSize.width / 2 + (index) * 30, 222 - cardSize.height / 2);
                } else {
                    begin = cc.p(225 + cardSize.width / 2 + (index - 6) * 30, 185 - cardSize.height / 2);
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(size.width - 248 - cardSize.width / 2, 167 + cardSize.height + 21 * index - cardSize.height / 2);
                } else {
                    begin = cc.p(size.width - 209 - cardSize.width / 2, 167 + cardSize.height + 21 * (index - 6) - cardSize.height / 2);
                }

            }
            else if (pos == 2) {
                var cardSize = cc.size(33, 46);
                if (index < 6) {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index) * 30, size.height - 129 - cardSize.height / 2);
                } else {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index - 6) * 30, size.height - 93 - cardSize.height / 2);
                }

            }
            else if (pos == 3) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(248 + cardSize.width / 2, size.height - 176 - cardSize.height / 2 - 21 * index);
                } else {
                    begin = cc.p(209 + cardSize.width / 2, size.height - 176 - cardSize.height / 2 - 21 * (index - 6));
                }

            }
            return begin;
        },

        getNiaoCardPos:function (pos, index) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                var cardSize = cc.size(29, 43);
                begin = cc.p(220 + cardSize.width / 2 + index * 26, 192 - cardSize.height / 2);
            }
            else if (pos == 1) {
                var cardSize = cc.size(36, 31);
                begin = cc.p(size.width - 224 - cardSize.width / 2, 150 + cardSize.height + 21 * index - cardSize.height / 2);
            }
            else if (pos == 2) {
                var cardSize = cc.size(29, 43);
                begin = cc.p(size.width - 202 - cardSize.width / 2 - (index) * 26, size.height - 92 - cardSize.height / 2);
            }
            else if (pos == 3) {
                var cardSize = cc.size(36, 31);
                begin = cc.p(225 + cardSize.width / 2, size.height - 130 - cardSize.height / 2 - 21 * index);
            }
            return begin;
        },
        
        getNiaoBigCardPos:function (index, length) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (length == 2) {
                var cardSize = cc.size(70, 104);
                begin = cc.p(570 + cardSize.width / 2  + index * 72, 329 + cardSize.height / 2);
            } else if (length == 4) {
                var cardSize = cc.size(70, 104);
                begin = cc.p(500 + cardSize.width / 2  + index * 72, 329 + cardSize.height / 2);
            } else {
                var cardSize = cc.size(70, 104);
                begin = cc.p(430 + cardSize.width / 2  + index * 72, 329 + cardSize.height / 2);
            }
            cc.log("begin:" + begin);
            return begin;
        },

        getZhongMaCardPos1: function (pos, index) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                var cardSize = cc.size(33, 46);

                if (index < 6) {
                    begin = cc.p(225 + cardSize.width / 2 + (index) * 28 + 3, 222 - cardSize.height / 2 - 3);
                } else {
                    begin = cc.p(225 + cardSize.width / 2 + (index - 6) * 28 + 3, 185 - cardSize.height / 2);
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(size.width - 248 - cardSize.width / 2, 167 + cardSize.height + 21 * index - cardSize.height / 2);
                } else {
                    begin = cc.p(size.width - 209 - cardSize.width / 2, 167 + cardSize.height + 21 * (index - 6) - cardSize.height / 2);
                }

            }
            else if (pos == 2) {
                var cardSize = cc.size(33, 46);
                if (index < 6) {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index) * 28 + 40, size.height - 129 - cardSize.height / 2);
                } else {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index - 6) * 28 + 40, size.height - 93 - cardSize.height / 2);
                }

            }
            else if (pos == 3) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(248 + cardSize.width / 2, size.height - 176 - cardSize.height / 2 - 21 * index);
                } else {
                    begin = cc.p(209 + cardSize.width / 2, size.height - 176 - cardSize.height / 2 - 21 * (index - 6));
                }

            }
            return begin;
        },

        getZhongMaCardPos2: function (pos, index) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                var cardSize = cc.size(33, 46);

                if (index < 6) {
                    begin = cc.p(225 + cardSize.width / 2 - 34 + (index) * 28, 222 - cardSize.height / 2 + 48);
                } else {
                    begin = cc.p(225 + cardSize.width / 2 -34 + (index - 6) * 28, 185 - cardSize.height / 2 + 50);
                }

            }
            else if (pos == 1) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(size.width - 248 - cardSize.width / 2 - 35, 167 + cardSize.height + 21 * index - cardSize.height / 2 + 5);
                } else {
                    begin = cc.p(size.width - 209 - cardSize.width / 2 - 35, 167 + cardSize.height + 21 * (index - 6) - cardSize.height / 2 + 5);
                }

            }
            else if (pos == 2) {
                var cardSize = cc.size(33, 46);
                if (index < 6) {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index) * 28 + 40, size.height - 129 - cardSize.height / 2 - 35);
                } else {
                    begin = cc.p(size.width - 230 - cardSize.width / 2 - (index - 6) * 28 + 40, size.height - 93 - cardSize.height / 2 - 35);
                }

            }
            else if (pos == 3) {
                var cardSize = cc.size(41, 36);
                if (index < 6) {
                    begin = cc.p(248 + cardSize.width / 2 + 35, size.height - 176 - cardSize.height / 2 - 21 * index + 25);
                } else {
                    begin = cc.p(209 + cardSize.width / 2 + 35, size.height - 176 - cardSize.height / 2 - 21 * (index - 6) + 25);
                }

            }
            return begin;
        },

        getHuCardOrder: function (pos, index) {
            var level = 100;
            if (pos == 0) {
                if (index < 6) {
                    level = 100 - index;
                } else {
                    level = 120 - index;
                }
            } else if (pos == 1) {
                level = 100 - index;
            } else if (pos == 2) {
                level = 100 - index;
            }
            else if (pos == 3) {
                if (index < 6) {
                    level = 100 + index;
                } else {
                    level = 80 + index;
                }

            }
            return level;
        },

        getPengKangCardPos: function (pos, index) {
            var _vec = cc.p(0, 0);
            var size = cc.director.getWinSize();
            if (pos == 0) {
                if (MJModel.seatid != -1) {

                    var cardSize = cc.size(55, 80);
                    _vec.x = 16 + (cardSize.width * 3) * (index) + 10 * index;
                    _vec.y = 5 + cardSize.height / 2;
                } else {
                    var cardSize = cc.size(56, 86);
                    _vec.x = 155 + (cardSize.width * 3) * (index) + 10 * index;
                    _vec.y = 10 + cardSize.height / 2;
                }
            }
            else if (pos == 1) {
                var cardSize = cc.size(51, 47);
                _vec.x = size.width - 135 - cardSize.width / 2;
                _vec.y = 150 + (cardSize.height + 30 * 2) * (index) - 18 * index;
            }
            else if (pos == 2) {
                var cardSize = cc.size(40, 61);
                _vec.x = size.width - 285 - (cardSize.width * 3) * (index) - 5 * index;
                _vec.y = size.height - 15 - cardSize.height / 2;
            }
            else if (pos == 3) {
                var cardSize = cc.size(51, 47);
                _vec.x = 135 + cardSize.width / 2;
                _vec.y = size.height - 90 - (cardSize.height + 30 * 2) * (index) + 18 * index;
            }
            return _vec;
        },


        getPengKangCardOrder: function (pos, index) {
            if (pos == 1) {
                return 20 - index + 220;
            }
            return 10 + index;
        },

        getPengKangCardPos2: function (pos, index, dex) {
            var _vec = cc.p(0, 0);
            var _midpos = this.getPengKangCardPos(pos, index);
            if (pos == 0) {
                if (MJModel.seatid != -1) {
                    var cardSize = cc.size(55, 80);
                    if (dex == 3) {
                        _vec.x = _midpos.x + cardSize.width + cardSize.width / 2;
                        _vec.y = _midpos.y + 18;
                    } else {
                        _vec.x = _midpos.x + cardSize.width * dex + cardSize.width / 2;
                        _vec.y = _midpos.y;
                    }
                } else {
                    var cardSize = cc.size(56, 86);

                    if (dex == 3) {
                        _vec.x = _midpos.x + cardSize.width + cardSize.width / 2;
                        _vec.y = _midpos.y + 22;
                    } else {
                        _vec.x = _midpos.x + cardSize.width * dex + cardSize.width / 2;
                        _vec.y = _midpos.y;
                    }
                }

            }
            else if (pos == 1) {

                var cardSize = cc.size(51, 47);

                _vec.x = _midpos.x;
                if (dex == 3) {
                    _vec.y = _midpos.y + cardSize.height * 2 - cardSize.height / 2;
                } else {
                    _vec.y = _midpos.y + cardSize.height + 30 * dex - cardSize.height / 2;
                }

            }
            else if (pos == 2) {
                var cardSize = cc.size(40, 61);
                if (dex == 3) {
                    _vec.x = _midpos.x - cardSize.width - cardSize.width / 2;
                    _vec.y = _midpos.y + 15;

                } else {
                    _vec.x = _midpos.x - cardSize.width * dex - cardSize.width / 2;
                    _vec.y = _midpos.y;

                }

            }
            else if (pos == 3) {

                var cardSize = cc.size(51, 47);


                _vec.x = _midpos.x;
                if (dex == 3) {
                    _vec.y = _midpos.y - 30 * 2 + cardSize.height - cardSize.height / 2;
                } else {
                    _vec.y = _midpos.y - 30 * dex - cardSize.height / 2;
                }

            }
            return _vec;
        },


        getCardValuePos: function (pos, type, _size) {
            var _vec = cc.p(_size.width / 2, _size.height / 2);
            switch (type) {
                case CardType.Card_Out: {
                    if (pos == 0) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 8;
                    } else if (pos == 1) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 8;
                    } else if (pos == 2) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 8;
                    } else if (pos == 3) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 8;
                    }
                }
                    break;
                case CardType.Card_Outing: {
                    _vec.x = _vec.x;
                    _vec.y = _vec.y + 10;

                }
                    break;
                case CardType.Card_Hand: {
                    if (pos == 0) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y - 12;
                    } else if (pos == 1) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    } else if (pos == 2) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    } else if (pos == 3) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    }
                }
                    break;
                case CardType.Card_Peng:
                case CardType.Card_End: {
                    if (pos == 0) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 10;
                    } else if (pos == 1) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    } else if (pos == 2) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    } else if (pos == 3) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 7;
                    }
                }
                    break;
                case CardType.Card_Hu: {
                    if (pos == 0) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 5;
                    } else if (pos == 1) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 5;
                    } else if (pos == 2) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 5;
                    } else if (pos == 3) {
                        _vec.x = _vec.x;
                        _vec.y = _vec.y + 5;
                    }
                }
                    break;
            }

            return _vec;
        },

        getPengKangCardOrder2: function (pos, index, dex) {
            var _level;
            switch (pos) {
                case 0: {
                    _level = (dex + 1) + (index + 1) * 5;
                }
                    break;
                case 1: {
                    if (dex <= 2) {
                        _level = (3 - dex) + (8 - index) * 5;
                    } else {
                        _level = 4 + (8 - index) * 5;
                    }
                }
                    break;
                case 2: {
                    _level = (dex + 1) + (index + 1) * 5;
                }
                    break;
                case 3: {
                    _level = (dex + 1) + (index + 1) * 5;
                }
                    break;
            }
            return _level;
        },

        ///////////////////////////////////////////////
        getCardBgStr: function (value, pos, type, index) {

            var _reValue = "";

            if (pos == 0) {
                if (type == CardType.Card_Hand) {
                    if (MJModel.seatid != -1) {
                        if (value == 0) {
                            _reValue = "RoomMJ2/card_bg/my_an_gang_card.png";
                        }
                        else {
                            _reValue = "RoomMJ2/card_bg/my_hand_card.png";
                        }


                    } else {

                        _reValue = "RoomMJ2/card_bg/back_card_0.png";
                    }

                }
                else if (type == CardType.Card_Outing) {
                    _reValue = "RoomMJ2/card_bg/outing_card_v.png";

                }
                else if (type == CardType.Card_Hu) {
                    _reValue = "RoomMJ2/card_bg/hu_card_v.png";

                }
                else if (type == CardType.Card_Peng || type == CardType.Card_End) {

                    if (MJModel.seatid != -1) {

                        if (value == 0) {
                            _reValue = "RoomMJ2/card_bg/my_an_gang_card.png";
                        }
                        else {
                            _reValue = "RoomMJ2/card_bg/my_peng_card.png";
                        }
                    } else {
                        if (value == 0) {
                            _reValue = "RoomMJ2/card_bg/gang_card_0.png";
                        }
                        else {
                            _reValue = "RoomMJ2/card_bg/peng_gang_card_0.png";
                        }
                    }

                }
                else if (type == CardType.Card_Mo) {
                    if (MJModel.seatid != -1) {
                        _reValue = "RoomMJ2/card_bg/my_hand_back.png";
                    } else {
                        _reValue = "RoomMJ2/card_bg/back_card_0.png";
                    }

                }
                else if (type == CardType.Card_Out) {

                    _reValue = "RoomMJ2/card_bg/out_card_v.png";
                }
            }
            else if (pos == 1) {
                if (type == CardType.Card_Hand) {

                    _reValue = "RoomMJ2/card_bg/back_card_1.png";

                } else if (type == CardType.Card_Outing) {

                    _reValue = "RoomMJ2/card_bg/outing_card_h.png";

                } else if (type == CardType.Card_Hu) {

                    _reValue = "RoomMJ2/card_bg/hu_card_h.png";

                } else if (type == CardType.Card_Peng || type == CardType.Card_End) {

                    if (value == 0) {

                        _reValue = "RoomMJ2/card_bg/gang_card_h.png";

                    } else {

                        _reValue = "RoomMJ2/card_bg/out_card_h.png";
                    }

                } else if (type == CardType.Card_Mo) {


                    _reValue = "RoomMJ2/card_bg/gang_card_h.png";

                } else if (type == CardType.Card_Out) {

                    _reValue = "RoomMJ2/card_bg/out_card_h.png";
                }
            }
            else if (pos == 2) {
                if (type == CardType.Card_Hand) {

                    _reValue = "RoomMJ2/card_bg/back_card_2.png";

                } else if (type == CardType.Card_Outing) {

                    _reValue = "RoomMJ2/card_bg/outing_card_v.png";

                } else if (type == CardType.Card_Hu) {

                    _reValue = "RoomMJ2/card_bg/hu_card_v.png";

                } else if (type == CardType.Card_Peng || type == CardType.Card_End) {

                    if (value == 0) {

                        _reValue = "RoomMJ2/card_bg/gang_card_v.png";

                    } else {

                        _reValue = "RoomMJ2/card_bg/out_card_v.png";
                    }

                } else if (type == CardType.Card_Mo) {


                    _reValue = "RoomMJ2/card_bg/gang_card_v.png";

                } else if (type == CardType.Card_Out) {


                    _reValue = "RoomMJ2/card_bg/out_card_v.png";
                }
            }

            else if (pos == 3) {
                if (type == CardType.Card_Hand) {

                    _reValue = "RoomMJ2/card_bg/back_card_3.png";

                } else if (type == CardType.Card_Outing) {

                    _reValue = "RoomMJ2/card_bg/outing_card_h.png";

                } else if (type == CardType.Card_Hu) {

                    _reValue = "RoomMJ2/card_bg/hu_card_h.png";

                } else if (type == CardType.Card_Peng || type == CardType.Card_End) {

                    if (value == 0) {

                        _reValue = "RoomMJ2/card_bg/gang_card_h.png";

                    } else {

                        _reValue = "RoomMJ2/card_bg/out_card_h.png";
                    }

                } else if (type == CardType.Card_Mo) {


                    _reValue = "RoomMJ2/card_bg/gang_card_h.png";

                } else if (type == CardType.Card_Out) {

                    _reValue = "RoomMJ2/card_bg/out_card_h.png";
                }
            }

            return getResPath(_reValue);
        },

        dealCardParam: function (card, pos, type, index) {
            if (card != null) {
                if (type == CardType.Card_Out) {
                    var f = 1.0;
                    if (pos == 0) {
                        if (index < 7) {
                            card = cc.pScale(f * 0.99 * 0.99);
                        } else if (index < 16) {
                            card = cc.pScale(f * 0.99);
                        } else {
                            card = cc.pScale(f);
                        }
                    } else if (pos == 1) {
                        if (index < 7) {
                            card = cc.pScale(f * pow(0.99, index));
                        } else if (index < 16) {
                            var cur = index - 7;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else {
                            var cur = index - 16;
                            card = cc.pScale(f * pow(0.99, cur));
                        }
                    } else if (pos == 2) {
                        if (index < 7) {
                            card = cc.pScale(f);
                        } else if (index < 16) {
                            card = cc.pScale(f * 0.99);
                        } else {
                            card = cc.pScale(f * 0.99 * 0.99);
                        }
                    } else if (pos == 3) {
                        if (index < 7) {
                            var cur = 7 - index;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else if (index < 16) {
                            var cur = 16 - index;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else {
                            var cur = 27 - index;
                            if (cur < 0) {
                                cur = 0;
                            }
                            card = cc.pScale(f * pow(0.99, cur));
                        }
                    }
                }
            }
        },

        dealCardValueParam: function (card, pos, type, index) {
            if (card != null) {
                if (type == CardType.Card_Out) {
                    var f = 1.0;
                    if (pos == 0) {
                        if (index < 7) {
                            card = cc.pScale(f * 0.99 * 0.99);
                        } else if (index < 16) {
                            card = cc.pScale(f * 0.99);
                        } else {
                            card = cc.pScale(f);
                        }
                    } else if (pos == 1) {
                        if (index < 7) {
                            card = cc.pScale(f * pow(0.99, index));
                        } else if (index < 16) {
                            var cur = index - 7;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else {
                            var cur = index - 16;
                            card = cc.pScale(f * pow(0.99, cur));
                        }
                    } else if (pos == 2) {
                        if (index < 7) {
                            card = cc.pScale(f);
                        } else if (index < 16) {
                            card = cc.pScale(f * 0.99);
                        } else {
                            card = cc.pScale(f * 0.99 * 0.99);
                        }
                    } else if (pos == 3) {
                        if (index < 7) {
                            var cur = 7 - index;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else if (index < 16) {
                            var cur = 16 - index;
                            card = cc.pScale(f * pow(0.99, cur));
                        } else {
                            var cur = 27 - index;
                            if (cur < 0) {
                                cur = 0;
                            }
                            card = cc.pScale(f * pow(0.99, cur));
                        }
                    }
                } else if (type == CardType.Card_Peng || type == CardType.Card_End) {
                    if (pos == 2) {
                        card = cc.pScale(0.5);
                    }
                }
            }
        },

        getHuTipPos: function (pos) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                var huSize = cc.size(203, 119);
                begin = cc.p(215 + huSize.width / 2, 130 + huSize.height / 2);
            }
            else if (pos == 1) {
                var huSize = cc.size(117, 154);
                begin = cc.p(size.width - 200 - huSize.width / 2, 160 + huSize.height / 2);
            }
            else if (pos == 2) {
                var huSize = cc.size(203, 119);
                begin = cc.p(size.width - 220 - huSize.width / 2, size.height - 85 - huSize.height / 2);
            }
            else if (pos == 3) {
                var huSize = cc.size(117, 154);
                begin = cc.p(200 + huSize.width / 2, size.height - 170 - huSize.height / 2);
            }
            return begin;
        },

        getNiaoTipsPos:function (pos) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                var huSize = cc.size(203, 88);
                begin = cc.p(200 + huSize.width / 2, 136 + huSize.height / 2);
            }
            else if (pos == 1) {
                var huSize = cc.size(82, 154);
                begin = cc.p(size.width - 210 - huSize.width / 2, 140 + huSize.height / 2);
            }
            else if (pos == 2) {
                var huSize = cc.size(203, 88);
                begin = cc.p(size.width - 180 - huSize.width / 2, size.height - 78 - huSize.height / 2);
            }
            else if (pos == 3) {
                var huSize = cc.size(82, 154);
                begin = cc.p(210 + huSize.width / 2, size.height - 122 - huSize.height / 2);
            }
            return begin;
        },

        getZhongMaTipPos1: function (pos) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                var huSize = cc.size(203, 119);
                begin = cc.p(215 + huSize.width / 2, 130 + huSize.height / 2 );
            }
            else if (pos == 1) {
                var huSize = cc.size(117, 154);
                begin = cc.p(size.width - 200 - huSize.width / 2, 160 + huSize.height / 2);
            }
            else if (pos == 2) {
                var huSize = cc.size(203, 119);
                begin = cc.p(size.width - 220 - huSize.width / 2 + 45, size.height - 85 - huSize.height / 2);
            }
            else if (pos == 3) {
                var huSize = cc.size(117, 154);
                begin = cc.p(200 + huSize.width / 2, size.height - 170 - huSize.height / 2);
            }
            return begin;
        },

        getZhongMaTipPos2: function (pos) {
            var begin = cc.p(0, 0);
            var size = cc.director.getWinSize();

            if (pos == 0) {
                var huSize = cc.size(203, 119);
                begin = cc.p(215 + huSize.width / 2 - 40, 130 + huSize.height / 2 + 50);
            }
            else if (pos == 1) {
                var huSize = cc.size(117, 154);
                begin = cc.p(size.width - 200 - huSize.width / 2 - 35, 160 + huSize.height / 2 + 5);
            }
            else if (pos == 2) {
                var huSize = cc.size(203, 119);
                begin = cc.p(size.width - 220 - huSize.width / 2 + 45, size.height - 85 - huSize.height / 2 - 35);
            }
            else if (pos == 3) {
                var huSize = cc.size(117, 154);
                begin = cc.p(200 + huSize.width / 2 + 35, size.height - 170 - huSize.height / 2 + 25);
            }
            return begin;
        },

        getOperatorTipPos: function (pos) {
            var begin;
            var size = cc.director.getWinSize();
            if (pos == 0) {
                begin = cc.p(size.width / 2, 206);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 275, 383);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, 521);
            }
            else if (pos == 3) {
                begin = cc.p(275, 383);
            }
            return begin;
        },

        getBaoTingPos: function (index) {
            var begin;
            var headPos = this.getPlayerPos(index);
            var headSize = cc.size(95, 112);
            var tingSize = cc.size(50, 49);

            begin = cc.p(headPos.x - headSize.width / 2 + tingSize.width / 2, headPos.y + headSize.height / 2 - tingSize.height / 2 + 5);
            return begin;
        },

        getFengWeiPos: function (pos) {
            var begin;
            var size = cc.director.getWinSize();
            if (pos == 0) {
                begin = cc.p(size.width / 2, 219);
            }
            else if (pos == 1) {
                begin = cc.p(size.width - 320, 417);
            }
            else if (pos == 2) {
                begin = cc.p(size.width / 2, 539);
            }
            else if (pos == 3) {
                begin = cc.p(320, 417);
            }
            return begin;
        },

        getRotation: function (pos, type) {

            if (type == CardType.Card_Outing) {
                if (pos == 1) {
                    return 270;
                } else if (pos == 2) {
                    return 180;
                } else if (pos == 3) {
                    return 90;
                } else {
                    return 0;
                }
            } else {
                if (pos == 2 || pos == 3) {
                    return 180;
                } else {
                    return 0;
                }
            }

        },

        getScale: function (pos, type) {
            if (type == CardType.Card_Outing) {
                return 0.86;
            } else {
                if (pos == 0) {
                    if (type == CardType.Card_Peng || type == CardType.Card_End) {
                        if (MJModel.seatid != -1) {
                            return 0.64;
                        } else {
                            return 0.68;
                        }
                    } else if (type == CardType.Card_Out) {
                        return 0.5;
                    } else if (type == CardType.Card_Hu) {
                        return 0.38;
                    } else {
                        if (MJModel.seatid != -1) {
                            return 1;
                        } else {
                            return 0.86;
                        }

                    }

                } else if (pos == 2) {
                    if (type == CardType.Card_Hu) {
                        return 0.38;
                    } else {
                        return 0.5;
                    }
                } else {
                    if (type == CardType.Card_Hu) {
                        return 0.72;
                    } else {
                        return 1;
                    }
                }

            }

        },

        getCardValueStr: function (value, pos, type) {
            if (pos == 0 || pos == 2 || type == CardType.Card_Outing) {
                return getResPath("RoomMJ2/card_value/card_0_") + Utils.toHex2(value) + ".png";
            } else {
                return getResPath("RoomMJ2/card_value/card_1_") + Utils.toHex2(value) + ".png";
            }

        }
    };