package de.jurihock.aubergine;

class Test {

  init {
    System.loadLibrary("de.jurihock.aubergine")
  }

  external fun hello() : String

  external fun start() : Long
  external fun stop(id : Long) : Long

}
