//#include <ntddk.h>
//#include <windef.h>
//#include <tdikrnl.h>
//#include <tdi.h>
//
//#define HTONS(a) (((0xFF&a)<<8) + ((0xFF00&a)>>8))
//PDEVICE_OBJECT pfilterdevobj = NULL;
//PDEVICE_OBJECT pdodevobj = NULL;
//typedef struct _NETWORK_ADDRESS {
//	CHAR address[4];
//	CHAR port[2];
//}NETWORK_ADDRESS, *PNETWORK_ADDRESS;
//
//VOID DrvUnload(PDRIVER_OBJECT driver) {
//	IoDetachDevice(pdodevobj);
//	IoDeleteDevice(pfilterdevobj);
//}
//NTSTATUS NotSupported(PDEVICE_OBJECT device, PIRP pirp) {
//	IoSkipCurrentIrpStackLocation(pirp);
//	return IoCallDriver(pdodevobj, pirp);
//}
//NTSTATUS Dispatch(PDEVICE_OBJECT device, PIRP pirp) {
//	PIO_STACK_LOCATION pirpstack = NULL;
//	if (device == pfilterdevobj) {
//		pirpstack = IoGetCurrentIrpStackLocation(pirp);
//		if (pirpstack == NULL) {
//			return STATUS_UNSUCCESSFUL;
//		}
//		if (pirpstack->MinorFunction == TDI_CONNECT) {
//			PTDI_REQUEST_KERNEL_CONNECT ptdiconnect = (PTDI_REQUEST_KERNEL_CONNECT)(&pirpstack->Parameters);
//			PTA_ADDRESS ta_addr = ((PTRANSPORT_ADDRESS)(ptdiconnect->RequestConnectionInformation->RemoteAddress))->Address;
//			PTDI_ADDRESS_IP tdi_addr = (PTDI_ADDRESS_IP)(ta_addr->Address);
//			DWORD address = tdi_addr->in_addr;
//			unsigned short port = 0;
//			port = tdi_addr->sin_port;
//			NETWORK_ADDRESS data = { 0 };
//			data.address[0] = ((PCHAR)&address)[0];
//			data.address[1] = ((PCHAR)&address)[1];
//			data.address[2] = ((PCHAR)&address)[2];
//			data.address[3] = ((PCHAR)&address)[3];
//
//			data.port[0] = ((char*)&port)[0];
//			data.port[1] = ((char*)&port)[1];
//			//port = (data.port[0] * 256) + data.port[1];
//			port = HTONS(port);
//			KdPrint(("connect %d-%d-%d-%d:%d", 
//				data.address[0], data.address[1], data.address[2], data.address[3], port));
//			IoSkipCurrentIrpStackLocation(pirp);
//			return IoCallDriver(pdodevobj, pirp);
//		}
//	}
//	IoSkipCurrentIrpStackLocation(pirp);
//	return IoCallDriver(pdodevobj, pirp);
//}
//
//NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING regPath) {
//	NTSTATUS status = STATUS_SUCCESS;
//	driver->DriverUnload = DrvUnload;
//	status = IoCreateDevice(driver, 0, NULL, FILE_DEVICE_NETWORK, FILE_DEVICE_SECURE_OPEN, FALSE, &pfilterdevobj);
//	if (!NT_SUCCESS(status)) {
//		return status;
//	}
//	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
//		driver->MajorFunction[i] = NotSupported;
//	}
//	driver->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = Dispatch;
//
//	UNICODE_STRING devicename = { 0 };
//	RtlInitUnicodeString(&devicename, L"\\Device\\Tcp");
//	status = IoAttachDevice(pfilterdevobj, &devicename, &pdodevobj);
//	if (!NT_SUCCESS(status)) {
//		IoDeleteDevice(pfilterdevobj);
//		return status;
//	}
//	return status;
//}