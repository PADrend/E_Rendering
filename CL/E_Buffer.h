/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_BUFFER_H_
#define E_RENDERING_CL_E_BUFFER_H_

#include "E_Memory.h"

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Memory/Buffer.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Buffer

		E_Buffer ---|> E_Memory ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Buffer> >
			|
			--------------> Rendering::CL::Buffer		*/
class E_Buffer : public E_Memory {
		ES_PROVIDES_TYPE_NAME(Buffer)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Buffer(Rendering::CL::Buffer * _obj,EScript::Type * type=nullptr) :
			E_Memory(_obj, type ? type : E_Buffer::getTypeObject()) {}
		virtual ~E_Buffer() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Buffer,	Rendering::CL::Buffer*, 			dynamic_cast<Rendering::CL::Buffer*>((**eObj).get()))
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Buffer *,			E_Rendering::E_CL::E_Buffer,	obj?new E_Rendering::E_CL::E_Buffer(obj) : nullptr)

#endif // E_RENDERING_CL_E_BUFFER_H_
#endif // RENDERING_HAS_LIB_OPENCL

