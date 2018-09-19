#include "js_wb_auto.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "js.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
JSClass  *jsb_PlatformHelper_class;
JSObject *jsb_PlatformHelper_prototype;

bool js_wb_PlatformHelper_loadInviteCode(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::loadInviteCode();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_loadInviteCode : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getStoreChainData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::map<std::string, std::string> ret = PlatformHelper::getStoreChainData();
        jsval jsret = JSVAL_NULL;
        jsret = std_map_string_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getStoreChainData : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_openUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_openUrl : Error processing arguments");

        bool ret = PlatformHelper::openUrl(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_openUrl : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_showToast(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_showToast : Error processing arguments");
        PlatformHelper::showToast(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_showToast : Error processing arguments");
        PlatformHelper::showToast(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_showToast : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_openSavedPhotosAlbum(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::openSavedPhotosAlbum();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_openSavedPhotosAlbum : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_sdkDoForInt_C(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForInt_C : Error processing arguments");

        int ret = PlatformHelper::sdkDoForInt_C(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForInt_C : Error processing arguments");

        int ret = PlatformHelper::sdkDoForInt_C(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_sdkDoForInt_C : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_loadCode(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::loadCode();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_loadCode : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_weiXin_sysInfo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::weiXin_sysInfo();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_weiXin_sysInfo : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_cConViewV(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::cConViewV();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_cConViewV : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_showOpenLocationDialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::showOpenLocationDialog();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_showOpenLocationDialog : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getStringByPasteBord(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::getStringByPasteBord();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getStringByPasteBord : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_openCamera(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::openCamera();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_openCamera : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_sdkDoForVoid_C(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForVoid_C : Error processing arguments");
        PlatformHelper::sdkDoForVoid_C(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForVoid_C : Error processing arguments");
        PlatformHelper::sdkDoForVoid_C(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_sdkDoForVoid_C : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_IOSPay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 3) {
        std::string arg0;
        double arg1 = 0;
        int arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_IOSPay : Error processing arguments");
        PlatformHelper::IOSPay(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_IOSPay : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_iphoneTypeName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::iphoneTypeName();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_iphoneTypeName : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_askExitGame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::askExitGame();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_askExitGame : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_checkIsInstallWeiXin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        bool ret = PlatformHelper::checkIsInstallWeiXin();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_checkIsInstallWeiXin : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeFriendsCircleShareURL(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 5) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        const char* arg2 = nullptr;
        const char* arg3 = nullptr;
        std::string arg4;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        std::string arg2_tmp; ok &= jsval_to_std_string(cx, args.get(2), &arg2_tmp); arg2 = arg2_tmp.c_str();
        std::string arg3_tmp; ok &= jsval_to_std_string(cx, args.get(3), &arg3_tmp); arg3 = arg3_tmp.c_str();
        ok &= jsval_to_std_string(cx, args.get(4), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeFriendsCircleShareURL : Error processing arguments");
        PlatformHelper::WeFriendsCircleShareURL(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeFriendsCircleShareURL : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_registWeiXinLoginId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::registWeiXinLoginId();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_registWeiXinLoginId : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_removeIosUpdateTip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::removeIosUpdateTip();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_removeIosUpdateTip : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_loadCodeNew(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::loadCodeNew();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_loadCodeNew : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_iphoneVersion(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::iphoneVersion();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_iphoneVersion : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeChatShareText(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeChatShareText : Error processing arguments");
        PlatformHelper::WeChatShareText(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeChatShareText : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_vibrate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::vibrate();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_vibrate : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getUserAddress(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::map<std::string, std::string> ret = PlatformHelper::getUserAddress();
        jsval jsret = JSVAL_NULL;
        jsret = std_map_string_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getUserAddress : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_cConViewH(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::cConViewH();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_cConViewH : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getPowStateSig(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::getPowStateSig();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getPowStateSig : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeFriendsCircleShare(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 4) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        const char* arg2 = nullptr;
        const char* arg3 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        std::string arg2_tmp; ok &= jsval_to_std_string(cx, args.get(2), &arg2_tmp); arg2 = arg2_tmp.c_str();
        std::string arg3_tmp; ok &= jsval_to_std_string(cx, args.get(3), &arg3_tmp); arg3 = arg3_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeFriendsCircleShare : Error processing arguments");
        PlatformHelper::WeFriendsCircleShare(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeFriendsCircleShare : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_stopRecord(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::stopRecord();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_stopRecord : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_openPhotoLibrary(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::openPhotoLibrary();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_openPhotoLibrary : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getGPSInfo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::getGPSInfo();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getGPSInfo : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeChatFrendsCircleShareImage(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeChatFrendsCircleShareImage : Error processing arguments");
        PlatformHelper::WeChatFrendsCircleShareImage(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeChatFrendsCircleShareImage : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_encryptWithString(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_encryptWithString : Error processing arguments");

        const char* ret = PlatformHelper::encryptWithString(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = c_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_encryptWithString : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_IOSPayClearId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_IOSPayClearId : Error processing arguments");
        PlatformHelper::IOSPayClearId(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_IOSPayClearId : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeChatShareImage(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeChatShareImage : Error processing arguments");
        PlatformHelper::WeChatShareImage(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeChatShareImage : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_playRecord(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_playRecord : Error processing arguments");
        PlatformHelper::playRecord(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_playRecord : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_startRecord(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::startRecord();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_startRecord : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_isPlaying(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        bool ret = PlatformHelper::isPlaying();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_isPlaying : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_phoneNetInfo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::map<std::string, std::string> ret = PlatformHelper::phoneNetInfo();
        jsval jsret = JSVAL_NULL;
        jsret = std_map_string_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_phoneNetInfo : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getPhoneStateSig(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::getPhoneStateSig();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getPhoneStateSig : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_setPvrEncryptionKey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 4) {
        unsigned int arg0 = 0;
        unsigned int arg1 = 0;
        unsigned int arg2 = 0;
        unsigned int arg3 = 0;
        ok &= jsval_to_uint32(cx, args.get(0), &arg0);
        ok &= jsval_to_uint32(cx, args.get(1), &arg1);
        ok &= jsval_to_uint32(cx, args.get(2), &arg2);
        ok &= jsval_to_uint32(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_setPvrEncryptionKey : Error processing arguments");
        PlatformHelper::setPvrEncryptionKey(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_setPvrEncryptionKey : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_WeChatShare(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 4) {
        const char* arg0 = nullptr;
        const char* arg1 = nullptr;
        const char* arg2 = nullptr;
        const char* arg3 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        std::string arg2_tmp; ok &= jsval_to_std_string(cx, args.get(2), &arg2_tmp); arg2 = arg2_tmp.c_str();
        std::string arg3_tmp; ok &= jsval_to_std_string(cx, args.get(3), &arg3_tmp); arg3 = arg3_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_WeChatShare : Error processing arguments");
        PlatformHelper::WeChatShare(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_WeChatShare : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_sdkDoForStr_C(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForStr_C : Error processing arguments");

        std::string ret = PlatformHelper::sdkDoForStr_C(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_sdkDoForStr_C : Error processing arguments");

        std::string ret = PlatformHelper::sdkDoForStr_C(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_sdkDoForStr_C : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getDistanceByCoordinate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        cocos2d::Point arg0;
        cocos2d::Point arg1;
        ok &= jsval_to_ccpoint(cx, args.get(0), &arg0);
        ok &= jsval_to_ccpoint(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_getDistanceByCoordinate : Error processing arguments");

        double ret = PlatformHelper::getDistanceByCoordinate(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = DOUBLE_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getDistanceByCoordinate : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_weiXinLoad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 0) {
        PlatformHelper::weiXinLoad();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_weiXinLoad : Error processing arguments");
        PlatformHelper::weiXinLoad(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_weiXinLoad : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_callPhone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_callPhone : Error processing arguments");
        PlatformHelper::callPhone(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_callPhone : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_exitWithoutParam(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::exitWithoutParam();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_exitWithoutParam : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_getNetWorkType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        std::string ret = PlatformHelper::getNetWorkType();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_getNetWorkType : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_saveStoreChainData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 5) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_std_string(cx, args.get(4), &arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_PlatformHelper_saveStoreChainData : Error processing arguments");
        PlatformHelper::saveStoreChainData(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_saveStoreChainData : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_removeIosLocationTip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::removeIosLocationTip();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_removeIosLocationTip : wrong number of arguments");
    return false;
}

bool js_wb_PlatformHelper_clearStoreChainData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        PlatformHelper::clearStoreChainData();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_PlatformHelper_clearStoreChainData : wrong number of arguments");
    return false;
}


void js_register_wb_PlatformHelper(JSContext *cx, JS::HandleObject global) {
    jsb_PlatformHelper_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_PlatformHelper_class->name = "PlatformHelper";
    jsb_PlatformHelper_class->addProperty = JS_PropertyStub;
    jsb_PlatformHelper_class->delProperty = JS_DeletePropertyStub;
    jsb_PlatformHelper_class->getProperty = JS_PropertyStub;
    jsb_PlatformHelper_class->setProperty = JS_StrictPropertyStub;
    jsb_PlatformHelper_class->enumerate = JS_EnumerateStub;
    jsb_PlatformHelper_class->resolve = JS_ResolveStub;
    jsb_PlatformHelper_class->convert = JS_ConvertStub;
    jsb_PlatformHelper_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("loadInviteCode", js_wb_PlatformHelper_loadInviteCode, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getStoreChainData", js_wb_PlatformHelper_getStoreChainData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("openUrl", js_wb_PlatformHelper_openUrl, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("showToast", js_wb_PlatformHelper_showToast, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("openSavedPhotosAlbum", js_wb_PlatformHelper_openSavedPhotosAlbum, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sdkDoForInt_C", js_wb_PlatformHelper_sdkDoForInt_C, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadCode", js_wb_PlatformHelper_loadCode, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("weiXin_sysInfo", js_wb_PlatformHelper_weiXin_sysInfo, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("cConViewV", js_wb_PlatformHelper_cConViewV, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("showOpenLocationDialog", js_wb_PlatformHelper_showOpenLocationDialog, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getStringByPasteBord", js_wb_PlatformHelper_getStringByPasteBord, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("openCamera", js_wb_PlatformHelper_openCamera, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sdkDoForVoid_C", js_wb_PlatformHelper_sdkDoForVoid_C, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("IOSPay", js_wb_PlatformHelper_IOSPay, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("iphoneTypeName", js_wb_PlatformHelper_iphoneTypeName, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("askExitGame", js_wb_PlatformHelper_askExitGame, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkIsInstallWeiXin", js_wb_PlatformHelper_checkIsInstallWeiXin, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeFriendsCircleShareURL", js_wb_PlatformHelper_WeFriendsCircleShareURL, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("registWeiXinLoginId", js_wb_PlatformHelper_registWeiXinLoginId, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("removeIosUpdateTip", js_wb_PlatformHelper_removeIosUpdateTip, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadCodeNew", js_wb_PlatformHelper_loadCodeNew, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("iphoneVersion", js_wb_PlatformHelper_iphoneVersion, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeChatShareText", js_wb_PlatformHelper_WeChatShareText, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("vibrate", js_wb_PlatformHelper_vibrate, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getUserAddress", js_wb_PlatformHelper_getUserAddress, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("cConViewH", js_wb_PlatformHelper_cConViewH, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPowStateSig", js_wb_PlatformHelper_getPowStateSig, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeFriendsCircleShare", js_wb_PlatformHelper_WeFriendsCircleShare, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stopRecord", js_wb_PlatformHelper_stopRecord, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("openPhotoLibrary", js_wb_PlatformHelper_openPhotoLibrary, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getGPSInfo", js_wb_PlatformHelper_getGPSInfo, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeChatFrendsCircleShareImage", js_wb_PlatformHelper_WeChatFrendsCircleShareImage, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("encryptWithString", js_wb_PlatformHelper_encryptWithString, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("IOSPayClearId", js_wb_PlatformHelper_IOSPayClearId, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeChatShareImage", js_wb_PlatformHelper_WeChatShareImage, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playRecord", js_wb_PlatformHelper_playRecord, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("startRecord", js_wb_PlatformHelper_startRecord, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isPlaying", js_wb_PlatformHelper_isPlaying, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("phoneNetInfo", js_wb_PlatformHelper_phoneNetInfo, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPhoneStateSig", js_wb_PlatformHelper_getPhoneStateSig, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPvrEncryptionKey", js_wb_PlatformHelper_setPvrEncryptionKey, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("WeChatShare", js_wb_PlatformHelper_WeChatShare, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sdkDoForStr_C", js_wb_PlatformHelper_sdkDoForStr_C, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getDistanceByCoordinate", js_wb_PlatformHelper_getDistanceByCoordinate, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("weiXinLoad", js_wb_PlatformHelper_weiXinLoad, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("callPhone", js_wb_PlatformHelper_callPhone, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("exitWithoutParam", js_wb_PlatformHelper_exitWithoutParam, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getNetWorkType", js_wb_PlatformHelper_getNetWorkType, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("saveStoreChainData", js_wb_PlatformHelper_saveStoreChainData, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("removeIosLocationTip", js_wb_PlatformHelper_removeIosLocationTip, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clearStoreChainData", js_wb_PlatformHelper_clearStoreChainData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_PlatformHelper_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_PlatformHelper_class,
        dummy_constructor<PlatformHelper>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_PlatformHelper_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "PlatformHelper"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<PlatformHelper>(cx, jsb_PlatformHelper_class, proto, JS::NullPtr());
}

JSClass  *jsb_JPacket_class;
JSObject *jsb_JPacket_prototype;

bool js_wb_JPacket_end(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JPacket_end : Invalid Native Object");
    if (argc == 0) {
        cobj->end();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JPacket_end : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_JPacket_parse(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JPacket_parse : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JPacket_parse : Error processing arguments");
        int ret = cobj->parse(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_JPacket_parse : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JPacket_tostring(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JPacket_tostring : Invalid Native Object");
    if (argc == 0) {
        std::string& ret = cobj->tostring();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_JPacket_tostring : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_JPacket_sefe_check(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JPacket_sefe_check : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->sefe_check();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_JPacket_sefe_check : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_JPacket_decodeStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JPacket_decodeStr : Error processing arguments");

        std::string ret = JPacket::decodeStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_JPacket_decodeStr : wrong number of arguments");
    return false;
}

bool js_wb_JPacket_codeStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JPacket_codeStr : Error processing arguments");

        std::string ret = JPacket::codeStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_JPacket_codeStr : wrong number of arguments");
    return false;
}

bool js_wb_JPacket_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JPacket* cobj = new (std::nothrow) JPacket();

    js_type_class_t *typeClass = js_get_type_from_native<JPacket>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "JPacket"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_JPacket_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (JPacket)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        JPacket *nobj = static_cast<JPacket *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_wb_JPacket(JSContext *cx, JS::HandleObject global) {
    jsb_JPacket_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_JPacket_class->name = "JPacket";
    jsb_JPacket_class->addProperty = JS_PropertyStub;
    jsb_JPacket_class->delProperty = JS_DeletePropertyStub;
    jsb_JPacket_class->getProperty = JS_PropertyStub;
    jsb_JPacket_class->setProperty = JS_StrictPropertyStub;
    jsb_JPacket_class->enumerate = JS_EnumerateStub;
    jsb_JPacket_class->resolve = JS_ResolveStub;
    jsb_JPacket_class->convert = JS_ConvertStub;
    jsb_JPacket_class->finalize = js_JPacket_finalize;
    jsb_JPacket_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("end", js_wb_JPacket_end, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("parse", js_wb_JPacket_parse, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("tostring", js_wb_JPacket_tostring, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sefe_check", js_wb_JPacket_sefe_check, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("decodeStr", js_wb_JPacket_decodeStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("codeStr", js_wb_JPacket_codeStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_JPacket_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_JPacket_class,
        js_wb_JPacket_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_JPacket_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "JPacket"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<JPacket>(cx, jsb_JPacket_class, proto, JS::NullPtr());
}

JSClass  *jsb_CCTcpResponse_class;
JSObject *jsb_CCTcpResponse_prototype;

bool js_wb_CCTcpResponse_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpResponse_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpResponse_parseDatta(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_parseDatta : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->parseDatta();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpResponse_parseDatta : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpResponse_get_cmd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_cmd : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->cmd);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_cmd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_cmd : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_cmd : Error processing new value");
    cobj->cmd = arg0;
    return true;
}
bool js_wb_CCTcpResponse_get_data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_data : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->data);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_data : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_data : Error processing new value");
    cobj->data = arg0;
    return true;
}
bool js_wb_CCTcpResponse_get_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_id : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->id);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_id : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_id : Error processing new value");
    cobj->id = arg0;
    return true;
}
bool js_wb_CCTcpResponse_get_seqNo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_seqNo : Invalid Native Object");

    jsval jsret = long_long_to_jsval(cx, cobj->seqNo);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_seqNo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_seqNo : Invalid Native Object");

    bool ok = true;
    long long arg0 = 0;
    ok &= jsval_to_long_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_seqNo : Error processing new value");
    cobj->seqNo = arg0;
    return true;
}
bool js_wb_CCTcpResponse_get_svrid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_svrid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->svrid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_svrid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_svrid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_svrid : Error processing new value");
    cobj->svrid = arg0;
    return true;
}
bool js_wb_CCTcpResponse_get_time(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_get_time : Invalid Native Object");

    jsval jsret = long_long_to_jsval(cx, cobj->time);
    args.rval().set(jsret);
    return true;
}
bool js_wb_CCTcpResponse_set_time(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    CCTcpResponse* cobj = (CCTcpResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpResponse_set_time : Invalid Native Object");

    bool ok = true;
    long long arg0 = 0;
    ok &= jsval_to_long_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpResponse_set_time : Error processing new value");
    cobj->time = arg0;
    return true;
}

void js_register_wb_CCTcpResponse(JSContext *cx, JS::HandleObject global) {
    jsb_CCTcpResponse_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_CCTcpResponse_class->name = "CCTcpResponse";
    jsb_CCTcpResponse_class->addProperty = JS_PropertyStub;
    jsb_CCTcpResponse_class->delProperty = JS_DeletePropertyStub;
    jsb_CCTcpResponse_class->getProperty = JS_PropertyStub;
    jsb_CCTcpResponse_class->setProperty = JS_StrictPropertyStub;
    jsb_CCTcpResponse_class->enumerate = JS_EnumerateStub;
    jsb_CCTcpResponse_class->resolve = JS_ResolveStub;
    jsb_CCTcpResponse_class->convert = JS_ConvertStub;
    jsb_CCTcpResponse_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("cmd", js_wb_CCTcpResponse_get_cmd, js_wb_CCTcpResponse_set_cmd, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("data", js_wb_CCTcpResponse_get_data, js_wb_CCTcpResponse_set_data, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("id", js_wb_CCTcpResponse_get_id, js_wb_CCTcpResponse_set_id, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("seqNo", js_wb_CCTcpResponse_get_seqNo, js_wb_CCTcpResponse_set_seqNo, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("svrid", js_wb_CCTcpResponse_get_svrid, js_wb_CCTcpResponse_set_svrid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("time", js_wb_CCTcpResponse_get_time, js_wb_CCTcpResponse_set_time, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("release", js_wb_CCTcpResponse_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("parseDatta", js_wb_CCTcpResponse_parseDatta, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_CCTcpResponse_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_CCTcpResponse_class,
        dummy_constructor<CCTcpResponse>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_CCTcpResponse_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "CCTcpResponse"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<CCTcpResponse>(cx, jsb_CCTcpResponse_class, proto, JS::NullPtr());
}

JSClass  *jsb_CCTcpClient_class;
JSObject *jsb_CCTcpClient_prototype;

bool js_wb_CCTcpClient_send_data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_send_data : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data : Error processing arguments");
        cobj->send_data(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_send_data : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_reset_response_queue(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_reset_response_queue : Invalid Native Object");
    if (argc == 0) {
        cobj->reset_response_queue();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_reset_response_queue : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_connect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_connect : Invalid Native Object");
    if (argc == 0) {
        cobj->connect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_connect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_read_res_pb2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_read_res_pb2 : Invalid Native Object");
    if (argc == 0) {
        cobj->read_res_pb2();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_read_res_pb2 : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_read_res_pb3(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_read_res_pb3 : Invalid Native Object");
    if (argc == 0) {
        cobj->read_res_pb3();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_read_res_pb3 : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_write_from_string(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_write_from_string : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_write_from_string : Error processing arguments");
        ssize_t ret = cobj->write_from_string(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = ssize_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_write_from_string : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_disconnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_disconnect : Invalid Native Object");
    if (argc == 0) {
        cobj->disconnect();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_disconnect : Error processing arguments");
        cobj->disconnect(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_disconnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_data_req_pb3(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_data_req_pb3 : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_data_req_pb3 : Error processing arguments");
        cobj->data_req_pb3(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_data_req_pb3 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_read_res_pb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_read_res_pb : Invalid Native Object");
    if (argc == 0) {
        cobj->read_res_pb();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_read_res_pb : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_disconnect_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_disconnect_req : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_disconnect_req : Error processing arguments");
        cobj->disconnect_req(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_disconnect_req : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_create_request(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_create_request : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_request : Error processing arguments");
        long long ret = cobj->create_request(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 3) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_request : Error processing arguments");
        long long ret = cobj->create_request(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 4) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        long long arg3 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_long_long(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_request : Error processing arguments");
        long long ret = cobj->create_request(arg0, arg1, arg2, arg3);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_create_request : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCTcpClient_connect_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_connect_req : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_connect_req : Error processing arguments");
        cobj->connect_req(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_connect_req : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_get(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_get : Invalid Native Object");
    if (argc == 0) {
        CCTcpResponse* ret = cobj->get();
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<CCTcpResponse>(cx, (CCTcpResponse*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_get : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_send_data_pb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_send_data_pb : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb : Error processing arguments");
        long long ret = cobj->send_data_pb(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb : Error processing arguments");
        long long ret = cobj->send_data_pb(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        int arg1 = 0;
        long long arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_long_long(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb : Error processing arguments");
        long long ret = cobj->send_data_pb(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_send_data_pb : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_reset_connect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_reset_connect : Invalid Native Object");
    if (argc == 0) {
        cobj->reset_connect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_reset_connect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_data_req_pb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_data_req_pb : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_data_req_pb : Error processing arguments");
        cobj->data_req_pb(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_data_req_pb : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_write_from_string_pb3(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_write_from_string_pb3 : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_write_from_string_pb3 : Error processing arguments");
        ssize_t ret = cobj->write_from_string_pb3(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = ssize_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_write_from_string_pb3 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_put(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_put : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_put : Error processing arguments");
        cobj->put(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_put : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_data_req_pb2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_data_req_pb2 : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_data_req_pb2 : Error processing arguments");
        cobj->data_req_pb2(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_data_req_pb2 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_set_host_port(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_set_host_port : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_set_host_port : Error processing arguments");
        cobj->set_host_port(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_set_host_port : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCTcpClient_send_data_pb3(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_send_data_pb3 : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb3 : Error processing arguments");
        long long ret = cobj->send_data_pb3(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb3 : Error processing arguments");
        long long ret = cobj->send_data_pb3(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        int arg1 = 0;
        long long arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_long_long(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb3 : Error processing arguments");
        long long ret = cobj->send_data_pb3(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_send_data_pb3 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_send_data_pb2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_send_data_pb2 : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb2 : Error processing arguments");
        long long ret = cobj->send_data_pb2(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb2 : Error processing arguments");
        long long ret = cobj->send_data_pb2(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        int arg1 = 0;
        long long arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_long_long(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_send_data_pb2 : Error processing arguments");
        long long ret = cobj->send_data_pb2(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_send_data_pb2 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_create_response(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_create_response : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        long long arg3 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_long_long(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        long long arg3 = 0;
        int arg4 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_long_long(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_create_response : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCTcpClient_isConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_isConnect : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isConnect();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_isConnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_sendCodeData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_sendCodeData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_sendCodeData : Error processing arguments");
        cobj->sendCodeData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_sendCodeData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_write_from_string_pb2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_write_from_string_pb2 : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_write_from_string_pb2 : Error processing arguments");
        ssize_t ret = cobj->write_from_string_pb2(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = ssize_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_write_from_string_pb2 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_data_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_data_req : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_data_req : Error processing arguments");
        cobj->data_req(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_data_req : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_write_from_string_pb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_write_from_string_pb : Invalid Native Object");
    if (argc == 1) {
        CCTcpRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (CCTcpRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_write_from_string_pb : Error processing arguments");
        ssize_t ret = cobj->write_from_string_pb(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = ssize_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_write_from_string_pb : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_setProtoType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_setProtoType : Invalid Native Object");
    if (argc == 1) {
        ProtoType arg0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_setProtoType : Error processing arguments");
        cobj->setProtoType(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_setProtoType : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCTcpClient_read_res(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCTcpClient* cobj = (CCTcpClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCTcpClient_read_res : Invalid Native Object");
    if (argc == 0) {
        cobj->read_res();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCTcpClient_read_res : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCTcpClient_destroyInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        CCTcpClient::destroyInstance();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_CCTcpClient_destroyInstance : wrong number of arguments");
    return false;
}

bool js_wb_CCTcpClient_sendToAll(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_sendToAll : Error processing arguments");
        CCTcpClient::sendToAll(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_CCTcpClient_sendToAll : wrong number of arguments");
    return false;
}

bool js_wb_CCTcpClient_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 0) {

        CCTcpClient* ret = CCTcpClient::getInstance();
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<CCTcpClient>(cx, (CCTcpClient*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCTcpClient_getInstance : Error processing arguments");

        CCTcpClient* ret = CCTcpClient::getInstance(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<CCTcpClient>(cx, (CCTcpClient*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_CCTcpClient_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_CCTcpClient_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    CCTcpClient* cobj = new (std::nothrow) CCTcpClient();

    js_type_class_t *typeClass = js_get_type_from_native<CCTcpClient>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "CCTcpClient"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_CCTcpClient_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (CCTcpClient)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        CCTcpClient *nobj = static_cast<CCTcpClient *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_wb_CCTcpClient(JSContext *cx, JS::HandleObject global) {
    jsb_CCTcpClient_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_CCTcpClient_class->name = "CCTcpClient";
    jsb_CCTcpClient_class->addProperty = JS_PropertyStub;
    jsb_CCTcpClient_class->delProperty = JS_DeletePropertyStub;
    jsb_CCTcpClient_class->getProperty = JS_PropertyStub;
    jsb_CCTcpClient_class->setProperty = JS_StrictPropertyStub;
    jsb_CCTcpClient_class->enumerate = JS_EnumerateStub;
    jsb_CCTcpClient_class->resolve = JS_ResolveStub;
    jsb_CCTcpClient_class->convert = JS_ConvertStub;
    jsb_CCTcpClient_class->finalize = js_CCTcpClient_finalize;
    jsb_CCTcpClient_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("send_data", js_wb_CCTcpClient_send_data, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("reset_response_queue", js_wb_CCTcpClient_reset_response_queue, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("connect", js_wb_CCTcpClient_connect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("read_res_pb2", js_wb_CCTcpClient_read_res_pb2, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("read_res_pb3", js_wb_CCTcpClient_read_res_pb3, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("write_from_string", js_wb_CCTcpClient_write_from_string, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disconnect", js_wb_CCTcpClient_disconnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("data_req_pb3", js_wb_CCTcpClient_data_req_pb3, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("read_res_pb", js_wb_CCTcpClient_read_res_pb, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disconnect_req", js_wb_CCTcpClient_disconnect_req, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("create_request", js_wb_CCTcpClient_create_request, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("connect_req", js_wb_CCTcpClient_connect_req, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("get", js_wb_CCTcpClient_get, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("send_data_pb", js_wb_CCTcpClient_send_data_pb, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("reset_connect", js_wb_CCTcpClient_reset_connect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("data_req_pb", js_wb_CCTcpClient_data_req_pb, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("write_from_string_pb3", js_wb_CCTcpClient_write_from_string_pb3, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("put", js_wb_CCTcpClient_put, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("data_req_pb2", js_wb_CCTcpClient_data_req_pb2, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("set_host_port", js_wb_CCTcpClient_set_host_port, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("send_data_pb3", js_wb_CCTcpClient_send_data_pb3, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("send_data_pb2", js_wb_CCTcpClient_send_data_pb2, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("create_response", js_wb_CCTcpClient_create_response, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isConnect", js_wb_CCTcpClient_isConnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendCodeData", js_wb_CCTcpClient_sendCodeData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("write_from_string_pb2", js_wb_CCTcpClient_write_from_string_pb2, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("data_req", js_wb_CCTcpClient_data_req, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("write_from_string_pb", js_wb_CCTcpClient_write_from_string_pb, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setProtoType", js_wb_CCTcpClient_setProtoType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("read_res", js_wb_CCTcpClient_read_res, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("destroyInstance", js_wb_CCTcpClient_destroyInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendToAll", js_wb_CCTcpClient_sendToAll, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getInstance", js_wb_CCTcpClient_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_CCTcpClient_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_CCTcpClient_class,
        js_wb_CCTcpClient_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_CCTcpClient_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "CCTcpClient"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<CCTcpClient>(cx, jsb_CCTcpClient_class, proto, JS::NullPtr());
}

JSClass  *jsb_CCHttpAgent_class;
JSObject *jsb_CCHttpAgent_prototype;

bool js_wb_CCHttpAgent_sendLogPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendLogPost : Invalid Native Object");
    if (argc == 2) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendLogPost : Error processing arguments");
        cobj->sendLogPost(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendLogPost : Error processing arguments");
        cobj->sendLogPost(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendLogPost : Error processing arguments");
        cobj->sendLogPost(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        int arg4 = 0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendLogPost : Error processing arguments");
        cobj->sendLogPost(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendLogPost : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_setImageUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_setImageUrl : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_setImageUrl : Error processing arguments");
        cobj->setImageUrl(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_setImageUrl : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendHttpUrlPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendHttpUrlPost : Invalid Native Object");
    if (argc == 2) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpUrlPost : Error processing arguments");
        cobj->sendHttpUrlPost(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpUrlPost : Error processing arguments");
        cobj->sendHttpUrlPost(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpUrlPost : Error processing arguments");
        cobj->sendHttpUrlPost(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        int arg4 = 0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpUrlPost : Error processing arguments");
        cobj->sendHttpUrlPost(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendHttpUrlPost : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_sendHttpPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendHttpPost : Invalid Native Object");
    if (argc == 2) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpPost : Error processing arguments");
        cobj->sendHttpPost(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpPost : Error processing arguments");
        cobj->sendHttpPost(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpPost : Error processing arguments");
        cobj->sendHttpPost(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        int arg4 = 0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpPost : Error processing arguments");
        cobj->sendHttpPost(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendHttpPost : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_setImageDownloadUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_setImageDownloadUrl : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_setImageDownloadUrl : Error processing arguments");
        cobj->setImageDownloadUrl(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_setImageDownloadUrl : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendPayHttpPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendPayHttpPost : Invalid Native Object");
    if (argc == 2) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendPayHttpPost : Error processing arguments");
        cobj->sendPayHttpPost(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendPayHttpPost : Error processing arguments");
        cobj->sendPayHttpPost(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendPayHttpPost : Error processing arguments");
        cobj->sendPayHttpPost(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        int arg4 = 0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendPayHttpPost : Error processing arguments");
        cobj->sendPayHttpPost(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendPayHttpPost : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent__makeMultiDir(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent__makeMultiDir : Invalid Native Object");
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent__makeMultiDir : Error processing arguments");
        cobj->_makeMultiDir(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent__makeMultiDir : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendAppStoreFinishHttpPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : Invalid Native Object");
    if (argc == 2) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : Error processing arguments");
        cobj->sendAppStoreFinishHttpPost(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : Error processing arguments");
        cobj->sendAppStoreFinishHttpPost(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : Error processing arguments");
        cobj->sendAppStoreFinishHttpPost(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::function<void (std::basic_string<char>)> arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        int arg4 = 0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_std_string(cx, args.get(3), &arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : Error processing arguments");
        cobj->sendAppStoreFinishHttpPost(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendAppStoreFinishHttpPost : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_setUidAndSkey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_setUidAndSkey : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_setUidAndSkey : Error processing arguments");
        cobj->setUidAndSkey(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_setUidAndSkey : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_popPackets(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_popPackets : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_popPackets : Error processing arguments");
        cobj->popPackets(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_popPackets : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_httpImageCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_httpImageCallback : Invalid Native Object");
    if (argc == 2) {
        cocos2d::network::HttpClient* arg0 = nullptr;
        cocos2d::network::HttpResponse* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::network::HttpClient*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::network::HttpResponse*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_httpImageCallback : Error processing arguments");
        cobj->httpImageCallback(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_httpImageCallback : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_httpImagePostCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_httpImagePostCallback : Invalid Native Object");
    if (argc == 2) {
        cocos2d::network::HttpClient* arg0 = nullptr;
        cocos2d::network::HttpResponse* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::network::HttpClient*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::network::HttpResponse*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_httpImagePostCallback : Error processing arguments");
        cobj->httpImagePostCallback(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_httpImagePostCallback : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCHttpAgent_getPacketData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_getPacketData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_getPacketData : Error processing arguments");
        std::string ret = cobj->getPacketData(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_getPacketData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendHttpImagePost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendHttpImagePost : Invalid Native Object");
    if (argc == 4) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImagePost : Error processing arguments");
        int ret = cobj->sendHttpImagePost(arg0, arg1, arg2, arg3);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 5) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        int arg4 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImagePost : Error processing arguments");
        int ret = cobj->sendHttpImagePost(arg0, arg1, arg2, arg3, arg4);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendHttpImagePost : wrong number of arguments: %d, was expecting %d", argc, 4);
    return false;
}
bool js_wb_CCHttpAgent_getUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_getUrl : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getUrl();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_getUrl : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_CCHttpAgent_deletHttpPacket(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_deletHttpPacket : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_deletHttpPacket : Error processing arguments");
        cobj->deletHttpPacket(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_deletHttpPacket : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendHttpImageWithUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendHttpImageWithUrl : Invalid Native Object");
    if (argc == 4) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImageWithUrl : Error processing arguments");
        cobj->sendHttpImageWithUrl(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        int arg4 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImageWithUrl : Error processing arguments");
        cobj->sendHttpImageWithUrl(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendHttpImageWithUrl : wrong number of arguments: %d, was expecting %d", argc, 4);
    return false;
}
bool js_wb_CCHttpAgent_setUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_setUrl : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_setUrl : Error processing arguments");
        cobj->setUrl(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_setUrl : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_setImageUploadUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_setImageUploadUrl : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_setImageUploadUrl : Error processing arguments");
        cobj->setImageUploadUrl(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_setImageUploadUrl : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_CCHttpAgent_sendHttpImage(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_sendHttpImage : Invalid Native Object");
    if (argc == 4) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImage : Error processing arguments");
        cobj->sendHttpImage(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::function<void (std::basic_string<char>)> arg3;
        int arg4 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        do {
		    if(JS_TypeOfValue(cx, args.get(3)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(3), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg3 = lambda;
		    }
		    else
		    {
		        arg3 = nullptr;
		    }
		} while(0)
		;
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_sendHttpImage : Error processing arguments");
        cobj->sendHttpImage(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_sendHttpImage : wrong number of arguments: %d, was expecting %d", argc, 4);
    return false;
}
bool js_wb_CCHttpAgent_httpCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CCHttpAgent* cobj = (CCHttpAgent *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_CCHttpAgent_httpCallback : Invalid Native Object");
    if (argc == 2) {
        cocos2d::network::HttpClient* arg0 = nullptr;
        cocos2d::network::HttpResponse* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::network::HttpClient*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::network::HttpResponse*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_CCHttpAgent_httpCallback : Error processing arguments");
        cobj->httpCallback(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_CCHttpAgent_httpCallback : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_CCHttpAgent_destroyInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        CCHttpAgent::destroyInstance();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_CCHttpAgent_destroyInstance : wrong number of arguments");
    return false;
}

bool js_wb_CCHttpAgent_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = CCHttpAgent::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<CCHttpAgent>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "CCHttpAgent"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_CCHttpAgent_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_CCHttpAgent_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    CCHttpAgent* cobj = new (std::nothrow) CCHttpAgent();

    js_type_class_t *typeClass = js_get_type_from_native<CCHttpAgent>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "CCHttpAgent"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_wb_CCHttpAgent(JSContext *cx, JS::HandleObject global) {
    jsb_CCHttpAgent_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_CCHttpAgent_class->name = "CCHttpAgent";
    jsb_CCHttpAgent_class->addProperty = JS_PropertyStub;
    jsb_CCHttpAgent_class->delProperty = JS_DeletePropertyStub;
    jsb_CCHttpAgent_class->getProperty = JS_PropertyStub;
    jsb_CCHttpAgent_class->setProperty = JS_StrictPropertyStub;
    jsb_CCHttpAgent_class->enumerate = JS_EnumerateStub;
    jsb_CCHttpAgent_class->resolve = JS_ResolveStub;
    jsb_CCHttpAgent_class->convert = JS_ConvertStub;
    jsb_CCHttpAgent_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("sendLogPost", js_wb_CCHttpAgent_sendLogPost, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setImageUrl", js_wb_CCHttpAgent_setImageUrl, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendHttpUrlPost", js_wb_CCHttpAgent_sendHttpUrlPost, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendHttpPost", js_wb_CCHttpAgent_sendHttpPost, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setImageDownloadUrl", js_wb_CCHttpAgent_setImageDownloadUrl, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendPayHttpPost", js_wb_CCHttpAgent_sendPayHttpPost, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("_makeMultiDir", js_wb_CCHttpAgent__makeMultiDir, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendAppStoreFinishHttpPost", js_wb_CCHttpAgent_sendAppStoreFinishHttpPost, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setUidAndSkey", js_wb_CCHttpAgent_setUidAndSkey, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("popPackets", js_wb_CCHttpAgent_popPackets, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("httpImageCallback", js_wb_CCHttpAgent_httpImageCallback, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("httpImagePostCallback", js_wb_CCHttpAgent_httpImagePostCallback, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_CCHttpAgent_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPacketData", js_wb_CCHttpAgent_getPacketData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendHttpImagePost", js_wb_CCHttpAgent_sendHttpImagePost, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getUrl", js_wb_CCHttpAgent_getUrl, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("deletHttpPacket", js_wb_CCHttpAgent_deletHttpPacket, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendHttpImageWithUrl", js_wb_CCHttpAgent_sendHttpImageWithUrl, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setUrl", js_wb_CCHttpAgent_setUrl, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setImageUploadUrl", js_wb_CCHttpAgent_setImageUploadUrl, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendHttpImage", js_wb_CCHttpAgent_sendHttpImage, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("httpCallback", js_wb_CCHttpAgent_httpCallback, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("destroyInstance", js_wb_CCHttpAgent_destroyInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getInstance", js_wb_CCHttpAgent_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_CCHttpAgent_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_CCHttpAgent_class,
        js_wb_CCHttpAgent_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_CCHttpAgent_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "CCHttpAgent"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<CCHttpAgent>(cx, jsb_CCHttpAgent_class, proto, JS::NullPtr());
}

JSClass  *jsb_BaseLayer_class;
JSObject *jsb_BaseLayer_prototype;

bool js_wb_BaseLayer_setMore_img(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setMore_img : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<void ()> arg2;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        do {
		    if(JS_TypeOfValue(cx, args.get(2)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(2), args.thisv()));
		        auto lambda = [=]() -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(0, nullptr, &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg2 = lambda;
		    }
		    else
		    {
		        arg2 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setMore_img : Error processing arguments");
        BaseLayer* ret = cobj->setMore_img(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setMore_img : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_BaseLayer_setHead(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setHead : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setHead : Error processing arguments");
        cobj->setHead(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setHead : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_setPreByName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setPreByName : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setPreByName : Error processing arguments");
        BaseLayer* ret = cobj->setPreByName(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setPreByName : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_setPreView(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setPreView : Invalid Native Object");
    if (argc == 1) {
        BaseLayer* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (BaseLayer*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setPreView : Error processing arguments");
        BaseLayer* ret = cobj->setPreView(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setPreView : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_removeToPreByName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_removeToPreByName : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_removeToPreByName : Error processing arguments");
        cobj->removeToPreByName(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_removeToPreByName : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_setCanReturn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setCanReturn : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setCanReturn : Error processing arguments");
        cobj->setCanReturn(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setCanReturn : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_getSpriteHeight(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_getSpriteHeight : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getSpriteHeight();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_getSpriteHeight : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_setTitle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setTitle : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setTitle : Error processing arguments");
        BaseLayer* ret = cobj->setTitle(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setTitle : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_BaseLayer_setReturn_img(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setReturn_img : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<void ()> arg2;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        do {
		    if(JS_TypeOfValue(cx, args.get(2)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(2), args.thisv()));
		        auto lambda = [=]() -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(0, nullptr, &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg2 = lambda;
		    }
		    else
		    {
		        arg2 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setReturn_img : Error processing arguments");
        BaseLayer* ret = cobj->setReturn_img(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setReturn_img : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_BaseLayer_removePre(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_removePre : Invalid Native Object");
    if (argc == 2) {
        BaseLayer* arg0 = nullptr;
        BaseLayer* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (BaseLayer*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (BaseLayer*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_removePre : Error processing arguments");
        cobj->removePre(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_removePre : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_BaseLayer_onExit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_onExit : Invalid Native Object");
    if (argc == 0) {
        cobj->onExit();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_onExit : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_setBackground(JSContext *cx, uint32_t argc, jsval *vp)
{
    bool ok = true;
    BaseLayer* cobj = nullptr;

    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx);
    obj.set(args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (BaseLayer *)(proxy ? proxy->ptr : nullptr);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setBackground : Invalid Native Object");
    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            BaseLayer* ret = cobj->setBackground(arg0);
            jsval jsret = JSVAL_NULL;
            if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            cocos2d::Color4B arg0;
            ok &= jsval_to_cccolor4b(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            BaseLayer* ret = cobj->setBackground(arg0);
            jsval jsret = JSVAL_NULL;
            if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_wb_BaseLayer_setBackground : wrong number of arguments");
    return false;
}
bool js_wb_BaseLayer_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_disappear(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_disappear : Invalid Native Object");
    if (argc == 0) {
        cobj->disappear();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_disappear : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_setReturn_txt(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setReturn_txt : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::function<void ()> arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        do {
		    if(JS_TypeOfValue(cx, args.get(1)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(1), args.thisv()));
		        auto lambda = [=]() -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(0, nullptr, &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg1 = lambda;
		    }
		    else
		    {
		        arg1 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setReturn_txt : Error processing arguments");
        BaseLayer* ret = cobj->setReturn_txt(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setReturn_txt : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_BaseLayer_setMore_txt(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_setMore_txt : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::function<void ()> arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        do {
		    if(JS_TypeOfValue(cx, args.get(1)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(1), args.thisv()));
		        auto lambda = [=]() -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(0, nullptr, &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg1 = lambda;
		    }
		    else
		    {
		        arg1 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_setMore_txt : Error processing arguments");
        BaseLayer* ret = cobj->setMore_txt(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<BaseLayer>(cx, (BaseLayer*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_setMore_txt : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_BaseLayer_onKeyReleased(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_onKeyReleased : Invalid Native Object");
    if (argc == 2) {
        cocos2d::EventKeyboard::KeyCode arg0;
        cocos2d::Event* arg1 = nullptr;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_BaseLayer_onKeyReleased : Error processing arguments");
        cobj->onKeyReleased(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_onKeyReleased : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_BaseLayer_getHeadH(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_getHeadH : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getHeadH();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_getHeadH : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_appear(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    BaseLayer* cobj = (BaseLayer *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_BaseLayer_appear : Invalid Native Object");
    if (argc == 0) {
        cobj->appear();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_BaseLayer_appear : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_BaseLayer_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = BaseLayer::create();
        js_type_class_t *typeClass = js_get_type_from_native<BaseLayer>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "BaseLayer"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_BaseLayer_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_BaseLayer(JSContext *cx, JS::HandleObject global) {
    jsb_BaseLayer_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_BaseLayer_class->name = "BaseLayer";
    jsb_BaseLayer_class->addProperty = JS_PropertyStub;
    jsb_BaseLayer_class->delProperty = JS_DeletePropertyStub;
    jsb_BaseLayer_class->getProperty = JS_PropertyStub;
    jsb_BaseLayer_class->setProperty = JS_StrictPropertyStub;
    jsb_BaseLayer_class->enumerate = JS_EnumerateStub;
    jsb_BaseLayer_class->resolve = JS_ResolveStub;
    jsb_BaseLayer_class->convert = JS_ConvertStub;
    jsb_BaseLayer_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setMore_img", js_wb_BaseLayer_setMore_img, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setHead", js_wb_BaseLayer_setHead, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPreByName", js_wb_BaseLayer_setPreByName, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPreView", js_wb_BaseLayer_setPreView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("removeToPreByName", js_wb_BaseLayer_removeToPreByName, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCanReturn", js_wb_BaseLayer_setCanReturn, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getSpriteHeight", js_wb_BaseLayer_getSpriteHeight, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setTitle", js_wb_BaseLayer_setTitle, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setReturn_img", js_wb_BaseLayer_setReturn_img, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("removePre", js_wb_BaseLayer_removePre, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onExit", js_wb_BaseLayer_onExit, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setBackground", js_wb_BaseLayer_setBackground, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_BaseLayer_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disappear", js_wb_BaseLayer_disappear, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setReturn_txt", js_wb_BaseLayer_setReturn_txt, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setMore_txt", js_wb_BaseLayer_setMore_txt, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onKeyReleased", js_wb_BaseLayer_onKeyReleased, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getHeadH", js_wb_BaseLayer_getHeadH, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("appear", js_wb_BaseLayer_appear, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_BaseLayer_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_BaseLayer_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_BaseLayer_class,
        dummy_constructor<BaseLayer>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_BaseLayer_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "BaseLayer"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<BaseLayer>(cx, jsb_BaseLayer_class, proto, parent_proto);
}

JSClass  *jsb_ZJHModel_class;
JSObject *jsb_ZJHModel_prototype;

bool js_wb_ZJHModel_matchEnterName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_matchEnterName : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->matchEnterName();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_matchEnterName : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_reLoginLoadGame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_reLoginLoadGame : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_reLoginLoadGame : Error processing arguments");
        cobj->reLoginLoadGame(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_reLoginLoadGame : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_requestPay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_requestPay : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        int arg1 = 0;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_requestPay : Error processing arguments");
        cobj->requestPay(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_requestPay : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_ZJHModel_listnerGameResum(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_listnerGameResum : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_listnerGameResum : Error processing arguments");
        cobj->listnerGameResum(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_listnerGameResum : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_setRoomData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_setRoomData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_setRoomData : Error processing arguments");
        cobj->setRoomData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_setRoomData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_requestThirdPay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_requestThirdPay : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_requestThirdPay : Error processing arguments");
        cobj->requestThirdPay(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_requestThirdPay : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_roomToShareView(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_roomToShareView : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_roomToShareView : Error processing arguments");
        cobj->roomToShareView(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_roomToShareView : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_uploadMyLocation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_uploadMyLocation : Invalid Native Object");
    if (argc == 0) {
        cobj->uploadMyLocation();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_uploadMyLocation : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_iosPurchaseResult(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_iosPurchaseResult : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        const char* arg1 = nullptr;
        const char* arg2 = nullptr;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        std::string arg1_tmp; ok &= jsval_to_std_string(cx, args.get(1), &arg1_tmp); arg1 = arg1_tmp.c_str();
        std::string arg2_tmp; ok &= jsval_to_std_string(cx, args.get(2), &arg2_tmp); arg2 = arg2_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_iosPurchaseResult : Error processing arguments");
        cobj->iosPurchaseResult(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_iosPurchaseResult : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_ZJHModel_gotoVideo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_gotoVideo : Invalid Native Object");
    if (argc == 4) {
        int arg0 = 0;
        int arg1 = 0;
        std::string arg2;
        int arg3 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoVideo : Error processing arguments");
        cobj->gotoVideo(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_gotoVideo : wrong number of arguments: %d, was expecting %d", argc, 4);
    return false;
}
bool js_wb_ZJHModel_setCurGameID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_setCurGameID : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_setCurGameID : Error processing arguments");
        cobj->setCurGameID(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_setCurGameID : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_roomHearHttpRequest(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_roomHearHttpRequest : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_roomHearHttpRequest : Error processing arguments");
        cobj->roomHearHttpRequest(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_roomHearHttpRequest : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_roomToHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_roomToHall : Invalid Native Object");
    if (argc == 0) {
        cobj->roomToHall();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_roomToHall : Error processing arguments");
        cobj->roomToHall(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_roomToHall : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_initPhoneData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_initPhoneData : Invalid Native Object");
    if (argc == 0) {
        cobj->initPhoneData();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_initPhoneData : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_setUserType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_setUserType : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_setUserType : Error processing arguments");
        cobj->setUserType(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_setUserType : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_moneyShowDeal(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_moneyShowDeal : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_moneyShowDeal : Error processing arguments");
        std::string ret = cobj->moneyShowDeal(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_moneyShowDeal : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_init : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_gotoGameLoad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_gotoGameLoad : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoGameLoad : Error processing arguments");
        cobj->gotoGameLoad(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_gotoGameLoad : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_toMatchHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_toMatchHall : Invalid Native Object");
    if (argc == 0) {
        cobj->toMatchHall();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_toMatchHall : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_converPointTwoBit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_converPointTwoBit : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_converPointTwoBit : Error processing arguments");
        std::string ret = cobj->converPointTwoBit(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_converPointTwoBit : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_dealAppUpdate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_dealAppUpdate : Invalid Native Object");
    if (argc == 0) {
        cobj->dealAppUpdate();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_dealAppUpdate : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_getRoomData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_getRoomData : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getRoomData();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_getRoomData : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_checkUploadMoblieContacts(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_checkUploadMoblieContacts : Invalid Native Object");
    if (argc == 0) {
        cobj->checkUploadMoblieContacts();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_checkUploadMoblieContacts : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_randomThirdID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_randomThirdID : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_randomThirdID : Error processing arguments");
        std::string ret = cobj->randomThirdID(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_randomThirdID : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_gotoRoomWithCode(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_gotoRoomWithCode : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoRoomWithCode : Error processing arguments");
        cobj->gotoRoomWithCode(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        bool arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoRoomWithCode : Error processing arguments");
        cobj->gotoRoomWithCode(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        bool arg1;
        std::string arg2;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        ok &= jsval_to_std_string(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoRoomWithCode : Error processing arguments");
        cobj->gotoRoomWithCode(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_gotoRoomWithCode : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_hidePhoneNum(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_hidePhoneNum : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_hidePhoneNum : Error processing arguments");
        std::string ret = cobj->hidePhoneNum(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_hidePhoneNum : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_changeScreenSize(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_changeScreenSize : Invalid Native Object");
    if (argc == 2) {
        double arg0 = 0;
        double arg1 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        ok &= JS::ToNumber( cx, args.get(1), &arg1) && !std::isnan(arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_changeScreenSize : Error processing arguments");
        cobj->changeScreenSize(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_changeScreenSize : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_toMatchRoom(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_toMatchRoom : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_toMatchRoom : Error processing arguments");
        cobj->toMatchRoom(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_toMatchRoom : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_startRoomHeart(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_startRoomHeart : Invalid Native Object");
    if (argc == 0) {
        cobj->startRoomHeart();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_startRoomHeart : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_toGameHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_toGameHall : Invalid Native Object");
    if (argc == 0) {
        cobj->toGameHall();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_toGameHall : Error processing arguments");
        cobj->toGameHall(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_toGameHall : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_noticeGamePlayEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_noticeGamePlayEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_noticeGamePlayEvent : Error processing arguments");
        cobj->noticeGamePlayEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_noticeGamePlayEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_gotoRoom(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_gotoRoom : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoRoom : Error processing arguments");
        cobj->gotoRoom(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_gotoRoom : Error processing arguments");
        cobj->gotoRoom(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_gotoRoom : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_isOpenIPV6(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_isOpenIPV6 : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isOpenIPV6();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_isOpenIPV6 : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_listnerPaySucceed(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_listnerPaySucceed : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_listnerPaySucceed : Error processing arguments");
        cobj->listnerPaySucceed(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_listnerPaySucceed : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_hallToRoomV(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_hallToRoomV : Invalid Native Object");
    if (argc == 0) {
        cobj->hallToRoomV();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_hallToRoomV : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_checkGamePlayingError(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_checkGamePlayingError : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_checkGamePlayingError : Error processing arguments");
        cobj->checkGamePlayingError(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_checkGamePlayingError : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_toHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_toHall : Invalid Native Object");
    if (argc == 0) {
        cobj->toHall();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_toHall : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_setDiquCach(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_setDiquCach : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_setDiquCach : Error processing arguments");
        cobj->setDiquCach(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_setDiquCach : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_goldCovertStr2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_goldCovertStr2 : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_goldCovertStr2 : Error processing arguments");
        std::string ret = cobj->goldCovertStr2(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_goldCovertStr2 : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_loadingImageResource(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_loadingImageResource : Invalid Native Object");
    if (argc == 2) {
        cocos2d::ui::ImageView* arg0 = nullptr;
        std::string arg1;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::ui::ImageView*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_loadingImageResource : Error processing arguments");
        cobj->loadingImageResource(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_loadingImageResource : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_isYoukeLogin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_isYoukeLogin : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isYoukeLogin();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_isYoukeLogin : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_setPhoneAndPwd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_setPhoneAndPwd : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_setPhoneAndPwd : Error processing arguments");
        cobj->setPhoneAndPwd(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_setPhoneAndPwd : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ZJHModel_isCanPay(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_isCanPay : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isCanPay();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_isCanPay : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_hotUpdateGame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_hotUpdateGame : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_hotUpdateGame : Error processing arguments");
        cobj->hotUpdateGame(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_hotUpdateGame : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_checkNoRmb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_checkNoRmb : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_checkNoRmb : Error processing arguments");
        bool ret = cobj->checkNoRmb(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_checkNoRmb : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_checkNoMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_checkNoMoney : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_checkNoMoney : Error processing arguments");
        bool ret = cobj->checkNoMoney(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_checkNoMoney : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_hallToRoom(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_hallToRoom : Invalid Native Object");
    if (argc == 0) {
        cobj->hallToRoom();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_hallToRoom : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_createTestMatchHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_createTestMatchHall : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_createTestMatchHall : Error processing arguments");
        cobj->createTestMatchHall(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_createTestMatchHall : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ZJHModel_GameResum(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_GameResum : Invalid Native Object");
    if (argc == 0) {
        cobj->GameResum();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_GameResum : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_hallToRoomH(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_hallToRoomH : Invalid Native Object");
    if (argc == 0) {
        cobj->hallToRoomH();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ZJHModel_hallToRoomH : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ZJHModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = ZJHModel::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<ZJHModel>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "ZJHModel"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_ZJHModel_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_ZJHModel_getStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_getStr : Error processing arguments");

        const char* ret = ZJHModel::getStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = c_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_ZJHModel_getStr : wrong number of arguments");
    return false;
}

bool js_wb_ZJHModel_get_count(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_count : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->count);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_count(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_count : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_count : Error processing new value");
    cobj->count = arg0;
    return true;
}
bool js_wb_ZJHModel_get_GameServer(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_GameServer : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->GameServer);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_GameServer(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_GameServer : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_GameServer : Error processing new value");
    cobj->GameServer = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isPause(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isPause : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isPause);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isPause(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isPause : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isPause : Error processing new value");
    cobj->isPause = arg0;
    return true;
}
bool js_wb_ZJHModel_get_uid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_uid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->uid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_uid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_uid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_uid : Error processing new value");
    cobj->uid = arg0;
    return true;
}
bool js_wb_ZJHModel_get_bgMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_bgMusic : Invalid Native Object");

    jsval jsret = DOUBLE_TO_JSVAL(cobj->bgMusic);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_bgMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_bgMusic : Invalid Native Object");

    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_bgMusic : Error processing new value");
    cobj->bgMusic = arg0;
    return true;
}
bool js_wb_ZJHModel_get_sound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_sound : Invalid Native Object");

    jsval jsret = DOUBLE_TO_JSVAL(cobj->sound);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_sound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_sound : Invalid Native Object");

    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_sound : Error processing new value");
    cobj->sound = arg0;
    return true;
}
bool js_wb_ZJHModel_get_vibrate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_vibrate : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vibrate);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_vibrate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_vibrate : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_vibrate : Error processing new value");
    cobj->vibrate = arg0;
    return true;
}
bool js_wb_ZJHModel_get_musicOn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_musicOn : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->musicOn);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_musicOn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_musicOn : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_musicOn : Error processing new value");
    cobj->musicOn = arg0;
    return true;
}
bool js_wb_ZJHModel_get_soundOn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_soundOn : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->soundOn);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_soundOn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_soundOn : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_soundOn : Error processing new value");
    cobj->soundOn = arg0;
    return true;
}
bool js_wb_ZJHModel_get_phoneOs(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_phoneOs : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->phoneOs);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_phoneOs(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_phoneOs : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_phoneOs : Error processing new value");
    cobj->phoneOs = arg0;
    return true;
}
bool js_wb_ZJHModel_get_thirdId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_thirdId : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->thirdId);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_thirdId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_thirdId : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_thirdId : Error processing new value");
    cobj->thirdId = arg0;
    return true;
}
bool js_wb_ZJHModel_get_UserKey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_UserKey : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->UserKey);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_UserKey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_UserKey : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_UserKey : Error processing new value");
    cobj->UserKey = arg0;
    return true;
}
bool js_wb_ZJHModel_get_vname(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_vname : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->vname);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_vname(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_vname : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_vname : Error processing new value");
    cobj->vname = arg0;
    return true;
}
bool js_wb_ZJHModel_get_vcode(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_vcode : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vcode);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_vcode(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_vcode : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_vcode : Error processing new value");
    cobj->vcode = arg0;
    return true;
}
bool js_wb_ZJHModel_get_channel_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_channel_id : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->channel_id);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_channel_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_channel_id : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_channel_id : Error processing new value");
    cobj->channel_id = arg0;
    return true;
}
bool js_wb_ZJHModel_get_agentKey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_agentKey : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->agentKey);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_agentKey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_agentKey : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_agentKey : Error processing new value");
    cobj->agentKey = arg0;
    return true;
}
bool js_wb_ZJHModel_get_phone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_phone : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->phone);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_phone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_phone : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_phone : Error processing new value");
    cobj->phone = arg0;
    return true;
}
bool js_wb_ZJHModel_get_pwd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_pwd : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->pwd);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_pwd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_pwd : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_pwd : Error processing new value");
    cobj->pwd = arg0;
    return true;
}
bool js_wb_ZJHModel_get_pwdOK(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_pwdOK : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->pwdOK);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_pwdOK(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_pwdOK : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_pwdOK : Error processing new value");
    cobj->pwdOK = arg0;
    return true;
}
bool js_wb_ZJHModel_get_nick(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_nick : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->nick);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_nick(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_nick : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_nick : Error processing new value");
    cobj->nick = arg0;
    return true;
}
bool js_wb_ZJHModel_get_signature(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_signature : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->signature);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_signature(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_signature : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_signature : Error processing new value");
    cobj->signature = arg0;
    return true;
}
bool js_wb_ZJHModel_get_vipLv(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_vipLv : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vipLv);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_vipLv(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_vipLv : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_vipLv : Error processing new value");
    cobj->vipLv = arg0;
    return true;
}
bool js_wb_ZJHModel_get_portrit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_portrit : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->portrit);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_portrit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_portrit : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_portrit : Error processing new value");
    cobj->portrit = arg0;
    return true;
}
bool js_wb_ZJHModel_get_userType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_userType : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->userType);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_userType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_userType : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_userType : Error processing new value");
    cobj->userType = arg0;
    return true;
}
bool js_wb_ZJHModel_get_userCity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_userCity : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->userCity);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_userCity(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_userCity : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_userCity : Error processing new value");
    cobj->userCity = arg0;
    return true;
}
bool js_wb_ZJHModel_get_provice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_provice : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->provice);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_provice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_provice : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_provice : Error processing new value");
    cobj->provice = arg0;
    return true;
}
bool js_wb_ZJHModel_get_city(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_city : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->city);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_city(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_city : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_city : Error processing new value");
    cobj->city = arg0;
    return true;
}
bool js_wb_ZJHModel_get_qu(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_qu : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->qu);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_qu(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_qu : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_qu : Error processing new value");
    cobj->qu = arg0;
    return true;
}
bool js_wb_ZJHModel_get_weiXinBangPhone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_weiXinBangPhone : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->weiXinBangPhone);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_weiXinBangPhone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_weiXinBangPhone : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_weiXinBangPhone : Error processing new value");
    cobj->weiXinBangPhone = arg0;
    return true;
}
bool js_wb_ZJHModel_get_NOPwdSetPwd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_NOPwdSetPwd : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->NOPwdSetPwd);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_NOPwdSetPwd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_NOPwdSetPwd : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_NOPwdSetPwd : Error processing new value");
    cobj->NOPwdSetPwd = arg0;
    return true;
}
bool js_wb_ZJHModel_get_versionIOS(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_versionIOS : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->versionIOS);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_versionIOS(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_versionIOS : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_versionIOS : Error processing new value");
    cobj->versionIOS = arg0;
    return true;
}
bool js_wb_ZJHModel_get_phoneimei(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_phoneimei : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->phoneimei);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_phoneimei(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_phoneimei : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_phoneimei : Error processing new value");
    cobj->phoneimei = arg0;
    return true;
}
bool js_wb_ZJHModel_get_phonename(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_phonename : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->phonename);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_phonename(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_phonename : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_phonename : Error processing new value");
    cobj->phonename = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isTrait(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isTrait : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isTrait);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isTrait(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isTrait : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isTrait : Error processing new value");
    cobj->isTrait = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gameHelpUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gameHelpUrl : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->gameHelpUrl);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gameHelpUrl(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gameHelpUrl : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gameHelpUrl : Error processing new value");
    cobj->gameHelpUrl = arg0;
    return true;
}
bool js_wb_ZJHModel_get_source_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_source_id : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->source_id);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_source_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_source_id : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_source_id : Error processing new value");
    cobj->source_id = arg0;
    return true;
}
bool js_wb_ZJHModel_get_user(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_user : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->user);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_user(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_user : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_user : Error processing new value");
    cobj->user = arg0;
    return true;
}
bool js_wb_ZJHModel_get_name(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_name : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->name);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_name(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_name : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_name : Error processing new value");
    cobj->name = arg0;
    return true;
}
bool js_wb_ZJHModel_get_sex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_sex : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->sex);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_sex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_sex : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_sex : Error processing new value");
    cobj->sex = arg0;
    return true;
}
bool js_wb_ZJHModel_get_avatar(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_avatar : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->avatar);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_avatar(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_avatar : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_avatar : Error processing new value");
    cobj->avatar = arg0;
    return true;
}
bool js_wb_ZJHModel_get_money(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_money : Invalid Native Object");

    jsval jsret = long_to_jsval(cx, cobj->money);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_money(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_money : Invalid Native Object");

    bool ok = true;
    long arg0 = 0;
    ok &= jsval_to_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_money : Error processing new value");
    cobj->money = arg0;
    return true;
}
bool js_wb_ZJHModel_get_rmb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_rmb : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->rmb);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_rmb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_rmb : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_rmb : Error processing new value");
    cobj->rmb = arg0;
    return true;
}
bool js_wb_ZJHModel_get_coin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_coin : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->coin);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_coin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_coin : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_coin : Error processing new value");
    cobj->coin = arg0;
    return true;
}
bool js_wb_ZJHModel_get_skey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_skey : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->skey);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_skey(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_skey : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_skey : Error processing new value");
    cobj->skey = arg0;
    return true;
}
bool js_wb_ZJHModel_get_dsc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_dsc : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->dsc);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_dsc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_dsc : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_dsc : Error processing new value");
    cobj->dsc = arg0;
    return true;
}
bool js_wb_ZJHModel_get_exp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_exp : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->exp);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_exp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_exp : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_exp : Error processing new value");
    cobj->exp = arg0;
    return true;
}
bool js_wb_ZJHModel_get_total_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_total_win : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->total_win);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_total_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_total_win : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_total_win : Error processing new value");
    cobj->total_win = arg0;
    return true;
}
bool js_wb_ZJHModel_get_total_board(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_total_board : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->total_board);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_total_board(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_total_board : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_total_board : Error processing new value");
    cobj->total_board = arg0;
    return true;
}
bool js_wb_ZJHModel_get_curRoomID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_curRoomID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curRoomID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_curRoomID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_curRoomID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_curRoomID : Error processing new value");
    cobj->curRoomID = arg0;
    return true;
}
bool js_wb_ZJHModel_get_vlevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_vlevel : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vlevel);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_vlevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_vlevel : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_vlevel : Error processing new value");
    cobj->vlevel = arg0;
    return true;
}
bool js_wb_ZJHModel_get_mySeatid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_mySeatid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->mySeatid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_mySeatid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_mySeatid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_mySeatid : Error processing new value");
    cobj->mySeatid = arg0;
    return true;
}
bool js_wb_ZJHModel_get_myPoint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_myPoint : Invalid Native Object");

    jsval jsret = vector2_to_jsval(cx, cobj->myPoint);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_myPoint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_myPoint : Invalid Native Object");

    bool ok = true;
    cocos2d::Vec2 arg0;
    ok &= jsval_to_vector2(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_myPoint : Error processing new value");
    cobj->myPoint = arg0;
    return true;
}
bool js_wb_ZJHModel_get_myBet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_myBet : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->myBet);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_myBet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_myBet : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_myBet : Error processing new value");
    cobj->myBet = arg0;
    return true;
}
bool js_wb_ZJHModel_get_pauseTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_pauseTime : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->pauseTime);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_pauseTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_pauseTime : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_pauseTime : Error processing new value");
    cobj->pauseTime = arg0;
    return true;
}
bool js_wb_ZJHModel_get_enterGame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_enterGame : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->enterGame);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_enterGame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_enterGame : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_enterGame : Error processing new value");
    cobj->enterGame = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gamePuase(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gamePuase : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->gamePuase);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gamePuase(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gamePuase : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gamePuase : Error processing new value");
    cobj->gamePuase = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gamehasinit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gamehasinit : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->gamehasinit);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gamehasinit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gamehasinit : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gamehasinit : Error processing new value");
    cobj->gamehasinit = arg0;
    return true;
}
bool js_wb_ZJHModel_get_roomUserID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_roomUserID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->roomUserID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_roomUserID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_roomUserID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_roomUserID : Error processing new value");
    cobj->roomUserID = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isGameLoad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isGameLoad : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isGameLoad);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isGameLoad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isGameLoad : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isGameLoad : Error processing new value");
    cobj->isGameLoad = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isReplace(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isReplace : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isReplace);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isReplace(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isReplace : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isReplace : Error processing new value");
    cobj->isReplace = arg0;
    return true;
}
bool js_wb_ZJHModel_get_recoveryHallParam(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_recoveryHallParam : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->recoveryHallParam);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_recoveryHallParam(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_recoveryHallParam : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_recoveryHallParam : Error processing new value");
    cobj->recoveryHallParam = arg0;
    return true;
}
bool js_wb_ZJHModel_get_curGameType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_curGameType : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curGameType);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_curGameType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_curGameType : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_curGameType : Error processing new value");
    cobj->curGameType = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isGamePhzFrist(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isGamePhzFrist : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isGamePhzFrist);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isGamePhzFrist(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isGamePhzFrist : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isGamePhzFrist : Error processing new value");
    cobj->isGamePhzFrist = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isOnVideo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isOnVideo : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isOnVideo);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isOnVideo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isOnVideo : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isOnVideo : Error processing new value");
    cobj->isOnVideo = arg0;
    return true;
}
bool js_wb_ZJHModel_get_roomVideoData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_roomVideoData : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->roomVideoData);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_roomVideoData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_roomVideoData : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_roomVideoData : Error processing new value");
    cobj->roomVideoData = arg0;
    return true;
}
bool js_wb_ZJHModel_get_packName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_packName : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->packName);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_packName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_packName : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_packName : Error processing new value");
    cobj->packName = arg0;
    return true;
}
bool js_wb_ZJHModel_get_IosTransactionId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_IosTransactionId : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->IosTransactionId);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_IosTransactionId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_IosTransactionId : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_IosTransactionId : Error processing new value");
    cobj->IosTransactionId = arg0;
    return true;
}
bool js_wb_ZJHModel_get_IosReceipt(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_IosReceipt : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->IosReceipt);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_IosReceipt(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_IosReceipt : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_IosReceipt : Error processing new value");
    cobj->IosReceipt = arg0;
    return true;
}
bool js_wb_ZJHModel_get_appStoreInvoice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_appStoreInvoice : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->appStoreInvoice);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_appStoreInvoice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_appStoreInvoice : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_appStoreInvoice : Error processing new value");
    cobj->appStoreInvoice = arg0;
    return true;
}
bool js_wb_ZJHModel_get_iosTotalFee(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_iosTotalFee : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->iosTotalFee);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_iosTotalFee(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_iosTotalFee : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_iosTotalFee : Error processing new value");
    cobj->iosTotalFee = arg0;
    return true;
}
bool js_wb_ZJHModel_get_appleId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_appleId : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->appleId);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_appleId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_appleId : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_appleId : Error processing new value");
    cobj->appleId = arg0;
    return true;
}
bool js_wb_ZJHModel_get_iosPrice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_iosPrice : Invalid Native Object");

    jsval jsret = DOUBLE_TO_JSVAL(cobj->iosPrice);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_iosPrice(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_iosPrice : Invalid Native Object");

    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_iosPrice : Error processing new value");
    cobj->iosPrice = arg0;
    return true;
}
bool js_wb_ZJHModel_get_serverDiffrentTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_serverDiffrentTime : Invalid Native Object");

    jsval jsret = long_long_to_jsval(cx, cobj->serverDiffrentTime);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_serverDiffrentTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_serverDiffrentTime : Invalid Native Object");

    bool ok = true;
    long long arg0 = 0;
    ok &= jsval_to_long_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_serverDiffrentTime : Error processing new value");
    cobj->serverDiffrentTime = arg0;
    return true;
}
bool js_wb_ZJHModel_get_curGameID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_curGameID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curGameID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_curGameID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_curGameID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_curGameID : Error processing new value");
    cobj->curGameID = arg0;
    return true;
}
bool js_wb_ZJHModel_get_curGameHasHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_curGameHasHall : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curGameHasHall);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_curGameHasHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_curGameHasHall : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_curGameHasHall : Error processing new value");
    cobj->curGameHasHall = arg0;
    return true;
}
bool js_wb_ZJHModel_get_firtEnterApp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_firtEnterApp : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->firtEnterApp);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_firtEnterApp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_firtEnterApp : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_firtEnterApp : Error processing new value");
    cobj->firtEnterApp = arg0;
    return true;
}
bool js_wb_ZJHModel_get_isNoticeFirstShow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_isNoticeFirstShow : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isNoticeFirstShow);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_isNoticeFirstShow(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_isNoticeFirstShow : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_isNoticeFirstShow : Error processing new value");
    cobj->isNoticeFirstShow = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gameUseDefaultSetting(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gameUseDefaultSetting : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->gameUseDefaultSetting);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gameUseDefaultSetting(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gameUseDefaultSetting : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gameUseDefaultSetting : Error processing new value");
    cobj->gameUseDefaultSetting = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gameUseDefaultChat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gameUseDefaultChat : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->gameUseDefaultChat);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gameUseDefaultChat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gameUseDefaultChat : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gameUseDefaultChat : Error processing new value");
    cobj->gameUseDefaultChat = arg0;
    return true;
}
bool js_wb_ZJHModel_get_gameDefaultChat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_gameDefaultChat : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->gameDefaultChat);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_gameDefaultChat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_gameDefaultChat : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_gameDefaultChat : Error processing new value");
    cobj->gameDefaultChat = arg0;
    return true;
}
bool js_wb_ZJHModel_get_phonePlatfm(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_phonePlatfm : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->phonePlatfm);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_phonePlatfm(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_phonePlatfm : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_phonePlatfm : Error processing new value");
    cobj->phonePlatfm = arg0;
    return true;
}
bool js_wb_ZJHModel_get_iosAppBuySuceesePostTimes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_get_iosAppBuySuceesePostTimes : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->iosAppBuySuceesePostTimes);
    args.rval().set(jsret);
    return true;
}
bool js_wb_ZJHModel_set_iosAppBuySuceesePostTimes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    ZJHModel* cobj = (ZJHModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ZJHModel_set_iosAppBuySuceesePostTimes : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_ZJHModel_set_iosAppBuySuceesePostTimes : Error processing new value");
    cobj->iosAppBuySuceesePostTimes = arg0;
    return true;
}
bool js_wb_ZJHModel_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    ZJHModel* cobj = new (std::nothrow) ZJHModel();

    js_type_class_t *typeClass = js_get_type_from_native<ZJHModel>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "ZJHModel"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_wb_ZJHModel(JSContext *cx, JS::HandleObject global) {
    jsb_ZJHModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_ZJHModel_class->name = "ZJHModel";
    jsb_ZJHModel_class->addProperty = JS_PropertyStub;
    jsb_ZJHModel_class->delProperty = JS_DeletePropertyStub;
    jsb_ZJHModel_class->getProperty = JS_PropertyStub;
    jsb_ZJHModel_class->setProperty = JS_StrictPropertyStub;
    jsb_ZJHModel_class->enumerate = JS_EnumerateStub;
    jsb_ZJHModel_class->resolve = JS_ResolveStub;
    jsb_ZJHModel_class->convert = JS_ConvertStub;
    jsb_ZJHModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("count", js_wb_ZJHModel_get_count, js_wb_ZJHModel_set_count, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("GameServer", js_wb_ZJHModel_get_GameServer, js_wb_ZJHModel_set_GameServer, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isPause", js_wb_ZJHModel_get_isPause, js_wb_ZJHModel_set_isPause, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("uid", js_wb_ZJHModel_get_uid, js_wb_ZJHModel_set_uid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("bgMusic", js_wb_ZJHModel_get_bgMusic, js_wb_ZJHModel_set_bgMusic, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("sound", js_wb_ZJHModel_get_sound, js_wb_ZJHModel_set_sound, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vibrate", js_wb_ZJHModel_get_vibrate, js_wb_ZJHModel_set_vibrate, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("musicOn", js_wb_ZJHModel_get_musicOn, js_wb_ZJHModel_set_musicOn, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("soundOn", js_wb_ZJHModel_get_soundOn, js_wb_ZJHModel_set_soundOn, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("phoneOs", js_wb_ZJHModel_get_phoneOs, js_wb_ZJHModel_set_phoneOs, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("thirdId", js_wb_ZJHModel_get_thirdId, js_wb_ZJHModel_set_thirdId, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("UserKey", js_wb_ZJHModel_get_UserKey, js_wb_ZJHModel_set_UserKey, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vname", js_wb_ZJHModel_get_vname, js_wb_ZJHModel_set_vname, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vcode", js_wb_ZJHModel_get_vcode, js_wb_ZJHModel_set_vcode, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("channel_id", js_wb_ZJHModel_get_channel_id, js_wb_ZJHModel_set_channel_id, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("agentKey", js_wb_ZJHModel_get_agentKey, js_wb_ZJHModel_set_agentKey, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("phone", js_wb_ZJHModel_get_phone, js_wb_ZJHModel_set_phone, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("pwd", js_wb_ZJHModel_get_pwd, js_wb_ZJHModel_set_pwd, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("pwdOK", js_wb_ZJHModel_get_pwdOK, js_wb_ZJHModel_set_pwdOK, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("nick", js_wb_ZJHModel_get_nick, js_wb_ZJHModel_set_nick, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("signature", js_wb_ZJHModel_get_signature, js_wb_ZJHModel_set_signature, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vipLv", js_wb_ZJHModel_get_vipLv, js_wb_ZJHModel_set_vipLv, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("portrit", js_wb_ZJHModel_get_portrit, js_wb_ZJHModel_set_portrit, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("userType", js_wb_ZJHModel_get_userType, js_wb_ZJHModel_set_userType, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("userCity", js_wb_ZJHModel_get_userCity, js_wb_ZJHModel_set_userCity, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("provice", js_wb_ZJHModel_get_provice, js_wb_ZJHModel_set_provice, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("city", js_wb_ZJHModel_get_city, js_wb_ZJHModel_set_city, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("qu", js_wb_ZJHModel_get_qu, js_wb_ZJHModel_set_qu, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("weiXinBangPhone", js_wb_ZJHModel_get_weiXinBangPhone, js_wb_ZJHModel_set_weiXinBangPhone, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("NOPwdSetPwd", js_wb_ZJHModel_get_NOPwdSetPwd, js_wb_ZJHModel_set_NOPwdSetPwd, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("versionIOS", js_wb_ZJHModel_get_versionIOS, js_wb_ZJHModel_set_versionIOS, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("phoneimei", js_wb_ZJHModel_get_phoneimei, js_wb_ZJHModel_set_phoneimei, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("phonename", js_wb_ZJHModel_get_phonename, js_wb_ZJHModel_set_phonename, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isTrait", js_wb_ZJHModel_get_isTrait, js_wb_ZJHModel_set_isTrait, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gameHelpUrl", js_wb_ZJHModel_get_gameHelpUrl, js_wb_ZJHModel_set_gameHelpUrl, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("source_id", js_wb_ZJHModel_get_source_id, js_wb_ZJHModel_set_source_id, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("user", js_wb_ZJHModel_get_user, js_wb_ZJHModel_set_user, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("name", js_wb_ZJHModel_get_name, js_wb_ZJHModel_set_name, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("sex", js_wb_ZJHModel_get_sex, js_wb_ZJHModel_set_sex, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("avatar", js_wb_ZJHModel_get_avatar, js_wb_ZJHModel_set_avatar, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("money", js_wb_ZJHModel_get_money, js_wb_ZJHModel_set_money, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("rmb", js_wb_ZJHModel_get_rmb, js_wb_ZJHModel_set_rmb, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("coin", js_wb_ZJHModel_get_coin, js_wb_ZJHModel_set_coin, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("skey", js_wb_ZJHModel_get_skey, js_wb_ZJHModel_set_skey, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("dsc", js_wb_ZJHModel_get_dsc, js_wb_ZJHModel_set_dsc, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("exp", js_wb_ZJHModel_get_exp, js_wb_ZJHModel_set_exp, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("total_win", js_wb_ZJHModel_get_total_win, js_wb_ZJHModel_set_total_win, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("total_board", js_wb_ZJHModel_get_total_board, js_wb_ZJHModel_set_total_board, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curRoomID", js_wb_ZJHModel_get_curRoomID, js_wb_ZJHModel_set_curRoomID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vlevel", js_wb_ZJHModel_get_vlevel, js_wb_ZJHModel_set_vlevel, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("mySeatid", js_wb_ZJHModel_get_mySeatid, js_wb_ZJHModel_set_mySeatid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("myPoint", js_wb_ZJHModel_get_myPoint, js_wb_ZJHModel_set_myPoint, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("myBet", js_wb_ZJHModel_get_myBet, js_wb_ZJHModel_set_myBet, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("pauseTime", js_wb_ZJHModel_get_pauseTime, js_wb_ZJHModel_set_pauseTime, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("enterGame", js_wb_ZJHModel_get_enterGame, js_wb_ZJHModel_set_enterGame, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gamePuase", js_wb_ZJHModel_get_gamePuase, js_wb_ZJHModel_set_gamePuase, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gamehasinit", js_wb_ZJHModel_get_gamehasinit, js_wb_ZJHModel_set_gamehasinit, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("roomUserID", js_wb_ZJHModel_get_roomUserID, js_wb_ZJHModel_set_roomUserID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isGameLoad", js_wb_ZJHModel_get_isGameLoad, js_wb_ZJHModel_set_isGameLoad, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isReplace", js_wb_ZJHModel_get_isReplace, js_wb_ZJHModel_set_isReplace, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("recoveryHallParam", js_wb_ZJHModel_get_recoveryHallParam, js_wb_ZJHModel_set_recoveryHallParam, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curGameType", js_wb_ZJHModel_get_curGameType, js_wb_ZJHModel_set_curGameType, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isGamePhzFrist", js_wb_ZJHModel_get_isGamePhzFrist, js_wb_ZJHModel_set_isGamePhzFrist, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isOnVideo", js_wb_ZJHModel_get_isOnVideo, js_wb_ZJHModel_set_isOnVideo, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("roomVideoData", js_wb_ZJHModel_get_roomVideoData, js_wb_ZJHModel_set_roomVideoData, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("packName", js_wb_ZJHModel_get_packName, js_wb_ZJHModel_set_packName, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("IosTransactionId", js_wb_ZJHModel_get_IosTransactionId, js_wb_ZJHModel_set_IosTransactionId, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("IosReceipt", js_wb_ZJHModel_get_IosReceipt, js_wb_ZJHModel_set_IosReceipt, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("appStoreInvoice", js_wb_ZJHModel_get_appStoreInvoice, js_wb_ZJHModel_set_appStoreInvoice, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("iosTotalFee", js_wb_ZJHModel_get_iosTotalFee, js_wb_ZJHModel_set_iosTotalFee, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("appleId", js_wb_ZJHModel_get_appleId, js_wb_ZJHModel_set_appleId, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("iosPrice", js_wb_ZJHModel_get_iosPrice, js_wb_ZJHModel_set_iosPrice, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("serverDiffrentTime", js_wb_ZJHModel_get_serverDiffrentTime, js_wb_ZJHModel_set_serverDiffrentTime, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curGameID", js_wb_ZJHModel_get_curGameID, js_wb_ZJHModel_set_curGameID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curGameHasHall", js_wb_ZJHModel_get_curGameHasHall, js_wb_ZJHModel_set_curGameHasHall, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("firtEnterApp", js_wb_ZJHModel_get_firtEnterApp, js_wb_ZJHModel_set_firtEnterApp, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isNoticeFirstShow", js_wb_ZJHModel_get_isNoticeFirstShow, js_wb_ZJHModel_set_isNoticeFirstShow, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gameUseDefaultSetting", js_wb_ZJHModel_get_gameUseDefaultSetting, js_wb_ZJHModel_set_gameUseDefaultSetting, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gameUseDefaultChat", js_wb_ZJHModel_get_gameUseDefaultChat, js_wb_ZJHModel_set_gameUseDefaultChat, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("gameDefaultChat", js_wb_ZJHModel_get_gameDefaultChat, js_wb_ZJHModel_set_gameDefaultChat, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("phonePlatfm", js_wb_ZJHModel_get_phonePlatfm, js_wb_ZJHModel_set_phonePlatfm, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("iosAppBuySuceesePostTimes", js_wb_ZJHModel_get_iosAppBuySuceesePostTimes, js_wb_ZJHModel_set_iosAppBuySuceesePostTimes, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("matchEnterName", js_wb_ZJHModel_matchEnterName, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("reLoginLoadGame", js_wb_ZJHModel_reLoginLoadGame, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("requestPay", js_wb_ZJHModel_requestPay, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("listnerGameResum", js_wb_ZJHModel_listnerGameResum, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setRoomData", js_wb_ZJHModel_setRoomData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("requestThirdPay", js_wb_ZJHModel_requestThirdPay, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("roomToShareView", js_wb_ZJHModel_roomToShareView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("uploadMyLocation", js_wb_ZJHModel_uploadMyLocation, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("iosPurchaseResult", js_wb_ZJHModel_iosPurchaseResult, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoVideo", js_wb_ZJHModel_gotoVideo, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCurGameID", js_wb_ZJHModel_setCurGameID, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("roomHearHttpRequest", js_wb_ZJHModel_roomHearHttpRequest, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("roomToHall", js_wb_ZJHModel_roomToHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initPhoneData", js_wb_ZJHModel_initPhoneData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setUserType", js_wb_ZJHModel_setUserType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("moneyShowDeal", js_wb_ZJHModel_moneyShowDeal, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_ZJHModel_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoGameLoad", js_wb_ZJHModel_gotoGameLoad, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toMatchHall", js_wb_ZJHModel_toMatchHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("converPointTwoBit", js_wb_ZJHModel_converPointTwoBit, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dealAppUpdate", js_wb_ZJHModel_dealAppUpdate, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getRoomData", js_wb_ZJHModel_getRoomData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkUploadMoblieContacts", js_wb_ZJHModel_checkUploadMoblieContacts, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("randomThirdID", js_wb_ZJHModel_randomThirdID, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoRoomWithCode", js_wb_ZJHModel_gotoRoomWithCode, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hidePhoneNum", js_wb_ZJHModel_hidePhoneNum, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("changeScreenSize", js_wb_ZJHModel_changeScreenSize, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toMatchRoom", js_wb_ZJHModel_toMatchRoom, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("startRoomHeart", js_wb_ZJHModel_startRoomHeart, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toGameHall", js_wb_ZJHModel_toGameHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("noticeGamePlayEvent", js_wb_ZJHModel_noticeGamePlayEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoRoom", js_wb_ZJHModel_gotoRoom, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isOpenIPV6", js_wb_ZJHModel_isOpenIPV6, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("listnerPaySucceed", js_wb_ZJHModel_listnerPaySucceed, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hallToRoomV", js_wb_ZJHModel_hallToRoomV, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkGamePlayingError", js_wb_ZJHModel_checkGamePlayingError, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toHall", js_wb_ZJHModel_toHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setDiquCach", js_wb_ZJHModel_setDiquCach, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("goldCovertStr2", js_wb_ZJHModel_goldCovertStr2, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadingImageResource", js_wb_ZJHModel_loadingImageResource, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isYoukeLogin", js_wb_ZJHModel_isYoukeLogin, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPhoneAndPwd", js_wb_ZJHModel_setPhoneAndPwd, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isCanPay", js_wb_ZJHModel_isCanPay, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hotUpdateGame", js_wb_ZJHModel_hotUpdateGame, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkNoRmb", js_wb_ZJHModel_checkNoRmb, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkNoMoney", js_wb_ZJHModel_checkNoMoney, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hallToRoom", js_wb_ZJHModel_hallToRoom, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createTestMatchHall", js_wb_ZJHModel_createTestMatchHall, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("GameResum", js_wb_ZJHModel_GameResum, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hallToRoomH", js_wb_ZJHModel_hallToRoomH, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_wb_ZJHModel_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getStr", js_wb_ZJHModel_getStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_ZJHModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_ZJHModel_class,
        js_wb_ZJHModel_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_ZJHModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "ZJHModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<ZJHModel>(cx, jsb_ZJHModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_Sound_class;
JSObject *jsb_Sound_prototype;

bool js_wb_Sound_playDNGameEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_playDNGameEffect : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_playDNGameEffect : Error processing arguments");
        cobj->playDNGameEffect(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_playDNGameEffect : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_Sound_playGameEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_playGameEffect : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_playGameEffect : Error processing arguments");
        cobj->playGameEffect(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_playGameEffect : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_Sound_PlayBgSound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_PlayBgSound : Invalid Native Object");
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_PlayBgSound : Error processing arguments");
        cobj->PlayBgSound(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        const char* arg0 = nullptr;
        bool arg1;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_PlayBgSound : Error processing arguments");
        cobj->PlayBgSound(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        const char* arg0 = nullptr;
        bool arg1;
        bool arg2;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        arg1 = JS::ToBoolean(args.get(1));
        arg2 = JS::ToBoolean(args.get(2));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_PlayBgSound : Error processing arguments");
        cobj->PlayBgSound(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_PlayBgSound : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_Sound_playDNGameCardTypeEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_playDNGameCardTypeEffect : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_playDNGameCardTypeEffect : Error processing arguments");
        cobj->playDNGameCardTypeEffect(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_playDNGameCardTypeEffect : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_Sound_playEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    bool ok = true;
    Sound* cobj = nullptr;

    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx);
    obj.set(args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (Sound *)(proxy ? proxy->ptr : nullptr);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_playEffect : Invalid Native Object");
    do {
        if (argc == 2) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            bool arg1;
            arg1 = JS::ToBoolean(args.get(1));
            int ret = cobj->playEffect(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            jsret = int32_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 3) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            bool arg1;
            arg1 = JS::ToBoolean(args.get(1));
            bool arg2;
            arg2 = JS::ToBoolean(args.get(2));
            int ret = cobj->playEffect(arg0, arg1, arg2);
            jsval jsret = JSVAL_NULL;
            jsret = int32_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            cobj->playEffect(arg0);
            args.rval().setUndefined();
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_wb_Sound_playEffect : wrong number of arguments");
    return false;
}
bool js_wb_Sound_preLoadSound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_preLoadSound : Invalid Native Object");
    if (argc == 0) {
        cobj->preLoadSound();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_preLoadSound : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_Sound_preLoadEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_preLoadEffect : Invalid Native Object");
    if (argc == 1) {
        const char* arg0 = nullptr;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_preLoadEffect : Error processing arguments");
        cobj->preLoadEffect(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_preLoadEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_Sound_StopBgSound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_StopBgSound : Invalid Native Object");
    if (argc == 0) {
        cobj->StopBgSound();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_StopBgSound : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_Sound_loadEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    Sound* cobj = (Sound *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_Sound_loadEffect : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_Sound_loadEffect : Error processing arguments");
        cobj->loadEffect(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_Sound_loadEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_Sound_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = Sound::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<Sound>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "Sound"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_Sound_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_Sound_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Sound* cobj = new (std::nothrow) Sound();

    js_type_class_t *typeClass = js_get_type_from_native<Sound>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "Sound"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_wb_Sound(JSContext *cx, JS::HandleObject global) {
    jsb_Sound_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Sound_class->name = "Sound";
    jsb_Sound_class->addProperty = JS_PropertyStub;
    jsb_Sound_class->delProperty = JS_DeletePropertyStub;
    jsb_Sound_class->getProperty = JS_PropertyStub;
    jsb_Sound_class->setProperty = JS_StrictPropertyStub;
    jsb_Sound_class->enumerate = JS_EnumerateStub;
    jsb_Sound_class->resolve = JS_ResolveStub;
    jsb_Sound_class->convert = JS_ConvertStub;
    jsb_Sound_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("playDNGameEffect", js_wb_Sound_playDNGameEffect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playGameEffect", js_wb_Sound_playGameEffect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("PlayBgSound", js_wb_Sound_PlayBgSound, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playDNGameCardTypeEffect", js_wb_Sound_playDNGameCardTypeEffect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playEffect", js_wb_Sound_playEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("preLoadSound", js_wb_Sound_preLoadSound, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("preLoadEffect", js_wb_Sound_preLoadEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("StopBgSound", js_wb_Sound_StopBgSound, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("loadEffect", js_wb_Sound_loadEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_wb_Sound_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_Sound_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Sound_class,
        js_wb_Sound_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Sound_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Sound"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Sound>(cx, jsb_Sound_class, proto, JS::NullPtr());
}

JSClass  *jsb_PlayerModel_class;
JSObject *jsb_PlayerModel_prototype;

bool js_wb_PlayerModel_reset(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_reset : Invalid Native Object");
    if (argc == 0) {
        cobj->reset();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_PlayerModel_reset : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_PlayerModel_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_PlayerModel_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_PlayerModel_prepra2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_prepra2 : Invalid Native Object");
    if (argc == 0) {
        cobj->prepra2();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_PlayerModel_prepra2 : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_PlayerModel_prepra(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_prepra : Invalid Native Object");
    if (argc == 0) {
        cobj->prepra();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_PlayerModel_prepra : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_PlayerModel_get_seatid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_seatid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->seatid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_seatid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_seatid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_seatid : Error processing new value");
    cobj->seatid = arg0;
    return true;
}
bool js_wb_PlayerModel_get_occupied(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_occupied : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->occupied);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_occupied(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_occupied : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_occupied : Error processing new value");
    cobj->occupied = arg0;
    return true;
}
bool js_wb_PlayerModel_get_ready(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_ready : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->ready);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_ready(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_ready : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_ready : Error processing new value");
    cobj->ready = arg0;
    return true;
}
bool js_wb_PlayerModel_get_betting(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_betting : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->betting);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_betting(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_betting : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_betting : Error processing new value");
    cobj->betting = arg0;
    return true;
}
bool js_wb_PlayerModel_get_see(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_see : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->see);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_see(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_see : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_see : Error processing new value");
    cobj->see = arg0;
    return true;
}
bool js_wb_PlayerModel_get_role(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_role : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->role);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_role(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_role : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_role : Error processing new value");
    cobj->role = arg0;
    return true;
}
bool js_wb_PlayerModel_get_status(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_status : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->status);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_status(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_status : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_status : Error processing new value");
    cobj->status = arg0;
    return true;
}
bool js_wb_PlayerModel_get_bet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_bet : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->bet);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_bet(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_bet : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_bet : Error processing new value");
    cobj->bet = arg0;
    return true;
}
bool js_wb_PlayerModel_get_uid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_uid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->uid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_uid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_uid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_uid : Error processing new value");
    cobj->uid = arg0;
    return true;
}
bool js_wb_PlayerModel_get_name(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_name : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->name);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_name(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_name : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_name : Error processing new value");
    cobj->name = arg0;
    return true;
}
bool js_wb_PlayerModel_get_sex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_sex : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->sex);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_sex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_sex : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_sex : Error processing new value");
    cobj->sex = arg0;
    return true;
}
bool js_wb_PlayerModel_get_avatar(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_avatar : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->avatar);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_avatar(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_avatar : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_avatar : Error processing new value");
    cobj->avatar = arg0;
    return true;
}
bool js_wb_PlayerModel_get_birthday(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_birthday : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->birthday);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_birthday(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_birthday : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_birthday : Error processing new value");
    cobj->birthday = arg0;
    return true;
}
bool js_wb_PlayerModel_get_zone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_zone : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->zone);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_zone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_zone : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_zone : Error processing new value");
    cobj->zone = arg0;
    return true;
}
bool js_wb_PlayerModel_get_dsc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_dsc : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->dsc);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_dsc(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_dsc : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_dsc : Error processing new value");
    cobj->dsc = arg0;
    return true;
}
bool js_wb_PlayerModel_get_exp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_exp : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->exp);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_exp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_exp : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_exp : Error processing new value");
    cobj->exp = arg0;
    return true;
}
bool js_wb_PlayerModel_get_rmb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_rmb : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->rmb);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_rmb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_rmb : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_rmb : Error processing new value");
    cobj->rmb = arg0;
    return true;
}
bool js_wb_PlayerModel_get_money(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_money : Invalid Native Object");

    jsval jsret = long_to_jsval(cx, cobj->money);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_money(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_money : Invalid Native Object");

    bool ok = true;
    long arg0 = 0;
    ok &= jsval_to_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_money : Error processing new value");
    cobj->money = arg0;
    return true;
}
bool js_wb_PlayerModel_get_matchMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_matchMoney : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->matchMoney);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_matchMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_matchMoney : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_matchMoney : Error processing new value");
    cobj->matchMoney = arg0;
    return true;
}
bool js_wb_PlayerModel_get_coin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_coin : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->coin);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_coin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_coin : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_coin : Error processing new value");
    cobj->coin = arg0;
    return true;
}
bool js_wb_PlayerModel_get_total_board(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_total_board : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->total_board);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_total_board(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_total_board : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_total_board : Error processing new value");
    cobj->total_board = arg0;
    return true;
}
bool js_wb_PlayerModel_get_total_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_total_win : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->total_win);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_total_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_total_win : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_total_win : Error processing new value");
    cobj->total_win = arg0;
    return true;
}
bool js_wb_PlayerModel_get_pcount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_pcount : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->pcount);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_pcount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_pcount : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_pcount : Error processing new value");
    cobj->pcount = arg0;
    return true;
}
bool js_wb_PlayerModel_get_vtime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_vtime : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vtime);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_vtime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_vtime : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_vtime : Error processing new value");
    cobj->vtime = arg0;
    return true;
}
bool js_wb_PlayerModel_get_vlevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_vlevel : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->vlevel);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_vlevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_vlevel : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_vlevel : Error processing new value");
    cobj->vlevel = arg0;
    return true;
}
bool js_wb_PlayerModel_get_redid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_redid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->redid);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_redid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_redid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_redid : Error processing new value");
    cobj->redid = arg0;
    return true;
}
bool js_wb_PlayerModel_get_redmoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_redmoney : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->redmoney);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_redmoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_redmoney : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_redmoney : Error processing new value");
    cobj->redmoney = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isBig(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isBig : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isBig);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isBig(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isBig : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isBig : Error processing new value");
    cobj->isBig = arg0;
    return true;
}
bool js_wb_PlayerModel_get_card_type(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_card_type : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->card_type);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_card_type(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_card_type : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_card_type : Error processing new value");
    cobj->card_type = arg0;
    return true;
}
bool js_wb_PlayerModel_get_offline(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_offline : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->offline);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_offline(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_offline : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_offline : Error processing new value");
    cobj->offline = arg0;
    return true;
}
bool js_wb_PlayerModel_get_jinbiRound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_jinbiRound : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->jinbiRound);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_jinbiRound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_jinbiRound : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_jinbiRound : Error processing new value");
    cobj->jinbiRound = arg0;
    return true;
}
bool js_wb_PlayerModel_get_fanbeiRound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_fanbeiRound : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->fanbeiRound);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_fanbeiRound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_fanbeiRound : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_fanbeiRound : Error processing new value");
    cobj->fanbeiRound = arg0;
    return true;
}
bool js_wb_PlayerModel_get_fanbeiNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_fanbeiNums : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->fanbeiNums);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_fanbeiNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_fanbeiNums : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_fanbeiNums : Error processing new value");
    cobj->fanbeiNums = arg0;
    return true;
}
bool js_wb_PlayerModel_get_huanpaiTimes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_huanpaiTimes : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->huanpaiTimes);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_huanpaiTimes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_huanpaiTimes : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_huanpaiTimes : Error processing new value");
    cobj->huanpaiTimes = arg0;
    return true;
}
bool js_wb_PlayerModel_get_jinbika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_jinbika : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->jinbika);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_jinbika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_jinbika : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_jinbika : Error processing new value");
    cobj->jinbika = arg0;
    return true;
}
bool js_wb_PlayerModel_get_fanbeika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_fanbeika : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->fanbeika);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_fanbeika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_fanbeika : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_fanbeika : Error processing new value");
    cobj->fanbeika = arg0;
    return true;
}
bool js_wb_PlayerModel_get_huanpaika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_huanpaika : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->huanpaika);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_huanpaika(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_huanpaika : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_huanpaika : Error processing new value");
    cobj->huanpaika = arg0;
    return true;
}
bool js_wb_PlayerModel_get_allBetMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_allBetMoney : Invalid Native Object");

    jsval jsret = long_to_jsval(cx, cobj->allBetMoney);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_allBetMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_allBetMoney : Invalid Native Object");

    bool ok = true;
    long arg0 = 0;
    ok &= jsval_to_long(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_allBetMoney : Error processing new value");
    cobj->allBetMoney = arg0;
    return true;
}
bool js_wb_PlayerModel_get_myBetNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_myBetNums : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->myBetNums);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_myBetNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_myBetNums : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_myBetNums : Error processing new value");
    cobj->myBetNums = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isOut(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isOut : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isOut);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isOut(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isOut : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isOut : Error processing new value");
    cobj->isOut = arg0;
    return true;
}
bool js_wb_PlayerModel_get_dangle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_dangle : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->dangle);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_dangle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_dangle : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_dangle : Error processing new value");
    cobj->dangle = arg0;
    return true;
}
bool js_wb_PlayerModel_get_cardType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_cardType : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->cardType);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_cardType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_cardType : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_cardType : Error processing new value");
    cobj->cardType = arg0;
    return true;
}
bool js_wb_PlayerModel_get_baccarat_playCount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_baccarat_playCount : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->baccarat_playCount);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_baccarat_playCount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_baccarat_playCount : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_baccarat_playCount : Error processing new value");
    cobj->baccarat_playCount = arg0;
    return true;
}
bool js_wb_PlayerModel_get_baccarat_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_baccarat_win : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->baccarat_win);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_baccarat_win(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_baccarat_win : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_baccarat_win : Error processing new value");
    cobj->baccarat_win = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isRobort(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isRobort : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isRobort);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isRobort(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isRobort : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isRobort : Error processing new value");
    cobj->isRobort = arg0;
    return true;
}
bool js_wb_PlayerModel_get_lifeNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_lifeNums : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->lifeNums);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_lifeNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_lifeNums : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_lifeNums : Error processing new value");
    cobj->lifeNums = arg0;
    return true;
}
bool js_wb_PlayerModel_get_mj_cardNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_mj_cardNums : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->mj_cardNums);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_mj_cardNums(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_mj_cardNums : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_mj_cardNums : Error processing new value");
    cobj->mj_cardNums = arg0;
    return true;
}
bool js_wb_PlayerModel_get_mj_curScore(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_mj_curScore : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->mj_curScore);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_mj_curScore(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_mj_curScore : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_mj_curScore : Error processing new value");
    cobj->mj_curScore = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isTuoGuan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isTuoGuan : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isTuoGuan);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isTuoGuan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isTuoGuan : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isTuoGuan : Error processing new value");
    cobj->isTuoGuan = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isTimeOut(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isTimeOut : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isTimeOut);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isTimeOut(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isTimeOut : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isTimeOut : Error processing new value");
    cobj->isTimeOut = arg0;
    return true;
}
bool js_wb_PlayerModel_get_zhuan_surplustime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_zhuan_surplustime : Invalid Native Object");

    jsval jsret = DOUBLE_TO_JSVAL(cobj->zhuan_surplustime);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_zhuan_surplustime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_zhuan_surplustime : Invalid Native Object");

    bool ok = true;
    double arg0 = 0;
    ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_zhuan_surplustime : Error processing new value");
    cobj->zhuan_surplustime = arg0;
    return true;
}
bool js_wb_PlayerModel_get_isOffline(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_isOffline : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->isOffline);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_isOffline(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_isOffline : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_isOffline : Error processing new value");
    cobj->isOffline = arg0;
    return true;
}
bool js_wb_PlayerModel_get_mingpai_start(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_mingpai_start : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->mingpai_start);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_mingpai_start(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_mingpai_start : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_mingpai_start : Error processing new value");
    cobj->mingpai_start = arg0;
    return true;
}
bool js_wb_PlayerModel_get_base_double(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_base_double : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->base_double);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_base_double(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_base_double : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_base_double : Error processing new value");
    cobj->base_double = arg0;
    return true;
}
bool js_wb_PlayerModel_get_mingpai_play(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_mingpai_play : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->mingpai_play);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_mingpai_play(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_mingpai_play : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_mingpai_play : Error processing new value");
    cobj->mingpai_play = arg0;
    return true;
}
bool js_wb_PlayerModel_get_qiangguan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_get_qiangguan : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->qiangguan);
    args.rval().set(jsret);
    return true;
}
bool js_wb_PlayerModel_set_qiangguan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PlayerModel* cobj = (PlayerModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_PlayerModel_set_qiangguan : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_PlayerModel_set_qiangguan : Error processing new value");
    cobj->qiangguan = arg0;
    return true;
}
bool js_wb_PlayerModel_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    PlayerModel* cobj = new (std::nothrow) PlayerModel();

    js_type_class_t *typeClass = js_get_type_from_native<PlayerModel>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "PlayerModel"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_PlayerModel_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (PlayerModel)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        PlayerModel *nobj = static_cast<PlayerModel *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_wb_PlayerModel(JSContext *cx, JS::HandleObject global) {
    jsb_PlayerModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_PlayerModel_class->name = "PlayerModel";
    jsb_PlayerModel_class->addProperty = JS_PropertyStub;
    jsb_PlayerModel_class->delProperty = JS_DeletePropertyStub;
    jsb_PlayerModel_class->getProperty = JS_PropertyStub;
    jsb_PlayerModel_class->setProperty = JS_StrictPropertyStub;
    jsb_PlayerModel_class->enumerate = JS_EnumerateStub;
    jsb_PlayerModel_class->resolve = JS_ResolveStub;
    jsb_PlayerModel_class->convert = JS_ConvertStub;
    jsb_PlayerModel_class->finalize = js_PlayerModel_finalize;
    jsb_PlayerModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("seatid", js_wb_PlayerModel_get_seatid, js_wb_PlayerModel_set_seatid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("occupied", js_wb_PlayerModel_get_occupied, js_wb_PlayerModel_set_occupied, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("ready", js_wb_PlayerModel_get_ready, js_wb_PlayerModel_set_ready, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("betting", js_wb_PlayerModel_get_betting, js_wb_PlayerModel_set_betting, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("see", js_wb_PlayerModel_get_see, js_wb_PlayerModel_set_see, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("role", js_wb_PlayerModel_get_role, js_wb_PlayerModel_set_role, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("status", js_wb_PlayerModel_get_status, js_wb_PlayerModel_set_status, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("bet", js_wb_PlayerModel_get_bet, js_wb_PlayerModel_set_bet, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("uid", js_wb_PlayerModel_get_uid, js_wb_PlayerModel_set_uid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("name", js_wb_PlayerModel_get_name, js_wb_PlayerModel_set_name, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("sex", js_wb_PlayerModel_get_sex, js_wb_PlayerModel_set_sex, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("avatar", js_wb_PlayerModel_get_avatar, js_wb_PlayerModel_set_avatar, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("birthday", js_wb_PlayerModel_get_birthday, js_wb_PlayerModel_set_birthday, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("zone", js_wb_PlayerModel_get_zone, js_wb_PlayerModel_set_zone, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("dsc", js_wb_PlayerModel_get_dsc, js_wb_PlayerModel_set_dsc, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("exp", js_wb_PlayerModel_get_exp, js_wb_PlayerModel_set_exp, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("rmb", js_wb_PlayerModel_get_rmb, js_wb_PlayerModel_set_rmb, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("money", js_wb_PlayerModel_get_money, js_wb_PlayerModel_set_money, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("matchMoney", js_wb_PlayerModel_get_matchMoney, js_wb_PlayerModel_set_matchMoney, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("coin", js_wb_PlayerModel_get_coin, js_wb_PlayerModel_set_coin, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("total_board", js_wb_PlayerModel_get_total_board, js_wb_PlayerModel_set_total_board, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("total_win", js_wb_PlayerModel_get_total_win, js_wb_PlayerModel_set_total_win, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("pcount", js_wb_PlayerModel_get_pcount, js_wb_PlayerModel_set_pcount, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vtime", js_wb_PlayerModel_get_vtime, js_wb_PlayerModel_set_vtime, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("vlevel", js_wb_PlayerModel_get_vlevel, js_wb_PlayerModel_set_vlevel, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("redid", js_wb_PlayerModel_get_redid, js_wb_PlayerModel_set_redid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("redmoney", js_wb_PlayerModel_get_redmoney, js_wb_PlayerModel_set_redmoney, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isBig", js_wb_PlayerModel_get_isBig, js_wb_PlayerModel_set_isBig, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("card_type", js_wb_PlayerModel_get_card_type, js_wb_PlayerModel_set_card_type, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("offline", js_wb_PlayerModel_get_offline, js_wb_PlayerModel_set_offline, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("jinbiRound", js_wb_PlayerModel_get_jinbiRound, js_wb_PlayerModel_set_jinbiRound, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("fanbeiRound", js_wb_PlayerModel_get_fanbeiRound, js_wb_PlayerModel_set_fanbeiRound, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("fanbeiNums", js_wb_PlayerModel_get_fanbeiNums, js_wb_PlayerModel_set_fanbeiNums, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("huanpaiTimes", js_wb_PlayerModel_get_huanpaiTimes, js_wb_PlayerModel_set_huanpaiTimes, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("jinbika", js_wb_PlayerModel_get_jinbika, js_wb_PlayerModel_set_jinbika, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("fanbeika", js_wb_PlayerModel_get_fanbeika, js_wb_PlayerModel_set_fanbeika, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("huanpaika", js_wb_PlayerModel_get_huanpaika, js_wb_PlayerModel_set_huanpaika, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("allBetMoney", js_wb_PlayerModel_get_allBetMoney, js_wb_PlayerModel_set_allBetMoney, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("myBetNums", js_wb_PlayerModel_get_myBetNums, js_wb_PlayerModel_set_myBetNums, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isOut", js_wb_PlayerModel_get_isOut, js_wb_PlayerModel_set_isOut, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("dangle", js_wb_PlayerModel_get_dangle, js_wb_PlayerModel_set_dangle, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("cardType", js_wb_PlayerModel_get_cardType, js_wb_PlayerModel_set_cardType, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("baccarat_playCount", js_wb_PlayerModel_get_baccarat_playCount, js_wb_PlayerModel_set_baccarat_playCount, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("baccarat_win", js_wb_PlayerModel_get_baccarat_win, js_wb_PlayerModel_set_baccarat_win, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isRobort", js_wb_PlayerModel_get_isRobort, js_wb_PlayerModel_set_isRobort, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("lifeNums", js_wb_PlayerModel_get_lifeNums, js_wb_PlayerModel_set_lifeNums, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("mj_cardNums", js_wb_PlayerModel_get_mj_cardNums, js_wb_PlayerModel_set_mj_cardNums, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("mj_curScore", js_wb_PlayerModel_get_mj_curScore, js_wb_PlayerModel_set_mj_curScore, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isTuoGuan", js_wb_PlayerModel_get_isTuoGuan, js_wb_PlayerModel_set_isTuoGuan, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isTimeOut", js_wb_PlayerModel_get_isTimeOut, js_wb_PlayerModel_set_isTimeOut, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("zhuan_surplustime", js_wb_PlayerModel_get_zhuan_surplustime, js_wb_PlayerModel_set_zhuan_surplustime, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isOffline", js_wb_PlayerModel_get_isOffline, js_wb_PlayerModel_set_isOffline, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("mingpai_start", js_wb_PlayerModel_get_mingpai_start, js_wb_PlayerModel_set_mingpai_start, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("base_double", js_wb_PlayerModel_get_base_double, js_wb_PlayerModel_set_base_double, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("mingpai_play", js_wb_PlayerModel_get_mingpai_play, js_wb_PlayerModel_set_mingpai_play, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("qiangguan", js_wb_PlayerModel_get_qiangguan, js_wb_PlayerModel_set_qiangguan, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("reset", js_wb_PlayerModel_reset, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("release", js_wb_PlayerModel_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("prepra2", js_wb_PlayerModel_prepra2, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("prepra", js_wb_PlayerModel_prepra, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_PlayerModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_PlayerModel_class,
        js_wb_PlayerModel_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_PlayerModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "PlayerModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<PlayerModel>(cx, jsb_PlayerModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_JsUtils_class;
JSObject *jsb_JsUtils_prototype;

bool js_wb_JsUtils_onGameChangeEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onGameChangeEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onGameChangeEvent : Error processing arguments");
        cobj->onGameChangeEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onGameChangeEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onPowerChangeEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onPowerChangeEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onPowerChangeEvent : Error processing arguments");
        cobj->onPowerChangeEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onPowerChangeEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onChatStartEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onChatStartEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onChatStartEvent : Error processing arguments");
        cobj->onChatStartEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onChatStartEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onIMEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onIMEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onIMEvent : Error processing arguments");
        cobj->onIMEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onIMEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onChatStopEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onChatStopEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onChatStopEvent : Error processing arguments");
        cobj->onChatStopEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onChatStopEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onChatEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onChatEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onChatEvent : Error processing arguments");
        cobj->onChatEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onChatEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_onJSModuleEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JsUtils* cobj = (JsUtils *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_JsUtils_onJSModuleEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_onJSModuleEvent : Error processing arguments");
        cobj->onJSModuleEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_JsUtils_onJSModuleEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_JsUtils_resetJsVM(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        JsUtils::resetJsVM();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_resetJsVM : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_decodeBase64(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_decodeBase64 : Error processing arguments");

        std::string ret = JsUtils::decodeBase64(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_decodeBase64 : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = JsUtils::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<JsUtils>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "JsUtils"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_initLoad(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        JsUtils::initLoad();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_initLoad : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_postNotifi(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_postNotifi : Error processing arguments");
        JsUtils::postNotifi(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_postNotifi : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_postNotifiStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_postNotifiStr : Error processing arguments");
        JsUtils::postNotifiStr(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_postNotifiStr : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_decodeVideoData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_decodeVideoData : Error processing arguments");

        std::string ret = JsUtils::decodeVideoData(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_decodeVideoData : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_postNotifiInt(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_postNotifiInt : Error processing arguments");
        JsUtils::postNotifiInt(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_postNotifiInt : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_runScript(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_runScript : Error processing arguments");
        JsUtils::runScript(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_runScript : wrong number of arguments");
    return false;
}

bool js_wb_JsUtils_encodeBase64(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        const unsigned char* arg0 = nullptr;
        unsigned int arg1 = 0;
        #pragma warning NO CONVERSION TO NATIVE FOR unsigned char*
		ok = false;
        ok &= jsval_to_uint32(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_JsUtils_encodeBase64 : Error processing arguments");

        std::string ret = JsUtils::encodeBase64(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_JsUtils_encodeBase64 : wrong number of arguments");
    return false;
}


void js_register_wb_JsUtils(JSContext *cx, JS::HandleObject global) {
    jsb_JsUtils_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_JsUtils_class->name = "JsUtils";
    jsb_JsUtils_class->addProperty = JS_PropertyStub;
    jsb_JsUtils_class->delProperty = JS_DeletePropertyStub;
    jsb_JsUtils_class->getProperty = JS_PropertyStub;
    jsb_JsUtils_class->setProperty = JS_StrictPropertyStub;
    jsb_JsUtils_class->enumerate = JS_EnumerateStub;
    jsb_JsUtils_class->resolve = JS_ResolveStub;
    jsb_JsUtils_class->convert = JS_ConvertStub;
    jsb_JsUtils_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("onGameChangeEvent", js_wb_JsUtils_onGameChangeEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onPowerChangeEvent", js_wb_JsUtils_onPowerChangeEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onChatStartEvent", js_wb_JsUtils_onChatStartEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onIMEvent", js_wb_JsUtils_onIMEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onChatStopEvent", js_wb_JsUtils_onChatStopEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onChatEvent", js_wb_JsUtils_onChatEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onJSModuleEvent", js_wb_JsUtils_onJSModuleEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("resetJsVM", js_wb_JsUtils_resetJsVM, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("decodeBase64", js_wb_JsUtils_decodeBase64, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getInstance", js_wb_JsUtils_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initLoad", js_wb_JsUtils_initLoad, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("postNotifi", js_wb_JsUtils_postNotifi, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("postNotifiStr", js_wb_JsUtils_postNotifiStr, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("decodeVideoData", js_wb_JsUtils_decodeVideoData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("postNotifiInt", js_wb_JsUtils_postNotifiInt, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("runScript", js_wb_JsUtils_runScript, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("encodeBase64", js_wb_JsUtils_encodeBase64, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_JsUtils_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_JsUtils_class,
        empty_constructor, 0,
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_JsUtils_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "JsUtils"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<JsUtils>(cx, jsb_JsUtils_class, proto, JS::NullPtr());
}

JSClass  *jsb_RoomPublic_class;
JSObject *jsb_RoomPublic_prototype;

bool js_wb_RoomPublic_updateTimeLabel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_updateTimeLabel : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_updateTimeLabel : Error processing arguments");
        cobj->updateTimeLabel(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_updateTimeLabel : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_notificationGameStart(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_notificationGameStart : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_notificationGameStart : Error processing arguments");
        cobj->notificationGameStart(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_notificationGameStart : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_checkGameEnd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_checkGameEnd : Invalid Native Object");
    if (argc == 0) {
        cobj->checkGameEnd();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_checkGameEnd : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_setMyVisible(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_setMyVisible : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_setMyVisible : Error processing arguments");
        cobj->setMyVisible(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_setMyVisible : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_checkEndRoomTip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_checkEndRoomTip : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_checkEndRoomTip : Error processing arguments");
        cobj->checkEndRoomTip(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_checkEndRoomTip : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_powerSigChange(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_powerSigChange : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_powerSigChange : Error processing arguments");
        cobj->powerSigChange(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_powerSigChange : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_getCurrentTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_getCurrentTime : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_getCurrentTime : Error processing arguments");
        cobj->getCurrentTime(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_getCurrentTime : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_notificationPlayerNumber(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_notificationPlayerNumber : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_notificationPlayerNumber : Error processing arguments");
        cobj->notificationPlayerNumber(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_notificationPlayerNumber : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_getCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_getCallback : Invalid Native Object");
    if (argc == 0) {
        MyCallBack_Str ret = cobj->getCallback();
        jsval jsret = JSVAL_NULL;
        #pragma warning NO CONVERSION FROM NATIVE FOR std::function;
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_getCallback : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_setGameStatus(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_setGameStatus : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_setGameStatus : Error processing arguments");
        cobj->setGameStatus(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_setGameStatus : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_checkGameEndRepeat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_checkGameEndRepeat : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_checkGameEndRepeat : Error processing arguments");
        cobj->checkGameEndRepeat(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_checkGameEndRepeat : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_refreshMenu(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_refreshMenu : Invalid Native Object");
    if (argc == 0) {
        cobj->refreshMenu();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_refreshMenu : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_setCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_setCallback : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_setCallback : Error processing arguments");
        cobj->setCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_setCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_setRequestFlag(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_setRequestFlag : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_setRequestFlag : Error processing arguments");
        cobj->setRequestFlag(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_setRequestFlag : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_setOnlineNumber(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_setOnlineNumber : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_setOnlineNumber : Error processing arguments");
        cobj->setOnlineNumber(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_setOnlineNumber : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_inviteShare(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_inviteShare : Invalid Native Object");
    if (argc == 0) {
        cobj->inviteShare();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_inviteShare : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_requestExit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_requestExit : Invalid Native Object");
    if (argc == 0) {
        cobj->requestExit();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_requestExit : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_notificationGameResume(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_notificationGameResume : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_notificationGameResume : Error processing arguments");
        cobj->notificationGameResume(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_notificationGameResume : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_notificationRoomStart(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_notificationRoomStart : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_notificationRoomStart : Error processing arguments");
        cobj->notificationRoomStart(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_notificationRoomStart : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_showPlayerList(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_showPlayerList : Invalid Native Object");
    if (argc == 0) {
        cobj->showPlayerList();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_showPlayerList : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomPublic_buttonCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_buttonCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_buttonCallback : Error processing arguments");
        cobj->buttonCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_buttonCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_menuCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_menuCallback : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_menuCallback : Error processing arguments");
        cobj->menuCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_menuCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_checkEndRoomTip_getData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomPublic* cobj = (RoomPublic *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomPublic_checkEndRoomTip_getData : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomPublic_checkEndRoomTip_getData : Error processing arguments");
        cobj->checkEndRoomTip_getData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomPublic_checkEndRoomTip_getData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomPublic_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomPublic::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomPublic>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomPublic"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomPublic_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomPublic(JSContext *cx, JS::HandleObject global) {
    jsb_RoomPublic_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomPublic_class->name = "RoomPublic";
    jsb_RoomPublic_class->addProperty = JS_PropertyStub;
    jsb_RoomPublic_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomPublic_class->getProperty = JS_PropertyStub;
    jsb_RoomPublic_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomPublic_class->enumerate = JS_EnumerateStub;
    jsb_RoomPublic_class->resolve = JS_ResolveStub;
    jsb_RoomPublic_class->convert = JS_ConvertStub;
    jsb_RoomPublic_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("updateTimeLabel", js_wb_RoomPublic_updateTimeLabel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationGameStart", js_wb_RoomPublic_notificationGameStart, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkGameEnd", js_wb_RoomPublic_checkGameEnd, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setMyVisible", js_wb_RoomPublic_setMyVisible, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkEndRoomTip", js_wb_RoomPublic_checkEndRoomTip, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("powerSigChange", js_wb_RoomPublic_powerSigChange, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentTime", js_wb_RoomPublic_getCurrentTime, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationPlayerNumber", js_wb_RoomPublic_notificationPlayerNumber, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomPublic_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCallback", js_wb_RoomPublic_getCallback, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setGameStatus", js_wb_RoomPublic_setGameStatus, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkGameEndRepeat", js_wb_RoomPublic_checkGameEndRepeat, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("refreshMenu", js_wb_RoomPublic_refreshMenu, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCallback", js_wb_RoomPublic_setCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setRequestFlag", js_wb_RoomPublic_setRequestFlag, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setOnlineNumber", js_wb_RoomPublic_setOnlineNumber, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("inviteShare", js_wb_RoomPublic_inviteShare, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("requestExit", js_wb_RoomPublic_requestExit, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationGameResume", js_wb_RoomPublic_notificationGameResume, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationRoomStart", js_wb_RoomPublic_notificationRoomStart, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("showPlayerList", js_wb_RoomPublic_showPlayerList, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonCallback", js_wb_RoomPublic_buttonCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("menuCallback", js_wb_RoomPublic_menuCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkEndRoomTip_getData", js_wb_RoomPublic_checkEndRoomTip_getData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomPublic_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomPublic_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomPublic_class,
        dummy_constructor<RoomPublic>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomPublic_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomPublic"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomPublic>(cx, jsb_RoomPublic_class, proto, parent_proto);
}

JSClass  *jsb_RoomHEndTouPiao_class;
JSObject *jsb_RoomHEndTouPiao_prototype;

bool js_wb_RoomHEndTouPiao_initWithJsonStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_initWithJsonStr : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_initWithJsonStr : Error processing arguments");
        cobj->initWithJsonStr(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_initWithJsonStr : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHEndTouPiao_updateTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_updateTime : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_updateTime : Error processing arguments");
        cobj->updateTime(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_updateTime : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHEndTouPiao_updateState(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_updateState : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        int arg1 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_updateState : Error processing arguments");
        cobj->updateState(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_updateState : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomHEndTouPiao_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHEndTouPiao_setCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_setCallback : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_setCallback : Error processing arguments");
        cobj->setCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_setCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHEndTouPiao_getCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_getCallback : Invalid Native Object");
    if (argc == 0) {
        MyCallBack_Str ret = cobj->getCallback();
        jsval jsret = JSVAL_NULL;
        #pragma warning NO CONVERSION FROM NATIVE FOR std::function;
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_getCallback : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHEndTouPiao_buttonCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_buttonCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_buttonCallback : Error processing arguments");
        cobj->buttonCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_buttonCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHEndTouPiao_setDefaultTips(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHEndTouPiao* cobj = (RoomHEndTouPiao *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHEndTouPiao_setDefaultTips : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHEndTouPiao_setDefaultTips : Error processing arguments");
        cobj->setDefaultTips(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_setDefaultTips : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHEndTouPiao_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomHEndTouPiao::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomHEndTouPiao>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomHEndTouPiao"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomHEndTouPiao_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomHEndTouPiao(JSContext *cx, JS::HandleObject global) {
    jsb_RoomHEndTouPiao_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomHEndTouPiao_class->name = "RoomHEndTouPiao";
    jsb_RoomHEndTouPiao_class->addProperty = JS_PropertyStub;
    jsb_RoomHEndTouPiao_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomHEndTouPiao_class->getProperty = JS_PropertyStub;
    jsb_RoomHEndTouPiao_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomHEndTouPiao_class->enumerate = JS_EnumerateStub;
    jsb_RoomHEndTouPiao_class->resolve = JS_ResolveStub;
    jsb_RoomHEndTouPiao_class->convert = JS_ConvertStub;
    jsb_RoomHEndTouPiao_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("initWithJsonStr", js_wb_RoomHEndTouPiao_initWithJsonStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("updateTime", js_wb_RoomHEndTouPiao_updateTime, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("updateState", js_wb_RoomHEndTouPiao_updateState, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomHEndTouPiao_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCallback", js_wb_RoomHEndTouPiao_setCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCallback", js_wb_RoomHEndTouPiao_getCallback, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonCallback", js_wb_RoomHEndTouPiao_buttonCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setDefaultTips", js_wb_RoomHEndTouPiao_setDefaultTips, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomHEndTouPiao_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomHEndTouPiao_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomHEndTouPiao_class,
        dummy_constructor<RoomHEndTouPiao>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomHEndTouPiao_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomHEndTouPiao"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomHEndTouPiao>(cx, jsb_RoomHEndTouPiao_class, proto, parent_proto);
}

JSClass  *jsb_MsgModel_class;
JSObject *jsb_MsgModel_prototype;

bool js_wb_MsgModel_handler_cmd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_handler_cmd : Invalid Native Object");
    if (argc == 3) {
        int arg0 = 0;
        std::string arg1;
        long long arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_long_long(cx, args.get(2), &arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_handler_cmd : Error processing arguments");
        cobj->handler_cmd(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_handler_cmd : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_MsgModel_sendText(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendText : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendText : Error processing arguments");
        cobj->sendText(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendText : Error processing arguments");
        cobj->sendText(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendText : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_goout_group(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_goout_group : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_goout_group : Error processing arguments");
        cobj->goout_group(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_goout_group : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_start(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_start : Invalid Native Object");
    if (argc == 0) {
        cobj->start();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_start : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_MsgModel_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_init : Invalid Native Object");
    if (argc == 0) {
        cobj->init();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_MsgModel_sendSound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendSound : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendSound : Error processing arguments");
        cobj->sendSound(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        int arg1 = 0;
        int arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendSound : Error processing arguments");
        cobj->sendSound(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendSound : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_MsgModel_notifyLogout(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_notifyLogout : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_notifyLogout : Error processing arguments");
        cobj->notifyLogout(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_notifyLogout : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_getData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_getData : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_getData : Error processing arguments");
        cobj->getData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_getData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_sendImg_group(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendImg_group : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendImg_group : Error processing arguments");
        cobj->sendImg_group(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendImg_group : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_notificationReflash(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_notificationReflash : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_notificationReflash : Error processing arguments");
        cobj->notificationReflash(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_notificationReflash : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_stop(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_stop : Invalid Native Object");
    if (argc == 0) {
        cobj->stop();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_stop : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_MsgModel_sendText_group(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendText_group : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendText_group : Error processing arguments");
        cobj->sendText_group(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendText_group : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_initAccountData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_initAccountData : Invalid Native Object");
    if (argc == 0) {
        cobj->initAccountData();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_initAccountData : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_MsgModel_join_group(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_join_group : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_join_group : Error processing arguments");
        cobj->join_group(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_join_group : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_doHeartbeat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_doHeartbeat : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_doHeartbeat : Error processing arguments");
        cobj->doHeartbeat(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_doHeartbeat : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_sendImg(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendImg : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendImg : Error processing arguments");
        cobj->sendImg(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendImg : Error processing arguments");
        cobj->sendImg(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendImg : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_MsgModel_sendSound_group(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_sendSound_group : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_sendSound_group : Error processing arguments");
        cobj->sendSound_group(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_MsgModel_sendSound_group : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_MsgModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = MsgModel::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<MsgModel>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "MsgModel"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_MsgModel_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_MsgModel_get_curChatUID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_curChatUID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curChatUID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_curChatUID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_curChatUID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_curChatUID : Error processing new value");
    cobj->curChatUID = arg0;
    return true;
}
bool js_wb_MsgModel_get_ip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_ip : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->ip);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_ip(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_ip : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_ip : Error processing new value");
    cobj->ip = arg0;
    return true;
}
bool js_wb_MsgModel_get_port(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_port : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->port);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_port(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_port : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_port : Error processing new value");
    cobj->port = arg0;
    return true;
}
bool js_wb_MsgModel_get_curGroupID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_curGroupID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curGroupID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_curGroupID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_curGroupID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_curGroupID : Error processing new value");
    cobj->curGroupID = arg0;
    return true;
}
bool js_wb_MsgModel_get_isShowBeOfflineDialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_isShowBeOfflineDialog : Invalid Native Object");

    jsval jsret = BOOLEAN_TO_JSVAL(cobj->isShowBeOfflineDialog);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_isShowBeOfflineDialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_isShowBeOfflineDialog : Invalid Native Object");

    bool ok = true;
    bool arg0;
    arg0 = JS::ToBoolean(args.get(0));
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_isShowBeOfflineDialog : Error processing new value");
    cobj->isShowBeOfflineDialog = arg0;
    return true;
}
bool js_wb_MsgModel_get_connect_count(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_connect_count : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->connect_count);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_connect_count(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_connect_count : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_connect_count : Error processing new value");
    cobj->connect_count = arg0;
    return true;
}
bool js_wb_MsgModel_get_lastHeartBeatTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_lastHeartBeatTime : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->lastHeartBeatTime);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_lastHeartBeatTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_lastHeartBeatTime : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_lastHeartBeatTime : Error processing new value");
    cobj->lastHeartBeatTime = arg0;
    return true;
}
bool js_wb_MsgModel_get_curUID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_get_curUID : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->curUID);
    args.rval().set(jsret);
    return true;
}
bool js_wb_MsgModel_set_curUID(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    MsgModel* cobj = (MsgModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_MsgModel_set_curUID : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_MsgModel_set_curUID : Error processing new value");
    cobj->curUID = arg0;
    return true;
}

void js_register_wb_MsgModel(JSContext *cx, JS::HandleObject global) {
    jsb_MsgModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_MsgModel_class->name = "MsgModel";
    jsb_MsgModel_class->addProperty = JS_PropertyStub;
    jsb_MsgModel_class->delProperty = JS_DeletePropertyStub;
    jsb_MsgModel_class->getProperty = JS_PropertyStub;
    jsb_MsgModel_class->setProperty = JS_StrictPropertyStub;
    jsb_MsgModel_class->enumerate = JS_EnumerateStub;
    jsb_MsgModel_class->resolve = JS_ResolveStub;
    jsb_MsgModel_class->convert = JS_ConvertStub;
    jsb_MsgModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("curChatUID", js_wb_MsgModel_get_curChatUID, js_wb_MsgModel_set_curChatUID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("ip", js_wb_MsgModel_get_ip, js_wb_MsgModel_set_ip, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("port", js_wb_MsgModel_get_port, js_wb_MsgModel_set_port, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curGroupID", js_wb_MsgModel_get_curGroupID, js_wb_MsgModel_set_curGroupID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("isShowBeOfflineDialog", js_wb_MsgModel_get_isShowBeOfflineDialog, js_wb_MsgModel_set_isShowBeOfflineDialog, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("connect_count", js_wb_MsgModel_get_connect_count, js_wb_MsgModel_set_connect_count, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("lastHeartBeatTime", js_wb_MsgModel_get_lastHeartBeatTime, js_wb_MsgModel_set_lastHeartBeatTime, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("curUID", js_wb_MsgModel_get_curUID, js_wb_MsgModel_set_curUID, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("handler_cmd", js_wb_MsgModel_handler_cmd, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendText", js_wb_MsgModel_sendText, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("goout_group", js_wb_MsgModel_goout_group, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("start", js_wb_MsgModel_start, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_MsgModel_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendSound", js_wb_MsgModel_sendSound, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notifyLogout", js_wb_MsgModel_notifyLogout, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getData", js_wb_MsgModel_getData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendImg_group", js_wb_MsgModel_sendImg_group, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationReflash", js_wb_MsgModel_notificationReflash, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stop", js_wb_MsgModel_stop, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendText_group", js_wb_MsgModel_sendText_group, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initAccountData", js_wb_MsgModel_initAccountData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("join_group", js_wb_MsgModel_join_group, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("doHeartbeat", js_wb_MsgModel_doHeartbeat, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendImg", js_wb_MsgModel_sendImg, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendSound_group", js_wb_MsgModel_sendSound_group, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_wb_MsgModel_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_MsgModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_MsgModel_class,
        dummy_constructor<MsgModel>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_MsgModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "MsgModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<MsgModel>(cx, jsb_MsgModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_RoomChat2_class;
JSObject *jsb_RoomChat2_prototype;

bool js_wb_RoomChat2_hide(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_hide : Invalid Native Object");
    if (argc == 0) {
        cobj->hide();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_hide : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomChat2_show(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_show : Invalid Native Object");
    if (argc == 0) {
        cobj->show();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_show : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomChat2_btnCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_btnCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomChat2_btnCallback : Error processing arguments");
        cobj->btnCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_btnCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomChat2_notificationChat(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_notificationChat : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomChat2_notificationChat : Error processing arguments");
        cobj->notificationChat(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_notificationChat : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomChat2_initList(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_initList : Invalid Native Object");
    if (argc == 0) {
        cobj->initList();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_initList : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomChat2_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomChat2_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomChat2_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomChat2_getRecordData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomChat2* cobj = (RoomChat2 *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomChat2_getRecordData : Invalid Native Object");
    if (argc == 0) {
        cobj->getRecordData();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomChat2_getRecordData : Error processing arguments");
        cobj->getRecordData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomChat2_getRecordData : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomChat2_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomChat2::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomChat2>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomChat2"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomChat2_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomChat2(JSContext *cx, JS::HandleObject global) {
    jsb_RoomChat2_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomChat2_class->name = "RoomChat2";
    jsb_RoomChat2_class->addProperty = JS_PropertyStub;
    jsb_RoomChat2_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomChat2_class->getProperty = JS_PropertyStub;
    jsb_RoomChat2_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomChat2_class->enumerate = JS_EnumerateStub;
    jsb_RoomChat2_class->resolve = JS_ResolveStub;
    jsb_RoomChat2_class->convert = JS_ConvertStub;
    jsb_RoomChat2_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("hide", js_wb_RoomChat2_hide, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("show", js_wb_RoomChat2_show, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("btnCallback", js_wb_RoomChat2_btnCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationChat", js_wb_RoomChat2_notificationChat, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initList", js_wb_RoomChat2_initList, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomChat2_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomChat2_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getRecordData", js_wb_RoomChat2_getRecordData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomChat2_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomChat2_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomChat2_class,
        dummy_constructor<RoomChat2>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomChat2_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomChat2"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomChat2>(cx, jsb_RoomChat2_class, proto, parent_proto);
}

JSClass  *jsb_RoomHelpH_class;
JSObject *jsb_RoomHelpH_prototype;

bool js_wb_RoomHelpH_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHelpH* cobj = (RoomHelpH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHelpH_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHelpH_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHelpH_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHelpH* cobj = (RoomHelpH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHelpH_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHelpH_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHelpH_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomHelpH_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomHelpH::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomHelpH>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomHelpH"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomHelpH_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomHelpH(JSContext *cx, JS::HandleObject global) {
    jsb_RoomHelpH_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomHelpH_class->name = "RoomHelpH";
    jsb_RoomHelpH_class->addProperty = JS_PropertyStub;
    jsb_RoomHelpH_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomHelpH_class->getProperty = JS_PropertyStub;
    jsb_RoomHelpH_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomHelpH_class->enumerate = JS_EnumerateStub;
    jsb_RoomHelpH_class->resolve = JS_ResolveStub;
    jsb_RoomHelpH_class->convert = JS_ConvertStub;
    jsb_RoomHelpH_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("init", js_wb_RoomHelpH_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomHelpH_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomHelpH_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomHelpH_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomHelpH_class,
        dummy_constructor<RoomHelpH>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomHelpH_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomHelpH"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomHelpH>(cx, jsb_RoomHelpH_class, proto, parent_proto);
}

JSClass  *jsb_RoomHExitDialog_class;
JSObject *jsb_RoomHExitDialog_prototype;

bool js_wb_RoomHExitDialog_show(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_show : Invalid Native Object");
    if (argc == 0) {
        cobj->show();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_show : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHExitDialog_buttonClicked(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_buttonClicked : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHExitDialog_buttonClicked : Error processing arguments");
        cobj->buttonClicked(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_buttonClicked : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHExitDialog_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHExitDialog_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomHExitDialog_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHExitDialog_setCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_setCallback : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomHExitDialog_setCallback : Error processing arguments");
        cobj->setCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_setCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomHExitDialog_getCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomHExitDialog* cobj = (RoomHExitDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomHExitDialog_getCallback : Invalid Native Object");
    if (argc == 0) {
        MyCallBack_Str ret = cobj->getCallback();
        jsval jsret = JSVAL_NULL;
        #pragma warning NO CONVERSION FROM NATIVE FOR std::function;
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomHExitDialog_getCallback : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomHExitDialog_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomHExitDialog::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomHExitDialog>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomHExitDialog"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomHExitDialog_create : wrong number of arguments");
    return false;
}

bool js_wb_RoomHExitDialog_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    RoomHExitDialog* cobj = new (std::nothrow) RoomHExitDialog();

    js_type_class_t *typeClass = js_get_type_from_native<RoomHExitDialog>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "RoomHExitDialog"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomHExitDialog(JSContext *cx, JS::HandleObject global) {
    jsb_RoomHExitDialog_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomHExitDialog_class->name = "RoomHExitDialog";
    jsb_RoomHExitDialog_class->addProperty = JS_PropertyStub;
    jsb_RoomHExitDialog_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomHExitDialog_class->getProperty = JS_PropertyStub;
    jsb_RoomHExitDialog_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomHExitDialog_class->enumerate = JS_EnumerateStub;
    jsb_RoomHExitDialog_class->resolve = JS_ResolveStub;
    jsb_RoomHExitDialog_class->convert = JS_ConvertStub;
    jsb_RoomHExitDialog_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("show", js_wb_RoomHExitDialog_show, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonClicked", js_wb_RoomHExitDialog_buttonClicked, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomHExitDialog_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomHExitDialog_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCallback", js_wb_RoomHExitDialog_setCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCallback", js_wb_RoomHExitDialog_getCallback, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomHExitDialog_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomHExitDialog_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomHExitDialog_class,
        js_wb_RoomHExitDialog_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomHExitDialog_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomHExitDialog"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomHExitDialog>(cx, jsb_RoomHExitDialog_class, proto, parent_proto);
}

JSClass  *jsb_RoomRealTimeScore_class;
JSObject *jsb_RoomRealTimeScore_prototype;

bool js_wb_RoomRealTimeScore_hide(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_hide : Invalid Native Object");
    if (argc == 0) {
        cobj->hide();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_hide : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRealTimeScore_inviteFriend(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_inviteFriend : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRealTimeScore_inviteFriend : Error processing arguments");
        cobj->inviteFriend(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_inviteFriend : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomRealTimeScore_show(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_show : Invalid Native Object");
    if (argc == 0) {
        cobj->show();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_show : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRealTimeScore_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRealTimeScore_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomRealTimeScore_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRealTimeScore_myInit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_myInit : Invalid Native Object");
    if (argc == 0) {
        cobj->myInit();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_myInit : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRealTimeScore_addLayerTouchEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_addLayerTouchEvent : Invalid Native Object");
    if (argc == 0) {
        cobj->addLayerTouchEvent();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_addLayerTouchEvent : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRealTimeScore_getData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRealTimeScore* cobj = (RoomRealTimeScore *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRealTimeScore_getData : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRealTimeScore_getData : Error processing arguments");
        cobj->getData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRealTimeScore_getData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomRealTimeScore_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomRealTimeScore::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomRealTimeScore>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomRealTimeScore"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomRealTimeScore_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomRealTimeScore(JSContext *cx, JS::HandleObject global) {
    jsb_RoomRealTimeScore_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomRealTimeScore_class->name = "RoomRealTimeScore";
    jsb_RoomRealTimeScore_class->addProperty = JS_PropertyStub;
    jsb_RoomRealTimeScore_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomRealTimeScore_class->getProperty = JS_PropertyStub;
    jsb_RoomRealTimeScore_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomRealTimeScore_class->enumerate = JS_EnumerateStub;
    jsb_RoomRealTimeScore_class->resolve = JS_ResolveStub;
    jsb_RoomRealTimeScore_class->convert = JS_ConvertStub;
    jsb_RoomRealTimeScore_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("hide", js_wb_RoomRealTimeScore_hide, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("inviteFriend", js_wb_RoomRealTimeScore_inviteFriend, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("show", js_wb_RoomRealTimeScore_show, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomRealTimeScore_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomRealTimeScore_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("myInit", js_wb_RoomRealTimeScore_myInit, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("addLayerTouchEvent", js_wb_RoomRealTimeScore_addLayerTouchEvent, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getData", js_wb_RoomRealTimeScore_getData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomRealTimeScore_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomRealTimeScore_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomRealTimeScore_class,
        dummy_constructor<RoomRealTimeScore>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomRealTimeScore_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomRealTimeScore"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomRealTimeScore>(cx, jsb_RoomRealTimeScore_class, proto, parent_proto);
}

JSClass  *jsb_RoomRule_class;
JSObject *jsb_RoomRule_prototype;

bool js_wb_RoomRule_initHelpWeb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_initHelpWeb : Invalid Native Object");
    if (argc == 0) {
        cobj->initHelpWeb();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_initHelpWeb : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRule_selectBtnDraw(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_selectBtnDraw : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        bool arg1;
        int arg2 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRule_selectBtnDraw : Error processing arguments");
        cocos2d::ui::Button* ret = cobj->selectBtnDraw(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::ui::Button>(cx, (cocos2d::ui::Button*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_selectBtnDraw : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_wb_RoomRule_initHelp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_initHelp : Invalid Native Object");
    if (argc == 0) {
        cobj->initHelp();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_initHelp : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRule_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRule_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomRule_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomRule_createRichText(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_createRichText : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRule_createRichText : Error processing arguments");
        cocos2d::ui::RichText* ret = cobj->createRichText(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<cocos2d::ui::RichText>(cx, (cocos2d::ui::RichText*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_createRichText : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomRule_buttonCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomRule* cobj = (RoomRule *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomRule_buttonCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomRule_buttonCallback : Error processing arguments");
        cobj->buttonCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomRule_buttonCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomRule_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomRule::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomRule>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomRule"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomRule_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomRule(JSContext *cx, JS::HandleObject global) {
    jsb_RoomRule_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomRule_class->name = "RoomRule";
    jsb_RoomRule_class->addProperty = JS_PropertyStub;
    jsb_RoomRule_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomRule_class->getProperty = JS_PropertyStub;
    jsb_RoomRule_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomRule_class->enumerate = JS_EnumerateStub;
    jsb_RoomRule_class->resolve = JS_ResolveStub;
    jsb_RoomRule_class->convert = JS_ConvertStub;
    jsb_RoomRule_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("initHelpWeb", js_wb_RoomRule_initHelpWeb, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("selectBtnDraw", js_wb_RoomRule_selectBtnDraw, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initHelp", js_wb_RoomRule_initHelp, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomRule_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomRule_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createRichText", js_wb_RoomRule_createRichText, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonCallback", js_wb_RoomRule_buttonCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomRule_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomRule_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomRule_class,
        dummy_constructor<RoomRule>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomRule_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomRule"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomRule>(cx, jsb_RoomRule_class, proto, parent_proto);
}

JSClass  *jsb_RoomSettingH_class;
JSObject *jsb_RoomSettingH_prototype;

bool js_wb_RoomSettingH_initSetting(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomSettingH* cobj = (RoomSettingH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomSettingH_initSetting : Invalid Native Object");
    if (argc == 0) {
        cobj->initSetting();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomSettingH_initSetting : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomSettingH_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomSettingH* cobj = (RoomSettingH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomSettingH_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomSettingH_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_RoomSettingH_buttonClicked(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomSettingH* cobj = (RoomSettingH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomSettingH_buttonClicked : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomSettingH_buttonClicked : Error processing arguments");
        cobj->buttonClicked(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomSettingH_buttonClicked : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_RoomSettingH_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    RoomSettingH* cobj = (RoomSettingH *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_RoomSettingH_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_RoomSettingH_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_RoomSettingH_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_RoomSettingH_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = RoomSettingH::create();
        js_type_class_t *typeClass = js_get_type_from_native<RoomSettingH>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "RoomSettingH"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_RoomSettingH_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_RoomSettingH(JSContext *cx, JS::HandleObject global) {
    jsb_RoomSettingH_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_RoomSettingH_class->name = "RoomSettingH";
    jsb_RoomSettingH_class->addProperty = JS_PropertyStub;
    jsb_RoomSettingH_class->delProperty = JS_DeletePropertyStub;
    jsb_RoomSettingH_class->getProperty = JS_PropertyStub;
    jsb_RoomSettingH_class->setProperty = JS_StrictPropertyStub;
    jsb_RoomSettingH_class->enumerate = JS_EnumerateStub;
    jsb_RoomSettingH_class->resolve = JS_ResolveStub;
    jsb_RoomSettingH_class->convert = JS_ConvertStub;
    jsb_RoomSettingH_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("initSetting", js_wb_RoomSettingH_initSetting, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_RoomSettingH_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonClicked", js_wb_RoomSettingH_buttonClicked, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_RoomSettingH_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_RoomSettingH_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_RoomSettingH_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_RoomSettingH_class,
        dummy_constructor<RoomSettingH>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_RoomSettingH_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "RoomSettingH"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<RoomSettingH>(cx, jsb_RoomSettingH_class, proto, parent_proto);
}

JSClass  *jsb_EndRoomTipsDialog_class;
JSObject *jsb_EndRoomTipsDialog_prototype;

bool js_wb_EndRoomTipsDialog_init1(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_init1 : Invalid Native Object");
    if (argc == 0) {
        cobj->init1();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_init1 : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_EndRoomTipsDialog_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_EndRoomTipsDialog_onTouchBegan(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_onTouchBegan : Invalid Native Object");
    if (argc == 2) {
        cocos2d::Touch* arg0 = nullptr;
        cocos2d::Event* arg1 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Touch*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (args.get(1).isNull()) { arg1 = nullptr; break; }
            if (!args.get(1).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(1).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (cocos2d::Event*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_EndRoomTipsDialog_onTouchBegan : Error processing arguments");
        bool ret = cobj->onTouchBegan(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_onTouchBegan : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_EndRoomTipsDialog_buttonClicked(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_buttonClicked : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_EndRoomTipsDialog_buttonClicked : Error processing arguments");
        cobj->buttonClicked(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_buttonClicked : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_EndRoomTipsDialog_setCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_setCallback : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_EndRoomTipsDialog_setCallback : Error processing arguments");
        cobj->setCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_setCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_EndRoomTipsDialog_init2(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_init2 : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::string arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_EndRoomTipsDialog_init2 : Error processing arguments");
        cobj->init2(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        bool arg2;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        arg2 = JS::ToBoolean(args.get(2));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_EndRoomTipsDialog_init2 : Error processing arguments");
        cobj->init2(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_init2 : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_EndRoomTipsDialog_getCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    EndRoomTipsDialog* cobj = (EndRoomTipsDialog *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_EndRoomTipsDialog_getCallback : Invalid Native Object");
    if (argc == 0) {
        MyCallBack_Str ret = cobj->getCallback();
        jsval jsret = JSVAL_NULL;
        #pragma warning NO CONVERSION FROM NATIVE FOR std::function;
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_getCallback : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_EndRoomTipsDialog_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = EndRoomTipsDialog::create();
        js_type_class_t *typeClass = js_get_type_from_native<EndRoomTipsDialog>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "EndRoomTipsDialog"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_EndRoomTipsDialog_create : wrong number of arguments");
    return false;
}

bool js_wb_EndRoomTipsDialog_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    EndRoomTipsDialog* cobj = new (std::nothrow) EndRoomTipsDialog();

    js_type_class_t *typeClass = js_get_type_from_native<EndRoomTipsDialog>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "EndRoomTipsDialog"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_EndRoomTipsDialog(JSContext *cx, JS::HandleObject global) {
    jsb_EndRoomTipsDialog_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_EndRoomTipsDialog_class->name = "EndRoomTipsDialog";
    jsb_EndRoomTipsDialog_class->addProperty = JS_PropertyStub;
    jsb_EndRoomTipsDialog_class->delProperty = JS_DeletePropertyStub;
    jsb_EndRoomTipsDialog_class->getProperty = JS_PropertyStub;
    jsb_EndRoomTipsDialog_class->setProperty = JS_StrictPropertyStub;
    jsb_EndRoomTipsDialog_class->enumerate = JS_EnumerateStub;
    jsb_EndRoomTipsDialog_class->resolve = JS_ResolveStub;
    jsb_EndRoomTipsDialog_class->convert = JS_ConvertStub;
    jsb_EndRoomTipsDialog_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("init1", js_wb_EndRoomTipsDialog_init1, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_EndRoomTipsDialog_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onTouchBegan", js_wb_EndRoomTipsDialog_onTouchBegan, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("buttonClicked", js_wb_EndRoomTipsDialog_buttonClicked, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCallback", js_wb_EndRoomTipsDialog_setCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init2", js_wb_EndRoomTipsDialog_init2, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCallback", js_wb_EndRoomTipsDialog_getCallback, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_EndRoomTipsDialog_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_EndRoomTipsDialog_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_EndRoomTipsDialog_class,
        js_wb_EndRoomTipsDialog_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_EndRoomTipsDialog_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "EndRoomTipsDialog"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<EndRoomTipsDialog>(cx, jsb_EndRoomTipsDialog_class, proto, parent_proto);
}

JSClass  *jsb_SoundModel_class;
JSObject *jsb_SoundModel_prototype;

bool js_wb_SoundModel_reset(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_reset : Invalid Native Object");
    if (argc == 0) {
        cobj->reset();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_reset : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_SoundModel_clear(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_clear : Invalid Native Object");
    if (argc == 0) {
        cobj->clear();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_clear : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_SoundModel_onChatStopEvent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_onChatStopEvent : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_SoundModel_onChatStopEvent : Error processing arguments");
        cobj->onChatStopEvent(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_onChatStopEvent : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_SoundModel_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_init : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_SoundModel_checkNext(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_checkNext : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_SoundModel_checkNext : Error processing arguments");
        cobj->checkNext(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_checkNext : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_SoundModel_addSound(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_addSound : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_SoundModel_addSound : Error processing arguments");
        cobj->addSound(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_SoundModel_addSound : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_SoundModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = SoundModel::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<SoundModel>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "SoundModel"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_SoundModel_getInstance : wrong number of arguments");
    return false;
}

bool js_wb_SoundModel_get_can_play(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_get_can_play : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->can_play);
    args.rval().set(jsret);
    return true;
}
bool js_wb_SoundModel_set_can_play(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SoundModel* cobj = (SoundModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_SoundModel_set_can_play : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_wb_SoundModel_set_can_play : Error processing new value");
    cobj->can_play = arg0;
    return true;
}

void js_register_wb_SoundModel(JSContext *cx, JS::HandleObject global) {
    jsb_SoundModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_SoundModel_class->name = "SoundModel";
    jsb_SoundModel_class->addProperty = JS_PropertyStub;
    jsb_SoundModel_class->delProperty = JS_DeletePropertyStub;
    jsb_SoundModel_class->getProperty = JS_PropertyStub;
    jsb_SoundModel_class->setProperty = JS_StrictPropertyStub;
    jsb_SoundModel_class->enumerate = JS_EnumerateStub;
    jsb_SoundModel_class->resolve = JS_ResolveStub;
    jsb_SoundModel_class->convert = JS_ConvertStub;
    jsb_SoundModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("can_play", js_wb_SoundModel_get_can_play, js_wb_SoundModel_set_can_play, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("reset", js_wb_SoundModel_reset, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clear", js_wb_SoundModel_clear, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onChatStopEvent", js_wb_SoundModel_onChatStopEvent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_SoundModel_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("checkNext", js_wb_SoundModel_checkNext, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("addSound", js_wb_SoundModel_addSound, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_wb_SoundModel_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_SoundModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_SoundModel_class,
        dummy_constructor<SoundModel>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_SoundModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "SoundModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<SoundModel>(cx, jsb_SoundModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_AudioMgr_class;
JSObject *jsb_AudioMgr_prototype;

bool js_wb_AudioMgr_preloadBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_preloadBackgroundMusic : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_preloadBackgroundMusic : Error processing arguments");
        cobj->preloadBackgroundMusic(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_preloadBackgroundMusic : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_stopAllEffects(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_stopAllEffects : Invalid Native Object");
    if (argc == 0) {
        cobj->stopAllEffects();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_stopAllEffects : Error processing arguments");
        cobj->stopAllEffects(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_stopAllEffects : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_end(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_end : Invalid Native Object");
    if (argc == 0) {
        cobj->end();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_end : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_playBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_playBackgroundMusic : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_playBackgroundMusic : Error processing arguments");
        cobj->playBackgroundMusic(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_playBackgroundMusic : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_setFinishCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_setFinishCallback : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::function<void (int, const std::basic_string<char> &)> arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        do {
		    if(JS_TypeOfValue(cx, args.get(1)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(1), args.thisv()));
		        auto lambda = [=](int larg0, const std::basic_string<char> & larg1) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[2];
		            largv[0] = int32_to_jsval(cx, larg0);
		            largv[1] = std_string_to_jsval(cx, larg1);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(2, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg1 = lambda;
		    }
		    else
		    {
		        arg1 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_setFinishCallback : Error processing arguments");
        cobj->setFinishCallback(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_setFinishCallback : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_AudioMgr_setIsPlayEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_setIsPlayEffect : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_setIsPlayEffect : Error processing arguments");
        cobj->setIsPlayEffect(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_setIsPlayEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_stopBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_stopBackgroundMusic : Invalid Native Object");
    if (argc == 0) {
        cobj->stopBackgroundMusic();
        args.rval().setUndefined();
        return true;
    }
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_stopBackgroundMusic : Error processing arguments");
        cobj->stopBackgroundMusic(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_stopBackgroundMusic : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_pauseBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_pauseBackgroundMusic : Invalid Native Object");
    if (argc == 0) {
        cobj->pauseBackgroundMusic();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_pauseBackgroundMusic : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_preloadEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_preloadEffect : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_preloadEffect : Error processing arguments");
        cobj->preloadEffect(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_preloadEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_resumeBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_resumeBackgroundMusic : Invalid Native Object");
    if (argc == 0) {
        cobj->resumeBackgroundMusic();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_resumeBackgroundMusic : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_resumeAllEffects(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_resumeAllEffects : Invalid Native Object");
    if (argc == 0) {
        cobj->resumeAllEffects();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_resumeAllEffects : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_playEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_playEffect : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_playEffect : Error processing arguments");
        int ret = cobj->playEffect(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        bool arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_playEffect : Error processing arguments");
        int ret = cobj->playEffect(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_playEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_getIsPlayMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_getIsPlayMusic : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsPlayMusic();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_getIsPlayMusic : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_setEffectsVolume(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_setEffectsVolume : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_setEffectsVolume : Error processing arguments");
        cobj->setEffectsVolume(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_setEffectsVolume : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_stopEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_stopEffect : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_stopEffect : Error processing arguments");
        cobj->stopEffect(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        int arg0 = 0;
        bool arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        arg1 = JS::ToBoolean(args.get(1));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_stopEffect : Error processing arguments");
        cobj->stopEffect(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_stopEffect : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_setBackgroundMusicVolume(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_setBackgroundMusicVolume : Invalid Native Object");
    if (argc == 1) {
        double arg0 = 0;
        ok &= JS::ToNumber( cx, args.get(0), &arg0) && !std::isnan(arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_setBackgroundMusicVolume : Error processing arguments");
        cobj->setBackgroundMusicVolume(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_setBackgroundMusicVolume : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_pauseAllEffects(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_pauseAllEffects : Invalid Native Object");
    if (argc == 0) {
        cobj->pauseAllEffects();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_pauseAllEffects : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_setIsPlayMusic(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_setIsPlayMusic : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_wb_AudioMgr_setIsPlayMusic : Error processing arguments");
        cobj->setIsPlayMusic(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_setIsPlayMusic : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_AudioMgr_getIsPlayEffect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    AudioMgr* cobj = (AudioMgr *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_AudioMgr_getIsPlayEffect : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsPlayEffect();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_AudioMgr_getIsPlayEffect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_AudioMgr_destroyInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        AudioMgr::destroyInstance();
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_wb_AudioMgr_destroyInstance : wrong number of arguments");
    return false;
}

bool js_wb_AudioMgr_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        AudioMgr* ret = AudioMgr::getInstance();
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<AudioMgr>(cx, (AudioMgr*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_AudioMgr_getInstance : wrong number of arguments");
    return false;
}


void js_register_wb_AudioMgr(JSContext *cx, JS::HandleObject global) {
    jsb_AudioMgr_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_AudioMgr_class->name = "AudioMgr";
    jsb_AudioMgr_class->addProperty = JS_PropertyStub;
    jsb_AudioMgr_class->delProperty = JS_DeletePropertyStub;
    jsb_AudioMgr_class->getProperty = JS_PropertyStub;
    jsb_AudioMgr_class->setProperty = JS_StrictPropertyStub;
    jsb_AudioMgr_class->enumerate = JS_EnumerateStub;
    jsb_AudioMgr_class->resolve = JS_ResolveStub;
    jsb_AudioMgr_class->convert = JS_ConvertStub;
    jsb_AudioMgr_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("preloadBackgroundMusic", js_wb_AudioMgr_preloadBackgroundMusic, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stopAllEffects", js_wb_AudioMgr_stopAllEffects, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("end", js_wb_AudioMgr_end, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playBackgroundMusic", js_wb_AudioMgr_playBackgroundMusic, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setFinishCallback", js_wb_AudioMgr_setFinishCallback, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setIsPlayEffect", js_wb_AudioMgr_setIsPlayEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stopBackgroundMusic", js_wb_AudioMgr_stopBackgroundMusic, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pauseBackgroundMusic", js_wb_AudioMgr_pauseBackgroundMusic, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("preloadEffect", js_wb_AudioMgr_preloadEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resumeBackgroundMusic", js_wb_AudioMgr_resumeBackgroundMusic, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resumeAllEffects", js_wb_AudioMgr_resumeAllEffects, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playEffect", js_wb_AudioMgr_playEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsPlayMusic", js_wb_AudioMgr_getIsPlayMusic, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setEffectsVolume", js_wb_AudioMgr_setEffectsVolume, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stopEffect", js_wb_AudioMgr_stopEffect, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setBackgroundMusicVolume", js_wb_AudioMgr_setBackgroundMusicVolume, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pauseAllEffects", js_wb_AudioMgr_pauseAllEffects, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setIsPlayMusic", js_wb_AudioMgr_setIsPlayMusic, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsPlayEffect", js_wb_AudioMgr_getIsPlayEffect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("destroyInstance", js_wb_AudioMgr_destroyInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getInstance", js_wb_AudioMgr_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_AudioMgr_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_AudioMgr_class,
        dummy_constructor<AudioMgr>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_AudioMgr_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "AudioMgr"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<AudioMgr>(cx, jsb_AudioMgr_class, proto, JS::NullPtr());
}

JSClass  *jsb_DataBase_class;
JSObject *jsb_DataBase_prototype;

bool js_wb_DataBase_insert(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    DataBase* cobj = (DataBase *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_DataBase_insert : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_DataBase_insert : Error processing arguments");
        bool ret = cobj->insert(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_DataBase_insert : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_DataBase_update(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    DataBase* cobj = (DataBase *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_DataBase_update : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_DataBase_update : Error processing arguments");
        bool ret = cobj->update(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_DataBase_update : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_DataBase_select(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    DataBase* cobj = (DataBase *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_DataBase_select : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_DataBase_select : Error processing arguments");
        std::string ret = cobj->select(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_DataBase_select : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_DataBase_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_DataBase_getInstance : Error processing arguments");

        DataBase* ret = DataBase::getInstance(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<DataBase>(cx, (DataBase*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_wb_DataBase_getInstance : wrong number of arguments");
    return false;
}


void js_register_wb_DataBase(JSContext *cx, JS::HandleObject global) {
    jsb_DataBase_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_DataBase_class->name = "DataBase";
    jsb_DataBase_class->addProperty = JS_PropertyStub;
    jsb_DataBase_class->delProperty = JS_DeletePropertyStub;
    jsb_DataBase_class->getProperty = JS_PropertyStub;
    jsb_DataBase_class->setProperty = JS_StrictPropertyStub;
    jsb_DataBase_class->enumerate = JS_EnumerateStub;
    jsb_DataBase_class->resolve = JS_ResolveStub;
    jsb_DataBase_class->convert = JS_ConvertStub;
    jsb_DataBase_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("insert", js_wb_DataBase_insert, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("update", js_wb_DataBase_update, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("select", js_wb_DataBase_select, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_wb_DataBase_getInstance, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_DataBase_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_DataBase_class,
        dummy_constructor<DataBase>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_DataBase_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "DataBase"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<DataBase>(cx, jsb_DataBase_class, proto, JS::NullPtr());
}

JSClass  *jsb_ChatFriendsBottom_class;
JSObject *jsb_ChatFriendsBottom_prototype;

bool js_wb_ChatFriendsBottom_checkContent(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_checkContent : Invalid Native Object");
    if (argc == 0) {
        cobj->checkContent();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_checkContent : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ChatFriendsBottom_setBottomType(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_setBottomType : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_setBottomType : Error processing arguments");
        cobj->setBottomType(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_setBottomType : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_btnCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_btnCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_btnCallback : Error processing arguments");
        cobj->btnCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_btnCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_setCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_setCallback : Invalid Native Object");
    if (argc == 1) {
        std::function<void (std::basic_string<char>)> arg0;
        do {
		    if(JS_TypeOfValue(cx, args.get(0)) == JSTYPE_FUNCTION)
		    {
		        JS::RootedObject jstarget(cx, args.thisv().toObjectOrNull());
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, jstarget, args.get(0), args.thisv()));
		        auto lambda = [=](std::basic_string<char> larg0) -> void {
		            JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
		            jsval largv[1];
		            largv[0] = std_string_to_jsval(cx, larg0);
		            JS::RootedValue rval(cx);
		            bool succeed = func->invoke(1, &largv[0], &rval);
		            if (!succeed && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_setCallback : Error processing arguments");
        cobj->setCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_setCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_editBoxEditingDidBegin(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_editBoxEditingDidBegin : Invalid Native Object");
    if (argc == 1) {
        cocos2d::ui::EditBox* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::ui::EditBox*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_editBoxEditingDidBegin : Error processing arguments");
        cobj->editBoxEditingDidBegin(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_editBoxEditingDidBegin : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_editBoxReturn(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_editBoxReturn : Invalid Native Object");
    if (argc == 1) {
        cocos2d::ui::EditBox* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::ui::EditBox*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_editBoxReturn : Error processing arguments");
        cobj->editBoxReturn(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_editBoxReturn : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ChatFriendsBottom_editBoxTextChanged(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_editBoxTextChanged : Invalid Native Object");
    if (argc == 2) {
        cocos2d::ui::EditBox* arg0 = nullptr;
        std::string arg1;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::ui::EditBox*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_editBoxTextChanged : Error processing arguments");
        cobj->editBoxTextChanged(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_editBoxTextChanged : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_wb_ChatFriendsBottom_getCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_getCallback : Invalid Native Object");
    if (argc == 0) {
        MyCallBack_Str ret = cobj->getCallback();
        jsval jsret = JSVAL_NULL;
        #pragma warning NO CONVERSION FROM NATIVE FOR std::function;
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_getCallback : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ChatFriendsBottom_editBoxEditingDidEnd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_editBoxEditingDidEnd : Invalid Native Object");
    if (argc == 1) {
        cocos2d::ui::EditBox* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::ui::EditBox*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_editBoxEditingDidEnd : Error processing arguments");
        cobj->editBoxEditingDidEnd(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_editBoxEditingDidEnd : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_faceCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_faceCallback : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_faceCallback : Error processing arguments");
        cobj->faceCallback(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_faceCallback : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_scollToView(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_scollToView : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_scollToView : Error processing arguments");
        cobj->scollToView(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_scollToView : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_notificationReflash(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatFriendsBottom* cobj = (ChatFriendsBottom *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatFriendsBottom_notificationReflash : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatFriendsBottom_notificationReflash : Error processing arguments");
        cobj->notificationReflash(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatFriendsBottom_notificationReflash : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatFriendsBottom_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = ChatFriendsBottom::create();
        js_type_class_t *typeClass = js_get_type_from_native<ChatFriendsBottom>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "ChatFriendsBottom"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_ChatFriendsBottom_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_ChatFriendsBottom(JSContext *cx, JS::HandleObject global) {
    jsb_ChatFriendsBottom_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_ChatFriendsBottom_class->name = "ChatFriendsBottom";
    jsb_ChatFriendsBottom_class->addProperty = JS_PropertyStub;
    jsb_ChatFriendsBottom_class->delProperty = JS_DeletePropertyStub;
    jsb_ChatFriendsBottom_class->getProperty = JS_PropertyStub;
    jsb_ChatFriendsBottom_class->setProperty = JS_StrictPropertyStub;
    jsb_ChatFriendsBottom_class->enumerate = JS_EnumerateStub;
    jsb_ChatFriendsBottom_class->resolve = JS_ResolveStub;
    jsb_ChatFriendsBottom_class->convert = JS_ConvertStub;
    jsb_ChatFriendsBottom_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("checkContent", js_wb_ChatFriendsBottom_checkContent, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setBottomType", js_wb_ChatFriendsBottom_setBottomType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("btnCallback", js_wb_ChatFriendsBottom_btnCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCallback", js_wb_ChatFriendsBottom_setCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("editBoxEditingDidBegin", js_wb_ChatFriendsBottom_editBoxEditingDidBegin, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("editBoxReturn", js_wb_ChatFriendsBottom_editBoxReturn, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_ChatFriendsBottom_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("editBoxTextChanged", js_wb_ChatFriendsBottom_editBoxTextChanged, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCallback", js_wb_ChatFriendsBottom_getCallback, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("editBoxEditingDidEnd", js_wb_ChatFriendsBottom_editBoxEditingDidEnd, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("faceCallback", js_wb_ChatFriendsBottom_faceCallback, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("scollToView", js_wb_ChatFriendsBottom_scollToView, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("notificationReflash", js_wb_ChatFriendsBottom_notificationReflash, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_ChatFriendsBottom_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_ChatFriendsBottom_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_ChatFriendsBottom_class,
        dummy_constructor<ChatFriendsBottom>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_ChatFriendsBottom_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "ChatFriendsBottom"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<ChatFriendsBottom>(cx, jsb_ChatFriendsBottom_class, proto, parent_proto);
}

JSClass  *jsb_ChatRecordEffect_class;
JSObject *jsb_ChatRecordEffect_prototype;

bool js_wb_ChatRecordEffect_recordError(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordError : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordError : Error processing arguments");
        cobj->recordError(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordError : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_recordCancel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordCancel : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordCancel : Error processing arguments");
        cobj->recordCancel(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordCancel : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_onExit(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_onExit : Invalid Native Object");
    if (argc == 0) {
        cobj->onExit();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_onExit : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ChatRecordEffect_recordStart(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordStart : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordStart : Error processing arguments");
        cobj->recordStart(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordStart : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_recordOver(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordOver : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordOver : Error processing arguments");
        cobj->recordOver(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordOver : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_recordIng(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordIng : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordIng : Error processing arguments");
        cobj->recordIng(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordIng : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_init : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->init();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_wb_ChatRecordEffect_playNoFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_playNoFile : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_playNoFile : Error processing arguments");
        cobj->playNoFile(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_playNoFile : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_recordStop(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordStop : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordStop : Error processing arguments");
        cobj->recordStop(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordStop : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_recordTimeOut(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    ChatRecordEffect* cobj = (ChatRecordEffect *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_wb_ChatRecordEffect_recordTimeOut : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Ref* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::Ref*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_wb_ChatRecordEffect_recordTimeOut : Error processing arguments");
        cobj->recordTimeOut(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_wb_ChatRecordEffect_recordTimeOut : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_wb_ChatRecordEffect_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = ChatRecordEffect::create();
        js_type_class_t *typeClass = js_get_type_from_native<ChatRecordEffect>(ret);
        JS::RootedObject jsret(cx, jsb_ref_autoreleased_create_jsobject(cx, ret, typeClass, "ChatRecordEffect"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_wb_ChatRecordEffect_create : wrong number of arguments");
    return false;
}


extern JSObject *jsb_cocos2d_Layer_prototype;

void js_register_wb_ChatRecordEffect(JSContext *cx, JS::HandleObject global) {
    jsb_ChatRecordEffect_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_ChatRecordEffect_class->name = "ChatRecordEffect";
    jsb_ChatRecordEffect_class->addProperty = JS_PropertyStub;
    jsb_ChatRecordEffect_class->delProperty = JS_DeletePropertyStub;
    jsb_ChatRecordEffect_class->getProperty = JS_PropertyStub;
    jsb_ChatRecordEffect_class->setProperty = JS_StrictPropertyStub;
    jsb_ChatRecordEffect_class->enumerate = JS_EnumerateStub;
    jsb_ChatRecordEffect_class->resolve = JS_ResolveStub;
    jsb_ChatRecordEffect_class->convert = JS_ConvertStub;
    jsb_ChatRecordEffect_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("recordError", js_wb_ChatRecordEffect_recordError, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordCancel", js_wb_ChatRecordEffect_recordCancel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onExit", js_wb_ChatRecordEffect_onExit, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordStart", js_wb_ChatRecordEffect_recordStart, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordOver", js_wb_ChatRecordEffect_recordOver, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordIng", js_wb_ChatRecordEffect_recordIng, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_wb_ChatRecordEffect_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playNoFile", js_wb_ChatRecordEffect_playNoFile, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordStop", js_wb_ChatRecordEffect_recordStop, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("recordTimeOut", js_wb_ChatRecordEffect_recordTimeOut, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_wb_ChatRecordEffect_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JS::RootedObject parent_proto(cx, jsb_cocos2d_Layer_prototype);
    jsb_ChatRecordEffect_prototype = JS_InitClass(
        cx, global,
        parent_proto,
        jsb_ChatRecordEffect_class,
        dummy_constructor<ChatRecordEffect>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_ChatRecordEffect_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "ChatRecordEffect"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<ChatRecordEffect>(cx, jsb_ChatRecordEffect_class, proto, parent_proto);
}

void register_all_wb(JSContext* cx, JS::HandleObject obj) {
    // Get the global ns
    JS::RootedObject ns(cx, ScriptingCore::getInstance()->getGlobalObject());

    js_register_wb_RoomRealTimeScore(cx, ns);
    js_register_wb_RoomChat2(cx, ns);
    js_register_wb_CCHttpAgent(cx, ns);
    js_register_wb_JsUtils(cx, ns);
    js_register_wb_RoomPublic(cx, ns);
    js_register_wb_JPacket(cx, ns);
    js_register_wb_EndRoomTipsDialog(cx, ns);
    js_register_wb_AudioMgr(cx, ns);
    js_register_wb_ZJHModel(cx, ns);
    js_register_wb_RoomSettingH(cx, ns);
    js_register_wb_PlayerModel(cx, ns);
    js_register_wb_BaseLayer(cx, ns);
    js_register_wb_ChatFriendsBottom(cx, ns);
    js_register_wb_Sound(cx, ns);
    js_register_wb_PlatformHelper(cx, ns);
    js_register_wb_SoundModel(cx, ns);
    js_register_wb_ChatRecordEffect(cx, ns);
    js_register_wb_RoomRule(cx, ns);
    js_register_wb_CCTcpClient(cx, ns);
    js_register_wb_DataBase(cx, ns);
    js_register_wb_MsgModel(cx, ns);
    js_register_wb_RoomHEndTouPiao(cx, ns);
    js_register_wb_RoomHelpH(cx, ns);
    js_register_wb_CCTcpResponse(cx, ns);
    js_register_wb_RoomHExitDialog(cx, ns);
}

