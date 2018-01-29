#ifndef QWEECORDCATEGORY_H
#define QWEECORDCATEGORY_H

#include <QObject>

#include "qweecordchannel.h"

class QWeeCordGuild;

class QWeeCordCategory : public QWeeCordChannel
{
  Q_OBJECT
public:
  explicit QWeeCordCategory(QSharedPointer<QDiscordChannel> channel, QWeeCordGuild *parent = nullptr);

  void addChannel(QWeeCordChannel *channel);
signals:

public slots:
private:
  QList<QWeeCordChannel*> _channels;
};

#endif // QWEECORDCATEGORY_H
