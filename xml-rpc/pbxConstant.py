from enum import Enum
import os

pbx_host = os.getenv('PBX_HOST', "172.16.88.38")
pbx_username = os.getenv('PBX_USERNAME','freeswitch')
pbx_password = os.getenv('PBX_PASSWORD','works')
pbx_port = os.getenv('PBX_PORT','8080')
record_folder = os.getenv('RECORD_FOLDER','/var/lib/freeswitch/recordings/callbot/')
record_prefix = os.getenv('RECORD_PREFIX','https://14.232.240.205/file/callbot/')

class PbxHangupCause(Enum):
    NONE = 0
    UNALLOCATED_NUMBER = 1
    NO_ROUTE_TRANSIT_NET = 2
    NO_ROUTE_DESTINATION = 3
    CHANNEL_UNACCEPTABLE = 6
    CALL_AWARDED_DELIVERED = 7
    NORMAL_CLEARING = 16
    USER_BUSY = 17
    NO_USER_RESPONSE = 18
    NO_ANSWER = 19
    SUBSCRIBER_ABSENT = 20
    CALL_REJECTED = 21
    NUMBER_CHANGED = 22
    REDIRECTION_TO_NEW_DESTINATION = 23
    EXCHANGE_ROUTING_ERROR = 25
    DESTINATION_OUT_OF_ORDER = 27
    INVALID_NUMBER_FORMAT = 28
    FACILITY_REJECTED = 29
    RESPONSE_TO_STATUS_ENQUIRY = 30
    NORMAL_UNSPECIFIED = 31
    NORMAL_CIRCUIT_CONGESTION = 34
    NETWORK_OUT_OF_ORDER = 38
    NORMAL_TEMPORARY_FAILURE = 41
    SWITCH_CONGESTION = 42
    ACCESS_INFO_DISCARDED = 43
    REQUESTED_CHAN_UNAVAIL = 44
    PRE_EMPTED = 45
    FACILITY_NOT_SUBSCRIBED = 50
    OUTGOING_CALL_BARRED = 52
    INCOMING_CALL_BARRED = 54
    BEARERCAPABILITY_NOTAUTH = 57
    BEARERCAPABILITY_NOTAVAIL = 58
    SERVICE_UNAVAILABLE = 63
    BEARERCAPABILITY_NOTIMPL = 65
    CHAN_NOT_IMPLEMENTED = 66
    FACILITY_NOT_IMPLEMENTED = 69
    SERVICE_NOT_IMPLEMENTED = 79
    INVALID_CALL_REFERENCE = 81
    INCOMPATIBLE_DESTINATION = 88
    INVALID_MSG_UNSPECIFIED = 95
    MANDATORY_IE_MISSING = 96
    MESSAGE_TYPE_NONEXIST = 97
    WRONG_MESSAGE = 98
    IE_NONEXIST = 99
    INVALID_IE_CONTENTS = 100
    WRONG_CALL_STATE = 101
    RECOVERY_ON_TIMER_EXPIRE = 102
    MANDATORY_IE_LENGTH_ERROR = 103
    PROTOCOL_ERROR = 111
    INTERWORKING = 127
    SUCCESS = 142
    ORIGINATOR_CANCEL = 487
    CRASH = 700
    SYSTEM_SHUTDOWN = 701
    LOSE_RACE = 702
    MANAGER_REQUEST = 703
    BLIND_TRANSFER = 800
    ATTENDED_TRANSFER = 801
    ALLOTTED_TIMEOUT = 802
    USER_CHALLENGE = 803
    MEDIA_TIMEOUT = 804
    PICKED_OFF = 805
    USER_NOT_REGISTERED = 806
    PROGRESS_TIMEOUT = 807
    INVALID_GATEWAY = 808
    GATEWAY_DOWN = 809
    INVALID_URL = 810
    INVALID_PROFILE = 811
    NO_PICKUP = 812
    SRTP_READ_ERROR = 813
    BOWOUT = 814
    BUSY_EVERYWHERE = 815
    DECLINE = 816
    DOES_NOT_EXIST_ANYWHERE = 817
    NOT_ACCEPTABLE = 818
    UNWANTED = 819
    NO_IDENTITY = 820
    BAD_IDENTITY_INFO = 821
    UNSUPPORTED_CERTIFICATE = 822
    INVALID_IDENTITY = 823
    STALE_DATE = 824


CALLBOT_STATUS = {}