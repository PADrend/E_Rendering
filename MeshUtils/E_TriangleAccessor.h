
#ifndef E_RENDERING_E_TRIANGLEACCESSOR_H_
#define E_RENDERING_E_TRIANGLEACCESSOR_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/MeshUtils/TriangleAccessor.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{


/*!	EScript wrapper class for Rendering::MeshUtils::TriangleAccessor

		E_TriangleAccessor ---|> EScript::ReferenceObject<Util::Reference<Rendering::MeshUtils::TriangleAccessor> >
			|
			--------------> Rendering::MeshUtils::TriangleAccessor		*/
class E_TriangleAccessor : public EScript::ReferenceObject<Util::Reference<Rendering::MeshUtils::TriangleAccessor> > {
		ES_PROVIDES_TYPE_NAME(TriangleAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_TriangleAccessor(Util::Reference<Rendering::MeshUtils::TriangleAccessor> _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_TriangleAccessor::getTypeObject()) {}
		virtual ~E_TriangleAccessor() {}

};
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_TriangleAccessor,	Rendering::MeshUtils::TriangleAccessor*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::MeshUtils::TriangleAccessor *,			E_Rendering::E_TriangleAccessor, new E_Rendering::E_TriangleAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::MeshUtils::TriangleAccessor>,			E_Rendering::E_TriangleAccessor, new E_Rendering::E_TriangleAccessor(obj))

#endif // E_RENDERING_E_TRIANGLEACCESSOR_H_

