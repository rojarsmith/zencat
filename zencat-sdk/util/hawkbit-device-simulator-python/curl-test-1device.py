import sys
import platform
from jproperties import Properties
import http.client
import base64
import json

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

def test_single_device():
    print(hawkbit_domain)
    print(hawkbit_username)
    print(hawkbit_password)
    auth = base64.b64encode(f"{hawkbit_username}:{hawkbit_password}".encode()).decode()
    conn = http.client.HTTPSConnection(hawkbit_domain)
    url = "/rest/v1/system/configs/authentication.targettoken.enabled/"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Basic {auth}"
    }

    try:
        conn.request("GET", url, headers=headers)
        response = conn.getresponse()
        data = response.read()
        print(data)
        json_data = data.decode('utf-8')
        parsed_json = json.loads(json_data)
        formatted_json = json.dumps(parsed_json, indent=4)
        print(formatted_json)
        value = parsed_json.get("value")

        if value != True:
            url = "/rest/v1/system/configs/authentication.targettoken.enabled/"
            data = json.dumps({"value": True})
            conn.request("PUT", url, body=data, headers=headers)
            response = conn.getresponse()
            data = response.read()
            json_data = data.decode('utf-8')
            parsed_json = json.loads(json_data)
            formatted_json = json.dumps(parsed_json, indent=4)
            print(formatted_json)

        url = "/rest/v1/system/configs/authentication.gatewaytoken.enabled/"
        conn.request("GET", url, headers=headers)
        response = conn.getresponse()
        data = response.read()
        print(data)
        json_data = data.decode('utf-8')
        parsed_json = json.loads(json_data)
        formatted_json = json.dumps(parsed_json, indent=4)
        print(formatted_json)
        value = parsed_json.get("value")

    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        conn.close()

if __name__ == "__main__":
    test_single_device()