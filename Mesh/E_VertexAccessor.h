/*
	This file is part of the E_Rendering library.
	Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef _E_Rendering_E_VertexAccessor_H_
#define _E_Rendering_E_VertexAccessor_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Mesh/VertexAccessor.h>

namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{

/*!	EScript wrapper class for Rendering::VertexAccessor.

		E_VertexAccessor ---|> EScript::ReferenceObject<Util::Reference<Rendering::VertexAccessor> >
			|
			--------------> Rendering::VertexAccessor		*/
class E_VertexAccessor : public EScript::ReferenceObject<Util::Reference<Rendering::VertexAccessor> > {
		ES_PROVIDES_TYPE_NAME(VertexAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);
		
		E_VertexAccessor(Rendering::VertexAccessor * t):ReferenceObject<Util::Reference<Rendering::VertexAccessor> >(t,getTypeObject()){}
		E_VertexAccessor(const Util::Reference<Rendering::VertexAccessor>& t):ReferenceObject<Util::Reference<Rendering::VertexAccessor> >(t,getTypeObject()){}
		virtual ~E_VertexAccessor(){}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexAccessor, Rendering::VertexAccessor*, (**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::VertexAccessor *, E_Rendering::E_VertexAccessor, obj ? new E_Rendering::E_VertexAccessor(obj) : nullptr)
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::VertexAccessor>, E_Rendering::E_VertexAccessor, obj ? new E_Rendering::E_VertexAccessor(obj) : nullptr)

#endif // _E_Rendering_E_VertexAccessor_H_
