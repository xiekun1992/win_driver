//#include <ntddk.h>
//
//UINT8 DayOfMon[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
//#define SECPMD_OF_DAY 86400
//ULONG BanedTime = 1410065407;
//
//extern POBJECT_TYPE* PsThreadType;
//BOOLEAN TimeSwitch = FALSE;
//PETHREAD pThreadObj;
//
//NTSTATUS Unload(PDRIVER_OBJECT driver) {
//	KdPrint(("unload"));
//	TimeSwitch = TRUE;
//	KeWaitForSingleObject(pThreadObj, Executive, KernelMode, FALSE, NULL);
//	//ObDereferenceObject(pThreadObj);
//	return STATUS_SUCCESS;
//}
//BOOLEAN CheckTimeLocal() {
//	LARGE_INTEGER snow, now, tickcount;
//	TIME_FIELDS now_fields;
//
//	KeQuerySystemTime(&snow);
//	ExSystemTimeToLocalTime(&snow, &now);
//	RtlTimeToTimeFields(&now, &now_fields);
//
//	KdPrint(("%d-%d-%d\n", now_fields.Year, now_fields.Month, now_fields.Day));
//
//	UINT16 iYear, iMon, iDay, iHour, iMin, iSec;
//	iYear = now_fields.Year;
//	iMon = now_fields.Month;
//	iDay = now_fields.Day;
//	iHour = now_fields.Hour;
//	iMin = now_fields.Minute;
//	iSec = now_fields.Second;
//	SHORT i, Cyear = 0;
//	ULONG CountDay = 0;
//	for (i = 1970; i < iYear; i++) {
//		if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0)) {
//			Cyear++;
//		}
//	}
//	CountDay = Cyear * 366 + (iYear - 1970 - Cyear) * 365;
//	for (i = 1; i < iMon; i++) {
//		if ((i == 2) && (iYear % 4 == 0) && (iYear % 100 != 0) || (iYear % 400 == 0)) {
//			CountDay += 29;
//		}
//		else {
//			CountDay += DayOfMon[i - 1];
//		}
//	}
//	CountDay += (iDay - 1);
//	CountDay = CountDay * SECPMD_OF_DAY + (unsigned long)iHour * 3600 + (unsigned long)iMin * 60 + iSec;
//	KdPrint(("%d", CountDay));
//	return TRUE;
//	if (CountDay < BanedTime) {
//	}
//	return FALSE;
//}
//VOID CheckTimeThread() {
//	LARGE_INTEGER SleepTime;
//	SleepTime.QuadPart = -5 * 1000 * 1000 * 10;
//	while (1) {
//		if (TimeSwitch) {
//			break;
//		}
//		if (!CheckTimeLocal()) {
//			KdPrint(("driver invalid"));
//		}
//		KeDelayExecutionThread(KernelMode, FALSE, &SleepTime);
//	}
//	PsTerminateSystemThread(STATUS_SUCCESS);
//}
//NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING regPath) {
//	driver->DriverUnload = Unload;
//	if (!CheckTimeLocal()) {
//		return STATUS_NOT_SUPPORTED;
//	}
//	OBJECT_ATTRIBUTES ObjAddr = { 0 };
//	HANDLE ThreadHandle = 0;
//	InitializeObjectAttributes(&ObjAddr, NULL, OBJ_KERNEL_HANDLE, 0, NULL);
//	NTSTATUS status = PsCreateSystemThread(&ThreadHandle, THREAD_ALL_ACCESS, &ObjAddr, NULL, NULL, CheckTimeThread, NULL);
//	if (!NT_SUCCESS(status)) {
//		return STATUS_NOT_SUPPORTED;
//	}
//	status = ObReferenceObjectByHandle(ThreadHandle, THREAD_ALL_ACCESS, *PsThreadType, KernelMode, &pThreadObj, NULL);
//	if (!NT_SUCCESS(status)) {
//		ZwClose(ThreadHandle);
//		return STATUS_NOT_SUPPORTED;
//	}
//	ZwClose(ThreadHandle);
//	KdPrint(("driver loaded"));
//	return STATUS_SUCCESS;
//}