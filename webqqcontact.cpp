/*
    webqqcontact.cpp - Kopete Webqq Protocol

    Copyright (c) 2003      by Will Stephenson		 <will@stevello.free-online.co.uk>
    Kopete    (c) 2002-2003 by the Kopete developers <kopete-devel@kde.org>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU General Public                   *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/



#include <QList>
#include <QDebug>

#include <kaction.h>
#include <kdebug.h>
#include <klocale.h>

#include "kopeteaccount.h"
#include "kopetegroup.h"
#include "kopetechatsession.h"
#include "kopeteonlinestatus.h"
#include "kopetemetacontact.h"
#include "kopetechatsessionmanager.h"
#include "kopeteuiglobal.h"
#include "kopeteview.h"
#include "kopetetransfermanager.h"
#include "kopeteavatarmanager.h"

#include "webqqaccount.h"
#include "webqqfakeserver.h"
#include "webqqprotocol.h"
#include "webqqloginverifywidget.h"
#include "webqquserinfoform.h"

#include "translate.h"

#include "webqqcontact.h"

WebqqContact::WebqqContact( Kopete::Account* _account, const QString &uniqueName,
		const QString &displayName, Kopete::MetaContact *parent )
: Kopete::Contact( _account, uniqueName, parent )
{
	kDebug( 14210 ) << " uniqueName: " << uniqueName << ", displayName: " << displayName;
	m_type = WebqqContact::Null;
	// FIXME: ? setDisplayName( displayName );
	m_msgManager = 0L;
	
	setOnlineStatus( WebqqProtocol::protocol()->WebqqOffline );
}

WebqqContact::~WebqqContact()
{
}

void WebqqContact::setType( WebqqContact::Type type )
{
	m_type = type;
}

bool WebqqContact::isReachable()
{
    return true;
}

void WebqqContact::serialize( QMap< QString, QString > &serializedData, QMap< QString, QString > & /* addressBookData */ )
{
    QString value;
	switch ( m_type )
	{
	case Null:
		value = QLatin1String("null");
	case Echo:
		value = QLatin1String("echo");
	case Group:
		value = QLatin1String("group");
	}
	serializedData[ "contactType" ] = value;
}

Kopete::ChatSession* WebqqContact::manager( CanCreateFlags canCreateFlags )
{
	kDebug( 14210 ) ;
	if ( m_msgManager )
	{
		return m_msgManager;
	}
	else if ( canCreateFlags == CanCreate )
	{
		QList<Kopete::Contact*> contacts;
		contacts.append(this);
		Kopete::ChatSession::Form form = ( m_type == Group ?
				  Kopete::ChatSession::Chatroom : Kopete::ChatSession::Small );
		m_msgManager = Kopete::ChatSessionManager::self()->create(account()->myself(), contacts, protocol(), form );
		connect(m_msgManager, SIGNAL(messageSent(Kopete::Message&,Kopete::ChatSession*)),
				this, SLOT(sendMessage(Kopete::Message&)) );
		connect(m_msgManager, SIGNAL(destroyed()), this, SLOT(slotChatSessionDestroyed()));
		return m_msgManager;
	}
	else
	{
		return 0;
	}
}


QList<KAction *> *WebqqContact::customContextMenuActions() //OBSOLETE
{
	//FIXME!!!  this function is obsolete, we should use XMLGUI instead
	/*m_actionCollection = new KActionCollection( this, "userColl" );
	m_actionPrefs = new KAction(i18n( "&Contact Settings" ), 0, this,
			SLOT(showContactSettings()), m_actionCollection, "contactSettings" );

	return m_actionCollection;*/
	return 0L;
}

void WebqqContact::showContactSettings()
{
	//WebqqContactSettings* p = new WebqqContactSettings( this );
	//p->show();
}

void WebqqContact::setDisplayPicture(const QByteArray &data)
{
	//setProperty( WebqqProtocol::protocol()->iconCheckSum, checksum );	
	
	Kopete::AvatarManager::AvatarEntry entry;
	entry.name = contactId();
	entry.category = Kopete::AvatarManager::Contact;
	entry.contact = this;
	entry.image = QImage::fromData(data);
	entry = Kopete::AvatarManager::self()->add(entry);

	if (!entry.dataPath.isNull())
	{
		setProperty( Kopete::Global::Properties::self()->photo(), QString() );
		setProperty( Kopete::Global::Properties::self()->photo() , entry.dataPath );
		//emit displayPictureChanged();
	}
}


void WebqqContact::slotUserInfo()
{
	
	WebqqUserInfoForm *form = new WebqqUserInfoForm(this);
	form->show();
}



