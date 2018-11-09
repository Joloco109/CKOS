#include "MissionInfo.h"

MissionInfo::MissionInfo(std::shared_ptr<krpc::services::SpaceCenter> _spacecenter, std::shared_ptr<krpc::services::SpaceCenter::Vessel> _vessel)
		: spacecenter(_spacecenter), vessel(_vessel)
	{
		ut = std::make_shared<krpc::Stream<double>>(spacecenter->ut_stream());
		std::cout << "MissionInfo created!" << std::endl;
	}
