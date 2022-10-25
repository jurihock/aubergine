package de.jurihock.aubergine

import android.app.Application
import android.content.pm.PackageManager
import timber.log.Timber as Log

class Main : Application() {

  private class ModifiedDebugTree : Log.DebugTree() {
    override fun log(priority: Int, tag: String?, message: String, t: Throwable?) {
        super.log(priority, "AUBERGINE $tag", message, t)
    }
  }

  private fun features() {
    val hasLowLatencyFeature: Boolean =
        packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_LOW_LATENCY)

    val hasProFeature: Boolean =
        packageManager.hasSystemFeature(PackageManager.FEATURE_AUDIO_PRO)

    Log.i("Available audio features: " +
          "Low Latency ${ if (hasLowLatencyFeature) ":)" else ":(" } " +
          "Pro ${ if (hasProFeature) ":)" else ":(" }")
  }

  override fun onCreate() {
    super.onCreate()

    if (BuildConfig.DEBUG) {
      Log.plant(ModifiedDebugTree())
    } else {
      Log.plant(ModifiedDebugTree())
    }

    features()
  }

}
