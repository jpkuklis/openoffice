/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_dbui.hxx"

#ifndef _DBU_REGHELPER_HXX_
#include "dbu_reghelper.hxx"
#endif
#ifndef _COM_SUN_STAR_DOCUMENT_XEVENTLISTENER_HPP_
#include <com/sun/star/document/XEventListener.hpp>
#endif
#ifndef _COM_SUN_STAR_CONTAINER_XSET_HPP_
#include <com/sun/star/container/XSet.hpp>
#endif
#ifndef DBAUI_DBTYPEWIZDLGSETUP_HXX
#include "DBTypeWizDlgSetup.hxx"
#endif
#ifndef DBAUI_DBWIZSETUP_HXX
#include "dbwizsetup.hxx"
#endif
#ifndef _COM_SUN_STAR_SDB_XOFFICEDATABASEDOCUMENT_HPP_
#include <com/sun/star/sdb/XOfficeDatabaseDocument.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XDATASOURCE_HPP_
#include <com/sun/star/sdbc/XDataSource.hpp>
#endif
#ifndef _SV_MSGBOX_HXX
#include <vcl/msgbox.hxx>
#endif

using namespace dbaui;
namespace css = ::com::sun::star;
                         
extern "C" void SAL_CALL createRegistryInfo_ODBTypeWizDialogSetup()
{
	static OMultiInstanceAutoRegistration< ODBTypeWizDialogSetup > aAutoRegistration;
}

//.........................................................................
namespace dbaui
{
//.........................................................................

	using namespace ::com::sun::star::uno;
	using namespace ::com::sun::star::lang;
	using namespace ::com::sun::star::beans;
    using namespace ::com::sun::star::sdb;
    using namespace ::com::sun::star::sdbc;

//=========================================================================
//-------------------------------------------------------------------------
ODBTypeWizDialogSetup::ODBTypeWizDialogSetup(const Reference< XMultiServiceFactory >& _rxORB)
	:ODatabaseAdministrationDialog(_rxORB)
    ,m_bOpenDatabase(sal_True)
	,m_bStartTableWizard(sal_False)
{
    registerProperty(::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("OpenDatabase")), 3, PropertyAttribute::TRANSIENT,
		&m_bOpenDatabase, getBooleanCppuType());

    registerProperty(::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("StartTableWizard")), 4, PropertyAttribute::TRANSIENT,
		&m_bStartTableWizard, getBooleanCppuType());
}
//-------------------------------------------------------------------------
Sequence<sal_Int8> SAL_CALL ODBTypeWizDialogSetup::getImplementationId(  ) throw(RuntimeException)
{
	static ::cppu::OImplementationId aId;
	return aId.getImplementationId();
}

//-------------------------------------------------------------------------
Reference< XInterface > SAL_CALL ODBTypeWizDialogSetup::Create(const Reference< XMultiServiceFactory >& _rxFactory)
{
	Reference < XInterface > xDBWizard = *(new ODBTypeWizDialogSetup(_rxFactory));
	return xDBWizard;
}

//-------------------------------------------------------------------------
::rtl::OUString SAL_CALL ODBTypeWizDialogSetup::getImplementationName() throw(RuntimeException)
{
	return getImplementationName_Static();
}

//-------------------------------------------------------------------------
::rtl::OUString ODBTypeWizDialogSetup::getImplementationName_Static() throw(RuntimeException)
{
	return ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("org.openoffice.comp.dbu.ODBTypeWizDialogSetup"));
}

//-------------------------------------------------------------------------
::comphelper::StringSequence SAL_CALL ODBTypeWizDialogSetup::getSupportedServiceNames() throw(RuntimeException)
{
	return getSupportedServiceNames_Static();
}

//-------------------------------------------------------------------------
::comphelper::StringSequence ODBTypeWizDialogSetup::getSupportedServiceNames_Static() throw(RuntimeException)
{
	::comphelper::StringSequence aSupported(1);
	aSupported.getArray()[0] = ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.sdb.DatabaseWizardDialog"));
	return aSupported;
}

//-------------------------------------------------------------------------
Reference<XPropertySetInfo>  SAL_CALL ODBTypeWizDialogSetup::getPropertySetInfo() throw(RuntimeException)
{
	return createPropertySetInfo( getInfoHelper() );
}

//-------------------------------------------------------------------------
::cppu::IPropertyArrayHelper& ODBTypeWizDialogSetup::getInfoHelper()
{
	return *const_cast<ODBTypeWizDialogSetup*>(this)->getArrayHelper();
}

//------------------------------------------------------------------------------
::cppu::IPropertyArrayHelper* ODBTypeWizDialogSetup::createArrayHelper( ) const
{
	Sequence< Property > aProps;
	describeProperties(aProps);
	return new ::cppu::OPropertyArrayHelper(aProps);
}
//------------------------------------------------------------------------------
Dialog*	ODBTypeWizDialogSetup::createDialog(Window* _pParent)
{
	return new ODbTypeWizDialogSetup(_pParent, m_pDatasourceItems, m_aContext.getLegacyServiceFactory(), m_aInitialSelection);
}
// -----------------------------------------------------------------------------
void ODBTypeWizDialogSetup::executedDialog(sal_Int16 _nExecutionResult)
{
    if ( _nExecutionResult == RET_OK )
    {
        const ODbTypeWizDialogSetup* pDialog = static_cast< ODbTypeWizDialogSetup* >( m_pDialog );
        m_bOpenDatabase = pDialog->IsDatabaseDocumentToBeOpened();
        m_bStartTableWizard = pDialog->IsTableWizardToBeStarted();
    }
}

//.........................................................................
}	// namespace dbaui
//.........................................................................

