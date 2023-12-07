package com.agil.agilv8

import android.app.Application

class AgilV8App : Application() {

    override fun onCreate() {
        super.onCreate()
        mInstance = this
    }

    companion object {
        private lateinit var mInstance: AgilV8App

        fun getInstance(): AgilV8App {
            return mInstance
        }
    }
}