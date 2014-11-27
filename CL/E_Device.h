#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_DEVICE_H_
#define E_RENDERING_CL_E_DEVICE_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Device.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Device

		E_Device ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Device> >
			|
			--------------> Rendering::CL::Device		*/
class E_Device : public EScript::ReferenceObject<Util::Reference<Rendering::CL::Device> > {
		ES_PROVIDES_TYPE_NAME(Device)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Device(Rendering::CL::Device * _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_Device::getTypeObject()) {}
		virtual ~E_Device() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Device,	Rendering::CL::Device*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Device *,			E_Rendering::E_CL::E_Device,	obj?new E_Rendering::E_CL::E_Device(obj) : nullptr)

#endif // E_RENDERING_CL_E_DEVICE_H_
#endif // RENDERING_HAS_LIB_OPENCL

