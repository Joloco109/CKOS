#include "MissionInfo.h"

using krpc::services::SpaceCenter;
using krpc::Stream;

MissionInfo::MissionInfo(std::shared_ptr<SpaceCenter> _spacecenter, std::shared_ptr<SpaceCenter::Vessel> _vessel)
		: spacecenter(_spacecenter), vessel(_vessel)
	{
		ut = std::make_shared<Stream<double>>(spacecenter->ut_stream());
		//std::cout << "MissionInfo created!" << std::endl;
	}
