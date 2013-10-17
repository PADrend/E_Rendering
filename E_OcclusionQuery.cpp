/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_OcclusionQuery.h"

#include "E_RenderingContext.h"
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

using namespace EScript;
using Rendering::OcclusionQuery;

namespace E_Rendering {

//! (static)
EScript::Type * E_OcclusionQuery::getTypeObject() {
	// E_OcclusionQuery ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_OcclusionQuery::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! constructor
	ES_CTOR(typeObject,0, 0, new E_OcclusionQuery)

	//! static methods
	ES_FUN(typeObject, "enableTestMode", 1, 1, (OcclusionQuery::enableTestMode(parameter[0].to<Rendering::RenderingContext&>(rt)), EScript::create(nullptr)))
	ES_FUN(typeObject, "disableTestMode", 1, 1, (OcclusionQuery::disableTestMode(parameter[0].to<Rendering::RenderingContext&>(rt)), EScript::create(nullptr)))

	//! methods
	ES_MFUN(typeObject,OcclusionQuery, "isResultAvailable", 0, 0, thisObj->isResultAvailable())
	ES_MFUN(typeObject,OcclusionQuery, "getResult", 0, 0, thisObj->getResult())
	ES_MFUN(typeObject,OcclusionQuery, "begin", 0, 0, (thisObj->begin(), EScript::create(nullptr)))
	ES_MFUN(typeObject,OcclusionQuery, "end", 0, 0, (thisObj->end(), EScript::create(nullptr)))
}

//---

//! (ctor)
E_OcclusionQuery::E_OcclusionQuery():ReferenceObject<OcclusionQuery,EScript::Policies::SameEObjects_ComparePolicy>(getTypeObject()){
}

//! (dtor)
E_OcclusionQuery::~E_OcclusionQuery(){
}

}
