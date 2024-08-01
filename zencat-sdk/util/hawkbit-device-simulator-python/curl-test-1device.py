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


class ReqMethod:
    GET = "GET"
    PUT = "PUT"


def fetch(conn, method, url, body=None, headers=None):
    parsed_json = None
    try:
        conn.request(method, url, body=body, headers=headers)
        response = conn.getresponse()
        data = response.read()
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


if __name__ == "__main__":
    test_single_device()