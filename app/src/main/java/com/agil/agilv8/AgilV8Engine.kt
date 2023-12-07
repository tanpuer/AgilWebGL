package com.agil.agilv8

import android.content.res.AssetManager
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.Surface
import java.util.concurrent.atomic.AtomicBoolean

class AgilV8Engine {

    private val agilV8HandlerThread: HandlerThread =
        HandlerThread("agil-v8", Thread.MAX_PRIORITY).apply {
            start()
        }
    private val agilV8Handler = Handler(agilV8HandlerThread.looper)

    private val ctx = AgilV8App.getInstance()

    private var finishDraw = AtomicBoolean(true)

    init {
        agilV8Handler.post {
            val start = System.currentTimeMillis()
            nativeInit(ctx.assets)
            Log.d(TAG, "init cost ${System.currentTimeMillis() - start}")
        }
    }

    fun executeJS(path: String, moduleName: String) {
        agilV8Handler.post {
            nativeExecuteJS(path, moduleName)
        }
    }

    fun create(surface: Surface) {
        agilV8Handler.post {
            val start = System.currentTimeMillis()
            nativeCreate(surface)
            Log.d(TAG, "create cost ${System.currentTimeMillis() - start}")
        }
    }

    fun change(width: Int, height: Int, time: Long) {
        agilV8Handler.post {
            val start = System.currentTimeMillis()
            nativeChange(width, height, time)
            Log.d(TAG, "change cost ${System.currentTimeMillis() - start}")
        }
    }

    fun destroy() {
        agilV8Handler.post {
            nativeDestroy()
        }
    }

    fun doFrame(time: Long) {
        if (!finishDraw.get()) {
            Log.d(TAG, "doFrame ignore current vysnc draw")
            return
        }
        agilV8Handler.post {
            finishDraw.set(false)
            val start = System.currentTimeMillis()
            nativeDoFrame(time)
//            Log.d(TAG, "doFrame cost ${System.currentTimeMillis() - start}")
            finishDraw.set(true)
        }
    }

    fun release() {
        agilV8Handler.post {
            nativeRelease()
        }
        agilV8HandlerThread.quitSafely()
    }

    private external fun nativeInit(assetManager: AssetManager)
    private external fun nativeExecuteJS(path: String, moduleName: String)
    private external fun nativeCreate(surface: Surface);
    private external fun nativeChange(width: Int, height: Int, time: Long)
    private external fun nativeDestroy()
    private external fun nativeDoFrame(time: Long)
    private external fun nativeRelease()

    companion object {
        init {
            System.loadLibrary("agilV8")
        }

        private const val TAG = "AgilV8Engine"
    }

}