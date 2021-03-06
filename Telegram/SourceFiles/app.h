/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2016 John Preston, https://desktop.telegram.org
*/
#pragma once

#include "types.h"

class AppClass;
class Window;
class MainWidget;
class SettingsWidget;
class ApiWrap;
class Font;
class Color;
class FileUploader;

#include "history.h"
#include "layout.h"

typedef QMap<HistoryItem*, NullType> HistoryItemsMap;
typedef QHash<PhotoData*, HistoryItemsMap> PhotoItems;
typedef QHash<DocumentData*, HistoryItemsMap> DocumentItems;
typedef QHash<WebPageData*, HistoryItemsMap> WebPageItems;
typedef QHash<int32, HistoryItemsMap> SharedContactItems;
typedef QHash<ClipReader*, HistoryItem*> GifItems;

typedef QHash<PhotoId, PhotoData*> PhotosData;
typedef QHash<DocumentId, DocumentData*> DocumentsData;

struct ReplyMarkup {
	ReplyMarkup(int32 flags = 0) : flags(flags) {
	}
	typedef QList<QList<QString> > Commands;
	Commands commands;
	int32 flags;
};

class LayeredWidget;

namespace App {
	AppClass *app();
	Window *wnd();
	MainWidget *main();
	SettingsWidget *settings();
	bool passcoded();
	FileUploader *uploader();
	ApiWrap *api();

	void logOut();
	bool loggedOut();

	QString formatPhone(QString phone);

	int32 onlineForSort(UserData *user, int32 now);
	int32 onlineWillChangeIn(UserData *user, int32 nowOnServer);
	QString onlineText(UserData *user, int32 nowOnServer, bool precise = false);
	bool onlineColorUse(UserData *user, int32 now);

	UserData *feedUsers(const MTPVector<MTPUser> &users, bool emitPeerUpdated = true); // returns last user
	PeerData *feedChats(const MTPVector<MTPChat> &chats, bool emitPeerUpdated = true); // returns last chat
	void feedParticipants(const MTPChatParticipants &p, bool requestBotInfos, bool emitPeerUpdated = true);
	void feedParticipantAdd(const MTPDupdateChatParticipantAdd &d, bool emitPeerUpdated = true);
	void feedParticipantDelete(const MTPDupdateChatParticipantDelete &d, bool emitPeerUpdated = true);
	void feedChatAdmins(const MTPDupdateChatAdmins &d, bool emitPeerUpdated = true);
	void feedParticipantAdmin(const MTPDupdateChatParticipantAdmin &d, bool emitPeerUpdated = true);
	bool checkEntitiesAndViewsUpdate(const MTPDmessage &m); // returns true if item found and it is not detached
	void updateEditedMessage(const MTPDmessage &m);
	void addSavedGif(DocumentData *doc);
	void checkSavedGif(HistoryItem *item);
	void feedMsgs(const QVector<MTPMessage> &msgs, NewMessageType type);
	void feedMsgs(const MTPVector<MTPMessage> &msgs, NewMessageType type);
	void feedInboxRead(const PeerId &peer, MsgId upTo);
	void feedOutboxRead(const PeerId &peer, MsgId upTo);
	void feedWereDeleted(ChannelId channelId, const QVector<MTPint> &msgsIds);
	void feedUserLinks(const MTPVector<MTPcontacts_Link> &links, bool emitPeerUpdated = true);
	void feedUserLink(MTPint userId, const MTPContactLink &myLink, const MTPContactLink &foreignLink, bool emitPeerUpdated = true);

	void markPeerUpdated(PeerData *data);
	void clearPeerUpdated(PeerData *data);
	void emitPeerUpdated();

	ImagePtr image(const MTPPhotoSize &size);
	StorageImageLocation imageLocation(int32 w, int32 h, const MTPFileLocation &loc);
	StorageImageLocation imageLocation(const MTPPhotoSize &size);

