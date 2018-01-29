#ifndef QWEECORDGUILD_H
#define QWEECORDGUILD_H

#include <QObject>
#include <QDiscord>

class QWeeCord;
class QWeeChatBuffer;
class QWeeCordChannel;
class QWeeCordCategory;

class QWeeCordGuild : public QObject
{
  Q_OBJECT
public:
  QWeeCordGuild(QSharedPointer<QDiscordGuild> &guild, QWeeCord *parent = nullptr);

  void createBuffer();


signals:

public slots:
  void onMessage(const QDiscordMessage &message);
  void onNewGuildMember(QSharedPointer<QDiscordMember> member);
public:
  QSharedPointer<QDiscordGuild> &_guild;
  QWeeCord *_weecord;
  QMap<QDiscordID, QWeeCordChannel*> _channels;
  QMap<QDiscordID, QWeeCordCategory*> _categories;

  QWeeChatBuffer *_buffer;
};

#endif // QWEECORDGUILD_H
