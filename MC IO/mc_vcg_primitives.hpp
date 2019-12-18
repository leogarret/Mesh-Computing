#ifndef MC_VCG_PRIMITIVES_H
#define MC_VCG_PRIMITIVES_H

#include "mc_vcg_mesh.hpp"

#include <ctime>
#include <mtlogger\mtlogger.hpp>

namespace mc
{

	namespace mvcg
	{

		namespace primitives
		{
			int createCube(mc::mvcg::Mesh &mesh);
		};

	};

};

#endif //!MC_VCG_PRIMITIVES_H