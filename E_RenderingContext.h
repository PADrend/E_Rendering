/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef __E_RenderingContext_H
#define __E_RenderingContext_H

#include <EScript/Objects/ReferenceObject.h>

namespace Rendering {
class RenderingContext;
}
namespace E_Rendering {

/***
 **   E_RenderingContext ---|> Object
 **/
class E_RenderingContext : public EScript::ReferenceObject<Rendering::RenderingContext *> {
	ES_PROVIDES_TYPE_NAME(RenderingContext)

	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		//! (ctor)
		E_RenderingContext(Rendering::RenderingContext * manager) : ReferenceObject_t(manager,getTypeObject()) {}
		//! (dtor)
		virtual ~E_RenderingContext(){}

};
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_RenderingContext,	Rendering::RenderingContext*,		**eObj)
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_RenderingContext,	Rendering::RenderingContext&,		***eObj)
ES_CONV_OBJ_TO_EOBJ(Rendering::RenderingContext*, 		E_Rendering::E_RenderingContext,	new E_Rendering::E_RenderingContext(obj))

#endif // __E_RenderingContext_H