	PhotoData *feedPhoto(const MTPPhoto &photo, const PreparedPhotoThumbs &thumbs);
	PhotoData *feedPhoto(const MTPPhoto &photo, PhotoData *convert = 0);
	PhotoData *feedPhoto(const MTPDphoto &photo, PhotoData *convert = 0);
	DocumentData *feedDocument(const MTPdocument &document, const QPixmap &thumb);
	DocumentData *feedDocument(const MTPdocument &document, DocumentData *convert = 0);
	DocumentData *feedDocument(const MTPDdocument &document, DocumentData *convert = 0);
	WebPageData *feedWebPage(const MTPDwebPage &webpage, WebPageData *convert = 0);
	WebPageData *feedWebPage(const MTPDwebPagePending &webpage, WebPageData *convert = 0);
	WebPageData *feedWebPage(const MTPWebPage &webpage);

	PeerData *peerLoaded(const PeerId &id);
	UserData *userLoaded(const PeerId &id);
	ChatData *chatLoaded(const PeerId &id);
	ChannelData *channelLoaded(const PeerId &id);
	UserData *userLoaded(int32 user);
	ChatData *chatLoaded(int32 chat);
	ChannelData *channelLoaded(int32 channel);

	PeerData *peer(const PeerId &id);
	UserData *user(const PeerId &id);
	ChatData *chat(const PeerId &id);
	ChannelData *channel(const PeerId &id);
	UserData *user(int32 user_id);
	ChatData *chat(int32 chat_id);
	ChannelData *channel(int32 channel_id);
	UserData *self();
	PeerData *peerByName(const QString &username);
	QString peerName(const PeerData *peer, bool forDialogs = false);
	PhotoData *photo(const PhotoId &photo);
	PhotoData *photoSet(const PhotoId &photo, PhotoData *convert, const uint64 &access, int32 date, const ImagePtr &thumb, const ImagePtr &medium, const ImagePtr &full);
	DocumentData *document(const DocumentId &document);
	DocumentData *documentSet(const DocumentId &document, DocumentData *convert, const uint64 &access, int32 date, const QVector<MTPDocumentAttribute> &attributes, const QString &mime, const ImagePtr &thumb, int32 dc, int32 size, const StorageImageLocation &thumbLocation);
	WebPageData *webPage(const WebPageId &webPage);
	WebPageData *webPageSet(const WebPageId &webPage, WebPageData *convert, const QString &, const QString &url, const QString &displayUrl, const QString &siteName, const QString &title, const QString &description, PhotoData *photo, DocumentData *doc, int32 duration, const QString &author, int32 pendingTill);
	ImageLinkData *imageLink(const QString &imageLink);
	ImageLinkData *imageLinkSet(const QString &imageLink, ImageLinkType type);
	void forgetMedia();

	MTPPhoto photoFromUserPhoto(MTPint userId, MTPint date, const MTPUserProfilePhoto &photo);

	Histories &histories();
	History *history(const PeerId &peer);
	History *historyFromDialog(const PeerId &peer, int32 unreadCnt, int32 maxInboxRead);
	History *historyLoaded(const PeerId &peer);
	HistoryItem *histItemById(ChannelId channelId, MsgId itemId);
	inline HistoryItem *histItemById(const FullMsgId &msgId) {
		return histItemById(msgId.channel, msgId.msg);
	}
	void historyRegItem(HistoryItem *item);
	void historyItemDetached(HistoryItem *item);
	void historyUnregItem(HistoryItem *item);
	void historyClearMsgs();
	void historyClearItems();
	void historyRegReply(HistoryReply *reply, HistoryItem *to);
	void historyUnregReply(HistoryReply *reply, HistoryItem *to);

	void historyRegRandom(uint64 randomId, const FullMsgId &itemId);
	void historyUnregRandom(uint64 randomId);
	FullMsgId histItemByRandom(uint64 randomId);
	void historyRegSentData(uint64 randomId, const PeerId &peerId, const QString &text);
	void historyUnregSentData(uint64 randomId);
	void histSentDataByItem(uint64 randomId, PeerId &peerId, QString &text);

	void hoveredItem(HistoryItem *item);
	HistoryItem *hoveredItem();
	void pressedItem(HistoryItem *item);
	HistoryItem *pressedItem();
	void hoveredLinkItem(HistoryItem *item);
	HistoryItem *hoveredLinkItem();
	void pressedLinkItem(HistoryItem *item);
	HistoryItem *pressedLinkItem();
	void contextItem(HistoryItem *item);
	HistoryItem *contextItem();
	void mousedItem(HistoryItem *item);
	HistoryItem *mousedItem();

