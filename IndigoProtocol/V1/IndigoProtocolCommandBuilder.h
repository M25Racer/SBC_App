#pragma once

#if !defined (_MSC_VER)
    #include "stdint.h"

	//#define memcpy memcpy_fast

#endif

#include "TransportProtocolHeader.h"
#include "CommandHeaderHeader.h"
#include "IndigoBaseProtocolBuilderV1.h"
#include "IndigoBaseProtocolUsbBuilderV1.h"


class IndigoProtocolCommandBuilder
{
public:
    IndigoProtocolCommandBuilder();

    void Initialize();
    uint32_t GetLastError() const;

    void SetAMI();
    void SetUSB();

    bool IsAMI() const;
    bool IsUSB() const;

    void SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest);
    void SetNotifyAMI(IndigoBaseProtocolBuilderNotify notify, const void *param);
    void SetNotifyUSB(IndigoBaseProtocolBuilderNotify notify, const void *param);

    void AddOffsets(CmdRepeatOffset *offsets, uint32_t count);

    // добавляем заголовочную часть команды в транспортный уровень
    bool AddCmd(uint32_t cmd_id);
    bool AddCmdGetRawData(uint32_t count_chunks);
    bool AddCmdGetData(uint32_t count_samples, uint32_t sample_size);

    bool AddError(uint32_t packet_num, uint32_t cmd_id, uint32_t err_id, const char *err_str);
    // добавляем тело команды в транспортный уровень
    bool BodyAdd(const void *body, uint32_t size);
    bool BodyEnd();


private:
    enum Constants
    {
        AmiType = 1,
        UsbType = 2,
    };

    uint8_t m_buf[32];
    uint32_t m_buf_len = 0;

    uint32_t m_type = AmiType;
    IndigoBaseProtocolBuilderV1 m_ami_builder;
    IndigoProtocolUsbBuilderV1 m_usb_builder;

private:
    bool TransmitHeader(uint32_t body_len);

    bool AddBuf(const void *data, uint32_t size);
    bool AddBufStdHeader(uint32_t cmd_id);
    void StrCopy(char *dst, uint32_t dst_size, const char *src);
};
