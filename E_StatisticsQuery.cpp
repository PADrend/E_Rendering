/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_StatisticsQuery.h"

#include "E_RenderingContext.h"
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

using namespace EScript;
using Rendering::StatisticsQuery;

namespace E_Rendering {

//! (static)
EScript::Type * E_StatisticsQuery::getTypeObject() {
	// E_StatisticsQuery ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_StatisticsQuery::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	//! static methods
	ES_FUN(typeObject, "createTimeElapsedQuery", 0, 0, new E_StatisticsQuery(StatisticsQuery::createTimeElapsedQuery()));
	
	//! methods
	ES_MFUN(typeObject,StatisticsQuery, "isResultAvailable", 1, 1, thisObj->isResultAvailable(parameter[0].to<Rendering::RenderingContext&>(rt)))
	ES_MFUN(typeObject,StatisticsQuery, "getResult", 1, 1, thisObj->getResult(parameter[0].to<Rendering::RenderingContext&>(rt)))
	ES_MFUN(typeObject,StatisticsQuery, "getResult64", 1, 1, static_cast<double>(thisObj->getResult64(parameter[0].to<Rendering::RenderingContext&>(rt))))
	ES_MFUN(typeObject,StatisticsQuery, "begin", 1, 1, (thisObj->begin(parameter[0].to<Rendering::RenderingContext&>(rt)), EScript::create(nullptr)))
	ES_MFUN(typeObject,StatisticsQuery, "end", 1, 1, (thisObj->end(parameter[0].to<Rendering::RenderingContext&>(rt)), EScript::create(nullptr)))
}

}
