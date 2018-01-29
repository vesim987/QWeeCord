#include "qweecordchannel.h"

#include <QRegularExpression>
#include <QDiscord>
#include <QWeeChat>

#include "qweecord.h"
#include "qweecordguild.h"



QWeeCordChannel::QWeeCordChannel(QSharedPointer<QDiscordChannel> channel, QWeeCordGuild *parent) : _channel(channel), _guild(parent), QObject(parent)
{
  createBuffer();

  QList<QSharedPointer<QDiscordRole>> roles = _guild->_guild->roles();

  qSort(roles.begin(), roles.end(), [] (auto a, auto b) {
    return a->position() < b->position();
  });

  for(auto &role : roles) {
    if(role->hoist())
      _groups.insert(role->id(), weechat_nicklist_add_group(_buffer->_buffer, nullptr,
                                                            role->name().toLocal8Bit().data(),
                                                            "weechat.color.nicklist_group",
                                                            true));
      //qWarning() << role->name() << " " << role->position();
  }

  _groups.insert(QDiscordID(0), weechat_nicklist_add_group(_buffer->_buffer, nullptr,
                                                        "Online",
                                                        "weechat.color.nicklist_group",
                                                        true));
}

void QWeeCordChannel::createBuffer() {
  _buffer = new QWeeChatBuffer("channel." + _channel->id().toString());
  _buffer->setProperty("server", _guild->_guild->id().toString());
  _buffer->setProperty("type", "channel");
  _buffer->setProperty("nick", _guild->_weecord->_discord.state().user()->username());
  _buffer->setProperty("channel", _channel->id().toString());
  _buffer->set("title", _channel->topic());
  _buffer->set("nicklist", "1");

  if(_channel->type() == QDiscordChannel::Type::Category)
    _buffer ->set("short_name", "[" + _channel->name() + "]");
  else
    _buffer ->set("short_name", "  #" + _channel->name());

  QObject::connect(_buffer, &QWeeChatBuffer::onInput, this, &QWeeCordChannel::onInput);
}

void QWeeCordChannel::onInput(const QString &input) {
  QDiscordMessage::create(_guild->_weecord->_discord.rest(), _channel->id(), input);
}

void QWeeCordChannel::onMessage(const QDiscordMessage &message) {
  QString tags = "notify_message,localecho,no_highlight";

  QString content = "";
  for(auto &attachment : message.attachments()) {
    content += attachment.url() + " ";
  }

  content +=  message.content();

  for(auto &mention : message.mentions()) {
    content.replace(mention.mention(), "@" + mention.username());
  }

  QRegularExpression emotRegex("<(:\\w+:)\\d+>");
  QRegularExpressionMatchIterator it = emotRegex.globalMatch(content);
  while(it.hasNext()) {
    auto match = it.next();
    content.replace(match.captured(0), match.captured(1));
  }


  QString author = message.author().username();

  //auto member = _guild->_guild->member(message.author().id());

  //if(member)
    //author = member->nickname().value_or(member->user().username());

  message.author().id();

  _buffer->printf(author + "\t" +  content, tags, message.timestamp());

}

void QWeeCordChannel::onNewGuildMember(QSharedPointer<QDiscordMember> member)
{
  _buffer->set("nicklist", "true");
  weechat_nicklist_add_nick(_buffer->_buffer, _groups[QDiscordID(0)],
                            member->nickname().value_or(member->user().username()).toStdString().c_str(),
                            "bar_fg",
                            "", "lightgreen", true);
}
