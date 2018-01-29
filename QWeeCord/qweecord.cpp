#include "qweecord.h"


QWeeCord::QWeeCord() : _discord()
{
  QObject::connect(&_discord, &QDiscord::loggedIn, [this] { weechat_printf(NULL, "logged in"); });

  QDiscordState *state = &_discord.state();

  QObject::connect(state, &QDiscordState::guildAvailable, [this](QSharedPointer<QDiscordGuild> guild) {
    this->_guilds_map.insert(guild->id(), new QWeeCordGuild(guild, this));
    guild->listMembers(1000, [this, guild] (QList<QDiscordMember> list) { //TODO: add members limit to config
      for(auto &member : list) {
        QSharedPointer<QDiscordMember> ptr(new QDiscordMember(member));
        _discord.state().guildMemberAddReceived(ptr, guild->id());
      }
    });
  });

  QObject::connect(state, &QDiscordState::guildMemberAdded, [this](QSharedPointer<QDiscordMember> member) {
    auto guild = _guilds_map.find(member->guild()->id());
    if(guild != _guilds_map.end())
      (*guild)->onNewGuildMember(member);
  });

/*
  QObject::connect(state, &QDiscordState::guildMemberUpdated, [this](QSharedPointer<QDiscordMember> member) {

  });
*/
  QObject::connect(state, &QDiscordState::messageCreated, this, &QWeeCord::onMessage);

  /*
    QObject::connect(&_discord, &QDiscord::loggedIn, [this, state] {
      QDiscordChannel::getPrivateChannels(_discord.rest(), [this] (QList<QDiscordChannel> channels) {
        for(auto &channel : channels) {
          for(auto &user : channel.recipients()) {
            qWarning() << user->username();
          }
        }
      });
    });*/

  QObject::connect(&_discord, &QDiscord::loginFailed, [] { qWarning() << "login failed"; });
  QObject::connect(&_discord, &QDiscord::loggedOut, [] { qWarning() << "logged out"; });

  _config = new QWeeChatConfig("qweecord", this);
  auto section = _config->addSection("discord", false, false);
  auto token = section->addOption("token", "your_token");
  _config->read();

  auto qweecordCommand = new QWeeChatCommand("qweecord", "Start QWeeCord", this);
  QObject::connect(qweecordCommand, &QWeeChatCommand::onCommand, this, &QWeeCord::qweecordCommand);
}

void QWeeCord::onMessage(const QDiscordMessage &message) {
  switch(message.channel()->type()) {
  case QDiscordChannel::Type::Text:
  {
    auto guild = _guilds_map.find(message.guild()->id());
    if(guild != _guilds_map.end())
      (*guild)->onMessage(message);
  }
  break;
  case QDiscordChannel::Type::DirectMessage:
    qWarning() << "Direct message: " << message.channel()->name() << ": " << message.content();
  break;
  case QDiscordChannel::Type::GroupDirectMessage:
    qWarning() << "Group direct message: " << message.channel()->name() << ": " << message.content();
  break;
  default:
    qWarning() << "unsupported message type " << (int)message.channel()->type() << " in " << message.channel()->name();
  }
}

void QWeeCord::qweecordCommand() {
  if(!_discord.isConnected())
    _discord.login(QDiscordToken(_config->getSection("discord")->getOption("token")->value<QString>(), QDiscordToken::Type::None));
}
