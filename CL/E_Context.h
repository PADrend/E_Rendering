#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_CONTEXT_H_
#define E_RENDERING_CL_E_CONTEXT_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Context.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Context

		E_Context ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Context> >
			|
			--------------> Rendering::CL::Context		*/
class E_Context : public EScript::ReferenceObject<Util::Reference<Rendering::CL::Context> > {
		ES_PROVIDES_TYPE_NAME(Context)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Context(Rendering::CL::Context * _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_Context::getTypeObject()) {}
		virtual ~E_Context() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Context,	Rendering::CL::Context*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Context *,			E_Rendering::E_CL::E_Context,	obj?new E_Rendering::E_CL::E_Context(obj) : nullptr)

#endif // E_RENDERING_CL_E_CONTEXT_H_
#endif // RENDERING_HAS_LIB_OPENCL

