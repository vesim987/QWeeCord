#ifndef QWEECORDCHANNEL_H
#define QWEECORDCHANNEL_H

#include <QObject>
#include <QDiscord>

#include "weechat/weechat-plugin.h"

class QWeeCordGuild;
class QWeeChatBuffer;

class QWeeCordChannel : public QObject
{
  Q_OBJECT
public:
  explicit QWeeCordChannel(QSharedPointer<QDiscordChannel> channel, QWeeCordGuild *parent);
  void createBuffer();
signals:

public slots:
  void onInput(const QString &input);
  void onMessage(const QDiscordMessage &message);
  void onNewGuildMember(QSharedPointer<QDiscordMember> member);

private:
  QWeeChatBuffer *_buffer;
  QSharedPointer<QDiscordChannel> _channel;
  QWeeCordGuild *_guild;
  QMap<QDiscordID, t_gui_nick_group*> _groups;
};

#endif // QWEECORDCHANNEL_H
