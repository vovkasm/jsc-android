#include "unicode/utypes.h"
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unicode/udata.h>

extern "C" {
    JNIEXPORT void JNICALL Java_org_webkit_androidjsc_JscManager_init(JNIEnv* env, jobject obj, jobject assetManager) {
        AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
        AAsset* asset = AAssetManager_open(mgr, "icudt71l.dat", AASSET_MODE_BUFFER);
        const void* buffer = AAsset_getBuffer(asset);
        if (!buffer) {
            __android_log_print(ANDROID_LOG_DEBUG, "JSC", "Can't get buffer for icudt71l.dat");
            AAsset_close(asset);
            return;
        }
        UErrorCode err = U_ZERO_ERROR;
        udata_setCommonData(buffer, &err);
        if (err != U_ZERO_ERROR) {
            __android_log_print(ANDROID_LOG_DEBUG, "JSC", "Can't udata_setCommonData");
        }
    }
}
