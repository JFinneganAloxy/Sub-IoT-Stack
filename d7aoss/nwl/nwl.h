/*! \file nwl.h
 *
 * \copyright (C) Copyright 2013 University of Antwerp (http://www.cosys-lab.be) and others.\n
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.\n
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * \author maarten.weyn@uantwerpen.be
 *
 */

#ifndef NWL_H_
#define NWL_H_

#include "../types.h"
#include "../dll/dll.h"

#define BPID_AdvP 0xF0
#define BPID_BeaconP 0xF1

typedef enum {
	ProtocolTypeBackgroundProtocol,
	ProtocolTypeNetworkProtocol,
	ProtocolTypeDatastreamProtocol
} Protocol_Type;


typedef struct {
	uint8_t bpid;
	uint8_t data_length;
	uint8_t* protocol_data;
} nwl_background_frame_t;

typedef struct {
	uint8_t frame_id;
	uint8_t payload_length;
	uint8_t* payload;
} nwl_ff_D7ADP_t;

typedef struct {
	uint8_t* d7anls_header;
	uint8_t* d7anp_routing_header;
	uint8_t payload_length;
	uint8_t* payload;
	uint8_t* d7anls_auth_data;
} nwl_ff_D7ANP_t;

typedef struct {
	uint16_t eta;
} AdvP_Data;


typedef struct
{
    /// Protocol Type
    Protocol_Type  protocol_type;
    /// Data
    void* data;
    dll_rx_res_t* dll_rx_res;
} nwl_rx_res_t;

typedef struct {
	uint8_t nls_code;
	uint8_t* nls_initialization_data;
	uint8_t* target_address;
} nwl_security;

typedef struct {
	uint8_t hop_control;
	uint8_t hop_ext;
	uint8_t* origin_device_id;
	uint8_t* destination_device_id;
} nwl_routing_header;


typedef void (*nwl_tx_callback_t)(Dll_Tx_Result);
typedef void (*nwl_rx_callback_t)(nwl_rx_res_t *);

void nwl_init();
void nwl_set_tx_callback(nwl_tx_callback_t);
void nwl_set_rx_callback(nwl_rx_callback_t);

// parameters for DLL
//TODO: set this through configuration files


// Background frames

/*! \brief Builds a advertising protocol background frame  (Network Layer)
 *
 *  Creates a Background Frame for the advertising protocol
 *
 *
 *  \param uint16_t eta 		Number of ticks which needs to be put in the data.
 *  \param uint8_t spectrum_id 	The channel on which to send the background frame.
 *  \param uint8_t tx_eirp 		The send EIRP.
 *  \param uint8_t subnet 		The subnet to of the background frame.
 */
void nwl_build_advertising_protocol_data(uint16_t eta, uint8_t spectrum_id, int8_t tx_eirp, uint8_t subnet);

/*! \brief Builds a beacon protocol background frame  (Network Layer)
 *
 *  Creates a Background Frame for the beacon protocol
 *  This protocol is not in the draft but is evaluated for proposition
 *  It broadcasts the VID of the node
 *
 *
 *  \param uint8_t spectrum_id 	The channel on which to send the background frame.
 *  \param uint8_t tx_eirp 		The send EIRP.
 *  \param uint8_t subnet 		The subnet to of the background frame.
 */
void nwl_build_beaconprotocol_data(uint8_t spectrum_id, int8_t tx_eirp, uint8_t subnet);

// Foreground frames
void nwl_build_network_protocol_data(uint8_t* data, uint8_t length, nwl_security* security, nwl_routing_header* routing, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp, uint8_t dialog_id);
//void nwl_build_datastream_protocol_data(uint8_t* data, uint8_t length, nwl_security* security, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp, uint8_t dialog_id);

void nwl_rx_start(uint8_t subnet, uint8_t spectrum_id, Protocol_Type type);
void nwl_rx_stop();
#endif