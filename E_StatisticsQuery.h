/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef E_StatisticsQuery_H_
#define E_StatisticsQuery_H_

#include <Rendering/StatisticsQuery.h>
#include <EScript/Objects/ReferenceObject.h>

namespace E_Rendering{

/**
 *   [E_StatisticsQuery] ---|> [ReferenceObject] ---|> [Object]
 */
class E_StatisticsQuery : public EScript::ReferenceObject<Rendering::StatisticsQuery,EScript::Policies::SameEObjects_ComparePolicy> {
		ES_PROVIDES_TYPE_NAME(StatisticsQuery)
	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_StatisticsQuery(Rendering::StatisticsQuery && qo):
			ReferenceObject<Rendering::StatisticsQuery,EScript::Policies::SameEObjects_ComparePolicy>(std::move(qo),getTypeObject()){ }
			
		virtual ~E_StatisticsQuery() { }
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_StatisticsQuery,	Rendering::StatisticsQuery*,	&(**eObj))

#endif /* E_StatisticsQuery_H_ */
