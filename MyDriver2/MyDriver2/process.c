//#include <ntddk.h>
//
//VOID nothing(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create) {
//	KdPrint(("process notified"));
//}
//VOID DriverUnload(PDRIVER_OBJECT driver) {
//	PsSetCreateProcessNotifyRoutine(nothing, TRUE);
//}
//NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING regPath) {
//	driver->DriverUnload = DriverUnload;
//
//	PsSetCreateProcessNotifyRoutine(nothing, FALSE);
//	return STATUS_SUCCESS;
//}