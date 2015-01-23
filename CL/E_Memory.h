/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_MEMORY_H_
#define E_RENDERING_CL_E_MEMORY_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Memory/Memory.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Memory

		E_Memory ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Memory> >
			|
			--------------> Rendering::CL::Memory		*/
class E_Memory : public EScript::ReferenceObject<Util::Reference<Rendering::CL::Memory> > {
		ES_PROVIDES_TYPE_NAME(Memory)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Memory(Rendering::CL::Memory * _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_Memory::getTypeObject()) {}
		virtual ~E_Memory() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Memory,	Rendering::CL::Memory*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Memory *,			E_Rendering::E_CL::E_Memory,	obj?new E_Rendering::E_CL::E_Memory(obj) : nullptr)

#endif // E_RENDERING_CL_E_MEMORY_H_
#endif // RENDERING_HAS_LIB_OPENCL

