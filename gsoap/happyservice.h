#ifndef HAPPYSERVICE
#define HAPPYSERVICE
//gsoap ns service name:	happy soa service
//gsoap ns service protocol:	SOAP
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://localhost/add.wsdl
//gsoap ns service location:	http://localhost
//gsoap ns service executable:  happy.cgi
//gsoap ns schema namespace:	urn:happy

int happy__ChatOneByOne(std::wstring key, std::wstring content, std::wstring *result);
int happy__ChatShake(std::wstring key, std::wstring content, std::wstring *result);
int happy__GetResource(std::wstring srcuid, std::wstring srcuip, std::wstring type, std::wstring resource, int *len);
int happy__SetResource(std::wstring desuid, std::wstring type, std::wstring resource, std::wstring resname, int *len);

// Q_UNUSED
int happy__ChatGroup(std::wstring gkey, std::wstring key, std::wstring content, std::wstring *result);
int happy__SendFileExec(std::wstring key, std::wstring content, std::wstring *result);
int happy__SendFileAccept(std::wstring key, std::wstring content, std::wstring *result);
int happy__UpdateHappy(std::wstring key, std::wstring content, std::wstring *result);
int happy__InviteGroup(std::wstring builder, std::wstring title, std::wstring gkey, std::wstring members, std::wstring *result);
int happy__InviteGroupRespond(std::wstring key, std::wstring gkey, std::wstring content, std::wstring *result);
int happy__UpdateGroupUser(std::wstring gkey, std::wstring newusers, std::wstring *result);

#endif // HAPPYSERVICE

