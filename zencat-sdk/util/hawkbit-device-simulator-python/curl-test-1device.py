import sys
import platform
from jproperties import Properties
import http.client
import base64
import json
import re

os_name = platform.system()

if os_name == "Windows":
    print("OS: Windows")
else:
    sys.exit(1)

# Load config
with open("config.properties", "r+b") as f:
    p = Properties()
    p.load(f, "utf-8")

    hawkbit_domain = p.properties.get("hawkbit.domain")
    if hawkbit_domain.startswith("https://"):
        hawkbit_domain = hawkbit_domain[len("https://"):]
    hawkbit_username = p.properties.get("hawkbit.username")
    hawkbit_password = p.properties.get("hawkbit.password")
    hawkbit_gatewaysecuritytoken = p.properties.get("hawkbit.gateway-security-token")
    global hawkbit_devicetargetid 
    hawkbit_devicetargetid = p.properties.get("hawkbit.device-target-id")
    hawkbit_devicesecuritytoken = p.properties.get("hawkbit.device-security-token")


class ReqMethod:
    GET = "GET"
    PUT = "PUT"
    POST = "POST"


def fetch(conn, method, url, body=None, headers=None, no_rep=False):
    parsed_json = None
    try:
        conn.request(method, url, body=body, headers=headers)
        response = conn.getresponse()
        data = response.read()
        if data == b'' and response.code == 200:
            json_data = data.decode('utf-8')
            parsed_json = json.loads("{}")    
        else:
            json_data = data.decode('utf-8')
            parsed_json = json.loads(json_data)
    except Exception as e:
        msg = f"An error occurred: {e}"
        print(msg)
    finally:
        conn.close()
    
    return parsed_json


def print_fjson(parsed_json):
    formatted_json = json.dumps(parsed_json, indent=4)
    print(formatted_json)


def get_next_devicetargetid(input_string):
    match = re.search(r'(\D*)(\d+)', input_string)
    if match:
        prefix = match.group(1)
        nd = int(match.group(2)) + 1
        formatted_number = f"{nd:02}"
        return prefix + formatted_number
    return input_string
    # match = re.search(r'\d+', input_string)
    # nd = 0
    # if match:
    #     nd = int(match.group(0))
    # nd += 1
    # formatted_number = f"{nd:02}"
    # return formatted_number

def get_num_devicetargetid(input_string):
    match = re.search(r'\d+', input_string)
    nd = 0
    if match:
        nd = int(match.group(0))    
    return nd


def test_single_device():
    print(hawkbit_domain)
    print(hawkbit_username)
    print(hawkbit_password)
    print(hawkbit_devicetargetid)
    auth = base64.b64encode(f"{hawkbit_username}:{hawkbit_password}".encode()).decode()
    conn = http.client.HTTPSConnection(hawkbit_domain)
    url = "/rest/v1/system/configs/authentication.targettoken.enabled/"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Basic {auth}"
    }

    parsed_json = fetch(conn, ReqMethod.GET, url, headers=headers)
    print_fjson(parsed_json)
    value = parsed_json.get("value")
    if value != True:
        url = "/rest/v1/system/configs/authentication.targettoken.enabled/"
        body = json.dumps({"value": True})
        parsed_json = fetch(conn, ReqMethod.PUT, url, body=body, headers=headers)
        print_fjson(parsed_json)

    url = "/rest/v1/system/configs/authentication.gatewaytoken.enabled/"
    parsed_json = fetch(conn, ReqMethod.GET, url, headers=headers)
    print_fjson(parsed_json)
    value = parsed_json.get("value")
    print(f"value={value}")
    if value != True:
        url = "/rest/v1/system/configs/authentication.gatewaytoken.enabled/"
        body = json.dumps({"value": True})
        parsed_json = fetch(conn, ReqMethod.PUT, url, body=body, headers=headers)
        print_fjson(parsed_json)

    url = "/rest/v1/system/configs/authentication.gatewaytoken.key/"
    body = json.dumps({"value": hawkbit_gatewaysecuritytoken})
    parsed_json = fetch(conn, ReqMethod.PUT, url, body=body, headers=headers)
    print_fjson(parsed_json)

    devicetargetid = hawkbit_devicetargetid
    devicetargetid_found = False

    while devicetargetid_found == False:
        url = f"/rest/v1/targets/{devicetargetid}"
        parsed_json = fetch(conn, ReqMethod.GET, url, headers=headers)
        print_fjson(parsed_json)
        if "errorCode" in parsed_json:
            error_code = parsed_json.get("errorCode")
            print(f"errorCode={error_code}")
            if "entitiyNotFound" in error_code:
                devicetargetid_found = True
            else:
                sys.exit(1)
        else:               
            devicetargetid = get_next_devicetargetid(devicetargetid)
    
    num_devicetargetid = get_num_devicetargetid(devicetargetid)

    url = "/rest/v1/targets"
    body = json.dumps([{"controllerId": devicetargetid,
                       "name": f"EDevice {num_devicetargetid}",
                       "description": f"Emu device {num_devicetargetid}",
                       "securityToken": hawkbit_devicesecuritytoken}])
    parsed_json = fetch(conn, ReqMethod.POST, url, body=body, headers=headers)
    print_fjson(parsed_json)

    url = f"/DEFAULT/controller/v1/{devicetargetid}"
    headers = {
        "Authorization": f"TargetToken {hawkbit_devicesecuritytoken}"
    }
    parsed_json = fetch(conn, ReqMethod.GET, url, headers=headers)
    print_fjson(parsed_json)

    url = f"/DEFAULT/controller/v1/{devicetargetid}/configData"
    headers = {
        "Authorization": f"TargetToken {hawkbit_devicesecuritytoken}",
        "Content-Type": "application/json"
    }
    body = json.dumps({"mode": "merge",
                       "data": {"PN": "BD01", "HWRev": "1.0"}})
    parsed_json = fetch(conn, ReqMethod.PUT, url, body=body, headers=headers)
    print_fjson(parsed_json)
    

if __name__ == "__main__":
    test_single_device()