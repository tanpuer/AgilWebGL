package com.agil.agilv8

import android.content.Context
import android.util.AttributeSet
import android.view.Choreographer
import android.view.SurfaceHolder
import android.view.SurfaceView

class AgilV8View @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : SurfaceView(context, attrs), SurfaceHolder.Callback, Choreographer.FrameCallback {

    init {
        holder.addCallback(this)
    }

    private var engine: AgilV8Engine? = null
    private var created = false
    private var start = System.currentTimeMillis()

    override fun surfaceCreated(holder: SurfaceHolder) {
        created = true
        if (engine != null) {
            engine?.create(holder.surface)
        } else {
            engine = AgilV8Engine()
            engine?.create(holder.surface)
//            engine.executeJS("demo.js", "demo")
            engine?.executeJS("primitive.js", "primitive")
        }
        Choreographer.getInstance().postFrameCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        engine?.change(width, height, System.currentTimeMillis() - start)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        Choreographer.getInstance().removeFrameCallback(this)
        created = false
        engine?.destroy()
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (created) {
            engine?.doFrame(System.currentTimeMillis() - start)
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    fun release() {
        engine?.release()
    }

}