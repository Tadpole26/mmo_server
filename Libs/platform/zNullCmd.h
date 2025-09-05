#pragma once
#include "zType.h"
#include "zMacro.h"
#include "zLogMgr.h"

namespace Cmd
{
	struct zNullCmd
	{
		union
		{
			uint16 cmdId;
			struct
			{
				uint16 firstId : 8;
				uint16 secondId : 8;
			};
		};
	};

	template<typename ProtoT>
	struct zProtoPacket 
	{
	private:
		BYTE _buffer[MSGBUF_MAX + 2];
		uint16 _allsize = 0;
		ProtoT _protoMsg;
	public:
		ProtoT& getProtoMsg() { return _protoMsg; }
		inline const char* ptr() { return _buffer; }
		inline const uint16 size() { return _allsize; }
		inline const uint16 pack(const uint16 firstId, const uint16 secondId = 0)
		{
			assert(_allsize == 0);
			zNullCmd* ptNullCmd = (zNullCmd*)_buffer;
			ptNullCmd->firstId = firstId;
			ptNullCmd->secondId = secondId;
			_allsize += sizeof(uint16);
			auto protosize = _protoMsg.ByteSize();
			if (protosize <= MSGBUF_MAX)
			{
				_protoMsg.SerializeToArray(&_buffer[_allsize], protosize);
				_allsize += protosize;
				return _allsize;
			}
			else
			{
				return 0;
			}
		}
	};

	template <typename MSG, uint16 FIRST, uint16 SECOND = 0>
	std::pair<bool, MSG> parseProto(const Cmd::zNullCmd* ptNullCmd, const uint16 nCmdLen)
	{
		std::pair<bool, MSG> p;
		if (nCmdLen < sizeof(uint16))
		{
			Log_Error("parseProto[%u,%u,%s], nCmdLen != sizeof(uint16)", FIRST, SECOND, p.second.GetTypeName().c_str());
			return p;
		}
		if (ptNullCmd->firstId != FIRST)
		{
			Log_Error("parseProto[%u,%u,%s], firstId != FIRST", FIRST, SECOND, p.second.GetTypeName().c_str());
			return p;
		}
		if (ptNullCmd->secondId != SECOND)
		{
			Log_Error("parseProto[%u,%u,%s], secondId != SECOND", FIRST, SECOND, p.second.GetTypeName().c_str());
			return p;
		}
		if (nCmdLen > sizeof(uint16) && !p.second.ParseFromArray((BYTE*)ptNullCmd + sizeof(uint16), nCmdLen - sizeof(uint16)))
		{
			Log_Error("parseProto[%u,%u,%s], !ParseFromArray", FIRST, SECOND, p.second.GetTypeName().c_str());
			return p;
		}
		p.first = true;
	}

	template <typename ProtoT>
	struct zProtoParser
	{
	private:
		zNullCmd _nullcmd;
		bool _flag = false;
		ProtoT _protoMsg;
	public:
		ProtoT& getProtoMsg() { return _protoMsg; }
		inline const uint16 cmdId() { return _nullcmd.cmdId; }
		inline const uint16 firstId() { return _nullcmd.firstId; }
		inline const uint16 secondId() { return _nullcmd.secondId; }
		zProtoParser(const char* buffer, const uint16 buffersize)
		{
			parse(buffer, buffersize);
		}
		bool parse(const char* buffer, const uint16 buffersize)
		{
			assert(buffersize >= sizeof(uint16));
			assert(buffer != nullptr);
			_nullcmd = *(zNullCmd*)buffer;
			if (buffersize >= sizeof(uint16))
			{
				_flag = _protoMsg.ParseFromArray((BYTE *)buffer + sizeof(uint16), buffersize - sizeof(uint16));
			}
			else if (buffersize == sizeof(uint16))
			{
				_flag = true;
			}
		}
	};
}
