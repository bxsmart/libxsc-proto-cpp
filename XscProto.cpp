/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "XscProto.h"

typedef struct
{
	char d[0x40];
} xsc_ie; 

static xsc_ie __xsc_transm_ie__[] = { 
		{ "PONG" },
		{ "HEADER" },
		{ "QOS" },
		{ "ROUTE" },
		{ "SECURITY" },
		{ "TRACE" },
		{ "OOB" } };

static xsc_ie __xsc_trans_ie__[] = { 
		{ "BEGIN" },
		{ "END" },
		{ "CONTINUE" },
		{ "DIALOG" },
		{ "CANCEL" },
		{ "ABORT" },
		{ "UNIDIRECTION" },
		{ "PARTIAL" } };

static xsc_ie __xsc_common_ie__[] = { 
		{ "STID" },
		{ "DTID" },
		{ "TID" },
		{ "UID" },
		{ "SID" },
		{ "MSG" },
		{ "RET" },
		{ "DAT" },
		{ "SNE" },
		{ "MNE" },
		{ "DNE" },
		{ "HAVE-NEXT-PART" },
		{ "SEQUENCE" },
		{ "ACTION" },
		{ "DESC" },
		{ "QOS" },
		{ "ALGORITHM" },
		{ "CHECK-SUM" },
		{ "TRACE-ID" },
		{ "SPAN-ID" },
		{ "P-SPAN-ID" },
		{ "TIME-STAMP" },
		{ "GENERATE-TIME-STAMP" },
		{ "UPDATE-TIME-STAMP" },
		{ "PLATFORM" },
		{ "CGT" },
		{ "INTERCEPT" },
		{ "CLIENT_OOB" },
		{ "DEVICE_ID" },
		{ "NE_GROUP" } };

static xsc_ie __xsc_ret__[] = { 
		{ "SUCCESS" },
		{ "FAILURE" },
		{ "INVALID" },
		{ "PRESENT" },
		{ "NOT_PRESENT" },
		{ "EXCEPTION" },
		{ "NOT_FOUND" },
		{ "TIME_OUT" },
		{ "FORBIDDEN" },
		{ "FORMAT_ERROR" },
		{ "MISSING_PARAMETER" },
		{ "UNSUPPORTED" },
		{ "NO_AUTH" },
		{ "NO_PERMISSION" },
		{ "NO_RECORD" },
		{ "OVER_LIMIT" },
		{ "DUPLICATE_OPER" },
		{ "UPDATE" },
		{ "NOT_UPDATE" },
		{ "ROUTE_FAILED" },
		{ "CONNECTION_DISC" },
		{ "USR_OR_PASSWORD_ERROR" },
		};

static const char* __XSC_IU__ = "IEI_UNKNOWN";
static const char* __XSC_PING__ = "PING";

XscProto::XscProto()
{

}

const char* XscProto::tagDesc(uchar t)
{
	if (t == XSC_TAG_TRANSM_PING)
		return __XSC_PING__;
	if (t >= XSC_TAG_TRANSM_PONG && t <= XSC_TAG_TRANSM_OOB)
		return __xsc_transm_ie__[t - XSC_TAG_TRANSM_PONG].d;
	if (t >= XSC_TAG_TRANS_BEGIN && t <= XSC_TAG_TRANS_PARTIAL)
		return __xsc_trans_ie__[t - XSC_TAG_TRANS_BEGIN].d;
	if (t >= XSC_TAG_STID && t <= XSC_TAG_NE_GROUP)
		return __xsc_common_ie__[t].d;
	return __XSC_IU__;
}

string XscProto::v2str(xsc_tlv* node)
{
	switch (node->t)
	{
	case XSC_TAG_MSG:
	case XSC_TAG_SNE:
	case XSC_TAG_MNE:
	case XSC_TAG_DNE:
	case XSC_TAG_ACTION:
	case XSC_TAG_DESC:
	case XSC_TAG_PLATFORM:
	case XSC_TAG_CGT:
	case XSC_TAG_DEVICE_ID:
	case XSC_TAG_NE_GROUP:
		return string((char*) node->v, node->l);
	case XSC_TAG_TRACE_ID:
	case XSC_TAG_SPAN_ID:
	case XSC_TAG_P_SPAN_ID:
		return Net::hex2strLowerCase(node->v, node->l);
	case XSC_TAG_TS:
	case XSC_TAG_GTS:
	case XSC_TAG_UTS:
		return DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(Net::ntohll(*((ullong*) node->v))).c_str();
	case XSC_TAG_RET:
	{
		ushort ret = htons(*((ushort*) node->v));
		return (ret >= RET_SUCCESS && ret <= RET_USR_OR_PASSWORD_ERROR) ? __xsc_ret__[ret].d : Net::hex2strUperCaseSpace(node->v, node->l);
	}
	default:
		return Net::hex2strUperCaseSpace(node->v, node->l);
	}
}

uchar XscProto::tlvLen(uint len)
{
	return len < 0x000000FE ? 1 : (len <= 0x0000FFFF ? 3 : 5);
}

XscProto::~XscProto()
{

}

