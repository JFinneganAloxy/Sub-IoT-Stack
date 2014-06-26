/*! \file trans.c
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
 * \author Dragan.Subotic@uantwerpen.be
 *
 */


#include "trans.h"
#include "../framework/timer.h"
#include "../framework/log.h"
#include <string.h>


static uint8_t dialogid = 0;

static trans_tx_callback_t trans_tx_callback;
static trans_rx_datastream_callback_t trans_rx_datastream_callback;
static trans_rx_query_callback_t trans_rx_query_callback;

static Trans_Rx_Query_Result query_result;
static D7AQP_Dialog_Template dialog_template;

//Flow Control Process
static void control_tx_callback(Dll_Tx_Result Result)
{
	switch(Result){
			case DLLTxResultOK:
				#ifdef LOG_TRANS_ENABLED
				log_print_stack_string(LOG_TRANS, "Trans: Packet is sent");
				#endif
				trans_tx_callback(TransPacketSent);
				break;
			case DLLTxResultFail:
				trans_tx_callback(TransPacketFail);
				#ifdef LOG_TRANS_ENABLED
				log_print_stack_string(LOG_TRANS, "Trans: Fail to sent");
				#endif
				break;
		}
	return;
}

static void nwl_rx_callback(nwl_rx_res_t* result)
{
	if (result->protocol_type == ProtocolTypeBackgroundProtocol)
	{
		// Should this be here?
	}
	else if (result->protocol_type == ProtocolTypeNetworkProtocol)
	{
		query_result.nwl_rx_res = result;

		nwl_ff_D7ANP_t* data = (nwl_ff_D7ANP_t*) result->data;

		uint8_t pointer = 0;
		query_result.d7aqp_command.command_code = data->payload[pointer++];
		if (query_result.d7aqp_command.command_code & D7AQP_COMMAND_CODE_EXTENSION)
			query_result.d7aqp_command.command_extension = data->payload[pointer++];
		else
			query_result.d7aqp_command.command_extension = 0;

		if (query_result.d7aqp_command.command_extension & D7AQP_COMMAND_EXTENSION_NORESPONSE)
		{
			query_result.d7aqp_command.dialog_template = NULL;
		} else {
			dialog_template.response_timeout = (uint16_t)(data->payload[pointer++] << 8) | (uint16_t)(data->payload[pointer++]);
			dialog_template.response_channel_list_lenght = data->payload[pointer++];
			dialog_template.response_channel_list = &data->payload[pointer];
			pointer += dialog_template.response_channel_list_lenght;

			query_result.d7aqp_command.dialog_template = &dialog_template;
		}

		//TODO: implement other queries
		query_result.d7aqp_command.ack_template = NULL;
		query_result.d7aqp_command.global_query_template = NULL;
		query_result.d7aqp_command.local_query_template = NULL;
		query_result.d7aqp_command.error_template = NULL;

		switch (query_result.d7aqp_command.command_code & 0x70)
		{
			// Response
			case D7AQP_COMMAND_TYPE_RESPONSE:
			{
				switch (query_result.d7aqp_command.command_code & 0x0F)
				{
					case D7AQP_OPCODE_ANNOUNCEMENT_FILE:
					{
						query_result.d7aqp_command.command_data = NULL;
						break;
					}
				}
				break;
			}

			// NA2P Request
			case D7AQP_COMMAND_TYPE_NA2P_REQUEST:
			{
				switch (query_result.d7aqp_command.command_code & 0x0F)
				{
					case D7AQP_OPCODE_ANNOUNCEMENT_FILE:
					{
						D7AQP_Single_File_Return_Template sfr_tmpl;
						sfr_tmpl.return_file_id = data->payload[pointer++];
						sfr_tmpl.file_offset = data->payload[pointer++];
						sfr_tmpl.isfb_total_length = data->payload[pointer++];
						sfr_tmpl.file_data = &data->payload[pointer];

						pointer += sfr_tmpl.isfb_total_length - sfr_tmpl.file_offset;

						query_result.d7aqp_command.command_data = &sfr_tmpl;
					}
				}
				break;
			}
		}

		trans_rx_query_callback(&query_result);
	}
	else if (result->protocol_type == ProtocolTypeDatastreamProtocol)
	{
		Trans_Rx_Datastream_Result datastream_result;
		nwl_ff_D7ADP_t* data = (nwl_ff_D7ADP_t*) result->data;

		//TODO: check data->frame_id?
		datastream_result.lenght = data->payload_length;
		datastream_result.payload = data->payload;

		trans_rx_datastream_callback(&datastream_result);
	}
}

