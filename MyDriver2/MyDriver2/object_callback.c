//#include <ntifs.h>
//#include <ntddk.h>
//
//typedef struct _LDR_DATA_TABLE_ENTRY {
//	LIST_ENTRY64 InLoadOrderLinks;
//	LIST_ENTRY64 InMemoryOrderLinks;
//	LIST_ENTRY64 InInitializationOrderLinks;
//	PVOID DllBase;
//	PVOID EntryPoint;
//	ULONG SizeOfImage;
//	UNICODE_STRING FullDllName;
//	UNICODE_STRING BaseDllName;
//	ULONG Flags;
//	USHORT LoadCount;
//	USHORT TlsIndex;
//	PVOID SectionPointer;
//	ULONG CheckSum;
//	PVOID LoadedImports;
//	PVOID EntryPointActivationContext;
//	PVOID PatchInformation;
//	LIST_ENTRY64 ForwarderLinks;
//	LIST_ENTRY64 ServiceTagLinks;
//	LIST_ENTRY64 StaticLinks;
//	PVOID ContextInformation;
//	ULONG OriginalBase;
//	LARGE_INTEGER LoadTime;
//}LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
//
//NTKERNELAPI
//UCHAR* PsGetProcessImageFileName(PEPROCESS Process);
//
//OB_PREOP_CALLBACK_STATUS protectProcess(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation) {
//	PUCHAR imageFilename = PsGetProcessImageFileName(OperationInformation->Object);
//
//	if (strstr(imageFilename, "calc")) {
//		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
//		OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
//	}
//
//	return OB_PREOP_SUCCESS;
//}
//PVOID handle = NULL;
//VOID DriverUnload(PDRIVER_OBJECT pdriver) {
//	ObUnRegisterCallbacks(handle);
//}
//NTSTATUS DriverEntry(PDRIVER_OBJECT pdriver, PUNICODE_STRING regPath) {
//	pdriver->DriverUnload = DriverUnload;
//
//	NTSTATUS status = STATUS_SUCCESS;
//	PLDR_DATA_TABLE_ENTRY ldr = (PLDR_DATA_TABLE_ENTRY)pdriver->DriverSection;
//	ldr->Flags |= 0x20;
//
//	OB_CALLBACK_REGISTRATION ob = { 0 };
//	OB_OPERATION_REGISTRATION oor = { 0 };
//
//	ob.Version = ObGetFilterVersion();
//	ob.OperationRegistrationCount = 1;
//	ob.OperationRegistration = &oor;
//	UNICODE_STRING attde = { 0 };
//	RtlInitUnicodeString(&attde, L"321999");
//	ob.RegistrationContext = NULL;
//	
//	oor.ObjectType = PsProcessType;
//	oor.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
//	oor.PreOperation = protectProcess;
//	oor.PostOperation = NULL;
//
//	status = ObRegisterCallbacks(&ob, &handle);
//
//	return status;
//}