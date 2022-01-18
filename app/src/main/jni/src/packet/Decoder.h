#pragma once
#include <iostream>
#include <string>
#include <enet/enet.h>

#include "Common.h"
#include "utilities/Macros.h"

namespace packet {
    namespace decoder {
        inline int GetMessageTypeFromPacket(ENetPacket* packet) {
            if (packet->dataLength > 3) {
                return *(packet->data);
            }

            LOGE("Bad packet length, ignoring message");
            return 0;
        }

        inline char* GetTextPointerFromPacket(ENetPacket* packet) {
            memset(packet->data + packet->dataLength - 1, 0, 1);
            return (char*)(packet->data + 4);
        }

        inline char* GetStructPointerFromPacket(ENetPacket* packet, int length) {
            if (packet->dataLength >= length + 4) {
                return (char*)(packet->data + 4);
            }
            return 0;
        }

        inline GameUpdatePacket* GetStructPointerFromTankPacket(ENetPacket* packet) {
            if (packet->dataLength >= sizeof(GameUpdatePacket)) {
                GameUpdatePacket* gameUpdatePacket = (GameUpdatePacket*)(packet->data + 4);
                if ((gameUpdatePacket->unk6 & 8) == 0) {
                    gameUpdatePacket->dataExtendedLength = 0;
                }
                else {
                    if (packet->dataLength < gameUpdatePacket->dataExtendedLength + sizeof(GameUpdatePacket)) {
                        LOGE("Packet too small for extended packet to be valid");
                        LOGE("Sizeof float is 4.  TankUpdatePacket size: 56");
                        return nullptr;
                    }
                }
                return gameUpdatePacket;
            }
            return nullptr;
        }

        inline uint8_t* GetExtendedDataPointerFromTankPacket(GameUpdatePacket* gameUpdatePacket) {
            return reinterpret_cast<uint8_t*>(&gameUpdatePacket->dataExtended);
        }
    } // namespace PacketDecodee
} // namespace Client