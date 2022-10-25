package de.jurihock.aubergine

import android.os.Binder
import java.lang.ref.WeakReference

class AudioServiceBinder<T>(service: T) : Binder() where T : AudioService {

  private var service : WeakReference<T>

  init {
    this.service = WeakReference<T>(service)
  }

  fun getService() : T? {
    return service.get()
  }

}
