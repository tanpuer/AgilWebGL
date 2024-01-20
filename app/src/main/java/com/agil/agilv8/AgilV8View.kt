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

    private val engine = AgilV8Engine()
    private var created = false
    private var start = System.currentTimeMillis()
    private var executed = false

    override fun surfaceCreated(holder: SurfaceHolder) {
        engine.create(holder.surface)
        created = true
        Choreographer.getInstance().postFrameCallback(this)
        if (!executed) {
            executed = true
//            engine.executeJS("demo.js", "demo")
            engine.executeJS("primitive.js", "primitive")
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        engine.change(width, height, System.currentTimeMillis() - start)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        Choreographer.getInstance().removeFrameCallback(this)
        created = false
        engine.destroy()
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (created) {
            engine.doFrame(System.currentTimeMillis() - start)
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    fun release() {
        engine.release()
        executed = false
    }

}