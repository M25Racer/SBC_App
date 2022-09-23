#include "indigo_base_protocol.h"

static ToolAddress g_self_address;

IndigoBaseProtocolBuilderV1 g_frame_builder;

void init_common_header(CmdCommonHeaderV1 *hdr)
{
    hdr->version = SELF_CMD_VERSION;
    hdr->type    = COMMAND_TYPE_COMMAND;
}

void init_command_header(CmdTypeCommandHeader *hdr, uint8_t cmd_id)
{
    hdr->version = SELF_CMD_VERSION;
    hdr->cmd_id  = cmd_id;
}

void init_error_header(CmdTypeErrorHeaderV1 *hdr, uint16_t frame_num, uint8_t cmd_id, uint16_t  error_id)
{
    hdr->version   = SELF_CMD_VERSION;
    hdr->type      = COMMAND_TYPE_ERROR;
    hdr->frame_num = frame_num;
    hdr->cmd_id    = cmd_id;
    hdr->error_id  = error_id;
}

void command_sync_wait_creator(uint16_t time, EnumCmdWait type)
{
    CmdWait cmd_wait;
    cmd_wait.type = type;//EnumCmdWaitAgainSend;
    cmd_wait.ms = time;

    //Fill Command Structures
    CmdCommonHeaderV1 cmd_common_header; // = { .version = SELF_CMD_VERSION, .type = COMMAND_TYPE_COMMAND };
    init_common_header(&cmd_common_header);

    CmdTypeCommandHeader cmd_type_header; // = { .version = SELF_CMD_VERSION, .cmd_id = CMD_GET_STATUS };
    init_command_header(&cmd_type_header, CMD_WAIT);

    g_self_address.tool_id = 0x97;  // SRP2 address
    g_self_address.serial_num = 0;

    bool r = false;

    //Build Command
    r = g_frame_builder.BodyAdd((uint8_t *)&cmd_common_header, sizeof(cmd_common_header));

    if(r)
        r = g_frame_builder.BodyAdd((uint8_t *)&cmd_type_header, sizeof(cmd_type_header));

    if(r)
        r = g_frame_builder.BodyAdd((uint8_t *)&cmd_wait, sizeof(cmd_wait));

    if(r)
        r = g_frame_builder.BodyEnd(); //will initiate frame_builded_cb() call (from notify.cpp) by several times
}