void trans_init(){
	nwl_init();
	nwl_set_tx_callback(&control_tx_callback);
	nwl_set_rx_callback(&nwl_rx_callback);
}

void trans_set_tx_callback(trans_tx_callback_t cb)
{
	trans_tx_callback = cb;
}

void trans_set_datastream_rx_callback(trans_rx_datastream_callback_t cb)
{
	trans_rx_datastream_callback = cb;
}

void trans_set_query_rx_callback(trans_rx_query_callback_t cb)
{
	trans_rx_query_callback = cb;
}

void trans_tx_foreground_frame(uint8_t* data, uint8_t length, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp) {
	nwl_build_network_protocol_data(data, length, NULL, NULL, subnet, spectrum_id, tx_eirp, dialogid++);
	dll_initiate_csma_ca();
}


/*! \brief Creates a D7AQP Query (transport layer)
 *
 *  Creates a D7AQP Query based on the Command Request Template
 *  Currently only Announcement of file is uses
 *
 *  \todo implement other query
 *
 *  \param request_template Pointer to the Command Request Template
 *  \param file_template The corresponding file template
 *  \param subnet The subnet which needs to be used to send the query
 *  \param spectrum_id The spectrum_id which needs to be used to send the query
 *  \param tx_eirp The transmit EIRP which need to be used to send the query
 */
void trans_tx_query(D7AQP_Command* command, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp)
{
	uint8_t data[64];//TODO: should be dynamic or queue
	uint8_t pointer = 0;
	uint8_t i = 0;

	data[pointer++] = command->command_code;
	if (command->command_code & D7AQP_COMMAND_CODE_EXTENSION)
		data[pointer++] = command->command_extension;

	if (command->dialog_template != NULL)
	{
		data[pointer++] = command->dialog_template->response_timeout >> 8;
		data[pointer++] = command->dialog_template->response_timeout & 0xFF;
		data[pointer++] = command->dialog_template->response_channel_list_lenght;

		for (i=0;i<command->dialog_template->response_channel_list_lenght;i++)
			data[pointer++] = command->dialog_template->response_channel_list[i];
	}

	switch (command->command_code & 0x70)
	{
		// Response
		case D7AQP_COMMAND_TYPE_RESPONSE:
		{
			switch (command->command_code & 0x0F)
			{
				case D7AQP_OPCODE_ANNOUNCEMENT_FILE:
				{
					break;
				}
			}
			break;
		}

		// NA2P Request
		case D7AQP_COMMAND_TYPE_NA2P_REQUEST:
		{
			switch (command->command_code & 0x0F)
			{
				case D7AQP_OPCODE_ANNOUNCEMENT_FILE:
				{
					D7AQP_Single_File_Return_Template* sfr_tmpl = (D7AQP_Single_File_Return_Template*) command->command_data;
					data[pointer++] = sfr_tmpl->return_file_id;
					data[pointer++] = sfr_tmpl->file_offset;
					data[pointer++] = sfr_tmpl->isfb_total_length;

					memcpy(&data[pointer], sfr_tmpl->file_data, sfr_tmpl->isfb_total_length - sfr_tmpl->file_offset);
					pointer+= sfr_tmpl->isfb_total_length - sfr_tmpl->file_offset;
				}
				break;
			}
			break;
		}
	}

	nwl_build_network_protocol_data(data, pointer, NULL, NULL, subnet, spectrum_id, tx_eirp, dialogid++);
	dll_initiate_csma_ca();
}

void trans_tx_datastream(uint8_t* data, uint8_t length, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp) {
	nwl_build_datastream_protocol_data(data, length, NULL, subnet, spectrum_id, tx_eirp, dialogid++);
	dll_initiate_csma_ca();
}

void trans_rx_datastream_start(uint8_t subnet, uint8_t spectrum_id)
{
	nwl_rx_start(subnet, spectrum_id, ProtocolTypeDatastreamProtocol);
}


void trans_rx_query_start(uint8_t subnet, uint8_t spectrum_id)
{
	nwl_rx_start(subnet, spectrum_id, ProtocolTypeNetworkProtocol);
}

void trans_rx_stop()
{
	nwl_rx_stop();
}


//void trans_tx_background_frame(uint8_t* data, uint8_t subnet, uint8_t spectrum_id, int8_t tx_eirp){
//	nwl_build_background_frame(data, subnet, spectrum_id, tx_eirp);
//}