var RoomZJHConfig = {

    tag_return: 100,
    tag_help: 101,
    tag_setting: 102,
    tag_chat: 103,
    tag_pot_btn: 104,
    tag_pot_nums: 105,
    tag_ready: 106,
    tag_shop: 107,
    tag_uplist: 108,
    tag_compare: 109,
    tag_see: 110,
    tag_call: 111,
    tag_fold: 112,
    tag_raise: 113,
    tag_allin: 114,
    tag_callall: 115,
    tag_max: 116,
    tag_show_card: 117,
    tag_up_table: 118,
    tag_laba_btn: 119,
    tag_back_return: 120,
    tag_back_swtich: 121,
    tag_back_cancel: 122,

    tag_balance_return: 123,
    tag_balance_swtich: 124,
    tag_balance_ready: 125,
    tag_call_money: 126,
    tag_bet_bg: 127,
    tag_My_Mul_select_bg: 128,
    tag_My_bet_tab: 129,
    tag_myinfo_bg: 130,
    tag_myinfo_vip: 131,
    tag_myinfo_money_nums: 132,
    tag_myinfo_rmb_nums: 133,
    tag_myinfo_money_add: 134,
    tag_myinfo_rmb_add: 135,
    tag_outside: 136,
    tag_my_info_btn: 137,
    tag_periphery_bet_flag: 138,
    tag_chat_bg: 139,
    tag_total_money: 140,
    tag_bet_tip: 141,
    tag_Ec_btn: 142,

    tag_My_tab_bet_btn: 400,
    tag_My_tab_bet_nums: 410,
    tag_cardType_Tips: 420,
    tag_mybet_label_begin: 500,
    tag_mybet_icon_begin: 510,
    tag_tagBetBegin: 520,
    tag_player_info_btn: 530,
    tag_preplayers_sitdown_btn: 600,
    tag_card_begin: 1000,
    tag_card_douniu_cardtype: 1050,
    tag_card_douniu_cardtype_win: 1100,
    tag_card_common_bg: 1150,
    tag_card_common: 1200,
    tag_card_flag_fold_begin: 1250,
    tag_card_flag_see_begin: 1260,
    tag_card_flag_lose_begin: 1270,

    getTablePos: function () {
        var size = cc.director.getWinSize();
        return cc.p(size.width / 2, size.height / 2 + 15);
    },

    getCMCard: function () {
        var begin = this.getTablePos();
        var pos = cc.p(begin.x - 100, begin.y - 15);
        return pos;
    },

    getPotPos: function () {
        var size = cc.director.getWinSize();
        var begin = cc.p(80, size.height - 50);
        return begin;

    },

    getTimePos: function () {
        var size = cc.director.getWinSize();
        var begin = cc.p(size.width / 2, 600);
        return begin;

    },

    getMyAvatarPos: function () {
        return cc.p(75, 71);
    },

    getMyAvatarPos1: function () {
        return cc.p(25, 5);
    },
    getOutSidePos: function () {
        return cc.p(70, 200);
    },

    getUpTablePos: function () {
        var size = cc.director.getWinSize();
        return cc.p(size.width - 70, 200);
    },

    getPlayerPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getTablePos();
        if (index == 0) {
            begin = cc.p(begin.x, begin.y - 320);
        }
        else if (index == 1) {
            begin = cc.p(begin.x + 280, begin.y - 60);
        }
        else if (index == 2) {
            begin = cc.p(begin.x + 280, begin.y + 250);
        }
        else if (index == 3) {
            begin = cc.p(begin.x - 280, begin.y + 250);
        }
        else if (index == 4) {
            begin = cc.p(begin.x - 280, begin.y - 60);
        }
        return begin;
    },


    getPerPlayerPos: function (index) {

        var begin = this.getTablePos();
        var x = -300;
        var dx = 71.6;
        var y = 210;
        if (index == 0) {
            begin = cc.p(begin.x + x, begin.y + y);
        }
        else if (index == 1) {
            begin = cc.p(begin.x + x + (index) * dx, begin.y + y);

        }
        else if (index == 2) {
            begin = cc.p(begin.x + x + (index) * dx, begin.y + y);
        }
        else if (index == 3) {
            begin = cc.p(begin.x + x + 235 + (index) * dx, begin.y + y);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + x + 235 + (index) * dx, begin.y + y);
        }
        else if (index == 5) {
            begin = cc.p(begin.x + x + 235 + (index) * dx, begin.y + y);
        }
        return begin;
    },

    getUpPlayerPos: function (index) {
        var begin = this.getTablePos();
        if (index == 0) {
            begin = cc.p(begin.x, begin.y + 230);
        }
        else if (index == 1) {
            begin = cc.p(begin.x - 79, begin.y + 205);
        }
        else if (index == 2) {
            begin = cc.p(begin.x + 79, begin.y + 205);
        }
        return begin;
    },

    getBetPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x - 55, begin.y + 5);
        }
        else if (index == 1) {
            begin = cc.p(begin.x, begin.y);
        }
        else if (index == 2) {
            begin = cc.p(begin.x, begin.y);
        }
        else if (index == 3) {
            begin = cc.p(begin.x, begin.y);
        }
        else if (index == 4) {
            begin = cc.p(begin.x, begin.y);
        }
        return begin;
    },

    getOtherBetPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getTablePos();
        if (index == 0) {
            begin = cc.p(begin.x, begin.y - 30)
        }
        else if (index == 1) {
            begin = cc.p(begin.x + 220, begin.y - 59)
        }
        else if (index == 2) {
            begin = cc.p(begin.x + 180, begin.y + 100)
        }
        else if (index == 3) {
            begin = cc.p(begin.x - 180, begin.y + 100)
        }
        else if (index == 4) {
            begin = cc.p(begin.x - 220, begin.y - 59)
        }
        return begin;
    },

    getMyBetPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x + 102, begin.y + 60);
        }
        else if (index == 1) {
            begin = cc.p(begin.x + 50, begin.y + 55);
        }
        else if (index == 2) {
            begin = cc.pAdd(begin, cc.p(50, 55));
            begin = cc.p(begin.x + 50, begin.y + 55);
        }
        else if (index == 3) {
            begin = cc.p(begin.x - 50, begin.y + 55);
        }
        else if (index == 4) {
            begin = cc.p(begin.x - 50, begin.y + 55);
        }
        return begin;
    },
    getPlayerCenterPos: function (index) {
        return this.getPlayerPos(index);
    },

    getOtherPlayerBetPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x, begin.y - 20);
        }
        else {
            begin = cc.p(begin.x - 35, begin.y - 80);
        }
        return begin;
    },

    getTablePlayerBetPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x + 130, begin.y + 67);
        }
        else if (index == 1) {
            begin = cc.pAdd(begin, cc.p(-60, 45));
            begin = cc.p(begin.x - 60, begin.y + 45);
        }
        else if (index == 2) {
            begin = cc.p(begin.x - 60, begin.y + 15);
        }
        else if (index == 3) {
            begin = cc.p(begin.x + 60, begin.y + 15);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + 60, begin.y + 45);
        }
        return begin;
    },
    getUserInfoPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x + 105, begin.y + 60);
        }
        else if (index == 1) {
            begin = cc.p(begin.x - 45, begin.y + 55);
        }
        else if (index == 2) {
            begin = cc.p(begin.x - 45, begin.y + 55);
        }
        else if (index == 3) {
            begin = cc.pAdd(begin, cc.p(45, 55));
            begin = cc.p(begin.x + 45, begin.y + 55);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + 45, begin.y + 55);
        }
        return begin;
    },

    getCardTypeTips: function (index) {
        var size = cc.director.getWinSize();
        var x = -33;

        var begin = this.getCardPos(index, 0);
        begin = cc.p(begin.x + x, begin.y - 42);
        return begin;
    },

    getCardPos: function (pos, index, card_nums) {
        if (card_nums == undefined) {
            card_nums = 3;
        }

        var begin = this.getPlayerPos(pos);
        var deltaX = 24;//每张牌间距离
        var offsetX = 115;//中间卡牌离头像距离

        if (pos == 0) {

            begin = cc.p(begin.x + (index - 1) * 60, begin.y - 145);
        }
        else if (pos == 1 || pos == 2) {
            begin = cc.p(begin.x - offsetX + (index - 1) * deltaX, begin.y);
        }
        else if (pos == 3 || pos == 4) {
            begin = cc.p(begin.x + offsetX + (index - 1) * deltaX, begin.y);
        }
        return begin;
    },

    getFlagPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.pAdd(begin, cc.p(110, 65));
            begin = cc.p(begin.x + 110, begin.y + 65);
        }
        else if (index == 1) {
            begin = cc.p(begin.x - 110, begin.y + 65);
        }
        else if (index == 2) {
            begin = cc.p(begin.x - 110, begin.y + 65);
        }
        else if (index == 3) {
            begin = cc.p(begin.x + 110, begin.y + 65);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + 110, begin.y + 65);
        }
        return begin;

    },

    getChatPos: function (index) {
        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x, begin.y + 80);
        }
        else if (index == 1) {
            begin = cc.p(begin.x - 55, begin.y + 40);
        }
        else if (index == 2) {
            begin = cc.p(begin.x - 55, begin.y + 40);
        }
        else if (index == 3) {
            begin = cc.p(begin.x + 55, begin.y + 40);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + 55, begin.y + 40);
        }
        return begin;
    },

    getComparePos: function (index) {

        var size = cc.director.getWinSize();

        var begin = this.getPlayerPos(index);
        if (index == 0) {
            begin = cc.p(begin.x + 168, begin.y);
        }
        else if (index == 1) {
            begin = cc.p(begin.x - 100, begin.y - 10);
        }
        else if (index == 2) {
            begin = cc.p(begin.x - 100, begin.y - 40);
        }
        else if (index == 3) {
            begin = cc.p(begin.x + 100, begin.y - 40);
        }
        else if (index == 4) {
            begin = cc.p(begin.x + 100, begin.y - 10);
        }
        return begin;
    }

};