package com.example.root.nativeappdemo

import android.app.Activity
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val TAG = MainActivity::class.java.simpleName
    private val gpioProcessor = GpioProcessor()
    private var led: GpioProcessor.Gpio? = null
    private var isHigh = false
    private val handler = Handler()
    private val runnable = object : Runnable {
        override fun run() {
            if (isHigh)
                led!!.high()
            else
                led!!.low()
            isHigh = !isHigh
            handler.postDelayed(this,1000)
        }

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        led = gpioProcessor.pin31
        led!!.out()
        jni_action.setOnClickListener {
            /*val status = toggleLED()
            led_status.text = if (status)"ON" else "OFF"
            Log.d(TAG,"Status : $status")*/

            handler.post(runnable)

        }
    }

    /*fun callMeFromJNI(text:String){
        Log.d(TAG, "A callback from Native Library : $text")
    }*/

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    /*external fun toggleLED():Boolean
    external fun initLED():Boolean

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("demo-lib")
        }
    }*/
}