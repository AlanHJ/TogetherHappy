/* soapService.cpp
   Generated by gSOAP 2.8.23 from happyservice.h

Copyright(C) 2000-2015, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "soapService.h"

Service::Service() : soap(SOAP_IO_DEFAULT)
{	Service_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

Service::Service(const struct soap &_soap) : soap(_soap)
{ }

Service::Service(soap_mode iomode) : soap(iomode)
{	Service_init(iomode, iomode);
}

Service::Service(soap_mode imode, soap_mode omode) : soap(imode, omode)
{	Service_init(imode, omode);
}

Service::~Service()
{
	this->destroy();
}

void Service::Service_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"happy", "http://tempuri.org/happy.xsd", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this, namespaces);
}

void Service::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void Service::reset()
{	this->destroy();
	soap_done(this);
	soap_initialize(this);
	Service_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

#ifndef WITH_PURE_VIRTUAL
Service *Service::copy()
{	Service *dup = SOAP_NEW_COPY(Service(*(struct soap*)this));
	return dup;
}
#endif

int Service::soap_close_socket()
{	return soap_closesock(this);
}

int Service::soap_force_close_socket()
{	return soap_force_closesock(this);
}

int Service::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this, string, detailXML);
}

int Service::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this, subcodeQName, string, detailXML);
}

int Service::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this, string, detailXML);
}

int Service::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this, subcodeQName, string, detailXML);
}

void Service::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void Service::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *Service::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

void Service::soap_noheader()
{	this->header = NULL;
}

const SOAP_ENV__Header *Service::soap_header()
{	return this->header;
}

int Service::run(int port)
{	if (soap_valid_socket(this->master) || soap_valid_socket(bind(NULL, port, 100)))
	{	for (;;)
		{	if (!soap_valid_socket(accept()) || serve())
				break;
			soap_destroy(this);
			soap_end(this);
		}
	}
	return this->error;
}

SOAP_SOCKET Service::bind(const char *host, int port, int backlog)
{	return soap_bind(this, host, port, backlog);
}

SOAP_SOCKET Service::accept()
{	return soap_accept(this);
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int Service::ssl_accept()
{	return soap_ssl_accept(this);
}
#endif

int Service::serve()
{
#ifndef WITH_FASTCGI
	unsigned int k = this->max_keep_alive;
#endif
	do
	{

#ifndef WITH_FASTCGI
		if (this->max_keep_alive > 0 && !--k)
			this->keep_alive = 0;
#endif

		if (soap_begin_serve(this))
		{	if (this->error >= SOAP_STOP)
				continue;
			return this->error;
		}
		if (dispatch() || (this->fserveloop && this->fserveloop(this)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(this);
#else
			return soap_send_fault(this);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(this);
		soap_end(this);
	} while (1);
#else
	} while (this->keep_alive);
#endif
	return SOAP_OK;
}

static int serve_happy__ChatOneByOne(Service*);
static int serve_happy__ChatShake(Service*);
static int serve_happy__GetResource(Service*);
static int serve_happy__SetResource(Service*);
static int serve_happy__ChatGroup(Service*);
static int serve_happy__SendFileExec(Service*);
static int serve_happy__SendFileAccept(Service*);
static int serve_happy__UpdateHappy(Service*);
static int serve_happy__InviteGroup(Service*);
static int serve_happy__InviteGroupRespond(Service*);
static int serve_happy__UpdateGroupUser(Service*);

int Service::dispatch()
{	soap_peek_element(this);
	if (!soap_match_tag(this, this->tag, "happy:ChatOneByOne"))
		return serve_happy__ChatOneByOne(this);
	if (!soap_match_tag(this, this->tag, "happy:ChatShake"))
		return serve_happy__ChatShake(this);
	if (!soap_match_tag(this, this->tag, "happy:GetResource"))
		return serve_happy__GetResource(this);
	if (!soap_match_tag(this, this->tag, "happy:SetResource"))
		return serve_happy__SetResource(this);
	if (!soap_match_tag(this, this->tag, "happy:ChatGroup"))
		return serve_happy__ChatGroup(this);
	if (!soap_match_tag(this, this->tag, "happy:SendFileExec"))
		return serve_happy__SendFileExec(this);
	if (!soap_match_tag(this, this->tag, "happy:SendFileAccept"))
		return serve_happy__SendFileAccept(this);
	if (!soap_match_tag(this, this->tag, "happy:UpdateHappy"))
		return serve_happy__UpdateHappy(this);
	if (!soap_match_tag(this, this->tag, "happy:InviteGroup"))
		return serve_happy__InviteGroup(this);
	if (!soap_match_tag(this, this->tag, "happy:InviteGroupRespond"))
		return serve_happy__InviteGroupRespond(this);
	if (!soap_match_tag(this, this->tag, "happy:UpdateGroupUser"))
		return serve_happy__UpdateGroupUser(this);
	return this->error = SOAP_NO_METHOD;
}

static int serve_happy__ChatOneByOne(Service *soap)
{	struct happy__ChatOneByOne soap_tmp_happy__ChatOneByOne;
	struct happy__ChatOneByOneResponse soap_tmp_happy__ChatOneByOneResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__ChatOneByOneResponse(soap, &soap_tmp_happy__ChatOneByOneResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__ChatOneByOneResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__ChatOneByOne(soap, &soap_tmp_happy__ChatOneByOne);
	if (!soap_get_happy__ChatOneByOne(soap, &soap_tmp_happy__ChatOneByOne, "happy:ChatOneByOne", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->ChatOneByOne(soap_tmp_happy__ChatOneByOne.key, soap_tmp_happy__ChatOneByOne.content, soap_tmp_happy__ChatOneByOneResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__ChatOneByOneResponse(soap, &soap_tmp_happy__ChatOneByOneResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__ChatOneByOneResponse(soap, &soap_tmp_happy__ChatOneByOneResponse, "happy:ChatOneByOneResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__ChatOneByOneResponse(soap, &soap_tmp_happy__ChatOneByOneResponse, "happy:ChatOneByOneResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__ChatShake(Service *soap)
{	struct happy__ChatShake soap_tmp_happy__ChatShake;
	struct happy__ChatShakeResponse soap_tmp_happy__ChatShakeResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__ChatShakeResponse(soap, &soap_tmp_happy__ChatShakeResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__ChatShakeResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__ChatShake(soap, &soap_tmp_happy__ChatShake);
	if (!soap_get_happy__ChatShake(soap, &soap_tmp_happy__ChatShake, "happy:ChatShake", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->ChatShake(soap_tmp_happy__ChatShake.key, soap_tmp_happy__ChatShake.content, soap_tmp_happy__ChatShakeResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__ChatShakeResponse(soap, &soap_tmp_happy__ChatShakeResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__ChatShakeResponse(soap, &soap_tmp_happy__ChatShakeResponse, "happy:ChatShakeResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__ChatShakeResponse(soap, &soap_tmp_happy__ChatShakeResponse, "happy:ChatShakeResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__GetResource(Service *soap)
{	struct happy__GetResource soap_tmp_happy__GetResource;
	struct happy__GetResourceResponse soap_tmp_happy__GetResourceResponse;
	int soap_tmp_int;
	soap_default_happy__GetResourceResponse(soap, &soap_tmp_happy__GetResourceResponse);
	soap_default_int(soap, &soap_tmp_int);
	soap_tmp_happy__GetResourceResponse.len = &soap_tmp_int;
	soap_default_happy__GetResource(soap, &soap_tmp_happy__GetResource);
	if (!soap_get_happy__GetResource(soap, &soap_tmp_happy__GetResource, "happy:GetResource", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetResource(soap_tmp_happy__GetResource.srcuid, soap_tmp_happy__GetResource.srcuip, soap_tmp_happy__GetResource.type, soap_tmp_happy__GetResource.resource, soap_tmp_happy__GetResourceResponse.len);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__GetResourceResponse(soap, &soap_tmp_happy__GetResourceResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__GetResourceResponse(soap, &soap_tmp_happy__GetResourceResponse, "happy:GetResourceResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__GetResourceResponse(soap, &soap_tmp_happy__GetResourceResponse, "happy:GetResourceResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__SetResource(Service *soap)
{	struct happy__SetResource soap_tmp_happy__SetResource;
	struct happy__SetResourceResponse soap_tmp_happy__SetResourceResponse;
	int soap_tmp_int;
	soap_default_happy__SetResourceResponse(soap, &soap_tmp_happy__SetResourceResponse);
	soap_default_int(soap, &soap_tmp_int);
	soap_tmp_happy__SetResourceResponse.len = &soap_tmp_int;
	soap_default_happy__SetResource(soap, &soap_tmp_happy__SetResource);
	if (!soap_get_happy__SetResource(soap, &soap_tmp_happy__SetResource, "happy:SetResource", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SetResource(soap_tmp_happy__SetResource.desuid, soap_tmp_happy__SetResource.type, soap_tmp_happy__SetResource.resource, soap_tmp_happy__SetResource.resname, soap_tmp_happy__SetResourceResponse.len);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__SetResourceResponse(soap, &soap_tmp_happy__SetResourceResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__SetResourceResponse(soap, &soap_tmp_happy__SetResourceResponse, "happy:SetResourceResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__SetResourceResponse(soap, &soap_tmp_happy__SetResourceResponse, "happy:SetResourceResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__ChatGroup(Service *soap)
{	struct happy__ChatGroup soap_tmp_happy__ChatGroup;
	struct happy__ChatGroupResponse soap_tmp_happy__ChatGroupResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__ChatGroupResponse(soap, &soap_tmp_happy__ChatGroupResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__ChatGroupResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__ChatGroup(soap, &soap_tmp_happy__ChatGroup);
	if (!soap_get_happy__ChatGroup(soap, &soap_tmp_happy__ChatGroup, "happy:ChatGroup", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->ChatGroup(soap_tmp_happy__ChatGroup.gkey, soap_tmp_happy__ChatGroup.key, soap_tmp_happy__ChatGroup.content, soap_tmp_happy__ChatGroupResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__ChatGroupResponse(soap, &soap_tmp_happy__ChatGroupResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__ChatGroupResponse(soap, &soap_tmp_happy__ChatGroupResponse, "happy:ChatGroupResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__ChatGroupResponse(soap, &soap_tmp_happy__ChatGroupResponse, "happy:ChatGroupResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__SendFileExec(Service *soap)
{	struct happy__SendFileExec soap_tmp_happy__SendFileExec;
	struct happy__SendFileExecResponse soap_tmp_happy__SendFileExecResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__SendFileExecResponse(soap, &soap_tmp_happy__SendFileExecResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__SendFileExecResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__SendFileExec(soap, &soap_tmp_happy__SendFileExec);
	if (!soap_get_happy__SendFileExec(soap, &soap_tmp_happy__SendFileExec, "happy:SendFileExec", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SendFileExec(soap_tmp_happy__SendFileExec.key, soap_tmp_happy__SendFileExec.content, soap_tmp_happy__SendFileExecResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__SendFileExecResponse(soap, &soap_tmp_happy__SendFileExecResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__SendFileExecResponse(soap, &soap_tmp_happy__SendFileExecResponse, "happy:SendFileExecResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__SendFileExecResponse(soap, &soap_tmp_happy__SendFileExecResponse, "happy:SendFileExecResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__SendFileAccept(Service *soap)
{	struct happy__SendFileAccept soap_tmp_happy__SendFileAccept;
	struct happy__SendFileAcceptResponse soap_tmp_happy__SendFileAcceptResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__SendFileAcceptResponse(soap, &soap_tmp_happy__SendFileAcceptResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__SendFileAcceptResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__SendFileAccept(soap, &soap_tmp_happy__SendFileAccept);
	if (!soap_get_happy__SendFileAccept(soap, &soap_tmp_happy__SendFileAccept, "happy:SendFileAccept", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SendFileAccept(soap_tmp_happy__SendFileAccept.key, soap_tmp_happy__SendFileAccept.content, soap_tmp_happy__SendFileAcceptResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__SendFileAcceptResponse(soap, &soap_tmp_happy__SendFileAcceptResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__SendFileAcceptResponse(soap, &soap_tmp_happy__SendFileAcceptResponse, "happy:SendFileAcceptResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__SendFileAcceptResponse(soap, &soap_tmp_happy__SendFileAcceptResponse, "happy:SendFileAcceptResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__UpdateHappy(Service *soap)
{	struct happy__UpdateHappy soap_tmp_happy__UpdateHappy;
	struct happy__UpdateHappyResponse soap_tmp_happy__UpdateHappyResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__UpdateHappyResponse(soap, &soap_tmp_happy__UpdateHappyResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__UpdateHappyResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__UpdateHappy(soap, &soap_tmp_happy__UpdateHappy);
	if (!soap_get_happy__UpdateHappy(soap, &soap_tmp_happy__UpdateHappy, "happy:UpdateHappy", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->UpdateHappy(soap_tmp_happy__UpdateHappy.key, soap_tmp_happy__UpdateHappy.content, soap_tmp_happy__UpdateHappyResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__UpdateHappyResponse(soap, &soap_tmp_happy__UpdateHappyResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__UpdateHappyResponse(soap, &soap_tmp_happy__UpdateHappyResponse, "happy:UpdateHappyResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__UpdateHappyResponse(soap, &soap_tmp_happy__UpdateHappyResponse, "happy:UpdateHappyResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__InviteGroup(Service *soap)
{	struct happy__InviteGroup soap_tmp_happy__InviteGroup;
	struct happy__InviteGroupResponse soap_tmp_happy__InviteGroupResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__InviteGroupResponse(soap, &soap_tmp_happy__InviteGroupResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__InviteGroupResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__InviteGroup(soap, &soap_tmp_happy__InviteGroup);
	if (!soap_get_happy__InviteGroup(soap, &soap_tmp_happy__InviteGroup, "happy:InviteGroup", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->InviteGroup(soap_tmp_happy__InviteGroup.builder, soap_tmp_happy__InviteGroup.title, soap_tmp_happy__InviteGroup.gkey, soap_tmp_happy__InviteGroup.members, soap_tmp_happy__InviteGroupResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__InviteGroupResponse(soap, &soap_tmp_happy__InviteGroupResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__InviteGroupResponse(soap, &soap_tmp_happy__InviteGroupResponse, "happy:InviteGroupResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__InviteGroupResponse(soap, &soap_tmp_happy__InviteGroupResponse, "happy:InviteGroupResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__InviteGroupRespond(Service *soap)
{	struct happy__InviteGroupRespond soap_tmp_happy__InviteGroupRespond;
	struct happy__InviteGroupRespondResponse soap_tmp_happy__InviteGroupRespondResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__InviteGroupRespondResponse(soap, &soap_tmp_happy__InviteGroupRespondResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__InviteGroupRespondResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__InviteGroupRespond(soap, &soap_tmp_happy__InviteGroupRespond);
	if (!soap_get_happy__InviteGroupRespond(soap, &soap_tmp_happy__InviteGroupRespond, "happy:InviteGroupRespond", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->InviteGroupRespond(soap_tmp_happy__InviteGroupRespond.key, soap_tmp_happy__InviteGroupRespond.gkey, soap_tmp_happy__InviteGroupRespond.content, soap_tmp_happy__InviteGroupRespondResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__InviteGroupRespondResponse(soap, &soap_tmp_happy__InviteGroupRespondResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__InviteGroupRespondResponse(soap, &soap_tmp_happy__InviteGroupRespondResponse, "happy:InviteGroupRespondResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__InviteGroupRespondResponse(soap, &soap_tmp_happy__InviteGroupRespondResponse, "happy:InviteGroupRespondResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_happy__UpdateGroupUser(Service *soap)
{	struct happy__UpdateGroupUser soap_tmp_happy__UpdateGroupUser;
	struct happy__UpdateGroupUserResponse soap_tmp_happy__UpdateGroupUserResponse;
	std::wstring soap_tmp_std__wstring;
	soap_default_happy__UpdateGroupUserResponse(soap, &soap_tmp_happy__UpdateGroupUserResponse);
	soap_default_std__wstring(soap, &soap_tmp_std__wstring);
	soap_tmp_happy__UpdateGroupUserResponse.result = &soap_tmp_std__wstring;
	soap_default_happy__UpdateGroupUser(soap, &soap_tmp_happy__UpdateGroupUser);
	if (!soap_get_happy__UpdateGroupUser(soap, &soap_tmp_happy__UpdateGroupUser, "happy:UpdateGroupUser", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->UpdateGroupUser(soap_tmp_happy__UpdateGroupUser.gkey, soap_tmp_happy__UpdateGroupUser.newusers, soap_tmp_happy__UpdateGroupUserResponse.result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_happy__UpdateGroupUserResponse(soap, &soap_tmp_happy__UpdateGroupUserResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_happy__UpdateGroupUserResponse(soap, &soap_tmp_happy__UpdateGroupUserResponse, "happy:UpdateGroupUserResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_happy__UpdateGroupUserResponse(soap, &soap_tmp_happy__UpdateGroupUserResponse, "happy:UpdateGroupUserResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */