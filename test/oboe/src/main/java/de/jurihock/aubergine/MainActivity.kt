package de.jurihock.aubergine

import androidx.appcompat.app.AppCompatActivity

import android.app.Activity
import android.os.Bundle
import android.widget.ToggleButton

class MainActivity : Activity() { // AppCompatActivity

  var test : Test = Test()
  var pipe : Long = 0L

  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)

    var button = ToggleButton(this)

    button.isPressed = false
    button.text = "START"

    button.setOnClickListener {
      if (pipe == 0L) {
        button.isPressed = true
        button.text = "STOP"
        pipe = test.start()
      } else {
        button.isPressed = false
        button.text = "START"
        pipe = test.stop(pipe)
      }
    }

    setContentView(button)
  }

}