void WebqqContact::sendMessage( Kopete::Message &message )
{
	kDebug( 14210 ) ;
	// convert to the what the server wants
	// For this 'protocol', there's nothing to do
	// send it
	/*
	static_cast<WebqqAccount *>( account() )->server()->sendMessage(
			message.to().first()->contactId(),
			message.plainBody() );
	*/
	
	/*this is for test*/
	 qDebug() << "msg:" << message.plainBody();
	//qDebug()<<
	//first prepare message
	QString targetQQNumber = message.to().first()->contactId();
	QString messageStr = message.plainBody();
	qq_send_im(targetQQNumber.toUtf8().constData(), messageStr.toUtf8().constData());
	
	// give it back to the manager to display
	manager()->appendMessage( message );
	// tell the manager it was sent successfully
	manager()->messageSucceeded();
}


static void cb_send_receipt(LwqqAsyncEvent* ev,LwqqMsg* msg,char* serv_id,char* what)
{
    printf("[%s] \n", __FUNCTION__);
    qq_account* ac = lwqq_async_event_get_owner(ev)->data; 
    LwqqMsgMessage* mmsg = (LwqqMsgMessage*)msg;
    if(lwqq_async_event_get_code(ev)==LWQQ_CALLBACK_FAILED) 
    {
       s_free(what);
      s_free(serv_id);
      lwqq_msg_free(msg);
    }
    
   

    if(ev == NULL){
        //qq_sys_msg_write(ac,msg->type,serv_id,_("Message body too long"),PURPLE_MESSAGE_ERROR,time(NULL));
    }else{
        int err = lwqq_async_event_get_result(ev);
        static char buf[1024]={0};
        //PurpleConversation* conv = find_conversation(msg->type,serv_id,ac);

        if(err > 0){
            snprintf(buf,sizeof(buf),_("Send failed, err:%d:\n%s"),err,what);
            
	    printf("FUNCTION: %s, %s\n",__FUNCTION__, buf);
	    //qq_sys_msg_write(ac, msg->type, serv_id, buf, PURPLE_MESSAGE_ERROR, time(NULL));
        }
        if(err == LWQQ_EC_LOST_CONN){
            ac->qq->action->poll_lost(ac->qq);
        }
    }
    if(mmsg->upload_retry <0)
    {
        //qq_sys_msg_write(ac, msg->type, serv_id, _("Upload content retry over limit"), PURPLE_MESSAGE_ERROR, time(NULL));
    }
    if(msg->type == LWQQ_MS_GROUP_MSG) 
      mmsg->group.group_code = NULL;
    else if(msg->type == LWQQ_MS_DISCU_MSG) 
      mmsg->discu.did = NULL;
failed:
    s_free(what);
    s_free(serv_id);
    lwqq_msg_free(msg);
}


int WebqqContact::qq_send_im( const char *who, const char *what)
{
    LwqqClient* lc = ((WebqqAccount*)account())->m_lc;
    LwqqMsg* msg;
    LwqqMsgMessage *mmsg;
    qq_account *ac = (qq_account*)(lc->data);

    msg = lwqq_msg_new(LWQQ_MS_BUDDY_MSG);
    mmsg = (LwqqMsgMessage*)msg;

    LwqqBuddy* buddy = find_buddy_by_qqnumber(lc,who);
    if(buddy)
	mmsg->super.to = s_strdup(buddy->uin);
    else 
	mmsg->super.to = s_strdup(who);
 

    mmsg->f_name = s_strdup(ac->font.family);
    mmsg->f_size = ac->font.size;
    mmsg->f_style = ac->font.style;
    strcpy(mmsg->f_color,"000000");

    translate_message_to_struct(NULL, NULL, what, msg, 0);

    LwqqAsyncEvent* ev = lwqq_msg_send(lc,mmsg);
    lwqq_async_add_event_listener(ev,_C_(4p, cb_send_receipt,ev,msg,strdup(who),strdup(what)));

    return 1;
}



void WebqqContact::receivedMessage( const QString &message )
{
	Kopete::ContactPtrList contactList;
	contactList.append( account()->myself() );
	// Create a Kopete::Message
	Kopete::Message newMessage( this, contactList );
	newMessage.setPlainBody( message );
	newMessage.setDirection( Kopete::Message::Inbound );

	// Add it to the manager
	manager(CanCreate)->appendMessage (newMessage);
}

void WebqqContact::slotChatSessionDestroyed()
{
	//FIXME: the chat window was closed?  Take appropriate steps.
	m_msgManager = 0L;
}

#include "webqqcontact.moc"

// vim: set noet ts=4 sts=4 sw=4:
