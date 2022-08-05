#include <QtGlobal>
//#include "StdAfx.h"
#if !defined (_MSC_VER)
    #include "string.h"
    #include "stddef.h"
    //#include "mt29fxg08_nand.h"
#endif
#include "../IndigoBaseTypes.h"
#include "IndigoProtocolCommandBuilder.h"
#include "CommandBodyHeader.h"


IndigoProtocolCommandBuilder::IndigoProtocolCommandBuilder() :
m_type(AmiType)
{
}

void IndigoProtocolCommandBuilder::Initialize()
{
    memset(m_buf, 0, sizeof(m_buf));
    m_buf_len = 0;

    if (IsAMI())
        m_ami_builder.Initialize();
    else
        m_usb_builder.Initialize();
}

uint32_t IndigoProtocolCommandBuilder::GetLastError() const
{
    uint32_t err;
    if (IsAMI())
        err= m_ami_builder.GetLastError();
    else
        err = m_usb_builder.GetLastError();
    return err;
}

void IndigoProtocolCommandBuilder::SetAMI()
{
    m_type = AmiType;
}

void IndigoProtocolCommandBuilder::SetUSB()
{
    m_type = UsbType;
}

bool IndigoProtocolCommandBuilder::IsAMI() const
{
    return m_type == AmiType;
}

bool IndigoProtocolCommandBuilder::IsUSB() const
{
    return m_type == UsbType;
}

void IndigoProtocolCommandBuilder::SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest)
{
    if (IsAMI())
        m_ami_builder.SetAddress(address_src, address_dest);
    else
        m_usb_builder.SetAddress(address_src, address_dest);
}

void IndigoProtocolCommandBuilder::SetNotifyAMI(IndigoBaseProtocolBuilderNotify notify, const void *param)
{
	m_ami_builder.SetNotify(notify, param);
}

void IndigoProtocolCommandBuilder::SetNotifyUSB(IndigoBaseProtocolBuilderNotify notify, const void *param)
{
	m_usb_builder.SetNotify(notify, param);
}

void IndigoProtocolCommandBuilder::AddOffsets(CmdRepeatOffset *offsets, uint32_t count)
{
    if (IsAMI())
        m_ami_builder.AddOffsets(offsets, count);
    else
        m_usb_builder.AddOffsets(offsets, count);
}

bool IndigoProtocolCommandBuilder::AddCmd(uint32_t cmd_id)
{
    if (!AddBufStdHeader(cmd_id))
        return false;
    if (!TransmitHeader(0))
        return false;
    return true;
}

//bool IndigoProtocolCommandBuilder::AddCmdGetRawData(uint32_t count_chunks)
//{
//    // формируем стандарнтые заголовки
//    if(!AddBufStdHeader(CMD_RAW_DATA))
//        return false;

//    // добавляем операнд
//    EnumCmdGetRawDataOperandV1 operand;
//    operand = EnumGetRawDataOperandData;
//    if (!AddBuf(&operand, sizeof(operand)))
//        return false;

//    uint32_t body_len = 0;
//#if !defined (_MSC_VER)
//    body_len = count_chunks * NAND_RAW_CHUNK_SIZE;
//#endif
//    // отсылаем заголовок
//    if (!TransmitHeader(body_len))
//        return false;
//    return true;
//}

bool IndigoProtocolCommandBuilder::AddCmdGetData(uint32_t count_samples, uint32_t sample_size)
{
    // формируем стандарнтые заголовки 
    if(!AddBufStdHeader(CMD_GET_DATA))
        return false;

    // добавляем операнд
    EnumCmdGetDataOperandV1 operand;
    operand = EnumGetDataOperandData;
    if (!AddBuf(&operand, sizeof(operand)))
        return false;

    // отсылаем заголовок
    if (!TransmitHeader(count_samples * sample_size))
        return false;
    return true;

}

bool IndigoProtocolCommandBuilder::AddError(uint32_t packet_num, uint32_t cmd_id, uint32_t err_id, const char *err_str)
{
    CmdTypeErrorHeaderV1 hdr_err;

    hdr_err.frame_num = packet_num;
    hdr_err.cmd_id = cmd_id;
    hdr_err.error_id = err_id;
    StrCopy(hdr_err.error_str, sizeof(hdr_err.error_str), err_str);

    if (!BodyAdd((const uint8_t *)&hdr_err, sizeof(hdr_err)))
        return false; 
    return true;
}

bool IndigoProtocolCommandBuilder::BodyAdd(const void *body, uint32_t size)
{
    bool res;
    if (IsAMI())
        res = m_ami_builder.BodyAdd((const uint8_t *)body, size);
    else
        res = m_usb_builder.BodyAdd((const uint8_t *)body, size);
    return res;
}

bool IndigoProtocolCommandBuilder::BodyEnd()
{
    bool res;
    if (IsAMI())
        res = m_ami_builder.BodyEnd();
    else
        res = m_usb_builder.BodyEnd();
    return res;
}

bool IndigoProtocolCommandBuilder::TransmitHeader(uint32_t body_len)
{
    bool res;
    if (IsAMI())
        res = m_ami_builder.BodyAdd(m_buf, m_buf_len); 
    else
    {
        uint32_t data_length = m_buf_len + body_len;
        m_usb_builder.SetDataLength(data_length);
        res = m_usb_builder.BodyAdd(m_buf, m_buf_len);
    }
    return res;
}

bool IndigoProtocolCommandBuilder::AddBuf(const void *data, uint32_t size)
{
    // проверим размера буфера хватает?
    if (m_buf_len + size > sizeof(m_buf))
        return false;
    if (!memcpy(&m_buf[m_buf_len], data, size))
        return false;

    m_buf_len += size;
    return true;
}

bool IndigoProtocolCommandBuilder::AddBufStdHeader(uint32_t cmd_id)
{
    CmdCommonHeaderV1 hdr_cmn;
    hdr_cmn.version = 1;
    hdr_cmn.type = COMMAND_TYPE_COMMAND;
    if (!AddBuf(&hdr_cmn, sizeof(hdr_cmn)))
        return false; 

    CmdTypeCommandHeader hdr_cmd;
    hdr_cmd.version = 1;
    hdr_cmd.cmd_id = cmd_id;
    if (!AddBuf(&hdr_cmd, sizeof(hdr_cmd)))
        return false; 

    return true;
}

void IndigoProtocolCommandBuilder::StrCopy(char *dst, uint32_t dst_size, const char *src)
{
    // копируем данные 
    while (*src)
    {
        // если место в приемнике закончилось, выходим
        if (!dst_size)
            break;

        *dst++ = *src++; 
        dst_size--;
    }
    // оставшуюся часть приемных данных, зануляем
    while (dst_size--)
        *dst++ = 0;
}
