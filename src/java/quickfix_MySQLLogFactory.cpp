/****************************************************************************
** Copyright (c) 2001-2004 quickfixengine.org  All rights reserved.
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifdef _MSC_VER
#include "stdafx.h"
#else
#include "config.h"
#endif

#ifdef HAVE_MYSQL

#include "JVM.h"
#include "quickfix_MySQLLogFactory.h"
#include "MySQLLog.h"
#include "SessionSettings.h"
#include "CallStack.h"
#include "Conversions.h"

JNIEXPORT void JNICALL Java_quickfix_MySQLLogFactory_create__
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  FIX::process_sleep(15);
  JVM::set( pEnv );
  JVMObject object( obj );

  JVMObject jsettings( object.getObject( "settings", "Lquickfix/SessionSettings;" ) );
  FIX::SessionSettings* pSettings
  = ( FIX::SessionSettings* ) jsettings.getInt( "cppPointer" );

  FIX::LogFactory* pFactory = new FIX::MySQLLogFactory( *pSettings );
  object.setInt( "cppPointer", ( int ) pFactory );

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_MySQLLogFactory_destroy
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MySQLLogFactory* pFactory
  = ( FIX::MySQLLogFactory* ) jobject.getInt( "cppPointer" );
  delete pFactory;

  QF_STACK_CATCH
}

JNIEXPORT jobject JNICALL Java_quickfix_MySQLLogFactory_create__Lquickfix_SessionID_2
( JNIEnv *pEnv, jobject obj, jobject sessionID )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobj( obj );
  JVMObject jsession( sessionID );

  FIX::MySQLLogFactory* pFactory
  = ( FIX::MySQLLogFactory* ) jobj.getInt( "cppPointer" );
  FIX::SessionID* pSessionID
  = ( FIX::SessionID* ) jsession.getInt( "cppPointer" );

  try
  {
    FIX::Log* pLog = pFactory->create( *pSessionID );
    JVMClass type( "Lquickfix/MySQLLog;" );
    jmethodID method = pEnv->GetMethodID( type, "<init>", "(I)V" );
    jobject result = pEnv->NewObject( type, method, ( jint ) pLog );
    return result;
  }
  catch ( FIX::ConfigError & e )
  {
    throwNew( "Lquickfix/ConfigError;", e.what() );
  }
  return 0;

  QF_STACK_CATCH
}
#endif //HAVE_MYSQL