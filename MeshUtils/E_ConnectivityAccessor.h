
#ifndef E_RENDERING_E_CONNECTIVITYACCESSOR_H_
#define E_RENDERING_E_CONNECTIVITYACCESSOR_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/MeshUtils/ConnectivityAccessor.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{


/*!	EScript wrapper class for Rendering::MeshUtils::ConnectivityAccessor

		E_ConnectivityAccessor ---|> EScript::ReferenceObject<Util::Reference<Rendering::MeshUtils::ConnectivityAccessor> >
			|
			--------------> Rendering::MeshUtils::ConnectivityAccessor		*/
class E_ConnectivityAccessor : public EScript::ReferenceObject<Util::Reference<Rendering::MeshUtils::ConnectivityAccessor> > {
		ES_PROVIDES_TYPE_NAME(ConnectivityAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_ConnectivityAccessor(Util::Reference<Rendering::MeshUtils::ConnectivityAccessor> _obj,EScript::Type * type=nullptr) :
				ReferenceObject_t(_obj, type ? type : E_ConnectivityAccessor::getTypeObject()) {}
		virtual ~E_ConnectivityAccessor() {}

};
}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ConnectivityAccessor,	Rendering::MeshUtils::ConnectivityAccessor*, 			(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::MeshUtils::ConnectivityAccessor *,			E_Rendering::E_ConnectivityAccessor,	new E_Rendering::E_ConnectivityAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::MeshUtils::ConnectivityAccessor>,			E_Rendering::E_ConnectivityAccessor,	new E_Rendering::E_ConnectivityAccessor(obj))

#endif // E_RENDERING_E_CONNECTIVITYACCESSOR_H_

