//#include <ntifs.h>
//#include <ntddk.h>
//
//LARGE_INTEGER cookie = { 0 };
//VOID DriverUnload(PDRIVER_OBJECT pdriver) {
//	CmUnRegisterCallback(cookie);
//}
//VOID registerCallback(PVOID CallbackContext, PVOID Argument1, PVOID Argument2) {
//	NTSTATUS status = STATUS_SUCCESS;
//	REG_NOTIFY_CLASS cls = (REG_NOTIFY_CLASS)Argument1;
//	UNICODE_STRING name = { 0 };
//	RtlInitUnicodeString(&name, L"*TEST");
//
//	switch (cls) {
//		case RegNtPreOpenKey:
//		case RegNtPreOpenKeyEx:
//		case RegNtPreCreateKey:
//		case RegNtPreCreateKeyEx: {
//			PREG_CREATE_KEY_INFORMATION pkeyInfo = (PREG_CREATE_KEY_INFORMATION)Argument2;
//			__try {
//				if (FsRtlIsNameInExpression(&name, pkeyInfo->CompleteName, TRUE, NULL)) {
//					status = STATUS_UNSUCCESSFUL;
//				}
//				//KdPrint(("key info %wZ", pkeyInfo->CompleteName));
//			} except(EXCEPTION_EXECUTE_HANDLER) {
//				KdPrint(("bad memory"));
//			}
//		}; break;
//		default: break;
//	}
//	return status;
//}
//NTSTATUS DriverEntry(PDRIVER_OBJECT pdriver, PUNICODE_STRING regPath) {
//	pdriver->DriverUnload = DriverUnload;
//
//	NTSTATUS status = STATUS_SUCCESS;
//	status = CmRegisterCallback(registerCallback, NULL, &cookie);
//
//	return status;
//}