/*
    qqprotocol.h - Kopete QQ Protocol

    Copyright (c) 2014      by Jun Zhang             <jun.zhang@i-soft.com.cn>
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

#ifndef TESTBEDPROTOCOL_H
#define TESTBEDPROTOCOL_H

#include <kopeteprotocol.h>
#include <QVariant>



/**
 * Encapsulates the generic actions associated with this protocol
 * @author Will Stephenson
 */
class QQProtocol : public Kopete::Protocol
{
	Q_OBJECT
public:
	QQProtocol(QObject *parent, const QVariantList &args);
	~QQProtocol();
	
	//Online Statuses, respect to Lwqq status
	const Kopete::OnlineStatus QQLogout;
	const Kopete::OnlineStatus QQOnline;
	const Kopete::OnlineStatus QQOffline;
	const Kopete::OnlineStatus QQAway;
	const Kopete::OnlineStatus QQHidden;
	const Kopete::OnlineStatus QQBusy;
	const Kopete::OnlineStatus QQCallme;
	//const Kopete::OnlineStatus QQInvisible;
	
	/*used to indicate connecting*/
	const Kopete::OnlineStatus QQConnecting;
	
	/*property*/
	const Kopete::PropertyTmpl 	iconCheckSum;
	const Kopete::PropertyTmpl	propNick;
	const Kopete::PropertyTmpl	propLongNick;
	const Kopete::PropertyTmpl	propGender;
	const Kopete::PropertyTmpl	propBirthday;
	const Kopete::PropertyTmpl	propProvince;
	const Kopete::PropertyTmpl	propPhone;
	const Kopete::PropertyTmpl	propSchool;
	const Kopete::PropertyTmpl	propEmail;
	const Kopete::PropertyTmpl	propXingzuo;
	const Kopete::PropertyTmpl	propCountry;
	const Kopete::PropertyTmpl	propCity;
	const Kopete::PropertyTmpl	propMobile;
	const Kopete::PropertyTmpl	propPage;
	const Kopete::PropertyTmpl	propJob;
	const Kopete::PropertyTmpl	propPersonal;
	/**
	 * Convert the serialised data back into a QQContact and add this
	 * to its Kopete::MetaContact
	 */
	virtual Kopete::Contact *deserializeContact(
			Kopete::MetaContact *metaContact,
			const QMap< QString, QString > & serializedData,
			const QMap< QString, QString > & addressBookData
		);
	/**
	 * Generate the widget needed to add QQContacts
	 */
	virtual AddContactPage * createAddContactWidget( QWidget *parent, Kopete::Account *account );
	/**
	 * Generate the widget needed to add/edit accounts for this protocol
	 */
	virtual KopeteEditAccountWidget * createEditAccountWidget( Kopete::Account *account, QWidget *parent );
	/**
	 * Generate a QQAccount
	 */
	virtual Kopete::Account * createNewAccount( const QString &accountId );
	/**
	 * Access the instance of this protocol
	 */
	static QQProtocol *protocol();

protected:
	static QQProtocol *s_protocol;
};

#endif
