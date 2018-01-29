#include <QCoreApplication>
#include <QThread>
#include <QtGlobal>
#include <weechat/weechat-plugin.h>

#include <qweecord.h>

t_weechat_plugin *weechat_plugin = nullptr;

WEECHAT_PLUGIN_NAME("qweecord")
WEECHAT_PLUGIN_DESCRIPTION("Discord plugin for Weechat")
WEECHAT_PLUGIN_AUTHOR("Vesim <vesim809@gmail.com>")
WEECHAT_PLUGIN_VERSION("0.1")
WEECHAT_PLUGIN_LICENSE("GPL3")

t_gui_buffer *debug_buffer = nullptr;

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  if(type == QtDebugMsg)
    return;
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
      weechat_printf(debug_buffer, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtInfoMsg:
      weechat_printf(debug_buffer, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtWarningMsg:
      weechat_printf(debug_buffer, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtCriticalMsg:
      weechat_printf(debug_buffer, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
  case QtFatalMsg:
      weechat_printf(debug_buffer, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      abort();
  }
}



int command_qdiscord_cb(const void *pointer, void *data, struct t_gui_buffer *buffer, int argc, char **argv, char **argv_eol)
{
  Q_UNUSED(pointer);
  Q_UNUSED(data);
  Q_UNUSED(buffer);
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  Q_UNUSED(argv_eol);

  QThread *thread = new QThread;
  QThread::connect(thread, &QThread::started, [] {
    new QWeeCord;
  });
  thread->start();

  return WEECHAT_RC_OK;
}

QThread *qtThread = nullptr;

void intitializePlugin() {
  new QWeeCord(); //INFO: becouse i can...
}


extern "C" int weechat_plugin_init(struct t_weechat_plugin *plugin, int argc, char *argv[])
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  weechat_plugin = plugin;

  std::atomic_bool initialized(false);

  qtThread = new QThread();
  QThread::connect(qtThread, &QThread::started, [&initialized] {
    if(QCoreApplication::instance() == NULL)
    {
      char *argv[] = {"SharedLibrary", NULL};
      int argc = 1;
      QCoreApplication app(argc, argv);
      qInstallMessageHandler(myMessageHandler);
      debug_buffer = weechat_buffer_new("qweecord", NULL, NULL, NULL, NULL, NULL, NULL);
      intitializePlugin();
      initialized.store(true);
      app.exec();
    }
  });
  qtThread->start();

  while(!initialized.load()) std::this_thread::yield();

  return WEECHAT_RC_OK;
}

extern "C" int weechat_plugin_end(struct t_weechat_plugin *plugin)
{
  /* make C compiler happy */
  (void) plugin;
  QCoreApplication::instance()->quit();
  //qtThread->wait();

  return WEECHAT_RC_OK;
}
