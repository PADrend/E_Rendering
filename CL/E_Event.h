/*
	This file is part of the Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_EVENT_H_
#define E_RENDERING_CL_E_EVENT_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Event.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Event

		E_Event ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Event> >
			|
			--------------> Rendering::CL::Event		*/
class E_Event : public EScript::ReferenceObject<Util::Reference<Rendering::CL::Event> > {
		ES_PROVIDES_TYPE_NAME(Event)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Event(Rendering::CL::Event * _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_Event::getTypeObject()) {}
		virtual ~E_Event() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Event,	Rendering::CL::Event*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Event *,			E_Rendering::E_CL::E_Event,	obj?new E_Rendering::E_CL::E_Event(obj) : nullptr)

#endif // E_RENDERING_CL_E_EVENT_H_
#endif // RENDERING_HAS_LIB_OPENCL

