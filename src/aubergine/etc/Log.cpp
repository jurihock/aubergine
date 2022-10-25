#include <aubergine/etc/Log.h>

#include <android/log.h>

INITIALIZE_EASYLOGGINGPP

#define ANDROID_LOG_TAG "AUBERGINE JNI"

class AndroidLogHandler : public el::LogDispatchCallback
{

public:

  void handle(const el::LogDispatchData* data)
  {
    const el::Level level = data->logMessage()->level();

    switch (level)
    {
      case el::Level::Verbose:
        __android_log_print(ANDROID_LOG_VERBOSE, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      case el::Level::Debug:
        __android_log_print(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      case el::Level::Info:
        __android_log_print(ANDROID_LOG_INFO, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      case el::Level::Warning:
        __android_log_print(ANDROID_LOG_WARN, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      case el::Level::Error:
        __android_log_print(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      case el::Level::Fatal:
        __android_log_print(ANDROID_LOG_FATAL, ANDROID_LOG_TAG, "%s", data->logMessage()->message().c_str());
        break;
      default:
        break;
    }
  }

  static bool install()
  {
    el::Helpers::uninstallLogDispatchCallback<el::base::DefaultLogDispatchCallback>("DefaultLogDispatchCallback");
    return el::Helpers::installLogDispatchCallback<AndroidLogHandler>("AndroidLogHandler");
  }

};

static bool IsAndroidLogHandlerInstalled = AndroidLogHandler::install();
