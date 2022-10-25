package de.jurihock.aubergine

import android.app.Service
import android.content.Intent
import android.os.IBinder

abstract class AudioService : Service() {

  abstract override fun onBind(intent: Intent?): IBinder?

  override fun onDestroy() {
    super.onDestroy()
  }

}
