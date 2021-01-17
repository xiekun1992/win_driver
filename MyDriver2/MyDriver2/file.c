//#include <ntifs.h>
//#include <ntddk.h>
//
//NTSTATUS KernelDeleteFile(PWCHAR filePath) {
//	UNICODE_STRING targetfilePath = { 0 };
//	NTSTATUS status = STATUS_SUCCESS;
//	OBJECT_ATTRIBUTES attr = { 0 };
//
//	RtlInitUnicodeString(&targetfilePath, filePath);
//	InitializeObjectAttributes(&attr, &targetfilePath, OBJ_CASE_INSENSITIVE, NULL, NULL);
//	
//	status = ZwDeleteFile(&attr);
//	if (!NT_SUCCESS(status)) {
//		KdPrint(("delete file failed"));
//	}
//	return status;
//}
//VOID DriverUnload(PDRIVER_OBJECT driver) {
//
//}
//VOID DeferredRoutine(
//	KDPC* Dpc,
//	PVOID DeferredContext,
//	PVOID SystemArgument1,
//	PVOID SystemArgument2) {
//	KdPrint(("%d", KeGetCurrentIrql()));
//	return;
//}
//NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING regPath) {
//	driver->DriverUnload = DriverUnload;
//	
//	KIRQL irql = KeGetCurrentIrql();
//	KIRQL prevIrql = KeRaiseIrqlToDpcLevel();
//	KeLowerIrql(prevIrql);
//
//	KSPIN_LOCK spinLock = 0;
//	KIRQL kirql = 0;
//	KeInitializeSpinLock(&spinLock);
//	KeAcquireSpinLock(&spinLock, &kirql);
//	KeReleaseSpinLock(&spinLock, kirql);
//
//	KDPC dpc = { 0 };
//	KeInitializeDpc(&dpc, DeferredRoutine, NULL);
//	KeInsertQueueDpc(&dpc, NULL, NULL);
//
//	return STATUS_SUCCESS;
//}