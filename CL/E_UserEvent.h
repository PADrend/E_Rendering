/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_USEREVENT_H_
#define E_RENDERING_CL_E_USEREVENT_H_

#include "E_Event.h"

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/UserEvent.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::UserEvent

		E_UserEvent ---|> E_Event ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Event> >
			|
			--------------> Rendering::CL::UserEvent		*/
class E_UserEvent : public E_Event {
		ES_PROVIDES_TYPE_NAME(UserEvent)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_UserEvent(Rendering::CL::UserEvent *_obj,EScript::Type * type=nullptr) :
			E_Event(_obj, type ? type : E_UserEvent::getTypeObject()) {}
		virtual ~E_UserEvent() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_UserEvent,	Rendering::CL::UserEvent*, 			dynamic_cast<Rendering::CL::UserEvent*>((**eObj).get()))
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::UserEvent *,			E_Rendering::E_CL::E_UserEvent,	obj?new E_Rendering::E_CL::E_UserEvent(obj) : nullptr)

#endif // E_RENDERING_CL_E_USEREVENT_H_
#endif // RENDERING_HAS_LIB_OPENCL

