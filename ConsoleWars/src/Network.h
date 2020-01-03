#pragma once
#include <queue>
#include <mutex>
#include "Player.h"

void startNetwork();

void endNetwork();

void receiveUDP(int port, std::queue<PlayerInput>& inputs, std::mutex& mtx);

void receiveTCP(int port, std::vector<Player>& players, std::vector<unsigned>& addresses, std::mutex& mtxPlayers, std::mutex& mtxIPs);

void broadcastMap(unsigned address, int port, const Buffer& buf);
