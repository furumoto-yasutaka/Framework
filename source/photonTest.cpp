#include "photonTest.h"
#include <iostream>


static const ExitGames::Common::JString appId = L"02292849-1f12-4962-9615-a7e786f29cb7"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// functions
MyPhoton::MyPhoton()
	: mLoadBalancingClient(*this, appId, appVersion)
{

}

void MyPhoton::connect(void)
{
	std::wcout << "Connecting..." << std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues());
}

void MyPhoton::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

//void MyPhoton::sendMyData(float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float angle,
//	float bulletX, float bulletY)
//{
//	float data[9];
//	data[0] = xpos;
//	data[1] = ypos;
//	data[2] = xvel;
//	data[3] = yvel;
//	data[4] = xaccel;
//	data[5] = yaccel;
//	data[6] = angle;
//	data[7] = bulletX;
//	data[8] = bulletY;
//
//	mLoadBalancingClient.opRaiseEvent(true, data, 9, 1);
//}
//
//void MyPhoton::sendMyData2(float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float angle,
//	float mXpos, float mYpos, float mXvel, float mYvel, float mXaccel, float mYaccel, float mAngle,
//	int opponentHealth, int fireMode,
//	int bulletNum, std::list<Bullet*> bulletList)
//{
//	float data[500];
//
//	int bulletInfoNum = bulletNum * 5;
//	int totalData = 18 + bulletInfoNum;
//
//	data[0] = xpos;
//	data[1] = ypos;
//	data[2] = xvel;
//	data[3] = yvel;
//	data[4] = xaccel;
//	data[5] = yaccel;
//	data[6] = angle;
//
//	data[7] = mXpos;
//	data[8] = mYpos;
//	data[9] = mXvel;
//	data[10] = mYvel;
//	data[11] = mXaccel;
//	data[12] = mYaccel;
//	data[13] = mAngle;
//	data[14] = opponentHealth;
//	data[15] = (float)bulletNum;
//
//	if (bulletNum > 0)
//	{
//		data[16] = Application::getInstance().isShot;
//
//		if (Application::getInstance().isShot > 0.0f)
//			Application::getInstance().isShot = -1;
//
//		/*if (bulletList.back()->life > 4.95f)
//			data[17] = 1;
//		else
//			data[17] = -1;*/
//
//		data[17] = (float)fireMode;
//
//		int startPos = 18;
//
//		for (std::list<Bullet*>::iterator i = bulletList.begin(); i != bulletList.end(); i++)
//		{
//			data[startPos] = (*i)->getPos().mVal[0];
//			data[startPos + 1] = (*i)->getPos().mVal[1];
//			startPos += 2;
//		}
//
//		for (std::list<Bullet*>::iterator i = bulletList.begin(); i != bulletList.end(); i++)
//		{
//			data[startPos] = (*i)->getVelocity().mVal[0];
//			data[startPos + 1] = (*i)->getVelocity().mVal[1];
//			startPos += 2;
//		}
//
//		for (std::list<Bullet*>::iterator i = bulletList.begin(); i != bulletList.end(); i++)
//		{
//			data[startPos] = (*i)->life;
//			startPos++;
//		}
//
//	}
//															↓イベントのID(どんなデータかとかを識別できる)
//	mLoadBalancingClient.opRaiseEvent(true, data, totalData, 1);
//}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// protocol implementations

void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout << "connectionErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
	const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	printf("きたよ");
	mLoadBalancingClient.opJoinRandomRoom();
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout << "clientErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout << "warningReturn : " << warningCode << std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout << "serverErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << "joinRoomEventAction" << std::endl;
	
	//if (playernrs.getSize() >= 2)
	//	Application::getInstance().sendMyData();
}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout << "leaveRoomEventAction" << std::endl;
}

// データがきたときに動く
void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	float* data = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
	if (data)
	{
		//Application::getInstance().OnReceivedOpponentData(data);
		//Application::getInstance().OnReceivedOpponentData2(data);
	}
	else
		std::cout << "invalid data" << std::endl;
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if (errorCode)
		std::wcout << "Failed to connect : " << errorString.cstr() << std::endl;
	else
	{
		std::wcout << "Connected to Photon Server." << std::endl;

		std::wcout << "Trying to join a random room..." << std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout << "disconnected" << std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "createRoomReturn" << std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinOrCreateRoomReturn" << std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinRoomReturn" << std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinRandomRoomReturn" << std::endl;

	if (errorCode)
	{
		std::wcout << "Failed to join a random room : " << errorCode << ", " << errorString.cstr() << std::endl;

		if (errorCode == 32760) //no match found error code
		{
			std::wcout << "Creating a room..." << std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(20));
		}

		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "leaveRoomReturn" << std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout << "joinLobbyReturn" << std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout << "leaveLobbyReturn" << std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "onLobbyStatsResponse" << std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "onLobbyStatsUpdate" << std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
