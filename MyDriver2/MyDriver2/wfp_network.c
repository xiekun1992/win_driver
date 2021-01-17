#include "ntddk.h"
#include <fwpsk.h>
#define INITGUID
#include <guiddef.h>
#include <fwpmk.h>

DEFINE_GUID(WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID, 0xd969fc67, 0x6fb2, 0x4504, 0x91, 0xce, 0xa9, 0x7c, 0x3c, 0x32, 0xad, 0x36);
DEFINE_GUID(WFP_SAMPLE_SUB_LAYER_GUID, 0xed6a516a, 0x36d1, 0x4881, 0xbc, 0xf0, 0xac, 0xeb, 0x4c, 0x4, 0xc2, 0x1c);

PDEVICE_OBJECT DeviceObject = NULL;
HANDLE EngineHandle = NULL;
UINT32 RegCalloutId = 0, AddCalloutId = 0;
UINT64 filterid = 0;

VOID UnInitWfp() {
	if (EngineHandle != NULL) {
		if (filterid != 0) {
			FwpmFilterDeleteById(EngineHandle, filterid);
			FwpmSubLayerDeleteByKey(EngineHandle, &WFP_SAMPLE_SUB_LAYER_GUID);
		}
		if (AddCalloutId != 0) {
			FwpmCalloutDeleteById(EngineHandle, AddCalloutId);
		}
		if (RegCalloutId != 0) {
			FwpsCalloutUnregisterById(RegCalloutId);
		}
		FwpmEngineClose(EngineHandle);
	}
}
VOID Unload() {
	UnInitWfp();
	IoDeleteDevice(DeviceObject);
	KdPrint(("unload"));
}
NTSTATUS NotifyCallback(FWPS_CALLOUT_NOTIFY_TYPE type, const GUID* filterKey, FWPS_FILTER1* filter) {
	return STATUS_SUCCESS;
}
VOID FlowDeleteCallback(UINT16 layerid, UINT32 calloutid, UINT64 flowcontext) {

}
VOID FilterCallback(const FWPS_INCOMING_VALUES0* Values, const FWPS_INCOMING_METADATA_VALUES0* MetaData, PVOID layerdata, const void* context, const FWPS_FILTER1* filter, UINT64 flowcontext, FWPS_CLASSIFY_OUT0* classifyout) {
	KdPrint(("data is here"));
	FWPS_STREAM_CALLOUT_IO_PACKET* packet;
	FWPS_STREAM_DATA* streamdata;
	UCHAR string[200] = { 0 };
	ULONG length = 0;
	SIZE_T bytes = 0;

	packet = (FWPS_STREAM_CALLOUT_IO_PACKET*)layerdata;
	streamdata = packet->streamData;
	RtlZeroMemory(classifyout, sizeof(FWPS_CLASSIFY_OUT));

	if (streamdata->flags & FWPS_STREAM_FLAG_RECEIVE) {
		length = streamdata->dataLength <= 200 ? streamdata->dataLength : 200;
		FwpsCopyStreamDataToBuffer(streamdata, string, length, &bytes);
		KdPrint(("data is %s", string));
	}

	packet->streamAction = FWPS_STREAM_ACTION_NONE; //FWPS_STREAM_ACTION_DROP_CONNECTION
	classifyout->actionType = FWP_ACTION_PERMIT;
	if (filter->flags && FWPS_FILTER_FLAG_CLEAR_ACTION_RIGHT) {
		classifyout->rights &= ~FWPS_RIGHT_ACTION_WRITE;
	}
}
NTSTATUS WfpOpenEngine() {
	return FwpmEngineOpen(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &EngineHandle);
}
NTSTATUS WfpRegisterCallout() {
	FWPS_CALLOUT Callout = { 0 };
	Callout.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID;
	Callout.flags = 0;
	Callout.classifyFn = FilterCallback;
	Callout.notifyFn = NotifyCallback;
	Callout.flowDeleteFn = FlowDeleteCallback;
	return FwpsCalloutRegister(DeviceObject, &Callout, &RegCalloutId);
}
NTSTATUS WfpAddCallout() {
	FWPM_CALLOUT callout = { 0 };
	callout.flags = 0;
	callout.displayData.name = L"EstablishedCalloutName";
	callout.displayData.description = L"EstablishedCalloutName";
	callout.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID;
	callout.applicableLayer = FWPM_LAYER_STREAM_V4;
	return FwpmCalloutAdd(EngineHandle, &callout, NULL, &AddCalloutId);
}
NTSTATUS WfpAddSublayer() {
	FWPM_SUBLAYER sublayer = { 0 };
	sublayer.displayData.name = L"Establishedsublayername";
	sublayer.displayData.description = L"Establishedsublayername";
	sublayer.subLayerKey = WFP_SAMPLE_SUB_LAYER_GUID;
	sublayer.weight = 65500;
	return FwpmSubLayerAdd(EngineHandle, &sublayer, NULL);
}
NTSTATUS WfpAddFilter() {
	FWPM_FILTER filter = { 0 };
	FWPM_FILTER_CONDITION condition[1] = { 0 };

	filter.displayData.name = L"filterCalloutName";
	filter.displayData.description = L"filterCalloutName";
	filter.layerKey = FWPM_LAYER_STREAM_V4;
	filter.subLayerKey = WFP_SAMPLE_SUB_LAYER_GUID;
	filter.weight.type = FWP_EMPTY;
	filter.numFilterConditions = 1;
	filter.filterCondition = condition;
	filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
	filter.action.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID;

	condition[0].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;
	condition[0].matchType = FWP_MATCH_LESS_OR_EQUAL;
	condition[0].conditionValue.type = FWP_UINT16;
	condition[0].conditionValue.uint16 = 65000;

	return FwpmFilterAdd(EngineHandle, &filter, NULL, &filterid);
}
NTSTATUS InitializeWfp() {
	if (!NT_SUCCESS(WfpOpenEngine())) {
		goto end;
	}
	if (!NT_SUCCESS(WfpRegisterCallout())) {
		goto end;
	}
	if (!NT_SUCCESS(WfpAddCallout())) {
		goto end;
	}
	if (!NT_SUCCESS(WfpAddSublayer())) {
		goto end;
	}
	if (!NT_SUCCESS(WfpAddFilter())) {
		goto end;
	}
	return STATUS_SUCCESS;
end:
	UnInitWfp();
	return STATUS_UNSUCCESSFUL;
}
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	NTSTATUS status;
	DriverObject->DriverUnload = Unload;
	status = IoCreateDevice(DriverObject, 0, NULL, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	status = InitializeWfp();
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(DeviceObject);
	}
	return status;
}