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



#ifndef ADC_X_DOCU_HXX
#define ADC_X_DOCU_HXX

// BASE CLASSES
#include <autodoc/x_parsing.hxx>




class X_Docu : public autodoc::X_Parser_Ifc
{
  public:
	// LIFECYCLE
						X_Docu(
							const char *        i_tag,
                            const char *        i_explanation );
                        ~X_Docu();
	// INQUIRY
	virtual E_Event	   	GetEvent() const;
	virtual void	   	GetInfo(
							std::ostream &      o_rOutputMedium ) const;

  private:
	String 				sTagName;
	String 				sExplanation;
};




#endif
