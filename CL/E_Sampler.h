#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_SAMPLER_H_
#define E_RENDERING_CL_E_SAMPLER_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Memory/Sampler.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Sampler

		E_Sampler ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Sampler> >
			|
			--------------> Rendering::CL::Sampler		*/
class E_Sampler : public EScript::ReferenceObject<Util::Reference<Rendering::CL::Sampler> > {
		ES_PROVIDES_TYPE_NAME(Sampler)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Sampler(Rendering::CL::Sampler * _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_Sampler::getTypeObject()) {}
		virtual ~E_Sampler() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Sampler,	Rendering::CL::Sampler*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Sampler *,			E_Rendering::E_CL::E_Sampler,	obj?new E_Rendering::E_CL::E_Sampler(obj) : nullptr)

#endif // E_RENDERING_CL_E_SAMPLER_H_
#endif // RENDERING_HAS_LIB_OPENCL

