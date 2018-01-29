#ifndef QWEECORD_H
#define QWEECORD_H

#include <QObject>
#include <QDiscord>
#include <QWeeChat>
#include "qweecordguild.h"


class QWeeCord : public QObject
{
  Q_OBJECT
public:
  QWeeCord();

private slots:
  void onMessage(const QDiscordMessage &message);

  void qweecordCommand();

private:
public:
  QDiscord _discord;
  QMap<QDiscordID, QWeeCordGuild*> _guilds_map;
  QMap<QDiscordID, QWeeCordChannel*> _dms_map;
  QWeeChatConfig *_config;
};

#endif  // QWEECORD_H