	const style::font &monofont();
	const QPixmap &sprite();
	const QPixmap &emoji();
	const QPixmap &emojiLarge();
	const QPixmap &emojiSingle(EmojiPtr emoji, int32 fontHeight);

	void clearHistories();

	void initMedia();
	void deinitMedia();
	void playSound();

	void checkImageCacheSize();

	bool isValidPhone(QString phone);

	void quit();
	bool quiting();
	void setQuiting();

	QImage readImage(QByteArray data, QByteArray *format = 0, bool opaque = true, bool *animated = 0);
	QImage readImage(const QString &file, QByteArray *format = 0, bool opaque = true, bool *animated = 0, QByteArray *content = 0);

	void regPhotoItem(PhotoData *data, HistoryItem *item);
	void unregPhotoItem(PhotoData *data, HistoryItem *item);
	const PhotoItems &photoItems();
	const PhotosData &photosData();

	void regDocumentItem(DocumentData *data, HistoryItem *item);
	void unregDocumentItem(DocumentData *data, HistoryItem *item);
	const DocumentItems &documentItems();
	const DocumentsData &documentsData();

	void regWebPageItem(WebPageData *data, HistoryItem *item);
	void unregWebPageItem(WebPageData *data, HistoryItem *item);
	const WebPageItems &webPageItems();

	void regSharedContactItem(int32 userId, HistoryItem *item);
	void unregSharedContactItem(int32 userId, HistoryItem *item);
	const SharedContactItems &sharedContactItems();
	QString phoneFromSharedContact(int32 userId);

	void regGifItem(ClipReader *reader, HistoryItem *item);
	void unregGifItem(ClipReader *reader);
	void stopGifItems();

	void regMuted(PeerData *peer, int32 changeIn);
	void unregMuted(PeerData *peer);
	void updateMuted();

	void regInlineResultLoader(FileLoader *loader, InlineResult *result);
	void unregInlineResultLoader(FileLoader *loader);
	InlineResult *inlineResultFromLoader(FileLoader *loader);

	void feedReplyMarkup(ChannelId channelId, MsgId msgId, const MTPReplyMarkup &markup);
	void clearReplyMarkup(ChannelId channelId, MsgId msgId);
	const ReplyMarkup &replyMarkup(ChannelId channelId, MsgId msgId);

	void setProxySettings(QNetworkAccessManager &manager);
	QNetworkProxy getHttpProxySettings();
	void setProxySettings(QTcpSocket &socket);

	QImage **cornersMask();
	void roundRect(Painter &p, int32 x, int32 y, int32 w, int32 h, const style::color &bg, RoundCorners index, const style::color *sh = 0);
	inline void roundRect(Painter &p, const QRect &rect, const style::color &bg, RoundCorners index, const style::color *sh = 0) {
		return roundRect(p, rect.x(), rect.y(), rect.width(), rect.height(), bg, index, sh);
	}
	void roundShadow(Painter &p, int32 x, int32 y, int32 w, int32 h, const style::color &sh, RoundCorners index);
	inline void roundShadow(Painter &p, const QRect &rect, const style::color &sh, RoundCorners index) {
		return roundShadow(p, rect.x(), rect.y(), rect.width(), rect.height(), sh, index);
	}
	void roundRect(Painter &p, int32 x, int32 y, int32 w, int32 h, const style::color &bg);
	inline void roundRect(Painter &p, const QRect &rect, const style::color &bg) {
		return roundRect(p, rect.x(), rect.y(), rect.width(), rect.height(), bg);
	}

	void initBackground(int32 id = DefaultChatBackground, const QImage &p = QImage(), bool nowrite = false);

	const style::color &msgServiceBg();
	const style::color &msgServiceSelectBg();
	const style::color &historyScrollBarColor();
	const style::color &historyScrollBgColor();
	const style::color &historyScrollBarOverColor();
	const style::color &historyScrollBgOverColor();
	const style::color &introPointHoverColor();

	struct WallPaper {
		WallPaper(int32 id, ImagePtr thumb, ImagePtr full) : id(id), thumb(thumb), full(full) {
		}
		int32 id;
		ImagePtr thumb;
		ImagePtr full;
	};
	typedef QList<WallPaper> WallPapers;
	DeclareSetting(WallPapers, ServerBackgrounds);

};

#include "facades.h"
