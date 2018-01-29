#include "qweecordcategory.h"

#include <QWeeChat>

QWeeCordCategory::QWeeCordCategory(QSharedPointer<QDiscordChannel> channel, QWeeCordGuild *parent)
  : QWeeCordChannel(channel, parent)
{
}

void QWeeCordCategory::addChannel(QWeeCordChannel *channel)
{
  _channels.push_back(channel);
}
