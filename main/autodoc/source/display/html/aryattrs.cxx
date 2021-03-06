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

#include <precomp.h>
#include "aryattrs.hxx"


// NOT FULLY DEFINED SERVICES
#include <ary/getncast.hxx>
#include <ary/cpp/c_class.hxx>
#include <ary/cpp/c_enum.hxx>
#include <ary/cpp/c_funct.hxx>
#include <ary/cpp/c_gate.hxx>
#include <ary/cpp/c_namesp.hxx>
#include <ary/cpp/cp_ce.hxx>
#include <ary/cpp/cp_type.hxx>
#include "strconst.hxx"




//********************       HtmlDisplay_Impl        *********************//

const char *
Get_ClassTypeKey( const ary::cpp::Class & i_rClass )
{
    return i_rClass.ClassKey() == ary::cpp::CK_class
                            ?   C_sHFTypeTitle_Class
                            :   i_rClass.ClassKey() == ary::cpp::CK_struct
                                    ?   C_sHFTypeTitle_Struct
                                    :   C_sHFTypeTitle_Union;

}

const char *
Get_TypeKey( const ary::cpp::CodeEntity & i_rCe )
{
    if ( ary::is_type<ary::cpp::Class>(i_rCe) )
    {
        return Get_ClassTypeKey(
                    ary::ary_cast<ary::cpp::Class>(i_rCe) );
    }
    if ( ary::is_type<ary::cpp::Enum>(i_rCe) )
    {
     	return "enum";
    }
    return "";
}

bool
Ce_IsInternal( const ary::cpp::CodeEntity & i_rCe )
{
    return NOT i_rCe.IsVisible();
}

const char *
SyntaxText_PreName( const ary::cpp::Function &      i_rFunction,
                    const ary::cpp::Gate &   i_rAryGate )
{
    static StreamStr  sResult( 150 );
    sResult.seekp(0);

    // write pre-name:
    const ary::cpp::FunctionFlags & rFlags = i_rFunction.Flags();
    if ( rFlags.IsStaticLocal() OR rFlags.IsStaticMember() )
        sResult << "static ";
    if ( rFlags.IsExplicit() )
        sResult << "explicit ";
    if ( rFlags.IsMutable() )
        sResult << "mutable ";
    if ( i_rFunction.Virtuality() != ary::cpp::VIRTUAL_none )
        sResult << "virtual ";
    i_rAryGate.Types().Get_TypeText( sResult, i_rFunction.ReturnType() );
    sResult << " ";

    return sResult.c_str();
}

const char *
SyntaxText_PostName( const ary::cpp::Function &     i_rFunction,
                     const ary::cpp::Gate &  i_rAryGate )
{
    static StreamStr  sResult( 850 );
    sResult.seekp(0);

    // parameters and con_vol
    i_rAryGate.Ces().Get_SignatureText( sResult, i_rFunction.Signature(), &i_rFunction.ParamInfos() );

    // write Exceptions:
    const std::vector< ary::cpp::Type_id > *
            pThrow = i_rFunction.Exceptions();
    if ( pThrow)
    {

        std::vector< ary::cpp::Type_id >::const_iterator
		    	it = pThrow->begin();
        std::vector< ary::cpp::Type_id >::const_iterator
	    		it_end = pThrow->end();

        if (it != it_end)
        {
            sResult << " throw( ";
            i_rAryGate.Types().Get_TypeText(sResult, *it);

            for ( ++it; it != it_end; ++it )
            {
                sResult << ", ";
                i_rAryGate.Types().Get_TypeText(sResult, *it);
            }
            sResult << " )";
        }
        else
        {
            sResult << " throw( )";
        }
    }   // endif // pThrow

    // abstractness:
    if ( i_rFunction.Virtuality() == ary::cpp::VIRTUAL_abstract )
        sResult << " = 0";

    // finish:
    sResult << ";";

    return sResult.c_str();
}

bool
Get_TypeText( const char * &                o_rPreName,
              const char * &                o_rName,
              const char * &                o_rPostName,
              ary::cpp::Type_id             i_nTypeid,
              const ary::cpp::Gate & i_rAryGate )
{
    static StreamStr       sResult_PreName(250);
    static StreamStr       sResult_Name(250);
    static StreamStr       sResult_PostName(250);

    sResult_PreName.seekp(0);
    sResult_Name.seekp(0);
    sResult_PostName.seekp(0);

    bool    ret = i_rAryGate.Types().Get_TypeText(
                                sResult_PreName,
                                sResult_Name,
                                sResult_PostName,
                                i_nTypeid );
    if ( sResult_PreName.tellp() > 0 )
    {
        char cLast = *( sResult_PreName.c_str() + (sResult_PreName.tellp() - 1) );
        if (cLast != ':' AND cLast != ' ')
            sResult_PreName << " ";
    }


    if (ret)
    {
        o_rPreName  = sResult_PreName.c_str();
        o_rName     = sResult_Name.c_str();
        o_rPostName = sResult_PostName.c_str();
    }
    else
    {
        o_rPreName  =  o_rName =  o_rPostName = "";
    }
    return ret;
}




//*********************         FunctionParam_Iterator      *****************//


FunctionParam_Iterator::FunctionParam_Iterator()
    :   // itTypes
        // itTypes_end
        // itNames_andMore
        // itNames_andMore_end
        eConVol(ary::cpp::CONVOL_none)
{
    static std::vector<ary::cpp::Type_id>   aTypesNull_;
    static StringVector                     aNamesNull_;

    itTypes = itTypes_end = aTypesNull_.end();
    itNames_andMore = itNames_andMore_end = aNamesNull_.end();
}

FunctionParam_Iterator::~FunctionParam_Iterator()
{
}

FunctionParam_Iterator &
FunctionParam_Iterator::operator++()
{
    if ( IsValid() )
    {
        ++itTypes;
        ++itNames_andMore;
    }
    return *this;
}

void
FunctionParam_Iterator::Assign( const ary::cpp::Function &  i_rFunction )
{
    const ary::cpp::OperationSignature &
        rSigna = i_rFunction.Signature();

    const std::vector<ary::cpp::Type_id> &
        rTypes = rSigna.Parameters();
    const StringVector &
        rNames = i_rFunction.ParamInfos();

    if ( rTypes.size() != rNames.size() OR rTypes.size() == 0 )
        return;

    itTypes     = rTypes.begin();
    itTypes_end = rTypes.end();
    itNames_andMore     = rNames.begin();
    itNames_andMore_end = rNames.end();

    eConVol = rSigna.ConVol();
}
