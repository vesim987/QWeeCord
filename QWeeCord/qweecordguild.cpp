#include "qweecordguild.h"

#include <QWeeChat>
#include "qweecord.h"
#include "qweecordchannel.h"
#include "qweecordcategory.h"

QWeeCordGuild::QWeeCordGuild(QSharedPointer<QDiscordGuild> &guild, QWeeCord *parent) : _guild(guild),_weecord(parent), QObject(parent)
{
  createBuffer();
  QList<QSharedPointer<QDiscordChannel>> channels = guild->channels();

  qSort(channels.begin(), channels.end(), [] (auto a, auto b) {
    return a->position() < b->position();
  });

  //TODO: rewrite this part
  for(auto &channel : channels) {
    if(channel->type() != QDiscordChannel::Type::Text)
      continue;
    if(!channel->parentId().isNull())
      continue;

    _channels.insert(channel->id(), new QWeeCordChannel(channel, this));
    channel->getMessages(100, [this](const QList<QDiscordMessage> &list) {

      for(auto it = list.rbegin(); it != list.rend(); ++it) {
        _weecord->_discord.state().messageCreateReceived(*it);
      }
    });
  }

  for(auto &category : channels) {
    if(category->type() == QDiscordChannel::Type::Category) {
      auto cat = new QWeeCordCategory(category, this);
      for(auto &channel : channels) {
        if(channel->type() != QDiscordChannel::Type::Text)
          continue;
        if(!channel->parentId().toString().compare(category->id().toString()) == 0)
          continue;


        auto temp = new QWeeCordChannel(channel, this);
        _channels.insert(channel->id(), temp);
        cat->addChannel(temp);

        channel->getMessages(100, [this](const QList<QDiscordMessage> &list) {

          for(auto it = list.rbegin(); it != list.rend(); ++it) {
            _weecord->_discord.state().messageCreateReceived(*it);
          }
        });
      }
      //_categories.insert(category->id(), cat);
    }
  }
}

void QWeeCordGuild::createBuffer() {
  _buffer = new QWeeChatBuffer("server." + _guild->name());
  _buffer->setProperty("server", _guild->name());
  _buffer->setProperty("channel", _guild->name());
  _buffer->setProperty("plugin", "irc");
  _buffer->setProperty("nick", _weecord->_discord.state().user()->username());
  _buffer->setProperty("type", "server");
  _buffer->set("short_name", _guild->name());
}

void QWeeCordGuild::onMessage(const QDiscordMessage &message) {
  QWeeCordChannel* channel = _channels[message.channelId()];
  if(channel)
    channel->onMessage(message);
}

void QWeeCordGuild::onNewGuildMember(QSharedPointer<QDiscordMember> member) {
  for(auto &channel : _channels) {
    channel->onNewGuildMember(member);
  }
}
