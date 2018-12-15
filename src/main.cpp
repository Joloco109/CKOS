#include <iostream>
#include <fstream>
#include <memory>

#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>

#include "Logger.h"
#include "MissionPlan.h"
#include "MissionInfo.h"
#include "Stages/Ascent.h"
#include "Stages/OrbitalInsertion.h"
#include "StreamLogger.h"

void testConn() {
	std::shared_ptr<Logger> logger = std::make_shared<Logger>("Main");

	krpc::Client* conn = new krpc::Client(krpc::connect("C++ main test"));
	krpc::services::KRPC* krpc = new krpc::services::KRPC(conn);
	logger->info("Connected to kRPC server version {}", krpc->get_status().version());

	using krpc::services::SpaceCenter;

	SpaceCenter* space_center = new SpaceCenter(conn);
	SpaceCenter::Vessel* vessel = new SpaceCenter::Vessel(space_center->active_vessel());

	auto info = std::make_shared<MissionInfo>(
				std::shared_ptr<SpaceCenter>(space_center), 
				std::shared_ptr<SpaceCenter::Vessel>(vessel));
	logger->setInfo(info);

	StreamLogger streamlogger(info);

	MissionPlaner plan{info};
	std::shared_ptr<MissionStage> firstStage = std::make_shared<AscentStage>("Ascent", info, 100000, 10, 90);
	std::shared_ptr<MissionStage> secondStage = std::make_shared<OrbitalInsertionStage>("Orbital Insertion", info);
	plan.addStage(firstStage);
	plan.addStage(secondStage);

	streamlogger.log_start();
	while (plan.update());

	std::cin.get();
}

int main() {
	try {
		testConn();
	}
	catch (krpc::ConnectionError& e) {
		std::cerr << "Error in Connection!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
	catch (krpc::RPCError& e) {
		std::cerr << "RPC error in Test!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
	catch (std::system_error& e) {
		std::cerr << "System error in Test!" << std::endl;
		std::cerr << "Error:" << e.what() << std::endl;
	}
}
