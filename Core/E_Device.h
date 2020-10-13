/*
	This file is part of the E_Rendering library.
	Copyright (C) 2020 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_RENDERING_CORE_DEVICE_H_
#define E_RENDERING_CORE_DEVICE_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Core/Device.h>
#include <Util/References.h>

namespace E_Rendering {

/*! EScript wrapper for Rendering::Device.
	[E_Device] ---|> [ReferenceObject] ---|> [Object]  */
class E_Device : public EScript::ReferenceObject<Rendering::Device::Ref> {
		ES_PROVIDES_TYPE_NAME(Device)
	public:
		static void init(EScript::Namespace & lib);
		static EScript::Type* getTypeObject();

		E_Device(Rendering::Device::Ref t) : ReferenceObject_t(t,getTypeObject()){}
		virtual ~E_Device(){}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Device, Rendering::Device*, (**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::Device*, E_Rendering::E_Device, new E_Rendering::E_Device(obj))
ES_CONV_OBJ_TO_EOBJ(Rendering::Device::Ref, E_Rendering::E_Device, new E_Rendering::E_Device(obj))

#endif // E_RENDERING_CORE_DEVICE_H_
