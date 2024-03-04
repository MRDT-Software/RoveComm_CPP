/******************************************************************************
 * @brief The RoveCommTCP class is used to send and receive data over a TCP
 *        connection. This class is a subclass of AutonomyThread, so it can be
 *        run in its own thread.
 *
 * @file RoveCommTCP.h
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-06
 *
 * @copyright Copyright Mars Rover Design Team 2024 - All Rights Reserved
 ******************************************************************************/

#ifndef ROVECOMM_TCP_H
#define ROVECOMM_TCP_H

#include "ExternalIncludes.h"
#include "RoveCommConsts.h"
#include "RoveCommGlobals.h"
#include "RoveCommManifest.h"
#include "RoveCommPacket.h"

/// \cond
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

/// \endcond

/******************************************************************************
 * @brief The RoveComm namespace contains all of the functionality for the
 *        RoveComm library. This includes the packet structure and the
 *        functions for packing and unpacking data.
 *
 * @author Eli Byrd (edbgkk@mst.edu)
 * @date 2024-02-07
 ******************************************************************************/
namespace rovecomm
{
    /******************************************************************************
     * @brief The RoveCommTCP class is used to send and receive data over a TCP
     *        connection.
     *
     * @author Eli Byrd (edbgkk@mst.edu)
     * @date 2024-02-07
     ******************************************************************************/
    class RoveCommTCP : AutonomyThread<void>
    {
        private:
            // Private member variables
            int m_nTCPSocket;
            struct sockaddr_in m_saTCPServerAddr;

            // Packet processing functions
            template<typename T>
            void ProcessPacket(const RoveCommData& stData, const std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<T>&)>, uint16_t>>& vCallbacks);
            void ReceiveTCPPacketAndCallback();

            // AutonomyThread member functions
            void ThreadedContinuousCode() override;
            void PooledLinearCode() override;

        public:
            // Constructor
            RoveCommTCP() : m_nTCPSocket(-1) {}

            // Initialization
            bool InitTCPSocket(const char* cIPAddress, int nPort);

            // Data transmission
            template<typename T>
            ssize_t SendTCPPacket(const RoveCommPacket<T>& stData, const char* cClientIPAddress, int nClientPort);

            // Callback management
            template<typename T>
            void AddTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback, const uint16_t& unCondition);

            template<typename T>
            void RemoveTCPCallback(std::function<void(const RoveCommPacket<T>&)> fnCallback);

            // Deinitialization
            void CloseTCPSocket();

            // Destructor
            ~RoveCommTCP();

            // NOTE: These functions are for testing purposes only and should not be used in production code!
            template<typename T>
            void CallProcessPacket(const RoveCommData& stData,
                                   const std::vector<std::tuple<std::function<void(const rovecomm::RoveCommPacket<T>&)>, uint16_t>>& vCallbacks)
            {
                ProcessPacket(stData, vCallbacks);
            }
    };
}    // namespace rovecomm

#endif    // ROVECOMM_TCP_H