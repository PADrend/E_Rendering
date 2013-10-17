/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_BUFFER_OBJECT_H
#define E_BUFFER_OBJECT_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/BufferObject.h>
#include <Util/References.h>

namespace E_Rendering{

/*! EScript wrapper for Rendering::BufferObject.
	[E_BufferObject] ---|> [ReferenceObject] ---|> [Object]  */
class E_BufferObject : public EScript::ReferenceObject<Util::Reference<Rendering::CountedBufferObject> > {
		ES_PROVIDES_TYPE_NAME(BufferObject)
	public:
		static void init(EScript::Namespace & lib);
		static EScript::Type* getTypeObject();

		E_BufferObject(Rendering::CountedBufferObject * t) : ReferenceObject_t(t,getTypeObject()){}
		virtual ~E_BufferObject(){}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_BufferObject,		Rendering::CountedBufferObject*,	(**eObj).get())
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_BufferObject,		Rendering::BufferObject*,			&(**eObj).get()->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CountedBufferObject*, 	E_Rendering::E_BufferObject,		obj ? new E_Rendering::E_BufferObject(obj) : nullptr)

#endif // E_BUFFER_OBJECT_H
