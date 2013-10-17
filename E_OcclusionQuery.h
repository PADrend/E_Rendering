/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef E_OCCLUSIONQUERY_H_
#define E_OCCLUSIONQUERY_H_

#include <Rendering/OcclusionQuery.h>
#include <EScript/Objects/ReferenceObject.h>

namespace E_Rendering{

/**
 *   [E_OcclusionQuery] ---|> [ReferenceObject] ---|> [Object]
 */
class E_OcclusionQuery : public EScript::ReferenceObject<Rendering::OcclusionQuery,EScript::Policies::SameEObjects_ComparePolicy> {
		ES_PROVIDES_TYPE_NAME(OcclusionQuery)
	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_OcclusionQuery();
		virtual ~E_OcclusionQuery();
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_OcclusionQuery,	Rendering::OcclusionQuery*,	&(**eObj))

#endif /* E_OCCLUSIONQUERY_H_ */
